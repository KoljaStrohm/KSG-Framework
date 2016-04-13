#pragma once

#include "Zeichnung.h"
#include "Array.h"

namespace Framework
{
    class Welt2D; // Welt2D.h
    struct MausEreignis;
    struct TastaturEreignis;

    enum KamAnimationTyp
    {
        WeltGröße,
        ScrollIn,
        ScrollOut,
        MoveKam
    };

    struct KamAnimation
    {
        KamAnimationTyp typ;
        double w1;
        double w2;
        double sek;
    };

    class Kam2D : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 TASTATUR_MOVE = 0x001000;
            const static __int64 MAUS_MOVE = 0x002000;
            const static __int64 WELT_EREIGNISSE = 0x004000;
            const static __int64 WELT_TICK = 0x008000;
        };

    private:
        Welt2D *welt;
        Vec2< double > wGr;
        Punkt maxWGr;
        Vec2< double > wPos;
        Punkt mausPos;
        Array< KamAnimation* > *animations;
        int ref;
        
    public:
        // Konstruktor
        Kam2D();
        // Destruktor
        ~Kam2D();
        // nicht constant
        void setWelt( Welt2D *welt );
        void setMaxWeltGröße( int br, int hö );
        void setWeltGröße( int x, int y, int sek );
        void scrollIn( int scroll, int sek );
        void scrollOut( int scroll, int sek );
        void moveKam( int x, int y, int sek );
        void finishAnimation();
        void clearAnimation();
        bool tick( double t ) override;
        void doMausEreignis( MausEreignis &me ) override;
        void doTastaturEreignis( TastaturEreignis &te ) override;
        void render( Bild &rObj ) override;
        // constant
        Welt2D *zWelt() const;
        Welt2D *getWelt() const;
        double getWeltX() const;
        double getWeltY() const;
        double getWeltBr() const;
        double getWeltHö() const;
        // Reference Counting
        Kam2D *getThis();
        Kam2D *release();
    };
}