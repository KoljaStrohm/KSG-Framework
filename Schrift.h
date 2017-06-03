#ifndef Schrift_H
#define Schrift_H

#include "Critical.h"
#include "Punkt.h"

namespace Framework
{
    class Bild; // Bild.h
    class Text; // Text.h
    struct VScrollData; // Scroll.h
    struct HScrollData; // Scroll.h
    class Buchstabe; // aus dieser Datei
    class Alphabet; // aus dieser Datei
    class Schrift; // aus dieser Datei

    // Speichert die Alphawerte eines Zeichens einer bestimmten Schrift
    // Die anderen Farbwerte werden durch die Schriftfarbe bestimmt. Daher nur die Alpha werte.
    class Buchstabe
    {
    private:
        Punkt size;
        Punkt pos;
        unsigned char *alpha;
        int schriftSize;
        int drawSg;
        int ref;

    public:
        // Erstellt ein neues leeres Zeichnung
        __declspec( dllexport ) Buchstabe();
        // Löscht das Zeichnung
        __declspec( dllexport ) ~Buchstabe();

        // Erstellt einen neuen Buchstaben mit bestimmter Größe
        //  size: Die Größe des Buchstabens in Pixel
        __declspec( dllexport ) void NeuBuchstabe( Punkt &size );
        // Setzt den Alphawert eines bestimmten Pixels
        //  pos: Die position des Pixels
        //  alpha: Der Wert des Pixels
        __declspec( dllexport ) void setPixel( Punkt &pos, unsigned char alpha );
        // Setzt den Alphawert eines bestimmten Pixels
        //  x: die x Position des Pixels
        //  y: die y Position des Pixels
        //  alpha: Der Wert des Pixels
        __declspec( dllexport ) void setPixel( int x, int y, unsigned char alpha );
        // Setzt den Alphawert eines bestimmten Pixels
        //  i: der Index des Pixels. Er berechnet sich durch x + y * breite und geht von 0 bis breite * höhe - 1
        //  alpha: Der Wert des Pixels
        __declspec( dllexport ) void setPixel( int i, unsigned char alpha );
        // Setzt die Position, an die der Buchstabe gezeichnet werden soll
        //  pos: Die position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setPosition( Punkt &pos );
        // Setzt die Position, an die der Buchstabe gezeichnet werden soll
        //  x: Die x position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        //  y: Die y position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setPosition( int x, int y );
        // Setzt die Schriftgröße, zu der der Buchstabe gehört
        //  sg: Die Schriftgröße des Buchstabens. Wenn der Buchstabe in einer anderen größe gezeichnet werden soll, wird automatisch skalliert
        __declspec( dllexport ) void setSchriftSize( int sg );
        // Setzt die Schriftgröße, in der der Buchstabe gezeichnet werden soll
        //  dsg: Die Schriftgröße, in der der Buchstabe gezeichnet werden soll. Ist die ungleich der Schriftgröße, zu der der Buchstabe gehört, so wird automatisch skalliert.
        __declspec( dllexport ) void setDrawSchriftSize( int dsg );

        // Gibt die alpha Werte des Buchstabens als array zurück wobei die werte Zeilenweise hintereinander stehen
        __declspec( dllexport ) unsigned char *getBuff() const;
        // Gibt die Größe des Buchstabens in Pixeln nicht skalliert zurück.
        __declspec( dllexport ) const Punkt &getSize() const;
        // Gibt die Breite des Buchstabens in Pixeln zurück (Der Wert wurde bereits mit der Draw Schriftgröße skalliert)
        __declspec( dllexport ) int getBreite() const;
        // Gibt die Höhe des Buchstabens in Pixeln zurück (Der Wert wurde bereits mit der Draw Schriftgröße skalliert)
        __declspec( dllexport ) int getHeight() const;
        // Gibt die nicht skallierte Höhe des Buchstabens in Pixeln zurück
        __declspec( dllexport ) int getNormHeight() const;
        // Zeichnet den Buchstaben in ein bestimmtes Bild
        // Nutze (setPosition) und (setDrawSchriftGröße) um die Position und die Größe zu verändern
        //  zRObj: Das Bild, in den der Buchstabe gezeichnet werden soll
        //  f: Die Farbe, in der der Buchstabe gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj, int f ) const;

        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Buchstabe *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Buchstabe *release();
    };

