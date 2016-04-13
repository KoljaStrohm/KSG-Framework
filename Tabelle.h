#ifndef Tabelle_H
#define Tabelle_H

#include "Zeichnung.h"
#include "Array.h"

namespace Framework
{
	class LRahmen; // Rahmen.h
	class AlphaFeld; // AlphaFeld.h
	class VScrollBar; // Scroll.h
	class HScrollBar; // Scroll.h
	class Text; // Text.h
	class ObjTabelle; // aus dieser Datei

    // Verwaltet eine Tabelle aus Zeichnungen
	class ObjTabelle : public ZeichnungHintergrund
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 SpaltenBreiteÄnderbar = 0x00001000; // Wenn dieser Flag gesetzt wird, so kann der Benutzer die Spalten mit der Maus vergrößern und verkleinern
            const static __int64 ZeilenHöheÄnderbar = 0x00002000; // Wenn dieser Flag gesetzt wird, so kann der Benutzer die Spalten mit der Maus vergrößern und verkleinern
            const static __int64 SpaltenBreiteMin = 0x00004000; // Wenn dieser Flag gesetzt wird, so kann der Nutzer trotz des Flags SpaltenBreiteÄnderbar die Spaltenbreite nicht kleiner machen als eine bestimmte Grenze
            const static __int64 SpaltenBreiteMax = 0x00008000; // Wenn dieser Flag gesetzt wird, so kann der Nutzer trotz des Flags SpaltenBreiteÄnderbar die Spaltenbreite nicht größer machen als eine bestimmte Grenze
            const static __int64 ZeilenHöheMin = 0x00010000; // Wenn dieser Flag gesetzt wird, so kann der Nutzer trotz des Flags ZeilenHöheÄnderbar die Zeilenhöhe nicht kleiner machen als eine bestimmte Grenze
            const static __int64 ZeilenHöheMax = 0x00020000; // Wenn dieser Flag gesetzt wird, so kann der Nutzer trotz des Flags ZeilenHöheÄnderbar die Zeilenhöhe nicht größer machen als eine bestimmte Grenze
            const static __int64 SpaltenBeweglich = 0x00040000; // Wenn dieser Flag gesetzt wird, so kann der Nutzer die Reihenfolge der Spalten bestimmen, in dem er sie per Drag and Drop verschiebt
            const static __int64 AuswahlRahmen = 0x0080000; // Wenn dieser Flag gesetzt ist, so bekommt das Feld, dass der Benutzer durch den Flag Erlaubt auswählen kann einen anderen Rahmen
            const static __int64 AuswahlBuffer = 0x00100000; // Wenn dieser Flag gesetzt ist, so bekommt das Feld, dass der Benutzer durch den Flag Erlaubt auswählen kann ein anderes AlphaFeld
            const static __int64 AuswahlMultistyled = 0x00200000; // Wenn dieser Flag gesetzt ist, so kann jedes Feld andere Rahmen und AlphaFelder beim Auswählen haben.
            const static __int64 Raster = 0x00400000; // Wenn dieser Flag gesetzt ist, so werden zwischen den Feldern Linien gezeichnet

