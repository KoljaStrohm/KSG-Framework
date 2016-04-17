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

    // Eine Polygon Struktur, die von Model2D verwendet wird
	// Model2D Klasse löscht die Zeiger
	struct Polygon2D
	{
		Array< Vertex > *vertex;
		Array< Punkt > *tKordinaten;
	};

    // Die Daten für ein 2D Model
	class Model2DData
	{
	private:
		RCArray< Array< Punkt > > outList;
		int ref;

		// Überprüft, ob ein Punkt im Model ist
        //  p: Der Punkt
        //  polygonId: Die Id des Polygons, das geprüft werden soll
		bool istPunktInnen( Vertex p, int polygonId = -1 ) const;
        // Überprüft, ob eine Linie innerhalb des Models ist
        //  a: Der Startpunkt der Linie
        //  b: Der Endpunkt der Linie
		bool istLinieInnen( Vertex a, Vertex b, int polygonId = -1 ) const;

	public:
		Array< Polygon2D > *polygons;
		RCArray< RCArray< DreieckListe< Vertex > > > *vListen;
		Punkt minP, maxP;

		// Konstruktor
		__declspec( dllexport ) Model2DData();
		// Destruktor
		__declspec( dllexport ) ~Model2DData();
		// Erstellt die Dreieck Listen anhand aller gegebenen Eckpunkte der Polygons
        //  polygons: Ein Array von Polygons
        //  return: gibt immer 1 zurück
		__declspec( dllexport ) bool erstelleModell( Array< Polygon2D > *polygons );
        // Löscht die erstellten Dreiecklisten und die Eckpunkte
		__declspec( dllexport ) void löscheModell();
        // Erhöht den Reference Counting Zähler.
        //  return: this.
		__declspec( dllexport ) Model2DData *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
		__declspec( dllexport ) Model2DData *release();
	};

#ifdef WIN32
    // Eine Zeichnung von einem Model
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
            const static __int64 Textur = 0x8; // Wenn dieser Flag gesetzt ist, wird beim Zeichnen eine Textur verwendet
            const static __int64 Rahmen = 0x10; // Wenn dieser Flag gesetzt ist, werden die Render der Polygone gezeichnet
            const static __int64 Alpha = 0x40; // Wenn dieser Fag gesetzt ist, wird beim Zeichnen alphablending verwendet
            const static __int64 Mesh = 0x20; // Wenn dieser Flag gesetzt ist, werden die Render der Dreiecke gezeichnet
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
		// Setzt die Daten des Models
        //  mdl: Die Model Daten
		__declspec( dllexport ) void setModel( Model2DData *mdl );
        // Setzt die Drehung des Models gegen den Uhrzeigersinn
        //  drehung: Der winkel in Bogenmas
		__declspec( dllexport ) void setDrehung( float drehung );
        // Fügt zum aktuellen Drehungswinkel etwas hinzu
        //  drehung: Der Winkel in Bogenmas, der hinzugefügt werden soll
		__declspec( dllexport ) void addDrehung( float drehung );
        // Setzt die Skallierung des Modells
        //  größe: Der Faktor, mit dem Skalliert wird
		__declspec( dllexport ) void setGröße( float größe );
        // Addiert zur Skallierung einen bestimmten Wert hinzu
        //  größe: Der Wert, der zur skallierung hinzugefügt werden soll
		__declspec( dllexport ) void addGröße( float größe );
#ifdef WIN32
        // Setzt die Textur
        //  t: Das Bild, das als Textur verwendet werden soll
		__declspec( dllexport ) void setTextur( Bild *t );
        // Setzt die Farbe
        //  f: Die Farbe im A8R8G8B8 Format
		__declspec( dllexport ) void setFarbe( int f );
        // Verarbeitet ein Maus Ereignis. Wird vom Framework automatisch aufgerufen.
        //  me: Das Ereignis
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Verarbeitet die Zeit, die seit dem letzten aufruf dieser Funktion vergangen ist
        //  tickVal: Die vergangene Zeit in Sekunden
		__declspec( dllexport ) bool tick( double tickVal ) override;
        // Zeichnet die Zeihnung in ein bestimmtes Bild
        //  zRObj: Das Bild, in das gezeichnet werden soll
		__declspec( dllexport ) void render( Bild &zRObj ) override;
#else
        // Setzt die Position
        //  p: Die neue Position
		__declspec( dllexport ) void setPosition( Punkt p );
#endif
		// gibt die Drehung des Models zurück
		__declspec( dllexport ) float getDrehung() const;
        // gibt den Skallierungs Faktor zurück
		__declspec( dllexport ) float getGröße() const;
        // Gibt zurück, ob ein Punkt in dem Model enthalten ist
		//  p: Der Punkt
        __declspec( dllexport ) bool istPunktInnen( Vertex p ) const;
        // Überprüft, ob eine Linie im Model enthalten ist
        //  a: Der startpunkt der Linie
        //  b: Der endpunkt der Linie
		__declspec( dllexport ) bool istLinieInnen( Vertex a, Vertex b ) const;
        // Überprüft, ob sich das Model mit einem anderen Überschneidet
        //  zMdl: Ein Zeiger auf das andere Model ohne erhöhten Reference Counter
        //  end: 0, falls alle Ecken beider Modele überprüft werdden sollen. 1, falls nur die Punkte dieses Models im anderen gesucht werden sollen
		__declspec( dllexport ) bool istModelInnen( const Model2D *zMdl, bool end = 0 ) const;
        // Gibt die Model Daten zurück
		__declspec( dllexport ) Model2DData *getModel() const;
        // Gibt die Model Daten ohne erhöhten Reference Counter zurück
		__declspec( dllexport ) Model2DData *zModel() const;
#ifndef WIN32
        // Gibt die Position des Models zurück
		Punkt getPosition() const;
#endif
        // Erhöht den Reference Counting Zähler.
        //  return: this.
		__declspec( dllexport ) Model2D *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
		__declspec( dllexport ) Model2D *release();
	};
}

#endif