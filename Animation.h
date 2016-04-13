#ifndef Animation_H
#define Animation_H

#include "Zeichnung.h"

namespace Framework
{
	class Bild; // Bild.h
	class LTDBDatei; // DateiSystem.h
	class InitDatei; // InitDatei.h
	class LRahmen; // Rahmen.h

	class Animation2DData
	{
	private:
		Bild **bilder;
		int bildAnzahl;
		int fps;
		bool wiederhohlen;
		bool transparent;
		CRITICAL_SECTION cs;
		int ref;

	public:
		// Konstruktor
		__declspec( dllexport ) Animation2DData();
		// Destruktor
		__declspec( dllexport ) ~Animation2DData();
		// nicht constant
		__declspec( dllexport ) void lock();
		__declspec( dllexport ) void unlock();
		__declspec( dllexport ) void ladeAnimation( InitDatei *datei );
		__declspec( dllexport ) void ladeAnimation( LTDBDatei *datei );
		__declspec( dllexport ) void setFPS( int fps );
		__declspec( dllexport ) void setWiederhohlend( bool wh );
		__declspec( dllexport ) void setTransparent( bool trp );
		__declspec( dllexport ) void reset();
		// constant
		__declspec( dllexport ) Bild *getBild( int i ) const;
		__declspec( dllexport ) Bild *zBild( int i ) const;
		__declspec( dllexport ) int getBildAnzahl() const;
		__declspec( dllexport ) int getFPS() const;
		__declspec( dllexport ) bool istWiederhohlend() const;
		__declspec( dllexport ) bool istTransparent() const;
		// Reference Counting
		__declspec( dllexport ) Animation2DData *getThis();
		__declspec( dllexport ) Animation2DData *release();
	};

	class Animation2D : public Zeichnung
	{
	private:
		Animation2DData *data;
		int jetzt;
		double ausgleich;
		unsigned char alpha;
		unsigned char maxAlpha;
		bool rahmen;
		LRahmen *ram;
		int aps;
		bool sichtbar;
		bool rend;
		int ref;

	public:
		// Konstruktor
		__declspec( dllexport ) Animation2D();
		// Destruktor
		__declspec( dllexport ) ~Animation2D();
		// nicht constant
		__declspec( dllexport ) void setRahmen( bool ram );
		__declspec( dllexport ) void setRahmenZ( LRahmen *ram );
		__declspec( dllexport ) void setRahmenBreite( int br );
		__declspec( dllexport ) void setRahmenFarbe( int fc );
		__declspec( dllexport ) void setAnimationDataZ( Animation2DData *data );
		__declspec( dllexport ) void setAlphaMaske( unsigned char alpha );
		__declspec( dllexport ) void setAPS( int aps );
		__declspec( dllexport ) void setSichtbar( bool sichtbar );
		__declspec( dllexport ) bool tick( double zeit ) override;
		__declspec( dllexport ) void render( Bild &zRObj ) override;
		// constant
		__declspec( dllexport ) Animation2DData *getAnimationData() const;
		__declspec( dllexport ) Animation2DData *zAnimationData() const;
		__declspec( dllexport ) bool istSichtbar() const;
		__declspec( dllexport ) int getJetzt() const;
		__declspec( dllexport ) unsigned char getAlphaMaske() const;
		__declspec( dllexport ) bool hatRahmen() const;
		__declspec( dllexport ) LRahmen *getRahmen() const;
		__declspec( dllexport ) LRahmen *zRahmen() const;
		__declspec( dllexport ) int getRahmenBreite() const;
		__declspec( dllexport ) int getRahmenFarbe() const;
		__declspec( dllexport ) Zeichnung *dublizieren() const override;
		// Reference Counting
		__declspec( dllexport ) Animation2D *getThis();
		__declspec( dllexport ) Animation2D *release();
	};
}

#endif