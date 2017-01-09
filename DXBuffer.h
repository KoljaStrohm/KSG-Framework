#pragma once

#include "Betriebssystem.h"

#ifdef WIN32
struct ID3D11Buffer;
enum D3D11_BIND_FLAG;
#else
typedef int D3D11_BIND_FLAG;
#endif

namespace Framework
{
    class Render3D;

    // Eine Schnittstelle zwischen dem Arbeitsspeicher und dem Grafikspeicher
    class DXBuffer
    {
    private:
#ifdef WIN32
        ID3D11Buffer *buffer;
        D3D11_BIND_FLAG bf;
#endif
        void *data;
        bool changed;
        int len;
        int altLen;
        int elLem;

    protected:
        int ref;

    public:
        // Konstruktor
        //  bind: Der verwendungszweck des Buffers. Beispiel: D3D11_BIND_INDEX_BUFFER, D3D11_BIND_VERTEX_BUFFER.
        //  eLen: Länge eines einzelnen Elements in Bytes
        __declspec( dllexport ) DXBuffer( D3D11_BIND_FLAG bind, int eLen );
        // Destruktor
        __declspec( dllexport ) virtual ~DXBuffer();
        // Setzt den geändert fläg, so das beim nächsten auruf von 'kopieren' die daten neu kopiert werden
        __declspec( dllexport ) void setChanged();
        // Ändert die länge des Buffers beim nächsten aufruf von 'kopieren'
        //  len: Die Länge in Bytes
        __declspec( dllexport ) void setLength( int len );
        // Legt fest, was beim nächsten aufruf von 'kopieren' kopiert wird
        //  data: Ein zeiger auf die Daten
        __declspec( dllexport ) void setData( void *data );
        // Kopiert die Daten in den Buffer, fals sie sich verändert haben
        //  zRObj: Das Objekt, mit dem die Grafikkarte angesprochen wird
        __declspec( dllexport ) void copieren( Render3D *zRObj );
        // Gibt die Länge eines Elementes in bytes zurück
        __declspec( dllexport ) int getElementLength() const;
#ifdef WIN32
        // Gibt den Buffer zurück
        __declspec( dllexport ) ID3D11Buffer *zBuffer() const;
#endif
        // Gibt die Anzahl der Elemente im Buffer zurück
        __declspec( dllexport ) int getElementAnzahl() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) DXBuffer *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual DXBuffer *release();
    };

    // Ein Buffer von Eckpunkten eines 3D Models
    class DXVertexBuffer : public DXBuffer
    {
    public:
        // Konstruktor
        // eSize: Die Länge eines Elementes in Bytes
        __declspec( dllexport ) DXVertexBuffer( int eSize );
		// Destruktor
		__declspec( dllexport ) virtual ~DXVertexBuffer();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual DXBuffer *release();
    };

    // Ein Buffer von Indizes aus dem Buffer mit Eckpunkten, wovon immer drei ein Dreieck ergeben, das gezeichnet wird
    class DXIndexBuffer : public DXBuffer
    {
    public:
        // Konstruktor
        // eSize: Die Länge eines Elementes in Bytes
        __declspec( dllexport ) DXIndexBuffer( int eSize );
		// Destruktor
		__declspec( dllexport ) virtual ~DXIndexBuffer();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual DXBuffer *release();
    };
}