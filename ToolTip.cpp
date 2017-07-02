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
    size( 0, 0 ),
    animationSpeed( 250 ),
    warten( 1 ),
    wartenCount( 0 ),
    tval( 0 ),
    mausIn( 0 ),
    alpha( 0 ),
    sichtbar( 0 ),
    zeichnen( 0 ),
	bildschirm( zScreen )
{
    bildschirm->addToolTip( ( ToolTip* )this->getThis() );
}

// Destruktor
ToolTip::~ToolTip()
{}

// nicht constant
void ToolTip::setSize( int breite, int height )
{
    size.x = breite;
    size.y = height;
    rend = 1;
}

void ToolTip::setSize( Punkt &gr )
{
    size = gr;
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
                alpha = (unsigned char)( alpha - val );
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
				TextFeld::setSize( 0, 0 );
            }
        }
        else
            wartenCount = 0;
    }
    else
    {
        if( getBreite() != size.x )
        {
			TextFeld::setSize( getBreite() + val, getSchriftSize() + getLinienRahmenBreite() * 2 );
            if( getBreite() > size.x )
				TextFeld::setSize( size.x, getHeight() );
            rend = 1;
        }
        else if( getHeight() != size.y )
        {
			TextFeld::setSize( getBreite(), getHeight() + val );
            if( getHeight() > size.y )
				TextFeld::setSize( getBreite(), size.y );
            rend = 1;
        }
    }
    return TextFeld::tick( tickVal );
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
        zSchrift()->setSchriftSize( getSchriftSize() );
        size = Punkt( zSchrift()->getTextBreite( zText() ) + getLinienRahmenBreite() * 2, zSchrift()->getTextHeight( zText() ) + getLinienRahmenBreite() * 2 );
        zSchrift()->unlock();
        zRObj.setAlpha( alpha );
        setPosition( pos );
        if( getX() + getBreite() > zRObj.getBreite() )
            setPosition( getX() - ( getX() + getBreite() - zRObj.getBreite() ), getY() );
        if( getY() + getHeight() > zRObj.getHeight() )
            setPosition( getX(), getY() - ( getY() + getHeight() - zRObj.getHeight() ) );
		TextFeld::render( zRObj );
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