    // Speichert alle Buchstaben der selben Schriftgröße.
    // Wird von der Schrift klasse verwendet
    class Alphabet
    {
    private:
        Buchstabe **zeichen;
        int schriftSize;
        int drawSchriftSize;
        Punkt pos;
        int zeilenHeight;
        int zeilenAbstand;
        int ref;

    public:
        // Erzeugt ein leeres Zeichnung
        __declspec( dllexport ) Alphabet();
        // Löscht ein Zeichnung
        __declspec( dllexport ) ~Alphabet();

        // Löscht alle gespeicherten Zeichen
        __declspec( dllexport ) void NeuAlphabet();
        // Fügt dem Alphabet einen Buchstaben hinzu
        // Wenn der hinzugefügte Buchstabe bereits existiert wird er überschrieben
        //  i: Der ASCII code des Buchstaben, der hinzugefügt werden soll
        //  buchstabe: Der Buchstabe, der hinzugefügt wird
        __declspec( dllexport ) void setBuchstabe( unsigned char i, Buchstabe *buchstabe );
        // Setzt die Schriftgröße des Alphabets und die der gespeicherten buchstaben
        //  gr: Die Schriftgröße des Alphabets
        __declspec( dllexport ) void setSchriftSize( int gr );
        // Setzt die Schriftgröße, in der die gespeicherten Buchstaben gezeichnet werden sollen.
        //  gr: Die Zeichen Schriftgröße. Wenn sie ungleich der Schriftgröße des Alphabets ist, wird automatisch skalliert
        __declspec( dllexport ) void setDrawSchriftSize( int gr );
        // Setzt den Zeilenabstand beim Zeichnen eines Textes
        //  za: Der Abschtand zum unteren Ende der Zeile darüber in Pixeln
        __declspec( dllexport ) void setZeilenAbstand( int za );
        // Setzt die Position, an die der Erste buchstabe gezeichnet werden soll
        //  pos: Die position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setDrawPosition( Punkt &pos );
        // Setzt die Position, an die der Erste buchstabe gezeichnet werden soll
        //  x: Die x Position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        //  y: Die y Position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setDrawPosition( int x, int y );

