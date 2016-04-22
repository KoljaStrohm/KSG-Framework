#ifndef RenderThread_H
#define RenderThread_H

#include "Thread.h"

namespace Framework
{
    class Bildschirm; // Bildschirm.h
    class ZeitMesser; // ZeitMesser.h
    class Bild; // Bild.h

    // Ein Thread, der ein Bildschirm verwaltet. Er ruft die render() und tick() funktionen automatisch auf
    class RenderTh : private Thread
    {
    private:
        bool stoppen;
        Bildschirm *bildschirm;
        ZeitMesser *zeit;
        double renderTickZeit;
        void *renderParameter;
        void *tickParameter;
        void( *renderFunktion )( void *, void *, Bild * );
        void( *tickFunktion )( void *, void *, double );
        bool pause;
        CRITICAL_SECTION cs;
        int maxFps;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) RenderTh();
        // Destruktor 
        __declspec( dllexport ) ~RenderTh();
        // Dies ist notwendig, falls mehrere Threads gleichzeitig das Objekt benutzen.
        // Wenn lock() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlock() aufgerufen hat.
        __declspec( dllexport ) void lock();
        // Dies ist notwendig, falls mehrere Threads gleichzeitig das Objekt benutzen.
        // Wenn lock() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlock() aufgerufen hat.
        __declspec( dllexport ) void unlock();
        // Setzt das Bildschirm Objekt, das verwaltet werden soll
        //  bildschirm: Der Bildschirm
        __declspec( dllexport ) void setBildschirm( Bildschirm *bildschirm );
        // Die Funktion, die automatisch in einem neuen Thread ausgeführt wird
        __declspec( dllexport ) void thread() override;
        // Beginnt den Render Thread
        __declspec( dllexport ) void beginn();
        // Beendet den Render Thread
        __declspec( dllexport ) void beenden();
        // Setzt die maximalen Bilder pro Sekunde
        //  fps: maximale Bilder pro Sekunde
        __declspec( dllexport ) void setMaxFps( int fps );
        // Pausiert den Render Thread
        //  p: 1, wenn der Thread pausiert werden soll
        __declspec( dllexport ) void setPause( bool p );
        // Setzt die Rückruffunktion, die immer vor dem Zeichnen aufgerufen wird
        //  rF: Die Rückruffunktion
        __declspec( dllexport ) void setRenderFunktion( void( *rF )( void *, void *, Bild * ) );
        // Setzt die Rückruffunktion, die immer vor dem Aktualisieren aller Objekte aufgerufen wird
        //  rF: Die Rückruffunktion
        __declspec( dllexport ) void setTickFunktion( void( *tF )( void *, void *, double ) );
        // Setzt den Parameter der Rückruffunktion, die immer vor dem Zeichnen aufgerufen wird
        //  p: Der Parameter
        __declspec( dllexport ) void setRenderFunktionParameter( void *p );
        // Setzt den Parameter der Rückruffunktion, die immer vor dem Aktualisieren aller Objekte aufgerufen wird
        __declspec( dllexport ) void setTickFunktionParameter( void *p );
        // Gibt den Bildschirm zurück, der von dem Thread verwaltet wird
        __declspec( dllexport ) Bildschirm *getBildschirm() const;
        // Gibt den Bildschirm ohne erhöhten Reference Counter zurück, der von dem Thread verwaltet wird
        __declspec( dllexport ) Bildschirm *zBildschirm() const;
        // Gibt die Zeit zurück, mit der die tick() Funktion des Bildschirms als letztes aufgerufen wurde
        __declspec( dllexport ) double getRenderTickZeit() const;
        // Erhöht den Reference Counter um 1
        //  Return: Ein zeiger auf diesen Shader
        __declspec( dllexport ) RenderTh *getThis();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        __declspec( dllexport ) RenderTh *release();
    };
}

#endif