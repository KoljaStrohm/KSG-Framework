#include "Model3D.h"
#include "Render3D.h"
#include "Model2D.h"
#include "DXBuffer.h"
#include "Textur.h"
#include <d3d11.h>

using namespace Framework;

// Inhalt der Knochen Klasse

// Konstruktor
Knochen::Knochen( int id )
{
    pos = Vec3< float >( 0, 0, 0 );
    winkel = Vec3< float >( 0, 0, 0 );
    geschwister = 0;
    kinder = 0;
    this->id = id;
}

// Destruktor
Knochen::~Knochen()
{
    delete geschwister;
    delete kinder;
}

// private

// Fügt dem Knochen ein Geschwister Knochen hinzu
//  k: Der Knochen, der hinzugefügt werden soll
void Knochen::addGeschwisterKnochen( Knochen *k )
{
    if( !geschwister )
        geschwister = k;
    else
        geschwister->addGeschwisterKnochen( k );
}

// public

// Setzt die Position des Knochens relativ zum Model Ursprung
//  pos: Die Position
void Knochen::setPosition( Vec3< float > &pos )
{
    this->pos = pos;
}

// Setzt die Drehung des Knochens relativ zum Model Ursprung
//  winkel: Ein Vektor der die Drehung um die verschiedenen Achsen als Komponenten hat
void Knochen::setDrehung( Vec3< float > &winkel )
{
    this->winkel = winkel;
}

// Fügt einem bestimmten Knochen ein Kind Knochen hinzu
//  id: Die id des Knochens, wo der Knochen als Kind hinzugefügt werden soll
//  k: Der Knochen, der hinzugefügt werden soll
void Knochen::addKind( int id, Knochen *k )
{
    if( this->id == id )
    {
        if( !kinder )
            kinder = k;
        else
            kinder->addGeschwisterKnochen( k );
    }
    else
    {
        if( kinder )
            kinder->addKind( id, k );
        else
        {
            Text err = "Es wurde kein Knochen mit der Id: ";
            err += id;
            err += " im Skelett gefunden, um ein Kind Knochen hinzuzufügen. Datei:" __FILE__ ", Zeile: ";
            err += __LINE__;
            err += "!";
            delete k;
            throw std::out_of_range( err );
        }
    }
}

// Berechnet die Matrizen des Knochen und die von all seinen Geschwister Knochen und Kind Knochen
//  elternMat: Die fertig berechnete Matrix des Elternknochens
//  matBuffer: Ein Array, in dem alle berechneten Matrizen gespeichert werden sollen
//  kamMatrix: Die vereiniegung der view und projektions Matrizen
void Knochen::kalkulateMatrix( Mat4< float > &elternMat, Mat4< float > *matBuffer, Mat4< float > &kamMat )
{
    if( geschwister )
        geschwister->kalkulateMatrix( elternMat, matBuffer, kamMat );
    matBuffer[ id ] = matBuffer[ id ].translation( pos ) * matBuffer[ id ].rotationZ( winkel.z ) * matBuffer[ id ].rotationX( winkel.x ) * matBuffer[ id ].rotationY( winkel.y );
    matBuffer[ id ] = elternMat * matBuffer[ id ];
    if( kinder )
        kinder->kalkulateMatrix( matBuffer[ id ], matBuffer, kamMat );
    matBuffer[ id ] = kamMat * matBuffer[ id ];
}

// Kopiert den Knochen mit allen Geschwister Knochen und Kind Knochen
Knochen *Knochen::kopiereKnochen() const
{
    Knochen *ret = new Knochen( id );
    ret->pos = pos;
    ret->winkel = winkel;
    if( geschwister )
        ret->geschwister = geschwister->kopiereKnochen();
    if( kinder )
        ret->kinder = kinder->kopiereKnochen();
    return ret;
}

// Gibt die Id des Knochens zurück
int Knochen::getId() const
{
    return id;
}

// Inhalt der Skelett Klasse

// Konstruktor
Skelett::Skelett()
{
    k = 0;
    nextId = 0;
    ref = 1;
}

// Destruktor
Skelett::~Skelett()
{
    if( k )
        delete k;
}

