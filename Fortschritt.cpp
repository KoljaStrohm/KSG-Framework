#include "Fortschritt.h"

#include "Rahmen.h"
#include "AlphaFeld.h"
#include "Bild.h"
#include "Schrift.h"
#include "Scroll.h"
#include "Text.h"

using namespace Framework;

// Inhalt der FBalken Klasse aus Fortschritt.h 
// Konstruktor 
FBalken::FBalken()
    : ZeichnungHintergrund(),
    maxAk( 0 ),
    ak( 0 ),
    fRahmen( 0 ),
    fBuffer( 0 ),
    fBgF( 0xFF000000 ),
    fBgBild( 0 ),
    schrift( 0 ),
    schriftFarbe( 0 ),
    schriftSize( 0 ),
    ref( 1 )
{
    style = 0;
}

// Destructor 
FBalken::~FBalken()
{
    if( fRahmen )
        fRahmen->release();
    if( fBuffer )
        fBuffer->release();
    if( fBgBild )
        fBgBild->release();
    if( schrift )
        schrift->release();
}

// nicht constant 
void FBalken::setAktionAnzahl( __int64 ak ) // setzt die anzahl der Aktionen
{
    maxAk = ak;
    rend = 1;
}

void FBalken::aktionPlus( __int64 aktionen ) // mehrere Aktionen sind fertig
{
    ak += aktionen;
    if( ak > maxAk )
        ak = maxAk;
    rend = 1;
}

void FBalken::reset() // setzt die fertigen Aktionen zurück
{
    ak = 0;
    rend = 1;
}

void FBalken::setFRahmenZ( LRahmen *ram ) // setzt einen Zeiger zum Fertig Rahmen
{
    if( fRahmen )
        fRahmen->release();
    fRahmen = ram;
    rend = 1;
}

void FBalken::setFRFarbe( int f ) // setzt die Fertig Rahmen Farbe
{
    if( !fRahmen )
        fRahmen = new LRahmen();
    fRahmen->setFarbe( f );
    rend = 1;
}

void FBalken::setFRBreite( int br ) // setzt die Fertig Rahmen Breite
{
    if( !fRahmen )
        fRahmen = new LRahmen();
    fRahmen->setRamenBreite( br );
    rend = 1;
}

void FBalken::setFAlphaFeldZ( AlphaFeld *af ) // setzt einen Zeiger zum Fertig Alpha Feld
{
    if( fBuffer )
        fBuffer->release();
    fBuffer = af;
    rend = 1;
}

void FBalken::setFAFFarbe( int f ) // setzt die Fertig Alpha Feld Farbe
{
    if( !fBuffer )
        fBuffer = new AlphaFeld();
    fBuffer->setFarbe( f );
    rend = 1;
}

void FBalken::setFAFStrength( int st ) // setzt die Stärke des Fertig Alpha Feldes
{
    if( !fBuffer )
        fBuffer = new AlphaFeld();
    fBuffer->setStrength( st );
    rend = 1;
}

void FBalken::setFBgFarbe( int f ) // setzt einen Zeiger zur Fertig Hintergrund Farbe
{
    fBgF = f;
    rend = 1;
}

void FBalken::setFBgBildZ( Bild *b ) // setzt das Fertig Hintergrund Bild
{
    if( fBgBild )
        fBgBild->release();
    fBgBild = b;
    rend = 1;
}

void FBalken::setFBgBild( Bild *b ) // kopiert in das Fertig Hintergrund Bild
{
    if( !fBgBild )
        fBgBild = new Bild();
    fBgBild->neuBild( b->getBreite(), b->getHeight(), 0 );
    fBgBild->drawBild( 0, 0, b->getBreite(), b->getHeight(), *b );
    b->release();
    rend = 1;
}

void FBalken::setSchriftZ( Schrift *s ) // setzt die Schrift
{
    if( schrift )
        schrift->release();
    schrift = s;
    rend = 1;
}

void FBalken::setSFarbe( int f ) // setzt die Schrift Farbe
{
    schriftFarbe = f;
    rend = 1;
}

void FBalken::setSSize( unsigned char gr ) // setzt die Schrift größe
{
    schriftSize = gr;
    rend = 1;
}

