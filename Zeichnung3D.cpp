#include "Zeichnung3D.h"

using namespace Framework;
// Inhalt der Zeichnung3D Klasse

// Konstruktor
Zeichnung3D::Zeichnung3D()
{
    welt = welt.identity();
    pos = Vec3< float >( 0, 0, 0 );
    angle = Vec3< float >( 0, 0, 0 );
    rend = 0;
    alpha = 0;
    radius = 0;
}

// Setzt die Position der Zeichnung in der Welt
//  p: Die Position
void Zeichnung3D::setPosition( Vec3< float > &p )
{
    pos = p;
    rend = 1;
}

// Setzt die Position der Zeichnung in der Welt
//  x: Die x Position
//  y: Die y Position
//  z: Die z Position
void Zeichnung3D::setPosition( float x, float y, float z )
{
    pos.x = x;
    pos.y = y;
    pos.z = z;
    rend = 1;
}

// Setzt die Position der Zeichnung in der Welt
//  x: Die x Position
void Zeichnung3D::setX( float x )
{
    pos.x = x;
    rend = 1;
}

// Setzt die Position der Zeichnung in der Welt
//  y: Die y Position
void Zeichnung3D::setY( float y )
{
    pos.y = y;
    rend = 1;
}

// Setzt die Position der Zeichnung in der Welt
//  z: Die z Position
void Zeichnung3D::setZ( float z )
{
    pos.z = z;
    rend = 1;
}

// Setzt die Drehung der Zeichnung in der Welt
//  d: Die drehung um die x, y und z achse
void Zeichnung3D::setDrehung( Vec3< float > &d )
{
    angle = d;
    rend = 1;
}

// Setzt die Drehung der Zeichnung in der Welt
//  xWinkel: Die drehung um die x achse
//  yWinkel: Die drehung um die y achse
//  zWinkel: Die drehung um die z achse
void Zeichnung3D::setDrehung( float xWinkel, float yWinkel, float zWinkel )
{
    angle.x = xWinkel;
    angle.y = yWinkel;
    angle.z = zWinkel;
    rend = 1;
}

// Setzt die Drehung der Zeichnung in der Welt
//  winkel: Die drehung um die x achse
void Zeichnung3D::setDrehungX( float winkel )
{
    angle.x = winkel;
    rend = 1;
}

// Setzt die Drehung der Zeichnung in der Welt
//  winkel: Die drehung um die y achse
void Zeichnung3D::setDrehungY( float winkel )
{
    angle.y = winkel;
    rend = 1;
}

// Setzt die Drehung der Zeichnung in der Welt
//  winkel: Die drehung um die z achse
void Zeichnung3D::setDrehungZ( float winkel )
{
    angle.z = winkel;
    rend = 1;
}

// Legt fest, ob das Objekt teilweise oder ganz transparente stellen enthält
//  a: true, wenn teilweise oder ganz transparente stellen vorhanden sind
void Zeichnung3D::setAlpha( bool a )
{
    alpha = a;
    rend = 1;
}

// Errechnet die Matrizen aller Knochen des Skeletts der Zeichnung
//  viewProj: Die miteinander multiplizierten Kameramatrizen
//  matBuffer: Ein Array mit Matrizen, der gefüllt werden soll
//  return: Die Anzahl der Matrizen, die die Zeichnung benötigt
int Zeichnung3D::errechneMatrizen( Mat4< float > &viewProj, Mat4< float > *matBuffer )
{
    matBuffer[ 0 ] = welt * viewProj;
    return 1;
}

// Verarbeitet ein Mausereignis
//  me: Das Mausereignis, das verarbeitet werden soll
void Zeichnung3D::doMausEreignis( MausEreignis &me )
{}

// Verarbeitet ein Tastaturereignis
//  te: das Tastaturereignis, das verarbeitet werden soll
void Zeichnung3D::doTastaturEreignis( TastaturEreignis &te )
{}

// Verarbeitet die vergangene Zeit
//  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
//  return: true, wenn sich das Objekt verändert hat, false sonnst.
bool Zeichnung3D::tick( double tickval )
{
    if( rend )
    {
        welt = welt.translation( pos ) * welt.rotationZ( angle.z ) * welt.rotationX( angle.x ) * welt.rotationY( angle.y );
        rend = 0;
        return 1;
    }
    return 0;
}

// Zeichnet das Objekt
//  zRObj: Ein Zeiger auf das Objekt, das zum Zeichnen verwendet werden soll (ohne erhöhten Reference Counter)
void Zeichnung3D::render( Render3D *zRObj )
{}

// Gibt zurück, ob das Objekt teilweise oder ganz transparente stellen enthält
bool Zeichnung3D::hatAlpha() const
{
    return alpha;
}

// Gibt den radius einer Kugel zurück, die das gesammte Model umschließt
float Zeichnung3D::getRadius() const
{
    return radius;
}

// Gibt einen Punkt zurück, der die Position der Zeichnung in der Welt darstellt
const Vec3< float > &Zeichnung3D::getPos() const
{
    return pos;
}

// Gibt die X Position der Zeichnung in der Welt zurück
float Zeichnung3D::getX() const
{
    return pos.x;
}

// Gibt die Y Position der Zeichnung in der Welt zurück
float Zeichnung3D::getY() const
{
    return pos.y;
}

// Gibt die Z Position der Zeichnung in der Welt zurück
float Zeichnung3D::getZ() const
{
    return pos.z;
}

// Gibt einen Vektor zurück, der die drehung der Zeichnung in der Welt darstellt. x ist die Drehung um die X Achse im Bogenmaß usw
const Vec3< float > &Zeichnung3D::getDrehung() const
{
    return angle;
}

// Gibt die Drehung um die X Achse im Bogenmaß zurück
float Zeichnung3D::getXDrehung() const
{
    return angle.x;
}

// Gibt die Drehung um die Y Achse im Bogenmaß zurück
float Zeichnung3D::getYDrehung() const
{
    return angle.y;
}

// Gibt die Drehung um die Z Achse im Bogenmaß zurück
float Zeichnung3D::getZDrehung() const
{
    return angle.z;
}

// Gibt die Matrix zurück, die die Zeichnung in den Welt Raum übersetzt
const Mat4< float > &Zeichnung3D::getMatrix() const
{
    return welt;
}