// Gibt die Id des nächsten Knochens zurück und berechnet die neue Id für den Knochen danach
// Es können maximal 128 Knochen für ein Skelett existieren. Wenn diese Zahl überschritten wird, so wird -1 zurückgegeben
int Skelett::getNextKnochenId()
{
    return nextId++;
}

// Fügt dem Skellet einen Knochen hinzu
//  k: Der Knochen
//  elternId: Die Id des Eltern Knochens. Wenn der Knochen kein Elternknochen besitzt, kannder Parameter weggelassen werden.
void Skelett::addKnochen( Knochen *k, int elternId )
{
    if( !this->k )
        this->k = k;
    else
        this->k->addKind( elternId, k );
    if( k->getId() >= nextId )
        nextId = k->getId() + 1;
}

// Berechnet die Matrizen der Knochen
//  modelMatrix: Die Matrix, die das Skelett in den Raum der Welt transformiert
//  matBuffer: Ein Array von Matrizen, der durch die Knochen Matrizen gefüllt wird
//  return: gibt die Anzahl der verwendeten Matrizen zurück
//  kamMatrix: Die vereiniegung der view und projektions Matrizen
int Skelett::kalkulateMatrix( Mat4< float > &modelMatrix, Mat4< float > *matBuffer, Mat4< float > &kamMatrix )
{
    k->kalkulateMatrix( modelMatrix, matBuffer, kamMatrix );
    return nextId;
}

// Kopiert das Skelett
Skelett *Skelett::kopiereSkelett() const
{
    Skelett *ret = new Skelett();
    ret->nextId = nextId;
    if( k )
        ret->addKnochen( k->kopiereKnochen() );
    return ret;
}

// Erhöht den Reference Counting Zähler.
Skelett *Skelett::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Skelett *Skelett::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt des Polygon3D Struct

// Konstruktor
Polygon3D::Polygon3D()
{
    indexAnz = 0;
    indexList = 0;
    indexBuffer = new DXIndexBuffer( sizeof( int ) );
}

// Destruktor
Polygon3D::~Polygon3D()
{
    indexBuffer->release();
    delete[] indexList;
}

// Inhalt der Model3DData Klasse

// Konstruktor
Model3DData::Model3DData()
{
    id = -1;
    skelett = 0;
    vertexList = 0;
    polygons = new Array< Polygon3D* >();
    vertexBuffer = new DXVertexBuffer( sizeof( Vertex3D ) );
    radius = 0;
    ref = 1;
}

// Destruktor
Model3DData::~Model3DData()
{
    clearModel();
    vertexBuffer->release();
    polygons->release();
}

// Löscht alle Model daten
void Model3DData::clearModel()
{
    delete[] vertexList;
    vertexList = 0;
    for( auto i = polygons->getArray(); i.set; i++ )
        delete i.var;
    polygons->leeren();
    if( skelett )
        skelett->release();
    skelett = 0;
    radius = 0;
}

// Setzt den Zeiger auf ein standartmäßig verwendete Skelett
//  s: Das Skelett, das verwendet werden soll
void Model3DData::setSkelettZ( Skelett *s )
{
    if( skelett )
        skelett->release();
    skelett = s;
}

// Setzt einen Zeiger auf eine Liste mit allen Vertecies des Models
//  vertexList: Ein Array mit Vertecies
//  anz: Die Anzahl der Vertecies im Array
void Model3DData::setVertecies( Vertex3D *vertexList, int anz )
{
    delete[] this->vertexList;
    this->vertexList = vertexList;
    vertexBuffer->setData( vertexList );
    vertexBuffer->setLänge( sizeof( Vertex3D ) * anz );
    radius = 0;
    for( int i = 0; i < anz; i++ )
    {
        float r = vertexList[ i ].pos.län();
        if( r > radius )
            radius = r;
    }
}

// Fügt ein Polygon zum Model hinzu
//  polygon: Das Polygon, das hinzugefügt erden soll
void Model3DData::addPolygon( Polygon3D *polygon )
{
    polygons->add( polygon );
}

