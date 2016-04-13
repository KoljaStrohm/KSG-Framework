#ifndef Zeichnung_H
#define Zeichnung_H

#include "Punkt.h"

namespace Framework
{
	struct VScrollData; // Scroll.h
	struct HScrollData; // Scroll.h
	struct MausEreignis; // MausEreignis.h
	struct TastaturEreignis; // TastaturEreignis.h
	class Bild; // Bild.h
	class Zeichnung; // Aus dieser Datei
	class ZeichnungArray; // Aus dieser Datei
	class ToolTip; // ToopTip.h
	class Bildschirm; // Bildschirm.h
    class LRahmen; // Rahmen.h
    class AlphaFeld; // AlphaFeld.h
    class VScrollBar; // Scroll.h
    class HScrollBar; // Scroll.h

	class Zeichnung
	{
    public:
        class Style
        {
        public:
            const static __int64 Sichtbar = 0x00001;
            const static __int64 Erlaubt = 0x00002;
            const static __int64 Fokus = 0x00040;// 0x80
        };
	protected:
		Punkt pos;
		Punkt gr;
		void *makParam;
		void *takParam;
		bool( *Mak )( void *, void *, MausEreignis );
		bool( *Tak )( void *, void *, TastaturEreignis );
        void *nmakParam;
        void *ntakParam;
        bool( *nMak )( void *, void *, MausEreignis );
        bool( *nTak )( void *, void *, TastaturEreignis );
		bool mausIn;
		CRITICAL_SECTION cs;
		ToolTip *toolTip;
        __int64 style;
		bool rend;

	public:
		// Konstruktor 
		__declspec( dllexport ) Zeichnung();
		// Destruktor 
		__declspec( dllexport ) ~Zeichnung();
		// nicht constant 
		__declspec( dllexport ) void setRender();
		__declspec( dllexport ) void setToolTipText( const char *txt, Bildschirm *zScreen );
		__declspec( dllexport ) void lockZeichnung();
		__declspec( dllexport ) void unlockZeichnung();
		__declspec( dllexport ) void setMausEreignisParameter( void *p ); // setzt den Parameter vom Maus Ereignis
		__declspec( dllexport ) void setTastaturEreignisParameter( void *p ); // setzt den Parameter vom Tastatur Ereignis
		__declspec( dllexport ) void setMausEreignis( bool( *ak )( void *, void *, MausEreignis ) ); // setzt das Maus Ereignis
		__declspec( dllexport ) void setTastaturEreignis( bool( *ak )( void *, void *, TastaturEreignis ) ); // setzt das TastaturEreignis
		__declspec( dllexport ) void setNMausEreignisParameter( void *p ); // setzt den Parameter vom Maus Ereignis
		__declspec( dllexport ) void setNTastaturEreignisParameter( void *p ); // setzt den Parameter vom Tastatur Ereignis
		__declspec( dllexport ) void setNMausEreignis( bool( *ak )( void *, void *, MausEreignis ) ); // setzt das Maus Ereignis
		__declspec( dllexport ) void setNTastaturEreignis( bool( *ak )( void *, void *, TastaturEreignis ) ); // setzt das TastaturEreignis
		__declspec( dllexport ) virtual void doMausEreignis( MausEreignis &me ); // ruft Mak auf
		__declspec( dllexport ) virtual void doTastaturEreignis( TastaturEreignis &te ); // ruft Tak auf
		__declspec( dllexport ) virtual bool tick( double tickval ); // tick
		__declspec( dllexport ) void setPosition( const Punkt &pos ); // setzt die position
		__declspec( dllexport ) void setX( int xPos ); // setzt die x position
		__declspec( dllexport ) void setY( int yPos ); // setzt die y position
		__declspec( dllexport ) void setGröße( const Punkt &gr ); // setzt die Größe
		__declspec( dllexport ) void setPosition( int x, int y ); // setzt die position
		__declspec( dllexport ) void setGröße( int x, int y ); // setzt die Größe
		__declspec( dllexport ) void setStyle( __int64 style ); // setzt den Style des Zeichnunges
		__declspec( dllexport ) void setStyle( __int64 style, bool add_löschen );
		__declspec( dllexport ) void addStyle( __int64 style );
		__declspec( dllexport ) void löscheStyle( __int64 style );
		__declspec( dllexport ) virtual void render( Bild &zRObj ); // zeichnet nach zRObj
		// constant 
		__declspec( dllexport ) bool hatMausEreignis() const; // prüft, ob Mak gesetzt ist
		__declspec( dllexport ) bool hatTastaturEreignis() const; // prüft, ob Tak gesetzt ist
		__declspec( dllexport ) const Punkt &getPosition() const; // gibt die Position zurück
		__declspec( dllexport ) const Punkt &getGröße() const; // gibt die Größe zurück
		__declspec( dllexport ) int getBreite() const; // gibt die Breite zurück
		__declspec( dllexport ) int getHöhe() const; // gibt die Höhe zurück
		__declspec( dllexport ) int getX() const; // gibt X zurück
		__declspec( dllexport ) int getY() const; // gibt Y zurück
		__declspec( dllexport ) ToolTip *getToolTip() const; // gibt den ToolTip Text
		__declspec( dllexport ) ToolTip *zToolTip() const;
        __declspec( dllexport ) inline bool hatStyle( __int64 style ) const; // prüft, ob style vorhanden
        __declspec( dllexport ) inline bool hatStyleNicht( __int64 style ) const; // prüft, ob style nicht vorhanden
		__declspec( dllexport ) virtual Zeichnung *dublizieren() const; // Erzeugt eine Kopie des Zeichnungs
		// reference Counting 
	};

