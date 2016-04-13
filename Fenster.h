#ifndef Fenster_H
#define Fenster_H

#include "Zeichnung.h"

namespace Framework
{
	class VScrollBar; // Scroll.h
	class HScrollBar; // Scroll.h
	class TextFeld; // TextFeld.h
	class LRahmen; // Rahmen.h
	class Bildschirm; // Bildschirm.h
	class AlphaFeld; // AlphaFeld.h
	class Schrift; // Schrift.h
	class Text; // Text.h
	class Bild; // Bild.h
	class WFenster; // aus dieser Datei
	class WFensterArray; // aus dieser Datei
	class Fenster; // aus dieser Datei

	// Erzeugen einer normalen Fensterklasse
	__declspec( dllexport ) WNDCLASS F_Normal( HINSTANCE hInst );
	__declspec( dllexport ) WNDCLASSEX F_NormalEx( HINSTANCE hInst );

	__declspec( dllexport ) LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	__declspec( dllexport ) void StartNachrichtenSchleife();
	__declspec( dllexport ) void StopNachrichtenSchleife( HWND hwnd );

	__declspec( dllexport ) unsigned char VirtualZuChar( int Virtual );
	// Klasse für ein Fenster in Windows
	class WFenster
	{
	private:
		HWND hWnd; // Handel zum Fenster
		int style;
		void *makParam;
		void *sakParam;
		void *takParam;
		bool( *MausAktion )( void *, void *, MausEreignis );
		void( *VSchließAktion )( void *, void * );
		void( *NSchließAktion )( void *, void * );
		bool( *TastaturAktion )( void *, void *, TastaturEreignis );
		Bildschirm *screen;
		int mx, my;
		bool verschiebbar;
		int ref;
		HWND rahmen;
		HBITMAP bitmap;
		HDC hdc;

