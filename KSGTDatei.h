#ifndef KSGTDatei_H
#define KSGTDatei_H

#include "Array.h"

namespace Framework
{
    class Text;

    // Verwaltet eine Datei in der Daten tabellarisch abgespeichert werden
    class KSGTDatei
    {
    private:
        Text *pfad;
        RCArray< RCArray< Text > > *data;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) KSGTDatei();
        // Konstruktor
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) KSGTDatei( const char *pfad );
        // Konstruktor
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) KSGTDatei( Text *pfad );
        // Destruktor
        __declspec( dllexport ) ~KSGTDatei();
        // Setzt den Pfad zur Datei
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) void setPfad( const char *pfad );
        // Setzt den Pfad zur Datei
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) void setPfad( Text *pfad );
        // lähdt alle Daten aus der angegebenen Datei
        //  return: 1, falls das laden erfolgreich war. 0, wenn ein Fehler beim laden aufgetreten ist
        __declspec( dllexport ) bool laden();
        // Fügt eine Zeile zur Tabelle hinzu
        //  feldAnzahl: Die Anzahl der Felder in der Zeile
        //  zWert: Einen Zeiger auf die Werte in der Zeile ohne erhöhten Reference Counter
        //  return: 1, wenn kein Fehler aufgetreten ist
        __declspec( dllexport ) bool addZeile( int feldAnzahl, RCArray< Text > *zWert );
        // Ersetzt eine existierende Zeile
        //  zeile: Der Index der Zeile, die ersetzt werden soll
        //  feldAnzahl: Die Anzahl der Felder in der Zeile
        //  zWert: Einen Zeiger auf die Werte in der Zeile ohne erhöhten Reference Counter
        //  return: 1, wenn die Zeile existierte und ersetzt wurde. 0, wenn die angegebene Zeile nicht existierte
        __declspec( dllexport ) bool setZeile( int zeile, int feldAnzahl, RCArray< Text > *zWert );
        // Löscht eine Zeile
        //  zeile: Der Index der Zeile, die gelöscht werden soll
        //  return: 1, wenn die Zeile gelöscht wurde. 0, wenn die Zeile nicht gefunden wurde
        __declspec( dllexport ) bool löscheZeile( int zeile );
        // Fügt einer Bestimmten Zeile einen Wert hinzu
        //  zeile: Der Index der Zeile, der ein Wert hinzugefügt werden soll
        //  pos: Die Position in der Zeile, an der der Wert hinzugefügt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich hinzugefügt wurde. 0, falls die Zeile nicht existiert, oder pos zu groß ist
        __declspec( dllexport ) bool addFeld( int zeile, int pos, Text *wert );
        // Fügt einer Bestimmten Zeile einen Wert hinzu
        //  zeile: Der Index der Zeile, der ein Wert hinzugefügt werden soll
        //  pos: Die Position in der Zeile, an der der Wert hinzugefügt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich hinzugefügt wurde. 0, falls die Zeile nicht existiert, oder pos zu groß ist
        __declspec( dllexport ) bool addFeld( int zeile, int pos, const char *wert );
        // Fügt einer Bestimmten Zeile am Ende einen Wert hinzu
        //  zeile: Der Index der Zeile, der ein Wert hinzugefügt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich hinzugefügt wurde. 0, falls die Zeile nicht existiert
        __declspec( dllexport ) bool addFeld( int zeile, Text *wert );
        // Fügt einer Bestimmten Zeile am Ende einen Wert hinzu
        //  zeile: Der Index der Zeile, der ein Wert hinzugefügt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich hinzugefügt wurde. 0, falls die Zeile nicht existiert
        __declspec( dllexport ) bool addFeld( int zeile, const char *wert );
        // Setzt einen Bestimmten Wert in einer Zeile
        //  zeile: Der Index der Zeile, in der ein Wert verändert werden soll
        //  feld: Die Position in der Zeile, an der der Wert gesetzt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich ersetzt wurde. 0, falls die Zeile oder der Wert nicht existiert
        __declspec( dllexport ) bool setFeld( int zeile, int feld, Text *wert );
        // Setzt einen Bestimmten Wert in einer Zeile
        //  zeile: Der Index der Zeile, in der ein Wert verändert werden soll
        //  feld: Die Position in der Zeile, an der der Wert gesetzt werden soll
        //  wert: Der Wert, der gespeichert werden soll
        //  return: 1, falls Der Wert erfolgreich ersetzt wurde. 0, falls die Zeile oder der Wert nicht existiert
        __declspec( dllexport ) bool setFeld( int zeile, int feld, const char *wert );
        // Entfernt einen bestimmten Wert
        //  zeile: Der Index der Zeile, aus der ein Wert gelöscht werden soll
        //  feld: Die Position in der Zeile, an der der Wert gelöscht werden soll
        //  return: 1, falls Der Wert erfolgreich gelöscht wurde. 0, falls die Zeile oder der Wert nicht existiert
        __declspec( dllexport ) bool löscheFeld( int zeile, int feld );
        // Speichert die Tabelle in der Datei
        //  return: 1, falls die Tabbelle erfolgreich gespeichert wurde
        __declspec( dllexport ) bool speichern();
        // Gibt die Anzahl der Zeilen zurück
        __declspec( dllexport ) int getZeilenAnzahl() const;
        // Gibt die Anzahl der Werte (Spalten) in einer Zeile zurück
        //  zeile: Der Index der Zeile, von der die Anzahl Werte ermittelt werden soll
        __declspec( dllexport ) int getFeldAnzahl( int zeile ) const;
        // Gibt einen bestimmten gespeicherten Wert zurück
        //  zeile: Der Index der Zeile, in der der Wert gespeichert wird
        //  feld: Der Index des Wertes in der Zeile
        //  return: Den gespeicherten Wert mit erhöhtem Reference Counter
        __declspec( dllexport ) Text *getFeld( int zeile, int feld ) const;
        // Gibt einen bestimmten gespeicherten Wert zurück
        //  zeile: Der Index der Zeile, in der der Wert gespeichert wird
        //  feld: Der Index des Wertes in der Zeile
        //  return: Den gespeicherten Wert ohne erhöhten Reference Counter
        __declspec( dllexport ) Text *zFeld( int zeile, int feld ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) KSGTDatei *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) KSGTDatei *release();
    };
}

#endif