#include "ToolTip.h"
#include "TextFeld.h"
#include "Text.h"
#include "AlphaFeld.h"
#include "Bild.h"
#include "MausEreignis.h"
#include "Schrift.h"
#include "Bildschirm.h"

using namespace Framework;

// Inhalt der ToolTip Klasse aus ToolTip.h
// Konstruktor
ToolTip::ToolTip( Bildschirm *zScreen )
    : TextFeld(),
    größe( 0, 0 ),
    animationSpeed( 250 ),
    warten( 1 ),
    wartenCount( 0 ),
    tval( 0 ),
    mausIn( 0 ),
    alpha( 0 ),
    sichtbar( 0 ),
    bildschirm( zScreen ),
    zeichnen( 0 )
{
    bildschirm->addToolTip( ( ToolTip* )this->getThis() );
}

// Destruktor
ToolTip::~ToolTip()
{}

// nicht constant
void ToolTip::setGröße( int breite, int höhe )
{
    größe.x = breite;
    größe.y = höhe;
    rend = 1;
}

void ToolTip::setGröße( Punkt &gr )
{
    größe = gr;
    rend = 1;
}

void ToolTip::setWarten( double warten )
{
    this->warten = warten;
}

void ToolTip::setAnimationSpeed( double speed )
{
    animationSpeed = speed;
}

void ToolTip::setMausIn( bool mausIn )
{
    if( this->mausIn != mausIn )
        rend = 1;
    this->mausIn = mausIn;
    if( !mausIn )
        sichtbar = 0;
}

void ToolTip::wartenReset()
{
    wartenCount = 0;
}

void ToolTip::setZeichnen()
{
    zeichnen = 1;
}

bool ToolTip::tick( double tickVal )
{
    this->tval += tickVal * animationSpeed;
    int val = ( int )this->tval;
    if( val < 1 )
    {
        bool ret = rend;
        rend = 0;
        return ret;
    }
    this->tval -= val;
    if( !sichtbar )
    {
        if( alpha )
        {
            if( alpha - val < 0 )
                alpha = 0;
            else
                alpha -= val;
            rend = 1;
        }
        if( mausIn )
        {
            wartenCount += tickVal;
            if( wartenCount >= warten )
            {
                sichtbar = 1;
                wartenCount = 0;
                alpha = 0xFF;
                __super::setGröße( 0, 0 );
            }
        }
        else
            wartenCount = 0;
    }
    else
    {
        if( getBreite() != größe.x )
        {
            __super::setGröße( getBreite() + val, getSchriftGröße() + getLinienRahmenBreite() * 2 );
            if( getBreite() > größe.x )
                __super::setGröße( größe.x, getHöhe() );
            rend = 1;
        }
        else if( getHöhe() != größe.y )
        {
            __super::setGröße( getBreite(), getHöhe() + val );
            if( getHöhe() > größe.y )
                __super::setGröße( getBreite(), größe.y );
            rend = 1;
        }
    }
    bool ret = rend;
    rend = 0;
    return ret;
}

void ToolTip::doMausEreignis( MausEreignis &me )
{
    if( mausIn )
        pos.x = me.mx, pos.y = me.my + 15;
    sichtbar = 0;
    if( alpha )
        rend = 1;
}

void ToolTip::render( Bild &zRObj )
{
    if( alpha && zeichnen )
    {
        zSchrift()->lock();
        zSchrift()->setSchriftGröße( getSchriftGröße() );
        größe = Punkt( zSchrift()->getTextBreite( zText() ) + getLinienRahmenBreite() * 2, zSchrift()->getTextHöhe( zText() ) + getLinienRahmenBreite() * 2 );
        zSchrift()->unlock();
        zRObj.setAlpha( alpha );
        setPosition( pos );
        if( getX() + getBreite() > zRObj.getBreite() )
            setPosition( getX() - ( getX() + getBreite() - zRObj.getBreite() ), getY() );
        if( getY() + getHöhe() > zRObj.getHöhe() )
            setPosition( getX(), getY() - ( getY() + getHöhe() - zRObj.getHöhe() ) );
        __super::render( zRObj );
        zRObj.releaseAlpha();
        zeichnen = 0;
    }
}

// constant
Bildschirm *ToolTip::zBildschirm() const
{
    return bildschirm;
}

// Reference Counting
TextFeld *ToolTip::getThis()
{
    ++ref;
    return this;
}

TextFeld *ToolTip::release()
{
    --ref;
    if( !ref )
    {
        delete this;
        return 0;
    }
    if( ref == 1 )
    {
        if( !bildschirm->removeToolTip( this ) )
            delete this;
    }
    return 0;
}