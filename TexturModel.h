#pragma once

#include "Model3D.h"

namespace Framework
{
    class Bild; // Bild.h

    // Ein 3D Modell, das zur Darstellung einer Textur im dreidimensionalen Raum verwendet werden kann
    class TexturModel : public Model3D
    {
    public:
        // Konstruktor
        __declspec( dllexport ) TexturModel();
        // Setzt die Textur die angezeigt werden soll
        //  textur: Die Textur als Bild
        __declspec( dllexport ) void setTextur( Bild *textur );
        // Setzt die Textur die angezeigt werden soll 
        //  id: Die id der Textur. Sie muss im Textur Register des Frameworks registriert sein
        __declspec( dllexport ) void setTextur( int id );
        // Setzt die Größe, in der Die Textur angezeigt wird
        //  gr: Ein Vektor, der für x und y die breite und höhe beinhaltet
        __declspec( dllexport ) void setGröße( Vec2< float > gr );
        // Setzt die Größe, in der die Textur angezeigt wird
        //  b: Die Breite, in der die Textur angezeigt wird
        //  h: Die Höhe, in der die Textur angezeigt wird
        __declspec( dllexport ) void setGröße( float b, float h );
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Model3D *release() override;
    };
}