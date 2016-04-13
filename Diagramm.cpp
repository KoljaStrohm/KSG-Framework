#include "Diagramm.h"
#include "Text.h"
#include "AlphaFeld.h"
#include "Bild.h"
#include "Rahmen.h"
#include "Schrift.h"
#include "Scroll.h"
#include "MausEreignis.h"
#include "ToolTip.h"
#include <math.h>

using namespace Framework;

// Inhalt der SLDiag Klasse aus Diagramm.h
// Konstruktor 
SLDiag::SLDiag()
    : ZeichnungHintergrund(),
    gF( 0xFF000000 ),
    lFarbe( new Array< int >() ),
    lName( new RCArray< Text >() ),
    ph( new RCArray< Array< int > >() ),
    pb( new RCArray< Array< int > >() ),
    lastValue( new Array< int >() ),
    schrift( 0 ),
    gitterGr( 0, 0 ),
    lines( 0 ),
    ref( 1 )
{
    style = 0;
}

// Destruktor 
SLDiag::~SLDiag()
{
    lFarbe->release();
    lName->release();
    ph->release();
    pb->release();
    lastValue->release();
    if( schrift )
        schrift->release();
}

// nicht constant 
void SLDiag::setSchriftZ( Schrift *schrift ) // setzt die Schrift
{
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    rend = 1;
}

void SLDiag::setGGröße( Punkt &gr ) // setzt die Größe des Gitters
{
    gitterGr = gr;
    rend = 1;
}

void SLDiag::setGFarbe( int f ) // setzt die Gitter Farbe
{
    gF = f;
    rend = 1;
}

void SLDiag::addLinie( const char *name ) // fügt eine Linie hinzu
{
    addLinie( new Text( name ) );
    rend = 1;
}

void SLDiag::addLinie( Text *txt )
{
    lFarbe->add( 0xFFFFFFFF, lines );
    lName->add( txt, lines );
    ph->add( new Array< int >(), lines );
    pb->add( new Array< int >(), lines );
    int rbr = rahmen && hatStyle( Style::Rahmen ) ? rahmen->getRBreite() : 0;
    pb->z( lines )->set( gr.x - rbr * 2, 0 );
    ++lines;
    rend = 1;
}

void SLDiag::setLFarbe( int lNum, int f ) // setzt die Linienfarbe
{
    lFarbe->set( f, lNum );
    rend = 1;
}

void SLDiag::addPunkt( int lNum, int x, int h ) // fügt einen Punkt hinzu
{
    Array< int > *ph_tmp = ph->z( lNum );
    Array< int > *pb_tmp = pb->z( lNum );
    if( ph_tmp && pb_tmp )
    {
        int i = pb_tmp->get( 0 );
        i -= x;
        if( i >= 0 )
            pb_tmp->set( i, 0 );
        while( i < 0 )
        {
            ph_tmp->lösche( 0 );
            pb_tmp->lösche( 0 );
            if( !pb_tmp->getEintragAnzahl() || !ph_tmp->getEintragAnzahl() )
                break;
            int ii = -i;
            i = pb_tmp->get( 0 );
            i -= ii;
            pb_tmp->set( i, 0 );
        }
        pb_tmp->add( x );
        ph_tmp->add( h );
        lastValue->set( h, lNum );
    }
    rend = 1;
}

void SLDiag::removeLinie( int lNum ) // entfernt eine Linie
{
    lFarbe->lösche( lNum );
    lName->lösche( lNum );
    ph->lösche( lNum );
    pb->lösche( lNum );
    --lines;
    rend = 1;
}

void SLDiag::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( hatStyle( Style::Sichtbar ) )
    {
        lockZeichnung();
        löscheStyle( Style::VScroll | Style::HScroll );
        __super::render( zRObj );
        if( !zRObj.setDrawOptions( innenPosition, innenGröße ) )
        {
            unlockZeichnung();
            return;
        }
        int hv = getHöchstValue();
        hv = hv ? hv : 1;
        double yFaktor = innenGröße.y / hv;
        if( hatStyle( Style::Gitter ) )
        {
            double ghö = gitterGr.y * yFaktor;
            int yo = innenGröße.y - 1;
            int bo = innenGröße.x, ho = innenGröße.y;
            int maxBr = (int)( (double)innenGröße.x / gitterGr.x + 0.5 );
            int maxHö = ghö ? (int)( innenGröße.y / ghö + 0.5 ) : 0;
            if( hatStyle( Style::HAlpha ) )
            {
                for( int i = 0; i < maxBr; ++i )
                    zRObj.drawLinieVAlpha( gitterGr.x * i, 0, ho, gF );
                for( int i = 0; i < maxHö; ++i )
                    zRObj.drawLinieHAlpha( 0, (int)( yo - ghö * i + 0.5 ), bo, gF );
            }
            else
            {
                for( int i = 0; i < maxBr; ++i )
                    zRObj.drawLinieV( gitterGr.x * i, 0, ho, gF );
                for( int i = 0; i < maxHö; ++i )
                    zRObj.drawLinieH( 0, (int)( yo - ghö * i + 0.5 ), bo, gF );
            }
        }
        for( int i = 0; i < lines; ++i )
        {
            int f_tmp = lFarbe->hat( i ) ? lFarbe->get( i ) : 0;
            Text *n_tmp = lName->z( i );
            Array< int > *ph_tmp = ph->z( i );
            Array< int > *pb_tmp = pb->z( i );
            if( hatStyle( Style::LinienName ) && schrift && n_tmp )
            {
                schrift->setSchriftGröße( 12 );
                schrift->setDrawPosition( 5, 5 + 15 * i );
                Text rtxt = n_tmp->getText();
                rtxt += ": ";
                rtxt += lastValue->get( i );
                schrift->renderText( &rtxt, zRObj, f_tmp );
            }
            if( ph_tmp && pb_tmp )
            {
                int lastX = 0;
                int lastY = innenGröße.y - 1;
                int ph_anz = ph_tmp->getEintragAnzahl();
                int pb_anz = pb_tmp->getEintragAnzahl();
                for( int ii = 0; ii < ph_anz && ii < pb_anz; ++ii )
                {
                    if( !ph_tmp || !pb_tmp )
                        break;
                    int xpos = lastX + pb_tmp->get( ii );
                    int ypos = innenGröße.y - (int)( ph_tmp->get( ii ) * yFaktor + 0.5 ) - 1;
                    if( ypos == -1 )
                        ++ypos;
                    if( ypos < 0 || xpos < 0 )
                        break;
                    if( hatStyle( Style::HAlpha ) )
                        zRObj.drawLinieAlpha( Punkt( lastX, lastY ), Punkt( xpos, ypos ), f_tmp );
                    else
                        zRObj.drawLinie( Punkt( lastX, lastY ), Punkt( xpos, ypos ), f_tmp );
                    lastX = xpos;
                    lastY = ypos;
                }
            }
        }
        zRObj.releaseDrawOptions();
        unlockZeichnung();
    }
}

// constant
Schrift *SLDiag::getSchrift() const // gibt die Schrift zurück
{
    if( schrift )
        return schrift->getThis();
    return 0;
}

Schrift *SLDiag::zSchrift() const
{
    return schrift;
}

const Punkt &SLDiag::getGGröße() const // gibt die Gitter Größe zurück
{
    return gitterGr;
}

int SLDiag::getGFarbe() const // gibt die Gitter Farbe zurück
{
    return gF;
}

