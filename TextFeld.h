#ifndef TextFeld_H
#define TextFeld_H

#include "Zeichnung.h"

namespace Framework
{
	class Schrift; // Schrift.h
	class Text; // Text.h
	class AlphaFeld; // AlphaFeld.h
	class LRahmen; // Rahmen.h
	class TextFeld; // aus dieser Datei
	class VScrollBar; // Scroll.h
    class HScrollBar; // Scroll.h

    // Verwaltet ein Textfeld
	class TextFeld : public ZeichnungHintergrund
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Mehrzeilig = 0x001000; // Wenn dieser Flag nicht gesetzt wird, werden alle Zeilenumbrüche automatisch aus dem Text entfernt
            const static __int64 HCenter = 0x002000; // Wenn dieser Flag gesetzt wird, wird der Text genau in der horizontaen Mitte des Feldes plaziert
            const static __int64 VCenter = 0x004000; // Wenn dieser Flag gesetzt wird, wird der Text genau in der vertikalen Mitte des Feldes plaziert
            const static __int64 Mehrfarbig = 0x008000; // Wenn dieser Flag gesetzt wird, kann der Text mehrfarbig sein. Die Textfarbe wird im Text selbst gesetzt mit "\r0xAARRGGBB"

            const static __int64 Center = HCenter | VCenter; // Vereint die Flags HCenter und VCenter
            const static __int64 TextFeld = Sichtbar | Erlaubt | Rahmen | Buffered | VCenter; // Vereint die Flags Sichtbar, Erlaubt, Rahmen, Buffered, VCenter
            const static __int64 Text = Sichtbar | Mehrfarbig | Mehrzeilig; // Vereint die Flags Sichtbar, Mehrfarbig, Mehrzeilig
            const static __int64 TextGebiet = Sichtbar | Erlaubt | Rahmen | Hintergrund | Mehrfarbig | Mehrzeilig | VScroll; // Vereint die Flags Sichtbar, Erlaubt, Rahmen, Hintergrund, Mehrzeilig, Mehrfarbig, VScroll
            const static __int64 Scroll = VScroll | HScroll; // Vereint die Flags VScroll und HScroll
        };
	private:
		unsigned char schriftGröße;
		Schrift *schrift;
		Text *text;
		int sF;
		unsigned char showChar;
		int begf, cpos;
		double tickVal;
		bool mausKlick;
    protected:
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) TextFeld();
		// Destruktor 
		__declspec( dllexport ) ~TextFeld();
		// setzt einen Zeiger auf den Text im Textfeld
        //  txt: Der Zeiger auf den Text
		__declspec( dllexport ) void setTextZ( Text *txt );
        // setzt den Text des Textfeldes
        //  txt: der Text
        __declspec( dllexport ) void setText( Text *txt );
        // setzt den Text des Textfeldes
        //  txt: der Text
        __declspec( dllexport ) void setText( const char *txt );
        // Fügt eine Zeile an den Text an
        //  zeile: Die neue Zeile
		__declspec( dllexport ) void addZeile( const char *zeile );
        // Setzt den ausgewählten textabschnitt fest (benötigt Flag zum Zeichnen: Erlaubt, Fokus)
        //  pos1: Die Cursorposition im Text
        //  pos2: Die Position im Text, bis zu der der Text eingefärbt werden soll
		__declspec( dllexport ) void setAuswahl( int pos1, int pos2 );
        // Setzt den ausgewählten textabschnitt fest (benötigt Flag zum Zeichnen: Erlaubt, Fokus)
        //  auswahl: Ein punkt mit x als Cursorposition und y als der Position, bis zu der der Text eingefärbt werden soll
        __declspec( dllexport ) void setAuswahl( Punkt &auswahl );
        // Setzt einen Zeiger zur Schrift
        //  schrift: Die Schrift, die zum Textzeichnen verwendet werden soll.
		__declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        // Setzt die Schriftgröße (Standart: 12)
        //  gr: Die Schriftgröße, die zum Textzeichnen verwendet werden soll
		__declspec( dllexport ) void setSchriftGröße( unsigned char gr );
        // Setzt die Schrift Farbe
        //  fc: Die Farbe, die zum Textzeichnen verwendet werden soll
		__declspec( dllexport ) void setSchriftFarbe( int fc );
        // Legt einen Buchstaben fest, der zum zeichnen verwendet werden soll, unabhängig vom Text des Textfeldes (benötigt Flag zum Zeichnen: Rahmen)
        //  c: Der Buchstabe, der gezeichnet werden soll
        // Beispiel: setShowChar( '*' ); Bei Passwort Textfeldern
		__declspec( dllexport ) void setSchowChar( unsigned char c );
        // Scrollt zu einer bestimmten Zeile (benötigt Flag zum Zeichnen: VScroll)
        //  zeile: Der Index der Zeile, die als oberste Zeile zu sehen sein soll
		__declspec( dllexport ) void setVScrollZuZeile( int zeile );
        // Scrollt zu einer bestimmten Position im Text. Ohne den Flag Erlaubt wrd immer ganz nach unten gescrollt. (benötigt Flag zum Zeichnen: VScroll)
        //  pos: Der Index des Zeichens, zu dem gescrollt werden soll. Standartmäßig wird zur Cursorposition gescrollt
		__declspec( dllexport ) void updateVScroll( int pos = -1 );
        // Scrollt zu einer bestimmten Position im Text. Benötigt den Flag Erlaubt. (benötigt Flag zum Zeichnen: HScroll)
        //  pos: Der Index des Zeichens, zu dem gescrollt werden soll. Standartmäßig wird zur Cursorposition gescrollt
		__declspec( dllexport ) void updateHScroll( int pos = -1 );
        // Aktualisiert das Objekt. Wird vom Framework aufgerufen
        //  tickVal: Die Zeit in sekunden, die seit dem lezten Aufruf dieser Funktion vergangen ist
        //  return: 1, wenn sich etwas verändert hat und das Bild neu gezeichnet werden muss. 0 sonst
		__declspec( dllexport ) virtual bool tick( double tickval ) override;
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
		__declspec( dllexport ) virtual void doMausEreignis( MausEreignis &me ) override;
        // Verarbeitet Tastatur Nachrichten
        //  me: Das Ereignis, was durch die Tastatureingabe ausgelößt wurde
		__declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
        // Zeichnet das Objekt nach zRObj, falls er sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
		__declspec( dllexport ) virtual void render( Bild &zRObj ) override;
		// Gibt den Text aus dem Textfeld zurück
		__declspec( dllexport ) Text *getText() const;
        // Gibt den Text aus dem Textfeld ohne erhöhten Reference Counter zurück
		__declspec( dllexport ) Text *zText() const;
        // Gibt die Schrift zurück.
        //  return: 0, falls die Schrift nicht gesetzt wurde
		__declspec( dllexport ) Schrift *getSchrift() const;
        // Gibt die Schrift ohne erhöhten Reference Counter zurük
        //  return: 0, falls die Schrift nicht gesetzt wurde
		__declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt die Schriftgröße zurück
		__declspec( dllexport ) unsigned char getSchriftGröße() const;
        // Gibt die Schriftfarbe im A8R8G8B8 Format zurück
		__declspec( dllexport ) int getSchriftFarbe() const;
        // Gibt den Anzeigebuchstabe zurück
        __declspec( dllexport ) unsigned char getShowChar() const;
        // Gibt die Cursorposition zurück
        __declspec( dllexport ) int getCursorPos() const;
        // Gibt den Index des Buchstaben zurück, con dem an der Text bis zur Cursorposition eingefärbt ist
        __declspec( dllexport ) int getFärbungPos() const;
        // Erzeugt eine Komplette Kopie des Textfeldes, welches ohne auswirkungen verändert werden kann
		__declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
		__declspec( dllexport ) virtual TextFeld *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
		__declspec( dllexport ) virtual TextFeld *release();
	};
}
#endif