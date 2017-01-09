#include "Textur.h"
#include "Bild.h"
#ifdef WIN32
#include "Render3D.h"
#include <d3d11.h>
#endif

using namespace Framework;


// Inhalt der Textur Klasse
// Konstruktor
Textur::Textur()
{
    bild = 0;
    txt = 0;
    view = 0;
    lastGr = Punkt( 0, 0 );
    id = -1;
    ref = 1;
}

// Destruktor
Textur::~Textur()
{
    if( bild )
        bild->release();
#ifdef WIN32
    if( txt )
        txt->Release();
    if( view )
        view->Release();
#endif
}

// Setzt einen Zeiger auf das Bild, welches die Textur enthält
//  b: Der Zeiger auf das Bild
void Textur::setBildZ( Bild *b )
{
    if( bild )
        bild->release();
    bild = b;
}

// Setzt das Bild welches die Textur enthält, indem es kopiert wird
//  b: Das Bild, was kopiert werden soll
void Textur::setBild( Bild *b )
{
    if( !b )
        return;
    if( !bild || bild->getBreite() != b->getBreite() || bild->getHeight() != b->getHeight() )
    {
        if( !bild )
            bild = new Bild();
        bild->neuBild( b->getBreite(), b->getHeight(), 0 );
    }
    bild->drawBild( 0, 0, bild->getBreite(), bild->getHeight(), *b );
    b->release();
}

// Aktualisiert die Textur. Die Pixel des aktuellen Bildes werden in den Graphikspeicher kopiert
//  zRObj: Das Objekt, mit dem die Graphikkarte angesprochen wird
bool Textur::updateTextur( Render3D *zRObj )
{
    if( !bild )
        return 0;
#ifdef WIN32
    if( !txt || lastGr != bild->getSize() )
    {
        if( txt )
            txt->Release();
        txt = 0;
        D3D11_TEXTURE2D_DESC bufferDesc;
        memset( &bufferDesc, 0, sizeof( D3D11_TEXTURE2D_DESC ) );
        bufferDesc.ArraySize = 1;
        bufferDesc.Width = bild->getBreite();
        bufferDesc.Height = bild->getHeight();
        bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.SampleDesc.Count = 1;
        bufferDesc.MipLevels = 1;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        HRESULT r = zRObj->zDevice()->CreateTexture2D( &bufferDesc, 0, &txt );
        if( r != S_OK )
            return 0;
    }
    D3D11_MAPPED_SUBRESOURCE buffer;
    zRObj->zContext()->Map( txt, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &buffer );
    int *bgBuff = bild->getBuffer();
    int tmpBr = 4 * bild->getBreite();
    for( int y = 0, pitch = 0, bry = 0; y < bild->getHeight(); ++y, pitch += buffer.RowPitch, bry += bild->getBreite() )
        memcpy( &( (BYTE *)buffer.pData )[ pitch ], ( void* )&( bgBuff[ bry ] ), tmpBr );
    zRObj->zContext()->Unmap( txt, 0 );
    if( !view || lastGr != bild->getSize() )
    {
        if( view )
            view->Release();
        view = 0;
        D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesk;
        memset( &resourceDesk, 0, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
        resourceDesk.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        resourceDesk.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceDesk.Texture2D.MipLevels = 1;
        HRESULT r = zRObj->zDevice()->CreateShaderResourceView( txt, &resourceDesk, &view );
        if( r != S_OK )
            return 0;
    }
    lastGr = bild->getSize();
#endif
    return 1;
}

// Gibt true zurük, wenn updateTextur aufgerufen werden muss
bool Textur::brauchtUpdate() const
{
    return !view;
}

// Gibt einen Zeiger auf das Bild zurück
Bild *Textur::getBild() const
{
    return bild ? bild->getThis() : 0;
}

// Gibt einen Zeiger auf das Bild ohne erhöhten Reference Counter zurück
Bild *Textur::zBild() const
{
    return bild;
}

// Gibt die Id der Textur zurück, wenn sie in einer TexturList registriert wurde. (siehe Framework::zTexturRegister())
int Textur::getId() const
{
    return id;
}

// Gibt die verwendtete Shader Resource View zurück
Textur::operator ID3D11ShaderResourceView*( ) const
{
    return view;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Textur *Textur::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Textur *Textur::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}