int SLDiag::getLinienNummer( const char *name ) const // gibt die Linien Nummer zurück
{
    for( int i = 0; i < lines; ++i )
    {
        if( lName->z( i )->istGleich( name ) )
            return i;
    }
    return -1;
}

int SLDiag::getLinienNummer( Text *name ) const
{
    for( int i = 0; i < lines; ++i )
    {
        if( lName->z( i )->istGleich( name->getText() ) )
        {
            name->release();
            return i;
        }
    }
    name->release();
    return -1;
}

Text *SLDiag::getLinienName( int lNum ) const // gibt den Linien Namen zurück
{
    return lName->get( lNum );
}

Text *SLDiag::zLinienNamen( int lNum ) const
{
    return lName->z( lNum );
}

int SLDiag::getHöchstValue() const // gibt den Höchsten Wert zurück
{
    int ret = 0;
    for( int i = 0; i < lines; ++i )
    {
        int tmp = getHöchstValue( i );
        ret = ret >= tmp ? ret : tmp;
    }
    return ret;
}

int SLDiag::getHöchstValue( int lNum ) const
{
    int ret = 0;
    Array< int > *tmp = ph->z( lNum );
    int anz = tmp->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
        ret = ret >= tmp->get( i ) ? ret : tmp->get( i );
    return ret;
}

int SLDiag::getMedian( int lNum ) const // gibt den durchschnittswert zurück
{
    int count;
    int all = 0;
    Array< int > *tmp = ph->z( lNum );
    int anz = tmp->getEintragAnzahl();
    for( count = 1; count <= anz; ++count )
        all += tmp->get( count - 1 );
    return (int)( (double)all / count + 0.5 );
}

int SLDiag::getLAnzahl() const // gibt die Linien Anzahl zurück
{
    return lines;
}

int SLDiag::getLastValue( int lNum ) const // gibt den letzten Wert zurück
{
    return lastValue->get( lNum );
}

// Reference Counting 
SLDiag *SLDiag::getThis()
{
    ++ref;
    return this;
}

SLDiag *SLDiag::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}


// Inhalt der DiagWert Struktur aus Diagramm.h
// Konstruktor
DiagWert::DiagWert()
    : style( 0 ),
    farbe( 0xFFFFFFFF ),
    hintergrund( 0 ),
    name( new Text() ),
    punkte( new Array< DiagPunkt* > ),
    ref( 1 )
{}

// Destruktor
DiagWert::~DiagWert()
{
    name->release();
    int anz = punkte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( punkte->hat( i ) )
            delete punkte->get( i );
    }
    punkte->release();
}

// Referenc Counting
DiagWert *DiagWert::getThis()
{
    ++ref;
    return this;
}

DiagWert *DiagWert::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der DiagDaten Struktur aus Diagramm.h
// Konstruktor
DiagDaten::DiagDaten()
    : style( 0 ),
    rasterDicke( 0 ),
    rasterBreite( 0 ),
    rasterHöhe( 0 ),
    rasterFarbe( 0 ),
    hIntervallFarbe( 0xFFFFFFFF ),
    vIntervallFarbe( 0xFFFFFFFF ),
    hIntervallName( new Text() ),
    vIntervallName( new Text() ),
    hIntervallTexte( new RCArray< Text > ),
    vIntervallTexte( new RCArray< Text > ),
    hIntervallWerte( new Array< double > ),
    vIntervallWerte( new Array< double > ),
    hIntervallBreite( 0 ),
    vIntervallHöhe( 0 ),
    werte( new RCArray< DiagWert >() ),
    ref( 1 )
{}

// Destruktor
DiagDaten::~DiagDaten()
{
    hIntervallName->release();
    vIntervallName->release();
    hIntervallWerte->release();
    vIntervallWerte->release();
    hIntervallTexte->release();
    vIntervallTexte->release();
    werte->release();
}

// Referenc Counting
DiagDaten *DiagDaten::getThis()
{
    ++ref;
    return this;
}

DiagDaten *DiagDaten::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der BaseDiag Klasse aus Diagramm.h
// Konstruktor
BaseDiag::BaseDiag( CRITICAL_SECTION *lock )
    : daten( new DiagDaten() ),
    ref( 1 ),
    changed( 0 ),
    lock( lock )
{}

// Destruktor
BaseDiag::~BaseDiag()
{
    if( daten )
        daten->release();
}

// nicht constant
void BaseDiag::setDiagDatenZ( DiagDaten *dd ) // Setzt einen Zeiger auf die Daten des Diagramms
{
    EnterCriticalSection( lock );
    if( daten )
        daten->release();
    daten = dd;
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::setDiagDaten( DiagDaten *dd ) // Kopiert die Daten eines Diagramms
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    daten->style = dd->style;
    daten->rasterBreite = dd->rasterBreite;
    daten->rasterHöhe = dd->rasterHöhe;
    daten->rasterFarbe = dd->rasterFarbe;
    daten->hIntervallFarbe = dd->hIntervallFarbe;
    daten->vIntervallFarbe = dd->vIntervallFarbe;
    daten->hIntervallName->setText( *dd->hIntervallName );
    daten->vIntervallName->setText( *dd->vIntervallName );
    int anz = dd->hIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( dd->hIntervallWerte->hat( i ) )
            daten->hIntervallWerte->set( dd->hIntervallWerte->get( i ), i );
    }
    anz = dd->vIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( dd->vIntervallWerte->hat( i ) )
            daten->vIntervallWerte->set( dd->vIntervallWerte->get( i ), i );
    }
    daten->hIntervallBreite = dd->hIntervallBreite;
    daten->vIntervallHöhe = dd->vIntervallHöhe;
    anz = dd->werte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        DiagWert *tmp = dd->werte->z( i );
        if( tmp )
        {
            DiagWert *w = new DiagWert();
            w->style = tmp->style;
            w->farbe = tmp->farbe;
            w->hintergrund = tmp->hintergrund;
            w->name->setText( tmp->name->getText() );
            int anz = tmp->punkte->getEintragAnzahl();
            for( int j = 0; j < anz; ++j )
            {
                if( tmp->punkte->hat( j ) )
                {
                    DiagPunkt *p = new DiagPunkt();
                    p->hIntervall = tmp->punkte->get( j )->hIntervall;
                    p->vIntervall = tmp->punkte->get( j )->vIntervall;
                    w->punkte->set( p, j );
                }
            }
            daten->werte->set( w, i );
        }
    }
    dd->release();
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::setRasterDicke( int d ) // Rasterdicke setzen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->rasterDicke != d )
        changed = 1;
    daten->rasterDicke = d;
    LeaveCriticalSection( lock );
}

void BaseDiag::setRasterBreite( int br ) // Rasterbreite setzen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->rasterBreite != br )
        changed = 1;
    daten->rasterBreite = br;
    LeaveCriticalSection( lock );
}

void BaseDiag::setRasterHöhe( int hö ) // Rasterhöhe setzen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->rasterHöhe != hö )
        changed = 1;
    daten->rasterHöhe = hö;
    LeaveCriticalSection( lock );
}

void BaseDiag::setRasterFarbe( int f ) // Rasterfarbe setzen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->rasterFarbe != f )
        changed = 1;
    daten->rasterFarbe = f;
    LeaveCriticalSection( lock );
}

void BaseDiag::setHIntervallBreite( double br ) // Intervall Breite
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    daten->hIntervallBreite = br;
    LeaveCriticalSection( lock );
}

void BaseDiag::setVIntervallHöhe( double hö ) // Intervall Höhe
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    daten->vIntervallHöhe = hö;
    LeaveCriticalSection( lock );
}

