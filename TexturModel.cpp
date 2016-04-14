#include "TexturModel.h"
#include "DXBuffer.h"
#include "Textur.h"
#include "Globals.h"
#include "TexturList.h"

using namespace Framework;

// Inhalt der TexturModel Klasse

// Konstruktor
TexturModel::TexturModel()
    : Model3D()
{
    Vertex3D *vertecies = new Vertex3D[ 4 ];
    vertecies[ 0 ].pos = Vec3<float >( -50, 50, 0.f );
    vertecies[ 0 ].tPos = Vec2< float >( 0.f, 0.f );
    vertecies[ 1 ].pos = Vec3<float >( 50, 50, 0.f );
    vertecies[ 1 ].tPos = Vec2< float >( 1.f, 0.f );
    vertecies[ 2 ].pos = Vec3<float >( -50, -50, 0.f );
    vertecies[ 2 ].tPos = Vec2< float >( 0.f, 1.f );
    vertecies[ 3 ].pos = Vec3<float >( 50, -50, 0.f );
    vertecies[ 3 ].tPos = Vec2< float >( 1.f, 1.f );

    model = new Model3DData();
    model->setVertecies( vertecies, 4 );
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

    textur = new Model3DTextur();
}

// Setzt die Textur die angezeigt werden soll
//  textur: Die Textur als Bild
void TexturModel::setTextur( Bild *textur )
{
    Textur *t = new Textur();
    t->setBildZ( textur );
    this->textur->setPolygonTextur( 0, t );
    rend = 1;
}

// Setzt die Textur die angezeigt werden soll 
//  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
void TexturModel::setTextur( int id )
{
    Textur *t = texturRegister->getTextur( id );
    if( !t )
        return;
    this->textur->setPolygonTextur( 0, t );
    rend = 1;
}

// Setzt die Größe, in der Die Textur angezeigt wird
//  gr: Ein Vektor, der für x und y die breite und höhe beinhaltet
void TexturModel::setGröße( Vec2< float > gr )
{
    gr /= 2;
    Vertex3D *vertecies = new Vertex3D[ 4 ];
    vertecies[ 0 ].pos = Vec3<float >( -gr.x, gr.y, 0.f );
    vertecies[ 0 ].tPos = Vec2< float >( 0.f, 0.f );
    vertecies[ 1 ].pos = Vec3<float >( gr.x, gr.y, 0.f );
    vertecies[ 1 ].tPos = Vec2< float >( 1.f, 0.f );
    vertecies[ 2 ].pos = Vec3<float >( -gr.x, -gr.y, 0.f );
    vertecies[ 2 ].tPos = Vec2< float >( 0.f, 1.f );
    vertecies[ 3 ].pos = Vec3<float >( gr.x, -gr.y, 0.f );
    vertecies[ 3 ].tPos = Vec2< float >( 1.f, 1.f );
    model->setVertecies( vertecies, 4 );
}

// Setzt die Größe, in der die Textur angezeigt wird
//  b: Die Breite, in der die Textur angezeigt wird
//  h: Die Höhe, in der die Textur angezeigt wird
void TexturModel::setGröße( float b, float h )
{
    b /= 2;
    h /= 2;
    Vertex3D *vertecies = new Vertex3D[ 4 ];
    vertecies[ 0 ].pos = Vec3<float >( -b, h, 0.f );
    vertecies[ 0 ].tPos = Vec2< float >( 0.f, 0.f );
    vertecies[ 1 ].pos = Vec3<float >( b, h, 0.f );
    vertecies[ 1 ].tPos = Vec2< float >( 1.f, 0.f );
    vertecies[ 2 ].pos = Vec3<float >( -b, -h, 0.f );
    vertecies[ 2 ].tPos = Vec2< float >( 0.f, 1.f );
    vertecies[ 3 ].pos = Vec3<float >( b, -h, 0.f );
    vertecies[ 3 ].tPos = Vec2< float >( 1.f, 1.f );
    model->setVertecies( vertecies, 4 );
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3D *TexturModel::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}