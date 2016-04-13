#include "Rahmen.h"
#include "Punkt.h"
#include "Bild.h"
#include "Scroll.h"
#include "ToolTip.h"
#include "Text.h"

using namespace Framework;

// Inhalt der LRahmen Klasse aus Rahmen.h
// Konstruktor 
LRahmen::LRahmen()
	: Zeichnung(),
	  br( 1 ),
	  farbe( 0xFF000000 ),
	  alpha( 0 ),
	  ref( 1 )
{
}

// nicht constant 
void LRahmen::setRamenBreite( int br ) // setzt die Breite des Rahmens
{
	this->br = br;
	rend = 1;
}

void LRahmen::setAlpha( bool a ) // Legt fest, ob der Alphawert der Farbe berücksichtigt werden soll
{
	alpha = a;
	rend = 1;
}

void LRahmen::setFarbe( int f ) // Legt die Farbe des Rahmens fest
{
	farbe = f;
	rend = 1;
}

void LRahmen::render( Bild &Obj ) // Zeichnet den Rahmen in das RenderZeichnung
{
    __super::render( Obj );
	int x = pos.x;
	int y = pos.y;
	int b = x + gr.x - 1;
	int h = y + gr.y - 1;
	if( alpha )
	{
		for( int i = 0; i < br; ++i )
		{
			Obj.drawLinieHAlpha( x + i + 1, y + i, gr.x - i * 2 - 1, farbe );
			Obj.drawLinieVAlpha( b - i, y + i + 1, gr.y - i * 2 - 2, farbe );
			Obj.drawLinieHAlpha( x + i + 1, h - i, gr.x - i * 2 - 1, farbe );
			Obj.drawLinieVAlpha( x + i, y + i, gr.y - i * 2, farbe );
		}
	}
	else
	{
		for( int i = 0; i < br; ++i )
		{
			Obj.drawLinieH( x + i + 1, y + i, gr.x - i * 2 - 1, farbe );
			Obj.drawLinieV( b - i, y + i + 1, gr.y - i * 2 - 2, farbe );
			Obj.drawLinieH( x + i + 1, h - i, gr.x - i * 2 - 1, farbe );
			Obj.drawLinieV( x + i, y + i, gr.y - i * 2, farbe );
		}
	}
}

// constant 
int LRahmen::getRBreite() const // Gibt die Breite des Rahmens zurück
{
	return br;
}

int LRahmen::getFarbe() const // Gibt die Farbe des Ramens zurück
{
	return farbe;
}

bool LRahmen::hatAlpha() const // Gibt zurück, ob der Alphawert der Farbe beachtet wird
{
	return alpha;
}

Zeichnung *LRahmen::dublizieren() const // Kopiert das Zeichnung
{
	LRahmen *obj = new LRahmen();
	obj->setPosition( pos );
	obj->setGröße( gr );
	obj->setMausEreignisParameter( makParam );
	obj->setTastaturEreignisParameter( takParam );
	obj->setMausEreignis( Mak );
	obj->setTastaturEreignis( Tak );
	if( toolTip )
		obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
	obj->setAlpha( alpha );
	obj->setFarbe( farbe );
	obj->setRamenBreite( br );
	return obj;
}

// Reference Counting 
LRahmen *LRahmen::getThis()
{
	++ref;
	return this;
}

LRahmen *LRahmen::release()
{
	--ref;
	if( ref == 0 )
		delete this;
	return 0;
}