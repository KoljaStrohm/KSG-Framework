#ifndef Bildschirm_H
#define Bildschirm_H

#include "Array.h"
#include "Punkt.h"
#include "Critical.h"

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
    class TexturModel; // TexturModel.h

    // Eine Struktur, die Informationen über einen Monitor beinhaltet
    struct Monitor
    {
        int x, y, breite, height; // Koordinaten des Monitors und die Auflösung
        bool existiert; // Speichert, ob der Monitor wirklich existiert
    };

    // Diese Klasse verwaltet das Bild auf dem Bildschirm
    class Bildschirm
    {
    protected:
        WFenster *fenster;
        Bild *renderB;
        int ref;
        ZeichnungArray *members;
        int fillColor;
        int deckFarbe;
        Zeichnung *onTop;
        bool renderOnTop;
        bool renderZeichnungen;
        bool vollbild;
        bool rendering;
        ZeitMesser *renderZeit;
        Punkt backBufferSize;
        Critical cs;
        RCArray< ToolTip > *tips;
        int tipAnzahl;
        bool testRend;
        bool fill;
        bool rend;

    public:
        // Konstruktor 
        //  fenster: Das Fenster, dessen Inhalt verwaltet werden soll
        __declspec( dllexport ) Bildschirm( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) virtual ~Bildschirm();
        // Dies ist notwendig, falls mehrere Threads gleichzeitig den Bildschirm benutzen.
        // Wenn lock() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlock() aufgerufen hat.
        __declspec( dllexport ) virtual void lock();
        // Dies ist notwendig, falls mehrere Threads gleichzeitig den Bildschirm benutzen.
        // Wenn lock() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlock() aufgerufen hat.
        __declspec( dllexport ) virtual void unlock();
        // Legt fest, ob der Bildschirm nach jedem Bild neu mit einer Farbe ausgefüllt wird (Standartmäßig gesetzt)
        //  f: 1, falls das Bild vor dem Zeichnen zurückgesetzt werden soll
        __declspec( dllexport ) virtual void setFill( bool f );
        // Aktualisiert die Objekte, mit der die Grafikkarte verwaltet wird
        __declspec( dllexport ) virtual void update() = 0;
        // Legt fest, ob von dem Zeichnen überprüft wird, ob sich das Bild vom letzten unterscheidet (Standartmäßig gesetzt)
        //  tr: 1, wenn nur gezeichnet werden soll, wenn es notwendig ist
        __declspec( dllexport ) virtual void setTestRend( bool tr );
        // Legt fest, ob die 2D GUI Zeichnungen gezeichnet werden sollen (Standartmäßig gesetzt)
        //  rO: 1, falls die Zeichnungen gezeichnet werden sollen
        __declspec( dllexport ) virtual void setRenderZeichnungen( bool rO );
        // Legt fest, ob eine Zeichnung über allen anderen Zeichnungen gezeichnet werden soll (Standartmäßig nicht gesetzt)
        //  onTop: 1, falls eine Zeichnung über allem gezeichnet werden soll
        __declspec( dllexport ) virtual void setOnTop( bool onTop );
        // Setzt die Zecihnung, die über allem gezeichnet werden soll, falls setOnTop( 1 ) gesetzt wurde
        //  obj: Die Zeichnung
        __declspec( dllexport ) virtual void setOnTopZeichnung( Zeichnung *obj );
        // Setzt eine Farbe, die nach den Zeichnungen mit alphablending über das gesamte Bild geblendet wird, wenn setOnTop( 1 ) gesetzt wurde. Die zeichnung, die mit setOnTopZeichnung() gesetzt wurde ist davon nicht betroffen
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) virtual void setdeckFarbe( int f );
        // Fügt dem Bild eine Zeichnung hinzu, die immer gezeichnet wird, wenn setRenderZeichnungen( 1 ) gesetzt wurde.
        //  obj: Die Zeichnung
        __declspec( dllexport ) virtual void addMember( Zeichnung *obj );
        // Entfernt eine Zeichnung aus dem Bild. Darf nicht aufgerufen werden, wärend doMausEreignis(), doTastaturEreignis(), tick() oder render() aufgerufen wird.
        //  obj: Die Zeichnung
        __declspec( dllexport ) virtual void removeMember( Zeichnung *obj );
        // Zeichnet ein Bild und präsentiert es auf dem Bildschirm
        __declspec( dllexport ) virtual void render() = 0;
        // Setzt die Farbe, mit der das Bild for dem Zeichnen gefüllt wird, wenn setFüll( 1 ) gesetzt wurde
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) virtual void setFillFarbe( int f );
        // Legt fest, ob das Bild im Vollbildmodus präsentiert werden soll. (Standartmäßig nicht gesetzt)
        //  fullscreen: 1, für Vollbildmodus
        __declspec( dllexport ) virtual void setVollbild( bool fullscreen );
        // Verarbeitet die Zeit, die seit dem letzten Aufruf der Funktion vergangen ist
        // Ruft die tick Funktionen aller sich im Bild befindenden Zeichnungen auf
        //  tickval: Die vergangene Zeit in Sekunden
        __declspec( dllexport ) virtual void tick( double tickval );
        // Setzt die Auflösung des Bildes, das angezeigt wird. Es wird von der Grafikkarte automatisch skalliert, so das es das Fenster ausfüllt
        //  breite: Die Breite des Bildes in Pixeln
        //  height: Die Höhe des Bildes in Pixeln
        __declspec( dllexport ) virtual void setBackBufferSize( int breite, int height );
        // Setzt die Auflösung des Bildes, das angezeigt wird. Es wird von der Grafikkarte automatisch skalliert, so das es das Fenster ausfüllt
        //  size: Die Breite und Höhe in Pixeln
        __declspec( dllexport ) virtual void setBackBufferSize( Punkt &size );
        // Verarbeitet ein Maus Ereignis. Wird vom Framework automatisch aufgerufen.
        // Gibt das Ereignis automatisch an alle sich im Bild befindenden Zeichnungen weiter
        //  me: Das Ereignis
        __declspec( dllexport ) virtual void doMausEreignis( MausEreignis &me );
        // Verarbeitet ein Tastatur Ereignis. Wird vom Framework automatisch aufgerufen
        // Gibt das Ereignis automatisch an alle sich im Bild befindenden Zeichnungen weiter
        //  te: Das Ereignis
        __declspec( dllexport ) virtual void doTastaturEreignis( TastaturEreignis &te );
        // Fügt einen Tooltip hinzu.
        //  tip: Der Tooltip
        __declspec( dllexport ) virtual void addToolTip( ToolTip *tip );
        // Entfernt einen Tooltip
        //  zTip: Der Tooltip ohne erhöhten reference Counter
        __declspec( dllexport ) virtual bool removeToolTip( ToolTip *zTip );
        // Gibt das Bild zurück, in das die 2D GUI Zeichnungen des Frameworks gezeichnet werden
        __declspec( dllexport ) virtual Bild *getRenderBild() const;
        // Gibt das Bild ohne erhöhten Reference Counter zurück, in das die 2D GUI Zeichnungen des Frameworks gezeichnet werden
        __declspec( dllexport ) virtual Bild *zRenderBild() const;
        // Gibt ein Array von 2D GUI Zeichnungen zurück, die sich im Bild befinden
        __declspec( dllexport ) virtual ZeichnungArray *getMembers() const;
        // Gibt die Farbe im A8R8G8B8 Format zurück, mit der das Bild vor dem Zeichnen befüllt wird
        __declspec( dllexport ) virtual int getFillFarbe() const;
        // Gibt zurück, ob man sich im Vollbildmodus befindet
        __declspec( dllexport ) virtual bool istVolbild() const;
        // Gibt die Auflösung in Pixeln zurück, in der gezeichnet wird
        __declspec( dllexport ) virtual const Punkt &getBackBufferSize() const;
        // Wartet, bis das Zeichnen des aktuellen Bildes abgeschlossen ist
        __declspec( dllexport ) virtual void warteAufRendern() const;
        // Gibt die Zeit in Sekunden zurück, die benötigt wurde, um das letzte Bild zu zeichnen
        __declspec( dllexport ) virtual double getRenderZeit() const;
        // Erhöht den Reference Counter um 1
        //  Return: Ein zeiger auf diesen Shader
        __declspec( dllexport ) virtual Bildschirm *getThis();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        __declspec( dllexport ) virtual Bildschirm *release();
    };
