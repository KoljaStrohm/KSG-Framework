#ifndef MausEreignis_H
#define MausEreignis_H

#include "Vec3.h"

namespace Framework
{
    const int M_Links = 0;
    const int M_Mitte = 1;
    const int M_Rechts = 2;
    const int ME_PLinks = 0;
    const int ME_PRechts = 1;
    const int ME_PMitte = 2;
    const int ME_RLinks = 3;
    const int ME_RRechts = 4;
    const int ME_RMitte = 5;
    const int ME_DKLinks = 6;
    const int ME_DKRechts = 7;
    const int ME_DKMitte = 8;
    const int ME_Betritt = 9;
    const int ME_Leaves = 10;
    const int ME_Bewegung = 11;
    const int ME_UScroll = 12;
    const int ME_DScroll = 13;
    const int ME_RScroll = 14;
    const int ME_LScroll = 15;

    // Speichert eine bestimmte Mauseingabe des Nutzers
    struct MausEreignis
    {
        // Art der Eingabe
        int id;
        // X koordinate auf dem Bildschirm relativ zur Zeichnungposition
        int mx;
        // Y Koordinate auf dem Bildschirm relativ zur Zeichnungposition
        int my;
        // X Koordinate auf dem Bildschirm zur linken oberen Ecke
        int rmx;
        // Y Koordinate auf dem Bildschirm zur linken oberen Ecke
        int rmy;
        // Speichert, ob die Eingabe bereits verarbeitet wurde
        bool verarbeitet;
    };

    // Speichert eine bestimmte Mauseingabe des Nutzers
    struct MausEreignis3D
    {
        // Art der Eingabe
        int id;
        // Die Position der Maus in der Welt
        Vec3< float > pos;
        // Die Richtung, in die die Kamera zeigt
        Vec3< float > dir;
        // Speichert, ob die Eingabe bereits verarbeitet wurde
        bool verarbeitet;
    };

    // Standart Maus Ereinis Rückruffunktion
    //  param: Ein beliebiger Parameter
    //  obj: Die Zeichnung, welches diese Funktion aufruft
    //  te: Das Mausereignis, welches verarbeitet werden soll
    //  return: (true), wenn aufrufende Zeichnung das Ereignis weiterverarbeiten soll. (false) sonnst.
    // Gibt immer (true) zurück
    __declspec( dllexport ) bool _ret1ME( void *param, void *obj, MausEreignis me );
}

#endif