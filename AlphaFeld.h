#ifndef AlphaFeld_H
#define AlphaFeld_H

#include "Zeichnung.h"

namespace Framework
{
	class Bild; // Bild.h
	class AlphaFeld; // Aus dieser Datei

	class AlphaFeld : public Zeichnung
	{
	private:
		int stärke;
		int farbe;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) AlphaFeld();
		// nicht constant 
		__declspec( dllexport ) void setStärke( int st ); // setzt die Stärke
		__declspec( dllexport ) void setFarbe( int f ); // setzt die Farbe
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
		// constant 
		__declspec( dllexport ) int getStärke() const; // gibt die Stärke zurück
		__declspec( dllexport ) int getFarbe() const; // gibt die Farbe zurück
		__declspec( dllexport ) Zeichnung *dublizieren() const override; // Kopiert das Zeichnung
		// Reference Counting 
		__declspec( dllexport ) AlphaFeld *getThis();
		__declspec( dllexport ) AlphaFeld *release();
	};
}

#endif