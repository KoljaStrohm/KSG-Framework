#ifndef Model2D_H
#define Model2D_H

#ifdef WIN32
#include "Zeichnung.h"
#endif
#include "Punkt.h"
#include "Array.h"
#include "DreieckListe.h"
#include "Vec3.h"

namespace Framework
{
	typedef Vec2< float > Vertex;
	class Bild;

	// Model2D Klasse löscht die Zeiger
	struct Polygon2D
	{
		Array< Vertex > *vertex;
		Array< Punkt > *tKordinaten;
	};

	class Model2DData
	{
	private:
		RCArray< Array< Punkt > > outList;
		int ref;

		// privat
		bool istPunktInnen( Vertex p, int polygonId = -1 ) const;
		bool istLinieInnen( Vertex a, Vertex b, int polygonId = -1 ) const;

	public:
		Array< Polygon2D > *polygons;
		RCArray< RCArray< DreieckListe< Vertex > > > *vListen;
		Punkt minP, maxP;

		// Konstruktor
		__declspec( dllexport ) Model2DData();
		// Destruktor
		__declspec( dllexport ) ~Model2DData();
		// nicht constant
		__declspec( dllexport ) bool erstelleModell( Array< Polygon2D > *polygons );
		__declspec( dllexport ) void löscheModell(); // setzt die Vertex daten zurück
		// Reference Counting
		__declspec( dllexport ) Model2DData *getThis();
		__declspec( dllexport ) Model2DData *release();
	};

#ifdef WIN32
	class Model2D : public Zeichnung
#else
	class Model2D
#endif
	{
    public:
#ifdef WIN32
        class Style : public Zeichnung::Style
        {
        public:
            const static __int64 Textur = 0x8;
            const static __int64 Rahmen = 0x10;
            const static __int64 Alpha = 0x40;
            const static __int64 Mesh = 0x20;
        };
#endif
	private:
		Model2DData *rData;
		float drehung;
		float größe;
		int ref;
#ifdef WIN32
		int farbe;
		Bild *textur;
#else
		Punkt pos;
#endif

	public:
		// Konstruktor
		__declspec( dllexport ) Model2D();
		// Destruktor
		__declspec( dllexport ) ~Model2D();
		// nicht constant
		__declspec( dllexport ) void setModel( Model2DData *mdl );
		__declspec( dllexport ) void setDrehung( float drehung );
		__declspec( dllexport ) void addDrehung( float drehung );
		__declspec( dllexport ) void setGröße( float größe );
		__declspec( dllexport ) void addGröße( float größe );
#ifdef WIN32
		__declspec( dllexport ) void setTextur( Bild *t );
		__declspec( dllexport ) void setFarbe( int f );
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
		__declspec( dllexport ) bool tick( double tickVal ) override;
		__declspec( dllexport ) void render( Bild &zRObj ) override;
#else
		__declspec( dllexport ) void setPosition( Punkt p );
#endif
		// constant
		__declspec( dllexport ) float getDrehung() const;
		__declspec( dllexport ) float getGröße() const;
		__declspec( dllexport ) bool istPunktInnen( Vertex p ) const;
		__declspec( dllexport ) bool istLinieInnen( Vertex a, Vertex b ) const;
		__declspec( dllexport ) bool istModelInnen( const Model2D *zMdl, bool end = 0 ) const;
		__declspec( dllexport ) Model2DData *getModel() const;
		__declspec( dllexport ) Model2DData *zModel() const;
#ifndef WIN32
		Punkt getPosition() const;
#endif
		// Reference Counting
		__declspec( dllexport ) Model2D *getThis();
		__declspec( dllexport ) Model2D *release();
	};
}

#endif