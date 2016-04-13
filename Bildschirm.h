#ifndef Bildschirm_H
#define Bildschirm_H

#include "Array.h"
#include "Punkt.h"

// DirectX 11 Types

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct D3D11_VIEWPORT;

// DirectX 9 Types

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;
struct _D3DLOCKED_RECT;

namespace Framework
{
    class Bild; // Bild.h
    class WFenster; // Fenster.h
    class Bildschirm; // aus dieser Datei
    class ZeichnungArray; // Zeichnung.h
    class Zeichnung3DArray; // Zeichnung3D.h
    class Zeichnung; // Zeichnung.h
    class Zeichnung3D; // Zeichnung3D.h
    class ZeitMesser; // Zeit.h
    struct MausEreignis; // MausEreignis.h
    struct TastaturEreignis; // TastaturEreignis.h
    class ToolTip; // Tooltip.h
    class PixelShader; // Shader.h
    class VertexShader; // Shader.h
    class Kam3D; // Kam3D.h
    class Render3D; // Render3D.h
    class DXVertexBuffer; // DXBuffer.h
    class DXIndexBuffer; // DXBuffer.h
    class Textur; // Textur.h

    struct Monitor
    {
        int x, y, breite, höhe;
        bool existiert;
    };

    //Diese Klasse wird das Bild auf dem Bildschirm verwalten
    class Bildschirm
    {
    protected:
        WFenster *fenster;
        Bild *renderB;
        int ref;
        ZeichnungArray *members;
        int füllFarbe;
        int deckFarbe;
        Zeichnung *onTop;
        bool renderOnTop;
        bool renderZeichnungen;
        bool vollbild;
        bool rendering;
        ZeitMesser *renderZeit;
        Punkt backBufferGröße;
        CRITICAL_SECTION cs;
        RCArray< ToolTip > *tips;
        int tipAnzahl;
        bool testRend;
        bool füll;
        bool rend;

    public:
        // Konstruktor 
        __declspec( dllexport ) Bildschirm( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) ~Bildschirm();
        // nicht constant 
        __declspec( dllexport ) virtual void lock();
        __declspec( dllexport ) virtual void unlock();
        __declspec( dllexport ) virtual void setFüll( bool f );
        __declspec( dllexport ) virtual void update() = 0; // aktualisiert directX
        __declspec( dllexport ) virtual void setTestRend( bool tr ); // legt fest, ob vo rendern auf updates geprüft werden soll
        __declspec( dllexport ) virtual void setRenderZeichnungen( bool rO ); // legt fest, ob die Zeichnunge gerendert werden
        __declspec( dllexport ) virtual void setOnTop( bool onTop ); // legt fest, ob das onTop Zeichnung gerendert wid
        __declspec( dllexport ) virtual void setOnTopZeichnung( Zeichnung *obj ); // setzt das OnTop Zeichnung
        __declspec( dllexport ) virtual void setdeckFarbe( int f ); // setzt die deckFarbe
        __declspec( dllexport ) virtual void addMember( Zeichnung *obj ); // Fügt ein Zeichnung hinzu
        __declspec( dllexport ) virtual void removeMember( Zeichnung *obj ); // Entfernt ein Zeichnung
        __declspec( dllexport ) virtual void render() = 0; // Zeichnet das Bild
        __declspec( dllexport ) virtual void setFüllFarbe( int f ); // setzt die Fill Farbe
        __declspec( dllexport ) virtual void setVollbild( bool fullscreen ); // setzt vollbild
        __declspec( dllexport ) virtual void tick( double tickval );
        __declspec( dllexport ) virtual void setBackBufferGröße( int breite, int höhe ); // setzt die Größe des Backbuffers
        __declspec( dllexport ) virtual void setBackBufferGröße( Punkt &größe );
        __declspec( dllexport ) virtual void doMausEreignis( MausEreignis &me ); // sendet maus Ereignis
        __declspec( dllexport ) virtual void doTastaturEreignis( TastaturEreignis &te ); // sendet tastatur Ereignis
        __declspec( dllexport ) virtual void addToolTip( ToolTip *tip ); // fügt ToolTip hinzu
        __declspec( dllexport ) virtual bool removeToolTip( ToolTip *zTip ); // entfernt ToolTip
        // constant 
        __declspec( dllexport ) virtual Bild *getRenderBild() const; // Gibt das RendezRObj zurück
        __declspec( dllexport ) virtual Bild *zRenderBild() const;
        __declspec( dllexport ) virtual ZeichnungArray *getMembers() const; // gibt die Zeichnunge zurück
        __declspec( dllexport ) virtual int getFüllFarbe() const; // gibt die Füll Farbe zurück
        __declspec( dllexport ) virtual bool istVolbild() const; // gibt zurück, ob vollbild an ist
        __declspec( dllexport ) virtual const Punkt &getBackBufferGröße() const; // gibt die Größe des Backbuffers zurück
        __declspec( dllexport ) virtual void warteAufRendern() const; // wartet auf die render Funktion
        __declspec( dllexport ) virtual double getRenderZeit() const; // gibt zurück wie viele Sekunden das Rendern dauert
        // Reference Counting 
        __declspec( dllexport ) virtual Bildschirm *getThis();
        __declspec( dllexport ) virtual Bildschirm *release();
    };