	class ZeichnungArray// Array von Zeichnungen
	{
	private:
        Zeichnung *This;
        ZeichnungArray *next;
		int index;

	public:
		// Konstruktor 
		__declspec( dllexport ) ZeichnungArray();
		// Destruktor 
		__declspec( dllexport ) ~ZeichnungArray();
		// nicht const 
		__declspec( dllexport ) bool addZeichnung( Zeichnung *obj ); // Fügt ein Zeichnung hinzu
		__declspec( dllexport ) bool removeZeichnung( Zeichnung *obj ); // Entfernt ein Zeichnung
		__declspec( dllexport ) bool removeZeichnung( int i ); // Entfernt das i-te Zeichnung
		__declspec( dllexport ) void setNext0(); // Setzt das nächste Zeichnung zu 0
		__declspec( dllexport ) void updateIndex( int i ); // aktualisiert die Index variable
		// constant 
		__declspec( dllexport ) ZeichnungArray *getNext() const; // gibt das nächste Zeichnung zurück
		__declspec( dllexport ) Zeichnung *getZeichnung( int i ) const; // gibt das i-te Zeichnung zurück
		__declspec( dllexport ) Zeichnung *getZeichnung() const; // gibt das Zeichnung zurück
		__declspec( dllexport ) int getIndex() const; // Gibt den Index zurück
		__declspec( dllexport ) void sendMausAll( MausEreignis &me ) const; // sendet me an alle volgenden Zeichnunge
		__declspec( dllexport ) void sendTastaturAll( TastaturEreignis &te ) const; // sendet te an alle volgenden Zeichnunge
		__declspec( dllexport ) void render( Bild &zRObj ); // Zeichnet alle nach zRObj
		__declspec( dllexport ) bool tick( double tickval ); // tick
	};

    class ZeichnungHintergrund : public Zeichnung
    {
    public:
        class Style : public Zeichnung::Style
        {
        public:
            const static __int64 Rahmen = 0x00010; // 0x4
            const static __int64 Hintergrund = 0x00020; // 0x8
            const static __int64 HAlpha = 0x00040; // 0x10
            const static __int64 HBild = 0x00080; // 0x20
            const static __int64 Buffered = 0x00100; // 0x40
            const static __int64 VScroll = 0x00200; // Wenn dieser Flag gesetzt wird, erscheint eine Scrollbar am rechten Rand
            const static __int64 HScroll = 0x00400; // Wenn dieser Flag gesetzt wird, erscheint eine Scrollbar am unteren Rand
        };

