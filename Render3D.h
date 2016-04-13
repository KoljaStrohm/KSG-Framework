#pragma once

#include "Array.h"
#include "Mat4.h"
#include "Vec4.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
enum D3D_PRIMITIVE_TOPOLOGY;
struct ID3D11ShaderResourceView;
struct ID3D11RasterizerState;

namespace Framework
{
    class Shader; // Shader.h
    class DXVertexBuffer; // DXBuffer.h
    class DXIndexBuffer; // DXBuffer.h
    class Textur; // Textur.h

    class Render3D
    {
    private:
        Mat4< float > matrixBuffer[ 3 ];
        Vec4< float > frustrum[ 6 ];
        Vec3< float > kamPos;
        ID3D11Device *device;
        ID3D11DeviceContext *context;
        ID3D11RasterizerState *texturRS;
        ID3D11RasterizerState *meshRS;
        Textur *defaultTextur;
        RCArray< RCArray< Shader > > *shader;
        Array< int > *shaderId;
        int lastObjektId;
        int lastTexturId;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) Render3D();
        // Destruktor
        __declspec( dllexport ) ~Render3D();
        // Setzt das Device, was zum zeichnen verwendet werden soll
        //  device: Das neue Device
        __declspec( dllexport ) void setDevice( ID3D11Device *device );
        // Setzt das Context Objekt, das zum Zeichnen verwendet werden soll
        //  context: das neue Conext Objekt
        __declspec( dllexport ) void setContext( ID3D11DeviceContext *context );
        // Setzt den aktuellen Shader. Er wird hinten an die Liste mit zuletzt verwendeten Shadern angefügt
        //  listIndex: Der Index der Liste mit zuletzt verwendeten Shadern
        //  sh: Der Shader, der verwendet werden soll
        __declspec( dllexport ) void benutzeShader( int listIndex, Shader *sh );
        // Sprinkt in der Liste mit zuletzt benutzten Shadern zurück und benutzt wieder den dortiegen Shader
        //  listIndex: Der Index der Liste mit zuletzt verwe deten Shadern
        //  anz: Die Anzahl der Shader, die zurückgesprungen werden soll. Bei 0 passiert nichts
        __declspec( dllexport ) void releaseShader( int listIndex, int anz = 1 );
        // Setzt die View und Projektion Matrizen, die zum zeichnen verwendet werden sollen
        //  view: Die View Matrix der Kamera
        //  proj: Die Projektion Matrix der Kamera
        //  kamPos: Die Position der Kamera in der Welt
        __declspec( dllexport ) void setKameraMatrix( Mat4< float > &view, Mat4< float > &proj, Vec3< float > &kamPos );
        // Beginnt das Zeichnen eines bestimmten objektes
        //  world: Die Matrix, die das Model aus dem Model space in den world space übersetzt
        //  zVertexBuffer: Ein VertexBuffer mit allen Punkten des Models ohne erhöhten Reference Counter
        __declspec( dllexport ) void beginnModel( Mat4< float > &world, DXVertexBuffer *zVertexBuffer, int modelId );
        // Zeichnet eine bestimmte struktur
        //  zIndexBuffer: Ein IndexBuffer, der auf verschiedene Vertices aus dem Vertex Buffer des Models zeigt. Ohne erhöhten Reference Counter
        //  textur: Ein Zeiger auf die Textur, die verwendet werden soll ohne erhöhten Reference Counter
        //  struktur: Die Struktur der angegebenen Indices, Beispiel: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST oder D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ...
        __declspec( dllexport ) void draw( DXIndexBuffer *zIndexBuffer, Textur *zTextur, D3D_PRIMITIVE_TOPOLOGY struktur = (D3D_PRIMITIVE_TOPOLOGY)4 );
        // Gibt einen der aktuell verwendeten Shader zurück
        //  listIndex: Der Index der Lise mit Shadern, von denen der aktuell benutzte zurückgegeben werden soll
        __declspec( dllexport ) Shader *getShader( int listIndex ) const;
        // Gibt einen der aktuell verwendeten Shader ohne erhöhten Reference Counter zurück
        //  listIndex: Der Index der Lise mit Shadern, von denen der aktuell benutzte zurückgegeben werden soll
        __declspec( dllexport ) Shader *zShader( int listIndex ) const;
        // Gibt das momentan verwendete Device Objekt ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) ID3D11Device *zDevice() const;
        // Gibt das momentan verwendete Context Objekt ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) ID3D11DeviceContext *zContext() const;
        // Überprüft, ob eine Kugel in dem Sichtbaren Raum der Welt liegt und gezeichnet werden muss
        //  pos: Der Mittelpunkt der Kugel
        //  radius: Der Radius der Kugel
        //  dist: Einen Zeiger auf einen float, in dem das quadrat des Abstands zur Kammeraposition gespeichert wird, falls diese Funktion true zurückgiebt und der Zeiger nicht 0 ist
        __declspec( dllexport ) bool isInFrustrum( const Vec3< float > &pos, float radius, float *dist = 0 ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Render3D *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Render3D *release();
    };
}