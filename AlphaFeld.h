#ifndef AlphaFeld_H
#define AlphaFeld_H

#include "Zeichnung.h"

namespace Framework
{
	class Bild; // Bild.h
	class AlphaFeld; // Aus dieser Datei

    // Eine Zeichnung des 2D GUI Frameworks, die einen Farbübergang zu einem Rechteck darstellt
	class AlphaFeld : public Zeichnung
	{
	private:
		int stärke;
		int farbe;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) AlphaFeld();
		// Setzt die Stärke des Übergangs. Dies ist der Wert um den der Alpha Wert der Farbe für jeden Pixel nach innen abnimmt
        //  st: Die Stärke
		__declspec( dllexport ) void setStärke( int st );
        // Setzt die Farbe des Alpha Feldes
        //  f: Die Farbe im A8R8G8B8 Format
		__declspec( dllexport ) void setFarbe( int f );
        // Zeichnet die Zeihnung in ein bestimmtes Bild
        //  zRObj: Das Bild, in das gezeichnet werden soll
		__declspec( dllexport ) void render( Bild &zRObj ) override;
		// Gibt die Stärke des Alpha Feldes zurück
		__declspec( dllexport ) int getStärke() const;
        // gibt die Farbe des Alpha Feldes im A8R8G8B8 Format zurück
		__declspec( dllexport ) int getFarbe() const;
        // Kopiert das Alpha Feld, so dass es ohne auswirkungen auf das Original verwendet werden kann
		__declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
		__declspec( dllexport ) AlphaFeld *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
		__declspec( dllexport ) AlphaFeld *release();
	};
}

#endif