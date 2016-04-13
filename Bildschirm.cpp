#include "Bildschirm.h"
#include "Bild.h"
#include "Fenster.h"
#include "Text.h"
#include "Zeichnung.h"
#include "Globals.h"
#include "Zeit.h"
#include "ToolTip.h"
#include "MausEreignis.h"
#include <iostream>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include "Datei.h"
#include "DefaultShader.h"
#include "comdef.h"
#include "Zeichnung3D.h"
#include "Shader.h"
#include "Kam3D.h"
#include "Render3D.h"
#include "Mat3.h"
#include "DXBuffer.h"
#include "Model3D.h"
#include "Textur.h"
#include <d3d11.h>
#include <d3d9.h>

using namespace Framework;

// Inhalt der Bildschirmklass aus Bildschirm.h
// Konstruktor 
Bildschirm::Bildschirm( WFenster *f )
    : fenster( f ),
    renderB( new Bild( 1 ) ),
    ref( 1 ),
    members( new ZeichnungArray() ),
    füllFarbe( 0xFF000000 ),
    deckFarbe( 0 ),
    onTop( 0 ),
    renderOnTop( 0 ),
    renderZeichnungen( 1 ),
    vollbild( 0 ),
    rendering( 0 ),
    renderZeit( new ZeitMesser() ),
    backBufferGröße( 0, 0 ),
    tips( new RCArray< ToolTip >() ),
    tipAnzahl( 0 ),
    testRend( 1 ),
    füll( 1 ),
    rend( 0 )
{
    InitializeCriticalSection( &cs );
}

// Destruktor 
Bildschirm::~Bildschirm()
{
    lock();
    if( renderB )
        renderB->release();
    if( fenster )
        fenster->release();
    delete members;
    tipAnzahl = 0;
    tips->release();
    renderZeit->release();
    unlock();
    DeleteCriticalSection( &cs );
}

// nicht konstant 
void Bildschirm::lock()
{
    EnterCriticalSection( &cs );
}

void Bildschirm::unlock()
{
    LeaveCriticalSection( &cs );
}

void Bildschirm::setFüll( bool f )
{
    füll = f;
}

void Bildschirm::setTestRend( bool tr ) // legt fest, ob vo rendern auf updates geprüft werden soll
{
    testRend = tr;
}

void Bildschirm::setRenderZeichnungen( bool rO ) // legt fest, ob die Zeichnunge gerendert werden
{
    lock();
    renderZeichnungen = rO;
    rend = 1;
    unlock();
}

void Bildschirm::setOnTop( bool onTop ) // legt fest, ob das onTop Zeichnung gerendert wid
{
    renderOnTop = onTop;
    rend = 1;
}

void Bildschirm::setOnTopZeichnung( Zeichnung *obj ) // setzt das OnTop Zeichnung
{
    lock();
    onTop = obj;
    rend = 1;
    unlock();
}

void Bildschirm::setdeckFarbe( int f ) // setzt die deckFarbe
{
    deckFarbe = f;
    rend = 1;
}

void Bildschirm::addMember( Zeichnung *obj ) // Fügt ein Zeichnung hinzu
{
    lock();
    members->addZeichnung( obj );
    members->updateIndex( 0 );
    rend = 1;
    unlock();
}

void Bildschirm::removeMember( Zeichnung *obj ) // Entfernt ein Zeichnung
{
    lock();
    members->removeZeichnung( obj );
    members->updateIndex( 0 );
    rend = 1;
    unlock();
}

void Bildschirm::setFüllFarbe( int f ) // setzt die Fill Farbe
{
    füllFarbe = f;
    rend = 1;
}

void Bildschirm::setVollbild( bool vollbild ) // setzt vollbild
{
    lock();
    this->vollbild = vollbild;
    rend = 1;
    unlock();
}

