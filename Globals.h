#ifndef Globals_H
#define Globals_H

#include "Punkt.h"

#ifndef Global
#define Global extern
#endif

namespace Framework
{
	class Maus; // Maus.h
	class WFensterArray; // Fenster.h
    class Thread; // Thread.h
    class ThreadRegister; // Thread.h
    class Model3DList; // Model3DList.h
    class TexturList; // TexturList.h

	Global WFensterArray WFensterA;
	Global bool MausTrack;
	Global bool MausStand[ 3 ];
	Global bool TastenStand[ 255 ];
	Global Maus MausZeiger;
	Global bool istInitialisiert;
	Global bool msgExit;
	Global Punkt mausPos;
    Global ThreadRegister *thRegister;
    Global Model3DList *m3dRegister;
    Global TexturList *texturRegister;

    // Gibt die Koordinaten der Maus auf dem Bildschirm zurück
	__declspec( dllexport ) const Punkt &getMausPos();
    // Gibt zurück, ob eine Taste der Maus momentan gedrückt wird
    //  taste: Die Taste, die geprüft werden soll
    // Beispiel: getMausStand( M_Links ); (Benötigt include <MausEreignis.h>
	__declspec( dllexport ) bool getMausStand( int taste );
    // Gibt zurück, ob eine Taste auf der tastatur momentan gedrückt wird
    //  taste: Die Taste, die überprüft werden soll
    // Beispiel: getTastenStand( T_Enter ); (Benötogt include <TastaturEreignis.h>
	__declspec( dllexport ) bool getTastenStand( unsigned char taste );
    // Legt fest, ob eine Taste auf der tastatur momentan gedrückt wird
    // Der Tastenstand wird vom Framework selbst verwaltet und muss nicht mit dieser Funktion gesetzt werden
    //  taste: Die Taste, deren Status gesetzt werden soll
    //  st: Ob die Taste momentan gedrückt wird. (true), wenn ja. (false) sonnst.
	__declspec( dllexport ) void setTastenStand( unsigned char taste, bool st );
    // Initialisiert das Framework
    // Wird in der (WinMain) des Frameworks automatisch aufgerufen
	__declspec( dllexport ) void initFramework();
    // Gibt den duch (initFramework) benutzten Arbeitsspeicher wieder frei
    // Wird in der (WinMain) des Frameworks automatisch aufgerufen
	__declspec( dllexport ) void releaseFramework();
    // Überprüft, ob ein bestimmter Zeiger auf ein Gültiges Thread Objekt zeigt
    //  t: Der zeiger, der überprüft werden soll
    //  return: 1, falls der Zeiger in Ordnung ist. 0, falls der Zeiger auf kein existentes Thread Objekt zeigt
    __declspec( dllexport ) bool istThreadOk( Thread *t );
    // Gibt das Model3DData Register des Frameworks ohne erhöhten reference Counter zurück
    __declspec( dllexport ) Model3DList *zM3DRegister();
    // Gibt das Textur Register des Frameworks ohne erhöhten reference Counter zurück
    __declspec( dllexport ) TexturList *zTexturRegister();
}

#endif