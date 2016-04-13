#pragma once

struct ID3D10Blob;
struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace Framework
{
    class Text;

    enum ShaderType
    {
        UNBEKANNT,
        VERTEX,
        PIXEL
    };

    class Shader
    {
    protected:
        Text *shader;
        ID3D10Blob *shaderBuffer;
        ShaderType type;
        ID3D11Buffer *constBuffers[ 14 ];
        int buffLän[ 14 ];
        int buffAnz;
        int ref;

    public:
        // Konstruktor
        Shader();
        // Destruktor
        ~Shader();
        // Lähdt den Shader Quellcode aus einer Textdatei
        //  pfad: Der Pfad zur Datei
        //  return: true, wenn der Shader erfolgreich geladen wurde
        bool ladeAusDatei( const char *pfad );
        // Setzt den Shader Quellcode
        //  zCode: Der Quellcode des Shaders
        void setShaderCode( Text *zCode );
        // Compiliert den Shader Quellcode
        //  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
        //  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
        //  type: Der Typ und die Version des Shaders. Beispiel: 'vs_5_0' für Vertexshader, 'ps_5_0' für Pixelshader.
        //  return: true, wenn der Quellcode keine Fehler enthällt
        virtual bool compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *type );
        // Nach dem Aufruf dieser Funktion wird dieser Shader als Pixel Shader benutzt
        //  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
        virtual void benutzeShader( ID3D11DeviceContext *zD3d11Context );
        // erstellt ein constanten Buffer, der constante daten an den Shader übergibt
        // es können maximal 14 Buffer erstellt werden
        //  zD3d11Device: Das Device, mit dem der Buffer erstellt werden soll
        //  größe: Die größe des buffers in byte
        //  index: Die position des Buffers im Buffer Array. Bereits vorhanderner Buffer wird ersetzt. Buffer 1 kann nicht erstellt werden, wenn Buffer 0 noch nicht erstellt wurde usw.
        bool erstelleConstBuffer( ID3D11Device *zD3d11Device, int größe, int index );
        // Löscht einen constanten Buffer
        //  index: der Index des Buffers, der gelöscht werden soll. Buffer 0 kann nicht gelöscht werden, solange Buffer 1 noch existiert usw.
        bool löscheConstBuffer( int index );
        // Kopiert daten in einen constanten buffer
        //  zD3d11Context: Das Context Objekt, das zum kopieren verwendt werden soll
        //  data: Einen zeiger auf en byte Array der größe des Buffers
        //  index: Der Index des Buffers
        bool füllConstBuffer( ID3D11DeviceContext *zD3d11Context, char *data, int index );
        // Gibt die Länge eines constanten Buffers zurück
        //  index: Der Index des Buffers
        int getConstBufferLänge( int index ) const;
        // Gibt den Shadertyp zurück
        ShaderType getType() const;
        // Erhöht den Reference Counter um 1
        //  Return: Ein zeiger auf diesen Shader
        Shader *getThis();
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        virtual Shader *release();
    };

    class PixelShader : public Shader
    {
    private:
        ID3D11PixelShader *pixelShader;

    public:
        // Konstruktor
        PixelShader();
        // Destruktor
        ~PixelShader();
        // Compiliert den Shader Quellcode
        //  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
        //  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
        //  type: Der die Version des Shaders. Beispiel: '5_0', '4_0'
        //  return: true, wenn kein Fehler aufgetreten ist
        bool compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *version ) override;
        // Nach dem Aufruf dieser Funktion wird dieser Shader als Pixel Shader benutzt
        //  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
        void benutzeShader( ID3D11DeviceContext *zD3d11Context );
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        Shader *release();
    };

    class VertexShader : public Shader
    {
    private:
        ID3D11VertexShader *vertexShader;
        ID3D11InputLayout *inputLayout;

    public:
        // Konstruktor
        VertexShader();
        // Destruktor
        ~VertexShader();
        // Compiliert den Shader Quellcode
        //  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
        //  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
        //  type: Der die Version des Shaders. Beispiel: '5_0', '4_0'
        //  return: true, wenn kein Fehler aufgetreten ist
        bool compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *version ) override;
        // erstellt ein InputLayout für den Shader
        // Darf erst nach compile aufgerufen werden
        //  zD3d11Device: Das Device, mit dem das Layout erstellt werden soll
        //  descArray: Ein Array mit initialisierungsdaten
        //  anz: Die Anzahl der Elemente im Array
        bool erstelleInputLayout( ID3D11Device *zD3d11Device, D3D11_INPUT_ELEMENT_DESC *descArray, int anz );
        // Nach dem Aufruf dieser Funktion wird dieser Shader als Vertex Shader benutzt
        //  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
        void benutzeShader( ID3D11DeviceContext *zD3d11Context );
        // Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
        //  Return: 0
        Shader *release();
    };
}