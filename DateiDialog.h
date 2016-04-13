#ifndef DateiDialog_H
#define DateiDialog_H

#include "Array.h"
#include "Thread.h"

namespace Framework
{
    class Text; // Text.h

    void InitDialog();

    // Erstellt einen Datei öffnen/speichern Dialog
    class DateiDialog
    {
    private:
        RCArray< Text > *typeName;
        RCArray< Text > *type;
        int fileIndex;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) DateiDialog();
        // Destruktor
        __declspec( dllexport ) ~DateiDialog();
        // Löscht die Liste mit zugelassenen Dateitypen
        __declspec( dllexport ) void löscheDateiTypen();
        // Fügt einen zugelassenen Dateityp hinzu
        //  name: Der Name des Dateitypes. Ist für den Nutzer in der Select Box sichtbar
        //  typ: Der Dateityp, der ausgewählt werden darf
        __declspec( dllexport ) void addDateiTyp( char *name, char *typ );
        // Fügt einen zugelassenen Dateityp hinzu
        //  name: Der Name des Dateitypes. Ist für den Nutzer in der Select Box sichtbar
        //  typ: Der Dateityp, der ausgewählt werden darf
        __declspec( dllexport ) void addDateiTyp( Text *name, Text *typ );
        // Setzt den zu Beginn ausgewählten Dateityp
        //  i: Der Index des Dateityps. Der, der als erstes hinzugefügt wurde, hat den Index 0.
        __declspec( dllexport ) void setDateiTypAuswahl( int i );
        // Zeigt den Dateidialog an
        //  open: true, wenn der Dialog zum öffnen dienen soll. false zum Speichern
        //  return: Den Pfad zur ausgewählten Datei
        __declspec( dllexport ) Text *anzeigen( bool open ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) DateiDialog *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) DateiDialog *release();
    };

    // Verwaltet einen Datei öffnen/speichern Dialog ohne zu warten
    class DateiDialogTh : public Thread
    {
    private:
        DateiDialog *dialog;
        Text *ret;
        bool öffnen;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) DateiDialogTh();
        // Destruktor
        __declspec( dllexport ) ~DateiDialogTh();
        // Legt fest, ob der Dialog zum öffnen oder speichern ist
        //  b: 1, wenn er zum öfnen ist. 0, wenn er zum speichern ist
        __declspec( dllexport ) void setÖffnen( bool b );
        // Löscht die liste mit erlaubten Dateitypen
        __declspec( dllexport ) void löscheDateiTypen();
        // Fügt einen zugelassenen Dateityp hinzu
        //  name: Der Name des Dateitypes. Ist für den Nutzer in der Select Box sichtbar
        //  typ: Der Dateityp, der ausgewählt werden darf
        __declspec( dllexport ) void addDateiTyp( char *name, char *typ );
        // Fügt einen zugelassenen Dateityp hinzu
        //  name: Der Name des Dateitypes. Ist für den Nutzer in der Select Box sichtbar
        //  typ: Der Dateityp, der ausgewählt werden darf
        __declspec( dllexport ) void addDateiTyp( Text *name, Text *typ );
        // Setzt den zu Beginn ausgewählten Dateityp
        //  i: Der Index des Dateityps. Der, der als erstes hinzugefügt wurde, hat den Index 0.
        __declspec( dllexport ) void setDateiTypAuswahl( int i );
        // Diese Funktion wird von der Klasse selbst aufgerufen.
        // Benutze die start Funktion um den Dialog anzuzeigen
        __declspec( dllexport ) void thread() override;
        // Gibt den Pfad zur Datei zurück.
        // Funktioniert erst, nachdem der Thread beendet wurde
        __declspec( dllexport ) Text *getPfad() const;
        // Gibt den Pfad zur Datei ohne erhöhten Reference Counter zurück.
        // Funktioniert erst, nachdem der Thread beendet wurde
        __declspec( dllexport ) Text *zPfad() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) DateiDialogTh *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) DateiDialogTh *release();
    };
};

#endif