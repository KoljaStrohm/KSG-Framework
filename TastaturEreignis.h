#ifndef TastaturEreignis_H
#define TastaturEreignis_H

#include "Betriebssystem.h"

namespace Framework
{
    // Es wurde eine Taste runtergedrückt
	const int TE_Press = 0;
    // Es wurde eine Taste losgelassen
	const int TE_Release = 1;
	const unsigned char T_Esc = 27;
	const unsigned char T_Tab = 0;
	const unsigned char T_Enter = 13;
	const unsigned char T_BackSpace = 8;
	const unsigned char T_Space = 32;
	const unsigned char T_Caps_Lock = 129;
	const unsigned char T_Shift = 130;
	const unsigned char T_Strg = 141;
	const unsigned char T_F1 = 143;
	const unsigned char T_F2 = 144;
	const unsigned char T_F3 = 157;
	const unsigned char T_F4 = 160;
	const unsigned char T_F5 = 130;
	const unsigned char T_F6 = 132;
	const unsigned char T_F7 = 133;
	const unsigned char T_F8 = 134;
	const unsigned char T_F9 = 135;
	const unsigned char T_F10 = 136;
	const unsigned char T_F11 = 137;
	const unsigned char T_F12 = 138;
	const unsigned char T_Pause = 139;
	const unsigned char T_Druck = 140;
	const unsigned char T_Einfg = 142;
	const unsigned char T_Entf = 145;
	const unsigned char T_Links = 146;
	const unsigned char T_Oben = 147;
	const unsigned char T_Rechts = 148;
	const unsigned char T_Unten = 149;
	const unsigned char T_Alt_Gr = 150;
	const unsigned char T_Num = 151;
	const unsigned char T_Pos1 = 152;
	const unsigned char T_BildO = 153;
	const unsigned char T_BildU = 154;
	const unsigned char T_Ende = 155;

    // Speichert den Input eines Nutzers auf der Tastatur
	struct TastaturEreignis
	{
        // Art der Eingabe
		int id;
        // Betroffene Taste
		unsigned char taste;
        // (true), wenn die Eingabe bereits verarbeitet wurde. (false) sonnst
		bool verarbeitet;
	};

    // Standart Tastatur Ereinis Rückruffunktion
    //  param: Ein beliebiger Parameter
    //  obj: Die Zeichnung, welches diese Funktion aufruft
    //  te: Das Tastaturereignis, welches verarbeitet werden soll
    //  return: (true), wenn aufrufende Zeichnung das Ereignis weiterverarbeiten soll. (false) sonnst.
    // Gibt immer (true) zurück
	__declspec( dllexport ) bool _ret1TE( void *param, void *obj, TastaturEreignis te );
    // Standart Tastatur Ereinis Rückruffunktion
    //  param: Ein beliebiger Parameter
    //  obj: Die Zeichnung, welches diese Funktion aufruft
    //  te: Das Tastaturereignis, welches verarbeitet werden soll
    //  return: (true), wenn aufrufende Zeichnung das Ereignis weiterverarbeiten soll. (false) sonnst.
    // Gibt nur (true) zurück, wenn die Taste zwischen '0' und '9' ist, oder zum Löschen oder Cursorbewegen dient
	__declspec( dllexport ) bool _nurNummernTE( void *param, void *obj, TastaturEreignis te );
    // Standart Tastatur Ereinis Rückruffunktion
    //  param: Ein beliebiger Parameter
    //  obj: Die Zeichnung, welches diese Funktion aufruft
    //  te: Das Tastaturereignis, welches verarbeitet werden soll
    //  return: (true), wenn aufrufende Zeichnung das Ereignis weiterverarbeiten soll. (false) sonnst.
    // Gibt nur (true) zurück, wenn die Taste zwischen '0' und '9' oder 'A' und 'F' ist, oder zum Löschen oder Cursorbewegen dient
	__declspec( dllexport ) bool _nurHexTE( void *param, void *obj, TastaturEreignis te );
}

#endif