        // Gibt den gespeicherten Buchstaben zu einem bestimmten ASCII Zeichen zurück
        //  i: Der ASCII code des Zeichens
        //  return: Ein Zeiger zu dem Buchstaben mit erhöhtem Reference Counter
        __declspec( dllexport ) Buchstabe *getBuchstabe( unsigned char i ) const;
        // Gibt den gespeicherten Buchstaben zu einem bestimmten ASCII Zeichen zurück
        //  i: Der ASCII code des Zeichens
        //  return: Ein Zeiger zu dem Buchstaben ohne erhöhtem Reference Counter
        __declspec( dllexport ) Buchstabe *zBuchstabe( unsigned char i ) const;
        // Prüft, ob zu einem bestimmten ASCII code ein Zeichen vorhanden ist
        //  b: Der zu prüfende ASCII code
        //  return: (true), wenn ein Zeichen zu dem Code gefunden wurde. (false) sonnst
        __declspec( dllexport ) bool hatBuchstabe( unsigned char b ) const;
        // Gibt die Schriftgröße zurück, deren Zeichen in diesem Alphabet gespeichert werden
        __declspec( dllexport ) int getSchriftSize() const;
        // Gibt die Schriftgröße zurück, in der die gespeicherten Zeichen gezeichnet werden
        __declspec( dllexport ) int getDrawSchriftSize() const;
        // Gibt den Abstand in Pixeln zum unteren Ende der darüber ligenden Zeile zurück
        __declspec( dllexport ) int getZeilenAbstand() const;
        // Gibt die skallierte Höhe zurück, die eine gezeichnete Zeile in Pixeln benötigt
        __declspec( dllexport ) int getZeilenHeight() const;
        // gibt die Position zurück, an der der erste Buchstabe gezeichnet wird
        __declspec( dllexport ) const Punkt &getPosition() const;
        // Ermittelt, wie viele Pixel benötigt werden, um einen Bestimmten Text vollständig darzustellen
        //  zTxt: Der Text, von dem die Breite in Pixeln ermitelt werden soll
        __declspec( dllexport ) int getTextBreite( Text *zTxt ) const;
        // Ermittelt, wie viele Pixel benötigt werden, um einen Bestimmten Text vollständig darzustellen
        //  zTxt: Der Text, von dem die Höhe in Pixeln ermitelt werden soll
        __declspec( dllexport ) int getTextHeight( Text *zTxt ) const;
        // Ermittelt das Zeichen im Text, auf das die Maus zeigt
        //  zTxt: Der Text, auf den die Maus Zeigt
        //  mausX: Die X Position der Maus in Pixeln Relativ zur Position des ersten Zeichens
        //  mausY: Die Y Position der Maus in Pixeln Relativ zur Position des ersten Zeichens
        __declspec( dllexport ) int textPos( Text *zTxt, int mausX, int mausY ) const;
        // Fügt Zeilenumbrüche in den Text ein, so dass er bei einer vorgegebenen Breite follständig angezeigt wird
        //  zText: Der text, in den die Zeilenumbrüche eingefügt werden sollen
        //  maxBreite: Die Breite in Pixeln auf der der Text follständig angezeigt werden soll
		//  schriftSize: Die Schriftgröße, in der der Text passend dargestellt werden soll
        __declspec( dllexport ) void textFormatieren( Text *zText, int maxBreite, int schriftSize );
        // Zeichnet einen Bestimmten Text auf ein Bild
        // Nutze (setDrawPosition) und (setDrawSchriftGröße) um die Position und die Größe zu verändern
        //  zText: Der Text, der gezeichnet werden soll
        //  zRObj: Das Bild, auf das gezeichnet werden soll
        //  f: Die Farbe, in der der Text gezeichnet werden soll
        __declspec( dllexport ) void render( Text *zTxt, Bild &zRObj, int f ) const;
        // Zeichnet einen Bestimmten Text mit Cursor und einfärbung auf ein Bild
        // Nutze (setPosition) und (setDrawSchriftGröße) um die Position und die Größe zu verändern
        //  zText: Der Text, der gezeichnet werden soll
        //  zRObj: Das Bild, auf das gezeichnet werden soll
        //  cpos: Die position des Cursors im Text
        //  cf: Die Farbe des Cursors
        //  fbeg: Die Position des Zeichens im Text, wo die Einfärbung beginnen soll. Der Text wird von dort bis zur Cursorposition eingefärbt
        //  ff: Die Hintergrund Farbe des eingefärbten Textes
        //  f: Die Farbe, in der der Text gezeichnet werden soll
        __declspec( dllexport ) void render( Text *zTxt, Bild &zRObj, int cpos, int cf, int fbeg, int ff, int f ) const;

        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Alphabet *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Alphabet *release();
    };

    // Specihert eine Liste von Alphabeten mit verschiedener Schriftgröße.
    // Wird von der Schrift Klasse verwendet, um alle Zeichen der Schriftgröße nach sortiert zu speichern.
    class AlphabetArray
    {
    private:
        AlphabetArray *next;
        Alphabet *This;

    public:
        // Erzeugt eine neue Liste
        __declspec( dllexport ) AlphabetArray();
        // Löscht eine Liste
        __declspec( dllexport ) ~AlphabetArray();

        // Fügt der Liste ein Alphabet hinzu
        // Wenn bereits ein Alphabet mit der selben Schriftgröße existiert, wird das Alphabet nicht hinzugefügt
        //  alphabet: Das Alphabet, welches hinzugefügt werden soll
        //  return: (true), wenn das Alphabet hinzugefügt wurde. (false) sonnst.
        __declspec( dllexport ) bool addAlphabet( Alphabet *alphabet );
        // Löscht ein Alphabet bestimmter Schriftgröße aus der Liste
        //  sg: Die Schriftgröße des Alphabets, welches entfernt werden soll
        //  return: (true), wenn ein Alphabet entfernt wurde. (false) sonnst
        __declspec( dllexport ) bool removeAlphabet( int sg );
        // Setzt die Schriftgröße, in der gezeichnet werden soll
        //  sg: Die Schriftgröße
        __declspec( dllexport ) void setDrawSchriftSize( int sg );
        // Setzt den Zeilenabstand, der zum zeichnen verwendet werden soll
        //  za: Der Zeilenabstand zum unteren Ende der darüber liegenden zeile in Pixeln
        __declspec( dllexport ) void setZeilenAbstand( int za );
        // Setzt den Zeiger auf das nächste Element der Liste auf 0
        __declspec( dllexport ) void setNext0();

