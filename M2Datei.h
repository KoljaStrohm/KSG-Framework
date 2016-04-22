#ifndef M2Datei_H
#define M2Datei_H

#include "Array.h"

namespace Framework
{
    class Model2DData; // Model2D.h
    class Text; // Text.h

    // Diese Klasse verwaltet das Framework eigenen Dateivormat für 2D Modell Daten
    // Es können mehrere 2D Modell Daten in einer Datei gespeichert werden
    class M2Datei
    {
    private:
        Text *pfad;
        RCArray< Text > *modelName;
        Array< __int64 > *modelPos;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) M2Datei();
        // Konstruktor
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) M2Datei( const char *pfad );
        // Konstruktor
        //  pfad: Der Pfad zur Datei
        __declspec( dllexport ) M2Datei( Text *pfad );
        // Destruktor
        __declspec( dllexport ) ~M2Datei();
        // Setzt den Pfad zur Datei
        //  pfad: Pfad zur Datei
        __declspec( dllexport ) void setPfad( const char *pfad );
        // Setzt einen Zeiger auf den Pfad zur Datei
        //  pfad: Zeiger auf den Pfad zur Datei
        __declspec( dllexport ) void setPfadZ( Text *pfad );
        // Ließt grundlegende Informationen aus der Datei, die für ihre Verwendung benötigt werden
        __declspec( dllexport ) void leseDaten();
        // Speichert 2D Modell Daten in der Datei
        //  zMdr: Ein Zeiger auf die zu speichernden Daten ohne erhöhtem Reference Counter
        //  name: Der Name, unter dem die Daten in der Datei gespeichert werden sollen
        //  return: 1, falls das Modell gespeichert wurde. 0, falls ein fehler beim speichern auftrat
        __declspec( dllexport ) bool saveModel( Model2DData *zMdr, Text *name );
        // Speichert 2D Modell Daten in der Datei
        //  zMdr: Ein Zeiger auf die zu speichernden Daten ohne erhöhtem Reference Counter
        //  name: Der Name, unter dem die Daten in der Datei gespeichert werden sollen
        //  return: 1, falls das Modell gespeichert wurde. 0, falls ein fehler beim speichern auftrat
        __declspec( dllexport ) bool saveModel( Model2DData *zMdr, const char *name );
        // Löscht ein 2D Modell aus der Datei
        //  name: Der Name des Modells
        //  return: 1, wenn das Modell gelöscht wurde. 0, wenn das Modell nicht gefunden wurde, oder ein fehler beim speichern auftrat
        __declspec( dllexport ) bool löscheModel( Text *name );
        // Löscht ein 2D Modell aus der Datei
        //  name: Der Name des Modells
        //  return: 1, wenn das Modell gelöscht wurde. 0, wenn das Modell nicht gefunden wurde, oder ein fehler beim speichern auftrat
        __declspec( dllexport ) bool löscheModel( const char *name );
        // Lähd ein 2D Modell aus der Datei
        //  name: Der name des zu ladenden Modells
        //  return: Die geladenen Daten
        __declspec( dllexport ) Model2DData *ladeModel( Text *name ) const;
        // Lähd ein 2D Modell aus der Datei
        //  name: Der name des zu ladenden Modells
        //  return: Die geladenen Daten
        __declspec( dllexport ) Model2DData *ladeModel( const char *name ) const;
        // überprft, ob ein bestimmtes 2D Modell in der Datei existiert
        //  name: Der Name des zu suchenden 2D Modells
        //  return: 1, wenn das Modell gefunden wurde. 0 sonst
        __declspec( dllexport ) bool hatModel( const char *name ) const;
        // ügibt die Anzahl der gespeicherten Modelle zurück
        __declspec( dllexport ) int getModelAnzahl() const;
        // Gibt den Namen eines Bestimmten Modells zurück
        //  i: Der Index des Modells
        //  return: Ein Zeiger aud den Namen des Modells ohne erhöhten Reference Counter
        __declspec( dllexport ) Text *zModelName( int i ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) M2Datei *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) M2Datei *release();
    };
}

#endif