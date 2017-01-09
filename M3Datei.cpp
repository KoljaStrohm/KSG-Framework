#include "M3Datei.h"
#include "Datei.h"

using namespace Framework;

// Inhalt der M3Datei Klasse
// Konstruktor
M3Datei::M3Datei()
{
    modelName = 0;
    modelPos = 0;
}

// Konstruktor
//  pfad: Der Pfad zur Datei
M3Datei::M3Datei( const char *pfad )
    : M3Datei()
{
    this->pfad = pfad;
}

// Konstruktor
//  pfad: Der Pfad zur Datei
M3Datei::M3Datei( Text *pfad )
    : M3Datei( pfad->getText() )
{
    pfad->release();
}

// Destruktor
M3Datei::~M3Datei()
{
    if( modelName )
        modelName->release();
    if( modelPos )
        modelPos->release();
}

// Setzt den Pfad zur Datei

//  pfad: Pfad zur Datei
void M3Datei::setPfad( const char *pfad )
{
    this->pfad = pfad;
    if( modelName )
        modelName = modelName->release();
    if( modelPos )
        modelPos = modelPos->release();
}

// Ließt grundlegende Informationen aus der Datei, die für ihre Verwendung benötigt werden
void M3Datei::leseDaten()
{
    if( modelName )
        modelName = modelName->release();
    if( modelPos )
        modelPos = modelPos->release();
    modelName = new RCArray< Text >();
    modelPos = new Array< __int64 >();
    Datei d;
    d.setDatei( pfad );
    if( !d.open( Datei::Style::lesen ) )
        return;
    unsigned char anz = 0;
    d.lese( (char*)&anz, 1 );
    for( int i = 0; i < anz; i++ )
    {
        char len = 0;
        d.lese( &len, 1 );
        char *n = new char[ len + 1 ];
        n[ (int)len ] = 0;
        d.lese( n, len );
        modelName->add( new Text( n ) );
        delete[] n;
        __int64 p = 0;
        d.lese( (char*)&p, 8 );
        modelPos->add( p );
    }
    d.close();
}

// Speichert 3D Modell Daten in der Datei
//  zMdr: Ein Zeiger auf die zu speichernden Daten ohne erhöhtem Reference Counter
//  name: Der Name, unter dem die Daten in der Datei gespeichert werden sollen
//  return: 1, falls das Modell gespeichert wurde. 0, falls ein fehler beim speichern auftrat
bool M3Datei::saveModel( Model3DData *zMdr, Text *name )
{
    bool ret = saveModel( zMdr, name->getText() );
    name->release();
    return ret;
}