    protected:
        int hintergrundFarbe;
        LRahmen *rahmen;
        Bild *hintergrundBild;
        AlphaFeld *hintergrundFeld;
        VScrollBar *vertikalScrollBar;
        HScrollBar *horizontalScrollBar;
        Punkt innenPosition;
        Punkt innenGröße;

    public:
        // Konstruktor 
        __declspec( dllexport ) ZeichnungHintergrund();
        // Destruktor 
        __declspec( dllexport ) ~ZeichnungHintergrund();
        // Setzt das Hintergrund Bild (benötigt Flag zum Zeichnen: Hintergrund, HBild)
        //  bild: Das Bild wird kopiert und als Hintergrundbild verwendet
        __declspec( dllexport ) void setHintergrundBild( Bild *bild );
        // Setzt einen Zeiger auf das Hintergrund Bild (benötigt Flag zum Zeichnen: Hintergrund)
        //  bild: Das Bild wid ohne es zu kopieren verwendet
        __declspec( dllexport ) void setHintergrundBildZ( Bild *bild );
        // Setzt die Hintergrund Farbe (benötigt Flag zum Zeichnen: Hintergrund)
        //  fc: Die Hintergrundfarbe im A8R8G8B8 Format
        __declspec( dllexport ) void setHintergrundFarbe( int fc );
        // Setzt einen Zeiger auf das AlphaFeld (benötigt Flag zum Zeichnen: Buffered)
        //  buff: Das AlphaFeld, das über den Hintergrund gezeichnet werden soll
        __declspec( dllexport ) void setAlphaFeldZ( AlphaFeld *buff );
        // Setzt die Stärke des AlphaFeldes (benötigt Flag zum Zeichnen: Buffered)
        //  st: Die Stärke des AlphaFeldes, welches über dem Hintergrund gezeichnet werden soll
        __declspec( dllexport ) void setAlphaFeldStärke( int st );
        // Setzt die Farbe des AlphaFeldes (benötigt Flag zum Zeichnen: Buffered)
        //  fc: Die Farbe des AlphaFeldes, welches über dem Hintergrund gezeichnet werden soll
        __declspec( dllexport ) void setAlphaFeldFarbe( int fc );
        // Setzt einen Zeiger zu dem Linien Rahmen, der um das TextFeld gezeichnet werden soll (benötigt Flag zum Zeichnen: Rahmen)
        //  ram: Der Rahmen
        __declspec( dllexport ) void setLinienRahmenZ( LRahmen *ram );
        // Setzt die Breite des Linien Rahmens (benötigt Flag zum Zeichnen: Rahmen)
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setLinienRahmenBreite( int br );
        // Setzt die Farbe des Linien Rahmens (benötigt Flag zum Zeichnen: Rahmen)
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setLinienRahmenFarbe( int fc );
        // Setzt die Scrollgeschwindigkeit der vertikalen ScrollBar (benötigt Flag zum Zeichnen: VScroll)
        //  ks: Die Scrollgeschwindigkeit in Pixeln für jeden Maus Klick
        __declspec( dllexport ) void setVertikalKlickScroll( int ks );
        // Scrollt an eine Bestimmte Stelle bei der vertikalen ScrollBar (benötigt Flag zum Zeichnen: VScroll)
        //  pos: Das Scroll Offset in Pixeln.
        __declspec( dllexport ) void setVertikalScrollPos( int pos );
        // Setzt die Farbe der vertikalen ScrollBar (benötigt Flag zum Zeichnen: VScroll)
        //  f: Die Fordergrundfarbe der ScrollBar im A8R8G8B8 Format
        //  bgF: Die Hintergrundfarbe der ScrollBar im A8R8G8B8 Format
        __declspec( dllexport ) void setVertikalScrollFarbe( int f, int bgF );
        // Setzt die Scrollgeschwindigkeit der horizontalen ScrollBar (benötigt Flag zum Zeichnen: HScroll)
        //  ks: Die Scrollgeschwindigkeit in Pixeln für jeden Maus Klick
        __declspec( dllexport ) void setHorizontalKlickScroll( int ks );
        // Scrollt an eine Bestimmte Stelle bei der horizontalen ScrollBar (benötigt Flag zum Zeichnen: HScroll)
        //  pos: Das Scroll Offset in Pixeln.
        __declspec( dllexport ) void setHorizontalScrollPos( int pos );
        // Setzt die Farbe der horizontalen ScrollBar (benötigt Flag zum Zeichnen: HScroll)
        //  f: Die Fordergrundfarbe der ScrollBar im A8R8G8B8 Format
        //  bgF: Die Hintergrundfarbe der ScrollBar im A8R8G8B8 Format
        __declspec( dllexport ) void setHorizontalScrollFarbe( int f, int bgF );
        // Updated den Zeichenhintergrund
        //  tickVal: Die vergangene Zeit in Sekunden, die seit dem Letzten Aufruf dieser Funktion verstrichen ist
        //  return: 1, wenn das Bild neu gezeichnet werden muss. 0 sonnst
        __declspec( dllexport ) bool tick( double tickVal ) override;
        // Zeichnet den Hintergrund eines Zeichnunges nach rObj
        __declspec( dllexport ) void render( Bild &rObj ) override;