void FBalken::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( !hatStyle( Style::Sichtbar ) )
        return;
    lockZeichnung();
    removeStyle( Style::VScroll | Style::HScroll );
	ZeichnungHintergrund::render( zRObj );
    if( !zRObj.setDrawOptions( pos, gr ) )
    {
        unlockZeichnung();
        return;
    }
    int xx = 0;
    int yy = 0;
    int b = gr.x;
    int h = gr.y;
    if( hatStyle( Style::L_R ) )
        b = (int)( ( gr.x / 100.0 ) * getProzent() );
    else if( hatStyle( Style::R_L ) )
    {
        b = (int)( ( gr.x / 100.0 ) * getProzent() );
        xx -= b;
    }
    else if( hatStyle( Style::O_U ) )
        h = (int)( ( gr.y / 100.0 ) * getProzent() );
    else if( hatStyle( Style::U_O ) )
    {
        h = (int)( ( gr.y / 100.0 ) * getProzent() );
        yy -= h;
    }
    if( maxAk == 0 )
        b = 0, h = 0;
    if( !zRObj.setDrawOptions( xx, yy, b, h ) )
    {
        zRObj.releaseDrawOptions();
        unlockZeichnung();
        return;
    }
    int rbr = 0;
    if( hatStyle( Style::FRahmen ) && fRahmen )
    {
        fRahmen->setSize( b, h );
        fRahmen->render( zRObj );
        rbr = fRahmen->getRBreite();
    }
    if( hatStyle( Style::FFarbe ) )
    {
        if( hatStyle( Style::FAlpha ) )
            zRObj.alphaRegion( rbr, rbr, b - rbr * 2, h - rbr * 2, fBgF );
        else
            zRObj.fillRegion( rbr, rbr, b - rbr * 2, h - rbr * 2, fBgF );
    }
    if( hatStyle( Style::FBild ) && fBgBild )
    {
        if( hatStyle( Style::FAlpha ) )
            zRObj.alphaBildSkall( rbr, rbr, gr.x - rbr * 2, gr.y - rbr * 2, *fBgBild );
        else
            zRObj.alphaBildSkall( rbr, rbr, gr.x - rbr * 2, gr.y - rbr * 2, *fBgBild );
    }
    if( hatStyle( Style::FBuffered ) && fBuffer )
    {
        fBuffer->setSize( b - rbr * 2, h - rbr * 2 );
        fBuffer->render( zRObj );
    }
    zRObj.releaseDrawOptions();
    if( hatStyle( Style::Prozent ) && schrift )
    {
        schrift->setSchriftSize( schriftSize );
        Text txt;
        txt.append( (int)getProzent() );
        txt.append( "%" );
        schrift->setDrawPosition( rbr + ( gr.x - rbr * 2 ) / 2 - schrift->getTextBreite( &txt ) / 2, rbr + ( gr.y - rbr * 2 ) / 2 - schrift->getTextHeight( &txt ) / 2 );
        schrift->renderText( &txt, zRObj, schriftFarbe );
    }
    zRObj.releaseDrawOptions();
    unlockZeichnung();
}

// constant 
__int64 FBalken::getAktionAnzahl() const // gibt die Anzahl der Aktionen zurück
{
    return maxAk;
}

double FBalken::getProzent() const // gibt die momentane Prozentzahl zurück
{
    if( !maxAk )
        return 0;
    return (double)ak / ( (double)maxAk / 100.0 );
}

__int64 FBalken::getAktion() const // gibt die fertigen Aktionen zurück
{
    return ak;
}

LRahmen *FBalken::getFRahmen() const // gibt den Fertig Rahmen zurück
{
    if( fRahmen )
        return fRahmen->getThis();
    return 0;
}

LRahmen *FBalken::zFRahmen() const
{
    return fRahmen;
}

AlphaFeld *FBalken::getFAlphaFeld() const // gibt das Fertig Alpha Feld zurück
{
    if( fBuffer )
        return fBuffer->getThis();
    return 0;
}

AlphaFeld *FBalken::zFAlphaFeld() const
{
    return fBuffer;
}

int FBalken::getFBgFarbe() const // gibt die Fertig Hintergrund Farbe zurück
{
    return fBgF;
}

Bild *FBalken::getFBgBild() const // gibt das Fertig Hintergrund Bild zurück
{
    if( fBgBild )
        return fBgBild->getThis();
    return 0;
}

Bild *FBalken::zFBgBild() const
{
    return fBgBild;
}

Schrift *FBalken::getSchrift() const // gibt die Schrift zurück
{
    if( schrift )
        return schrift->getThis();
    return 0;
}

Schrift *FBalken::zSchrift() const
{
    return schrift;
}

int FBalken::getSFarbe() const // gibt die Schrift Farbe zurück
{
    return schriftFarbe;
}

// Reference Counting 
FBalken *FBalken::getThis()
{
    ++ref;
    return this;
}

FBalken *FBalken::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}