void BaseDiag::setHIntervallFarbe( int f ) // Farbe des Horizontalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->hIntervallFarbe != f )
        changed = 1;
    daten->hIntervallFarbe = f;
    LeaveCriticalSection( lock );
}

void BaseDiag::setVIntervallFarbe( int f ) // Farbe des Vertikalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( daten->vIntervallFarbe != f )
        changed = 1;
    daten->vIntervallFarbe = f;
    LeaveCriticalSection( lock );
}

void BaseDiag::setHIntervallName( char *name ) // Setzt den Namen des Horizontalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->hIntervallName )
        daten->hIntervallName = new Text();
    if( !daten->hIntervallName->istGleich( name ) )
        changed = 1;
    daten->hIntervallName->setText( name );
    LeaveCriticalSection( lock );
}

void BaseDiag::setHIntervallName( Text *name )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->hIntervallName )
        daten->hIntervallName = new Text();
    if( !daten->hIntervallName->istGleich( *name ) )
        changed = 1;
    daten->hIntervallName->setText( name );
    LeaveCriticalSection( lock );
}

void BaseDiag::setVIntervallName( char *name ) // Setzt den Namen des Vertikalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->vIntervallName )
        daten->vIntervallName = new Text();
    if( !daten->vIntervallName->istGleich( name ) )
        changed = 1;
    daten->vIntervallName->setText( name );
    LeaveCriticalSection( lock );
}

void BaseDiag::setVIntervallName( Text *name )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->vIntervallName )
        daten->vIntervallName = new Text();
    if( !daten->vIntervallName->istGleich( *name ) )
        changed = 1;
    daten->vIntervallName->setText( name );
    LeaveCriticalSection( lock );
}

void BaseDiag::addHIntervallText( double hIntervall, char *text ) // Text eines Horizontalen Intervalls hinzufügen
{
    setHIntervallText( hIntervall, text );
}

void BaseDiag::addHIntervallText( double hIntervall, Text *text )
{
    setHIntervallText( hIntervall, *text );
    text->release();
}

