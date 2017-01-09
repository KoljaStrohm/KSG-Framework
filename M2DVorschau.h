#pragma once

#include "Zeichnung.h"

namespace Framework
{
    class Model2D; // Model2D.h
    class Model2DData; // Model2D.h
    class LRahmen; // Rahmen.h
    class AlphaFeld; // AlphaFeld.h
    struct MausEreignis; // MausEreignis.h

    // Eine Zeichnung des 2D GUI Frameworkd, die ein 2D Modell anzeigt
    class M2DVorschau : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 UsrScale = 0x001000; // Wenn dieser Style gesetzt ist, kann der Benutzer das Model Skallieren, indem er scrollt
            const static __int64 UsrMove = 0x002000; // Wenn dieser Style gesetzt ist, kann der Benutzer das Model per Drag and Drop verschieben
            const static __int64 UsrRot = 0x004000; // Wenn dieser Style gesetzt idt, kann der Benutzer das Model mit der rechten Maustaste drehen
        };

    private:
        Model2D *mdl;
        LRahmen *ram;
        AlphaFeld *af;
        int bgF;
        int mx;
        int my;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) M2DVorschau();
        // Destruktor
        __declspec( dllexport ) virtual ~M2DVorschau();
        // Setzt einen Zeiger auf das Model, das angezeigt werden soll
        //  mdl: Das Model
        __declspec( dllexport ) void setModel2DZ( Model2D *mdl );
        // Setzt die Daten des Models, das angezeigt werden soll
        //  mdl: Die Daten
        __declspec( dllexport ) void setModel2D( Model2DData *mdl );
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Aktualisiert das Objekt. Wird vom Framework aufgerufen
        //  tickVal: Die Zeit in sekunden, die seit dem lezten Aufruf dieser Funktion vergangen ist
        //  return: 1, wenn sich etwas verändert hat und das Bild neu gezeichnet werden muss. 0 sonst
        __declspec( dllexport ) bool tick( double tv ) override;
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &rb ) override;
        // Gibt das angezeigte Model ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Model2D *zModel() const;
        // Gibt das angezeigte Model zurück
        __declspec( dllexport ) Model2D *getModel() const;
        // Erzeugt eine Komplette Kopie der Zeichnung, die ohne auswirkungen auf die Originale verändert werden kann
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) M2DVorschau *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) M2DVorschau *release();
    };
}