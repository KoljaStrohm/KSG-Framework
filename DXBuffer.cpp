#include "DXBuffer.h"
#include "Render3D.h"
#include <iostream>
#include <d3d11.h>

using namespace Framework;

// Inhalt der DXBuffer Klasse

// Konstruktor
//  bind: Der verwendungszweck des Buffers. Beispiel: D3D11_BIND_INDEX_BUFFER, D3D11_BIND_VERTEX_BUFFER.
//  eLän: Länge eines einzelnen Elements in Bytes
DXBuffer::DXBuffer( D3D11_BIND_FLAG bind, int eLän )
{
    buffer = 0;
    bf = bind;
    data = 0;
    geändert = 0;
    län = 0;
    altLän = 0;
    elLän = eLän;
    ref = 1;
}

// Destruktor
DXBuffer::~DXBuffer()
{
    if( buffer )
        buffer->Release();
}

// Setzt den geändert fläg, so das beim nächsten auruf von 'kopieren' die daten neu kopiert werden
void DXBuffer::setGeändert()
{
    geändert = 1;
}

// Ändert die länge des Buffers beim nächsten aufruf von 'kopieren'
//  län: Die Länge in Bytes
void DXBuffer::setLänge( int län )
{
    this->län = län;
    geändert = 1;
}

// Legt fest, was beim nächsten aufruf von 'kopieren' kopiert wird
//  data: Ein zeiger auf die Daten
void DXBuffer::setData( void *data )
{
    this->data = data;
    geändert = 1;
}

// Kopiert die Daten in den Buffer, fals sie sich verändert haben
//  zRObj: Das Objekt, mit dem die Grafikkarte angesprochen wird
void DXBuffer::copieren( Render3D *zRObj )
{
    if( !geändert || !län || !data )
        return;
    if( län != altLän )
    {
        if( buffer )
            buffer->Release();
        buffer = 0;
    }
    if( !buffer )
    {
        D3D11_BUFFER_DESC desk;
        memset( &desk, 0, sizeof( desk ) );
        desk.Usage = D3D11_USAGE_DYNAMIC;
        desk.ByteWidth = län;
        desk.BindFlags = bf;
        desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA ini;
        memset( &ini, 0, sizeof( ini ) );
        ini.pSysMem = data;

        zRObj->zDevice()->CreateBuffer( &desk, &ini, &buffer );
        altLän = län;
    }
    else if( geändert )
    {
        D3D11_MAPPED_SUBRESOURCE map;
        zRObj->zContext()->Map( buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &map );
        memcpy( map.pData, data, län );
        zRObj->zContext()->Unmap( buffer, 0 );
        geändert = 0;
    }
}

// Gibt die Länge eines Elementes in bytes zurück
int DXBuffer::getElementLänge() const
{
    return elLän;
}

// Gibt den Buffer zurück
ID3D11Buffer *DXBuffer::zBuffer() const
{
    return buffer;
}

// Gibt die Anzahl der Elemente im Buffer zurück
int DXBuffer::getElementAnzahl() const
{
    return altLän / elLän;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
DXBuffer *DXBuffer::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
DXBuffer *DXBuffer::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der DXVertexBuffer Klasse

// Konstruktor
// eSize: Die Länge eines Elementes in Bytes
DXVertexBuffer::DXVertexBuffer( int eSize )
    : DXBuffer( D3D11_BIND_VERTEX_BUFFER, eSize )
{}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
DXBuffer *DXVertexBuffer::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der DXIndexBuffer Klasse

// Konstruktor
// eSize: Die Länge eines Elementes in Bytes
DXIndexBuffer::DXIndexBuffer( int eSize )
    : DXBuffer( D3D11_BIND_INDEX_BUFFER, eSize )
{}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
DXBuffer *DXIndexBuffer::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}