#ifndef Datei_H
#define Datei_H

#include "Array.h"
#include <fstream>

namespace Framework
{
    class Text; // Text.h
    class Zeit; // Zeit.h
	namespace Encryption
	{
		class Key; // Schlüssel.h
	}
    class Datei; // aus dieser Datei

    // Ließt und schreibt in eine Datei
    class Datei
    {
    public:
        class Style
        {
        public:
            const static int lesen = 0x01; // datei wird zum lesen geöffnet
            const static int schreiben = 0x02; // datei wirt zum schreiben geöffnet
            const static int ende = 0x04; // setzt dateizeiger ans Ende der Datei
        };
    private:
        int ref;
        std::fstream *stream;
        Text *pfad;
        __int64 gr;
        char tmpLByte;
        char tmpLBPos;
        char tmpSByte;
        char tmpSBPos;
		Encryption::Key *key;

    public:
        // Konstruktor 
        __declspec( dllexport ) Datei();
        // Destruktor 
        __declspec( dllexport ) ~Datei();
        // Setzt den Pfad zur Datei
        //  pfad: Der Pfad
        __declspec( dllexport ) void setDatei( const char *pfad );
        // Setzt den Pfad zur Datei
        //  pfad: Der Pfad
        __declspec( dllexport ) void setDatei( Text *pfad );
        // Benennt die Datei um oder verschiebt sie
        //  pfad: Der neue Pfad zur Datei. Wenn sich nur der Name ändert, wird sie nur umbenannt
        //  return: 1, wenn das umbenennen erfolgreich war. 0, sonst
        __declspec( dllexport ) bool umbenennen( const char *pfad );
        // Benennt die Datei um oder verschiebt sie
        //  pfad: Der neue Pfad zur Datei. Wenn sich nur der Name ändert, wird sie nur umbenannt
        //  return: 1, wenn das umbenennen erfolgreich war. 0, sonst
        __declspec( dllexport ) bool umbenennen( Text *pfad );
        // Löscht die Datei
        //  return: 1, wenn das löschen erfolgreich war. 0 sonst
        __declspec( dllexport ) bool remove();
        // Erstellt die Datei neu. Wenn im Pfad Ordner vorkommen, die nicht existieren, so werden sie erstellt
        //  return 1: wenn das erstellen erfolgreich war. 0, sonst
        __declspec( dllexport ) bool erstellen();
        // Öffnet die Datei
        //  style: Legt fest, ob die Datei zum lesen und/oder schreiben geöffnet wird. Alle Elemente aus Datei::Style:: sin möglich
        //  return 1: wenn die datei erfolgreich geöffnet werden konnte. 0 sonnst
        __declspec( dllexport ) bool open( int style );
        // Setzt die Position des Bytes, das als nächstes gelesen werden soll
        //  pos: Der Index des Bytes
        //  ende: 1, wenn der Index vom ende der Datei zählt. 0, wenn der Index vom Beginn der Datei zählt
        __declspec( dllexport ) void setLPosition( __int64 pos, bool ende );
        // Setzt die Position des Bytes, das als nächstes überschrieben wird
        //  pos: Der Index des Bytes
        //  ende: 1, wenn der Index vom ende der Datei zählt. 0, wenn der Index vom Beginn der Datei zählt
        __declspec( dllexport ) void setSPosition( __int64 pos, bool ende );
        // Schreibt in die Datei
        //  bytes: Ein Array von bytes, die geschrieben werden sollen.
        //  len: Wie viele Bytes in die Datei geschrieben werden sollen
        __declspec( dllexport ) void schreibe( char *bytes, int len );
        // Ließt aus der Datei
        //  bytes: Ein Array, der mit Bytes aus der Datei gefüllt werden soll
        //  len: Wie viele Bytes aus der Datei gelesen werden sollen
        __declspec( dllexport ) void lese( char *bytes, int len );
        // Ließt die nächste zeile der Datei ein
        //  return: Die gelesene Zeile als Text mit zeilenumbruch
        __declspec( dllexport ) Text *leseZeile();
        // Schließt die datei
        __declspec( dllexport ) void close();
		// Setzt den Schlüssel für die Datei
		__declspec( dllexport ) void setKey( char *s, int l );
#ifdef WIN32
        // Setzt den Zeitpunkt der letzten Änderung der Datei (nur für Windows)
        //  zeit: den Zeitpunkt der letzten Änderung
        //  return: 1, wenn der Zeitpunkt gesetzt wurde. 0 sonst
        __declspec( dllexport ) bool setLetzteÄnderung( Zeit *zeit );
#endif
        // Ließt das nächste Bit aus der Datei
        //  bit: Eine Referenz auf deinen bool, in dem das Bit gespeichert wird
        //  return 1, falls das lesen erfolgreich war. 0, sonst
        __declspec( dllexport ) bool getNextBit( bool &bit );
        // Speichert ein einzelnes Bit in der Datei
        //  bit: Das bit, welches gespeichert werden soll
        //  return 1, falls das speichern erfolgreich war
        __declspec( dllexport ) bool setNextBit( bool bit );
        // Prüft, ob die Datei ein Verzeichnis ist
        //  return 1, falls die Datei ein Verzeichnis ist. 0, sonst
        __declspec( dllexport ) bool istOrdner() const;
        // Prüft, ob die Datei bereits geöffnet wurde
        //  return: 1, wenn die Datei geöffnet ist. 0 sonnst
        __declspec( dllexport ) bool istOffen() const;
        // Gibt die Anzahl an Unterdateien von dem Verzeichnis zurück
        //  return: 0, falls die Datei kein Verzeichnis ist. Sonst die Anzahl der Unterdateien
        __declspec( dllexport ) int getUnterdateiAnzahl() const;
        // Gibt eine Liste mit unterdateien zurück
        //  return 0, falls die Datei kein Verzeichnis ist. Eine Liste mit den Namen der Unterdateien
        __declspec( dllexport ) RCArray< Text > *getDateiListe() const;
        // Gibt die Größe der Datei zurück
        // return -1, falls die Datei ein Verzeichnis ist oder ein Fehler auftrat. Sonst die größe der Datei
        __declspec( dllexport ) __int64 getSize() const;
        // Gibt den Zeitpunkt der letzten änderung zurück
        //  return: 0, falls ein Fehler aufgetreten ist. Der Zeitpunkt der letzten Änderung sonst
        __declspec( dllexport ) Zeit *getLastChange() const;
        // Prüft, ob die Datei existiert
        //  return: 1, falls die Datei existiert. 0 sonnst
        __declspec( dllexport ) bool existiert() const;
        // Gibt den Index des Bytes aus der Datei zurück, welches als nächstes gelesen werden würde
        // return -1, falls ein Fehler aufgetreten ist. Sonst die Position des Lesezeigers
        __declspec( dllexport ) __int64 getLPosition() const;
        // Gibt den Index des Bytes aus der Datei zurück, welches als nächstes überschrieben werden würde
        // return -1, falls ein Fehler aufgetreten ist. Sonst die Position des Schreibzeigers
        __declspec( dllexport ) __int64 getSPosition() const;
        // Prüft, ob die Datei vollständig gelesen wurde
        //  return 1, wenn die Datei vollständig gelesen wurde. 0, sonst
        __declspec( dllexport ) bool istEnde() const;
        // Gibt den Pfad zur Datei zurück
        __declspec( dllexport ) Text *getPfad() const;
        // Gibt den Pfad zur Datei ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Text *zPfad() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Datei *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Datei *release();
    };

