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

    // Eine Zeichnung für das 2D GUI Framework
    class Zeichnung
    {
    public:
        class Style
        {
        public:
            const static __int64 Sichtbar = 0x00001; // Wenn dieser Style gesetzt ist, wird die Zeichnung beim Zeichnen angezeigt
            const static __int64 Erlaubt = 0x00002; // Wenn dieser Style gesetzt ist, kann der Benutzer mit der Zeichnung interagieren
            const static __int64 Fokus = 0x00004; // Wenn dieser Style gesetzt ist, werden die Tastatur Ereignisse von der Zeichnung verarbeitet
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
        // Legt fest, ob sich die Zeichnung seit des letzten Bildes verändert hat und neu gezeichnet werden muss
        __declspec( dllexport ) void setRender();
        // Setzt den Text, der erscheint, wenn der Benutzer für längere Zeit mit der Maus in der Zeichnung verweilt
        //  txt: Der Text, der angezeigt werden soll
        //  zScreen: Ein Zeiger auf das Verwendete Bildschirm Objekt ohne erhöhten Reference Counter
        __declspec( dllexport ) void setToolTipText( const char *txt, Bildschirm *zScreen );
        // Dies ist notwendig, falls mehrere Threads gleichzeitig die Zeichnung benutzen.
        // Wenn lockZeichnung() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlockZeichnung() aufgerufen hat.
        __declspec( dllexport ) void lockZeichnung();
        // Dies ist notwendig, falls mehrere Threads gleichzeitig die Zeichnung benutzen.
        // Wenn lockZeichnung() von zwei threads aufgerufen wird, wartet der letzte so lange, bis der erste unlockZeichnung() aufgerufen hat.
        __declspec( dllexport ) void unlockZeichnung();
        // setzt den Parameter, der bei einem Maus Ereignis an die Rückruffunktion übergeben wird
        //  p: Der Parameter
        __declspec( dllexport ) void setMausEreignisParameter( void *p );
        // Setzt den Parameter, der bei einem Tastatur Ereignis an die Rückruffunktion übergeben wird
        //  p: Der Parameter
        __declspec( dllexport ) void setTastaturEreignisParameter( void *p );
        // Setzt die Rückruffunktion, die bei einem Maus Ereignis aufgerufen werden soll.
        // Wenn die Rückruffunktion 0 zurückgiebt, oder nicht gesetzt wurde, wird ein Maus Ereignis von der Zeichnung nicht weiter beachtet
        // Es kann die Standartfunktion __ret1ME verwendet werden, die in MausEreignis.h definiert ist und immer 1 zurückgibt
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setMausEreignis( bool( *ak )( void *, void *, MausEreignis ) );
        // Setzt die Rückruffunktion, die bei einem Tastatur Ereignis aufgerufen werdne soll.
        // Wenn die Rückruffunktion 0 zurückgiebt, oder nicht gesetzt wurde, wird ein Tastatur Ereignis von der Zeichnung nicht weiter beachtet
        // Es kann die Standartfunktion __ret1TE verwendet werden, die in TastaturEreignis.h definiert ist und immer 1 zurückgibt
        // Weitere Standartfunktionen sind _nurNummernTE und _nurHexTE ebenfals aus TastaturEreignis.h
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setTastaturEreignis( bool( *ak )( void *, void *, TastaturEreignis ) );
        // setzt den Parameter, der bei einem Maus Ereignis an die Rückruffunktion übergeben wird, die aufgerufen wird, fals das Ereignis von der Zeichnung verarbeitet wurde
        //  p: Der Parameter
        __declspec( dllexport ) void setNMausEreignisParameter( void *p );
        // Setzt den Parameter, der bei einem Tastatur Ereignis an die Rückruffunktion übergeben wird, die aufgerufen wird, fals das Ereignis von der Zeichnung verarbeitet wurde
        //  p: Der Parameter
        __declspec( dllexport ) void setNTastaturEreignisParameter( void *p );
        // Setzt die Rückruffunktion, die bei einem Maus Ereignis aufgerufen werden soll, nachdem das Ereignis bereits von der Zeichnung verarbeitet wurde
        // Wenn die Rückruffunktion 1 zurückgiebt, oder nicht gesetzt wurde, wird das Maus Ereignis von keiner weiteren Zeichnung verarbeitet, die zum Beispiel hinter dieser Zeichnung liegen
        // Es kann die Standartfunktion __ret1ME verwendet werden, die in MausEreignis.h definiert ist und immer 1 zurückgibt
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setNMausEreignis( bool( *ak )( void *, void *, MausEreignis ) );
        // Setzt die Rückruffunktion, die bei einem Tastatur Ereignis aufgerufen werdne soll, nachdem das Ereignis bereits von der Zeichnung verarbeitet wurde
        // Wenn die Rückruffunktion 1 zurückgiebt, oder nicht gesetzt wurde, wird das Tastatur Ereignis von keiner weiteren Zeichnung verarbeitet
        // Es kann die Standartfunktion __ret1TE verwendet werden, die in TastaturEreignis.h definiert ist und immer 1 zurückgibt
        // Weitere Standartfunktionen sind _nurNummernTE und _nurHexTE ebenfals aus TastaturEreignis.h
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setNTastaturEreignis( bool( *ak )( void *, void *, TastaturEreignis ) );
        // Verarbeitet ein Maus Ereignis. Wird vom Framework automatisch aufgerufen.
        //  me: Das Ereignis
        __declspec( dllexport ) virtual void doMausEreignis( MausEreignis &me );
        // Verarbeitet ein Tastatur Ereignis. Wird vom Framework automatisch aufgerufen
        //  te: Das Ereignis
        __declspec( dllexport ) virtual void doTastaturEreignis( TastaturEreignis &te );
        // Verarbeitet die Zeit, die seit dem letzten aufruf dieser Funktion vergangen ist
        //  tickVal: Die vergangene Zeit in Sekunden
        __declspec( dllexport ) virtual bool tick( double tickval );
        // Setzt die Position der Zeichnung
        //  pos: Die Position in Pixeln
        __declspec( dllexport ) void setPosition( const Punkt &pos );
        // Setzt die X Position der Zeichnung
        //  xPos: Die Position in Pixeln
        __declspec( dllexport ) void setX( int xPos );
        // Setzt die Y Position der Zeichnung
        //  yPos: Die Position in Pixeln
        __declspec( dllexport ) void setY( int yPos );
        // Setzt die Größe der Zeichnung
        //  gr: Ein Punkt mit x als Breite und y als Höhe in Pixeln
        __declspec( dllexport ) void setGröße( const Punkt &gr );
        // Setzt die Position der Zeichnung
        //  x: Die X Position in Pixeln
        //  y: Die Y Position in Pixeln
        __declspec( dllexport ) void setPosition( int x, int y );
        // Setzt die Größe der Zeichnung
        //  br: Die Breite in Pixeln
        //  hö: Die Höhe in Pixeln
        __declspec( dllexport ) void setGröße( int br, int hö );
        // Setzt den Style der Zeichnung
        //  style: Der neue Style bestehend aus den Flags aus der zugehörigen Style Klasse
        __declspec( dllexport ) void setStyle( __int64 style );
        // Setzt den Style der Zeichnung
        //  style: Alle Style Flags, die verändert werden sollen
        //  add_löschen: 1, falls der Style hinzugefügt werden soll. 0, falls der Style entfernt weden soll
        __declspec( dllexport ) void setStyle( __int64 style, bool add_löschen );
        // Fügt Style Flags hinzu
        //  style: Der Style, der hinzugefügt werden soll
        __declspec( dllexport ) void addStyle( __int64 style );
        // Entfernt Style Flags
        //  style: Der Style, der entfernt werden soll
        __declspec( dllexport ) void löscheStyle( __int64 style );
        // Zeichnet die Zeihnung in ein bestimmtes Bild
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) virtual void render( Bild &zRObj );
        // Gibt zurück, ob eine Rückruffunktion für Maus Ereignisse gesetzt wurde
        __declspec( dllexport ) bool hatMausEreignis() const;
        // Gibt zurück, ob eine Rückruffunktion für Tastatur Ereignisse gesetzt wurde
        __declspec( dllexport ) bool hatTastaturEreignis() const;
        // Gibt die Position der Zeichnung in Pixeln zurück
        __declspec( dllexport ) const Punkt &getPosition() const;
        // Gibt die Größe der Zeichnung in Pixeln zurück. x für Breite und y für Höhe
        __declspec( dllexport ) const Punkt &getGröße() const;
        // Gibt die Breite der Zeichnung in Pixeln zurück
        __declspec( dllexport ) int getBreite() const;
        // Gibt die Höhe der Zeichnung in Pixeln zurück
        __declspec( dllexport ) int getHöhe() const;
        // Gibt die X Position der Zeichnung in Pixeln zurück
        __declspec( dllexport ) int getX() const;
        // Gibt die Y Position der Zeichnung in Pixeln zurück
        __declspec( dllexport ) int getY() const;
        // Gibt einen Zeiger auf das Tooltip Objekt zurück, walls es verwendet wird
        __declspec( dllexport ) ToolTip *getToolTip() const;
        // Gibt einen Zeiger auf das Tooltip Objekt ohne erhöhten Reference Counter zurück, walls es verwendet wird
        __declspec( dllexport ) ToolTip *zToolTip() const;
        // Gibt zurück, ob bestimmte Styles gesetzt wurden
        //  style: Die Styles, die überprüft werden sollen
        //  return: 1, falls alle Styles in style gesetzt wurden
        __declspec( dllexport ) inline bool hatStyle( __int64 style ) const;
        // Gibt zurück, ob bestimmte Styles nicht gesetzt wurden
        //  style: Die Styles, die geprüft werden sollen
        //  return: 1, falls alle Styles in style nicht gesetzt wurden
        __declspec( dllexport ) inline bool hatStyleNicht( __int64 style ) const;
        // Kopiert die Komplette Zeichnung, so dass sie ohne Effekt auf das Original verändert werden kann
        __declspec( dllexport ) virtual Zeichnung *dublizieren() const;
    };

    // Ein Array von Zeichnungen, der von den Bildschirm Klassen verwendet wird, um die Objekte der GUI zu speichern
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
        // Fügt dem Array eine Zeichnung hinzu. Es kann jede Zeichnung nur einmal hinzugefügt werden
        //  obj: Die neue Zeichnung
        //  return: 1, falls die Zeichnung erfolgreich hinzugefügt wurde.
        __declspec( dllexport ) bool addZeichnung( Zeichnung *obj );
        // Entfernt eine Zeichnung
        //  obj: Die Zeichnung, die entfernt werden soll
        //  return 1, falls die Zeichnung erfolgreich entfernt wurde
        __declspec( dllexport ) bool removeZeichnung( Zeichnung *obj );
        // Entfernt eine Zeichnung
        //  i: Der Index der Zeichnung, die entfernt werden soll
        //  return 1, falls die Zeichnung erfolgreich entfernt wurde
        __declspec( dllexport ) bool removeZeichnung( int i );
        // Setzt den Zeiger auf das Nächste Array Element auf 0
        __declspec( dllexport ) void setNext0();
        // Aktualisiert die Array indizes.
        //  i: Der Index des ersten Elements
        __declspec( dllexport ) void updateIndex( int i );
        // Gibt den Zeiger auf das nchste Array Element zurück
        __declspec( dllexport ) ZeichnungArray *getNext() const;
        // Gibt eine Zeichnung zurück
        //  i: Der Index der Zeichnung
        __declspec( dllexport ) Zeichnung *getZeichnung( int i ) const;
        // Gibt die Zeichnung dieses Array Elements zurück
        __declspec( dllexport ) Zeichnung *getZeichnung() const;
        // Gibt den Index dieses Array Elements zurück
        __declspec( dllexport ) int getIndex() const;
        // Sendet an alle Zeichnungen ein Maus Ereignis. Die zuletzt hinzugefügte Zeichnung bekommt das Ereignis als Erste
        //  me: Das Ereignis
        __declspec( dllexport ) void sendMausAll( MausEreignis &me ) const;
        // Sendet an alle Zeichnungen ein Tastatur Ereignis. Die zuletzt hinzugefügte Zeichnung bekommt das Ereignis als Erste
        //  te: Das Ereignis
        __declspec( dllexport ) void sendTastaturAll( TastaturEreignis &te ) const;
        // Zeichnet alle Zeichnungen. Die zuletzt hinzugefügte Zeichnung ist oben
        //  zRObj: Das Bild, in das alle Zeichnungen gezeichnet werden sollen
        __declspec( dllexport ) void render( Bild &zRObj );
        // Verarbeitet bei allen Zeichnungen die Vergangene Zeit seit dem letzten Aufruf der Funktion
        //  tickVal: Die vergangene Zeit in Sekunden
        __declspec( dllexport ) bool tick( double tickval );
    };

    // Eine Zeichnung für das 2D GUI Framework mit Hintergrund, Rahmen und Scroll Balken
    class ZeichnungHintergrund : public Zeichnung
    {
    public:
        class Style : public Zeichnung::Style
        {
        public:
            const static __int64 Rahmen = 0x00010; // Wenn dieser Flag gesetzt wird, bekommt die Zeichnugn einen Rahmen
            const static __int64 Hintergrund = 0x00020; // Wenn dieser Flag gesetzt wird, bekommt die Zeichnung einen Hintergrund
            const static __int64 HAlpha = 0x00040; // Wenn dieser Flag gesetzt wird, wird der Hintergrund durchsichtig. Benötigt Flag Hintergrund
            const static __int64 HBild = 0x00080; // Wenn dieser Flag gesetzt wird, wird ein Bild als hintergrund verwendet. Benötigt Flag Hintergrund
            const static __int64 Buffered = 0x00100; // Wenn dieser Flag gesetzt wird, Erscheint ein Farbübergang als Rahmen
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