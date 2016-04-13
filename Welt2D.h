#pragma once

#include "Array.h"
#include "Punkt.h"

namespace Framework
{
    class Model2D; // Model2D.h
    struct MausEreignis;
    struct TastaturEreignis;
    class Bild;

    class Welt2D
    {
    public:
        class Style
        {
        public:
            const static __int64 HORIZONTAL_UNENDLICH = 0x0001; // Zeichnunge die rechts austreten erscheinen links wieder und umgekehrt
            const static __int64 VERTIKAL_UNENDLICH = 0x0002; // Zeichnunge die unten austreten erscheinen oben wieder und umgekehrt
            const static __int64 BEI_VERLASSEN_LÖSCHEN = 0x0004; // Entfernt Zeichnunge, die die Welt an den Rändern verlassen
            const static __int64 HINTERGRUND_BENUTZEN = 0x0008; // Legt fest ob es eine seperate Liste mit HintergrundZeichnungen gibt
            const static __int64 HINTERGRUND_EREIGNISSE = 0x0010; // Legt festm ob die HintergrundZeichnunge Benutzereingaben empfangen
            const static __int64 HINTERGRUND_TICK = 0x0020; // Legt fest, ob der Hintergrund annimerbar ist
        };
    private:
        RCArray< Model2D > *obj;
        int style;
        int ref;

    public:
        // Konstruktor
        Welt2D();
        // Destruktor
        ~Welt2D();
        // nicht constant
        void addModel( Model2D *obj );
        void removeModel( Model2D *zObj );
        void setModelInVordergrund( Model2D *zObj );
        void setModelInHintergrund( Model2D *zObj );
        void removeAll();
        void doMausEreignis( MausEreignis &me );
        void doTastaturEreignis( TastaturEreignis &me );
        bool tick( double t );
        void render( Bild &zRObj, Punkt &wPos, Punkt &wGr, Punkt &kamGr );
        // constant
        int getModelAnzahl() const;
        Model2D *getModel( int z ) const;
        Model2D *zModel( int z ) const;
        // Reference Counting
        Welt2D *getThis();
        Welt2D *release();
    };
}