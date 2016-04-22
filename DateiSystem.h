#ifndef DateiSystem_H
#define DateiSystem_H

#include <fstream>
#include "Array.h"
#include "Punkt.h"

namespace Framework
{
    class Bild; // Bild.h
    class Text; // Text.h
#ifdef WIN32
    class Buchstabe; // Schrift.h
    class Alphabet; // Schrift.h
    class Schrift; // Schrift.h
    class FBalken; // Fortschritt.h
#else
#define FBalken int
#endif
    class LTDBPixel; // aus dieser Datei
    class LTDBKopf; // aus dieser Datei
    class LTDBKörper; // aus dieser Datei
    class LTDBDatei; // aus dieser Datei
    class LTDSPixel; // aus dieser Datei
#ifdef WIN32
    class LTDSDateiKopf; // aus dieser Datei
    class LTDSSchriftKopf; // aus dieser Datei
    class LTDSBuchstabenKopf; // aus dieser Datei
    class LTDSBuchstabenKörper; // aus dieser Datei
    class LTDSDatei; // aus dieser Datei
#endif

    // LTDB Dateivormat --- Dient zum speichern von mehreren Bildern in einer Datei.

    // Dient zum Speichern und Laden eines einzelnen Pixels aus einem Bild im LTDB Dateiformat
    class LTDBPixel // Pixel einer LTDB Datei
    {
    private:
        LTDBPixel *davor; // Letzter Pixel
        char index; // Bitlänge des Pixels
        char iR, iG, iB, iA;
        char miR, miG, miB, miA;
        char maxIndex; // Länge des Pixels
        bool änder : 1; // Verändert sich etwas an den volgenden 5 Variablen
        bool änderR : 1; // Ändert sich Rot
        bool änderG : 1; // Ändert sich Grün
        bool änderB : 1; // Ändert sich Blau
        bool änderA : 1; // Ändert sich Alpha
        unsigned char komp : 3; // Komprimierung der Farbwerte
        unsigned char R; // Rot
        unsigned char G; // Grün
        unsigned char B; // Blau
        unsigned char A; // Alpha
        bool addBitZuFarbe( unsigned char bit ); // Fügt den Farbwerten ein Bit hinzu
        bool getNextFarbeBit( char &byte, int i ); // Speichert das nächste Farbbit in byte
        int ref;

    public:
        // Konstruktor
        //  davor: Der Pixel, der Vorher geladen wurde. 0, falls dieß der Erste Pixel ist
        __declspec( dllexport ) LTDBPixel( LTDBPixel *davor );
        // Destruktor 
        __declspec( dllexport ) ~LTDBPixel();
        // zum Laden gedacht. Fügt dem Pixel einiege bits hinzu
        //  byte: Das zuletzt aus der Datei gelesene Byte
        //  begin: Der Index des ersten Bits im byte, wo der Pixel beginnt
        //  return: Der Index des Bits im Byte, wo der Pixel aufgehöhrt hat. -1, falls der Pixel am ende Des Bytes noch nicht zuende ist
        __declspec( dllexport ) char addByte( char byte, char begin );
        // zum speichern gedacht. Setzt die Farbe, die im Pixel gespeichert werden soll
        //  f: Die zu speichernde Farbe
        __declspec( dllexport ) void setFarbe( int f );
        // Komprimiert den Pixel. Muss vor dem Speichern aufgerufen werden.
        __declspec( dllexport ) void komprimieren();
        // Gibt ein Teil der Bits zurück, die den Pixel representieren
        //  byte: Eine Referens auf das Byte, dass als nächstes gespeichert werden soll
        //  begin: Der Index des ersten Bits im zu speichernden Byte, wo der Pixel gespeichert werden soll
        //  return: Der Index des Bits im zu speichernden Byte, wo der Pixel aufhöhrt. -1, falls der Pixel im nächsten Byte fortgesetzt werden muss
        __declspec( dllexport ) char getNextByte( char &byte, int begin );
        // Gibt den Farbwert des Pixels zurück
        __declspec( dllexport ) int zuFarbe() const;
        // Gibt zurück, ob sich der Anteil an Rot in der Farbe im Vergleich zum Pixel davor geändert hat
        __declspec( dllexport ) bool getÄnderR() const;
        // Gibt zurück, ob sich der Anteil an Grün in der Farbe im Vergleich zum Pixel davor geändert hat
        __declspec( dllexport ) bool getÄnderG() const;
        // Gibt zurück, ob sich der Anteil an Blau in der Farbe im Vergleich zum Pixel davor geändert hat
        __declspec( dllexport ) bool getÄnderB() const;
        // Gibt zurück, ob sich der Anteil an Alpha in der Farbe im Vergleich zum Pixel davor geändert hat
        __declspec( dllexport ) bool getÄnderA() const;
        // Gibt die Komprimierung des Pixels zurück
        __declspec( dllexport ) unsigned char getKomp() const;
        // Gibt den Anteil an Rot in der Farbe des Pixels zurück
        __declspec( dllexport ) unsigned char getR() const;
        // Gibt den Anteil an Grün in der Farbe des Pixels zurück
        __declspec( dllexport ) unsigned char getG() const;
        // Gibt den Anteil an Blau in der Farbe des Pixels zurück
        __declspec( dllexport ) unsigned char getB() const;
        // Gibt den Anteil an Alpha in der Farbe des Pixels zurück
        __declspec( dllexport ) unsigned char getA() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDBPixel *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDBPixel *release();
    };