// Konvertiert ein 2d Model zu 3D
//  model: Das 2d Model, das zu 3d konvertiert werden soll
//  z: Die z koordinate aller punkte des Models
void Model3DData::copyModel2D( Model2DData *model, float z )
{
    if( model && model->vListen && model->polygons )
    {
        clearModel();
        int vAnz = 0;
        for( auto i = model->polygons->getArray(); i.set; i++ )
            vAnz += i.var.vertex->getEintragAnzahl();
        vertexList = new Vertex3D[ vAnz ];
        vertexBuffer->setData( vertexList );
        vertexBuffer->setLänge( sizeof( Vertex3D ) * vAnz );
        int index = 0;
        for( auto i = model->vListen->getArray(); i.set; i++ )
        {
            Polygon3D *p = new Polygon3D();
            p->indexAnz = 0;
            for( auto j = i.var->getArray(); j.set; j++ )
            {
                for( auto *k = &j.var->zListe()->getArray(); k->next->next && k->next->next->set; k = k->next )
                    p->indexAnz += 3;
            }
            p->indexList = new int[ p->indexAnz ];
            p->indexBuffer->setData( p->indexList );
            p->indexBuffer->setLänge( sizeof( int ) * p->indexAnz );
            p->indexAnz = 0;
            for( auto j = i.var->getArray(); j.set; j++ )
            {
                for( auto *k = &j.var->zListe()->getArray(); k && k->set; k = k->next )
                {
                    vertexList[ index ].pos = Vec3< float >( k->var->punkt->x, k->var->punkt->y, z );
                    vertexList[ index ].tPos = ( Vec2< float > )*k->var->textur;
                    if( k->next && k->next->set && k->next->next && k->next->next->set )
                    {
                        p->indexList[ p->indexAnz ] = index;
                        p->indexAnz++;
                        p->indexList[ p->indexAnz ] = index + 1;
                        p->indexAnz++;
                        p->indexList[ p->indexAnz ] = index + 2;
                        p->indexAnz++;
                    }
                    index++;
                }
            }
            addPolygon( p );
        }
    }
}

// Entfernt ein Polygon
//  index: Der Index des Polygons
void Model3DData::removePolygon( int index )
{
    if( !polygons->hat( index ) )
        return;
    delete polygons->get( index );
    polygons->lösche( index );
}

// Aktualisiert die Vertecies
//  zRObj: Das Objekt, mit dem die Grafikkarte verwaltet wird
void Model3DData::aktualisiereVertecies( Render3D *zRObj )
{
    vertexBuffer->copieren( zRObj );
}

// Berechnet die Matrizen der Knochen
//  modelMatrix: Die Matrix, die das Skelett in den Raum der Welt transformiert
//  matBuffer: Ein Array von Matrizen, der durch die Knochen Matrizen gefüllt wird
//  return: gibt die Anzahl der verwendeten Matrizen zurück
//  kamMatrix: Die vereiniegung der view und projektions Matrizen
int Model3DData::kalkulateMatrix( Mat4< float > &modelMatrix, Mat4< float > *matBuffer, Mat4< float > &kamMatrix ) const
{
    if( !skelett )
        return 0;
    return skelett->kalkulateMatrix( modelMatrix, matBuffer, kamMatrix );
}

// Zeichnet alle Polygons
//  world: Die Welt Matrix, die das Model in die Welt transformiert
//  zTxt: Eine Liste mit Texturen der einzelnen Polygone
//  zRObj: Das Objekt, mit dem gezeichnet werden soll
void Model3DData::render( Mat4< float > &welt, const Model3DTextur *zTxt, Render3D *zRObj )
{
    int ind = 0;
    for( auto *i = &polygons->getArray(); i && i->set; i = i->next )
    {
        i->var->indexBuffer->copieren( zRObj );
        Textur *t = zTxt->zPolygonTextur( ind );
        if( t && t->brauchtUpdate() )
            t->updateTextur( zRObj );
        zRObj->draw( i->var->indexBuffer, t );
        ind++;
    }
}

// Gibt die Anzahl an Polygonen zurück
int Model3DData::getPolygonAnzahl() const
{
    return polygons->getEintragAnzahl();
}

// Gibt ein bestimmtes Polygon zurück
//  index: Der Index des Polygons
Polygon3D *Model3DData::getPolygon( int index ) const
{
    if( !polygons->hat( index ) )
        return 0;
    return polygons->get( index );
}

