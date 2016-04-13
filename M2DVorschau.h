#pragma once

#include "Zeichnung.h"

namespace Framework
{
    class Model2D; // Model2D.h
    class Model2DData; // Model2D.h
    class LRahmen; // Rahmen.h
    class AlphaFeld; // AlphaFeld.h
    struct MausEreignis; // MausEreignis.h

    class M2DVorschau : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 UsrScale = 0x001000;
            const static __int64 UsrMove = 0x002000;
            const static __int64 UsrRot = 0x004000;
        };

    private:
        Model2D *mdl;
        LRahmen *ram;
        AlphaFeld *af;
        int bgF;
        int mx;
        int my;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) M2DVorschau();
        // Destruktor
        __declspec( dllexport ) ~M2DVorschau();
        // nicht constant
        __declspec( dllexport ) void setModel2DZ( Model2D *mdl );
        __declspec( dllexport ) void setModel2D( Model2DData *mdl );
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        __declspec( dllexport ) bool tick( double tv ) override;
        __declspec( dllexport ) void render( Bild &rb ) override;
        // constant
        __declspec( dllexport ) Model2D *zModel() const;
        __declspec( dllexport ) Model2D *getModel() const;
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Reference counting
        __declspec( dllexport ) M2DVorschau *getThis();
        __declspec( dllexport ) M2DVorschau *release();
    };
}