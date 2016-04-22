#include "Shader.h"
#include "Text.h"
#include "Datei.h"
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <iostream>

using namespace Framework;

// Inhalt der Shader Klasse

// Konstruktor
Shader::Shader()
{
    shader = new Text();
    shaderBuffer = 0;
    type = UNBEKANNT;
    for( int i = 0; i < 14; i++ )
    {
        constBuffers[ i ] = 0;
        buffLän[ i ] = 0;
    }
    buffAnz = 0;
    ref = 1;
}

// Destruktor
Shader::~Shader()
{
    shader->release();
    if( shaderBuffer )
        shaderBuffer->Release();
    for( int i = 0; i < 14; i++ )
    {
        if( constBuffers[ i ] )
            constBuffers[ i ]->Release();
    }
}

// Lähdt den Shader Quellcode aus einer Textdatei
//  pfad: Der Pfad zur Datei
//  return: true, wenn der Shader erfolgreich geladen wurde
bool Shader::ladeAusDatei( const char *pfad )
{
    Datei d;
    d.setDatei( pfad );
    __int64 gr = d.getGröße();
    if( gr > 10 * 1024 )
        return 0; // Datei zu groß für Shader Quellcode
    shader->füllText( ' ', (int)gr );
    if( !d.öffnen( Datei::Style::lesen ) )
        return 0;
    d.lese( shader->getText(), (int)gr );
    d.schließen();
    return 1;
}

// Setzt den Shader Quellcode
//  zCode: Der Quellcode des Shaders
void Shader::setShaderCode( Text *zCode )
{
    shader->setText( zCode->getText() );
}

// Compiliert den Shader Quellcode
//  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
//  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
//  type: Der Typ und die Version des Shaders. Beispiel: 'vs_5_0' für Vertexshader, 'ps_5_0' für Pixelshader.
//  return: true, wenn der Quellcode keine Fehler enthällt
bool Shader::compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *type )
{
    ID3D10Blob* errorMessage = 0;
    unsigned int flag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flag |= D3D10_SHADER_DEBUG;
#endif
    if( shaderBuffer )
        shaderBuffer->Release();
    HRESULT result = D3DCompile( shader->getText(), shader->getLänge(), 0, 0, 0, einstiegsFunktion, type, flag, 0, &shaderBuffer, &errorMessage );
    if( errorMessage )
    {
        char *err = (char*)errorMessage->GetBufferPointer();
        std::cout << err;
        errorMessage->Release();
    }
    return result == S_OK;
}

// Nach dem Aufruf dieser Funktion wird dieser Shader als Pixel Shader benutzt
//  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
void Shader::benutzeShader( ID3D11DeviceContext *zD3d11Context )
{}

// erstellt ein constanten Buffer, der constante daten an den Shader übergibt
// es können maximal 14 Buffer erstellt werden
//  zD3d11Device: Das Device, mit dem der Buffer erstellt werden soll
//  größe: Die größe des buffers in byte
//  index: Die position des Buffers im Buffer Array. Bereits vorhanderner Buffer wird ersetzt. Buffer 1 kann nicht erstellt werden, wenn Buffer 0 noch nicht erstellt wurde usw.
bool Shader::erstelleConstBuffer( ID3D11Device *zD3d11Device, int größe, int index )
{
    if( index < 0 || index >= 14 )
        return 0;
    bool ok = 1;
    for( int i = 0; i < index; i++ )
        ok &= constBuffers[ index ] != 0;
    if( !ok )
        return 0;
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = größe;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    if( constBuffers[ index ] )
    {
        constBuffers[ index ]->Release();
        constBuffers[ index ] = 0;
        buffLän[ index ] = 0;
    }
    HRESULT res = zD3d11Device->CreateBuffer( &bufferDesc, 0, &constBuffers[ index ] );
    if( res == S_OK )
        buffLän[ index ] = größe;
    for( buffAnz = 0; buffAnz < 14 && constBuffers[ buffAnz ]; buffAnz++ );
    return res == S_OK;
}

// Löscht einen constanten Buffer
//  index: der Index des Buffers, der gelöscht werden soll. Buffer 0 kann nicht gelöscht werden, solange Buffer 1 noch existiert usw.
bool Shader::löscheConstBuffer( int index )
{
    if( index < 0 || index >= 14 )
        return 0;
    bool ok = 1;
    for( int i = 13; i > index; i-- )
        ok &= constBuffers[ i ] == 0;
    if( !ok )
        return 0;
    if( constBuffers[ index ] )
    {
        constBuffers[ index ]->Release();
        constBuffers[ index ] = 0;
        buffLän[ index ] = 0;
    }
    for( buffAnz = 0; buffAnz < 14 && constBuffers[ buffAnz ]; buffAnz++ );
    return 1;
}