void Bildschirm::tick( double tickval )
{
    lock();
    if( !renderOnTop )
    {
        for( int i = 0; i < tipAnzahl; ++i )
            rend |= tips->z( i )->tick( tickval );
        rend |= members->tick( tickval );
    }
    else if( onTop )
    {
        rend |= onTop->tick( tickval );
        for( int i = 0; i < tipAnzahl; ++i )
            rend |= tips->z( i )->tick( tickval );
    }
    unlock();
}

void Bildschirm::setBackBufferGröße( int breite, int höhe ) // setzt die Größe des Backbuffers
{
    lock();
    backBufferGröße.x = breite;
    backBufferGröße.y = höhe;
    rend = 1;
    unlock();
}

void Bildschirm::setBackBufferGröße( Punkt &größe )
{
    lock();
    backBufferGröße = größe;
    rend = 1;
    unlock();
}

void Bildschirm::doMausEreignis( MausEreignis &me ) // sendet maus Ereignis
{
    int fBr = backBufferGröße.x;
    int fHö = backBufferGröße.y;
    if( fenster )
    {
        fBr = fenster->getKörperBreite();
        fHö = fenster->getKörperHöhe();
    }
    me.mx = (int)( me.mx * backBufferGröße.x / (double)fBr + 0.5 );
    me.my = (int)( me.my * backBufferGröße.y / (double)fHö + 0.5 );
    lock();
    if( !renderOnTop )
    {
        for( int i = 0; i < tipAnzahl; ++i )
            tips->z( i )->doMausEreignis( me );
        members->sendMausAll( me );
    }
    else if( onTop )
    {
        onTop->doMausEreignis( me );
        for( int i = 0; i < tipAnzahl; ++i )
            tips->z( i )->doMausEreignis( me );
    }
    unlock();
}

void Bildschirm::doTastaturEreignis( TastaturEreignis &te ) // sendet tastatur Ereignis
{
    lock();
    if( !renderOnTop )
        members->sendTastaturAll( te );
    else if( onTop )
        onTop->doTastaturEreignis( te );
    unlock();
}

void Bildschirm::addToolTip( ToolTip *tip ) // fügt ToolTip hinzu
{
    lock();
    tips->add( tip, tipAnzahl );
    ++tipAnzahl;
    rend = 1;
    unlock();
}

bool Bildschirm::removeToolTip( ToolTip *zTip ) // entfernt ToolTip
{
    lock();
    bool gefunden = 0;
    for( int i = 0; i < tipAnzahl; ++i )
    {
        ToolTip *tmp = tips->z( i );
        if( tmp == zTip )
        {
            tips->lösche( i );
            --tipAnzahl;
            gefunden = 1;
            rend = 1;
            break;
        }
    }
    unlock();
    return gefunden;
}

// constant 
Bild *Bildschirm::getRenderBild() const
{
    return renderB->getThis();
}

Bild *Bildschirm::zRenderBild() const
{
    return renderB;
}

ZeichnungArray *Bildschirm::getMembers() const // gibt die Zeichnunge zurück
{
    return members;
}

int Bildschirm::getFüllFarbe() const // gibt die Füll Farbe zurück
{
    return füllFarbe;
}

bool Bildschirm::istVolbild() const // gibt zurück, ob vollbild an ist
{
    return vollbild;
}

const Punkt &Bildschirm::getBackBufferGröße() const // gibt die Größe des Backbuffers zurück
{
    return backBufferGröße;
}

void Bildschirm::warteAufRendern() const // wartet auf die render Funktion
{
    while( rendering )
    {
        if( !rendering )
            return;
    }
}

double Bildschirm::getRenderZeit() const // gibt zurück wie viele Sekunden das Rendern dauert
{
    return renderZeit->getSekunden();
}

// Reference Counting 
Bildschirm *Bildschirm::getThis()
{
    ++ref;
    return this;
}

Bildschirm *Bildschirm::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

