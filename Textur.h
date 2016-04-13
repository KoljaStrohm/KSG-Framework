#pragma once

#include "Punkt.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace Framework
{
    class Bild; // Bild.h
    class Render3D; // Render3D.h
    class TexturList; // TexturList.h

    // Wandelt ein Bild in eine Textur um, die an die Grafikkarte zum rendern übergeben werden kann
    class Textur
    {
    private:
        Bild *bild;
        ID3D11Texture2D *txt;
        ID3D11ShaderResourceView *view;
        Punkt lastGr;
        int id;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) Textur();
        // Destruktor
        __declspec( dllexport ) ~Textur();
        // Setzt einen Zeiger auf das Bild, welches die Textur enthält
        //  b: Der Zeiger auf das Bild
        __declspec( dllexport ) void setBildZ( Bild *b );
        // Setzt das Bild welches die Textur enthält, indem es kopiert wird
        //  b: Das Bild, was kopiert werden soll
        __declspec( dllexport ) void setBild( Bild *b );
        // Aktualisiert die Textur. Die Pixel des aktuellen Bildes werden in den Graphikspeicher kopiert
        //  zRObj: Das Objekt, mit dem die Graphikkarte angesprochen wird
        __declspec( dllexport ) bool updateTextur( Render3D *zRObj );
        // Gibt true zurük, wenn updateTextur aufgerufen werden muss
        __declspec( dllexport ) bool brauchtUpdate() const;
        // Gibt einen Zeiger auf das Bild zurück
        __declspec( dllexport ) Bild *getBild() const;
        // Gibt einen Zeiger auf das Bild ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Bild *zBild() const;
        // Gibt die Id der Textur zurück, wenn sie in einer TexturList registriert wurde. (siehe Framework::zTexturRegister())
        __declspec( dllexport ) int getId() const;
        // Gibt die verwendtete Shader Resource View zurück
        __declspec( dllexport ) operator ID3D11ShaderResourceView*() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Textur *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Textur *release();

        friend TexturList;
    };
}