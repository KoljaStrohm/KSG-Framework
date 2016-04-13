#include "Render3D.h"
#include "Shader.h"
#include "Model3D.h"
#include "DXBuffer.h"
#include "Textur.h"
#include "Bild.h"
#include <d3d11.h>

using namespace Framework;

// Inhalt der Render3D Klasse
// Konstruktor
Render3D::Render3D()
{
    device = 0;
    context = 0;
    texturRS = 0;
    meshRS = 0;
    defaultTextur = new Textur();
    Bild *b = new Bild();
    b->neuBild( 10, 10, 0xFFFFFFFF );
    defaultTextur->setBildZ( b );
    shader = new RCArray< RCArray< Shader > >();
    shaderId = new Array< int >();
    lastObjektId = -1;
    lastTexturId = -1;
    ref = 1;
}

// Destruktor
Render3D::~Render3D()
{
    if( device )
        device->Release();
    if( context )
        context->Release();
    if( texturRS )
        texturRS->Release();
    if( meshRS )
        meshRS->Release();
    defaultTextur->release();
    shader->release();
    shaderId->release();
}

// Setzt das Device, was zum zeichnen verwendet werden soll
//  device: Das neue Device
void Render3D::setDevice( ID3D11Device *device )
{
    if( this->device )
        this->device->Release();
    this->device = device;
    if( device )
    {
        if( !texturRS )
        {
            D3D11_RASTERIZER_DESC rasterDesc;
            ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
            rasterDesc.AntialiasedLineEnable = false;
            rasterDesc.CullMode = D3D11_CULL_BACK;
            rasterDesc.DepthBiasClamp = 0.0f;
            rasterDesc.DepthClipEnable = true;
            rasterDesc.FillMode = D3D11_FILL_SOLID;
            rasterDesc.FrontCounterClockwise = false;
            rasterDesc.MultisampleEnable = false;
            rasterDesc.ScissorEnable = false;
            rasterDesc.SlopeScaledDepthBias = 0.0f;
            device->CreateRasterizerState( &rasterDesc, &texturRS );
        }
        if( !meshRS )
        {
            D3D11_RASTERIZER_DESC rasterDesc;
            ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
            rasterDesc.AntialiasedLineEnable = false;
            rasterDesc.CullMode = D3D11_CULL_BACK;
            rasterDesc.DepthBiasClamp = 0.0f;
            rasterDesc.DepthClipEnable = true;
            rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
            rasterDesc.FrontCounterClockwise = false;
            rasterDesc.MultisampleEnable = false;
            rasterDesc.ScissorEnable = false;
            rasterDesc.SlopeScaledDepthBias = 0.0f;
            device->CreateRasterizerState( &rasterDesc, &meshRS );
        }
        if( context )
        {
            context->RSSetState( texturRS );
            defaultTextur->updateTextur( this );
        }
    }
}

// Setzt das Context Objekt, das zum Zeichnen verwendet werden soll
//  context: das neue Conext Objekt
void Render3D::setContext( ID3D11DeviceContext *context )
{
    if( this->context )
        this->context->Release();
    this->context = context;
    if( context )
    {
        if( texturRS )
            context->RSSetState( texturRS );
        if( device )
            defaultTextur->updateTextur( this );
    }
}

// Setzt den aktuellen Shader. Er wird hinten an die Liste mit zuletzt verwendeten Shadern angefügt
//  listIndex: Der Index der Liste mit zuletzt verwendeten Shadern
//  sh: Der Shader, der verwendet werden soll
void Render3D::benutzeShader( int listIndex, Shader *sh )
{
    if( listIndex < 0 )
        return;
    if( !shader->z( listIndex ) || !shaderId->hat( listIndex ) )
    {
        shader->set( new RCArray< Shader >(), listIndex );
        shaderId->set( -1, listIndex );
    }
    int id = shaderId->get( listIndex ) + 1;
    shader->z( listIndex )->set( sh, id );
    shaderId->set( id, listIndex );
    sh->benutzeShader( context );
}

// Sprinkt in der Liste mit zuletzt benutzten Shadern zurück und benutzt wieder den dortiegen Shader
//  listIndex: Der Index der Liste mit zuletzt verwe deten Shadern
//  anz: Die Anzahl der Shader, die zurückgesprungen werden soll. Bei 0 passiert nichts
void Render3D::releaseShader( int listIndex, int anz )
{
    if( !shader->z( listIndex ) || !shaderId->hat( listIndex ) || anz < 0 )
        return;
    int id = shaderId->get( listIndex ) - anz;
    if( id < 0 )
        id = 0;
    if( shader->z( listIndex )->z( id ) )
        shader->z( listIndex )->z( id )->benutzeShader( context );
    shaderId->set( id, listIndex );
}