    // Der Kopf des LTDB Dateiformates. Hier werden informationen über alle Abgespeicherten Bilder verwaltet
    class LTDBDateiKopf
    {
    private:
        RCArray< Text > *bilder;
        Array< __int64 > *pos;
        int bAnzahl;
        int ref;

    public:
        // konstructor
        __declspec( dllexport ) LTDBDateiKopf();
        // destructor 
        __declspec( dllexport ) ~LTDBDateiKopf();
        // Entfernt ein Bild aus dem Dateikopf
        //  i: Der Index des Bildes, welches entfernt werden soll
        __declspec( dllexport ) void removeBild( int i );
        // Entfernt ein Bild aus dem Dateikopf
        //  txt: Der Name des Bildes, welches entfernt werden soll
        __declspec( dllexport ) void removeBild( Text *txt );
        // Fügt dem Dateikopf ein Bild hinzu
        //  txt: Der Name des Bildes.
        __declspec( dllexport ) void addBild( Text *txt );
        // Legt den Index des Bytes aus der Datei fest, wo das Bild anfängt
        //  i: Der index Des Bildes
        //  pos: Die Position des Bildes in der Datei
        __declspec( dllexport ) void setBildPos( int i, __int64 pos );
        // Legt den Index des Bytes aus der Datei fest, wo das Bild anfängt
        //  txt: Der Name Des Bildes
        //  pos: Die Position des Bildes in der Datei
        __declspec( dllexport ) void setBildPos( Text *txt, __int64 pos );
        // Lädt den Dateikopf einer LTDB Datei
        //  f: Ein Zeiger auf einen Fortschrittsbalken, der zum Laden verwendet werden soll. Kann 0 sein.
        //  inF: Der geöffnete ifstream der LTDB Datei, bei dem die Leseposition bereits auf das erste Byte des Dateiopfes zeigt.
        __declspec( dllexport ) void laden( FBalken *f, std::ifstream *inF );
        // Speichert den Dateikopf in einer LTDB Datei
        //  outF: Der geöffnete ofstream der LTDB Datei, bei dem die Schreibposition bereits auf das erste Byte des Dateikopfes zeigt.
        __declspec( dllexport ) void speichern( std::ofstream *outF ) const;
        // Gibt den Namen eines bestimmten Bildes zurück
        //  i: Der Index des Bildes, dessen Namen zurückgegeben werden soll
        //  return: Der Name des bildes
        __declspec( dllexport ) Text *getBild( int i ) const;
        // Gibt den Namen eines bestimmten Bildes zurück
        //  i: Der Index des Bildes, dessen Namen zurückgegeben werden soll
        //  return: Der Name des bildes ohne erhöhten Reference Counter
        __declspec( dllexport ) Text *zBild( int i ) const;
        // Gibt den Index des Ersten Bytes eines Bildes in der Datei zurück
        //  txt: Der Name des Bildes, dessen Beginn gefunden werden soll
        //  return: -1, falls das Bild nicht gefunden wurde.
        __declspec( dllexport ) __int64 getBildPosition( Text *txt ) const;
        // Gibt den Index des Ersten Bytes eines Bildes in der Datei zurück
        // Fals der Index nicht existiert wird die Exception std::out_of_range geworfen.
        //  indes: Der Indes des Bildes, dessen Beginn gefunden werden soll
        __declspec( dllexport ) __int64 getBildPosition( int index ) const;
        // Gibt den Index eines Bestimmten Bildes zurück
        //  txt: Der Name des Bildes
        //  return: -1, falls das Bild nicht gefunden wurde
        __declspec( dllexport ) int getBildIndex( Text *txt ) const;
        // Gibt die Anzahl der Bilder in der Datei zurück
        __declspec( dllexport ) int getbAnzahl() const;
        // Gibt eine Liste mit Bildern in der Datei ohne erhöhten Reference Counter zurück.
        // Die Liste sollte nicht verändert werden
        __declspec( dllexport ) RCArray< Text > *zBildListe() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDBDateiKopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDBDateiKopf *release();
    };

