#pragma once

#include "Betriebssystem.h"

struct ID3D11Buffer;
enum D3D11_BIND_FLAG;

namespace Framework
{
    class Render3D;

    // Eine Schnittstelle zwischen dem Arbeitsspeicher und dem Grafikspeicher
    class DXBuffer
    {
    private:
        ID3D11Buffer *buffer;
        D3D11_BIND_FLAG bf;
        void *data;
        bool geändert;
        int län;
        int altLän;
        int elLän;

    protected:
        int ref;

    public:
        // Konstruktor
        //  bind: Der verwendungszweck des Buffers. Beispiel: D3D11_BIND_INDEX_BUFFER, D3D11_BIND_VERTEX_BUFFER.
        //  eLän: Länge eines einzelnen Elements in Bytes
        __declspec( dllexport ) DXBuffer( D3D11_BIND_FLAG bind, int eLän );
        // Destruktor
        __declspec( dllexport ) ~DXBuffer();
        // Setzt den geändert fläg, so das beim nächsten auruf von 'kopieren' die daten neu kopiert werden
        __declspec( dllexport ) void setGeändert();
        // Ändert die länge des Buffers beim nächsten aufruf von 'kopieren'
        //  län: Die Länge in Bytes
        __declspec( dllexport ) void setLänge( int län );
        // Legt fest, was beim nächsten aufruf von 'kopieren' kopiert wird
        //  data: Ein zeiger auf die Daten
        __declspec( dllexport ) void setData( void *data );
        // Kopiert die Daten in den Buffer, fals sie sich verändert haben
        //  zRObj: Das Objekt, mit dem die Grafikkarte angesprochen wird
        __declspec( dllexport ) void copieren( Render3D *zRObj );
        // Gibt die Länge eines Elementes in bytes zurück
        __declspec( dllexport ) int getElementLänge() const;
        // Gibt den Buffer zurück
        __declspec( dllexport ) ID3D11Buffer *zBuffer() const;
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
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual DXBuffer *release();
    };
}