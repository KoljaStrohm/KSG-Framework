#ifndef Liste_H
#define Liste_H

#include "Zeichnung.h"
#include "Array.h"

namespace Framework
{
	class LRahmen; // Rahmen.h
	class AlphaFeld; // AlphaFeld.h
	class Bild; // Bild.h
	class Text; // Text.h
	class TextFeld; // TextFeld.h
	class VScrollBar; // Scroll.h
	struct TastaturEreignis; // TastaturEreignis.h
	struct MausEreignis; // MausEreignis.h
    class Schrift; // Schrift.h
	class AuswahlListe; // aus dieser datei

	class AuswahlListe : public ZeichnungHintergrund
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 FeldRahmen = 0x0001000;
            const static __int64 FeldHintergrund = 0x0002000;
            const static __int64 FeldHBild = 0x0004000;
            const static __int64 FeldHAlpha = 0x0008000;
            const static __int64 FeldBuffer = 0x0010000;
            const static __int64 AuswahlHintergrund = 0x0020000;
            const static __int64 AuswahlHBild = 0x0040000;
            const static __int64 AuswahlHAlpha = 0x0080000;
            const static __int64 AuswahlBuffer = 0x0100000;
            const static __int64 AuswahlRahmen = 0x0200000;
            const static __int64 MultiStyled = 0x0400000;
            const static __int64 MultiSelect = 0x0800000;
            const static __int64 Ausgewählt = 0x1000000;

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | FeldHAlpha | FeldHintergrund | FeldRahmen | AuswahlBuffer | AuswahlRahmen;
        };
	private:
		RCArray< TextFeld > *tfListe;
		int einträge, auswahl;
		int ahFarbe;
		Bild *ahBild;
		AlphaFeld *aBuffer;
		LRahmen *aRahmen;
		Array< __int64 > *styles;
		Array< int > *ahFarbeListe;
		RCArray< Bild > *ahBildListe;
		RCArray< AlphaFeld > *aBufferListe;
		RCArray< LRahmen > *aRahmenListe;
        Schrift *schrift;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) AuswahlListe();
		// Destruktor 
		__declspec( dllexport ) ~AuswahlListe();
		// nicht constant 
		__declspec( dllexport ) void update(); // aktualisiert die Auswahl Liste
		__declspec( dllexport ) void addEintrag( Text *txt ); // fügt einen Eintrag hinzu
		__declspec( dllexport ) void addEintrag( const char *txt );
		__declspec( dllexport ) void addEintrag( TextFeld *tf );
		__declspec( dllexport ) void addEintrag( int pos, Text *txt ); // fügt einen Eintrag bei position pos ein
		__declspec( dllexport ) void addEintrag( int pos, const char *txt );
		__declspec( dllexport ) void addEintrag( int pos, TextFeld *tf );
		__declspec( dllexport ) void setEintrag( int pos, Text *txt ); // ändert den pos - ten Eintrag
		__declspec( dllexport ) void setEintrag( int pos, unsigned char *txt );
		__declspec( dllexport ) void setEintrag( int pos, TextFeld *tf );
		__declspec( dllexport ) void setEintragPos( int vpos, int npos ); // taucht den Eintrag vpos mit dem Eintrag npos
		__declspec( dllexport ) void löscheEintrag( int pos ); // löscht den Eintrag pos
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift ); // legt die Schrift der Einträge fest
        __declspec( dllexport ) void setVScrollZuEintrag( int eintrag ); // scrollt zum Eintrag
        __declspec( dllexport ) void updateVScroll(); // scrollt zur Curser Position oder nach Unten
		__declspec( dllexport ) void setALRZ( LRahmen *rahmen ); // setzt einen Zeiger zum Auswahl Rahmen (nur ohne MulitStyled)
        __declspec( dllexport ) void setALRBreite( int br ); // setzt die Breite des Auswahl Rahmens (nur ohne MultiStyled)
        __declspec( dllexport ) void setALRFarbe( int fc ); // setzt die Farbe des Auswahl Rahmens (nur ohne MultiStyled)
		__declspec( dllexport ) void setAAFZ( AlphaFeld *buffer ); // setzt einen Zeiger zum Auswahl AlpaFeld (nur ohne MultiStyled)
        __declspec( dllexport ) void setAAFStärke( int st ); // setzt die Stärke des Auswahl Hintergrund Buffers (nur ohne MultiStyled)
        __declspec( dllexport ) void setAAFFarbe( int fc ); // setzt die Farbe des Auswahl Hintergrund Buffers (nur ohne MultiStyled)
        __declspec( dllexport ) void setAHBild( Bild *bild ); // setzt das Auswahl Hintergrund Bild (nur ohne MultiStyled)
        __declspec( dllexport ) void setAHBildZ( Bild *bild ); // setzt einen Zeiger zum Auswahl Hintergrund Bild (nur ohne MultiStyled)
        __declspec( dllexport ) void setAHFarbe( int fc ); // setzt die Auswahl Hintergrundfarbe (nur ohne MultiStyled)
		__declspec( dllexport ) void setALRZ( int pos, LRahmen *rahmen ); // setzt einen Zeiger zum Auswahl Rahmen (nur mit MulitStyled)
        __declspec( dllexport ) void setALRBreite( int pos, int br ); // setzt die Breite des Auswahl Rahmens (nur mit MultiStyled)
        __declspec( dllexport ) void setALRFarbe( int pos, int fc ); // setzt die Farbe des Auswahl Rahmens (nur mit MultiStyled)
		__declspec( dllexport ) void setAAFZ( int pos, AlphaFeld *buffer ); // setzt einen Zeiger zum Auswahl AlpaFeld (nur mit MultiStyled)
        __declspec( dllexport ) void setAAFStärke( int pos, int st ); // setzt die Stärke des Auswahl Hintergrund Buffers (nur mit MultiStyled)
        __declspec( dllexport ) void setAAFFarbe( int pos, int fc ); // setzt die Farbe des Auswahl Hintergrund Buffers (nur mit MultiStyled)
        __declspec( dllexport ) void setAHBild( int pos, Bild *bild ); // setzt das Auswahl Hintergrund Bild (nur mit MultiStyled)
        __declspec( dllexport ) void setAHBildZ( int pos, Bild *bild ); // setzt einen Zeiger zum Auswahl Hintergrund Bild (nur mit MultiStyled)
        __declspec( dllexport ) void setAHFarbe( int pos, int fc ); // setzt die Auswahl Hintergrundfarbe (nur mit MultiStyled)
		__declspec( dllexport ) void setMsStyle( int pos, __int64 style ); // setzt den Style des Eintrags (nur mit MultiStyled)
        __declspec( dllexport ) void setMsStyle( int pos, __int64 style, bool add_löschen );
		__declspec( dllexport ) void addMsStyle( int pos, __int64 style );
		__declspec( dllexport ) void löscheMsStyle( int pos, __int64 style );
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
		__declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
        __declspec( dllexport ) int getKlickEintrag( int my );
        __declspec( dllexport ) void setAuswahl( int ausw ); // setzt die Auswahl
		__declspec( dllexport ) void deSelect(); // deselectiert die Auswahl
		// constant 
		__declspec( dllexport ) int getEintragAnzahl() const; // gibt die Anzahl der Einträge zurück
		__declspec( dllexport ) int getAuswahl() const; // gibt den ersten ausgewählten Eintrag zurück
		__declspec( dllexport ) int getEintragPos( Text *eintragText ); // gibt die Position des eintrages mit dem entsprechenden Textes zurück
		__declspec( dllexport ) TextFeld *getEintrag( int pos ) const; // gibt den pos- ten Eintrag zurück
		__declspec( dllexport ) TextFeld *zEintrag( int pos ) const;
		__declspec( dllexport ) LRahmen *getARahmen() const; // gibt den Auswahl Rahmen zurück (ohne MultiStyled)
		__declspec( dllexport ) LRahmen *zARahmen() const;
		__declspec( dllexport ) int getAHFarbe() const; // gibt die Auswahl Hintergrund Farbe zurück (ohne MultiStyled)
		__declspec( dllexport ) Bild *getAHBild() const; // gibt das Auswahl Hintergrund Bild zurück (ohne MultiStyled)
		__declspec( dllexport ) Bild *zAHBild() const;
		__declspec( dllexport ) AlphaFeld *getABuffer() const; // gibt den Auswahl Buffer zurück (ohne MultiStyled)
		__declspec( dllexport ) AlphaFeld *zABuffer() const;
		__declspec( dllexport ) LRahmen *getARahmen( int pos ) const; // gibt den Auswahl Rahmen zurück (mit MultiStyled)
		__declspec( dllexport ) LRahmen *zARahmen( int pos ) const;
		__declspec( dllexport ) int getAHFarbe( int pos ) const; // gibt die Auswahl Hintergrund Farbe zurück (mit MultiStyled)
		__declspec( dllexport ) Bild *getAHBild( int pos ) const; // gibt das Auswahl Hintergrund Bild zurück (mit MultiStyled)
		__declspec( dllexport ) Bild *zAHBild( int pos ) const;
		__declspec( dllexport ) AlphaFeld *getABuffer( int pos ) const; // gibt den Auswahl Buffer zurück (mit MultiStyled)
		__declspec( dllexport ) AlphaFeld *zABuffer( int pos ) const;
		__declspec( dllexport ) inline bool hatMsStyle( int pos, __int64 style ) const; // prüft ob style vorhanden (mit MultiStyled)
		__declspec( dllexport ) inline bool hatMsStyleNicht( int pos, __int64 style ) const; // prüft obt style nicht vorhanden (mit MultiStyled)
		// Reference Counting 
		__declspec( dllexport ) AuswahlListe *getThis();
		__declspec( dllexport ) AuswahlListe *release();
	};
}

#endif