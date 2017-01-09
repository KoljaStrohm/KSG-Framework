#ifndef Scroll_H
#define Scroll_H

#include "Betriebssystem.h"

namespace Framework
{
    class Bild; // Bild.h
    struct MausEreignis; // MausEreignis.h

    struct ScrollData
    {
        int anzeige;
        int max;
        int scrollPos;
    };

    // Die Baseklasse vür die Horizontale und Vertikale Scrollbar
    class ScrollBar
    {
    protected:
        ScrollData *data;
        int knopfdruck;
        int farbe;
        int bgFarbe;
        bool bg;
        int klickScroll;
        int mx, my;
        bool mp;
        bool rend;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) ScrollBar();
        // Destruktor 
        __declspec( dllexport ) virtual ~ScrollBar();
        // Setzt die Fordergrundfarbe der Scrollbar
        //  fc: Die Farbe in A8R8G8B8 Format
        __declspec( dllexport ) void setFarbe( int fc );
        // Setzt die Hintergrundfarbe der Scrollbar
        //  fc: Die Farbe in A8R8G8B8 Format
        //  bgF: 1, fals ein Hintergrund gezeichnet werden soll
        __declspec( dllexport ) void setBgFarbe( int fc, bool bgF );
        // Aktualisiert die Scrollbar
        //  maxGr: Die Maximale Größe des Fensters in Pixeln
        //  anzeigeGr: Die angezeigte Größe des Fensters in Pixeln
        __declspec( dllexport ) void update( int maxGr, int anzeigeGr );
        // Setzt die Scroll geschwindigkeit
        //  klickScroll: Die anzahl an Pixeln, die pro Klick gescrollt werden
        __declspec( dllexport ) void setKlickScroll( int klickScroll );
        // Scrollt zu einer bestimmten Position
        //  pos: Die Position in Pixeln im Fenster, ab der gezeichnet werden soll
        __declspec( dllexport ) void scroll( int pos );
        // Verarbeitet ein Maus Ereignis
        //  x: Die X Position der Scroll Bar im Fenster
        //  y: Die Y Position der Scroll Bar im Fenster
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  return: 1, falls die Nachricht verarbeitet wurde. Setzt dafür nicht den verarbeitet flag des Maus Ereignisses
        __declspec( dllexport ) virtual bool doMausMessage( int x, int y, int br, int hi, MausEreignis &me ) = 0;
        // Gibt zurück, ob sich die Scrollbar seit dem letzten Aufruf der Funktion verändert hat
        __declspec( dllexport ) bool getRend();
        // Zeichnet die Scrollbar
        //  x: Die X Position der Scroll Bar
        //  y: Die Y Position der Scroll Bar
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) virtual void render( int x, int y, int br, int hi, Bild &zRObj ) const = 0;
        // Gibt einen zeiger auf die Scroll Daten zurück
        __declspec( dllexport ) ScrollData *getScrollData() const;
        // Gibt die Scroll geschwindigkeit zurück. Die Anzahl der gescrollten Pixel pro Klick
        __declspec( dllexport ) int getKlickScroll() const;
        // Gibt die Fordergrundfarbe der Scrollbar zurück
        __declspec( dllexport ) int getFarbe() const;
        // Gibt die Hintergrundfarbe der Scrollbar zurück
        __declspec( dllexport ) int getBgFarbe() const;
        // Gibt die aktuelle Scroll Position zurück
        __declspec( dllexport ) int getScroll() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) ScrollBar *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual ScrollBar *release();
    };

    // Eine vertikale Scrollbar, wie sie in manchen 2D GUI Zeichnungen des Frameworks verwendet wird
    class VScrollBar : public ScrollBar
    {
    public:
        // Konstruktor 
        __declspec( dllexport ) VScrollBar();
		// Destruktor 
		__declspec( dllexport ) virtual ~VScrollBar();
        // Verarbeitet ein Maus Ereignis
        //  x: Die X Position der Scroll Bar im Fenster
        //  y: Die Y Position der Scroll Bar im Fenster
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  return: 1, falls die Nachricht verarbeitet wurde. Setzt dafür nicht den verarbeitet flag des Maus Ereignisses
        __declspec( dllexport ) bool doMausMessage( int x, int y, int br, int hi, MausEreignis &me ) override;
        // Zeichnet die Scrollbar
        //  x: Die X Position der Scroll Bar
        //  y: Die Y Position der Scroll Bar
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) void render( int x, int y, int br, int hi, Bild &zRObj ) const override;
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) ScrollBar *release() override;
    };

    // Eine horizontale Scrollbar, wie sie in manchen 2D GUI Zeichnungen des Frameworks verwendet wird
    class HScrollBar : public ScrollBar
    {
    public:
        // Konstruktor 
        __declspec( dllexport ) HScrollBar();
		// Destruktor 
		__declspec( dllexport ) virtual ~HScrollBar();
        // Verarbeitet ein Maus Ereignis
        //  x: Die X Position der Scroll Bar im Fenster
        //  y: Die Y Position der Scroll Bar im Fenster
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  return: 1, falls die Nachricht verarbeitet wurde. Setzt dafür nicht den verarbeitet flag des Maus Ereignisses
        __declspec( dllexport ) bool doMausMessage( int x, int y, int br, int hi, MausEreignis &me ) override;
        // Zeichnet die Scrollbar
        //  x: Die X Position der Scroll Bar
        //  y: Die Y Position der Scroll Bar
        //  br: Die Breite der Scroll Bar
        //  hi: Die Höhe der Scroll Bar
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) void render( int x, int y, int br, int hi, Bild &zRObj ) const override;
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) ScrollBar *release() override;
    };
}

#endif