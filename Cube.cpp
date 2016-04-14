#include "Cube.h"
#include "DXBuffer.h"
#include "Textur.h"
#include "Globals.h"
#include "Model3DList.h"
#include "TexturList.h"

using namespace Framework;

// Inhalt der Cube Klasse

// Konstruktor
//  size: Die Größe des Würfels
Cube::Cube( float size )
    : Model3D()
{
    if( m3dRegister->hatModel( Standart3DTypes::würfel ) )
        model = m3dRegister->getModel( Standart3DTypes::würfel );
    else
    {
        model = new Model3DData();
        m3dRegister->addModel( model->getThis(), Standart3DTypes::würfel );
        float stdSize = 100;
        float left, right, top, bottom;
        // Calculate the screen coordinates of the left side of the bitmap.
        left = (float)( ( stdSize / 2.0 ) * -1 );
        // Calculate the screen coordinates of the right side of the bitmap.
        right = left + (float)stdSize;
        // Calculate the screen coordinates of the top of the bitmap.
        top = (float)( stdSize / 2.0 );
        // Calculate the screen coordinates of the bottom of the bitmap.
        bottom = top - (float)stdSize;
        float front = -stdSize / 2;
        float back = front + stdSize;

        Vertex3D *vertecies = new Vertex3D[ 24 ];
        vertecies[ 0 ].pos = Vec3<float >( left, top, front );
        vertecies[ 0 ].tPos = Vec2< float >( 0.f, 0.f );
        vertecies[ 1 ].pos = Vec3<float >( right, top, front );
        vertecies[ 1 ].tPos = Vec2< float >( 1.f, 0.f );
        vertecies[ 2 ].pos = Vec3<float >( left, bottom, front );
        vertecies[ 2 ].tPos = Vec2< float >( 0.f, 1.f );
        vertecies[ 3 ].pos = Vec3<float >( right, bottom, front );
        vertecies[ 3 ].tPos = Vec2< float >( 1.f, 1.f );
        vertecies[ 4 ].pos = Vec3<float >( left, top, back );
        vertecies[ 4 ].tPos = Vec2< float >( 0.0f, 0.0f );
        vertecies[ 5 ].pos = Vec3<float >( right, top, back );
        vertecies[ 5 ].tPos = Vec2< float >( 1.0f, 0.0f );
        vertecies[ 6 ].pos = Vec3<float >( left, bottom, back );
        vertecies[ 6 ].tPos = Vec2< float >( 0.0f, 1.0f );
        vertecies[ 7 ].pos = Vec3<float >( right, bottom, back );
        vertecies[ 7 ].tPos = Vec2< float >( 1.0f, 1.0f );

        vertecies[ 8 ].pos = Vec3<float >( left, top, front );
        vertecies[ 8 ].tPos = Vec2< float >( 1.f, 0.f );
        vertecies[ 9 ].pos = Vec3<float >( right, top, front );
        vertecies[ 9 ].tPos = Vec2< float >( 0.f, 0.f );
        vertecies[ 10 ].pos = Vec3<float >( left, bottom, front );
        vertecies[ 10 ].tPos = Vec2< float >( 1.f, 1.f );
        vertecies[ 11 ].pos = Vec3<float >( right, bottom, front );
        vertecies[ 11 ].tPos = Vec2< float >( 0.f, 1.f );
        vertecies[ 12 ].pos = Vec3<float >( left, top, back );
        vertecies[ 12 ].tPos = Vec2< float >( 0.0f, 0.0f );
        vertecies[ 13 ].pos = Vec3<float >( right, top, back );
        vertecies[ 13 ].tPos = Vec2< float >( 1.0f, 0.0f );
        vertecies[ 14 ].pos = Vec3<float >( left, bottom, back );
        vertecies[ 14 ].tPos = Vec2< float >( 0.0f, 1.0f );
        vertecies[ 15 ].pos = Vec3<float >( right, bottom, back );
        vertecies[ 15 ].tPos = Vec2< float >( 1.0f, 1.0f );

        vertecies[ 16 ].pos = Vec3<float >( left, top, front );
        vertecies[ 16 ].tPos = Vec2< float >( 0.f, 1.f );
        vertecies[ 17 ].pos = Vec3<float >( right, top, front );
        vertecies[ 17 ].tPos = Vec2< float >( 1.f, 1.f );
        vertecies[ 18 ].pos = Vec3<float >( left, bottom, front );
        vertecies[ 18 ].tPos = Vec2< float >( 0.f, 0.f );
        vertecies[ 19 ].pos = Vec3<float >( right, bottom, front );
        vertecies[ 19 ].tPos = Vec2< float >( 1.f, 0.f );
        vertecies[ 20 ].pos = Vec3<float >( left, top, back );
        vertecies[ 20 ].tPos = Vec2< float >( 0.0f, 0.0f );
        vertecies[ 21 ].pos = Vec3<float >( right, top, back );
        vertecies[ 21 ].tPos = Vec2< float >( 1.0f, 0.0f );
        vertecies[ 22 ].pos = Vec3<float >( left, bottom, back );
        vertecies[ 22 ].tPos = Vec2< float >( 0.0f, 1.0f );
        vertecies[ 23 ].pos = Vec3<float >( right, bottom, back );
        vertecies[ 23 ].tPos = Vec2< float >( 1.0f, 1.0f );

        model->setVertecies( vertecies, 24 );

        // front side
        Polygon3D *p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 0;
        p->indexList[ 1 ] = 3;
        p->indexList[ 2 ] = 2;
        p->indexList[ 3 ] = 0;
        p->indexList[ 4 ] = 1;
        p->indexList[ 5 ] = 3;
        model->addPolygon( p );
        // back side
        p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 4;
        p->indexList[ 1 ] = 6;
        p->indexList[ 2 ] = 7;
        p->indexList[ 3 ] = 4;
        p->indexList[ 4 ] = 7;
        p->indexList[ 5 ] = 5;
        model->addPolygon( p );
        // right side
        p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 1 + 8;
        p->indexList[ 1 ] = 7 + 8;
        p->indexList[ 2 ] = 3 + 8;
        p->indexList[ 3 ] = 1 + 8;
        p->indexList[ 4 ] = 5 + 8;
        p->indexList[ 5 ] = 7 + 8;
        model->addPolygon( p );
        // left side
        p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 0 + 8;
        p->indexList[ 1 ] = 2 + 8;
        p->indexList[ 2 ] = 6 + 8;
        p->indexList[ 3 ] = 0 + 8;
        p->indexList[ 4 ] = 6 + 8;
        p->indexList[ 5 ] = 4 + 8;
        model->addPolygon( p );
        // top side
        p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 4 + 16;
        p->indexList[ 1 ] = 1 + 16;
        p->indexList[ 2 ] = 0 + 16;
        p->indexList[ 3 ] = 4 + 16;
        p->indexList[ 4 ] = 5 + 16;
        p->indexList[ 5 ] = 1 + 16;
        model->addPolygon( p );
        // down side
        p = new Polygon3D();
        p->indexAnz = 6;
        p->indexList = new int[ p->indexAnz ];
        p->indexBuffer->setLänge( p->indexAnz * 4 );
        p->indexBuffer->setData( p->indexList );
        p->indexList[ 0 ] = 6 + 16;
        p->indexList[ 1 ] = 2 + 16;
        p->indexList[ 2 ] = 3 + 16;
        p->indexList[ 3 ] = 6 + 16;
        p->indexList[ 4 ] = 3 + 16;
        p->indexList[ 5 ] = 7 + 16;
        model->addPolygon( p );
    }

    textur = new Model3DTextur();
}