    // Die im LTDB Dateikopf gespeicherten Informationen für ein einzelnes Bild
    class LTDBKopf
    {
    private:
        __int64 a; // Det LTDB Dateikopf ist maximal 104 Bits lang
        __int32 b; // - bis zu 75 Bits für den Titel
        __int8 c;  // - 12 Bits für breite
        int ref;   // - 12 Bits för höhe

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDBKopf();
        // Lähdt die Informationen über ein Bild
        //  f: Der geöffnette und an die richtiege Stelle zeigende ifstream der LTDB Datei
        __declspec( dllexport ) void laden( std::ifstream *f );
        // Setzt die Informationen die gespeichert werden sollen
        //  titel: Der Titel des Bildes
        //  größe: Die Größe des Bildes
        //  return: Die Anzahl der Buchstaben aus dem Titel, die im LTDB Dateiformat nicht gespeichert werden können. Erlaubt ist nur a-z und A-Z und ä ü ö ß und Ä Ü Ö und .
        // Alle großbuchstaben im Titel werden in Kleinbuchstaben umgewandelt
        __declspec( dllexport ) int Init( Text *titel, const Punkt &größe );
        // Lähd informationen aus geladenen Bits. Wird von der laden( std::ifstream ) Funktion verwendet.
        //  BeginBit: Der Index des ersten Bits, welches ausgewertet werden soll
        //  EndBit: Der Index des letzten Bits, welches nichtmehr ausgewertet werden soll
        //  bits: Die Bits, von denen alle von BeginBit bis EndBit ausgewertet werden sollen
        // Insgesamt müssen 104 Bits gesetzt werden. Hierauf bezihen sich BeginBit und EndBit
        __declspec( dllexport ) void setBits( int BeginBit, int EndBit, __int16 bits );
        // Speichert die Informationen in eine Datei
        //  f: Der geöffnete und an die richtiege Stelle zeigende ofstream der LTDB Datei
        __declspec( dllexport ) void speichern( std::ofstream *f ) const;
        // Gibt die Länge des Titels zurück
        __declspec( dllexport ) int getTitelLänge() const;
        // Gibt den Titel des Bildes zurück
        __declspec( dllexport ) Text *getTitel() const;
        // Gibt die Größe des Bildes zurück
        __declspec( dllexport ) Punkt getGröße() const;
        // Gibt die nächsten zu speichernden Bits zurück
        //  begin: Der Index des ersten Bits, in das gespeichert werden soll
        //  end: Der Index des letzten Bits, in das gespeichert werden soll
        //  return: 16 Bits, in denen die Informationen zwischen begin und end stehen
        // Insgesamt müssen 104 Bits gelesen werden. Hierauf bezihen sich BeginBit und EndBit
        __declspec( dllexport ) __int16 getBits( int begin, int end )const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDBKopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDBKopf *release();
    };