            const static __int64 beweglich = SpaltenBreiteÄnderbar | ZeilenHöheÄnderbar | SpaltenBeweglich; // Vereint die Flags: SpaltenBreiteÄnderbar, ZeilenHöheÄnderbar, SpaltenBeweglich
            const static __int64 min_max = SpaltenBreiteMax | SpaltenBreiteMin | ZeilenHöheMax | ZeilenHöheMax; // Vereint die Flags: SpaltenBreiteMax, SpaltenBreiteMin, ZeilenHöheMax, ZeilenHöheMax
            const static __int64 scroll = VScroll | HScroll; // Vereint die Flags: VScroll, HScroll
            const static __int64 normal = Rahmen | Erlaubt | Sichtbar | AuswahlBuffer | AuswahlRahmen | Raster; // Vereint die Flags: Rahmen, Erlaubt, Sichtbar, SpaltenBeweglich, AuswahlBuffer, AuswahlRahmen, Raster
        };
	private:
		RCArray< Array< Zeichnung* > > *zZeichnungs;
		RCArray< Text > *spaltenNamen;
		RCArray< Text > *zeilenNamen;
		Array< int > *spaltenBreite;
		Array< int > *zeilenHöhe;
		Array< int > *minSpaltenBreite;
		Array< int > *maxSpaltenBreite;
		Array< int > *minZeilenHöhe;
		Array< int > *maxZeilenHöhe;
		int spaltenAnzahl, zeilenAnzahl;
		int klickSpalte;
		double mSpalte, mZeile;
		int mx, my;
		Punkt selected;
		int rasterFarbe;
		int rasterBreite;
		LRahmen *aRam;
		AlphaFeld *aAf;
		RCArray< RCArray< LRahmen > > *msaRam;
		RCArray< RCArray< AlphaFeld > > *msaAf;
		RCArray< Array< __int64 > > *styles;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) ObjTabelle();
		// Destruktor 
		__declspec( dllexport ) ~ObjTabelle();
		// Fügt der Tabelle eine Spalte hinzu
        //  name: Der Name der Spalte
		__declspec( dllexport ) void addSpalte( const char *name );
        // Fügt der Tabelle eine Spalte hinzu
        //  name: Der Name der Spalte
		__declspec( dllexport ) void addSpalte( Text *name );
        // Fügt der Tabelle an einer bestimmten Position eine Spalte hinzu
        //  sNum: Der Index der neuen Spalte
        //  name: Der Name der neuen Spalte
		__declspec( dllexport ) void addSpalte( int sNum, const char *name );
        // Fügt der Tabelle an einer bestimmten Position eine Spalte hinzu
        //  sNum: Der Index der neuen Spalte
        //  name: Der Name der neuen Spalte
		__declspec( dllexport ) void addSpalte( int sNum, Text *name );
        // Fügt der Tabelle eine Zeile hinzu
        //  name: Der Name der Zeile
		__declspec( dllexport ) void addZeile( const char *name );
        // Fügt der Tabelle eine Zeile hinzu
        //  name: Der Name der Zeile
		__declspec( dllexport ) void addZeile( Text *name );
        // Fügt der Tabelle an einer bestimmten Position eine Zeile hinzu
        //  zNum: Der Index der neuen Zeile
        //  name: Der Name der neuen Zeile
		__declspec( dllexport ) void addZeile( int zNum, const char *name );
        // Fügt der Tabelle an einer bestimmten Position eine Zeile hinzu
        //  sNum: Der Index der neuen Zeile
        //  name: Der Name der neuen Zeile
		__declspec( dllexport ) void addZeile( int zNum, Text *name );
        // Entfernt eine Spalte
        //  sNum: Der Index der Spalte
		__declspec( dllexport ) void removeSpalte( int sNum );
        // Entfernt eine Spalte
        //  name: Der Name der Spalte
		__declspec( dllexport ) void removeSpalte( const char *name );
        // Entfernt eine Spalte
        //  name: Der Name der Spalte
		__declspec( dllexport ) void removeSpalte( Text *name );
        // Entfernt eine Zeile
        //  zNum: Der Index der Zeile
		__declspec( dllexport ) void removeZeile( int zNum );
        // Entfernt eine Zeile
        //  name: Der Name der Zeile
		__declspec( dllexport ) void removeZeile( const char *name );
        // Entfernt eine Zeile
        //  name: Der Name der Zeile
		__declspec( dllexport ) void removeZeile( Text *name );
        // Setzt den Index einer Spalte
        //  name: Der Name der Spalte
        //  pos: Der neue Index der Spalte
		__declspec( dllexport ) void setSpaltePosition( const char *name, int pos );
        // Setzt den Index einer Spalte
        //  name: Der Name der Spalte
        //  pos: Der neue Index der Spalte
		__declspec( dllexport ) void setSpaltePosition( Text *name, int pos );
        // Setzt den Index einer Spalte
        //  sNum: Der alte Index der Spalte
        //  pos: Der neue Index der Spalte
		__declspec( dllexport ) void setSpaltePosition( int sNum, int pos );
        // Setzt den Index einer Zeile
        //  name: Der Name der Zeile
        //  pos: Der neue Index der Zeile
		__declspec( dllexport ) void setZeilePosition( const char *name, int pos );
        // Setzt den Index einer Zeile
        //  name: Der Name der Zeile
        //  pos: Der neue Index der Zeile
		__declspec( dllexport ) void setZeilePosition( Text *name, int pos );
        // Setzt den Index einer Zeile
        //  zNum: Der alte Index der Zeile
        //  pos: Der neue Index der Zeile
		__declspec( dllexport ) void setZeilePosition( int zNum, int pos );
        // Setzt ein Zeichnung, welches in einem Bestimmten Feld sein soll
        // Wenn bereits ein Zeichnung in dem Feld ist, wird es überschrieben.
        //  sNum: Der Index der Spalte, in der das Zeichnung stehen soll
        //  zNum: Der Index der Zeile, in der das Zeichnung stehen soll
        //  zObj: Das Zeichnung welches in dem Feld sein soll
		__declspec( dllexport ) void setZeichnungZ( int sNum, int zNum, Zeichnung *zObj );
        // Setzt ein Zeichnung, welches in einem Bestimmten Feld sein soll
        // Wenn bereits ein Zeichnung in dem Feld ist, wird es überschrieben.
        //  spaltenName: Der Name der Spalte, in der das Zeichnung stehen soll
        //  zeilenName: Der Name der Zeile, in der das Zeichnung stehen soll
        //  zZeichnung: Das Zeichnung welches in dem Feld sein soll
		__declspec( dllexport ) void setZeichnungZ( const char *spaltenName, const char *zeilenName, Zeichnung *zZeichnung );
        // Setzt ein Zeichnung, welches in einem Bestimmten Feld sein soll
        // Wenn bereits ein Zeichnung in dem Feld ist, wird es überschrieben.
        //  spaltenName: Der Name der Spalte, in der das Zeichnung stehen soll
        //  zeilenName: Der Name der Zeile, in der das Zeichnung stehen soll
        //  zZeichnung: Das Zeichnung welches in dem Feld sein soll
		__declspec( dllexport ) void setZeichnungZ( Text *spaltenName, Text *zeilenName, Zeichnung *zZeichnung );
        // Setzt die Spalten Breite
        //  sNum: Der Index der Spalte
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setSpaltenBreite( int sNum, int br );
        // Setzt die Spalten Breite
        //  name: Der Name der Spalte
        //  br: Die Breite in Pixeln
		__declspec( dllexport ) void setSpaltenBreite( const char *name, int br );
        // Setzt die Spalten Breite
        //  name: Der Name der Spalte
        //  br: Die Breite in Pixeln
		__declspec( dllexport ) void setSpaltenBreite( Text *name, int br );
        // Setzt die Zeilen Höhe
        //  zNum: Der Index der Zeile
        //  hö: Die Höhe in Pixeln
		__declspec( dllexport ) void setZeilenHöhe( int zNum, int hö );
        // Setzt die Zeilen Höhe
        //  name: Der Index der Zeile
        //  hö: Die Höhe in Pixeln
		__declspec( dllexport ) void setZeilenHöhe( const char *name, int hö );
        // Setzt die Zeilen Höhe
        //  name: Der Index der Zeile
        //  hö: Die Höhe in Pixeln
		__declspec( dllexport ) void setZeilenHöhe( Text *name, int hö );
        // Setzt die minimale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMin)
        //  sNum: Der Index der Spalte
        //  minBr: Die minimale Breite in Pixeln
		__declspec( dllexport ) void setMinSpaltenBreite( int sNum, int minBr );
        // Setzt die minimale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMin)
        //  name: Der Name der Spalte
        //  minBr: Die minimale Breite in Pixeln
		__declspec( dllexport ) void setMinSpaltenBreite( const char *name, int minBr );
        // Setzt die minimale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMin)
        //  name: Der Name der Spalte
        //  minBr: Die minimale Breite in Pixeln
		__declspec( dllexport ) void setMinSpaltenBreite( Text *name, int minBr );
        // Setzt die maximale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMax)
        //  sNum: Der Index der Spalte
        //  maxBr: Die maximale Breite in Pixeln
		__declspec( dllexport ) void setMaxSpaltenBreite( int sNum, int maxBr );
        // Setzt die maximale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMax)
        //  name: Der Name der Spalte
        //  maxBr: Die maximale Breite in Pixeln
		__declspec( dllexport ) void setMaxSpaltenBreite( const char *name, int maxBr );
        // Setzt die maximale Spalten Breite (benötigt Flag: SpaltenBreiteÄnderbar, SpaltenBreiteMax)
        //  name: Der Name der Spalte
        //  maxBr: Die maximale Breite in Pixeln
		__declspec( dllexport ) void setMaxSpaltenBreite( Text *name, int maxBr );
        // Setzt die minimale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMin)
        //  zNum: Der Index der Zeile
        //  minHö: Die minimale Höhe in Pixeln
		__declspec( dllexport ) void setMinZeilenHöhe( int zNum, int minHö );
        // Setzt die minimale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMin)
        //  name: Der Name der Zeile
        //  minHö: Die minimale Höhe in Pixeln
		__declspec( dllexport ) void setMinZeilenHöhe( const char *name, int minHö );
        // Setzt die minimale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMin)
        //  name: Der Name der Zeile
        //  minHö: Die minimale Höhe in Pixeln
		__declspec( dllexport ) void setMinZeilenHöhe( Text *name, int minHö );
        // Setzt die maximale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMax)
        //  zNum: Der Index der Zeile
        //  maxHö: Die maximale Höhe in Pixeln
		__declspec( dllexport ) void setMaxZeilenHöhe( int zNum, int maxHö );
        // Setzt die maximale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMax)
        //  name: Der Name der Zeile
        //  maxHö: Die maximale Höhe in Pixeln
		__declspec( dllexport ) void setMaxZeilenHöhe( const char *name, int maxHö );
        // Setzt die maximale Zeilen Höhe (benötigt Flag: ZeilenHöheÄnderbar, ZeienHöheMax)
        //  name: Der Name der Zeile
        //  maxHö: Die maximale Höhe in Pixeln
		__declspec( dllexport ) void setMaxZeilenHöhe( Text *name, int maxHö );
        // Legt fest, welches Feld ausgewählt ist (benötigt Flag: Erlaubt)
        //  sNum: Der Index der Spalte des Feldes
        //  zNum: Der Index der Zeile des Feldes
		__declspec( dllexport ) void setAuswahl( int sNum, int zNum );
        // Legt fest, welches Feld ausgewählt ist (benötigt Flag: Erlaubt)
        //  spaltenName: Der Name der Spalte des Feldes
        //  zeilenName: Der Name der Zeile des Feldes
		__declspec( dllexport ) void setAuswahl( const char *spaltenName, const char *zeilenName );
        // Legt fest, welches Feld ausgewählt ist (benötigt Flag: Erlaubt)
        //  spaltenName: Der Name der Spalte des Feldes
        //  zeilenName: Der Name der Zeile des Feldes
		__declspec( dllexport ) void setAuswahl( Text *spaltenName, Text *zeilenName );
		__declspec( dllexport ) void setRasterFarbe( int f ); // settzt die Farbe des Rasters
		__declspec( dllexport ) void setRasterBreite( int br ); // setzt die Breite des Rasters
		__declspec( dllexport ) void setARahmenZ( LRahmen *ram ); // setzt den auswahl Rahmen
		__declspec( dllexport ) void setARFarbe( int f ); // setzt die auswahl Rahmen Farbe
		__declspec( dllexport ) void setARBreite( int br ); // setzt die auswahl Rahmen Breite
		__declspec( dllexport ) void setAAlphaFeldZ( AlphaFeld *af ); // setzt das auswahl AlphaFeld
		__declspec( dllexport ) void setAAfFarbe( int f ); // setzt die Farbe des auswahl AlphaFeldes
		__declspec( dllexport ) void setAAfStärke( int st ); // setzt die Stärke des auswahl AlphaFeldes
		__declspec( dllexport ) void setARahmenZ( int sNum, int zNum, LRahmen *ram ); // setzt den auswahl Rahmen
		__declspec( dllexport ) void setARahmenZ( const char *spaltenName, const char *zeilenName, LRahmen *ram );
		__declspec( dllexport ) void setARahmenZ( Text *spaltenName, Text *zeilenName, LRahmen *ram );
		__declspec( dllexport ) void setARFarbe( int sNum, int zNum, int f ); // setzt die auswahl Rahmen Farbe
		__declspec( dllexport ) void setARFarbe( const char *spaltenName, const char *zeilenName, int f );
		__declspec( dllexport ) void setARFarbe( Text *spaltenName, Text *zeilenName, int f );
		__declspec( dllexport ) void setARBreite( int sNum, int zNum, int br ); // setzt die auswahl Rahmen Breite
		__declspec( dllexport ) void setARBreite( const char *spaltenName, const char *zeilenName, int br );
		__declspec( dllexport ) void setARBreite( Text *spaltenName, Text *zeilenName, int br );
		__declspec( dllexport ) void setAAlphaFeldZ( int sNum, int zNum, AlphaFeld *af ); // setzt das auswahl AlphaFeld
		__declspec( dllexport ) void setAAlphaFeldZ( const char *spaltenName, const char *zeilenName, AlphaFeld *af );
		__declspec( dllexport ) void setAAlphaFeldZ( Text *spaltenName, Text *zeilenName, AlphaFeld *af );
		__declspec( dllexport ) void setAAfFarbe( int sNum, int zNum, int f ); // setzt die Farbe des auswahl AlphaFeldes
		__declspec( dllexport ) void setAAfFarbe( const char *spaltenName, const char *zeilenName, int f );
		__declspec( dllexport ) void setAAfFarbe( Text *spaltenName, Text *zeilenName, int f );
		__declspec( dllexport ) void setAAfStärke( int sNum, int zNum, int st ); // setzt die Stärke des auswahl AlphaFeldes
		__declspec( dllexport ) void setAAfStärke( const char *spaltenName, const char *zeilenName, int st );
		__declspec( dllexport ) void setAAfStärke( Text *spaltenName, Text *zeilenName, int st );
		__declspec( dllexport ) void addMsStyle( int sNum, int zNum, __int64 style ); // setzt den Style wenn Multistyled
		__declspec( dllexport ) void addMsStyle( const char *spaltenName, const char *zeilenName, __int64 style );
		__declspec( dllexport ) void addMsStyle( Text *spaltenName, Text *zeilenName, __int64 style );
		__declspec( dllexport ) void setMsStyle( int sNum, int zNum, __int64 style );
		__declspec( dllexport ) void setMsStyle( const char *spaltenName, const char *zeilenName, __int64 style );
		__declspec( dllexport ) void setMsStyle( Text *spaltenName, Text *zeilenName, __int64 style );
		__declspec( dllexport ) void setMsStyle( int sNum, int zNum, __int64 style, bool add_remove );
		__declspec( dllexport ) void setMsStyle( const char *spaltenName, const char *zeilenName, __int64 style, bool add_remove );
		__declspec( dllexport ) void setMsStyle( Text *spaltenName, Text *zeilenName, __int64 style, bool add_remove );
		__declspec( dllexport ) void löscheMsStyle( int sNum, int zNum, __int64 style );
		__declspec( dllexport ) void löscheMsStyle( const char *spaltenName, const char *zeilenName, __int64 style );
		__declspec( dllexport ) void löscheMsStyle( Text *spaltenName, Text *zeilenName, __int64 style );
		__declspec( dllexport ) bool tick( double tickVal ) override; // tick Message
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override; // verarbeitet Nachrichten
		__declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
		// constant 
		__declspec( dllexport ) int getSpaltenAnzahl() const; // gibt die Anzahl der Spalten zurück
		__declspec( dllexport ) int getZeilenAnzahl() const; // gibt die Anzahl der Zeilen zurück
		__declspec( dllexport ) int getSpaltenNummer( const char *name ) const; // gibt die Nummer der Spalte mit dem Namen name zurück
		__declspec( dllexport ) int getSpaltenNummer( Text *name ) const;
		__declspec( dllexport ) Text *getSpaltenName( int num ) const; // gibt den Namen der Spalte mit Nummer num zurück
		__declspec( dllexport ) Text *zSpaltenName( int num ) const;
		__declspec( dllexport ) int getZeilenNummer( const char *name ) const; // gibt die Nummer der Zeile mit dem Namen name zurück
		__declspec( dllexport ) int getZeilenNummer( Text *name ) const;
		__declspec( dllexport ) Text *getZeilenName( int num ) const; // gibt den Namen der Zeile mit Nummer num zurück
		__declspec( dllexport ) Text *zZeilenName( int num ) const;
		__declspec( dllexport ) Punkt getZeichnungPosition( Zeichnung *zObj ) const; // gibt die Position eines Zeichnungs zurück
		__declspec( dllexport ) Zeichnung *zZeichnung( int sNum, int zNum ) const; // gibt das Zeichnung auf der Position zurück
		__declspec( dllexport ) Zeichnung *zZeichnung( const char *spaltenName, const char *zeilenName ) const;
		__declspec( dllexport ) Zeichnung *zZeichnung( Text *spaltenName, Text *zeilenName ) const;
		__declspec( dllexport ) int getSpaltenBreite( int num ) const; // gibt die Breite der Spalte zurück
		__declspec( dllexport ) int getSpaltenBreite( const char *name ) const;
		__declspec( dllexport ) int getSpaltenBreite( Text *name ) const;
		__declspec( dllexport ) int getZeilenHöhe( int num ) const; // gibt die Höhe der Zeile zurück
		__declspec( dllexport ) int getZeilenHöhe( const char *name ) const;
		__declspec( dllexport ) int getZeilenHöhe( Text *name ) const;
		__declspec( dllexport ) int getMinSpaltenBreite( int num ) const; // gibt die minimale Spaltengröße zurück
		__declspec( dllexport ) int getMinSpaltenBreite( const char *name ) const;
		__declspec( dllexport ) int getMinSpaltenBreite( Text *name ) const;
		__declspec( dllexport ) int getMaxSpaltenBreite( int num ) const; // gibt die maximale Spaltengröße zurück
		__declspec( dllexport ) int getMaxSpaltenBreite( const char *name ) const;
		__declspec( dllexport ) int getMaxSpaltenBreite( Text *name ) const;
		__declspec( dllexport ) int getMinZeilenHöhe( int num ) const; // gibt die minimale Zeilenhöhe zurück
		__declspec( dllexport ) int getMinZeilenHöhe( const char *name ) const;
		__declspec( dllexport ) int getMinZeilenHöhe( Text *name ) const;
		__declspec( dllexport ) int getMaxZeilenHöhe( int num ) const; // gibt die maximale Zeilenhöhe zurück
		__declspec( dllexport ) int getMaxZeilenHöhe( const char *name ) const;
		__declspec( dllexport ) int getMaxZeilenHöhe( Text *name ) const;
		__declspec( dllexport ) double getMausSpalte( int mx ) const; // ermittelt die Spalte unter der Maus
		__declspec( dllexport ) Text *getMausSpaltenName( int mx ) const;
		__declspec( dllexport ) Text *zMausSpaltenName( int mx ) const;
		__declspec( dllexport ) double getMausZeile( int my ) const; // ermittelt die Zeile unter der Maus
		__declspec( dllexport ) Text *getMausZeilenName( int my ) const;
		__declspec( dllexport ) Text *zMausZeilenName( int my ) const;
		__declspec( dllexport ) const Punkt &getAuswahlPosition() const; // gibt die Auswahl Position zurück
		__declspec( dllexport ) int getRasterFarbe() const; // gibt die Farbe des Rasters zurück
		__declspec( dllexport ) int getRasterBreite() const; // gibt die Breite des Rasters zurück
		__declspec( dllexport ) LRahmen *getARahmen() const; // gibt den auswahl Rahmen zurück
		__declspec( dllexport ) LRahmen *zARahmen() const;
		__declspec( dllexport ) AlphaFeld *getAAlphaFeld() const; // gibt das auswahl AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zAAlphaFeld() const;
		__declspec( dllexport ) LRahmen *getARahmen( int sNum, int zNum ) const; // gibt den auswahl Rahmen zurück
		__declspec( dllexport ) LRahmen *zARahmen( int sNum, int zNum ) const;
		__declspec( dllexport ) AlphaFeld *getAAlphaFeld( int sNum, int zNum ) const; // gibt das auswahl AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zAAlphaFeld( int sNum, int zNum ) const;
		__declspec( dllexport ) LRahmen *getARahmen( const char *spaltenName, const char *zeilenName ) const; // gibt den auswahl Rahmen zurück
		__declspec( dllexport ) LRahmen *zARahmen( const char *spaltenName, const char *zeilenName ) const;
		__declspec( dllexport ) AlphaFeld *getAAlphaFeld( const char *spaltenName, const char *zeilenName ) const; // gibt das auswahl AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zAAlphaFeld( const char *spaltenName, const char *zeilenName ) const;
		__declspec( dllexport ) LRahmen *getARahmen( Text *spaltenName, Text *zeilenName ) const; // gibt den auswahl Rahmen zurück
		__declspec( dllexport ) LRahmen *zARahmen( Text *spaltenName, Text *zeilenName ) const;
		__declspec( dllexport ) AlphaFeld *getAAlphaFeld( Text *spaltenName, Text *zeilenName ) const; // gibt das auswahl AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zAAlphaFeld( Text *spaltenName, Text *zeilenName ) const;
		__declspec( dllexport ) inline bool hatMsStyle( int sNum, int zNum, __int64 style ) const; // prüft, ob style vorhanden ist
		__declspec( dllexport ) inline bool hatMsStyleNicht( int sNum, int zNum, __int64 style ) const; // prüft, ob style nicht vorhanden ist
		__declspec( dllexport ) inline bool hatMsStyle( const char *spaltenName, const char *zeilenName, __int64 style ) const; // prüft, ob style vorhanden ist
		__declspec( dllexport ) inline bool hatMsStyleNicht( const char *spaltenName, const char *zeilenName, __int64 style ) const; // prüft, ob style nicht vorhanden ist
		__declspec( dllexport ) inline bool hatMsStyle( Text *spaltenName, Text *zeilenName, __int64 style ) const; // prüft, ob style vorhanden ist
		__declspec( dllexport ) inline bool hatMsStyleNicht( Text *spaltenName, Text *zeilenName, __int64 style ) const; // prüft, ob style nicht vorhanden ist
		__declspec( dllexport ) Zeichnung *dublizieren() const override; // Erzeugt eine Kopie des Zeichnungs
		// Reference Counting
		__declspec( dllexport ) ObjTabelle *getThis();
		__declspec( dllexport ) ObjTabelle *release();
	};
}
#endif