        // Gibt ein bestimmtes Alphabet mit erhöhtem Reference Counter zurück
        //  sg: Die Schriftgröße, dessen Alphabet gesucht werden soll
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *getAlphabet( unsigned char sg ) const;
        // Gibt ein bestimmtes Alphabet ohne erhöhtem Reference Counter zurück
        //  sg: Die Schriftgröße, dessen Alphabet gesucht werden soll
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *zAlphabet( unsigned char sg ) const;
        // Gibt ein bestimmtes Alphabet mit erhöhtem Reference Counter zurück
        //  index: Der Index des gesuchten Alphabets in der Reihenfolge wie sie der Liste hinzugefügt wurden
        //  count: Hier sollte 0 übergeben werden. Gibt an auf dem wievielten Element der Liste die Funktion aufgerufen wird.
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *getAlphabetI( int index, int count ) const;
        // Gibt ein bestimmtes Alphabet ohne erhöhtem Reference Counter zurück
        //  index: Der Index des gesuchten Alphabets in der Reihenfolge wie sie der Liste hinzugefügt wurden
        //  count: Hier sollte 0 übergeben werden. Gibt an auf dem wievielten Element der Liste die Funktion aufgerufen wird.
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *zAlphabetI( int index, int count ) const;
        // Gibt einen Zeiger auf das nächste Element der Liste zurück
        __declspec( dllexport ) AlphabetArray *getNext() const;
    };

    // Speichert alle Buchstaben einer Schrift in verschiedenen Schriftgrößen
    class Schrift
    {
    private:
        unsigned char alphabetAnzahl;
        AlphabetArray *alphabet;
        int schriftSize;
        int zeilenAbstand;
        Punkt drawPos;
        Critical cs;
        int ref;

    public:
        // Erzeugt eine leere Schrift
        __declspec( dllexport ) Schrift();
        // Löscht ein Zeichnung
        __declspec( dllexport ) ~Schrift();