    // Verwaltet die Pixeldaten eines einzelnen Bildes einer LTDB Datei
    class LTDBKörper
    {
    private:
        Punkt gr;
        Bild *b;
        int dateiLänge;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDBKörper();
        // Konstruktor
        //  k: Der LTDB Kopf des Bildes, der Informationen über die Größe des Bildes enthält
        __declspec( dllexport ) LTDBKörper( LTDBKopf *k );
        // Destruktor 
        __declspec( dllexport ) ~LTDBKörper();
        // Setzt die Informationen über die Größe des Bildes. Wird zum Laden benötigt.
        //  k: Der LTDB Kopf des Bildes
        __declspec( dllexport ) void init( LTDBKopf k );
        // Setzt die Informationen über die Größe des Bildes. Wird zum Laden benötigt.
        //  k: Der LTDB Kopf des Bildes
        __declspec( dllexport ) void init( LTDBKopf *k );
        // Lädt die Pixeldaten aus der Datei
        //  zF: Ein Fortschrittsbalken, der 0 sein kann
        //  inF: Der geöffnete und an die richtiege Stelle zeigende ifstream der LTDB Datei
        __declspec( dllexport ) void laden( FBalken *zF, std::ifstream *inF );
        // Setzt das Bild, welches gespeichert werden soll
        //  b: Das zu speichernde Bild
        __declspec( dllexport ) void setBild( Bild *b );
        // Speichert die Pixeldaten des Bildes in einer LTDB Datei
        //  zF: Ein Fortschrittsbalken, der 0 sein kann
        //  outF: Der geöffnete und an die richtiege Stelle zeigende ofstream der LTDB Datei
        __declspec( dllexport ) void speichern( FBalken *zF, std::ofstream *outF ) const;
        // Gibt das geladene Bild zurück
        __declspec( dllexport ) Bild *getBild() const;
        // Gibt die Größe des Bildes zurück
        __declspec( dllexport ) const Punkt &getGröße() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDBKörper *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDBKörper *release();
    };

    // Verwaltet eine LTDB Datei
    class LTDBDatei
    {
    private:
        Text *pfad;
        LTDBDateiKopf *datKpf;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDBDatei();
        // Destruktor 
        __declspec( dllexport ) ~LTDBDatei();
        // Setzt den Pfad zu Datei
        //  pfad: Der Pfad
        __declspec( dllexport ) void setDatei( Text *pfad );
        // Erstellt eine neue LTDB Datei
        __declspec( dllexport ) void erstellen();
        // Ließt grundlegende Informationen aus der Datei.
        // Wird benötigt, bevor mit der Datei gearbeitet wird
        //  zF: Ein Fortschrittsbalken, der 0 sein kann
        __declspec( dllexport ) void leseDaten( FBalken *zF );
        // Löscht die LTDB Datei
        __declspec( dllexport ) void löschen();
        // Löscht ein Bild aus der LTDB Datei
        //  zF: Ein Fortschrittsbalken der 0 sein kann
        //  name: Der Name des zu löschenden Bildes
        __declspec( dllexport ) void löschen( FBalken *zF, Text *name );
        // Lädt ein Bild aus der LTDB Datei
        //  zF: Ein Fortschrittsbalken, der 0 sein kann
        //  name: Der Name des Bildes, welches geladen werden soll
        //  return: Das geladene Bild. 0, falls das Bild nicht gefunden wurde
        __declspec( dllexport ) Bild *laden( FBalken *zF, Text *name );
        // Speichert ein neues Bild in der LTDB Datei
        //  zF: Ein Fortscrittsbalken, der 0 sein kann
        //  bild: Das Bild, welches gelöscht werden soll
        //  name: Der Name, unter dem das Bild gespeichert werden soll
        //  return: Anzahl der Warnungen, die beim Konvertieren des Namens in einen gültigen Namen aufgetreten sind. -1, falls bereis ein Bild mit dem selben Namen existiert
        __declspec( dllexport ) int speichern( FBalken *zF, Bild *bild, Text *name );
        // Gibt eine Liste mit gespeicherten Bildern zurück
        // Die Liste sollte nicht verändert werden
        __declspec( dllexport ) RCArray< Text > *zBildListe();
        // Gibt den Pfad zur LTDB Datei zurück
        __declspec( dllexport ) Text *getPfad() const;
        // Gibt die Anzahl der Bilder in der LTDB Datei zurück
        __declspec( dllexport ) int getBildAnzahl() const;
        // Prüft, ob die LTDB Datei existiert
        __declspec( dllexport ) bool istOffen() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDBDatei *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDBDatei *release();
    };

#ifdef WIN32
    // LTDS Dateivormat --- Dient zum speichern von Schriftarten

