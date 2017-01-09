#include "KSGTDatei.h"
#include "Text.h"
#include "Datei.h"

using namespace Framework;

// Inhalt der KSGTDatei Klasse aus KSGTDatei.h
// Konstruktor
KSGTDatei::KSGTDatei()
    : pfad( new Text() ),
    data( new RCArray< RCArray< Text > >() ),
    ref( 1 )
{}

KSGTDatei::KSGTDatei( const char *pfad )
    : pfad( new Text() ),
    data( new RCArray< RCArray< Text > >() ),
    ref( 1 )
{
    setPfad( pfad );
}

KSGTDatei::KSGTDatei( Text *pfad )
    : pfad( new Text() ),
    data( new RCArray< RCArray< Text > >() ),
    ref( 1 )
{
    setPfad( pfad );
}

// Destruktor
KSGTDatei::~KSGTDatei()
{
    pfad->release();
    data->release();
}

// nicht constant
void KSGTDatei::setPfad( const char *pfad )
{
    this->pfad->setText( pfad );
}

void KSGTDatei::setPfad( Text *pfad )
{
    this->pfad->setText( pfad );
}

bool KSGTDatei::laden()
{
    std::ifstream inf;
    inf.open( pfad->getText(), std::ios::binary );
    if( !inf.good() || !inf.is_open() )
        return 0;
    data->leeren();
    inf.seekg( 0, std::ios::end );
    __int64 gr = inf.tellg();
    __int64 pos = 0;
    int zeilenPos = 0;
    int feldPos = 0;
    while( pos < gr )
    {
        inf.seekg( pos, std::ios::beg );
        char c = 1;
        int len = 0;
        do
        {
            inf.read( &c, 1 );
            ++len;
        } while( c != 0 && c != '\n' && pos + len < gr );
        if( pos + len == gr )
            ++len;
        inf.seekg( pos, std::ios::beg );
        char *v = new char[ len ];
        v[ len - 1 ] = 0;
        if( len > 1 )
            inf.read( v, len - 1 );
        pos += len;
        if( len > 1 && !data->z( zeilenPos ) )
            data->set( new RCArray< Text >, zeilenPos );
        if( !data->z( zeilenPos )->z( feldPos ) )
            data->z( zeilenPos )->set( new Text(), feldPos );
        data->z( zeilenPos )->z( feldPos )->setText( v );
        delete[] v;
        if( c == 0 )
            ++feldPos;
        else if( c == '\n' )
        {
            ++zeilenPos;
            feldPos = 0;
        }
        else
            break;
    }
    inf.close();
    return 1;
}

bool KSGTDatei::addZeile( int feldAnzahl, RCArray< Text > *zWert )
{
    int pos = getZeilenAnzahl();
    data->set( new RCArray< Text >(), pos );
    for( int i = 0; i < feldAnzahl; ++i )
        data->z( pos )->set( new Text( zWert->z( i )->getText() ), i );
    return 1;
}

bool KSGTDatei::setZeile( int zeile, int feldAnzahl, RCArray< Text > *zWert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    data->set( new RCArray< Text >(), zeile );
    for( int i = 0; i < feldAnzahl; ++i )
        data->z( zeile )->set( new Text( zWert->z( i )->getText() ), i );
    return 1;
}

bool KSGTDatei::removeZeile( int zeile )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    data->remove( zeile );
    return 1;
}

bool KSGTDatei::addFeld( int zeile, int pos, Text *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
    {
        wert->release();
        return 0;
    }
    int fA = getFeldAnzahl( zeile );
    if( pos > fA )
    {
        wert->release();
        return 0;
    }
    data->z( zeile )->add( new Text( wert->getText() ), pos );
    wert->release();
    return 1;
}

bool KSGTDatei::addFeld( int zeile, int pos, const char *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    if( pos > fA )
        return 0;
    data->z( zeile )->add( new Text( wert ), pos );
    return 1;
}

bool KSGTDatei::addFeld( int zeile, Text *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
    {
        wert->release();
        return 0;
    }
    int fA = getFeldAnzahl( zeile );
    data->z( zeile )->set( new Text( wert->getText() ), fA );
    wert->release();
    return 1;
}

bool KSGTDatei::addFeld( int zeile, const char *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    data->z( zeile )->set( new Text( wert ), fA );
    return 1;
}

bool KSGTDatei::setFeld( int zeile, int feld, Text *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
    {
        wert->release();
        return 0;
    }
    int fA = getFeldAnzahl( zeile );
    if( feld >= fA )
    {
        wert->release();
        return 0;
    }
    data->z( zeile )->set( new Text( wert->getText() ), feld );
    wert->release();
    return 1;
}

bool KSGTDatei::setFeld( int zeile, int feld, const char *wert )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    if( feld >= fA )
        return 0;
    data->z( zeile )->set( new Text( wert ), feld );
    return 1;
}

bool KSGTDatei::removeFeld( int zeile, int feld )
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    if( feld >= fA )
        return 0;
    data->z( zeile )->remove( feld );
    return 1;
}

bool KSGTDatei::speichern()
{
    if( !pfad->getLength() )
        return 0;
    if( !DateiExistiert( pfad->getText() ) )
        DateiPfadErstellen( pfad->getText() );
    std::ofstream of( pfad->getText(), std::ios::binary );
    if( !of.good() || !of.is_open() )
        return 0;
    int zA = getZeilenAnzahl();
    for( int z = 0; z < zA; ++z )
    {
        if( z )
            of.write( "\n", 1 );
        int fA = getFeldAnzahl( z );
        for( int f = 0; f < fA; ++f )
        {
            if( f )
                of.write( "\0", 1 );
            if( data->z( z ) && data->z( z )->z( f ) )
                of.write( data->z( z )->z( f )->getText(), data->z( z )->z( f )->getLength() );
        }
    }
    of.close();
    return 1;
}

// constant
int KSGTDatei::getZeilenAnzahl() const
{
    int ret = 0;
    while( data->z( ret ) )
        ++ret;
    return ret;
}

int KSGTDatei::getFeldAnzahl( int zeile ) const
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int ret = 0;
    while( data->z( zeile )->z( ret ) )
        ++ret;
    return ret;
}

Text *KSGTDatei::getFeld( int zeile, int feld ) const
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    if( feld >= fA )
        return 0;
    return data->z( zeile )->get( feld );
}

Text *KSGTDatei::zFeld( int zeile, int feld ) const
{
    int zA = getZeilenAnzahl();
    if( zeile >= zA )
        return 0;
    int fA = getFeldAnzahl( zeile );
    if( feld >= fA )
        return 0;
    return data->z( zeile )->z( feld );
}

// Reference Counting
KSGTDatei *KSGTDatei::getThis()
{
    ++ref;
    return this;
}

KSGTDatei *KSGTDatei::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}