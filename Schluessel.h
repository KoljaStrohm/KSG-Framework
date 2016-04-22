#ifndef Schluessel_H
#define Schluessel_H

#include "Betriebssystem.h"

namespace Framework
{
    namespace Verschlüsselung
    {
        // Speichert einen Array von bytes
        class Bytes
        {
        private:
            char *bytes;
            bool del;
            int län;
            int ref;

        public:
            // Erzeugt einen lehren Byte Array
            __declspec( dllexport ) Bytes();
            // Erzeugt einen neuen Byte Array mit bestimmter Länge
            //  län: Die Länge des Arrays
            __declspec( dllexport ) Bytes( int län );
            // Erzeugt ein neuen Byte Array durch kopieren der bytes
            //  daten: Die Bytes, die kopiert werden sollen
            //  län: Die Anzahl der Bytes die kopiert werden sollen
            __declspec( dllexport ) Bytes( const char *daten, int län );
            // Löscht den Array
            __declspec( dllexport ) ~Bytes();
            // Befüllt den Bytearray durch kopieren der Bytes
            //  daten: Die Bytes, die kopiert werden sollen
            __declspec( dllexport ) void setBytes( const char *daten );
            // Löscht den Array und erzeugt einen neuen
            //  daten: Die Bytes, die kopiert werden sollen
            //  län: Die nänge des Arrays
            __declspec( dllexport ) void setBytes( const char *daten, int län );
            // Löscht den Array und übernimmt den übergebenen ohne zu kopieren
            //  daten: Der neue Byte Array
            //  län: Die nänge des Arrays
            __declspec( dllexport ) void setBytesZ( char *daten, int län );
            // Setzt alle Bytes des Arrays auf einen bestimmten Wert
            //  c: Der Wert, auf den die Bytes gesetzt werden sollen
            __declspec( dllexport ) void füll( const char c );
            // Setzt eine bestimmte Anzahl von Bytes des Arrays auf einen bestimmten Wert
            //  c: Der Wert, auf den die Bytes gesetzt werden sollen
            //  län: Die Anzahl der Bytes die gesetzt werden sollen
            __declspec( dllexport ) void füll( const char c, int län );
            // Setzt einen bestimmte Abschnitt von Bytes des Arrays auf einen bestimmten Wert
            //  c: Der Wert, auf den die Bytes gesetzt werden sollen
            //  beg: Die Startposition des zu setzenden Abschnittes
            //  end: Die Endposition des zu setzenden Abschnittes (nicht enthalten)
            __declspec( dllexport ) void füll( const char c, int beg, int end );
            // Kopiert bestimmte bytes in den Array
            //  c: Die Bytes, die kopiert werden sollen
            //  cLän: Die Anzahl an Bytes, die gesetzt werden sollen
            __declspec( dllexport ) void füll( const char *c, int cLän );
            // Setzt ein bestimmtes Byte auf einen Wert
            //  c: Der Wert, auf den das Byte gesetzt werden soll
            //  pos: Die Position des Bytes im Array
            __declspec( dllexport ) void set( const char c, int pos );
            // Gibt die Länge des Arrays zurück
            __declspec( dllexport ) int getLänge() const;
            // Gibt den Array von Bytes zurück
            __declspec( dllexport ) char *getBytes() const;
            // Erhöht den Reference Counting Zähler.
            //  return: this.
            __declspec( dllexport ) Bytes *getThis();
            // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
            //  return: 0.
            __declspec( dllexport ) Bytes *release();
        };

        // Kann Bytesequenzen mit bestimmten Schlüsseln verschlüsseln und entschlüsseln
        class Schlüssel
        {
        private:
            unsigned char *schlüssel;
            int län;
            int pos;
            int ref;

        public:
            // Erzeugt ein leeres Zeichnung
            __declspec( dllexport ) Schlüssel();
            // Erzeugt ein neues Zeichnung mi einem Schlüssel
            //  s: Der Schlüssel, der zum verschlüsseln und entchlüsseln verwendet werden soll
            //  län: Die Länge des Schlüssels
            __declspec( dllexport ) Schlüssel( const char *s, int län );
            // Löscht das Zeichnung
            __declspec( dllexport ) ~Schlüssel();
            // Setzt die Position im Schlüssel, wo mit dem verschlüsseln und entschlüsseln begonnen werden soll
            //  p: Die Position im Schlüssel
            __declspec( dllexport ) void setPos( int p );
            // Setzt den Schlüssel, der zum ver- und entschlüsseln verwendet werden soll
            //  s: Der Schlüssel
            //  län: Die Länge des Schlüssels
            __declspec( dllexport ) void setSchlüssel( const char *s, int län );
            // Verschlüsselt einen Byte Array mit dem gesetzten Schlüssel
            //  daten: Der Byte Array, der verschlüsselt werden soll. Wird von der Funktion verändert
            __declspec( dllexport ) void codieren( Bytes *daten );
            // Entschlüsselt einen Byte Array mit dem gesetzten Schlüssel
            //  daten: Der Byte Array, der entschlüsselt werden soll. Wird von der Funktion verändert
            __declspec( dllexport ) void decodieren( Bytes *daten );
            // Erhöht den Reference Counting Zähler.
            //  return: this.
            __declspec( dllexport ) Schlüssel *getThis();
            // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
            //  return: 0.
            __declspec( dllexport ) Schlüssel *release();
        };
    }
}

#endif