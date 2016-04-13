#pragma once

#include "Mat4.h"
#include "Zeichnung3D.h"
#include "Punkt.h"

// DirectX 11 Types

struct D3D11_VIEWPORT;

namespace Framework
{
    class Render3D; // Render3D.h
    class Welt3D; // Welt3D.h

    class Kam3D
    {
    private:
        Mat4< float > view;
        Mat4< float > proj;

        float öffnungswinkel;
        float minZ;
        float maxZ;
        Vec3< float > pos;
        float rotX;
        float rotY;
        float rotZ;

        D3D11_VIEWPORT *viewport;
        Welt3D *welt;
        int ref;

        // Aktualisiert die view und projektion matrizen
        void updateMatrix();

    public:
        // Konstruktor
        __declspec( dllexport ) Kam3D();
        // Destruktor
        __declspec( dllexport ) ~Kam3D();
        // Setzt die Position der Kamera in der 3D Welt
        __declspec( dllexport ) void setPosition( Vec3< float > pos );
        // zoomt heran, indem sich die Kamera etwas auf das Blickziel zubewegt
        //  val: Die länge der Strecke um die sich die Kamera bewegen soll
        __declspec( dllexport ) void scrollIn( float val );
        // zppmt heraus, indem sich die Kamera etwas von dem Blockziel entfernt
        //  val: Die länge der Strecke um die sich die Kamera bewegen soll
        __declspec( dllexport ) void scrollOut( float val );
        // Richtet die Kamera so aus, dass sie genau auf einen bestimmten Punkt zeigt
        //  ziel: Der Punkt, auf den die Kamera zeigen soll
        __declspec( dllexport ) void setAusrichtung( Vec3< float > ziel );
        // Setzt die Position des Bildes auf dem Bildschirm
        //  p: Ein Punkt mit x und y Koordinaten in Pixeln
        __declspec( dllexport ) void setBildschirmPosition( Punkt p );
        // Setzt die Position des Bildes auf dem Bildschirm
        //  x: Die x Koordinate in Pixeln
        //  y: Die y Koordinate in Pixeln
        __declspec( dllexport ) void setBildschirmPosition( int x, int y );
        // Setzt die Größe des Bildes auf dem Bildschirm
        //  p: Ein Punkt, mit x als Breite und y als Höhe in Pixlen
        __declspec( dllexport ) void setBildschirmGröße( Punkt p );
        // Setzt die Größe des Bildes auf dem Bildschirm
        //  br: Die Breite in Pixeln
        //  hö: Die Höhe in Pixeln
        __declspec( dllexport ) void setBildschirmGröße( int br, int hö );
        // Setzt die Welt, die gezeichnet werden soll
        //  w: Die Welt
        __declspec( dllexport ) void setWelt( Welt3D *w );
        // Verarbeitet die vergangene Zeit
        //  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
        //  return: true, wenn sich das Bild neu gezeichnet werden muss, false sonnst.
        __declspec( dllexport ) bool tick( double tv );
        // Verarbeitet ein Mausereignis
        //  me: Das Mausereignis, das verarbeitet werden soll
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me );
        // Verarbeitet ein Tastaturereignis
        //  te: das Tastaturereignis, das verarbeitet werden soll
        __declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te );
        // Zeichnet den Auschnitt der Welt, den die Kamera filmt
        //  zRObj: Das Render Objekt, mit dem gezeichnet werden soll
        __declspec( dllexport ) void render( Render3D *zRObj );
        // Erhöht den Reference Counter um 1
        //  Return: Ein zeiger auf diesen Shader
        __declspec( dllexport ) Kam3D *getThis();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        __declspec( dllexport ) Kam3D *release();
    };
}