	public:
		//--Konstruktor--
		__declspec( dllexport ) WFenster();
		__declspec( dllexport ) WFenster( HWND hWnd );
		//--Destruktor--
		__declspec( dllexport ) ~WFenster();
		// nicht constant
		__declspec( dllexport ) void erstellen( int style, WNDCLASS wc ); // Das Fenster Erstellen
		__declspec( dllexport ) void erstellenEx( int exStyle, int style, WNDCLASSEX wc ); // Das Fenster Erstellen
		__declspec( dllexport ) void setAnzeigeModus( int mod ); // Zeigt das Fenster im übergebenen Modus an
		__declspec( dllexport ) bool setFokus(); // Setzt den Fokus auf das Fenster
		__declspec( dllexport ) void setPosition( Punkt &pos ); // Bildschirmposition des Fensters setzen
		__declspec( dllexport ) void setGröße( Punkt &größe ); // Die größe des Fensters setzen
		__declspec( dllexport ) void setGröße( int breite, int höhe );
		__declspec( dllexport ) void setBounds( Punkt &pos, Punkt &größe ); // Größe und Porition in einem
		__declspec( dllexport ) void setBildschirm( Bildschirm *screen );
		__declspec( dllexport ) void zerstören(); // Zerstört das Fenster
		__declspec( dllexport ) void doMausAktion( MausEreignis &me ); // ruft MausAktion auf
		__declspec( dllexport ) void doVSchließAktion(); // ruft VSchließAktion auf
		__declspec( dllexport ) void doNSchließAktion(); // ruft NSchließAktion auf
		__declspec( dllexport ) void doTastaturAktion( TastaturEreignis &et ); // ruft TastaturAktion auf
		__declspec( dllexport ) void doRestoreMessage(); // macht den Rahmen sichtbar
		__declspec( dllexport ) void setMausEreignisParameter( void *p ); // setzt den Parameter vom Maus Ereignis
		__declspec( dllexport ) void setSchließEreignisParameter( void *p ); // setzt den Parameter vom Schließ Ereignis
		__declspec( dllexport ) void setTastaturEreignisParameter( void *p ); // setzt den Parameter vom Tastatur Ereignis
		__declspec( dllexport ) void setMausAktion( bool( *MausAk )( void *, void *, MausEreignis ) ); // setzt das MausEreignis
		__declspec( dllexport ) void setVSchließAktion( void( *vSchließAk )( void *, void * ) ); // setzt v schließ Aktion
		__declspec( dllexport ) void setNSchließAktion( void( *nSchließAk )( void *, void * ) ); // setzt n schließ Aktion
		__declspec( dllexport ) void setTastaturAktion( bool( *TastaturAk )( void *, void *, TastaturEreignis ) ); // setzt das TastaturEreignis
		__declspec( dllexport ) void setFensterHandle( HWND hWnd ); // setzt das operationsfenster
		__declspec( dllexport ) void setVerschiebbar( bool verschiebbar ); // legt fest, ob das Fenster durch ziehen mit Maus verschoben werden kann
		__declspec( dllexport ) void ladeRahmenFenster( Bild *zBild, HINSTANCE hinst ); // setzt einen Transpatenten Rahmen um das Fenster
		// constant
		__declspec( dllexport ) HWND getFensterHandle() const; // gibt das Fenster Handle zurück
		__declspec( dllexport ) Punkt getPosition() const; // gibt die Fensterposition zurück
		__declspec( dllexport ) Punkt getKörperPosition() const; // gibt die Fenster Körperposition zurück
		__declspec( dllexport ) Punkt getGröße() const; // gibt die Fenstergröße zurück
		__declspec( dllexport ) Punkt getKörperGröße() const; // gibt die Fenster Körpergröße zurück
		__declspec( dllexport ) int getKörperBreite() const; // gibt die Fenster Körperbreite zurück
		__declspec( dllexport ) int getKörperHöhe() const; // gibt die Fenster Körperhöhe zurück
		__declspec( dllexport ) bool hatMausAktion() const; // Prüft, ob eine Aktion bei MausEreignis festgelegt wurde
		__declspec( dllexport ) bool hatVSchließAktion() const; // Prüft, ob eine Aktion vor Fensterschließen festgelegt wurde
		__declspec( dllexport ) bool hatNSchließAktion() const; // Prüft, ob eine Aktion nach Fensterschließen festgelegt wurde
		__declspec( dllexport ) bool hatTastaturAktion() const; // Prüft, ob eine Aktion bei TastaturEreignis festgelegt wurde
		__declspec( dllexport ) Bildschirm *getBildschirm() const;
		__declspec( dllexport ) Bildschirm *zBildschirm() const;
		__declspec( dllexport ) bool istVerschiebbar() const; // prüft, ob das Fenster durch ziehen mit Maus verschoben werden kann
		// Reference Counting
		__declspec( dllexport ) WFenster *getThis();
		__declspec( dllexport ) WFenster *release();
	};

	// Verwaltung der WFenster im Framework
	class WFensterArray
	{
	private:
		WFensterArray *next;
		WFenster *This;

	public:
		// Konstruktor 
		__declspec( dllexport ) WFensterArray();
		// Destruktor 
		__declspec( dllexport ) ~WFensterArray();
		// add und remove 
		__declspec( dllexport ) bool addFenster( WFenster *fenster );
		__declspec( dllexport ) bool removeFenster( WFenster *fenster );
		__declspec( dllexport ) WFensterArray* getNext();
		__declspec( dllexport ) void setNext0();
		__declspec( dllexport ) void del();
		// Messages 
		__declspec( dllexport ) bool sendVSchließMessage( HWND hWnd );
		__declspec( dllexport ) bool sendNSchließMessage( HWND hwnd );
		__declspec( dllexport ) bool sendMausMessage( HWND hWnd, MausEreignis &me );
		__declspec( dllexport ) bool sendTastaturMessage( HWND hwnd, TastaturEreignis &te );
		__declspec( dllexport ) bool sendRestoreMessage( HWND hwnd );
		__declspec( dllexport ) WFenster *getThis();
	};

