#pragma once

#include "Mat4.h"

namespace Framework
{
    struct MausEreignis3D;
    struct TastaturEreignis;
    class Render3D; // Render3D.h

    // Ein Objekt, das von der Bildschirm3D Klasse gezeichnet werden kann.
    class Zeichnung3D
    {
    protected:
        Vec3< float > pos; // Position des Objekts
        Vec3< float > angle; // Drehungswinkel für x, y und z
        Mat4< float > welt; // Wlet translation matrix
        float radius; // Der radius einer Kugel, der das gesamte Objekt umschließt
        bool alpha; // Speichert ob das Objekt teilweise oder ganz transparente stellen enthält
        bool rend;

    public:
        // Konstruktor
        __declspec( dllexport ) Zeichnung3D();
        // Setzt die Position der Zeichnung in der Welt
        //  p: Die Position
        __declspec( dllexport ) void setPosition( Vec3< float > &p );
        // Setzt die Position der Zeichnung in der Welt
        //  x: Die x Position
        //  y: Die y Position
        //  z: Die z Position
        __declspec( dllexport ) void setPosition( float x, float y, float z );
        // Setzt die Position der Zeichnung in der Welt
        //  x: Die x Position
        __declspec( dllexport ) void setX( float x );
        // Setzt die Position der Zeichnung in der Welt
        //  y: Die y Position
        __declspec( dllexport ) void setY( float y );
        // Setzt die Position der Zeichnung in der Welt
        //  z: Die z Position
        __declspec( dllexport ) void setZ( float z );
        // Setzt die Drehung der Zeichnung in der Welt
        //  d: Die drehung um die x, y und z achse
        __declspec( dllexport ) void setDrehung( Vec3< float > &d );
        // Setzt die Drehung der Zeichnung in der Welt
        //  xWinkel: Die drehung um die x achse
        //  yWinkel: Die drehung um die y achse
        //  zWinkel: Die drehung um die z achse
        __declspec( dllexport ) void setDrehung( float xWinkel, float yWinkel, float zWinkel );
        // Setzt die Drehung der Zeichnung in der Welt
        //  winkel: Die drehung um die x achse
        __declspec( dllexport ) void setDrehungX( float winkel );
        // Setzt die Drehung der Zeichnung in der Welt
        //  winkel: Die drehung um die y achse
        __declspec( dllexport ) void setDrehungY( float winkel );
        // Setzt die Drehung der Zeichnung in der Welt
        //  winkel: Die drehung um die z achse
        __declspec( dllexport ) void setDrehungZ( float winkel );
        // Legt fest, ob das Objekt teilweise oder ganz transparente stellen enthält
        //  a: true, wenn teilweise oder ganz transparente stellen vorhanden sind
        __declspec( dllexport ) void setAlpha( bool a );
        // Errechnet die Matrizen aller Knochen des Skeletts der Zeichnung
        //  viewProj: Die miteinander multiplizierten Kameramatrizen
        //  matBuffer: Ein Array mit Matrizen, der gefüllt werden soll
        //  return: Die Anzahl der Matrizen, die die Zeichnung benötigt
        __declspec( dllexport ) virtual int errechneMatrizen( Mat4< float > &viewProj, Mat4< float > *matBuffer );
        // Verarbeitet ein Mausereignis
        //  me: Das Mausereignis, das verarbeitet werden soll
        __declspec( dllexport ) virtual void doMausEreignis( MausEreignis3D &me );
        // Verarbeitet ein Tastaturereignis
        //  te: das Tastaturereignis, das verarbeitet werden soll
        __declspec( dllexport ) virtual void doTastaturEreignis( TastaturEreignis &te );
        // Verarbeitet die vergangene Zeit
        //  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
        //  return: true, wenn sich das Objekt verändert hat, false sonnst.
        __declspec( dllexport ) virtual bool tick( double tickval );
        // Zeichnet das Objekt
        //  zRObj: Ein Zeiger auf das Objekt, das zum Zeichnen verwendet werden soll (ohne erhöhten Reference Counter)
        __declspec( dllexport ) virtual void render( Render3D *zRObj );
        // Gibt zurück, ob das Objekt teilweise oder ganz transparente stellen enthält
        __declspec( dllexport ) bool hatAlpha() const;
        // Gibt den radius einer Kugel zurück, die das gesammte Model umschließt
        __declspec( dllexport ) float getRadius() const;
        // Gibt einen Punkt zurück, der die Position der Zeichnung in der Welt darstellt
        __declspec( dllexport ) const Vec3< float > &getPos() const;
        // Gibt die X Position der Zeichnung in der Welt zurück
        __declspec( dllexport ) float getX() const;
        // Gibt die Y Position der Zeichnung in der Welt zurück
        __declspec( dllexport ) float getY() const;
        // Gibt die Z Position der Zeichnung in der Welt zurück
        __declspec( dllexport ) float getZ() const;
        // Gibt einen Vektor zurück, der die drehung der Zeichnung in der Welt darstellt. x ist die Drehung um die X Achse im Bogenmaß usw
        __declspec( dllexport ) const Vec3< float > &getDrehung() const;
        // Gibt die Drehung um die X Achse im Bogenmaß zurück
        __declspec( dllexport ) float getXDrehung() const;
        // Gibt die Drehung um die Y Achse im Bogenmaß zurück
        __declspec( dllexport ) float getYDrehung() const;
        // Gibt die Drehung um die Z Achse im Bogenmaß zurück
        __declspec( dllexport ) float getZDrehung() const;
        // Gibt die Matrix zurück, die die Zeichnung in den Welt Raum übersetzt
        __declspec( dllexport ) const Mat4< float > &getMatrix() const;
    };
}