        // Gibt das Hintergrundbild zurück.
        //  return: 0, falls kein Hintergrundbild verwendet wird
        __declspec( dllexport ) Bild *getHintergrundBild() const;
        // Gibt das Hintergrundbild ohne erhöhten reference Counter zurück.
        //  return: 0, falls kein Hintergrundbild verwendet wird
        __declspec( dllexport ) Bild *zHintergrundBild() const;
        // Gibt die Hintergrundfarbe im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getHintergrundFarbe() const;
        // Gibt das AlphaFeld zurück, das über den Hintergrund gezeichnet wird.
        //  return: 0, falls das AlphaFeld nicht definiert wurde
        __declspec( dllexport ) AlphaFeld *getAlphaFeld() const;
        // Gibt das AlphaFeld ohne erhöhten Reference Counter zurück, das über den Hintergrund gezeichnet wird.
        //  return: 0, falls das AlphaFeld nicht definiert wurde
        __declspec( dllexport ) AlphaFeld *zAlphaFeld() const;
        // Git die Stärke des Alphafeldes zurück
        __declspec( dllexport ) int getAlphaFeldStärke() const;
        // Gibt die Farbe des Alphafedes im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getAlphaFeldFarbe() const;
        // Gibt den Rahmen zurück
        //  return: 0, falls kein Rahmen definiert wurde
        __declspec( dllexport ) LRahmen *getLinienRahmen() const;
        // Gibt den Rahmen ohne erhöhten Reference Counter zurück
        //  return: 0, falls kein Rahmen definiert wurde
        __declspec( dllexport ) LRahmen *zLinienRahmen() const;
        // Gibt die Breite des Rahmens in Pixeln zurück
        __declspec( dllexport ) int getLinienRahmenBreite() const;
        // Gibt die Farbe des Rahmens im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getLinienRahmenFarbe() const;
        // Gibt die Scroll geschwindigkeit der vertikalen Scroll Bar zurück
        __declspec( dllexport ) int getVertikalKlickScroll() const;
        // Gibt die Scroll Position der vertikalen Scroll Bar zurück
        __declspec( dllexport ) int getVertikalScrollPos() const;
        // Gibt die Farbe der vertikalen Scroll Bar im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getVertikalScrollFarbe() const;
        // Gibt die Hintergrundfarbe der vertikalen Scroll Bar im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getVertikalScrollHintergrund() const;
        // Gibt die Scroll geschwindigkeit der horizontalen Scroll Bar zurück
        __declspec( dllexport ) int getHorizontalKlickScroll() const;
        // Gibt die Scroll Position der horizontalen Scroll Bar zurück
        __declspec( dllexport ) int getHorizontalScrollPos() const;
        // Gibt die Farbe der horizontalen Scroll Bar im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getHorizontalScrollFarbe() const;
        // Gibt die Hintergrundfarbe der horizontalen Scroll Bar im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getHorizontalScrollHintergrund() const;
        // Erzeugt eine Kopie der Zeichnung, die ohne Auswirkungen auf das Original verändert werden kann
        __declspec( dllexport ) virtual Zeichnung *dublizieren() const;
    };
}

#endif