int MonitorEnum( HMONITOR m, HDC dc, LPRECT r, LPARAM p )
{
    Monitor *mon = new Monitor();
    mon->existiert = 1;
    mon->x = r->left;
    mon->y = r->top;
    mon->breite = r->right - r->left;
    mon->höhe = r->bottom - r->top;
    ( ( Array< Monitor* >* )p )->add( mon );
    return 1;
}

Monitor Framework::getMonitor( int id )
{
    if( id < 0 )
    {
        Monitor m;
        m.existiert = 0;
        return m;
    }
    Array< Monitor* > *monitore = new Array< Monitor* >();
    EnumDisplayMonitors( 0, 0, (MONITORENUMPROC)MonitorEnum, (LPARAM)monitore );
    if( id >= monitore->getEintragAnzahl() )
    {
        for( int i = 0; monitore->get( i ); ++i )
            delete monitore->get( i );
        delete monitore;
        Monitor m;
        m.existiert = 0;
        return m;
    }
    Monitor m = *monitore->get( id );
    int anz = monitore->getEintragAnzahl();
    for( int i = 0; i < anz; ++i )
        delete monitore->get( i );
    delete monitore;
    return m;
}

// Bildschirm2D
// Konstruktor 
Bildschirm2D::Bildschirm2D( WFenster *fenster )
    : Bildschirm( fenster ),
    pDirect3D( 0 ),
    pDevice( 0 ),
    pBackBuffer( 0 ),
    backRect( new D3DLOCKED_RECT() )
{}

// Destruktor 
Bildschirm2D::~Bildschirm2D()
{
    cleanUpDirectX();
    delete backRect;
}

// private
void Bildschirm2D::cleanUpDirectX()
{
    backRect->pBits = NULL;
    if( pDevice )
    {
        pDevice->Release();
        pDevice = NULL;
    }
    if( pDirect3D )
    {
        pDirect3D->Release();
        pDirect3D = NULL;
    }
    if( pBackBuffer )
    {
        pBackBuffer->Release();
        pBackBuffer = NULL;
    }
}

// nicht constant 
void Bildschirm2D::update() // aktualisiert directX
{
    lock();
    HRESULT result;

    cleanUpDirectX();

    pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = !vollbild;
    d3dpp.hDeviceWindow = fenster->getFensterHandle();
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    if( !backBufferGröße.x || !backBufferGröße.y )
        backBufferGröße = fenster->getKörperGröße();
    d3dpp.BackBufferHeight = backBufferGröße.y;
    d3dpp.BackBufferWidth = backBufferGröße.x;
    if( renderB )
        renderB->release();
    renderB = new Bild( 1 );
    renderB->neuBild( backBufferGröße.x, backBufferGröße.y, füllFarbe );

    result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fenster->getFensterHandle(),
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice );
    if( pDevice )
        result = pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    rend = 1;
    unlock();
}