// Setzt die Textur des Würfels, so dass sie an allen Seiten gleich ist
//  textur: Die Textur als Bild
void Cube::setTextur( Bild *textur )
{
    Textur *t = new Textur();
    t->setBildZ( textur );
    this->textur->setPolygonTextur( LINKS, t->getThis() );
    this->textur->setPolygonTextur( OBEN, t->getThis() );
    this->textur->setPolygonTextur( RECHTS, t->getThis() );
    this->textur->setPolygonTextur( UNTEN, t->getThis() );
    this->textur->setPolygonTextur( VORNE, t->getThis() );
    this->textur->setPolygonTextur( HINTEN, t );
    rend = 1;
}

// Setzt die Textur des Würfels, so dass sie an allen Seiten gleich ist
//  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
void Cube::setTextur( int id )
{
    Textur *t = texturRegister->getTextur( id );
    if( !t )
        return;
    this->textur->setPolygonTextur( LINKS, t->getThis() );
    this->textur->setPolygonTextur( OBEN, t->getThis() );
    this->textur->setPolygonTextur( RECHTS, t->getThis() );
    this->textur->setPolygonTextur( UNTEN, t->getThis() );
    this->textur->setPolygonTextur( VORNE, t->getThis() );
    this->textur->setPolygonTextur( HINTEN, t );
    rend = 1;
}

// Setzt die Textur von einer bestimmten Seite des Würfels
//  textur: Die Textur als Bild
//  s: Die Seite, die gesetzt werden soll
void Cube::setTextur( Bild *textur, CubeSeite s )
{
    Textur *t = new Textur();
    t->setBildZ( textur );
    this->textur->setPolygonTextur( s, t );
    rend = 1;
}

// Setzt die Textur von einer bestimmten Seite des Würfels
//  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
//  s: Die Seite, die gesetzt werden soll
void Cube::setTextur( int id, CubeSeite s )
{
    Textur *t = texturRegister->getTextur( id );
    if( !t )
        return;
    this->textur->setPolygonTextur( s, t );
    rend = 1;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3D *Cube::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}