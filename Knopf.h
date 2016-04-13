#ifndef Knopf_H
#define Knopf_H

#include "TextFeld.h"

namespace Framework
{
	class TextFeld; // TextFeld.h
	class AlphaFeld; // AlphaFeld.h
	class Text; // Text.h
	class Schrift; // Schrift.h
	class LRahmen; // Rahmen.h
	class LTDBDatei; // Dateisystem.h
	class Knopf; // aus dieser Datei
	class KontrollKnopf; // aus dieser Datei

	class Knopf : public TextFeld
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 MehrfarbigText = 0x0010000;
            const static __int64 KlickFarbe = 0x0020000;
            const static __int64 KlickBild = 0x0040000;
            const static __int64 KlickAlpha = 0x0080000;
            const static __int64 KlickBuffer = 0x0100000;

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | Buffered | MehrfarbigText | KlickBuffer;
        };

	private:
		int klickFarbe;
		Bild *klickBild;
		AlphaFeld *klickBuffer;
		int klickIndex;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) Knopf();
		// Destruktor 
		__declspec( dllexport ) ~Knopf();
		// nicht constant 
		__declspec( dllexport ) void setKlickFarbe( int fc ); // setzt die Klick Farbe
		__declspec( dllexport ) void setKlickBild( Bild *bild ); // setzt das Klick Bild
		__declspec( dllexport ) void setKlickBildZ( Bild *bild ); // setzt einen Zeiger zum Klick Bild
		__declspec( dllexport ) void setKBZ( AlphaFeld *af );
		__declspec( dllexport ) void setKBStärke( int st ); // setzt die Stärke des Klick Buffers
		__declspec( dllexport ) void setKBFarbe( int fc ); // setzt die Farbe des Klick Buffers
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override; // Maus Ereignis
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichenet nach zRObj
		// constant 
		__declspec( dllexport ) int getKlickFarbe() const; // gibt getThis der Klick Farbe zurück
		__declspec( dllexport ) Bild *getKlickBild() const; // gibt getThis des Klick Bildes zurück
		__declspec( dllexport ) Bild *zKlickBild() const; // gibt einen Zeiger zum Klick Bild zurück
		__declspec( dllexport ) AlphaFeld *getKB() const; // gibt getThis des Klick Buffers zurück
		__declspec( dllexport ) AlphaFeld *zKB() const; // gibt den Klick Buffer zurück
		__declspec( dllexport ) int getKBFarbe() const; // gibt getThis der Farbe des Klick Buffers zurück
		__declspec( dllexport ) int getKBStärke() const; // gibt die Stärke des Klickbuffers zurück
		__declspec( dllexport ) Zeichnung *dublizieren() const override; // Erzeugt eine Kopie des Zeichnungs
		// Reference Counting 
		__declspec( dllexport ) Knopf *getThis();
		__declspec( dllexport ) Knopf *release();
	};

	class KontrollKnopf : public ZeichnungHintergrund
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Selected = 0x00080;

            const static __int64 MehrfarbigText = 0x001000;
            const static __int64 KlickFarbe = 0x002000;
            const static __int64 KlickBild = 0x004000;
            const static __int64 KlickAlpha = 0x008000;
            const static __int64 KlickBuffer = 0x010000;
            const static __int64 SelectFarbe = 0x020000;
            const static __int64 SelectBild = 0x040000;
            const static __int64 SelectAlpha = 0x080000;
            const static __int64 SelectBuffer = 0x100000;
            const static __int64 SelectText = 0x200000;
            const static __int64 MehrzeiligText = 0x400000;

            const static __int64 MausKlick = 0x800000;

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | KlickBuffer;
        };

	private:
		Text *txt;
		Text *sTxt;
		int sBgF;
		int kBgF;
		Bild *sBgB;
		Bild *kBgB;
		AlphaFeld *sAf;
		AlphaFeld *kAf;
		Bild *kasten;
		Bild *sKasten;
		Schrift *schrift;
		int sF;
		int sGr;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) KontrollKnopf();
		// Destruktor 
		__declspec( dllexport ) ~KontrollKnopf();
		// nicht constant 
		__declspec( dllexport ) void setTextZ( Text *txt ); // setzt den Text
		__declspec( dllexport ) void setText( Text *txt );
		__declspec( dllexport ) void setText( const char *txt );
		__declspec( dllexport ) void setSTextZ( Text *txt ); // setzt den Text bei Selectiert
		__declspec( dllexport ) void setSText( Text *txt );
		__declspec( dllexport ) void setSText( const char *txt );
		__declspec( dllexport ) void setSchriftZ( Schrift *schrift ); // setzt die Schrift
		__declspec( dllexport ) void setSFarbe( int f ); // setzt die Schrift Farbe
		__declspec( dllexport ) void setSGröße( int gr ); // setzt die Schrift Größe
		__declspec( dllexport ) void setSBgFarbe( int f ); // setzt die Select Hintergrundfarbe
		__declspec( dllexport ) void setKBgFarbe( int f ); // setzt die Klick Hintergrundfarbe
		__declspec( dllexport ) void setSBgBildZ( Bild *b ); // setzt das Select Hintergrundbild
		__declspec( dllexport ) void setSBgBild( Bild *b );
		__declspec( dllexport ) void setKBgBildZ( Bild *b ); // setzt das Klick Hintergrundbild
		__declspec( dllexport ) void setKBgBild( Bild *b );
		__declspec( dllexport ) void setSAlphaFeldZ( AlphaFeld *af ); // setzt das Select Alpha Feld
		__declspec( dllexport ) void setSAFFarbe( int f ); // setzt die Select Alphafeld Farbe
		__declspec( dllexport ) void setSAFStärke( int st ); // setzt die Select AlphaFeld Stärke
		__declspec( dllexport ) void setKAlphaFeldZ( AlphaFeld *af ); // setzt das Klick Alpha Feld
		__declspec( dllexport ) void setKAFFarbe( int f ); // setzt die Klick Alphafeld Farbe
		__declspec( dllexport ) void setKAFStärke( int st ); // setzt die Klick AlphaFeld Stärke
		__declspec( dllexport ) void loadData( LTDBDatei *zDat ); // läht die Systembilder
		__declspec( dllexport ) void loadData( const char *ltdb );
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override; // Nachrichten verarbeitung
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
		// constant 
		__declspec( dllexport ) Text *getText() const; // gibt den Text zurück
		__declspec( dllexport ) Text *zText() const;
		__declspec( dllexport ) Text *getSText() const; // gibt den Select Text zurück
		__declspec( dllexport ) Text *zSText() const;
		__declspec( dllexport ) Schrift *getSchrift() const; // gibt die Schrift zurück
		__declspec( dllexport ) Schrift *zSchrift() const;
		__declspec( dllexport ) int getSFarbe() const; // gibt die Schrift Farbe zurück
		__declspec( dllexport ) int getSGröße() const; // gibt die Schrift Größe zurück
		__declspec( dllexport ) int getSBgFarbe() const;// gibt die Select Hintergrundfarbe zurück
		__declspec( dllexport ) int getKBgFarbe() const; // gibt die Klick Hintergrundfarbe zurück
		__declspec( dllexport ) Bild *getSBgBild() const; // gibt das Select Hintergrundbild zurück
		__declspec( dllexport ) Bild *zSBgBild() const;
		__declspec( dllexport ) Bild *getKBgBild() const; // gibt das Klick Hintergrundbild zurück
		__declspec( dllexport ) Bild *zKBgBild() const;
		__declspec( dllexport ) AlphaFeld *getSAlphaFeld() const; // gibt das Select AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zSAlphaFeld() const;
		__declspec( dllexport ) AlphaFeld *getKAlphaFeld() const; // gibt das Klick AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zKAlphaFeld() const;
		// Reference Counting 
		__declspec( dllexport ) KontrollKnopf *getThis();
		__declspec( dllexport ) KontrollKnopf *release();
	};
}

#endif