void Bildschirm2D::render() // Zeichnet das Bild
{
    if( !rend && testRend )
        return;
    rendering = 1;
    int count = 0;
    if( renderB && pDevice )
    {
        lock();
        renderZeit->messungStart();
        if( füll )
            renderB->setFarbe( füllFarbe );
        if( renderZeichnungen )
        {
            if( renderOnTop && deckFarbe && ( deckFarbe & ( füllFarbe | 0xFF000000 ) ) == deckFarbe )
            {
                renderB->setAlpha( 255 - (unsigned char)( deckFarbe >> 24 ) );
                members->render( *renderB ); // zeichnen nach zwischenbuffer
                renderB->releaseAlpha();
            }
            else
            {
                members->render( *renderB ); // zeichnen nach zwischenbuffer
                if( renderOnTop && deckFarbe )
                    renderB->alphaRegion( 0, 0, renderB->getBreite(), renderB->getHöhe(), deckFarbe );
            }
            for( int i = 0; i < tipAnzahl; ++i )
                tips->z( i )->render( *renderB );
        }
        if( renderOnTop && onTop )
            onTop->render( *renderB );
        Bild *tmp = renderB->getThis();
        unlock();
        // Beginne Bild 
        HRESULT result;
        if( !füllFarbe )
            result = pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 0.0f, 0 );
        result = pBackBuffer->LockRect( backRect, 0, 0 );
        // kopieren zum Bildschrirm 
        int *bgBuff = tmp->getBuffer();
        int tmpBr = sizeof( D3DCOLOR )* tmp->getBreite();
        for( int y = 0, pitch = 0, bry = 0; y < tmp->getHöhe(); ++y, pitch += backRect->Pitch, bry += tmp->getBreite() )
            memcpy( &( (BYTE *)backRect->pBits )[ pitch ], (void*)&( bgBuff[ bry ] ), tmpBr );
        // Beende Bild 
        result = pBackBuffer->UnlockRect();
        tmp->release();
        result = pDevice->Present( 0, 0, 0, 0 );
        renderZeit->messungEnde();
        if( result != S_OK )
        {
            ++count;
            update();
        }
        else if( count )
            --count;
    }
    if( !pDevice )
    {
        ++count;
        update();
    }
    if( count > 10 )
    {
        WMessageBox( fenster ? fenster->getFensterHandle() : 0, new Text( "Fehler" ), new Text( "Es ist ein Fehler beim rendern aufgetreten." ), MB_ICONERROR );
        count = 0;
    }
    rendering = 0;
    rend = 0;
}

Bildschirm *Bildschirm2D::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Bildschirm3D
// Konstruktor 
Bildschirm3D::Bildschirm3D( WFenster *fenster )
    : Bildschirm( fenster ),
    d3d11Device( 0 ),
    d3d11Context( 0 ),
    d3d11SpawChain( 0 ),
    frameworkTextur( 0 ),
    vertexBuffer( 0 ),
    indexBuffer( 0 ),
    vertexShader( 0 ),
    pixelShader( 0 ),
    sampleState( 0 ),
    rtview( 0 ),
    dsView( 0 ),
    depthStencilBuffer( 0 ),
    depthStencilState( 0 ),
    depthDisabledStencilState( 0 ),
    blendStateAlphaBlend( 0 ),
    kameras( new RCArray< Kam3D >() ),
    rend3D( 0 ),
    vp( 0 ),
    renderObj( new Render3D() )
{}

// Destruktor 
Bildschirm3D::~Bildschirm3D()
{
    kameras->release();
    renderObj->release();
    cleanUpDirectX();
}

// private
void Bildschirm3D::cleanUpDirectX()
{
    if( d3d11Device )
    {
        d3d11Device->Release();
        d3d11Device = NULL;
    }
    if( d3d11Context )
    {
        d3d11Context->Release();
        d3d11Context = NULL;
    }
    if( d3d11SpawChain )
    {
        d3d11SpawChain->Release();
        d3d11SpawChain = NULL;
    }
    if( frameworkTextur )
    {
        frameworkTextur->release();
        frameworkTextur = NULL;
    }
    if( vertexBuffer )
    {
        vertexBuffer->release();
        vertexBuffer = NULL;
    }
    if( indexBuffer )
    {
        indexBuffer->release();
        indexBuffer = NULL;
    }
    if( pixelShader )
    {
        pixelShader->release();
        pixelShader = NULL;
    }
    if( vertexShader )
    {
        vertexShader->release();
        vertexShader = NULL;
    }
    if( sampleState )
    {
        sampleState->Release();
        sampleState = NULL;
    }
    if( rtview )
    {
        rtview->Release();
        rtview = NULL;
    }
    if( dsView )
    {
        dsView->Release();
        dsView = NULL;
    }
    if( depthStencilBuffer )
    {
        depthStencilBuffer->Release();
        depthStencilBuffer = NULL;
    }
    if( depthStencilState )
    {
        depthStencilState->Release();
        depthStencilState = NULL;
    }
    if( depthDisabledStencilState )
    {
        depthDisabledStencilState->Release();
        depthDisabledStencilState = NULL;
    }
    if( blendStateAlphaBlend )
    {
        blendStateAlphaBlend->Release();
        blendStateAlphaBlend = NULL;
    }
    delete vp;
    vp = 0;
}

