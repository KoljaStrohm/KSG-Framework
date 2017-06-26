#pragma once

#include "Vec3.h"

namespace Framework
{
    class Skelett;
    class Knochen;

    class Animation3D
    {
    private:
        struct KnochenData
        {
            KnochenData *next;
            double time;
            Vec3<float> pos;
            Vec3<float> rot;
        };

        KnochenData *kd[ MAX_KNOCHEN_ANZ ];
        double maxTime;
        int ref;

        // löscht eine KnochenData Struktur
        void deleteKnochenData( KnochenData *d );

    public:
        // Konstruktor
        __declspec( dllexport ) Animation3D();
        // Destructor
        __declspec( dllexport ) ~Animation3D();
        // Fügt für einen bestimmten Knochen ein Keyframe hinzu
        //  kId: Die Id des Knochens
        //  time: Die Zeit, die vergehen soll bis der Knochen die Position und die Rotation des Keyframes follständig erreicht hat
        //  pos: Die Position des Knochens die erreicht werden soll in Modellkoordinaten
        //  rot: Die Rotation des Knochens um seine Position die erreicht werden soll
        __declspec( dllexport ) void addKeyFrame( int kId, double time, Vec3<float> pos, Vec3<float> rot );
        // Wendet die Animation auf ein bestimmtes Skelett an
        // zS: Das Skelett
        // timeOffset: zeit in sekunden, die diese Animation bereits auf dem Skelett angewendet wurde. Wird automatisch von der Animation aktualisiert.
        // sec: zeit in Sekunden, die vergangen ist seit dem diese Methode zuletzt für das Skelett aufgerufen wurde
        __declspec( dllexport ) void apply( Skelett *zS, double &timeOffset, double sec ) const;
        // Wendet die Animation auf ein bestimmten Knochen an
        // zS: Der Knochen
        // timeOffset: zeit in sekunden, die diese Animation bereits auf dem Knochen angewendet wurde.
        // sec: zeit in Sekunden, die vergangen ist seit dem diese Methode zuletzt für dem Knochen aufgerufen wurde
        __declspec( dllexport ) void apply( Knochen *zK, double timeOffset, double sec ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Animation3D *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Animation3D *release();
    };
}