// Speichert 3D Modell Daten in der Datei
//  zMdr: Ein Zeiger auf die zu speichernden Daten ohne erhöhtem Reference Counter
//  name: Der Name, unter dem die Daten in der Datei gespeichert werden sollen
//  return: 1, falls das Modell gespeichert wurde. 0, falls ein fehler beim speichern auftrat
bool M3Datei::saveModel( Model3DData *zMdr, const char *name )
{
    if( !modelName || !pfad.getLength() )
        return 0;
    if( hatModel( name ) && !removeModel( name ) )
        return 0;
    int anz = modelName->getEintragAnzahl();
    anz = modelName->getEintragAnzahl();
    Datei d;
    d.setDatei( pfad );
    if( !d.open( Datei::Style::lesen ) )
        return 0;
    Datei neu;
    neu.setDatei( pfad );
    neu.zPfad()->append( "0" );
    while( neu.existiert() )
        neu.zPfad()->append( "0" );
    if( !neu.open( Datei::Style::schreiben ) )
    {
        if( d.istOffen() )
            d.close();
        return 0;
    }
    modelName->add( new Text( name ) );
    int offs = textLength( name ) + 9;
    for( int i = 0; i < anz; i++ )
        modelPos->set( modelPos->get( i ) + offs, i );
    if( d.getSize() < 0 )
        modelPos->add( offs + 1 );
    else
        modelPos->add( d.getSize() + offs );
    anz++;
    char tmp = (char)anz;
    neu.schreibe( &tmp, 1 );
    for( int i = 0; i < anz; i++ )
    {
        char len = (char)modelName->z( i )->getLength();
        neu.schreibe( &len, 1 );
        neu.schreibe( modelName->z( i )->getText(), len );
        __int64 pos = modelPos->get( i );
        neu.schreibe( (char*)&pos, 8 );
    }
    if( d.existiert() )
    {
        d.setLPosition( modelPos->get( 0 ) - offs, 0 );
        __int64 dl = d.getSize() - d.getLPosition();
        char bytes[ 2048 ];
        while( dl )
        {
            int l = dl > 2048 ? 2048 : (int)dl;
            d.lese( bytes, l );
            neu.schreibe( bytes, l );
            dl -= l;
        }
    }
    d.close();
    /*char pAnz = zMdr->polygons->getEintragAnzahl();
    neu.schreibe( &pAnz, 1 );
    for( int p = 0; p < pAnz; p++ )
    {
        int vAnz = zMdr->polygons->get( p ).vertex->getEintragAnzahl();
        char textur = 1;
        for( int i = 0; i < vAnz; i++ )
            textur &= (char)zMdr->polygons->get( p ).tKordinaten->hat( i );
        neu.schreibe( &textur, 1 );
        neu.schreibe( (char*)&vAnz, 4 );
        for( int i = 0; i < vAnz; i++ )
        {
            float v = zMdr->polygons->get( p ).vertex->get( i ).x;
            neu.schreibe( (char*)&v, 4 );
            v = zMdr->polygons->get( p ).vertex->get( i ).y;
            neu.schreibe( (char*)&v, 4 );
            if( textur )
            {
                int t = zMdr->polygons->get( p ).tKordinaten->get( i ).x;
                neu.schreibe( (char*)&t, 4 );
                t = zMdr->polygons->get( p ).tKordinaten->get( i ).y;
                neu.schreibe( (char*)&t, 4 );
            }
        }
    }*/
    d.remove();
    neu.close();
    neu.umbenennen( pfad );
    leseDaten();
    return 1;
}

// Löscht ein 3D Modell aus der Datei
//  name: Der Name des Modells
//  return: 1, wenn das Modell gelöscht wurde. 0, wenn das Modell nicht gefunden wurde, oder ein fehler beim speichern auftrat
bool M3Datei::removeModel( Text *name )
{
    return 0;
}

// Löscht ein 3D Modell aus der Datei
//  name: Der Name des Modells
//  return: 1, wenn das Modell gelöscht wurde. 0, wenn das Modell nicht gefunden wurde, oder ein fehler beim speichern auftrat
bool M3Datei::removeModel( const char *name )
{
    return 0;
}

// Lähd ein 3D Modell aus der Datei
//  name: Der name des zu ladenden Modells
//  return: Die geladenen Daten
Model3DData *M3Datei::ladeModel( Text *name ) const
{
    return 0;
}

// Lähd ein 3D Modell aus der Datei
//  name: Der name des zu ladenden Modells
//  return: Die geladenen Daten
Model3DData *M3Datei::ladeModel( const char *name ) const
{
    return 0;
}

// überprft, ob ein bestimmtes 3D Modell in der Datei existiert
//  name: Der Name des zu suchenden 3D Modells
//  return: 1, wenn das Modell gefunden wurde. 0 sonst
bool M3Datei::hatModel( const char *name ) const
{
    return 0;
}

// ügibt die Anzahl der gespeicherten Modelle zurück
int M3Datei::getModelAnzahl() const
{
    return 0;
}

// Gibt den Namen eines Bestimmten Modells zurück
//  i: Der Index des Modells
//  return: Ein Zeiger aud den Namen des Modells ohne erhöhten Reference Counter
Text *M3Datei::zModelName( int i ) const
{
    return 0;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
M3Datei *M3Datei::getThis()
{
    return 0;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
M3Datei *M3Datei::release()
{
    return 0;
}