    // Verwaltet einen einzelnen Pixel von einem Buchstaben
    class LTDSPixel
    {
    private:
        int ref;
        char index;
        char iA;
        char miA;
        char maxIndex; // Länge des Pixels
        bool änder;
        bool änderA;
        unsigned char komp : 3; // Komprimierung der Farbwerte
        unsigned char alpha;
        LTDSPixel *davor;
        bool addBitZuFarbe( unsigned char bit ); // Fügt den Farbwerten ein Bit hinzu
        bool getNextFarbeBit( char &byte, int i ); // Speichert das nächste Farbbit in byte

    public:
        // Konstruktor
        //  davor: Der Pixel, der vor diesem geladen wurde. 0, fals dieß der erste Pixel ist
        __declspec( dllexport ) LTDSPixel( LTDSPixel *davor );
        // Destruktor 
        __declspec( dllexport ) ~LTDSPixel();
        // Fügt dem Pixel einiege geladene Bits hinzu. Zum Laden gedacht.
        //  byte: Das letzte aus der Datei geladene Byte.
        //  begin: Der Index des ersten Bits im Byte, wo der Pixel beginnt
        //  return: Der Index des Letzten Bits in Byte, wo der Pixel aufhöhrt. -1, falls der Pixel im nächsten Byte weitergeht
        __declspec( dllexport ) char addByte( char byte, char begin );
        // Setzt den Alpha Wert des Pixels. Zum speichern gedacht.
        //  alpha: Der Alpha Wert des Pixels.
        __declspec( dllexport ) void setAlpha( unsigned char alpha );
        // Komprimiert den Pixel. Muss vor dem Speichern aufgerufen werden.
        __declspec( dllexport ) void Komp();
        // Gibt ein Teil der Bits zurück, die den Pixel representieren
        //  byte: Eine Referens auf das Byte, dass als nächstes gespeichert werden soll
        //  begin: Der Index des ersten Bits im zu speichernden Byte, wo der Pixel gespeichert werden soll
        //  return: Der Index des Bits im zu speichernden Byte, wo der Pixel aufhöhrt. -1, falls der Pixel im nächsten Byte fortgesetzt werden muss
        __declspec( dllexport ) char getNextByte( char &byte, int bbegin );
        // Gibt die Komprimierung des Pixels zurück
        __declspec( dllexport ) unsigned char getKomp() const;
        // Gibt zurück, ob sich der Alpha Wert im Vergleich zu dem Pixel davor geändert hat
        __declspec( dllexport ) bool getÄnderA() const;
        // Gibt den Alpha Wert des Pixels zurück
        __declspec( dllexport ) unsigned char getA() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSPixel *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSPixel *release();
    };