// Kopiert daten in einen constanten buffer
//  zD3d11Context: Das Context Objekt, das zum kopieren verwendt werden soll
//  data: Einen zeiger auf en byte Array der größe des Buffers
//  index: Der Index des Buffers
//  län: Die Länge der Daten in Bytes (-1 für die maximale größe des Buffers)
bool Shader::füllConstBuffer( ID3D11DeviceContext *zD3d11Context, char *data, int index, int län )
{
    if( index < 0 || index >= 14 )
        return 0;
    if( !constBuffers[ index ] )
        return 0;
    if( län < 0 )
        län = buffLän[ index ];
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT res = zD3d11Context->Map( constBuffers[ index ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    if( res != S_OK )
        return 0;
    memcpy( mappedResource.pData, data, län );
    zD3d11Context->Unmap( constBuffers[ index ], 0 );
    return 1;
}

// Gibt die Länge eines constanten Buffers zurück
//  index: Der Index des Buffers
int Shader::getConstBufferLänge( int index ) const
{
    if( index < 0 || index >= 14 )
        return 0;
    return buffLän[ index ];
}

// Gibt den Shadertyp zurück
ShaderType Shader::getType() const
{
    return type;
}

// Erhöht den Reference Counter um 1
//  Return: Ein zeiger auf diesen Shader
Shader *Shader::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
//  Return: 0
Shader *Shader::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der PixelShader Klasse

// Konstruktor
PixelShader::PixelShader()
    : Shader()
{
    pixelShader = 0;
}

// Destruktor
PixelShader::~PixelShader()
{
    if( pixelShader )
        pixelShader->Release();
}

// Compiliert den Shader Quellcode
//  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
//  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
//  type: Der die Version des Shaders. Beispiel: '5_0', '4_0'
//  return: true, wenn kein Fehler aufgetreten ist
bool PixelShader::compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *version )
{
    Text v = "ps_";
    v += version;
    if( !__super::compile( zD3d11Device, einstiegsFunktion, v ) )
        return 0;
    if( pixelShader )
        pixelShader->Release();
    pixelShader = 0;
    HRESULT result = zD3d11Device->CreatePixelShader( shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), 0, &pixelShader );
    shaderBuffer->Release();
    shaderBuffer = 0;
    return result == S_OK;
}

// Nach dem Aufruf dieser Funktion wird dieser Shader als Pixel Shader benutzt
//  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
void PixelShader::benutzeShader( ID3D11DeviceContext *zD3d11Context )
{
    if( buffAnz )
        zD3d11Context->PSSetConstantBuffers( 0, buffAnz, constBuffers );
    if( pixelShader )
        zD3d11Context->PSSetShader( pixelShader, 0, 0 );
}

// Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
//  Return: 0
Shader *PixelShader::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der VertexShader Klasse

// Konstruktor
VertexShader::VertexShader()
    : Shader()
{
    vertexShader = 0;
    inputLayout = 0;
}

// Destruktor
VertexShader::~VertexShader()
{
    if( vertexShader )
        vertexShader->Release();
    if( inputLayout )
        inputLayout->Release();
}

// Compiliert den Shader Quellcode
//  zD3d11Device: Das Device, mit welchem der Shader erstellt werden soll
//  einstiegsFunktion: Der Name der Funktion, die als erstes aufgerufen werden soll
//  type: Der die Version des Shaders. Beispiel: '5_0', '4_0'
//  return: true, wenn kein Fehler aufgetreten ist
bool VertexShader::compile( ID3D11Device *zD3d11Device, const char *einstiegsFunktion, const char *version )
{
    Text v = "vs_";
    v += version;
    if( !__super::compile( zD3d11Device, einstiegsFunktion, v ) )
        return 0;
    if( vertexShader )
        vertexShader->Release();
    vertexShader = 0;
    HRESULT result = zD3d11Device->CreateVertexShader( shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), 0, &vertexShader );
    return result == S_OK;
}

// erstellt ein InputLayout für den Shader
// Darf erst nach compile aufgerufen werden
//  zD3d11Device: Das Device, mit dem das Layout erstellt werden soll
//  descArray: Ein Array mit initialisierungsdaten
//  anz: Die Anzahl der Elemente im Array
bool VertexShader::erstelleInputLayout( ID3D11Device *zD3d11Device, D3D11_INPUT_ELEMENT_DESC *descArray, int anz )
{
    if( !shaderBuffer )
        return 0;
    if( inputLayout )
        inputLayout->Release();
    inputLayout = 0;
    HRESULT res = zD3d11Device->CreateInputLayout( descArray, anz, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &inputLayout );
    if( res == S_OK )
    {
        shaderBuffer->Release();
        shaderBuffer = 0;
    }
    return res == S_OK;
}

// Nach dem Aufruf dieser Funktion wird dieser Shader als Vertex Shader benutzt
//  zD3d11Context: Das Context Objekt, mit dem der Shader verwendet werden soll
void VertexShader::benutzeShader( ID3D11DeviceContext *zD3d11Context )
{
    if( buffAnz )
        zD3d11Context->VSSetConstantBuffers( 0, buffAnz, constBuffers );
    if( inputLayout )
        zD3d11Context->IASetInputLayout( inputLayout );
    if( vertexShader )
        zD3d11Context->VSSetShader( vertexShader, 0, 0 );
}

// Verringert den Reference Counter und löscht den Shader, falls der Refeence Counter auf 0 ist
//  Return: 0
Shader *VertexShader::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}