    // Datei Funktionen

    // Sucht einen unbenutzten Dateinamen
    //  zPfad: Ein Zeiger zu dem Pfad, in dem ein unbenutzter Dateiname gefunden werden soll. Ohne erhöhten Reference Counter.
    //          Wird auch als rückgabewert benutzt
    __declspec( dllexport ) void GetFreePfad( Text *zPfad );
    // Erstellt den vollständigen Pfad mit Datei
    //  pfad: Der Pfad zur Datei
    //  return: 1, falls das erstellen erfolgreich war.
    __declspec( dllexport ) bool DateiPfadErstellen( Text* pfad );
    // Löscht die angegebene Datei
    //  pfad: Der Pfad zur Datei
    //  return: 1, falls die Datei gelöscht wurde
    __declspec( dllexport ) bool DateiRemove( Text *pfad );
    // Benennt eine Datei um oder verschiebt sie
    //  pfad_alt: Der Pfad zur Datei, die umbenannt werden soll.
    //  pfad_neu: Der neue Pfad zur Datei. Wenn sich nur der Name ändert, wird sie nur umbenannt
    //  return: 1, wenn das umbenennen erfolgreich war. 0, sonst
    __declspec( dllexport ) bool DateiUmbenennen( Text *pfad_alt, Text *pfad_neu );
    // Prüft, ob Datei existiert
    //  pfad: Der Pfad zur Datei
    //  return: 1, wenn die Datei existiert. 0, wenn die Datei nicht gefunden wurde
    __declspec( dllexport ) bool DateiExistiert( Text *pfad );
    // prüft, ob pfad ein Verzeichnis ist
    //  pfad: Der Pfad zur Datei
    //  return: 1, wenn die Datei ein Verzeichnis ist. 0 sonst
    __declspec( dllexport ) bool DateiIstVerzeichnis( Text *pfad );
    // Erstellt den vollständigen Pfad mit Datei
    //  pfad: Der Pfad zur Datei
    //  return: 1, falls das erstellen erfolgreich war.
    __declspec( dllexport ) bool DateiPfadErstellen( const char *pfad );
    // Löscht die angegebene Datei
    //  pfad: Der Pfad zur Datei
    //  return: 1, falls die Datei gelöscht wurde
    __declspec( dllexport ) bool DateiRemove( const char *pfad );
    // Benennt eine Datei um oder verschiebt sie
    //  pfad_alt: Der Pfad zur Datei, die umbenannt werden soll.
    //  pfad_neu: Der neue Pfad zur Datei. Wenn sich nur der Name ändert, wird sie nur umbenannt
    //  return: 1, wenn das umbenennen erfolgreich war. 0, sonst
    __declspec( dllexport ) bool DateiUmbenennen( const char *pfad_alt, const char *pfad_neu );
    // Prüft, ob Datei existiert
    //  pfad: Der Pfad zur Datei
    //  return: 1, wenn die Datei existiert. 0, wenn die Datei nicht gefunden wurde
    __declspec( dllexport ) bool DateiExistiert( const char *pfad );
    // prüft, ob pfad ein Verzeichnis ist
    //  pfad: Der Pfad zur Datei
    //  return: 1, wenn die Datei ein Verzeichnis ist. 0 sonst
    __declspec( dllexport ) bool DateiIstVerzeichnis( const char *pfad );
}

#endif