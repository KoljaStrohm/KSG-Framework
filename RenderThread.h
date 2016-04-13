#ifndef RenderThread_H
#define RenderThread_H

#include "Thread.h"

namespace Framework
{
	class Bildschirm; // Bildschirm.h
	class ZeitMesser; // ZeitMesser.h
	class Bild; // Bild.h

	class RenderTh : private Thread
	{
	private:
		bool stoppen;
		Bildschirm *bildschirm;
		ZeitMesser *zeit;
		double renderTickZeit;
		void *renderParameter;
		void *tickParameter;
		void ( *renderFunktion )( void *, void *, Bild * );
		void ( *tickFunktion )( void *, void *, double );
		bool pause;
		CRITICAL_SECTION cs;
		int maxFps;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) RenderTh();
		// Destruktor 
		__declspec( dllexport ) ~RenderTh();
		// nicht constant 
		__declspec( dllexport ) void lock();
		__declspec( dllexport ) void unlock();
		__declspec( dllexport ) void setBildschirm( Bildschirm *bildschirm ); // setzt den Bildschirm
		__declspec( dllexport ) void thread() override; // Render Schleife
		__declspec( dllexport ) void beginn(); // beginnt rendering
		__declspec( dllexport ) void beenden(); // beendet den Thread
		__declspec( dllexport ) void setMaxFps( int fps ); // setzt die Anzahl der Bilder pro Sekunde
		__declspec( dllexport ) void setPause( bool p ); // Renderpause
		__declspec( dllexport ) void setRenderFunktion( void ( *rF )( void *, void *, Bild * ) ); // setzt die Rückruf Funktion beim Rendern
		__declspec( dllexport ) void setTickFunktion( void ( *tF )( void *, void *, double ) ); // setzt die Rückruf Funktion beim Tick
		__declspec( dllexport ) void setRenderFunktionParameter( void *p ); // setzt den Parameter der Rückruf Funktion beim Rendern
		__declspec( dllexport ) void setTickFunktionParameter( void *p ); // setzt den Parameter der Rückruf Funktion beim Tick
		// constant
		__declspec( dllexport ) Bildschirm *getBildschirm() const; // gibt den Bildschirm zurück
		__declspec( dllexport ) Bildschirm *zBildschirm() const;
		__declspec( dllexport ) double getRenderTickZeit() const; // gibt die Zeit zurück, die zum Rendern und zum Tick benötigt wird
		// Reference Counting
		__declspec( dllexport ) RenderTh *getThis();
		__declspec( dllexport ) RenderTh *release();
	};
}

#endif