    // Verwaltet den Kopf einer LTDS Datei. Er enthält Informationen über die in der Datei gespeicherten Schriftgrößen
    class LTDSDateiKopf
    {
    private:
        int ref;
        unsigned char sganzahl;
        unsigned char *gr;
        int *pos;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDSDateiKopf();
        // Destruktor 
        __declspec( dllexport ) ~LTDSDateiKopf();
        // Lädt den Kopf aus der LTDS Datei
        //  inF: Der geöffnete und an die richtige Stelle zeigende ifstream der LTDS Datei
        __declspec( dllexport ) void laden( std::ifstream *inF );
        // Fügt eine Schriftgröße hinzu
        //  sg: Die Schriftgröße, die hinzugefügt werden soll
        __declspec( dllexport ) void addSG( char sg );
        // Löscht eine Schriftgröße
        //  sg: Die Schriftgröße, die gelöscht werden soll
        __declspec( dllexport ) void removeSG( char sg );
        // Speichert den LTDS Kopf in der Datei
        //  outF: Der geöffnete und an die richtiege Stelle zeigende ofstream der LTDS Datei
        __declspec( dllexport ) void speichern( std::ofstream *outF ) const;
        // Gibt einen Array von gespeicherten Schriftgrößen zurück
        // Der Array sollte nicht verädert werden
        __declspec( dllexport ) unsigned char *getSchriftGrößeList() const;
        // Gibt einen Array mit positionen der ersten Bytes für jede Schriftgröße aus der Datei zurück
        __declspec( dllexport ) int *getPositionList() const;
        // Gibt die Anzahl der gespeicherten Schriftgrößen zurück
        __declspec( dllexport ) int getSchriftGrößeAnzahl() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSDateiKopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSDateiKopf *release();
    };

    // Der Kopf einer Schriftgröße. Enthält Informationen über die gespeicherten Zeichen
    class LTDSSchriftKopf
    {
    private:
        int ref;
        unsigned char schriftGröße;
        unsigned char *zeichen;
        int *pos;
        unsigned char zeichenAnzahl;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDSSchriftKopf();
        // Destruktor 
        __declspec( dllexport ) ~LTDSSchriftKopf();
        // Lädt den Kopf einer Schriftgröße aus der Datei
        //  inF: Der geöffnete und an de richtiege Stelle zeigende ifstream der LTDS Datei
        __declspec( dllexport ) void laden( std::ifstream *inF );
        // Setzt die Schriftgröße. Zum speichern gedacht
        //  gr: Die Schriftgröße
        __declspec( dllexport ) void setSchriftgröße( unsigned char gr );
        // Setzt das Alphabet, welches in der Schriftgröße gespeichert werden soll
        //  alphabet: Das Alphabet, welches alle zu speichernden Zeichen in der Schriftgröße enthält
        __declspec( dllexport ) void setZeichenAlphabet( Alphabet *alphabet );
        // Fügt der Schriftgröße ein Zeichen hinzu, welches gespeichert werden soll
        //  zeichen: Der ASCII code des Zeichens, welches hinzugefügt werden soll
        __declspec( dllexport ) void addZeichen( unsigned char zeichen );
        // Löscht ein Zeichen aus der Schriftgröße
        //  zeich: Der ASCII code des Zeichens, welches gelöscht werden soll
        __declspec( dllexport ) void removeZeichen( unsigned char zeich );
        // Speichert den Kopf der Schriftgröße in der LTDS Datei
        //  outF: Der geöffnete und an die richtiege Stelle zeigende ofstream der LTDS Datei
        __declspec( dllexport ) void speichern( std::ofstream *outF ) const;
        // Gibt die Schriftgröße zurück, zu der dieser Kopf gehöhrt
        __declspec( dllexport ) unsigned char getSchriftGröße() const;
        // Gibt die Anzahl der in der Schriftgröße gespeicherten Zeichen zurück
        __declspec( dllexport ) unsigned char getZeichenAnzahl() const;
        // Gibt einen Array mit den Positionen der ersten Bytes von den gespeicherten Zeichen aus der LTDS Datei zurück 
        __declspec( dllexport ) int *getPositionen() const;
        // Gibt einen Array mit den ASCII codes der Gespeicherten Zeichen zurück
        __declspec( dllexport ) unsigned char *getZeichen() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSSchriftKopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSSchriftKopf *release();
    };