// nicht constant 
void Bildschirm3D::addKamera( Kam3D *obj ) // Fügt ein Zeichnung hinzu
{
    lock();
    kameras->add( obj );
    rend3D = 1;
    unlock();
}

void Bildschirm3D::removeKamera( Kam3D *zObj ) // Entfernt ein Zeichnung
{
    lock();
    for( int i = 0; kameras->z( i ); i++ )
    {
        if( kameras->z( i ) == zObj )
        {
            kameras->lösche( i );
            break;
        }
    }
    rend3D = 1;
    unlock();
}

void Bildschirm3D::update() // aktualisiert directX
{
    lock();
    HRESULT result;
    cleanUpDirectX();
    //--------------------------------------------------------------------
    // Create Device

    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory( &scd, sizeof( DXGI_SWAP_CHAIN_DESC ) );

    // fill the swap chain description struct
    scd.BufferCount = 1;                                           // one back buffer
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;             // how swap chain is to be used
    scd.OutputWindow = fenster ? fenster->getFensterHandle() : 0;  // the window to be used
    scd.SampleDesc.Count = 1;
    // Set the scan line ordering and scaling to unspecified.
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.Windowed = !vollbild;
    if( !backBufferGröße.x || !backBufferGröße.y )
        backBufferGröße = fenster ? fenster->getKörperGröße() : Punkt( 0, 0 );
    scd.BufferDesc.Width = backBufferGröße.x;
    scd.BufferDesc.Height = backBufferGröße.y;                 // windowed/full-screen mode
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // use 32-bit color
                                                              // Discard the back buffer contents after presenting.
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if( renderB )
        renderB->release();
    renderB = new Bild( 1 );
    renderB->setAlpha3D( 1 );
    renderB->neuBild( backBufferGröße.x, backBufferGröße.y, füllFarbe );

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL support = D3D_FEATURE_LEVEL_11_0;
    // create a device, device context and swap chain using the information in the scd struct
    UINT flag = 0;
#ifdef _DEBUG
    flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    result = D3D11CreateDeviceAndSwapChain( NULL,
                                            D3D_DRIVER_TYPE_HARDWARE,
                                            NULL,
                                            flag,
                                            &featureLevel,
                                            1,
                                            D3D11_SDK_VERSION,
                                            &scd,
                                            &d3d11SpawChain,
                                            &d3d11Device,
                                            &support,
                                            &d3d11Context );

    ID3D11Texture2D *backBufferPtr;
    // Get the pointer to the back buffer.
    result = d3d11SpawChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backBufferPtr );
    // Create the render target view with the back buffer pointer.
    result = d3d11Device->CreateRenderTargetView( backBufferPtr, NULL, &rtview );
    // Release pointer to the back buffer as we no longer need it.
    backBufferPtr->Release();
    // Initialize the description of the depth buffer.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );
    // Set up the description of the depth buffer.
    depthBufferDesc.Width = backBufferGröße.x;
    depthBufferDesc.Height = backBufferGröße.y;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    // Create the texture for the depth buffer using the filled out description.
    result = d3d11Device->CreateTexture2D( &depthBufferDesc, NULL, &depthStencilBuffer );

    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // Create the depth stencil state.
    result = d3d11Device->CreateDepthStencilState( &depthStencilDesc, &depthStencilState );

    d3d11Context->OMSetDepthStencilState( depthStencilState, 1 );

    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // Create the depth stencil view.
    result = d3d11Device->CreateDepthStencilView( depthStencilBuffer, &depthStencilViewDesc, &dsView );

    d3d11Context->OMSetRenderTargets( 1, &rtview, dsView );

    vp = new D3D11_VIEWPORT();
    memset( vp, 0, sizeof( D3D11_VIEWPORT ) );
    vp->Width = (float)backBufferGröße.x;
    vp->Height = (float)backBufferGröße.y;
    vp->MinDepth = 0.0f;
    vp->MaxDepth = 1.0f;
    vp->TopLeftX = 0.0f;
    vp->TopLeftY = 0.0f;

    d3d11Context->RSSetViewports( 1, vp );

    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
    // Clear the second depth stencil state before setting the parameters.
    ZeroMemory( &depthDisabledStencilDesc, sizeof( depthDisabledStencilDesc ) );
    
    // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
    // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
    depthDisabledStencilDesc.DepthEnable = false;
    depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDisabledStencilDesc.StencilEnable = true;
    depthDisabledStencilDesc.StencilReadMask = 0xFF;
    depthDisabledStencilDesc.StencilWriteMask = 0xFF;
    depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    
    // Create the state using the device.
    result = d3d11Device->CreateDepthStencilState( &depthDisabledStencilDesc, &depthDisabledStencilState );

    //-------------------------------------------------
    // Shaders
    Text shader;
    getVertexShader( shader );
    vertexShader = new VertexShader();
    vertexShader->setShaderCode( &shader );
    vertexShader->compile( d3d11Device, "TextureVertexShader", "5_0" );

    getPixelShader( shader );
    pixelShader = new PixelShader();
    pixelShader->setShaderCode( &shader );
    pixelShader->compile( d3d11Device, "TexturePixelShader", "5_0" );

    D3D11_INPUT_ELEMENT_DESC polygonLayout[ 2 ];
    // Create the vertex input layout description.
    // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
    polygonLayout[ 0 ].SemanticName = "POSITION";
    polygonLayout[ 0 ].SemanticIndex = 0;
    polygonLayout[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[ 0 ].InputSlot = 0;
    polygonLayout[ 0 ].AlignedByteOffset = 0;
    polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[ 0 ].InstanceDataStepRate = 0;

    polygonLayout[ 1 ].SemanticName = "TEXCOORD";
    polygonLayout[ 1 ].SemanticIndex = 0;
    polygonLayout[ 1 ].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[ 1 ].InputSlot = 0;
    polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[ 1 ].InstanceDataStepRate = 0;

    vertexShader->erstelleInputLayout( d3d11Device, polygonLayout, 2 );
    vertexShader->erstelleConstBuffer( d3d11Device, sizeof( Mat4< float > ) * 3, 0 );

    // Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[ 0 ] = 0;
    samplerDesc.BorderColor[ 1 ] = 0;
    samplerDesc.BorderColor[ 2 ] = 0;
    samplerDesc.BorderColor[ 3 ] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    result = d3d11Device->CreateSamplerState( &samplerDesc, &sampleState );

    //---------------------------------------------------------------
    // Framework Backbuffer Texture


    unsigned long indices[ 6 ];
    Vertex3D vertices[ 6 ];

    // Load the index array with data.
    for( int i = 0; i < 6; i++ )
    {
        indices[ i ] = i;
    }
    vertexBuffer = new DXVertexBuffer( sizeof( Vertex3D ) );
    vertexBuffer->setLänge( sizeof( vertices ) );
    vertexBuffer->setData( vertices );

    indexBuffer = new DXIndexBuffer( sizeof( int ) );
    indexBuffer->setLänge( sizeof( int ) * 6 );
    indexBuffer->setData( indices );

    frameworkTextur = new Textur();
    frameworkTextur->setBildZ( renderB->getThis() );

    float left, right, top, bottom;
    // Calculate the screen coordinates of the left side of the bitmap.
    left = (float)( ( backBufferGröße.x / 2.0 ) * -1 );

    // Calculate the screen coordinates of the right side of the bitmap.
    right = left + (float)backBufferGröße.x;

    // Calculate the screen coordinates of the top of the bitmap.
    top = (float)( backBufferGröße.y / 2.0 );

    // Calculate the screen coordinates of the bottom of the bitmap.
    bottom = top - (float)backBufferGröße.y;

    // Load the vertex array with data.
    // First triangle.
    vertices[ 0 ].pos = Vec3< float >( left, top, 0.0f );  // Top left.
    vertices[ 0 ].tPos = Vec2< float >( 0.0f, 0.0f );

    vertices[ 1 ].pos = Vec3< float >( right, bottom, 0.0f );  // Bottom right.
    vertices[ 1 ].tPos = Vec2< float >( 1.0f, 1.0f );

    vertices[ 2 ].pos = Vec3< float >( left, bottom, 0.0f );  // Bottom left.
    vertices[ 2 ].tPos = Vec2< float >( 0.0f, 1.0f );

    // Second triangle.
    vertices[ 3 ].pos = Vec3< float >( left, top, 0.0f );  // Top left.
    vertices[ 3 ].tPos = Vec2< float >( 0.0f, 0.0f );

    vertices[ 4 ].pos = Vec3< float >( right, top, 0.0f );  // Top right.
    vertices[ 4 ].tPos = Vec2< float >( 1.0f, 0.0f );

    vertices[ 5 ].pos = Vec3< float >( right, bottom, 0.0f );  // Bottom right.
    vertices[ 5 ].tPos = Vec2< float >( 1.0f, 1.0f );


    D3D11_BLEND_DESC blendState;
    ZeroMemory( &blendState, sizeof( D3D11_BLEND_DESC ) );
    blendState.AlphaToCoverageEnable = false;
    blendState.IndependentBlendEnable = false;
    blendState.RenderTarget[ 0 ].BlendEnable = true;
    blendState.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendState.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendState.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
    blendState.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendState.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;
    blendState.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendState.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    d3d11Device->CreateBlendState( &blendState, &blendStateAlphaBlend );
    d3d11Context->OMSetBlendState( blendStateAlphaBlend, 0, 0xFFFFFFFF );

    // Setup Render Objekt
    if( renderObj )
        renderObj->release();
    renderObj = new Render3D();
    d3d11Device->AddRef();
    renderObj->setDevice( d3d11Device );
    d3d11Context->AddRef();
    renderObj->setContext( d3d11Context );

    renderObj->benutzeShader( VERTEX, vertexShader->getThis() );
    d3d11Context->PSSetSamplers( 0, 1, &sampleState );
    renderObj->benutzeShader( PIXEL, pixelShader->getThis() );

    vertexBuffer->copieren( renderObj );
    indexBuffer->copieren( renderObj );

    rend = 1;
    unlock();
}

