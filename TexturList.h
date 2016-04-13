#pragma once

#include "Array.h"

namespace Framework
{
    class Textur; // Textur.h

    // Verwaltet alle geladenen Texturdaten, so dass mehrere Zeichnungen die selben Daten benutzen können
    class TexturList
    {
    private:
        static int id;
        static CRITICAL_SECTION cs;
        RCArray< Textur > *textures;
        RCArray< Text > *names;
        int ref;

    public:
        // Konstruktor
        TexturList();
        // Destruktor
        ~TexturList();
        // Fügt der Liste eine Textur hinzu
        //  t: Die Textur
        //  name: Der name, unter dem die Textur in der Liste gespeichert wird
        __declspec( dllexport ) bool addTextur( Textur *t, const char *name );
        // Entfernt eine Textur aus der Liste
        //  name: Der Name der Textur
        __declspec( dllexport ) void löscheTextur( const char *name );
        // Überprüft, ob unter einem bestimmten Namen eine Textur abgespeichert wurde
        //  name: Der Name
        //  return: true, wenn eine Textur mit dem Namen existiert
        __declspec( dllexport ) bool hatTextur( const char *name ) const;
        // Gibt eine bestimmte Textur zurück
        //  name: Der Name der Textur
        __declspec( dllexport ) Textur *getTextur( const char *name ) const;
        // Gibt eine bestimmte Textur zurück
        //  id: Die Id der Textur
        __declspec( dllexport ) Textur *getTextur( int id ) const;
        // Gibt eine bestimmte Textur ohne erhöhten Reference Counter zurück
        //  name: Der Name der Textur
        __declspec( dllexport ) Textur *zTextur( const char *name ) const;
        // Gibt eine bestimmte Textur ohne erhöhten Reference Counter zurück
        //  id: Die Id der Textur
        __declspec( dllexport ) Textur *zTextur( int id ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) TexturList *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) TexturList *release();
        // Initialisiert statische private member. Wird vom Framework automatisch aufgerufen.
        static void init();
        // Löscht statische private member. Wird vom Framework automatisch aufgerufen.
        static void destroy();
    };
}