    // Der Kopf eines einzelnen Zeichens aus der LTDS Datei. Enthält informationen über die Pixelgröße des Zeichens
    class LTDSBuchstabenKopf
    {
    private:
        int ref;
        unsigned char zeichen;
        Punkt größe;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDSBuchstabenKopf();
        // Lädt die Daten aus der LTDS Datei
        //  inF: Der geöffnete und an die richtiege Stelle zeigende ifstream der LTDS Datei
        __declspec( dllexport ) void laden( std::ifstream *inF );
        // Setzt die Daten, die gespeichert werden sollen.
        //  zeichen: Der ASCII code des Zeichens
        //  größe: Die Größe des Zeichens in Pixeln
        __declspec( dllexport ) void init( unsigned char zeichen, const Punkt &größe );
        // Setzt die Daten, die gespeichert werden sollen.
        //  zeichen: Der ASCII code des Zeichens
        //  br: Die Breite des Zeichens in Pixeln
        //  hö: Die Höhe des Zeichens in Pixeln
        __declspec( dllexport ) void init( unsigned char zeichen, int br, int hö );
        // Speichert die Daten in der LTDS Datei
        //  outF: Der geöffnete und auf die richtiege Stelle zeigende ofstream der LTDS Datei
        __declspec( dllexport ) void speichern( std::ofstream *outF ) const;
        // Gibt den ASCII code des Zeichens zurück
        __declspec( dllexport ) unsigned char getZeichen() const;
        // Gibt die Breite des Zeichens in Pixeln zurück
        __declspec( dllexport ) int getBreite() const;
        // Gibt die Höhe des Zeichens in Pixeln zurück
        __declspec( dllexport ) int getHöhe() const;
        // Gib t die Größe des Zeichens in Pixeln zurück
        __declspec( dllexport ) const Punkt &getGröße() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSBuchstabenKopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSBuchstabenKopf *release();
    };

    // Verwaltet die Pixeldaten eines Zeichens
    class LTDSBuchstabenKörper
    {
    private:
        int ref;
        Punkt größe;
        unsigned char zeichen;
        Buchstabe *buchstabe;

    public:
        // Konstruktor
        //  kopf: Der Kopf des Zeichens
        __declspec( dllexport ) LTDSBuchstabenKörper( LTDSBuchstabenKopf *kopf );
        // Destruktor 
        __declspec( dllexport ) ~LTDSBuchstabenKörper();
        // Setzt den Buchstaben, der gespeichert werden soll
        //  zeichen: Der zu speichernde Buchstabe
        __declspec( dllexport ) void setBuchstabe( Buchstabe *zeichen );
        // Lädt die Pixel aus der LTDS Datei
        //  inF: Der geöffnete und auf die richtiege Stelle zeigende ifstream der LTDS Datei
        __declspec( dllexport ) void laden( std::ifstream *inF );
        // Speichert die Pixel in die LTDS Datei
        //  outF: der geöffnete und an die richtiege Stelle zeigende ofstream der LTDS Datei
        __declspec( dllexport ) void speichern( std::ofstream *outF ) const;
        // Gibt den geladenen Buchstaben zurück
        __declspec( dllexport ) Buchstabe *getBuchstabe() const;
        // Gibt den ASCII code des Buchstabens zurück
        __declspec( dllexport ) unsigned char getZeichen() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSBuchstabenKörper *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSBuchstabenKörper *release();
    };

    // Verwaltet eine LTDS Datei
    class LTDSDatei
    {
    private:
        int ref;
        Text *pfad;
        LTDSDateiKopf *dateiKopf;

