#ifndef Rahmen_H
#define Rahmen_H

#include "Zeichnung.h"

namespace Framework
{
    class Bild; // aus Bild.h
    struct VScrollData; // Scroll.h
    struct HScrollData; // Scroll.h
    class LRahmen; // aus dieser Datei

    // Eine Zeichnung des 2D GUI Frameworks, die einen Linienrahmen um ein Rechteck zeichnet
    class LRahmen : public Zeichnung
    {
    private:
        int br;
        int farbe;
        bool alpha;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) LRahmen();
		// Destruktor
		__declspec( dllexport ) virtual ~LRahmen();
        // Setzt die Breite des Rahmens
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setRamenBreite( int br );
        // Legt fest, ob beim zeichnen alpha blending verwendet werden soll
        //  a: 1, falls alpha blending verwendet werden soll
        __declspec( dllexport ) void setAlpha( bool a );
        // Setzt die Fare des Rahmens
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setFarbe( int f );
        // Zeichnet den Rahmen
        //  zRObj: Das Bild, in den der Rahmen gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die Breite des Rahmens in Pixeln zurück
        __declspec( dllexport ) int getRBreite() const;
        // Gibt die Farbe des Rahmens im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getFarbe() const;
        // Gibt zurück, ob der Rahmen mit Alpha Blending gezeichnet wird
        __declspec( dllexport ) bool hatAlpha() const;
        // Kopiert den Rahmen, so dass er ohne auswirkungen auf das Original verändert werden kann
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LRahmen *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LRahmen *release();
    };
}

#endif