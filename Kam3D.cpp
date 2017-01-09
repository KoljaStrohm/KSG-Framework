#include "Kam3D.h"
#include "Welt3D.h"
#include "Render3D.h"
#include "Shader.h"
#include "TastaturEreignis.h"
#include "Globals.h"
#include "MausEreignis.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace Framework;

// Inhalt der Kam3D Klasse
// Konstruktor
Kam3D::Kam3D()
{
    openingAngle = (float)PI / 4;
    minZ = 0.1f;
    maxZ = 5000;
    pos = Vec3< float >( 0, 0, -1000 );
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    viewport = new D3D11_VIEWPORT();
    viewport->TopLeftY = 0;
    viewport->TopLeftX = 0;
    viewport->MinDepth = 0.f;
    viewport->MaxDepth = 1.f;
    viewport->Width = 200;
    viewport->Height = 200;

    welt = 0;

    ref = 1;

    updateMatrix();
}

// Destruktor
Kam3D::~Kam3D()
{
    if( welt )
        welt->release();
    delete viewport;
}

// private

// Aktualisiert die view und projektion matrizen
void Kam3D::updateMatrix()
{
    view = view.rotationX( -rotX ) * view.rotationY( -rotY ) * view.rotationZ( -rotZ ) * view.translation( Vec3< float >( -pos.x, -pos.y, -pos.z ) );
    proj = proj.projektion( openingAngle, viewport->Width / viewport->Height, minZ, maxZ );
}

// Setzt die Position der Kamera in der 3D Welt
void Kam3D::setPosition( Vec3< float > pos )
{
    this->pos = pos;
    updateMatrix();
}

// zoomt heran, indem sich die Kamera etwas auf das Blickziel zubewegt
//  val: Die länge der Strecke um die sich die Kamera bewegen soll
void Kam3D::scrollIn( float val )
{
    Vec3< float > n( 0, 0, 1 );
    Mat4< float > tmp = tmp.rotationY( rotY ) * tmp.rotationX( rotX ) * tmp.rotationZ( rotZ );
    n = tmp * n * val;
    pos += n;
}

// zppmt heraus, indem sich die Kamera etwas von dem Blockziel entfernt
//  val: Die länge der Strecke um die sich die Kamera bewegen soll
void Kam3D::scrollOut( float val )
{
    Vec3< float > n( 0, 0, 1 );
    Mat4< float > tmp = tmp.rotationY( rotY ) * tmp.rotationX( rotX ) * tmp.rotationZ( rotZ );
    n = tmp * n * val;
    pos -= n;
}

// Richtet die Kamera so aus, dass sie genau auf einen bestimmten Punkt zeigt
//  ziel: Der Punkt, auf den die Kamera zeigen soll
void Kam3D::setAusrichtung( Vec3< float > ziel )
{

}

// Setzt die Position des Bildes auf dem Bildschirm
//  p: Ein Punkt mit x und y Koordinaten in Pixeln
void Kam3D::setBildschirmPosition( Punkt p )
{
    viewport->TopLeftX = (float)p.x;
    viewport->TopLeftY = (float)p.y;
}

// Setzt die Position des Bildes auf dem Bildschirm
//  x: Die x Koordinate in Pixeln
//  y: Die y Koordinate in Pixeln
void Kam3D::setBildschirmPosition( int x, int y )
{
    viewport->TopLeftX = (float)x;
    viewport->TopLeftY = (float)y;
}

// Setzt die Größe des Bildes auf dem Bildschirm
//  p: Ein Punkt, mit x als Breite und y als Höhe in Pixlen
void Kam3D::setBildschirmSize( Punkt p )
{
    viewport->Width = (float)p.x;
    viewport->Height = (float)p.y;
    updateMatrix();
}

// Setzt die Größe des Bildes auf dem Bildschirm
//  br: Die Breite in Pixeln
//  hö: Die Höhe in Pixeln
void Kam3D::setBildschirmSize( int br, int hö )
{
    viewport->Width = (float)br;
    viewport->Height = (float)hö;
    updateMatrix();
}

// Setzt die Welt, die gezeichnet werden soll
//  w: Die Welt
void Kam3D::setWelt( Welt3D *w )
{
    if( welt )
        welt->release();
    welt = w;
}

// Verarbeitet die vergangene Zeit
//  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
//  return: true, wenn sich das Bild neu gezeichnet werden muss, false sonnst.
bool Kam3D::tick( double tv )
{
    if( getTastenStand( T_Oben ) )
        rotX -= (float)tv;
    if( getTastenStand( T_Unten ) )
        rotX += (float)tv;
    if( getTastenStand( T_Links ) )
        rotY -= (float)tv;
    if( getTastenStand( T_Rechts ) )
        rotY += (float)tv;
    Vec3< float > n( 0, 0, 1 );
    Vec3< float > n2( 1, 0, 0 );
    Vec3< float > n3( 0, 1, 0 );
    Mat4< float > tmp = tmp.rotationY( rotY ) * tmp.rotationX( rotX );
    n = tmp * n;
    n = n * (float)tv * 60;
    n2 = tmp * n2;
    n2 = n2 * (float)tv * 60;
    n3 = tmp * n3;
    n3 = n3 * (float)tv * 60;
    if( getTastenStand( 'w' ) )
        pos += n;
    if( getTastenStand( 's' ) )
        pos -= n;
    if( getTastenStand( 'd' ) )
        pos += n2;
    if( getTastenStand( 'a' ) )
        pos -= n2;
    if( getTastenStand( ' ' ) )
        pos += n3;
    if( getTastenStand( T_Shift ) )
        pos -= n3;
    updateMatrix();
    if( welt )
        return welt->tick( tv );
    return 0;
}

// Verarbeitet ein Mausereignis
//  me: Das Mausereignis, das verarbeitet werden soll
void Kam3D::doMausEreignis( MausEreignis &me )
{
    if( me.verarbeitet )
        return;
    if( me.mx > viewport->TopLeftX && me.my > viewport->TopLeftY && me.mx < viewport->TopLeftX + viewport->Width && me.my < viewport->TopLeftY + viewport->Height )
    {
        MausEreignis3D me3d;
        me3d.id = me.id;
        me3d.verarbeitet = me.verarbeitet;
        Vec3< float > mausP = Vec3< float >( ( me.mx - viewport->TopLeftX ) / ( 0.5f * viewport->Width ) - 1, ( me.my - viewport->TopLeftY ) / ( 0.5f * viewport->Height ) - 1, 0 );
        Vec3< float > mausT = Vec3< float >( mausP.x, mausP.y, 1 );
        Mat4< float > mat = proj * view;
        mat = mat.getInverse();
        mausP = mat * mausP;
        mausT = mat * mausT;
        me3d.pos = mausP;
        me3d.dir = mausT - mausP;
        me.verarbeitet = 1;
    }
}

// Verarbeitet ein Tastaturereignis
//  te: das Tastaturereignis, das verarbeitet werden soll
void Kam3D::doTastaturEreignis( TastaturEreignis &te )
{

}

// Zeichnet den Auschnitt der Welt, den die Kamera filmt
//  zRObj: Das Render Objekt, mit dem gezeichnet werden soll
void Kam3D::render( Render3D *zRObj )
{
    zRObj->zContext()->RSSetViewports( 1, viewport );
    zRObj->setKameraMatrix( view, proj, pos );
    if( welt )
        welt->render( zRObj );
}

// Erhöht den Reference Counter um 1
//  Return: Ein zeiger auf diesen Shader
Kam3D *Kam3D::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
//  Return: 0
Kam3D *Kam3D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}