    public:
        // Konstruktor 
        __declspec( dllexport ) LTDSDatei();
        // Destruktor 
        __declspec( dllexport ) ~LTDSDatei();
        // Setzt den Pfad zur Datei
        //  txt: Der Pfad
        __declspec( dllexport ) void setPfad( Text *txt );
        // Lädt wichtiege Informationen aus der Datei. Muss vor dem Verwenden der Datei aufgerufen werden
        __declspec( dllexport ) void leseDaten();
        // Fügt der Datei eine Schriftgröße hinzu, fals diese noch nicht existiert
        //  alphabet: Das Alphabet, welches die Zeichen in der gewünschten Schriftgrö0e enthält
        __declspec( dllexport ) void addSchriftgröße( Alphabet *alphabet );
        // Fügt einer Schriftgröße einen Buchstaben hinzu
        //  gr: Die Schriftgröße des Buchstabens
        //  zeich: Der Buchstabe, der gespeichert werden soll
        //  zeichen: Der ASCII code des Buchstabens
        __declspec( dllexport ) void addBuchstabe( int gr, Buchstabe *zeich, unsigned char zeichen );
        // Löscht eine bestimmte Schriftgröße aus der Datei
        //  gr: Die Schriftgröße, die entfernt werden soll
        __declspec( dllexport ) void löscheSchrifrGröße( int gr );
        // Löscht einen Buchstaben aus einer Schriftgröße
        //  gr: Die Schriftgröße, aus der der Buchstabe entfernt werden soll
        //  zeichen: Der ASCII code des Zeichens, welches gelöscht werden soll
        __declspec( dllexport ) void löscheBuchstabe( int gr, unsigned char zeichen );
        // Löscht die LTDS Datei
        __declspec( dllexport ) void löscheDatei();
        // Erstellt die LTDS Datei
        __declspec( dllexport ) void erstelleDatei();
        // Speichert eine gesammte Schrift in der Datei
        //  schrift: Die schrift, die gespeichert werden soll
        __declspec( dllexport ) void speicherSchrift( Schrift *schrift );
        // Lädt die gesammte Schrift aus der Datei
        //  return: Die geladene Schrift. 0, falls ein Fehler beim Laden aufgetreten ist
        __declspec( dllexport ) Schrift *ladeSchrift();
        // Lädt eine einzelne Schriftgröße aus der Datei
        //  schriftgröße: Die zu ladende Schriftgröße
        //  return: Ein Alphabet mit den Zeichen in der Schriftgröße. 0, falls die Schriftgröße nicht gefunden wurde
        __declspec( dllexport ) Alphabet *ladeAlphabet( int schriftgröße );
        // Lädt ein bestimmtes Zeichen einer bestimmten Schriftgröße
        //  schriftgröße: Die Schriftgröße, zu dem das Zeichen gehört
        //  zeichen: Der ASCII code des zu ladenden Zeichens
        //  return: Der geladene Buchstabe. 0, falls das Zeichen nicht gefunden wurde.
        __declspec( dllexport ) Buchstabe *ladeBuchstabe( int schriftgröße, unsigned char zeichen );
        // Gibt den Pfad zur LTDS Datei zurück
        __declspec( dllexport ) Text *getPfad() const;
        // Gibt die Anzahl der gespeicherten Schriftgrößen zurück
        __declspec( dllexport ) int getAnzahlSchriftgrößen() const;
        // Gibt einen Array mit den gespeicherten Schriftgrößen zurück
        // Der Array sollte nicht verändert werden
        __declspec( dllexport ) unsigned char *getSchriftGrößen() const;
        // Gibt die Anzahl an gespeicherten Zeichen in einer Schriftgröße zurück
        //  sg: Die Schriftgröße, von der die Anzahl der ZEichen ermittelt werden soll
        //  return: Die Anzahl der Zeichen.
        __declspec( dllexport ) unsigned char getAnzahlBuchstaben( int sg );
        // Gibt einen Array mit Buchstaben einer bestimmten Schriftgröße zurück
        //  sg: Die Schriftgröße
        //  return: Der Array mit den ASCII codes der Zeichen. 0, falls die Schriftgröße nicht gefunden wurde.
        __declspec( dllexport ) unsigned char *getBuchstaben( int sg );
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) LTDSDatei *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) LTDSDatei *release();
    };
#endif
    // Bit Funktionen 

    // gibt 1-bits in gewinschter anzahl zurück.
    //  a: Die Anzahl der Bits, die 1 sein sollen
    //  return: 32 Bits, wobei die einser Bits von rechts beginnen
    __declspec( dllexport ) int Bits( int a );
    // Gibt zurück, wie viele Bits benötigt werden um eine Zahl darzustellen
    //  c: Die Zahl, die dargestellt werden soll
    //  return: Die Anzahl der benötigten Bits
    __declspec( dllexport ) int getBits( char c );
}
#endif