	// WMessageBox
	__declspec( dllexport ) void WMessageBox( HWND hWnd, Text *titel, Text *meldung, UINT style );

	// Fenster Klasse im Programm
	class Fenster : public Zeichnung
	{
    public:
        class Style : public Zeichnung::Style
        {
        public:
            const static __int64 KörperHintergrund = 0x000000008;
            const static __int64 KörperHAlpha = 0x000000010;
            const static __int64 KörperHBild = 0x000000020;
            const static __int64 KörperBuffered = 0x000000040;
            const static __int64 Titel = 0x000000080;
            const static __int64 TitelHintergrund = 0x000000100;
            const static __int64 TitelHAlpha = 0x000000200;
            const static __int64 TitelHBild = 0x000000400;
            const static __int64 TitelBuffered = 0x000000800;
            const static __int64 Schließbar = 0x000001000;
            const static __int64 SchließHintergrund = 0x000002000;
            const static __int64 SchließHAlpha = 0x000004000;
            const static __int64 SchließHBild = 0x000008000;
            const static __int64 SchließBuffer = 0x000010000;
            const static __int64 SchließKlickBuffer = 0x000020000;
            const static __int64 Beweglich = 0x000040000;
            const static __int64 BreiteÄnderbar = 0x000080000;
            const static __int64 HöheÄnderbar = 0x000100000;
            const static __int64 TitelHöheÄnderbar = 0x000200000;
            const static __int64 MinBr = 0x000400000;
            const static __int64 MaxBr = 0x000800000;
            const static __int64 MinHö = 0x001000000;
            const static __int64 MaxHö = 0x002000000;
            const static __int64 Körper_minBr = 0x004000000;
            const static __int64 Körper_maxBr = 0x008000000;
            const static __int64 Körper_minHö = 0x010000000;
            const static __int64 Körper_maxHö = 0x020000000;
            const static __int64 VScroll = 0x040000000;
            const static __int64 HScroll = 0x080000000;
            const static __int64 METransparenz = 0x100000000;
            const static __int64 Rahmen = 0x200000000;

            const static __int64 min_max = MinHö | MaxHö | MaxBr | MaxHö;
            const static __int64 körper_min_max = Körper_minBr | Körper_maxBr | Körper_minHö | Körper_maxBr;
            const static __int64 scroll = VScroll | HScroll;
            const static __int64 nichtfixiert = TitelHöheÄnderbar | HöheÄnderbar | BreiteÄnderbar | Beweglich;