    class Bildschirm2D : public Bildschirm
    {
    private:
        IDirect3D9			*pDirect3D;
        IDirect3DDevice9	*pDevice;
        IDirect3DSurface9	*pBackBuffer;
        _D3DLOCKED_RECT		*backRect;

        void cleanUpDirectX();

    public:
        // Konstruktor 
        __declspec( dllexport ) Bildschirm2D( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) ~Bildschirm2D();
        // nicht constant 
        __declspec( dllexport ) virtual void update(); // aktualisiert directX
        __declspec( dllexport ) virtual void render(); // Zeichnet das Bild
        // Reference Counting 
        __declspec( dllexport ) virtual Bildschirm *release();
    };

    class Bildschirm3D : public Bildschirm
    {
    private:
        ID3D11Device *d3d11Device;
        ID3D11DeviceContext *d3d11Context;
        IDXGISwapChain *d3d11SpawChain;
        DXVertexBuffer *vertexBuffer;
        DXIndexBuffer *indexBuffer;
        Textur *frameworkTextur;
        ID3D11SamplerState* sampleState;
        ID3D11RenderTargetView *rtview;
        ID3D11DepthStencilView *dsView;
        ID3D11Texture2D *depthStencilBuffer;
        ID3D11DepthStencilState *depthStencilState;
        ID3D11DepthStencilState *depthDisabledStencilState;
        ID3D11BlendState *blendStateAlphaBlend;
        RCArray< Kam3D > *kameras;
        PixelShader *pixelShader;
        VertexShader *vertexShader;
        Render3D *renderObj;
        D3D11_VIEWPORT *vp;
        bool rend3D;

        void cleanUpDirectX();

    public:
        // Konstruktor 
        __declspec( dllexport ) Bildschirm3D( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) ~Bildschirm3D();
        // nicht constant 
        __declspec( dllexport ) void addKamera( Kam3D *obj ); // Fügt eine Kamera hinzu
        __declspec( dllexport ) void removeKamera( Kam3D *zObj ); // Entfernt eine Kamera
        __declspec( dllexport ) void tick( double tickval );
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ); // sendet maus Ereignis
        __declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ); // sendet tastatur Ereignis
        __declspec( dllexport ) void update(); // aktualisiert directX
        __declspec( dllexport ) void render(); // Zeichnet das Bild
        // Reference Counting  
        __declspec( dllexport ) Bildschirm *release();
    };

    __declspec( dllexport ) Monitor getMonitor( int id );
}

#endif