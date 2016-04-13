#pragma once

#include "vec2.h"
#include "Array.h"
#include "Mat4.h"
#include "Zeichnung3D.h"

struct ID3D11Buffer;

namespace Framework
{
    struct Polygon2D; // Model2D.h
    class Textur; // Textur.h
    class Model2DData; // Model2D.h
    class DXIndexBuffer; // DXBuffer.h
    class DXVertexBuffer; // DXBuffer.h
    class Render3D; // Render3D.h
    class Model3DTextur; // Model3D.h
    class Model3DList; // Model3DList.h

    struct Vertex3D
    {
        Vec3< float > pos;
        Vec2< float > tPos;
    };

    class Knochen
    {
    private:
        Vec3< float > pos;
        Vec3< float > winkel;
        int *indexList;
        int indexAnz;
        DXIndexBuffer *indexBuffer;

    public:
        // Konstruktor
        Knochen();
        // Destruktor
        ~Knochen();
        // Setzt die Anzahl der mit dem Knochen verbundenen Vertecies
        //  anz: Die Anzahl der Vertecies
        void setVertexAnzahl( int anz );
        // Setzt deinen bestimmten Vertex des Knochens
        //  i: Der Index des Vertex im Knochen
        //  vId: Der Index des Vertex im Model
        inline void setVertex( int i, int vId );
        // Setzt die Position des Knochens relativ zum Model Ursprung
        //  pos: Die Position
        void setPosition( Vec3< float > &pos );
        // Setzt die Drehung des Knochens relativ zum Model Ursprung
        //  winkel: Ein Vektor der die Drehung um die verschiedenen Achsen als Komponenten hat
        void setDrehung( Vec3< float > &winkel );
        // 
        void render( Render3D *zRObj, Mat4< float > &mat );
    };

    class Skelett
    {
    private:

    public:

    };

    struct Polygon3D
    {
        int *indexList;
        int indexAnz;
        DXIndexBuffer *indexBuffer;

        // Konstruktor
        __declspec( dllexport ) Polygon3D();
        // Destruktor
        __declspec( dllexport ) ~Polygon3D();
    };

    // Speichert alle Geometrischen Daten eines Modells, also
    // Raum - und Textur Koordinaten aller Eckpunkte
    class Model3DData
    {
    private:
        Vertex3D *vertexList;
        DXVertexBuffer *vertexBuffer;
        Array< Polygon3D* > *polygons;
        float radius;
        int id;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) Model3DData();
        // Destruktor
        __declspec( dllexport ) ~Model3DData();
        // Löscht alle Model daten
        __declspec( dllexport ) void clearModel();
        // Setzt einen Zeiger auf eine Liste mit allen Vertecies des Models
        //  vertexList: Ein Array mit Vertecies
        //  anz: Die Anzahl der Vertecies im Array
        __declspec( dllexport ) void setVertecies( Vertex3D *vertexList, int anz );
        // Fügt ein Polygon zum Model hinzu
        //  polygon: Das Polygon, das hinzugefügt erden soll
        __declspec( dllexport ) void addPolygon( Polygon3D *polygon );
        // Konvertiert ein 2d Model zu 3D
        //  model: Das 2d Model, das zu 3d konvertiert werden soll
        //  z: Die z koordinate aller punkte des Models
        __declspec( dllexport ) void copyModel2D( Model2DData *model, float z );
        // Entfernt ein Polygon
        //  index: Der Index des Polygons
        __declspec( dllexport ) void removePolygon( int index );
        // Zeichnet alle Polygons
        //  world: Die Welt Matrix, die das Model in die Welt transformiert
        //  zTxt: Eine Liste mit Texturen der einzelnen Polygone
        //  zRObj: Das Objekt, mit dem gezeichnet werden soll
        __declspec( dllexport ) void render( Mat4< float > &welt, const Model3DTextur *zTxt, Render3D *zRObj );
        // Gibt die Anzahl an Polygonen zurück
        __declspec( dllexport ) int getPolygonAnzahl();
        // Gibt ein bestimmtes Polygon zurück
        //  index: Der Index des Polygons
        __declspec( dllexport ) Polygon3D *getPolygon( int index );
        // Gibt den radius einer Kugel zurück, die das gesammte Model umschließt
        __declspec( dllexport ) float getRadius() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Model3DData *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Model3DData *release();

        friend Model3DList;
    };

    // Speichert eine Liste mit Texturen und für welche Polygone welche Textur benutzt werden soll
    class Model3DTextur
    {
    private:
        RCArray< Textur > *textures;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) Model3DTextur();
        // Destruktor
        __declspec( dllexport ) ~Model3DTextur();
        // Legt fest, welche Textur für welches Polygon ist
        //  pI: Der Index des Polygons
        //  txt: Die Textur des Polygons
        __declspec( dllexport ) void setPolygonTextur( int pI, Textur *txt );
        // Gibt einen Zeiger auf die Textur eines Polygons zurück ohne erhöhten Reference Counter
        //  i: Der Index des Polygons
        __declspec( dllexport ) Textur *zPolygonTextur( int i ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Model3DTextur *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Model3DTextur *release();
    };

    class Model3D : public Zeichnung3D
    {
    protected:
        Model3DData *model;
        Model3DTextur *textur;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) Model3D();
        // Destruktor
        __declspec( dllexport ) ~Model3D();
        // Setzt die Daten des Models
        //  data: Die Daten
        __declspec( dllexport ) void setModelDaten( Model3DData *data );
        // Setzt die zum Zeichnen zu benutzenden Texturen
        //  txt: Ein Liste mit Texturen zu den verschiedenen Polygonen zugeordnet
        __declspec( dllexport ) void setModelTextur( Model3DTextur *txt );
        // Verarbeitet die vergangene Zeit
        //  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
        //  return: true, wenn sich das Objekt verändert hat, false sonnst.
        __declspec( dllexport ) virtual bool tick( double tickval );
        // Zeichnet das Model
        //  zRObj: Ein Zeiger auf das Objekt, das zum Zeichnen verwendet werden soll (ohne erhöhten Reference Counter)
        __declspec( dllexport ) void render( Render3D *zRObj ) override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Model3D *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual Model3D *release();
    };
}