            const static __int64 normal = Sichtbar | Erlaubt | Rahmen | Titel | TitelBuffered | Schließbar | SchließHBild | SchließKlickBuffer | Beweglich;
        };
	private:
		bool( *schließenMe )( void *, void *, MausEreignis );
		void *schließenMeParam;
		LRahmen *rahmen;
		TextFeld *titel;
		ZeichnungArray *members;
		int bgKörperFarbe;
		Bild *bgKörperBild;
		AlphaFeld *körperBuffer;
		int bgSchließFarbe;
		Bild *bgSchließBild;
		AlphaFeld *schließBuffer;
		AlphaFeld *schließKlickBuffer;
		VScrollBar *vScroll;
		HScrollBar *hScroll;
		Punkt min, max;
		Punkt kMin, kMax;
		bool schließKlick, klick;
		int mx, my;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) Fenster();
		// Destruktor 
		__declspec( dllexport ) ~Fenster();
		// nicht const 
		// -- Rahmen -- 
		__declspec( dllexport ) void setRahmenZ( LRahmen *ram ); // setzt den rahmen
		__declspec( dllexport ) void setRFarbe( int f ); // setzt die Rahmen Farbe
		__declspec( dllexport ) void setRBreite( int br ); // setzt die Breite des Rahmens
		// -- Titel -- 
		__declspec( dllexport ) void setTitel( Text *txt ); // setzt den Titel
		__declspec( dllexport ) void setTitelZ( Text *txt );
		__declspec( dllexport ) void setTitel( const char *txt );
		__declspec( dllexport ) void setTTextFeldZ( TextFeld *tf ); // setzt das Titel TextFeld
		// -- Schrift -- 
		__declspec( dllexport ) void setTSchriftZ( Schrift *schrift ); // setzt die Titel Schrift
		__declspec( dllexport ) void setTSFarbe( int f ); // setzt die Titel Schrift Farbe
		__declspec( dllexport ) void setTSGröße( int gr ); // setzt die Titel Schrift Größe
		// -- Titel Hintergrund -- 
		__declspec( dllexport ) void setTBgFarbe( int f ); // setzt Titel Hintergrund farbe
		// -- Titel AlphaFeld -- 
		__declspec( dllexport ) void setTAlphaFeldZ( AlphaFeld *af ); // setzt das Titel AlphaFeld
		__declspec( dllexport ) void setTAfFarbe( int f ); // setzt die Titel AlphFeld Farbe
		__declspec( dllexport ) void setTAfStärke( int st ); // setzt die Stärke des Titel AlphaFeldes
		// -- Titel Hintergrund Bild -- 
		__declspec( dllexport ) void setTBgBild( Bild *b ); // setzt das Titel Hintergrund Bild
		__declspec( dllexport ) void setTBgBildZ( Bild *b );
		// -- Titel Rahmen -- 
		__declspec( dllexport ) void setTRahmenZ( LRahmen *ram ); // set Titel Rahmen
		__declspec( dllexport ) void setTRFarbe( int f ); // setzt die Titel Rahmen Farbe
		__declspec( dllexport ) void setTRBreite( int br ); // setzt die Titel Rahmen Breite
		// -- Körper Hintergrund -- 
		__declspec( dllexport ) void setKBgFarbe( int f ); // setzt die Körper Hintergrund Farbe
		// -- Körper Hintergrund Bild -- 
		__declspec( dllexport ) void setKBgBild( Bild *b ); // setzt das Körper Hintergrund Bild
		__declspec( dllexport ) void setKBgBildZ( Bild *b );
		// -- Körper AlphaFeld -- 
		__declspec( dllexport ) void setKAlphaFeldZ( AlphaFeld *af ); // setzt das Körper AlphaFeld
		__declspec( dllexport ) void setKAfFarbe( int f ); // setzt Körper AlphaFeld Farbe
		__declspec( dllexport ) void setKAfStärke( int st ); // setzt die Stärke des Körper AlphaFeldes
		// -- Schließen --
		__declspec( dllexport ) void setSchließenMeParam( void *param );
		__declspec( dllexport ) void setSchließenMe( bool( *schließenMe )( void *, void *, MausEreignis ) ); // setzt das Schließen Mausereignis
		// -- Schließen Hintergrund -- 
		__declspec( dllexport ) void setSBgFarbe( int f ); // setzt die Schließ Hintergrund Farbe
		// -- Schließen Hintergrund Bild -- 
		__declspec( dllexport ) void setSBgBild( Bild *b ); // setzt das Schließ Hintergrund Bild
		__declspec( dllexport ) void setSBgBildZ( Bild *b );
		// -- Schließen AlphaFeld -- 
		__declspec( dllexport ) void setSAlphaFeldZ( AlphaFeld *af ); // setzt das Schließ AlphaFeld
		__declspec( dllexport ) void setSAfFarbe( int f ); // setzt die Farbe des Schließ AlphaFeldes
		__declspec( dllexport ) void setSAfStärke( int st ); // setzt die Stärke des Schließ AlphaFeldes
		// -- Schließen Klick AlphaFeld -- 
		__declspec( dllexport ) void setSKAlphaFeldZ( AlphaFeld *af ); // setzt das Schließ klick AlphaFeld
		__declspec( dllexport ) void setSKAfFarbe( int f ); // setzt die Farbe des Schließ klick AlphaFeldes
		__declspec( dllexport ) void setSKAfStärke( int st ); // setzt die Stärke des Schließ klick AlphaFeldes
		// -- min max -- 
		__declspec( dllexport ) void setMin( int mx, int my ); // setzt die Mindest Fenster Größe
		__declspec( dllexport ) void setMin( const Punkt &min );
		__declspec( dllexport ) void setMax( int mx, int my ); // setzt die Maximale Fenster Größe
		__declspec( dllexport ) void setMax( const Punkt &max );
		__declspec( dllexport ) void setKMin( int mx, int my ); // setzt die Mindest Körper Größe
		__declspec( dllexport ) void setKMin( const Punkt &min );
		__declspec( dllexport ) void setKMax( int mx, int my ); // setzt die Maximale Körper Größe
		__declspec( dllexport ) void setKMax( const Punkt &max );
		// -- scroll -- 
		__declspec( dllexport ) void setHScrollBarZ( HScrollBar *hScroll ); // setzt die Horizontale Scroll Bar
		__declspec( dllexport ) void setVScrollBarZ( VScrollBar *vScroll ); // setzt die Vertikale Scroll BAr
		__declspec( dllexport ) void setHSBMax( int max ); // setzt das Scroll Maximum
		__declspec( dllexport ) void setVSBMax( int max );
		__declspec( dllexport ) void setHSBScroll( int scroll ); // setzt die momentane Scroll Position
		__declspec( dllexport ) void setVSBScroll( int scroll );
		// -- Members -- 
		__declspec( dllexport ) void addMember( Zeichnung *zOobj ); // fügt einen Member hinzu
		__declspec( dllexport ) void removeMember( Zeichnung *zObj ); // entfernt einen Member
		// -- Messages -- 
		__declspec( dllexport ) bool tick( double tickval ) override; // tick
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
		__declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
		// -- Render -- 
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichent nach zRObj
		// constant 
		// -- Rahmen -- 
		__declspec( dllexport ) LRahmen *getRahmen() const; // gibt den Rahmen zurück
		__declspec( dllexport ) LRahmen *zRahmen() const;
		__declspec( dllexport ) int getRFarbe() const; // gibt die Farbe des Rahmens zurück
		__declspec( dllexport ) int getRBreite() const; // gibt die breite des Rahmens zurück
		// -- Titel  -- 
		__declspec( dllexport ) Text *getTitel() const; // gibt den Titel zurück
		__declspec( dllexport ) Text *zTitel() const;
		__declspec( dllexport ) TextFeld *getTTextFeld() const; // gibt das Titel TextFeld zurück
		__declspec( dllexport ) TextFeld *zTTextFeld() const;
		// -- Titel Schrift -- 
		__declspec( dllexport ) Schrift *getTSchrift() const; // gibt die Titel Schrift zurück
		__declspec( dllexport ) Schrift *zTSchrift() const;
		__declspec( dllexport ) int getTSFarbe() const; // gibt die Titel Schrift Farbe zurück
		__declspec( dllexport ) int getTSGröße() const; // gibt die Titel Schrift Größe zurück
		// -- Titel Hintergrund -- 
		__declspec( dllexport ) int getTBgFarbe() const; // gibt die Titel Hintergrund Farbe zurück
		// -- Titel AlphaFeld -- 
		__declspec( dllexport ) AlphaFeld *getTAlphaFeld() const; // gibt das Titel AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zTAlphaFeld() const;
		__declspec( dllexport ) int getTAfFarbe() const; // gibt die Farbe des Titel AlphaFeldes zurück
		__declspec( dllexport ) int getTAfStärke() const; // gibt die Stärke des TitelAlphaFeldes zurück
		// -- Titel Hintergrund Bild -- 
		__declspec( dllexport ) Bild *getTBgBild() const; // gibt das Titel Hintergrund Bild zurück
		__declspec( dllexport ) Bild *zTBgBild() const;
		// -- Titel Rahmen -- 
		__declspec( dllexport ) LRahmen *getTRahmen() const; // gibt den Titel Rahmen zurück
		__declspec( dllexport ) LRahmen *zTRahmen() const;
		__declspec( dllexport ) int getTRFarbe() const; // gibt die Farbe des Titel Rahmens zurück
		__declspec( dllexport ) int getTRBreite() const; // gibt die Breite des Titel Rahmens zurück
		// -- Körper Hintergrund -- 
		__declspec( dllexport ) int getKBgFarbe() const; // gibt die Körper Hintergrund Farbe zurück
		// -- Körper Hintergrund Bild -- 
		__declspec( dllexport ) Bild *getKBgBild() const; // gibt das Körper Hintergrund Bild zurück
		__declspec( dllexport ) Bild *zKBgBild() const;
		// -- Körper AlphaFeld -- 
		__declspec( dllexport ) AlphaFeld *getKAlphaFeld() const; // gibt das Körper AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zKAlphaFeld() const;
		__declspec( dllexport ) int getKAfFarbe() const; // gibt die Farbe des Körper AlphaFeldes zurück
		__declspec( dllexport ) int getKAfStärke() const; // gibt die Stärke des Körper AlphaFeldes zurück
		// -- Schließen Hintergrund -- 
		__declspec( dllexport ) int getSBgFarbe() const; // gibt die Schließ Hintergrund Farbe zurück
		// -- Schließen Hintergrund Bild -- 
		__declspec( dllexport ) Bild *getSBgBild() const; // gibt das Schließ Hintergrund Bild zurück
		__declspec( dllexport ) Bild *zSBgBild() const;
		// -- Schließen AlphaFeld -- 
		__declspec( dllexport ) AlphaFeld *getSAlphaFeld() const; // gibt das Schließ AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zSAlphaFeld() const;
		__declspec( dllexport ) int getSAfFarbe() const; // gibt die Farbe des Schließ AlphaFeldes zurück
		__declspec( dllexport ) int getSAfStärke() const; // gibt die Stärke des Schließ AlphaFeldes zurück
		// -- Schließen Klick AlphaFeld -- 
		__declspec( dllexport ) AlphaFeld *getSKAlphaFeld() const; // gibt das Schließ Klick AlphaFeld zurück
		__declspec( dllexport ) AlphaFeld *zSKAlphaFeld() const;
		__declspec( dllexport ) int getSKAfFarbe() const; // gibt die Farbe des Schließ Klick AlphaFeldes zurück
		__declspec( dllexport ) int getSKAfStärke() const; // gibt die Stärke des Schließ Klick AlphaFeldes zurück
		// -- min max -- 
		__declspec( dllexport ) const Punkt &getMin() const; // gibt die minimale Fenstergröße zurück
		__declspec( dllexport ) const Punkt &getMax() const; // gibt die maximale Fenstergröße zurück
		__declspec( dllexport ) const Punkt &getKMin() const; // gibt die minimale Fenstergröße zurück
		__declspec( dllexport ) const Punkt &getKMax() const; // gibt die maximale Fenstergröße zurück
		// -- scroll -- 
		__declspec( dllexport ) VScrollBar *getVScrollBar() const; // gibt die Vertikale Scroll Bar zurück
		__declspec( dllexport ) VScrollBar *zVScrollBar() const;
		__declspec( dllexport ) HScrollBar *getHScrollBar() const; // gibt die Horizontale Scroll Bar zurück
		__declspec( dllexport ) HScrollBar *zHScrollBar() const;
		// -- Members -- 
		__declspec( dllexport ) ZeichnungArray *getMembers() const; // gibt die Members zurück
		// -- Kopie --
		__declspec( dllexport ) Zeichnung *dublizieren() const override; // Erzeugt eine Kopie des Fensters
		// Reference Counting 
		__declspec( dllexport ) Fenster *getThis();
		__declspec( dllexport ) Fenster *release();
	};
}
#endif