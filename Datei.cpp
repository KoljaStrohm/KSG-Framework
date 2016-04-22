#include "Datei.h"
#include "Text.h"
#include "Zeit.h"
#ifdef WIN32
#include <direct.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#endif

using namespace Framework;

// Inhalt der Datei Klasse aus Datei.h
// Konstruktor 
Datei::Datei()
    : ref( 1 ),
    stream( 0 ),
    pfad( 0 ),
    gr( 0 ),
    tmpLByte( 0 ),
    tmpLBPos( 7 ),
    tmpSByte( 0 ),
    tmpSBPos( -1 )
{}

// Destruktor 
Datei::~Datei()
{
    if( stream )
        delete stream;
    if( pfad )
        pfad->release();
}

// nicht constant 
void Datei::setDatei( const char *pfad ) // setzt die Datei
{
    if( istOffen() )
        schließen();
    if( !this->pfad )
        this->pfad = new Text();
    this->pfad->setText( pfad );
    gr = 0;
}

void Datei::setDatei( Text *pfad )
{
    if( istOffen() )
        schließen();
    if( !this->pfad )
        this->pfad = new Text();
    this->pfad->setText( pfad );
    gr = 0;
}

bool Datei::umbenennen( const char *pfad ) // benennt die Datei um und verschiebt sie eventuell
{
    if( !pfad )
        return 0;
    if( DateiUmbenennen( this->pfad->getText(), pfad ) )
    {
        this->pfad->setText( pfad );
        return 1;
    }
    return 0;
}

bool Datei::umbenennen( Text *pfad )
{
    if( !this->pfad )
    {
        pfad->release();
        return 0;
    }
    if( DateiUmbenennen( this->pfad->getText(), pfad->getText() ) )
    {
        this->pfad->setText( pfad );
        return 1;
    }
    pfad->release();
    return 0;
}

bool Datei::löschen() // löscht die Datei
{
    if( !pfad )
        return 0;
    return DateiLöschen( pfad->getThis() );
}

bool Datei::erstellen() // erstellt die Datei
{
    if( !pfad )
        return 0;
    return DateiPfadErstellen( pfad->getThis() );
}

bool Datei::öffnen( int style ) // öffnet die Datei
{
    if( !pfad )
        return 0;
    if( stream )
        delete stream;
    stream = new std::fstream();
    std::ios_base::openmode om = std::ios::binary;
    if( ( style | Style::lesen ) == style )
        om |= std::ios::in;
    if( ( style | Style::schreiben ) == style )
        om |= std::ios::out;
    stream->open( pfad->getText(), om );
    if( ( style | Style::ende ) == style )
    {
        if( ( style | Style::lesen ) == style )
            stream->seekg( 0, std::ios::end );
        if( ( style | Style::schreiben ) == style )
            stream->seekp( 0, std::ios::end );
    }
    if( !stream->is_open() || !stream->good() )
    {
        delete stream;
        stream = 0;
        return 0;
    }
    tmpLBPos = 7;
    tmpSBPos = -1;
    return 1;
}

void Datei::setLPosition( __int64 pos, bool ende ) // setzt die Leseposition
{
    if( !pfad )
        return;
    if( stream )
    {
        if( ende )
            stream->seekg( pos, std::ios::end );
        else
            stream->seekg( pos, std::ios::beg );
    }
    tmpLBPos = 7;
}

void Datei::setSPosition( __int64 pos, bool ende ) // setzt die Schreibeposition
{
    if( !pfad )
        return;
    if( stream )
    {
        if( ende )
            stream->seekp( pos, std::ios::end );
        else
            stream->seekp( pos, std::ios::beg );
    }
    tmpSBPos = -1;
}

void Datei::schreibe( char *bytes, int län ) // schreibt bytes in datei
{
    if( !pfad || !stream )
        return;
    if( tmpSBPos >= 0 )
    {
        tmpSBPos = -1;
        stream->write( &tmpSByte, 1 );
        tmpSByte = 0;
    }
    stream->write( bytes, län );
}