        // Bereitet die Schrift auf das zeichnen eines Textes vor.
        // Dies ist notwendig, falls mehrere Threads gleichzeitig die Schrift benutzen
        __declspec( dllexport ) void lock();
        // Beendet den Zeichenforgang der Schrift
        // Muss für jeden aufruf von (lock();) einmal vom selben Thread aufgerufen werden,
        // damit die Schrift wieder von anderen Threads verwendet werden kann
        __declspec( dllexport ) void unlock();
        // Fügt der Schrift ein Alphabet hinzu. Sollte bereits ein Alphabet der selben Schriftgröße existieren, wird das Alphabet nicht hinzugefügt
        //  alphabet: Das Alphabet, welches hinzugefügt werden soll
        //  return: (true), wenn das Alphabet hinzugefügt wurde. (false) sonnst
        __declspec( dllexport ) bool addAlphabet( Alphabet *alphabet );
        // Löscht ein bestimmtes Alphabet aus der Schrift
        //  sg: Die Schriftgröße, deren Alphabet entfernt werden soll
        __declspec( dllexport ) void removeAlphabet( int sg );
        // Setzt die Position, an die der Erste buchstabe gezeichnet werden soll
        //  x: Die x Position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        //  y: Die y Position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setDrawPosition( int x, int y );
        // Setzt die Position, an die der Erste buchstabe gezeichnet werden soll
        //  pos: Die Position relativ zu der Draw Position im zeichen Bild (siehe Bild::setDrawOptions)
        __declspec( dllexport ) void setDrawPosition( Punkt &pos );
        // Setzt die Schriftgröße, in der gezeichnet werden soll. Die Schrift wählt automatisch das passende Alphabet zum Zeichnen
        //  sg: Die Schriftgröße
        __declspec( dllexport ) void setSchriftSize( int sg );
        // Setzt den Zeilenabstand, der zum zeichnen verwendet werden soll
        //  za: Der Zeilenabstand zum unteren Ende der darüber liegenden zeile in Pixeln
        __declspec( dllexport ) void setZeilenAbstand( int za );
        // Fügt Zeilenumbrüche in den Text ein, so dass er bei einer vorgegebenen Breite follständig angezeigt wird
        //  zText: Der text, in den die Zeilenumbrüche eingefügt werden sollen
        //  maxBreite: Die Breite in Pixeln auf der der Text follständig angezeigt werden soll
        //  schriftSize: Die Schriftgröße, die verwendet werden soll
        __declspec( dllexport ) void textFormatieren( Text *zText, int maxBreite, int schriftSize );
        // Zeichnet einen Bestimmten Text auf ein Bild
        // Nutze (setDrawPosition) und (setSchriftGröße) um die Position und die Größe zu verändern
        //  zText: Der Text, der gezeichnet werden soll
        //  zRObj: Das Bild, auf das gezeichnet werden soll
        //  f: Die Farbe, in der der Text gezeichnet werden soll
        __declspec( dllexport ) void renderText( Text *zTxt, Bild &zRObj, int f );
        // Zeichnet einen Bestimmten Text mit Cursor und einfärbung auf ein Bild
        // Nutze (setPosition) und (setDrawSchriftGröße) um die Position und die Größe zu verändern
        //  zText: Der Text, der gezeichnet werden soll
        //  zRObj: Das Bild, auf das gezeichnet werden soll
        //  cpos: Die position des Cursors im Text
        //  cf: Die Farbe des Cursors
        //  fbeg: Die Position des Zeichens im Text, wo die Einfärbung beginnen soll. Der Text wird von dort bis zur Cursorposition eingefärbt
        //  ff: Die Hintergrund Farbe des eingefärbten Textes
        //  f: Die Farbe, in der der Text gezeichnet werden soll
        __declspec( dllexport ) void renderText( Text *zTxt, Bild &zRObj, int cpos, int cf, int fbeg, int ff, int f );
        // Gibt ein bestimmtes Alphabet mit erhöhtem Reference Counter zurück
        //  sg: Die Schriftgröße, dessen Alphabet gesucht werden soll
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *getAlphabet( int sg ) const;
        // Gibt ein bestimmtes Alphabet ohne erhöhtem Reference Counter zurück
        //  sg: Die Schriftgröße, dessen Alphabet gesucht werden soll
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *zAlphabet( int sg ) const;
        // Gibt ein bestimmtes Alphabet mit erhöhtem Reference Counter zurück
        //  index: Der Index des gesuchten Alphabets in der Reihenfolge wie sie der Liste hinzugefügt wurden
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *getAlphabetI( int index ) const;
        // Gibt ein bestimmtes Alphabet ohne erhöhtem Reference Counter zurück
        //  index: Der Index des gesuchten Alphabets in der Reihenfolge wie sie der Liste hinzugefügt wurden
        //  return: (0), fals kein passendes Alphabet gefunden wurde
        __declspec( dllexport ) Alphabet *zAlphabetI( int index ) const;
        // Gibt zurück, wie viele Alphabete (und damit Schriftgrößen) in der Schrift enthalten sind
        __declspec( dllexport ) unsigned char getAlphabetAnzahl() const;
        // Gibt die Schriftgröße zurück, die zum Zeichnen verwendet wird
        __declspec( dllexport ) int getSchriftSize() const;
        // Gibt den Abstand in Pixeln zum unteren Ende der darüber ligenden Zeile zurück
        __declspec( dllexport ) int getZeilenabstand() const;
        // Gibt die x Koordinate des ersten Zeichens, dass gezeichnet wird, in Pixeln zurück
        __declspec( dllexport ) int getDrawX() const;
        // Gibt die y Koordinate des ersten Zeichens, dass gezeichnet wird, in Pixeln zurück
        __declspec( dllexport ) int getDrawY() const;
        // Gibt die Position des ersten Zeichens, dass gezeichnet wird, in Pixeln zurück
        __declspec( dllexport ) const Punkt &getDrawPosition() const;
        // Ermittelt, wie viele Pixel benötigt werden, um einen Bestimmten Text vollständig darzustellen
        //  zTxt: Der Text, von dem die Breite in Pixeln ermitelt werden soll
        __declspec( dllexport ) int getTextBreite( Text *zTxt ) const;
        // Ermittelt, wie viele Pixel benötigt werden, um einen Bestimmten Text vollständig darzustellen
        //  zTxt: Der Text, von dem die Höhe in Pixeln ermitelt werden soll
        __declspec( dllexport ) int getTextHeight( Text *zTxt ) const;
        // Ermittelt das Zeichen im Text, auf das die Maus zeigt
        //  zTxt: Der Text, auf den die Maus Zeigt
        //  mausX: Die X Position der Maus in Pixeln Relativ zur Position des ersten Zeichens
        //  mausY: Die Y Position der Maus in Pixeln Relativ zur Position des ersten Zeichens
        __declspec( dllexport ) int textPos( Text *zTxt, int mausX, int mausY ) const;

        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Schrift *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Schrift *release();
    };
}

#endif