// Setzt die View und Projektion Matrizen, die zum zeichnen verwendet werden sollen
//  view: Die View Matrix der Kamera
//  proj: Die Projektion Matrix der Kamera
//  kamPos: Die Position der Kamera in der Welt
void Render3D::setKameraMatrix( Mat4< float > &view, Mat4< float > &proj, Vec3< float > &kamPos )
{
    matrixBuffer[ 1 ] = view;
    matrixBuffer[ 2 ] = proj;
    this->kamPos = kamPos;

    Mat4< float > tmp = proj * view;

    frustrum[ 0 ].x = tmp.elements[ 3 ][ 0 ] + tmp.elements[ 0 ][ 0 ];
    frustrum[ 0 ].y = tmp.elements[ 3 ][ 1 ] + tmp.elements[ 0 ][ 1 ];
    frustrum[ 0 ].z = tmp.elements[ 3 ][ 2 ] + tmp.elements[ 0 ][ 2 ];
    frustrum[ 0 ].w = tmp.elements[ 3 ][ 3 ] + tmp.elements[ 0 ][ 3 ];

    frustrum[ 1 ].x = tmp.elements[ 3 ][ 0 ] - tmp.elements[ 0 ][ 0 ];
    frustrum[ 1 ].y = tmp.elements[ 3 ][ 1 ] - tmp.elements[ 0 ][ 1 ];
    frustrum[ 1 ].z = tmp.elements[ 3 ][ 2 ] - tmp.elements[ 0 ][ 2 ];
    frustrum[ 1 ].w = tmp.elements[ 3 ][ 3 ] - tmp.elements[ 0 ][ 3 ];

    frustrum[ 2 ].x = tmp.elements[ 3 ][ 0 ] - tmp.elements[ 1 ][ 0 ];
    frustrum[ 2 ].y = tmp.elements[ 3 ][ 1 ] - tmp.elements[ 1 ][ 1 ];
    frustrum[ 2 ].z = tmp.elements[ 3 ][ 2 ] - tmp.elements[ 1 ][ 2 ];
    frustrum[ 2 ].w = tmp.elements[ 3 ][ 3 ] - tmp.elements[ 1 ][ 3 ];

    frustrum[ 3 ].x = tmp.elements[ 3 ][ 0 ] + tmp.elements[ 1 ][ 0 ];
    frustrum[ 3 ].y = tmp.elements[ 3 ][ 1 ] + tmp.elements[ 1 ][ 1 ];
    frustrum[ 3 ].z = tmp.elements[ 3 ][ 2 ] + tmp.elements[ 1 ][ 2 ];
    frustrum[ 3 ].w = tmp.elements[ 3 ][ 3 ] + tmp.elements[ 1 ][ 3 ];

    frustrum[ 4 ].x = tmp.elements[ 2 ][ 0 ];
    frustrum[ 4 ].y = tmp.elements[ 2 ][ 1 ];
    frustrum[ 4 ].z = tmp.elements[ 2 ][ 2 ];
    frustrum[ 4 ].w = tmp.elements[ 2 ][ 3 ];

    frustrum[ 5 ].x = tmp.elements[ 3 ][ 0 ] - tmp.elements[ 2 ][ 0 ];
    frustrum[ 5 ].y = tmp.elements[ 3 ][ 1 ] - tmp.elements[ 2 ][ 1 ];
    frustrum[ 5 ].z = tmp.elements[ 3 ][ 2 ] - tmp.elements[ 2 ][ 2 ];
    frustrum[ 5 ].w = tmp.elements[ 3 ][ 3 ] - tmp.elements[ 2 ][ 3 ];

    for( int i = 0; i < 6; i++ )
        frustrum[ i ].normalize();
}