void Bildschirm3D::tick( double tickval )
{
    lock();
    __super::tick( tickval );
    for( auto i = kameras->getArray(); i.set; i++ )
        rend3D |= i.var->tick( tickval );
    unlock();
}

void Bildschirm3D::doMausEreignis( MausEreignis &me ) // sendet maus Ereignis
{
    lock();
    __super::doMausEreignis( me );
    for( int i = kameras->getEintragAnzahl() - 1; i >= 0; i-- )
        kameras->z( i )->doMausEreignis( me );
    unlock();
}

void Bildschirm3D::doTastaturEreignis( TastaturEreignis &te ) // sendet tastatur Ereignis
{
    lock();
    __super::doTastaturEreignis( te );
    for( int i = kameras->getEintragAnzahl() - 1; i >= 0; i-- )
        kameras->z( i )->doTastaturEreignis( te );
    unlock();
}

void Bildschirm3D::render() // Zeichnet das Bild
{
    if( !rend && !rend3D && testRend )
        return;
    rendering = 1;
    int count = 0;
    if( renderB && d3d11Device )
    {
        lock();
        renderZeit->messungStart();
        float color[ 4 ];
        // Setup the color to clear the buffer to.
        color[ 0 ] = ( ( füllFarbe >> 16 ) & 0xFF ) / 255.f; // R
        color[ 1 ] = ( ( füllFarbe >> 8 ) & 0xFF ) / 255.f; // G
        color[ 2 ] = ( füllFarbe & 0xFF ) / 255.f; // B
        color[ 3 ] = ( ( füllFarbe >> 24 ) & 0xFF ) / 255.f; // A
        // Clear the back buffer.
        if( rend3D || !testRend || rend )
        {
            d3d11Context->ClearRenderTargetView( rtview, color );
            // Clear the depth buffer.
            d3d11Context->ClearDepthStencilView( dsView, D3D11_CLEAR_DEPTH, 1, 0 );
            // Bind the render target view and depth stencil buffer to the output render pipeline.
            d3d11Context->OMSetRenderTargets( 1, &rtview, dsView );

            // Set the depth stencil state.
            d3d11Context->OMSetDepthStencilState( depthStencilState, 1 );

            for( auto i = kameras->getArray(); i.set; i++ )
                i.var->render( renderObj );
        }
        // Set the depth stencil state.
        d3d11Context->OMSetDepthStencilState( depthDisabledStencilState, 1 );

        if( rend || !testRend )
        {
            if( füll )
                renderB->setFarbe( füllFarbe );
            if( renderZeichnungen )
            {
                if( renderOnTop && deckFarbe && ( deckFarbe & ( füllFarbe | 0xFF000000 ) ) == deckFarbe )
                {
                    renderB->setAlpha( 255 - (unsigned char)( deckFarbe >> 24 ) );
                    members->render( *renderB ); // zeichnen nach zwischenbuffer
                    renderB->releaseAlpha();
                }
                else
                {
                    members->render( *renderB ); // zeichnen nach zwischenbuffer
                    if( renderOnTop && deckFarbe )
                        renderB->alphaRegion( 0, 0, renderB->getBreite(), renderB->getHöhe(), deckFarbe );
                }
                for( int i = 0; i < tipAnzahl; ++i )
                    tips->z( i )->render( *renderB );
            }
            if( renderOnTop && onTop )
                onTop->render( *renderB );
        }
        unlock();
        // Beginne Bild 
        HRESULT result;

        if( rend || !testRend )
            frameworkTextur->updateTextur( renderObj );

        d3d11Context->RSSetViewports( 1, vp );

        Mat4< float > welt = welt.identity();
        float screenAspect = (float)backBufferGröße.x / (float)backBufferGröße.y;
        renderObj->setKameraMatrix( welt.translation( Vec3< float >( 0.f, 0.f, backBufferGröße.y * 1.2075f ) ), welt.projektion( DirectX::XM_PI / 4.0f, screenAspect, 0.1f, 10000.f ), Vec3< float >( 0.f, 0.f, backBufferGröße.y * 1.2075f ) );
        renderObj->beginnModel( welt, vertexBuffer, -1 );
        renderObj->draw( indexBuffer, frameworkTextur );

        result = d3d11SpawChain->Present( 0, 0 );
        renderZeit->messungEnde();
#ifdef _DEBUG
        std::cout << renderZeit->getSekunden() << "\n";
#endif
        if( result != S_OK )
        {
            ++count;
            update();
        }
        else if( count )
            --count;
    }
    if( !d3d11Device )
    {
        ++count;
        update();
    }
    if( count > 10 )
    {
        WMessageBox( fenster ? fenster->getFensterHandle() : 0, new Text( "Fehler" ), new Text( "Es ist ein Fehler beim rendern aufgetreten." ), MB_ICONERROR );
        count = 0;
    }
    rendering = 0;
    rend = 0;
}

// Reference Counting 
Bildschirm *Bildschirm3D::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}