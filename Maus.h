#ifndef Maus_H
#define Maus_H

#include "Betriebssystem.h"

namespace Framework
{
    class Bild; // aus Bild.h
    class Maus; // aus dieser Datei

    namespace MausId
    {
        const int nichts = 0;
        const int normal = 1;
        const int hand = 2;
        const int warten = 3;
        const int verschieben = 4;
        const int text = 5;
        const int wahgerecht = 6;
        const int senkrecht = 7;
        const int diagonal1 = 8;
        const int diagonal2 = 9;
        const int verboten = 10;
    }

    // Diese Klasse verwaltet das Bild des Mauszeigers
    // Sie wird vom Framework intern verwendet
    // Siehe Globals.h MausZeiger
    class Maus
    {
    private:
        HCURSOR hMaus;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) Maus();
        // läd eine maus von Windows
        //  mausId: Werte aus dem Namespace MausId
        // beispiel: ladeMaus( MausId::hand );
        __declspec( dllexport ) void ladeMaus( int mausId );
        // Kopiert ein Bild und verwendet es als Mauszeiger.
        //  maus: Das Bild, was als Mauszeiger verwendet werden soll
        __declspec( dllexport ) void ladeMaus( Bild *maus );
        // Akzualisiert die Maus. Wird vom Framework selbst aufgerufen
        __declspec( dllexport ) void update();
        // gibt ein Händle zur maus zurück
        __declspec( dllexport ) HCURSOR getMausHandle();
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Maus *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Maus *release();
    };
}

#endif