// Beginnt das Zeichnen eines bestimmten objektes
//  world: Die Matrix, die das Model aus dem Model space in den world space übersetzt
//  zVertexBuffer: Ein VertexBuffer mit allen Punkten des Models ohne erhöhten Reference Counter
void Render3D::beginnModel( Mat4< float > &world, DXVertexBuffer *zVertexBuffer, int modelId )
{
    matrixBuffer[ 0 ] = world;
    if( shader->z( VERTEX ) && shaderId->hat( VERTEX ) )
        shader->z( VERTEX )->z( shaderId->get( VERTEX ) )->füllConstBuffer( context, (char*)matrixBuffer, 0 );
    if( lastObjektId == -1 || lastObjektId != modelId )
    {
        lastObjektId = modelId;
        unsigned int offset = 0;
        ID3D11Buffer *b = zVertexBuffer->zBuffer();
        unsigned int es = (unsigned)zVertexBuffer->getElementLänge();
        context->IASetVertexBuffers( 0, 1, &b, &es, &offset );
    }
}

// Zeichnet eine bestimmte struktur
//  zIndexBuffer: Ein IndexBuffer, der auf verschiedene Vertices aus dem Vertex Buffer des Models zeigt. Ohne erhöhten Reference Counter
//  textur: Ein Zeiger auf die Textur, die verwendet werden soll ohne erhöhten Reference Counter
//  struktur: Die Struktur der angegebenen Indices, Beispiel: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST oder D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ...
void Render3D::draw( DXIndexBuffer *zIndexBuffer, Textur *textur, D3D_PRIMITIVE_TOPOLOGY struktur )
{
    DXGI_FORMAT f = DXGI_FORMAT_R32_UINT;
    if( zIndexBuffer->getElementLänge() == 2 )
        f = DXGI_FORMAT_R16_UINT;
    if( zIndexBuffer->getElementLänge() == 1 )
        f = DXGI_FORMAT_R8_UINT;
    context->IASetIndexBuffer( zIndexBuffer->zBuffer(), f, 0 );
    context->IASetPrimitiveTopology( struktur );
    if( textur )
    {
        if( lastTexturId == -1 || lastTexturId != textur->getId() )
        {
            lastTexturId = textur->getId();
            ID3D11ShaderResourceView *v = *textur;
            context->PSSetShaderResources( 0, 1, &v );
        }
        context->DrawIndexed( zIndexBuffer->getElementAnzahl(), 0, 0 );
    }
    else
    {
        context->RSSetState( meshRS );
        ID3D11ShaderResourceView *v = *defaultTextur;
        context->PSSetShaderResources( 0, 1, &v );
        context->DrawIndexed( zIndexBuffer->getElementAnzahl(), 0, 0 );
        context->RSSetState( texturRS );
    }
}

// Gibt einen der aktuell verwendeten Shader zurück
//  listIndex: Der Index der Lise mit Shadern, von denen der aktuell benutzte zurückgegeben werden soll
Shader *Render3D::getShader( int listIndex ) const
{
    if( !shader->z( listIndex ) || !shaderId->hat( listIndex ) )
        return 0;
    return shader->z( listIndex )->get( shaderId->get( listIndex ) );
}

// Gibt einen der aktuell verwendeten Shader ohne erhöhten Reference Counter zurück
//  listIndex: Der Index der Lise mit Shadern, von denen der aktuell benutzte zurückgegeben werden soll
Shader *Render3D::zShader( int listIndex ) const
{
    if( !shader->z( listIndex ) || !shaderId->hat( listIndex ) )
        return 0;
    return shader->z( listIndex )->z( shaderId->get( listIndex ) );
}

// Gibt das momentan verwendete Device Objekt ohne erhöhten Reference Counter zurück
ID3D11Device *Render3D::zDevice() const
{
    return device;
}

// Gibt das momentan verwendete Context Objekt ohne erhöhten Reference Counter zurück
ID3D11DeviceContext *Render3D::zContext() const
{
    return context;
}

// Überprüft, ob eine Kugel in dem Sichtbaren Raum der Welt liegt und gezeichnet werden muss
//  pos: Der Mittelpunkt der Kugel
//  radius: Der Radius der Kugel
//  dist: Einen Zeiger auf einen float, in dem das quadrat des Abstands zur Kammeraposition gespeichert wird, falls diese Funktion true zurückgiebt und der Zeiger nicht 0 ist
bool Render3D::isInFrustrum( const Vec3< float > &pos, float radius, float *dist ) const
{
    for( int i = 0; i < 6; i++ )
    {
        if( frustrum[ i ] * pos + radius < 0 )
            return 0;
    }
    if( dist )
        *dist = kamPos.abstandSq( pos );
    return 1;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Render3D *Render3D::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Render3D *Render3D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}