#ifdef WIN32
    // Diese Klasse verwaltet das Bild auf dem Bildschirm ohne 3D Elemente
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
        //  fenster: Das Fenster, dessen Inhalt verwaltet werden soll
        __declspec( dllexport ) Bildschirm2D( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) ~Bildschirm2D();
        // Aktualisiert die Objekte, mit der die Grafikkarte verwaltet wird
        __declspec( dllexport ) virtual void update();
        // Zeichnet ein Bild und präsentiert es auf dem Bildschirm
        __declspec( dllexport ) virtual void render();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        __declspec( dllexport ) virtual Bildschirm *release();
    };

    // Diese Klasse verwaltet das Bild auf dem Bildschirm mit 3D Elementen
    class Bildschirm3D : public Bildschirm
    {
    private:
        ID3D11Device *d3d11Device;
        ID3D11DeviceContext *d3d11Context;
        IDXGISwapChain *d3d11SpawChain;
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
        TexturModel *texturModel;
        bool rend3D;

        void cleanUpDirectX();

    public:
        // Konstruktor 
        //  fenster: Das Fenster, dessen Inhalt verwaltet werden soll
        __declspec( dllexport ) Bildschirm3D( WFenster *fenster );
        // Destruktor 
        __declspec( dllexport ) ~Bildschirm3D();
        // Fügt dem Bildschirm eine Kamera hinzu
        //  obj: Die Kamera
        __declspec( dllexport ) void addKamera( Kam3D *obj );
        // Entfernt eine kamera von dem Bildschirm
        __declspec( dllexport ) void removeKamera( Kam3D *zObj );
        // Verarbeitet die Zeit, die seit dem letzten Aufruf der Funktion vergangen ist
        // Ruft die tick Funktionen aller sich im Bild befindenden Zeichnungen und der Kameras auf
        //  tickval: Die vergangene Zeit in Sekunden
        __declspec( dllexport ) void tick( double tickval );
        // Verarbeitet ein Maus Ereignis. Wird vom Framework automatisch aufgerufen.
        // Gibt das Ereignis automatisch an alle sich im Bild befindenden Zeichnungen und Kameras weiter
        //  me: Das Ereignis
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me );
        // Verarbeitet ein Tastatur Ereignis. Wird vom Framework automatisch aufgerufen
        // Gibt das Ereignis automatisch an alle sich im Bild befindenden Zeichnungen und Kameras weiter
        //  te: Das Ereignis
        __declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te );
        // Aktualisiert die Objekte, mit der die Grafikkarte verwaltet wird
        __declspec( dllexport ) void update();
        // Zeichnet ein Bild und präsentiert es auf dem Bildschirm
        __declspec( dllexport ) void render();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        __declspec( dllexport ) Bildschirm *release();
    };

    // Findet die Psition und Auflösung eines Monitors heraus
    //  id: Die ID des Monitors. Falls der Monitor nicht gefunden wurde ist der existiert flag der zurückgegebenen Monitor Struktur 0
    __declspec( dllexport ) Monitor getMonitor( int id );
#endif
}

#endif