void Datei::lese( char *bytes, int län ) // ließt bytes aus datei
{
    if( !pfad )
        return;
    if( stream )
        stream->read( bytes, län );
    tmpLBPos = 7;
    tmpSBPos = -1;
}

Text *Datei::leseZeile() // ließt eine zeile
{
    if( !pfad | !stream )
        return 0;
    if( istEnde() )
        return 0;
    Text *ret = new Text( "" );
    __int64 län = getGröße();
    for( char c = 0; c != '\n' && stream->tellg() < län; )
    {
        stream->read( &c, 1 );
        if( c )
            ret->anhängen( (const char*)&c, 1 );
    }
    tmpSBPos = 7;
    tmpSBPos = -1;
    return ret;
}

void Datei::schließen() // schließt die Datei
{
    if( !pfad || !stream )
        return;
    if( tmpSBPos >= 0 )
        stream->write( &tmpSByte, 1 );
    stream->close();
    delete stream;
    stream = 0;
}

#ifdef WIN32
bool Datei::setLetzteÄnderung( Zeit *zeit ) // setzt das änderungsdatum der Datei
{
    if( !pfad )
    {
        zeit->release();
        return 0;
    }
    HANDLE hFile = CreateFile( pfad->getText(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile == INVALID_HANDLE_VALUE )
    {
        zeit->release();
        return 0;
    }
    FILETIME ftCreate, ftAccess, ftWrite;
    if( !GetFileTime( hFile, &ftCreate, &ftAccess, &ftWrite ) )
    {
        CloseHandle( hFile );
        zeit->release();
        return 0;
    }
    SYSTEMTIME stUTC, stLocal;
    stLocal.wMilliseconds = 0;
    stLocal.wSecond = zeit->zUhrzeit()->getSekunde();
    stLocal.wMinute = zeit->zUhrzeit()->getMinute();
    stLocal.wHour = zeit->zUhrzeit()->getStunde();
    stLocal.wDay = zeit->zDatum()->getTag();
    stLocal.wMonth = zeit->zDatum()->getMonat();
    stLocal.wYear = zeit->zDatum()->getJahr();
    zeit->release();
    if( !TzSpecificLocalTimeToSystemTime( NULL, &stLocal, &stUTC ) )
    {
        CloseHandle( hFile );
        return 0;
    }
    if( !SystemTimeToFileTime( &stUTC, &ftWrite ) )
    {
        CloseHandle( hFile );
        return 0;
    }
    if( !SetFileTime( hFile, &ftCreate, &ftAccess, &ftWrite ) )
    {
        CloseHandle( hFile );
        return 0;
    }
    CloseHandle( hFile );
    return 1;
}
#endif

bool Datei::getNextBit( bool &bit ) // Datei Bitweise auslesen
{
    if( !pfad || !stream )
        return 0;
    if( tmpLBPos == 7 )
    {
        tmpLBPos = -1;
        stream->read( &tmpLByte, 1 );
    }
    tmpLBPos++;
    bit = ( tmpLByte >> ( 7 - tmpLBPos ) ) & 1;
    return 1;
}

bool Datei::setNextBit( bool bit ) // Datei Bitweise speichern
{
    if( !pfad || !stream )
        return 0;
    tmpSBPos++;
    tmpSByte |= ( (char)bit << ( 7 - tmpSBPos ) ) & ( 1 << ( 7 - tmpSBPos ) );
    if( tmpSBPos == 7 )
    {
        tmpSBPos = -1;
        stream->write( &tmpSByte, 1 );
        tmpSByte = 0;
    }
    return 1;
}

// constant 
bool Datei::istOrdner() const // prüft, ob die Datei ein Ordner ist
{
    if( !pfad )
        return 0;
    return DateiIstVerzeichnis( pfad->getThis() );
}

bool Datei::istOffen() const // prüft, ob die Datei geöffnet ist
{
    if( !pfad )
        return 0;
    if( stream )
        return stream->is_open() && stream->good();
    return 0;
}

int Datei::getUnterdateiAnzahl() const // gibt die Anzahl der unterdateien an
{
#ifdef WIN32
    if( !pfad )
        return 0;
    if( !DateiIstVerzeichnis( pfad->getThis() ) )
        return 0;
    int ret = 0;
    HANDLE fHandle;
    WIN32_FIND_DATA wfd;
    Text stxt = pfad->getText();
    stxt.ersetzen( '/', '\\' );
    if( stxt.positionVon( '\\' ) == stxt.getLänge() - 1 )
        stxt.anhängen( "*" );
    else
        stxt.anhängen( "\\*" );
    fHandle = FindFirstFile( stxt.getText(), &wfd );
    FindNextFile( fHandle, &wfd );
    while( FindNextFile( fHandle, &wfd ) )
        ++ret;
    FindClose( fHandle );
    return ret;
#else
    if( !pfad )
        return 0;
    if( !DateiIstVerzeichnis( pfad->getThis() ) )
        return 0;
    int ret = 0;
    Text stxt = pfad->getText();
    stxt.ersetzen( '\\', '/' );
    if( stxt.positionVon( '/' ) == stxt.getLaenge() - 1 )
        stxt.loeschen( stxt.getLaenge() - 1 );
    DIR *hdir;
    hdir = opendir( stxt.getText() );
    for( dirent *entry = readdir( hdir ); entry; entry = readdir( hdir ) )
    {
        if( entry && entry->d_name[ 0 ] != '.' )
            ++ret;
    }
    closedir( hdir );
    return ret;
#endif
}

RCArray< Text > *Datei::getDateiListe() const // gibt eine Liste mit unterdateien zurück
{
#ifdef WIN32
    if( !pfad )
        return 0;
    if( !DateiIstVerzeichnis( pfad->getThis() ) )
        return 0;
    HANDLE fHandle;
    WIN32_FIND_DATA wfd;
    Text stxt = pfad->getText();
    stxt.ersetzen( '/', '\\' );
    if( stxt.positionVon( '\\' ) == stxt.getLänge() - 1 )
        stxt.anhängen( "*" );
    else
        stxt.anhängen( "\\*" );
    fHandle = FindFirstFile( stxt.getText(), &wfd );
    FindNextFile( fHandle, &wfd );
    RCArray< Text > *ret = new RCArray< Text >();
    int count = 0;
    while( FindNextFile( fHandle, &wfd ) )
    {
        Text *txt = new Text( wfd.cFileName );
        ret->add( txt, count );
        ++count;
    }
    FindClose( fHandle );
    return ret;
#else
    if( !pfad )
        return 0;
    if( !DateiIstVerzeichnis( pfad->getThis() ) )
        return 0;
    Text stxt = pfad->getText();
    stxt.ersetzen( '\\', '/' );
    if( stxt.positionVon( '/' ) == stxt.getLaenge() - 1 )
        stxt.loeschen( stxt.getLaenge() - 1 );
    DIR *hdir;
    hdir = opendir( stxt.getText() );
    if( hdir )
    {
        RCArray< Text > *ret = new RCArray< Text >();
        int count = 0;
        for( dirent *entry = readdir( hdir ); entry; entry = readdir( hdir ) )
        {
            if( entry && entry->d_name[ 0 ] != '.' )
            {
                ret->add( new Text( entry->d_name ), count );
                ++count;
            }
        }
        closedir( hdir );
        return ret;
    }
    return 0;
#endif
}

__int64 Datei::getGröße() const // gibt die Größe der Datei zurück
{
    if( !pfad )
        return 0;
    if( gr )
        return gr;
    if( !stream || !istOffen() )
    {
        std::fstream *stream = new std::fstream();
        stream->open( pfad->getText(), std::ios::binary | std::ios::in );
        __int64 tmp = stream->tellg();
        stream->seekg( 0, std::ios::end );
        __int64 ret = stream->tellg();
        stream->seekg( tmp, std::ios::beg );
        stream->close();
        delete stream;
        __int64 *größe = (__int64*)&gr;
        *größe = ret;
        return ret;
    }
    __int64 tmp = stream->tellg();
    stream->seekg( 0, std::ios::end );
    __int64 ret = stream->tellg();
    stream->seekg( tmp, std::ios::beg );
    __int64 *größe = (__int64*)&gr;
    *größe = ret;
    return ret;
}

Zeit *Datei::getLetzteÄnderung() const // gibt das Datum der letzten Änderung
{
    if( !pfad )
        return 0;
#ifdef WIN32
    HANDLE hFile = CreateFile( pfad->getText(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile == INVALID_HANDLE_VALUE )
        return 0;
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    if( !GetFileTime( hFile, &ftCreate, &ftAccess, &ftWrite ) )
    {
        CloseHandle( hFile );
        return 0;
    }
    CloseHandle( hFile );
    if( !FileTimeToSystemTime( &ftWrite, &stUTC ) )
        return 0;
    if( !SystemTimeToTzSpecificLocalTime( NULL, &stUTC, &stLocal ) )
        return 0;
    Zeit *ret = new Zeit();
    ret->setZeit( stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond );
    return ret;
#else
    struct stat attrib;
    stat( pfad->getText(), &attrib );
    tm *clock = gmtime( &( attrib.st_mtime ) );
    Zeit *ret = new Zeit();
    ret->setZeit( clock->tm_year + 1900, clock->tm_mon + 1, clock->tm_mday, clock->tm_hour, clock->tm_min, clock->tm_sec );
    return ret;
#endif
}

bool Datei::existiert() const // prüft, ob die Datei existiert
{
    if( !pfad )
        return 0;
    return DateiExistiert( pfad->getThis() );
}

__int64 Datei::getLPosition() const // gibt die Leseposition zurück
{
    if( !stream )
        return 0;
    return stream->tellg();
}

__int64 Datei::getSPosition() const // gibt die Schreibeposition zurück
{
    if( !stream )
        return 0;
    return stream->tellp();
}

bool Datei::istEnde() const // prüft, ob die Datei zu ende ist
{
    if( !stream || stream->tellg() < 0 )
        return 1;
    __int64 i = getGröße();
    return stream->tellg() >= i;
}

Text *Datei::getPfad() const // gibt den Dateipfad zurück
{
    return pfad ? pfad->getThis() : 0;
}

Text *Datei::zPfad() const
{
    return pfad;
}

// Reference Counting 
Datei *Datei::getThis()
{
    ++ref;
    return this;
}

Datei *Datei::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Datei Funktionen
void Framework::GetFreePfad( Text *zPfad ) // Sucht einen unbenutzten Dateinamen
{
    Text txt = zPfad->getText();
    for( int i = 0; DateiExistiert( txt ); i++ )
    {
        txt = zPfad->getText();
        txt.anhängen( i );
    }
    zPfad->setText( txt );
}

bool Framework::DateiPfadErstellen( Text *pfad ) // Erstellt eine Datei in dem Pfad
{
    bool ret = DateiPfadErstellen( pfad->getText() );
    pfad->release();
    return ret;
}

bool Framework::DateiLöschen( Text *pfad ) // Löscht die angegebene Datei
{
    bool ret = DateiLöschen( pfad->getText() );
    pfad->release();
    return ret;
}

bool Framework::DateiUmbenennen( Text *pfad_alt, Text *pfad_neu ) // Benennt die Datei um
{
    bool ret = DateiUmbenennen( pfad_alt->getText(), pfad_neu->getText() );
    pfad_alt->release();
    pfad_neu->release();
    return ret;
}

bool Framework::DateiExistiert( Text *pfad ) // Prüft, ob Datei existiert
{
    bool ret = DateiExistiert( pfad->getText() );
    pfad->release();
    return ret;
}

bool Framework::DateiIstVerzeichnis( Text *pfad ) // prüft, ob pfad ein Verzeichnis ist
{
    bool ret = DateiIstVerzeichnis( pfad->getText() );
    pfad->release();
    return ret;
}

bool Framework::DateiPfadErstellen( const char *pfad ) // Erstellt eine Datei in dem Pfad
{
    Text pf = pfad;
    bool erst = 1;
#ifdef WIN32
    pf.ersetzen( "//", "\\" ); // Pfadangaben korrigieren
    pf.ersetzen( "/", "\\" );
    for( int i = 0; i < pf.anzahlVon( "\\" ); ++i ) // Jeden ordner erstellen wenn er nicht existiert
    {
        Text *t = pf.getTeilText( 0, pf.positionVon( "\\", i ) );
        if( !t || !t->getLänge() )
        {
            if( t )
                t->release();
            continue;
        }
        if( !DateiExistiert( t->getThis() ) )
#pragma warning(suppress: 6031)
            _mkdir( t->getText() );
        t->release();
        if( pf.positionVon( "\\", i ) == pf.getLänge() - 1 )
            erst = 0;
    }
#else
    pf.ersetzen( "\\", "/" ); // Pfadangaben korrigieren
    for( int i = 0; i < pf.anzahlVon( "/" ); ++i ) // Jeden ordner erstellen wenn er nicht existiert
    {
        Text *t = pf.getTeilText( 0, pf.positionVon( "/", i ) );
        if( !t || !t->getLänge() )
        {
            if( t )
                t->release();
            continue;
        }
        if( !DateiExistiert( t->getThis() ) )
            mkdir( t->getText(), 0777 );
        t->release();
        if( pf.positionVon( "\\", i ) == pf.getLänge() - 1 )
            erst = 0;
    }
#endif
    if( erst )
    {
        std::ofstream f( pf, std::ios::binary ); // Datei erstellen
        f.close();
    }
    return DateiExistiert( pf );
}

bool Framework::DateiLöschen( const char *pfad ) // Löscht die angegebene Datei
{
    Text pfa = pfad;
#ifdef WIN32
    pfa.ersetzen( '\\', '/' );
    bool ret = 0;
    // prüfen ob Datei existiert
    if( !DateiIstVerzeichnis( pfa.getThis() ) )
        ret = DeleteFile( pfa.getText() ) == 1; // Datei löschen
    else
    {
        ret = 1;
        Datei *dat = new Datei();
        dat->setDatei( pfa.getThis() );
        int anz = dat->getUnterdateiAnzahl();
        RCArray< Text > *liste = dat->getDateiListe();
        for( int i = 0; i < anz; ++i )
        {
            Text *pf = new Text( pfa.getText() );
            if( pf->getText()[ pf->getLänge() - 1 ] != '/' )
                pf->anhängen( "/" );
            pf->anhängen( liste->get( i ) );
            if( ret )
                ret = DateiLöschen( pf );
            else
                DateiLöschen( pf );
        }
        liste->release();
        dat->release();
        if( ret )
            ret = RemoveDirectory( pfa.getText() ) == 1;
        else
            RemoveDirectory( pfa.getText() );
    }
    return ret;
#else
    pfa.ersetzen( '\\', '/' );
    bool ret = 0;
    // pruefen ob Datei existiert
    if( !DateiIstVerzeichnis( pfa.getThis() ) )
        ret = std::remove( pfa.getText() ) == 0; // Datei loeschen
    else
    {
        ret = 1;
        Datei *dat = new Datei();
        dat->setDatei( pfa.getThis() );
        int anz = dat->getUnterdateiAnzahl();
        RCArray< Text > *liste = dat->getDateiListe();
        for( int i = 0; i < anz; ++i )
        {
            Text *pf = new Text( pfa.getText() );
            if( pf->getText()[ pf->getLaenge() - 1 ] != '/' )
                pf->anhaengen( "/" );
            pf->anhaengen( liste->get( i ) );
            if( ret )
                ret = DateiLoeschen( pf );
            else
                DateiLoeschen( pf );
        }
        liste->release();
        dat->release();
        if( ret )
            ret = std::remove( pfa.getText() ) == 0;
        else
            std::remove( pfa.getText() );
    }
    return ret;
#endif
}

bool Framework::DateiUmbenennen( const char *pfad_alt, const char *pfad_neu ) // Benennt die Datei um
{
#ifdef WIN32
    if( pfad_alt && pfad_neu && DateiExistiert( pfad_alt ) )
    {
        bool ret = 1;
        if( DateiIstVerzeichnis( pfad_alt ) )
        {
            if( !DateiExistiert( pfad_neu ) )
            {
                Text tmp = pfad_neu;
                tmp += "/a";
                DateiPfadErstellen( tmp );
                DateiLöschen( tmp );
            }
            Datei d;
            d.setDatei( pfad_alt );
            RCArray< Text > *list = d.getDateiListe();
            int anz = list->getEintragAnzahl();
            for( int i = 0; i < anz; i++ )
            {
                Text pf = pfad_neu;
                pf += "/";
                pf += list->z( i )->getText();
                Text pf_a = pfad_alt;
                pf_a += "/";
                pf_a += list->z( i )->getText();
                ret |= DateiUmbenennen( pf_a, pf );
            }
            d.löschen();
        }
        else
        {
            if( DateiExistiert( pfad_neu ) )
                return 0;
        }
        ret |= MoveFile( pfad_alt, pfad_neu ) == 1; // Datei umbenennen
        return ret;
    }
    return 0;
#else
    if( pfad_alt && pfad_neu && DateiExistiert( pfad_alt ) )
    {
        bool ret = 1;
        if( DateiIstVerzeichnis( pfad_alt ) )
        {
            if( !DateiExistiert( pfad_neu ) )
            {
                Text tmp = pfad_neu;
                tmp += "/a";
                DateiPfadErstellen( tmp );
                DateiLöschen( tmp );
            }
            Datei d;
            d.setDatei( pfad_alt );
            RCArray< Text > *list = d.getDateiListe();
            int anz = list->getEintragAnzahl();
            for( int i = 0; i < anz; i++ )
            {
                Text pf = pfad_neu;
                pf += "/";
                pf += list->z( i )->getText();
                Text pf_a = pfad_alt;
                pf_a += "/";
                pf_a += list->z( i )->getText();
                ret |= DateiUmbenennen( pf_a, pf );
            }
            d.löschen();
        }
        else
        {
            if( DateiExistiert( pfad_neu ) )
                return 0;
        }
        ret |= rename( pfad_alt, pfad_neu ) == 1; // Datei umbenennen
        return ret;
    }
    return 0;
#endif
}

bool Framework::DateiExistiert( const char *pfad ) // Prüft, ob Datei existiert
{
#ifdef WIN32
    bool ret = PathFileExists( pfad ) != 0;
    return ret;
#else
    std::ifstream file( pfad );
    if( file.good() )
        return 1;
    return 0;
#endif
}

bool Framework::DateiIstVerzeichnis( const char *pfad ) // prüft, ob pfad ein Verzeichnis ist
{
#ifdef WIN32
    WIN32_FIND_DATA wfd;
    HANDLE handle = FindFirstFile( pfad, &wfd );
    if( handle == INVALID_HANDLE_VALUE )
        return 0;
    FindClose( handle );
    return ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
    std::ifstream file( pfad );
    if( file.good() )
    {
        std::ifstream file2( pfad, std::ios::out );
        if( !file2.good() )
            return 1;
    }
    return 0;
#endif
}