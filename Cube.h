#pragma once

#include "Model3D.h"

namespace Framework
{
    class Bild;

    enum CubeSeite
    {
        VORNE, 
        HINTEN, 
        RECHTS, 
        LINKS, 
        OBEN, 
        UNTEN
    };

    // Ein Model eines Würfels
    class Cube : public Model3D
    {
    private:
        bool texturUpdate[ 6 ];

    public:
        // Konstruktor
        //  size: Die Größe des Würfels
        __declspec( dllexport ) Cube( float size );
        // Destruktor
        __declspec( dllexport ) ~Cube();
        // Setzt die Textur des Würfels, so dass sie an allen Seiten gleich ist
        //  textur: Die Textur als Bild
        __declspec( dllexport ) void setTextur( Bild *textur );
        // Setzt die Textur des Würfels, so dass sie an allen Seiten gleich ist
        //  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
        __declspec( dllexport ) void setTextur( int id );
        // Setzt die Textur von einer bestimmten Seite des Würfels
        //  textur: Die Textur als Bild
        //  s: Die Seite, die gesetzt werden soll
        __declspec( dllexport ) void setTextur( Bild *textur, CubeSeite s );
        // Setzt die Textur von einer bestimmten Seite des Würfels
        //  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
        //  s: Die Seite, die gesetzt werden soll
        __declspec( dllexport ) void setTextur( int id, CubeSeite s );
        // Zeichnet den Würfel
        //  zRObj: Ein Zeiger auf das Objekt, das zum Zeichnen verwendet werden soll (ohne erhöhten Reference Counter)
        __declspec( dllexport ) void render( Render3D *zRObj ) override;
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Model3D *release() override;
    };
}