void BaseDiag::setHIntervallTextZ( double hIntervall, Text *text ) // Setzt den Text eines Horizontalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->hIntervallWerte )
        daten->hIntervallWerte = new Array< double >();
    if( !daten->hIntervallTexte )
        daten->hIntervallTexte = new RCArray< Text >();
    int anz = daten->hIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->hIntervallWerte->hat( i ) && daten->hIntervallWerte->get( i ) == hIntervall )
        {
            daten->hIntervallTexte->set( text, i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    daten->hIntervallWerte->set( hIntervall, anz );
    daten->hIntervallTexte->set( text, anz );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::setHIntervallText( double hIntervall, Text *text )
{
    setHIntervallText( hIntervall, *text );
    text->release();
}

void BaseDiag::setHIntervallText( double hIntervall, char *text )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->hIntervallWerte )
        daten->hIntervallWerte = new Array< double >();
    if( !daten->hIntervallTexte )
        daten->hIntervallTexte = new RCArray< Text >();
    int anz = daten->hIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->hIntervallWerte->hat( i ) && daten->hIntervallWerte->get( i ) == hIntervall )
        {
            if( !daten->hIntervallTexte->z( i ) )
                daten->hIntervallTexte->set( new Text( text ), i );
            else
                daten->hIntervallTexte->z( i )->setText( text );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    daten->hIntervallWerte->set( hIntervall, anz );
    daten->hIntervallTexte->set( new Text( text ), anz );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::löscheHIntervallText( double hIntervall ) // Text eines Horizontalen Intervalls entfernen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->hIntervallWerte )
        daten->hIntervallWerte = new Array< double >();
    if( !daten->hIntervallTexte )
        daten->hIntervallTexte = new RCArray< Text >();
    int anz = daten->hIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->hIntervallWerte->hat( i ) && daten->hIntervallWerte->get( i ) == hIntervall )
        {
            daten->hIntervallTexte->lösche( i );
            daten->hIntervallWerte->lösche( i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    LeaveCriticalSection( lock );
}

void BaseDiag::addVIntervallText( double vIntervall, char *text ) // Text eines Vertikalen Intervalls hinzufügen
{
    setVIntervallText( vIntervall, text );
}

void BaseDiag::addVIntervallText( double vIntervall, Text *text )
{
    setVIntervallText( vIntervall, *text );
    text->release();
}

void BaseDiag::setVIntervallTextZ( double vIntervall, Text *text ) // Setzt den Text eines Vertikalen Intervalls
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->vIntervallWerte )
        daten->vIntervallWerte = new Array< double >();
    if( !daten->vIntervallTexte )
        daten->vIntervallTexte = new RCArray< Text >();
    int anz = daten->vIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->vIntervallWerte->hat( i ) && daten->vIntervallWerte->get( i ) == vIntervall )
        {
            daten->vIntervallTexte->set( text, i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    daten->vIntervallWerte->set( vIntervall, anz );
    daten->vIntervallTexte->set( text, anz );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::setVIntervallText( double vIntervall, Text *text )
{
    setVIntervallText( vIntervall, *text );
    text->release();
}

void BaseDiag::setVIntervallText( double vIntervall, char *text )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->vIntervallWerte )
        daten->vIntervallWerte = new Array< double >();
    if( !daten->vIntervallTexte )
        daten->vIntervallTexte = new RCArray< Text >();
    int anz = daten->vIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->vIntervallWerte->hat( i ) && daten->vIntervallWerte->get( i ) == vIntervall )
        {
            if( !daten->vIntervallTexte->z( i ) )
                daten->vIntervallTexte->set( new Text( text ), i );
            else
                daten->vIntervallTexte->z( i )->setText( text );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    daten->vIntervallWerte->set( vIntervall, anz );
    daten->vIntervallTexte->set( new Text( text ), anz );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::löscheVIntervallText( double vIntervall ) // Text eines Vertikalen Intervalls entfernen
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->vIntervallWerte )
        daten->vIntervallWerte = new Array< double >();
    if( !daten->vIntervallTexte )
        daten->vIntervallTexte = new RCArray< Text >();
    int anz = daten->vIntervallWerte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->vIntervallWerte->hat( i ) && daten->vIntervallWerte->get( i ) == vIntervall )
        {
            daten->vIntervallTexte->lösche( i );
            daten->vIntervallWerte->lösche( i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    LeaveCriticalSection( lock );
}

void BaseDiag::addWertZ( DiagWert *w ) // Fügt einen Wert hinzu
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    daten->werte->add( w );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addWert( DiagWert *w )
{
    EnterCriticalSection( lock );
    DiagWert *tmp = new DiagWert();
    tmp->style = w->style;
    tmp->farbe = w->farbe;
    tmp->hintergrund = w->hintergrund;
    tmp->name->setText( *w->name );
    int anz = w->punkte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( w->punkte->hat( i ) )
        {
            DiagPunkt *p = new DiagPunkt();
            p->hIntervall = w->punkte->get( i )->hIntervall;
            p->vIntervall = w->punkte->get( i )->vIntervall;
            tmp->punkte->set( p, i );
        }
    }
    w->release();
    addWertZ( tmp );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addWert( const char *name )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    DiagWert *w = new DiagWert();
    w->name->setText( name );
    daten->werte->add( w );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addWert( Text *txt )
{
    addWert( *txt );
    txt->release();
}

void BaseDiag::setWertFarbe( int wNum, int f ) // setzt die Farbe eines Wertes
{
    if( wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    daten->werte->z( wNum )->farbe = f;
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addPunktZ( int wNum, DiagPunkt *p ) // fügt einem Wert einen Punkt hinzu
{
    if( wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    daten->werte->z( wNum )->punkte->add( p );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addPunkt( int wNum, DiagPunkt *p )
{
    if( wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    DiagPunkt *tmp = new DiagPunkt();
    tmp->hIntervall = p->hIntervall;
    tmp->vIntervall = p->vIntervall;
    daten->werte->z( wNum )->punkte->add( tmp );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::addPunkt( int wNum, double hI, double vI )
{
    if( wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    DiagPunkt *tmp = new DiagPunkt();
    tmp->hIntervall = hI;
    tmp->vIntervall = vI;
    daten->werte->z( wNum )->punkte->add( tmp );
    LeaveCriticalSection( lock );
    changed = 1;
}

// Ändert einen vorhandenen Punkt eines Wertes
void BaseDiag::setPunktZ( int wNum, double hI, DiagPunkt *p )
{
    setPunktZ( wNum, getDiagPunktPos( wNum, hI ), p );
}

void BaseDiag::setPunkt( int wNum, double hI, DiagPunkt *p )
{
    setPunkt( wNum, getDiagPunktPos( wNum, hI ), p->hIntervall, p->vIntervall );
}

void BaseDiag::setPunkt( int wNum, double hI, double h, double v )
{
    setPunkt( wNum, getDiagPunktPos( wNum, hI ), h, v );
}

void BaseDiag::setPunktZ( int wNum, int pNum, DiagPunkt *p )
{
    if( pNum < 0 || wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    if( daten->werte->z( wNum )->punkte->hat( pNum ) )
        delete daten->werte->z( wNum )->punkte->get( pNum );
    daten->werte->z( wNum )->punkte->set( p, pNum );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::setPunkt( int wNum, int pNum, DiagPunkt *p )
{
    if( pNum < 0 || wNum < 0 )
        return;
    setPunkt( wNum, pNum, p->hIntervall, p->vIntervall );
}

void BaseDiag::setPunkt( int wNum, int pNum, double h, double v )
{
    if( pNum < 0 || wNum < 0 )
        return;
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    if( !daten->werte->z( wNum )->punkte->hat( pNum ) )
        daten->werte->z( wNum )->punkte->set( new DiagPunkt(), pNum );
    daten->werte->z( wNum )->punkte->get( pNum )->hIntervall = h;
    daten->werte->z( wNum )->punkte->get( pNum )->vIntervall = v;
    LeaveCriticalSection( lock );
    changed = 1;
}

// Löscht einen vorhandenen Punkt
void BaseDiag::löschePunkt( int wNum, double hI )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    int anz = daten->werte->z( wNum )->punkte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->werte->z( wNum )->punkte->hat( i ) && daten->werte->z( wNum )->punkte->get( i )->hIntervall == hI )
        {
            delete daten->werte->z( wNum )->punkte->get( i );
            daten->werte->z( wNum )->punkte->lösche( i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    LeaveCriticalSection( lock );
}

void BaseDiag::löschePunkt( int wNum, int pNum )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( !daten->werte->z( wNum )->punkte )
        daten->werte->z( wNum )->punkte = new Array< DiagPunkt* >();
    if( daten->werte->z( wNum )->punkte->hat( pNum ) )
    {
        delete daten->werte->z( wNum )->punkte->get( pNum );
        daten->werte->z( wNum )->punkte->lösche( pNum );
        changed = 1;
    }
    LeaveCriticalSection( lock );
}

void BaseDiag::removeWert( int wNum ) // entfernt einen Wert
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    daten->werte->lösche( wNum );
    LeaveCriticalSection( lock );
    changed = 1;
}

void BaseDiag::removeWert( char *name )
{
    EnterCriticalSection( lock );
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte )
        daten->werte = new RCArray< DiagWert >();
    int anz = daten->werte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        DiagWert *tmp = daten->werte->z( i );
        if( tmp && tmp->name && tmp->name->istGleich( name ) )
        {
            daten->werte->lösche( i );
            LeaveCriticalSection( lock );
            changed = 1;
            return;
        }
    }
    LeaveCriticalSection( lock );
}

void BaseDiag::removeWert( Text *name )
{
    removeWert( name->getText() );
    name->release();
}

void BaseDiag::addDatenStyle( int style ) // Setzt den Style der Daten
{
    if( !daten )
        daten = new DiagDaten();
    if( ( daten->style | style ) != daten->style )
        changed = 1;
    daten->style |= style;
}

void BaseDiag::setDatenStyle( int style )
{
    if( !daten )
        daten = new DiagDaten();
    if( daten->style != style )
        changed = 1;
    daten->style = style;
}

void BaseDiag::setDatenStyle( int style, bool addRemove )
{
    if( addRemove )
        addDatenStyle( style );
    else
        löscheDatenStyle( style );
}

void BaseDiag::löscheDatenStyle( int style )
{
    if( !daten )
        daten = new DiagDaten();
    if( ( daten->style & ~style ) != daten->style )
        changed = 1;
    daten->style &= ~style;
}

void BaseDiag::addWertStyle( int wNum, int style ) // Setzt den Style eines Wertes
{
    if( wNum < 0 )
        return;
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( ( daten->werte->z( wNum )->style | style ) != daten->werte->z( wNum )->style )
        changed = 1;
    daten->werte->z( wNum )->style |= style;
}

void BaseDiag::setWertStyle( int wNum, int style )
{
    if( wNum < 0 )
        return;
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( daten->werte->z( wNum )->style != style )
        changed = 1;
    daten->werte->z( wNum )->style = style;
}

void BaseDiag::setWertStyle( int wNum, int style, bool addRemove )
{
    if( wNum < 0 )
        return;
    if( addRemove )
        addWertStyle( wNum, style );
    else
        löscheWertStyle( wNum, style );
}

void BaseDiag::löscheWertStyle( int wNum, int style )
{
    if( wNum < 0 )
        return;
    if( !daten )
        daten = new DiagDaten();
    if( !daten->werte->z( wNum ) )
        daten->werte->set( new DiagWert(), wNum );
    if( ( daten->werte->z( wNum )->style & ~style ) != daten->werte->z( wNum )->style )
        changed = 1;
    daten->werte->z( wNum )->style &= ~style;
}

// constant
DiagDaten *BaseDiag::getDiagDaten() const // Gibt die Daten des Diagramms zurück
{
    return daten ? daten->getThis() : 0;
}

DiagDaten *BaseDiag::zDiagDaten() const
{
    return daten;
}

DiagWert *BaseDiag::getDiagWert( int wNum ) const // Gibt die Daten eines Wertes zurück
{
    return ( daten && daten->werte ) ? daten->werte->get( wNum ) : 0;
}

DiagWert *BaseDiag::zDiagWert( int wNum ) const
{
    return ( daten && daten->werte ) ? daten->werte->z( wNum ) : 0;
}

DiagWert *BaseDiag::getDiagWert( char *name ) const
{
    return getDiagWert( getDiagWertPos( name ) );
}

DiagWert *BaseDiag::zDiagWert( char *name ) const
{
    return zDiagWert( getDiagWertPos( name ) );
}

int BaseDiag::getDiagWertPos( char *name ) const // Gibt die Position eines Wertes zurück
{
    if( !daten || !daten->werte )
        return -1;
    int anz = daten->werte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->werte->z( i ) && daten->werte->z( i )->name && daten->werte->z( i )->name->istGleich( name ) )
            return i;
    }
    return -1;
}

int BaseDiag::getDiagPunktPos( int wNum, double hI ) const // Gibt die Position eines Punktes von einem Wert zurück
{
    if( !daten || !daten->werte || !daten->werte->z( wNum ) || !daten->werte->z( wNum )->punkte )
        return -1;
    int anz = daten->werte->z( wNum )->punkte->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
    {
        if( daten->werte->z( wNum )->punkte->hat( i ) && daten->werte->z( wNum )->punkte->get( i )->hIntervall == hI )
            return i;
    }
    return -1;
}

int BaseDiag::getDiagPunktPos( char *wName, double hI ) const
{
    return getDiagPunktPos( getDiagWertPos( wName ), hI );
}

bool BaseDiag::hatDatenStyle( int style ) const // Prüft den Style der Daten
{
    if( !daten )
        return 0;
    return ( daten->style | style ) == daten->style;
}

bool BaseDiag::hatDatenStyleNicht( int style ) const
{
    if( !daten )
        return 1;
    return ( daten->style | style ) != daten->style;
}

bool BaseDiag::hatWertStyle( int wNum, int style ) const // Prüft den Style eines Wertes
{
    if( !daten || !daten->werte || !daten->werte->z( wNum ) )
        return 0;
    return ( daten->werte->z( wNum )->style | style ) == daten->werte->z( wNum )->style;
}

bool BaseDiag::hatWertStyleNicht( int wNum, int style ) const
{
    if( !daten || !daten->werte || !daten->werte->z( wNum ) )
        return 1;
    return ( daten->werte->z( wNum )->style | style ) != daten->werte->z( wNum )->style;
}

// Reference Counting
BaseDiag *BaseDiag::getThis()
{
    ++ref;
    return this;
}

BaseDiag *BaseDiag::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der LDiag Klasse aus Diagramm.h
// Konstruktor
LDiag::LDiag()
    : ZeichnungHintergrund(),
    BaseDiag( &cs ),
    schrift( 0 ),
    dRam( new LRahmen() ),
    dBgF( 0 ),
    dBgB( 0 ),
    dAf( new AlphaFeld() ),
    vIntervallRB( new Bild() ),
    hIntervallRB( new Bild() ),
    schriftGr( 12 )
{
    vertikalScrollBar = new VScrollBar();
    horizontalScrollBar = new HScrollBar();
}

// Destruktor
LDiag::~LDiag()
{
    if( schrift )
        schrift->release();
    if( dRam )
        dRam->release();
    if( dBgB )
        dBgB->release();
    if( dAf )
        dAf->release();
    if( vIntervallRB )
        vIntervallRB->release();
    if( hIntervallRB )
        hIntervallRB->release();
}

// nicht constant
void LDiag::setSchriftZ( Schrift *schrift ) // Setzt die Schrift
{
    lockZeichnung();
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    rend = 1;
    unlockZeichnung();
}

void LDiag::setSchriftGröße( int gr )
{
    lockZeichnung();
    if( schriftGr != gr )
        rend = 1;
    schriftGr = gr;
    unlockZeichnung();
}

void LDiag::setDatenRahmenZ( LRahmen *ram ) // Setzt den inneren Rahmen um das eigentliche Diagramm
{
    lockZeichnung();
    if( dRam )
        dRam->release();
    dRam = ram;
    unlockZeichnung();
    rend = 1;
}

void LDiag::setDatenRahmen( LRahmen *ram )
{
    lockZeichnung();
    if( !dRam )
        dRam = new LRahmen();
    dRam->setAlpha( ram->hatAlpha() );
    dRam->setFarbe( ram->getFarbe() );
    dRam->setRamenBreite( ram->getRBreite() );
    unlockZeichnung();
    ram->release();
    rend = 1;
}

void LDiag::setDatenRahmenBreite( int br )
{
    lockZeichnung();
    if( !dRam )
        dRam = new LRahmen();
    if( dRam->getRBreite() != br )
        rend = 1;
    dRam->setRamenBreite( br );
    unlockZeichnung();
}

void LDiag::setDatenRahmenFarbe( int f )
{
    lockZeichnung();
    if( !dRam )
        dRam = new LRahmen();
    dRam->setFarbe( f );
    unlockZeichnung();
    rend = 1;
}

void LDiag::setDatenHintergrundFarbe( int f ) // Setzt den Hintergrund des eigentlichen Diagramms
{
    dBgF = f;
    rend = 1;
}

void LDiag::setDatenHintergrundBildZ( Bild *b )
{
    lockZeichnung();
    if( dBgB )
        dBgB->release();
    dBgB = b;
    unlockZeichnung();
    rend = 1;
}

void LDiag::setDatenHintergrundBild( Bild *b )
{
    lockZeichnung();
    if( !dBgB )
        dBgB = new Bild();
    dBgB->neuBild( b->getBreite(), b->getHöhe(), 0 );
    dBgB->drawBild( 0, 0, b->getBreite(), b->getHöhe(), *b );
    unlockZeichnung();
    b->release();
    rend = 1;
}

void LDiag::setDatenAlphaFeldZ( AlphaFeld *af ) // Setzt das AlphaFeld des eigentlichen Diagramms
{
    lockZeichnung();
    if( dAf )
        dAf->release();
    dAf = af;
    unlockZeichnung();
    rend = 1;
}

void LDiag::setDatenAlphaFeld( AlphaFeld *af )
{
    lockZeichnung();
    if( !dAf )
        dAf = new AlphaFeld();
    dAf->setFarbe( af->getFarbe() );
    dAf->setStärke( af->getStärke() );
    unlockZeichnung();
    af->release();
    rend = 1;
}

void LDiag::setDatenAlphaFeldFarbe( int f )
{
    lockZeichnung();
    if( !dAf )
        dAf = new AlphaFeld();
    dAf->setFarbe( f );
    unlockZeichnung();
    rend = 1;
}

void LDiag::setDatenAlphaFeldStärke( int st )
{
    lockZeichnung();
    if( !dAf )
        dAf = new AlphaFeld();
    if( dAf->getStärke() != st )
        rend = 1;
    dAf->setStärke( st );
    unlockZeichnung();
}

void LDiag::doMausEreignis( MausEreignis &me )
{
    bool nmakc = !me.verarbeitet;
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) )
    {
        if( mausIn )
        {
            mausIn = 0;
            if( toolTip )
                toolTip->setMausIn( 0 );
            MausEreignis me2;
            me2.id = ME_Verlässt;
            me2.mx = me.mx;
            me2.my = me.my;
            me2.verarbeitet = 0;
            doMausEreignis( me2 );
            return;
        }
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Verlässt )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
        return;
    }
    if( !mausIn && me.id != ME_Verlässt )
    {
        if( toolTip )
            toolTip->setMausIn( 1 );
        mausIn = 1;
        MausEreignis me2;
        me2.id = ME_Betritt;
        me2.mx = me.mx;
        me2.my = me.my;
        me2.verarbeitet = 0;
        doMausEreignis( me2 );
    }
    me.mx -= pos.x;
    me.my -= pos.y;
    if( hatDatenStyle( DiagDaten::Style::Sichtbar ) && ( hatStyle( Style::HScroll ) || hatStyle( Style::VScroll ) ) )
    {
        if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
        {
            if( me.id != ME_Betritt && me.id != ME_Verlässt )
            {
                lockZeichnung();
                int rbr = 0;
                if( hatStyle( Style::Rahmen ) && rahmen )
                    rbr = rahmen->getRBreite();
                bool vs = hatStyle( Style::VScroll ) && vertikalScrollBar;
                bool hs = hatStyle( Style::HScroll ) && horizontalScrollBar;
                if( vs )
                {
                    if( hs )
                        horizontalScrollBar->doMausMessage( rbr, gr.y - rbr - 15, gr.x - rbr * 2 - 15, 15, me );
                    vertikalScrollBar->doMausMessage( gr.x - rbr - 15, rbr, 15, gr.y - rbr * 2, me );
                }
                else if( hs )
                    horizontalScrollBar->doMausMessage( rbr, gr.y - rbr - 15, gr.x - rbr * 2, 15, me );
                unlockZeichnung();
            }
            me.verarbeitet = 1;
        }
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x;
    me.my += pos.y;
}

void LDiag::render( Bild &zRObj )
{
    if( !hatDatenStyle( DiagDaten::Style::Sichtbar ) )
        return;
    __super::render( zRObj );
    if( !zRObj.setDrawOptions( innenPosition, innenGröße ) )
        return;
    lockZeichnung();
    int dgy = 0;
    int dgbr = innenGröße.x;
    int dghö = innenGröße.y;
    int vIAnz = 0;
    int hIAnz = 0;
    if( daten )
    { // Auto werte berechnen
        vIAnz = daten->vIntervallWerte ? daten->vIntervallWerte->getEintragAnzahl() : 0;
        hIAnz = daten->hIntervallWerte ? daten->hIntervallWerte->getEintragAnzahl() : 0;
        if( daten->vIntervallWerte && hatDatenStyle( DiagDaten::Style::AutoIntervallHöhe ) )
        {
            double maxW = 0;
            for( int i = 0; i < vIAnz; ++i )
            {
                if( daten->vIntervallWerte->hat( i ) && daten->vIntervallWerte->get( i ) > maxW )
                    maxW = daten->vIntervallWerte->get( i );
            }
            if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                daten->vIntervallHöhe = maxW ? ( vertikalScrollBar->getScroll() / maxW ) : 0;
            else
                daten->vIntervallHöhe = maxW ? ( vIntervallRB->getBreite() / maxW ) : 0;
        }
        if( hatDatenStyle( DiagDaten::Style::AutoRasterHöhe ) )
            daten->rasterHöhe = daten->vIntervallHöhe;
        if( daten->hIntervallWerte && hatDatenStyle( DiagDaten::Style::AutoIntervallBreite ) )
        {
            double maxW = 0;
            for( int i = 0; i < vIAnz; ++i )
            {
                if( daten->hIntervallWerte->hat( i ) && daten->hIntervallWerte->get( i ) > maxW )
                    maxW = daten->hIntervallWerte->get( i );
            }
            if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                daten->hIntervallBreite = maxW ? ( horizontalScrollBar->getScroll() / maxW ) : 0;
            else
                daten->hIntervallBreite = maxW ? ( hIntervallRB->getBreite() / maxW ) : 0;
        }
        if( hatDatenStyle( DiagDaten::Style::AutoRasterBreite ) )
            daten->rasterBreite = daten->hIntervallBreite;
    }
    if( hatDatenStyle( DiagDaten::Style::VIntervall ) && schrift && schriftGr && daten )
    { // Rendern des vertikalen Intervalls
        int vIRBbr = innenGröße.y;
        if( hatDatenStyle( DiagDaten::Style::HIntervall ) && daten->hIntervallFarbe )
            vIRBbr -= schriftGr + 2;
        if( vIRBbr > 0 )
        {
            if( vIntervallRB->getBreite() != vIRBbr || vIntervallRB->getHöhe() != schriftGr + 2 )
                vIntervallRB->neuBild( vIRBbr, schriftGr + 2, 0 );
            else
                vIntervallRB->füllRegion( 0, 0, vIRBbr, schriftGr + 2, 0 );
            schrift->lock();
            schrift->setSchriftGröße( schriftGr );
            if( daten->vIntervallWerte )
            {
                int *rf = new int[ vIAnz ];
                int anz = 0;
                bool *fertig = new bool[ vIAnz ];
                ZeroMemory( fertig, vIAnz );
                for( int i = 0; i < vIAnz; ++i )
                {
                    double min = 0;
                    int mI = -1;
                    for( int j = 0; j < vIAnz; ++j )
                    {
                        if( !fertig[ j ] && daten->vIntervallWerte->hat( j ) && ( daten->vIntervallWerte->get( j ) < min || mI < 0 ) )
                        {
                            mI = j;
                            min = daten->vIntervallWerte->get( j );
                        }
                    }
                    if( mI < 0 )
                        break;
                    rf[ anz ] = mI;
                    ++anz;
                    fertig[ mI ] = 1;
                }
                delete[] fertig;
                Text txt;
                int xPos = 0;
                if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                    xPos -= vertikalScrollBar->getScroll();
                int rWeite = xPos - 10;
                for( int i = anz - 1; i >= 0; --i )
                {
                    txt = "";
                    if( hatDatenStyle( DiagDaten::Style::VIntervallTexte ) && daten->vIntervallTexte && daten->vIntervallTexte->z( rf[ i ] ) )
                        txt = daten->vIntervallTexte->z( rf[ i ] )->getText();
                    else if( hatDatenStyleNicht( DiagDaten::Style::VIntervallTexte ) || !daten->vIntervallWerte )
                        txt = daten->vIntervallWerte->get( rf[ i ] );
                    xPos = (int)( daten->vIntervallWerte->get( rf[ i ] ) * daten->vIntervallHöhe );
                    xPos = ( vertikalScrollBar && hatStyle( Style::VScroll ) ? vertikalScrollBar->getScroll() : vIntervallRB->getBreite() ) - xPos;
                    if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                        xPos -= vertikalScrollBar->getScroll();
                    int br = schrift->getTextBreite( &txt );
                    if( xPos + br > vIntervallRB->getBreite() && vIntervallRB->getBreite() - br >= rWeite + 10 )
                        xPos = vIntervallRB->getBreite() - br;
                    if( xPos >= rWeite + 10 )
                    {
                        schrift->setDrawPosition( xPos, 1 );
                        schrift->renderText( &txt, *vIntervallRB, daten->vIntervallFarbe );
                        rWeite = xPos + br + 10;
                    }
                }
                delete[] rf;
            }
            if( daten->vIntervallName )
            {
                int vinbr = schrift->getTextBreite( daten->vIntervallName );
                int vinx = vIntervallRB->getBreite() / 2 - vinbr / 2;
                vIntervallRB->füllRegion( vinx - 5, 1, vinbr + 10, schriftGr, 0 );
                schrift->setDrawPosition( vinx, 1 );
                schrift->renderText( daten->vIntervallName, *vIntervallRB, daten->vIntervallFarbe );
            }
            schrift->unlock();
            if( hatDatenStyle( DiagDaten::Style::HIntervall ) && daten->hIntervallFarbe )
                zRObj.alphaBild90( innenGröße.x - vIntervallRB->getHöhe(), schriftGr + 2, vIntervallRB->getBreite(), vIntervallRB->getHöhe(), *vIntervallRB );
            else
                zRObj.alphaBild90( innenGröße.x - vIntervallRB->getHöhe(), 0, vIntervallRB->getBreite(), vIntervallRB->getHöhe(), *vIntervallRB );
            dgbr -= vIntervallRB->getHöhe();
        }
    }
    if( hatDatenStyle( DiagDaten::Style::HIntervall ) && schrift && schriftGr && daten )
    { // Rendern des horizontalen Intervalls
        int hIRBbr = innenGröße.x;
        if( hatDatenStyle( DiagDaten::Style::VIntervall ) && daten->vIntervallFarbe )
            hIRBbr -= schriftGr + 2;
        if( hIRBbr > 0 )
        {
            if( hIntervallRB->getBreite() != hIRBbr || hIntervallRB->getHöhe() != schriftGr + 2 )
                hIntervallRB->neuBild( hIRBbr, schriftGr + 2, 0 );
            else
                hIntervallRB->füllRegion( 0, 0, hIRBbr, schriftGr + 2, 0 );
            schrift->lock();
            schrift->setSchriftGröße( schriftGr );
            if( daten->hIntervallWerte )
            {
                int *rf = new int[ hIAnz ];
                int anz = 0;
                bool *fertig = new bool[ hIAnz ];
                ZeroMemory( fertig, hIAnz );
                for( int i = 0; i < hIAnz; ++i )
                {
                    double min = 0;
                    int mI = -1;
                    for( int j = 0; j < hIAnz; ++j )
                    {
                        if( !fertig[ j ] && daten->hIntervallWerte->hat( j ) && ( daten->hIntervallWerte->get( j ) < min || mI < 0 ) )
                        {
                            mI = j;
                            min = daten->hIntervallWerte->get( j );
                        }
                    }
                    if( mI < 0 )
                        break;
                    rf[ anz ] = mI;
                    ++anz;
                    fertig[ mI ] = 1;
                }
                delete[] fertig;
                Text txt;
                int xPos = 0;
                if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                    xPos -= horizontalScrollBar->getScroll();
                int rWeite = xPos - 10;
                for( int i = 0; i < anz; ++i )
                {
                    txt = "";
                    if( hatDatenStyle( DiagDaten::Style::HIntervallTexte ) && daten->hIntervallTexte && daten->hIntervallTexte->z( rf[ i ] ) )
                        txt = daten->hIntervallTexte->z( rf[ i ] )->getText();
                    else if( hatDatenStyleNicht( DiagDaten::Style::HIntervallTexte ) || !daten->hIntervallWerte )
                        txt = daten->hIntervallWerte->get( rf[ i ] );
                    xPos = (int)( daten->hIntervallWerte->get( rf[ i ] ) * daten->hIntervallBreite );
                    if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                        xPos -= horizontalScrollBar->getScroll();
                    int br = schrift->getTextBreite( &txt );
                    if( xPos + br > hIntervallRB->getBreite() && hIntervallRB->getBreite() - br >= rWeite + 10 )
                        xPos = hIntervallRB->getBreite() - br;
                    if( xPos >= rWeite + 10 )
                    {
                        schrift->setDrawPosition( xPos, 1 );
                        schrift->renderText( &txt, *hIntervallRB, daten->hIntervallFarbe );
                        rWeite = xPos + br + 10;
                    }
                }
                delete[] rf;
            }
            if( daten->hIntervallName )
            {
                int hinbr = schrift->getTextBreite( daten->hIntervallName );
                int hinx = hIntervallRB->getBreite() / 2 - hinbr / 2;
                hIntervallRB->füllRegion( hinx - 5, 1, hinbr + 10, schriftGr, 0 );
                schrift->setDrawPosition( hinx, 1 );
                schrift->renderText( daten->hIntervallName, *hIntervallRB, daten->hIntervallFarbe );
            }
            schrift->unlock();
            zRObj.alphaBild( 0, 0, hIntervallRB->getBreite(), hIntervallRB->getHöhe(), *hIntervallRB );
            dghö -= hIntervallRB->getHöhe();
            dgy += hIntervallRB->getHöhe();
        }
    }
    if( !zRObj.setDrawOptions( 0, dgy, dgbr, dghö ) )
    {
        zRObj.releaseDrawOptions();
        unlockZeichnung();
        return;
    }
    dgy = 0;
    int dgrbr = 0;
    if( hatStyle( Style::DatenRahmen ) && dRam )
    {
        dRam->setGröße( dgbr, dghö );
        dRam->render( zRObj );
        dgrbr = dRam->getRBreite();
    }
    if( !zRObj.setDrawOptions( dgrbr, dgrbr, dgbr - dgrbr * 2, dghö - dgrbr * 2 ) )
    {
        zRObj.releaseDrawOptions();
        zRObj.releaseDrawOptions();
        unlockZeichnung();
        return;
    }
    if( hatStyle( Style::DatenHintergrund ) )
    {
        if( hatStyle( Style::DatenHAlpha ) )
            zRObj.alphaRegion( 0, 0, dgbr - dgrbr * 2, dghö - dgrbr * 2, dBgF );
        else
            zRObj.füllRegion( 0, 0, dgbr - dgrbr * 2, dghö - dgrbr * 2, dBgF );
        if( hatStyle( Style::DatenHBild ) && dBgB )
        {
            if( hatStyle( Style::DatenHAlpha ) )
                zRObj.alphaBild( 0, 0, dgbr - dgrbr * 2, dghö - dgrbr * 2, *dBgB );
            else
                zRObj.drawBild( 0, 0, dgbr - dgrbr * 2, dghö - dgrbr * 2, *dBgB );
        }
    }
    if( hatStyle( Style::DatenBuffered ) && dAf )
    {
        dAf->setGröße( dgbr - dgrbr * 2, dghö - dgrbr * 2 );
        dAf->render( zRObj );
    }
    if( hatDatenStyle( DiagDaten::Style::Raster ) && daten && daten->rasterDicke && daten->rasterBreite && daten->rasterHöhe )
    { // Raster
        int maxBr = dgbr;
        if( horizontalScrollBar && hatStyle( Style::HScroll ) )
            maxBr = horizontalScrollBar->getScroll();
        int maxHö = dghö;
        if( vertikalScrollBar && hatStyle( Style::VScroll ) )
            maxHö = vertikalScrollBar->getScroll();
        for( double x = horizontalScrollBar && hatStyle( Style::HScroll ) ? -horizontalScrollBar->getScroll() : 0; x < maxBr; x += daten->rasterBreite )
        {
            for( int i = 0; i < daten->rasterDicke; ++i )
            {
                if( hatDatenStyle( DiagDaten::Style::RasterAlpha ) )
                    zRObj.drawLinieVAlpha( (int)( x + i ), dgy + dgrbr, dghö - dgrbr * 2, daten->rasterFarbe );
                else
                    zRObj.drawLinieV( (int)( x + i ), dgy + dgrbr, dghö - dgrbr * 2, daten->rasterFarbe );
            }
        }
        for( double y = maxHö; y > ( vertikalScrollBar && hatStyle( Style::VScroll ) ? -vertikalScrollBar->getScroll() : 0 ); y -= daten->rasterHöhe )
        {
            for( int i = 0; i < daten->rasterDicke; ++i )
            {
                if( hatDatenStyle( DiagDaten::Style::RasterAlpha ) )
                    zRObj.drawLinieHAlpha( 0, (int)( y + i ), dgbr - dgrbr * 2, daten->rasterFarbe );
                else
                    zRObj.drawLinieH( 0, (int)( y + i ), dgbr - dgrbr * 2, daten->rasterFarbe );
            }
        }
    }
    if( daten && daten->werte && daten->werte->getEintragAnzahl() && daten->hIntervallBreite && daten->vIntervallHöhe )
    { // Werte
        int wAnz = daten->werte->getEintragAnzahl();
        for( int i = 0; i < wAnz; ++i )
        {
            DiagWert *wert = daten->werte->z( i );
            if( wert && hatWertStyle( i, DiagWert::Style::Sichtbar ) && wert->punkte && wert->punkte->getEintragAnzahl() )
            {
                int pAnz = wert->punkte->getEintragAnzahl();
                int *rf = new int[ pAnz ];
                bool *fertig = new bool[ pAnz ];
                int anz = 0;
                ZeroMemory( fertig, pAnz );
                for( int j = 0; j < pAnz; ++j )
                {
                    double min = -1;
                    int p = -1;
                    for( int pi = 0; pi < pAnz; ++pi )
                    {
                        if( wert->punkte->hat( pi ) && !fertig[ pi ] && ( p < 0 || wert->punkte->get( pi )->hIntervall < min ) )
                        {
                            min = wert->punkte->get( pi )->hIntervall;
                            p = pi;
                        }
                    }
                    if( p < 0 )
                        break;
                    rf[ anz ] = p;
                    fertig[ p ] = 1;
                    ++anz;
                }
                delete[] fertig;
                int rpx = horizontalScrollBar && hatStyle( Style::HScroll ) ? -horizontalScrollBar->getScroll() : 0;
                int rpy = vertikalScrollBar && hatStyle( Style::VScroll ) ? -vertikalScrollBar->getScroll() : 0;
                rpy += dgy;
                int dgmhö = vertikalScrollBar && hatStyle( Style::VScroll ) ? dgy + vertikalScrollBar->getScrollData()->maxHöhe + dgrbr : dgy + dghö - dgrbr;
                if( hatWertStyle( i, DiagWert::Style::Hintergrund ) )
                {
                    DiagPunkt *vorher = 0;
                    DiagPunkt *jetzt = 0;
                    for( int j = 0; j < anz; ++j )
                    {
                        jetzt = wert->punkte->get( rf[ j ] );
                        if( jetzt && vorher )
                        {
                            int ax = (int)( rpx + vorher->hIntervall * daten->hIntervallBreite );
                            int ay = (int)( dgmhö - vorher->vIntervall * daten->vIntervallHöhe );
                            int bx = (int)( rpx + jetzt->hIntervall * daten->hIntervallBreite );
                            int by = (int)( dgmhö - jetzt->vIntervall * daten->vIntervallHöhe );
                            if( ax >= bx )
                            {
                                vorher = jetzt;
                                continue;
                            }
                            if( !( ax > dgbr - dgrbr || bx < 0 || ( ay > dgy + dghö - dgrbr && by > dgy + dghö - dgrbr ) ) )
                            {
                                double yf = (double)( by - ay ) / ( bx - ax );
                                double y = (double)ay;
                                ax = ax < 0 ? 0 : ax;
                                bx = bx > dgbr - dgrbr ? dgbr - dgrbr : bx;
                                if( hatWertStyle( i, DiagWert::Style::HAlpha ) )
                                {
                                    for( int x = ax; x < bx; x++, y += yf )
                                        zRObj.drawLinieVAlpha( x, (int)( y + 0.5 ), dgmhö - (int)( y + 0.5 ), wert->hintergrund );
                                }
                                else
                                {
                                    for( int x = ax; x < bx; x++, y += yf )
                                        zRObj.drawLinieV( x, (int)( y + 0.5 ), dgmhö - (int)( y + 0.5 ), wert->hintergrund );
                                }
                                if( hatWertStyle( i, DiagWert::Style::Alpha ) )
                                    zRObj.drawLinieAlpha( Punkt( ax, ay ), Punkt( bx, by ), wert->farbe );
                                else
                                    zRObj.drawLinie( Punkt( ax, ay ), Punkt( bx, by ), wert->farbe );
                            }
                        }
                        vorher = jetzt;
                    }
                }
                else
                {
                    DiagPunkt *vorher = 0;
                    DiagPunkt *jetzt = 0;
                    for( int j = 0; j < anz; ++j )
                    {
                        jetzt = wert->punkte->get( rf[ j ] );
                        if( jetzt && vorher )
                        {
                            if( hatWertStyle( i, DiagWert::Style::Alpha ) )
                                zRObj.drawLinieAlpha( Punkt( (int)( rpx + vorher->hIntervall * daten->hIntervallBreite ), (int)( dgmhö - vorher->vIntervall * daten->vIntervallHöhe ) ),
                                                      Punkt( (int)( rpx + jetzt->hIntervall * daten->hIntervallBreite ), (int)( dgmhö - jetzt->vIntervall * daten->vIntervallHöhe ) ), wert->farbe );
                            else
                                zRObj.drawLinie( Punkt( (int)( rpx + vorher->hIntervall * daten->hIntervallBreite ), (int)( dgmhö - vorher->vIntervall * daten->vIntervallHöhe ) ),
                                                 Punkt( (int)( rpx + jetzt->hIntervall * daten->hIntervallBreite ), (int)( dgmhö - jetzt->vIntervall * daten->vIntervallHöhe ) ), wert->farbe );
                        }
                        vorher = jetzt;
                    }
                }
                delete[] rf;
            }
        }
    }
    if( daten && daten->werte && daten->werte->getEintragAnzahl() && schrift && schriftGr )
    {
        int wAnz = daten->werte->getEintragAnzahl();
        int rx = 5;
        int ry = 5;
        schrift->lock();
        schrift->setSchriftGröße( schriftGr );
        for( int i = 0; i < wAnz; ++i )
        {
            DiagWert *w = daten->werte->z( i );
            if( w && w->name && hatWertStyle( i, DiagWert::Style::Name ) && hatWertStyle( i, DiagWert::Style::Sichtbar ) )
            {
                int br = schrift->getTextBreite( w->name );
                zRObj.alphaRegion( rx, ry, br, schriftGr, 0xA0000000 );
                schrift->setDrawPosition( rx, ry );
                schrift->renderText( w->name, zRObj, w->farbe );
                ry += 15;
            }
        }
        schrift->unlock();
    }
    zRObj.releaseDrawOptions();
    zRObj.releaseDrawOptions();
    zRObj.releaseDrawOptions();
    unlockZeichnung();
}

// constant
Schrift *LDiag::getSchrift() const // Gibt die Schrift zurück
{
    return schrift ? schrift->getThis() : 0;
}

Schrift *LDiag::zSchrift() const
{
    return schrift;
}

LRahmen *LDiag::getDatenRahmen() const // Gibt den inneren Rahmen um das eigentliche Diagramm zurück
{
    return dRam ? dRam->getThis() : 0;
}

LRahmen *LDiag::zDatenRahmen() const
{
    return dRam;
}

int LDiag::getDatenRahmenFarbe() const
{
    return dRam ? dRam->getFarbe() : 0;
}

int LDiag::getDatenRahmenBreite() const
{
    return dRam ? dRam->getRBreite() : 0;
}

int LDiag::getDatenHintergrundFarbe() const // Gibt den Hintergrund des eigentlichen Diagramms zurück
{
    return dBgF;
}

Bild *LDiag::getDatenHintergrundBild() const
{
    return dBgB ? dBgB->getThis() : 0;
}

Bild *LDiag::zDatenHintergrundBild() const
{
    return dBgB;
}

AlphaFeld *LDiag::getDatenAlphaFeld() const // Gibt das AlphaFeld des eigentlichen Diagramms zurück
{
    return dAf ? dAf->getThis() : 0;
}

AlphaFeld *LDiag::zDatenAlphaFeld() const
{
    return dAf;
}

int LDiag::getDatenAlphaFeldFarbe() const
{
    return dAf ? dAf->getFarbe() : 0;
}

int LDiag::getDatenAlphaFeldStärke() const
{
    return dAf ? dAf->getStärke() : 0;
}

// Reference Counting
BaseDiag *LDiag::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}