// Gibt den radius einer Kugel zurück, die das gesammte Model umschließt
float Model3DData::getRadius() const
{
    return radius;
}

// Gibt die Id der Daten zurück, wenn sie in einer Model3DList registriert wurden. (siehe Framework::zM3DRegister())
int Model3DData::getId() const
{
    return id;
}

// Gibt einen Buffer mit allen Vertecies des Models zurück
const DXVertexBuffer *Model3DData::zVertexBuffer() const
{
    return vertexBuffer;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Model3DData *Model3DData::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3DData *Model3DData::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der Model3DTextur

// Konstruktor
Model3DTextur::Model3DTextur()
{
    textures = new RCArray< Textur >();
    ref = 1;
}

// Destruktor
Model3DTextur::~Model3DTextur()
{
    textures->release();
}

// Legt fest, welche Textur für welches Polygon ist
//  pI: Der Index des Polygons
//  txt: Die Textur des Polygons
void Model3DTextur::setPolygonTextur( int pI, Textur *txt )
{
    textures->set( txt, pI );
}

// Gibt einen Zeiger auf die Textur eines Polygons zurück ohne erhöhten Reference Counter
//  i: Der Index des Polygons
Textur *Model3DTextur::zPolygonTextur( int i ) const
{
    return textures->z( i );
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Model3DTextur *Model3DTextur::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3DTextur *Model3DTextur::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der Model3D Klasse
// Konstruktor
Model3D::Model3D()
    : Zeichnung3D()
{
    model = 0;
    textur = 0;
    skelett = 0;
    ref = 1;
}

// Destruktor
Model3D::~Model3D()
{
    if( model )
        model->release();
    if( textur )
        textur->release();
    if( skelett )
        skelett->release();
}

// Setzt den Zeiger auf das zum Annimieren verwendete Skelett
//  s: Das Skelett, das verwendet werden soll
void Model3D::setSkelettZ( Skelett *s )
{
    if( skelett )
        skelett->release();
    skelett = s;
}

// Setzt die Daten des Models
//  data: Die Daten
void Model3D::setModelDaten( Model3DData *data )
{
    if( model )
        model->release();
    model = data;
}

// Setzt die zum Zeichnen zu benutzenden Texturen
//  txt: Ein Liste mit Texturen zu den verschiedenen Polygonen zugeordnet
void Model3D::setModelTextur( Model3DTextur *txt )
{
    if( textur )
        textur->release();
    textur = txt;
}

// Errechnet die Matrizen aller Knochen des Skeletts des Models
//  viewProj: Die miteinander multiplizierten Kameramatrizen
//  matBuffer: Ein Array mit Matrizen, der gefüllt werden soll
//  return: Die Anzahl der Matrizen, die das Model benötigt
int Model3D::errechneMatrizen( Mat4< float > &viewProj, Mat4< float > *matBuffer )
{
    int ret = 0;
    if( skelett )
        ret = skelett->kalkulateMatrix( welt, matBuffer, viewProj );
    else if( model )
        ret = model->kalkulateMatrix( welt, matBuffer, viewProj );
    if( !ret )
        return __super::errechneMatrizen( viewProj, matBuffer );
    return ret;
}

// Verarbeitet die vergangene Zeit
//  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
//  return: true, wenn sich das Objekt verändert hat, false sonnst.
bool Model3D::tick( double tickval )
{
    radius = model ? model->getRadius() : 0;
    return __super::tick( tickval );
}

// Zeichnet das Model
//  zRObj: Ein Zeiger auf das Objekt, das zum Zeichnen verwendet werden soll (ohne erhöhten Reference Counter)
void Model3D::render( Render3D *zRObj )
{
    if( !model )
        return;
    model->aktualisiereVertecies( zRObj );
    zRObj->beginnModel( this );
    model->render( welt, textur, zRObj );
}

// Gibt die Id der Daten zurück, wenn sie in einer Model3DList registriert wurden. (siehe Framework::zM3DRegister())
int Model3D::getDatenId() const
{
    return model ? model->getId() : -1;
}

// Gibt einen Buffer mit allen Vertecies des Models zurück
const DXVertexBuffer *Model3D::zVertexBuffer() const
{
    return model ? model->zVertexBuffer() : 0;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Model3D *Model3D::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3D *Model3D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}