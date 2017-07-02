#include "AlphaFeld.h"
#include "Punkt.h"
#include "Bild.h"
#include "Scroll.h"
#include "ToolTip.h"
#include "Text.h"

using namespace Framework;

// Inhalt der AlphaFeld Klasse aus AlphaFeld.h
// Konstruktor 
AlphaFeld::AlphaFeld()
	: Zeichnung(),
	strength( 5 ),
	farbe( 0x9B000000 ),
	ref( 1 )
{}

AlphaFeld::~AlphaFeld()
{}

// nicht constant 
void AlphaFeld::setStrength( int st ) // setzt die Stärke
{
	strength = st;
	rend = 1;
}

void AlphaFeld::setFarbe( int f ) // setzt die Farbe
{
	farbe = f;
	rend = 1;
}

void AlphaFeld::render( Bild &zRObj ) // zeichnet nach zRObj
{
	Zeichnung::render( zRObj );
	int br = gr.x - 1;
	int hi = gr.y - 1;
	int xp = pos.x, yp = pos.y;
	int a = ( farbe >> 24 ) & 0xFF;
	int index = ( br / 2 ) * ( br <= hi ) + ( hi / 2 ) * ( br > hi );
	int fc = farbe & 0x00FFFFFF;
	int fc2 = farbe;
	if( strength > 0 )
		index = index > ( a / strength ) ? a / strength : index;
	if( strength < 0 )
		index = index > ( ( 255 - a ) / -strength ) ? ( ( 255 - a ) / -strength ) : index;
	for( int i = 0; i < index; ++i )
	{
		a -= strength;
		fc2 = ( a << 24 ) | fc;
		int i2 = i << 1;
		zRObj.drawLinieHAlpha( xp + i + 1, yp + i, br - i2, fc2 );		// oben links --- oben rechts
		zRObj.drawLinieVAlpha( xp + br - i, yp + i + 1, hi - i2, fc2 );	// oben rechts -- unten rechts
		zRObj.drawLinieHAlpha( xp + i, yp + hi - i, br - i2, fc2 );		// unten rechts - unten links
		zRObj.drawLinieVAlpha( xp + i, yp + i, hi - i2, fc2 );			// unten links -- oben links
	}
	if( index == br / 2 )
	{
		for( int i = index; i <= index + ( br - index ) - index; ++i )
			zRObj.drawLinieVAlpha( xp + i, yp + index, hi - ( index << 1 ) + 1, fc2 ); // rest Fläche senkrecht
	}
	else
	{
		for( int i = index; i <= index + ( hi - index ) - index; ++i )
			zRObj.drawLinieHAlpha( xp + index, yp + i, br - ( index << 1 ) + 1, fc2 ); // rest Fläche waagerecht
	}
}

// constant 
int AlphaFeld::getStrength() const // gibt die Stärke zurück
{
	return strength;
}

int AlphaFeld::getFarbe() const // gibt die Farbe zurück
{
	return farbe;
}

Zeichnung *AlphaFeld::dublizieren() const // Kopiert das Zeichnung
{
	AlphaFeld *obj = new AlphaFeld();
	obj->setPosition( pos );
	obj->setSize( gr );
	obj->setMausEreignisParameter( makParam );
	obj->setTastaturEreignisParameter( takParam );
	obj->setMausEreignis( mak );
	obj->setTastaturEreignis( tak );
	if( toolTip )
		obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
	obj->setStrength( strength );
	obj->setFarbe( farbe );
	return obj;
}

// Reference Counting 
AlphaFeld *AlphaFeld::getThis()
{
	++ref;
	return this;
}

AlphaFeld *AlphaFeld::release()
{
	--ref;
	if( !ref )
		delete this;
	return 0;
}