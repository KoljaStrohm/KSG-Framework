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
#ifdef WIN32
    // Erzeugt eine normale Fensterklasse der Windows API
    //  hInst: Die HINSTANCE des Programms (Wird vom Framework an die Start funktion in der Startparam Struktur übergeben)
    __declspec( dllexport ) WNDCLASS F_Normal( HINSTANCE hInst );
    // Erzeugt eine normale Fensterklasse der Windows API
    //  hInst: Die HINSTANCE des Programms (Wird vom Framework an die Start funktion in der Startparam Struktur übergeben)
    __declspec( dllexport ) WNDCLASSEX F_NormalEx( HINSTANCE hInst );
    // Funktion des Frameworks, die alle Nachichten von Windows oder anderen Prozessen verarbeitet
    __declspec( dllexport ) LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
    // Startet eine Schleife, die die Benutzereingaben verarbeitet. Die Schleife läuft, bis irgendwo im Programm StopNachrichtenSchleife aufgerufen wird
    __declspec( dllexport ) void StartNachrichtenSchleife();
    // Stoppt die Ausführung der Nachrichten Schleife des Frameworks.
    //  hwnd: Ein Handle auf ein beliebiges Fenster des Frameworks, das Nachrichten empfangen kann. Wird benötigt, um eine Nachricht zu senden, damit die funktion StartNachrichtenSchleife nicht mehr wartet und sofort beendet wird
    __declspec( dllexport ) void StopNachrichtenSchleife( HWND hwnd );
    // Übersetzt einen Keycode, der von Windows gesendet wurde in den Buchstaben der gedrückten Taste
    __declspec( dllexport ) unsigned char VirtualZuChar( int Virtual );

    // Klasse für ein Fenster der Windows API
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
        //--Konstruktor--
        //  hwnd: Ein Handle zum Fenster, das von dieser Klasse verwaltet werden soll
        __declspec( dllexport ) WFenster( HWND hWnd );
        //--Destruktor--
        __declspec( dllexport ) ~WFenster();
        // erstellt das Fenster
        //  style: Der Style des Fensters. Beispiel: WS_OVERLAPPEDWINDOW (Für ein normales Fenster wie man es kennt)
        //  wc: Die Fensterklasse der Windows API, von der ein Fenster erstellt werden soll. Kann mit der Funktion F_Normal erstellt werden. Es muss lpszClassName gesetzt worden sein.
        __declspec( dllexport ) void erstellen( int style, WNDCLASS wc );
        // erstellt das Fenster
        //  exStyle: Der EX Style des Fensters. Beispiel: WS_EX_OVERLAPPEDWINDOW (Für ein normales Fenster wie man es kennt)
        //  style: Der Style des Fensters. Beispiel: WS_OVERLAPPEDWINDOW (Für ein normales Fenster wie man es kennt)
        //  wc: Die Fensterklasse der Windows API, von der ein Fenster erstellt werden soll. Kann mit der Funktion F_Normal erstellt werden. Es muss lpszClassName gesetzt worden sein.
        __declspec( dllexport ) void erstellenEx( int exStyle, int style, WNDCLASSEX wc );
        // Setzt den Anzeigemodus des Fensters
        //  mod: Der Modus. Beispiel: SW_SHOWNORMAL, um das Fenster anzuzeigen und SW_HIDE um es einzuklappen
        __declspec( dllexport ) void setAnzeigeModus( int mod );
        // Setzt den Fokus auf das Fenster, so dass Tastatureingaben empfangen werden
        __declspec( dllexport ) bool setFokus();
        // Setzt die Position des fensters auf dem Bildschirm
        //  pos: Die Position in Pixeln
        __declspec( dllexport ) void setPosition( Punkt &pos );
        // Setzt die Größe des Fensters auf dem Bildschirm
        //  größe: Die Größe in Pixeln
        __declspec( dllexport ) void setSize( Punkt &größe );
        // Setzt die Größe des Fensters auf dem Bildschirm
        //  breite: Die Breite in Pixeln
        //  höhe: Die Höhe in Pixeln
        __declspec( dllexport ) void setSize( int breite, int höhe );
        // Setzt die Position und die Größe des Fensters
        //  pos: Die Position in Pixeln
        //  größe: Die Größe in Pixeln
        __declspec( dllexport ) void setBounds( Punkt &pos, Punkt &größe );
        // Setzt das Verwendete Bildschirm Objekt, um Tastatur und Maus Eingaben an die Objekte des Frameworks weiterzugeben. Muss vor dem Zerstören des Fensters mit 0 aufgerufen werden.
        //  screen: Das Bildschirm Objekt
        __declspec( dllexport ) void setBildschirm( Bildschirm *screen );
        // Zerstört das Fenster
        __declspec( dllexport ) void zerstören();
        // Verarbeitet Maus Nachrichten. Ruft MausAktion auf und gibt die Ereignisse an den Bildschirm mit den Objekten weiter, wenn MausAktion 1 zurückgibt
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausAktion( MausEreignis &me );
        // Ruft die VSchließAktion Rückruffunktion auf
        __declspec( dllexport ) void doVSchließAktion();
        // Ruft die NSchließAktion Rückruffunktion auf
        __declspec( dllexport ) void doNSchließAktion();
        // Verarbeitet Tastatur Nachrichten. Ruft TastaturAktion auf und gibt die Ereignisse an den Bildschirm mit den Objekten weiter, wenn TastaturAktion 1 zurückgibt
        //  me: Das Ereignis, was durch die Tastatureingabe ausgelößt wurde
        __declspec( dllexport ) void doTastaturAktion( TastaturEreignis &et );
        // Macht den Rahmen des Fensters sichtbar, fals ladeRahmenFenster aufgerufen wurde
        __declspec( dllexport ) void doRestoreMessage();
        // setzt den Parameter, der bei einem Maus Ereignis an die Rückruffunktion übergeben wird
        //  p: Der Parameter
        __declspec( dllexport ) void setMausEreignisParameter( void *p );
        // setzt den Parameter, der beim Schließen an die Rückruffunktion übergeben wird
        //  p: Der Parameter
        __declspec( dllexport ) void setSchließEreignisParameter( void *p );
        // Setzt den Parameter, der bei einem Tastatur Ereignis an die Rückruffunktion übergeben wird
        //  p: Der Parameter
        __declspec( dllexport ) void setTastaturEreignisParameter( void *p );
        // Setzt die Rückruffunktion, die bei einem Maus Ereignis aufgerufen werden soll.
        // Wenn die Rückruffunktion 0 zurückgiebt, oder nicht gesetzt wurde, wird ein Maus Ereignis von dem Fenster nicht weiter beachtet
        // Es kann die Standartfunktion __ret1ME verwendet werden, die in MausEreignis.h definiert ist und immer 1 zurückgibt
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setMausAktion( bool( *MausAk )( void *, void *, MausEreignis ) );
        // Setzt die Rückruffunktion, die bei vor dem Schließen aufgerufen werden soll.
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setVSchließAktion( void( *vSchließAk )( void *, void * ) );
        // Setzt die Rückruffunktion, die bei nach dem Schließen aufgerufen werden soll.
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setNSchließAktion( void( *nSchließAk )( void *, void * ) );
        // Setzt die Rückruffunktion, die bei einem Tastatur Ereignis aufgerufen werdne soll.
        // Wenn die Rückruffunktion 0 zurückgiebt, oder nicht gesetzt wurde, wird ein Tastatur Ereignis von der Zeichnung nicht weiter beachtet
        // Es kann die Standartfunktion __ret1TE verwendet werden, die in TastaturEreignis.h definiert ist und immer 1 zurückgibt
        // Weitere Standartfunktionen sind _nurNummernTE und _nurHexTE ebenfals aus TastaturEreignis.h
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setTastaturAktion( bool( *TastaturAk )( void *, void *, TastaturEreignis ) );
        //  Setzt das Handle zum Fenster, das von dieser Klasse verwaltet werden soll
        //  hwnd: Das Handle
        __declspec( dllexport ) void setFensterHandle( HWND hWnd );
        // legt fest, ob das Fenster durch ziehen mit Maus verschoben werden kann
        //  verschiebbar: 1, wenn das Fenster verschoben werden darf
        __declspec( dllexport ) void setVerschiebbar( bool verschiebbar );
        // Setzt einen Transparenten Rahmen um das Fenster
        //  zBild: Ein Bild, was den Rahmen enthält
        //  hins: Die HINSTANCE des Programms (Wird vom Framework an die Start funktion in der Startparam Struktur übergeben)
        __declspec( dllexport ) void ladeRahmenFenster( Bild *zBild, HINSTANCE hinst ); // setzt einen Transpatenten Rahmen um das Fenster
        // Gibt das Handle des verwalteten Fensters zurück
        __declspec( dllexport ) HWND getFensterHandle() const;
        // Gibt die Position des Fensters in Pixeln zurück
        __declspec( dllexport ) Punkt getPosition() const;
        // Gibt die Größe des Fensters in Pixeln zurück
        __declspec( dllexport ) Punkt getGröße() const;
        // Gibt die Größe des Fensterkörpers in Pixeln zurück
        __declspec( dllexport ) Punkt getKörperGröße() const;
        // Gibt die Breite des Fensterkörpers in Pixeln zurück
        __declspec( dllexport ) int getKörperBreite() const;
        // Gibt die Höhe des Fensterkörpers in Pixeln zurück
        __declspec( dllexport ) int getKörperHöhe() const;
        // Gibt zurück, ob eine Rückruffunktion für ein MausEreignis gesetzt wurde
        __declspec( dllexport ) bool hatMausAktion() const;
        // Gibt zurück, ob eine Rückruffunktion für das Ereignis vor dem Schließen des Fensters gesetzt wurde
        __declspec( dllexport ) bool hatVSchließAktion() const;
        // Gibt zurück, ob eine Rückruffunktion für das Ereignis nach dem Schließen des Fensters gesetzt wurde
        __declspec( dllexport ) bool hatNSchließAktion() const;
        // Gibt zurück, ob eine Rückruffunktion für ein TastaturEreignis gesetzt wurde
        __declspec( dllexport ) bool hatTastaturAktion() const;
        // Gibt den Bidschirm zurück, an dessen Zeichnungen die Ereignisse Weitergegeben werden
        __declspec( dllexport ) Bildschirm *getBildschirm() const;
        // Gibt den Bildschirm ohne erhöhten Reference Counter zurück, an dessen Zeichnungen die Ereignisse Weitergegeben werden
        __declspec( dllexport ) Bildschirm *zBildschirm() const;
        // Gibt zurück, ob das Fenster verschiebbar ist
        __declspec( dllexport ) bool istVerschiebbar() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) WFenster *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) WFenster *release();
    };

    // Verwaltet alle Windows API Fenster im Framework
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
        // Fügt ein neues Fenster hinzu
        //  fenster: Das Fenster
        __declspec( dllexport ) bool addFenster( WFenster *fenster );
        // Entfernt ein Fenster
        //  fenster: Das Fenster
        __declspec( dllexport ) bool removeFenster( WFenster *fenster );
        // gibt das nächste Element zurück
        __declspec( dllexport ) WFensterArray* getNext();
        // Setzt das nächste Element auf 0
        __declspec( dllexport ) void setNext0();
        // Löscht das Element
        __declspec( dllexport ) void del();
        // Sendet das Ereignis, was vor dem Schließen aufgerufen wird an ein bestimmtes Fenster
        //  hWnd: Das Handle zum Fenster
        __declspec( dllexport ) bool sendVSchließMessage( HWND hWnd );
        // Sendet das Ereignis, was nach dem Schließen aufgerufen wird an ein bestimmtes Fenster
        //  hWnd: Das Handle zum Fenster
        __declspec( dllexport ) bool sendNSchließMessage( HWND hwnd );
        // Sendet ein Maus Ereignis an ein bestimmtes Fenster
        //  hWnd: Das Handle zum Fenster
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) bool sendMausMessage( HWND hWnd, MausEreignis &me );
        // Sendet ein Tastatur Ereignis an ein bestimmtes Fenster
        //  hWnd: Das Handle zum Fenster
        //  me: Das Ereignis, was durch die Tastatureingabe ausgelößt wurde
        __declspec( dllexport ) bool sendTastaturMessage( HWND hwnd, TastaturEreignis &te );
        // Sendet das Ereignis, was durch das öffnen des Fensters ausgelößt wurde an ein bestimmtes Fenster
        //  hWnd: Das Handle zum Fenster
        __declspec( dllexport ) bool sendRestoreMessage( HWND hwnd );
        // Gibt das Fenster dieses Eintrags zurück
        __declspec( dllexport ) WFenster *getThis();
    };

    // Erzeugt ein Windows API Popup Fenster, mit einer Meldung
    //  hWnd: Ein Handle zu dem Fenster, was blockiert werden soll, bis das Popup Fenster geschlossen wurde. Kann 0 sein
    //  titel: Der Titel des Popup Fensters
    //  meldung: Die Meldung, die im Fenster angezeigt werden soll
    //  style: Bestimmt das Icon, was im Fenster angezeigt wird. Beispiel: MB_ICONERROR, MB_ICONINFORMATION
    __declspec( dllexport ) void WMessageBox( HWND hWnd, Text *titel, Text *meldung, UINT style );
#endif
    // Fenster Klasse im Programm
    class Fenster : public Zeichnung
    {
    public:
        class Style : public Zeichnung::Style
        {
        public:
            const static __int64 BodyHintergrund = 0x000000008; // Legt fest, ob der Körper des Fensters einen Hintergrund hat
            const static __int64 BodyHAlpha = 0x000000010; // Legt fest, ob beim zeichnen des Körperhintergrundes alpha blending verwendet werden soll
            const static __int64 BodyHBild = 0x000000020; // Legt fest, ob ein Bild als Hintergrund des Körpers verwendet werden soll
            const static __int64 BodyBuffered = 0x000000040; // Legt fest, ob der Körper einen Farbübergang besitzt
            const static __int64 Titel = 0x000000080; // Legt fest, ob das Fenster eine Titelleiste hat
            const static __int64 TitelHintergrund = 0x000000100; // Legt fest, ob die titelleiste des Fensters einen Hintergrund hat
            const static __int64 TitelHAlpha = 0x000000200; // Legt fest, ob zum zeichnen des Titel Hintergrundes alpha blending verwendet werden soll
            const static __int64 TitelHBild = 0x000000400; // Legt fest, ob für den Titelhintergrund ein Bild verwendet werden soll
            const static __int64 TitelBuffered = 0x000000800; // Legt fest, ob die Titel Leiste einen Farbübergang besitzt
            const static __int64 Closable = 0x000001000; // Legt fest, ob in der Titelleiste ein Knopf zum Schließen des Fensters angezeigt werden soll
            const static __int64 ClosingHintergrund = 0x000002000; // Legt fest, ob der Schließen Knopf einen Hintergrund hat
            const static __int64 ClosingHAlpha = 0x000004000; // Legt fest, ob beim Zeichnen des Hintergrunds des Schließen Knopfes alpha blending verwendet werden soll
            const static __int64 ClosingHBild = 0x000008000; // Legt fest, ob für den Hintergrund des Schließen Knopfes ein Bild verwendet werden soll
            const static __int64 ClosingBuffer = 0x000010000; // Legt fest, ob der Schließen Knopf einen Farbübergang besitzt
            const static __int64 ClosingKlickBuffer = 0x000020000; // Legt fest, ob der Schließen Knopf einen Farbübergang besitzt, während er gedrückt wird
            const static __int64 Beweglich = 0x000040000; // Legt fest, ob der Benutzer das Fenster durch gedrückt halten der linken Maustaste in der Titelleiste das Fenster verschieben kann
            const static __int64 BreiteChangeable = 0x000080000; // Legt fest, ob der Benutzer die Breite des Fensters durch das gedrückt halten der linken Maustaste auf dem rechten oder linken Fensterrand verändern kann
            const static __int64 HeightChangeable = 0x000100000;  // Legt fest, ob der Benutzer die Höhe des Fensters durch das gedrückt halten der linken Maustaste auf dem oberen oder unteren Fensterrand verändern kann
            const static __int64 TitelHeightChangeable = 0x000200000; // Legt fest, ob der Benutzer die Höhe der Titel Leiste durch gedrückt halten der linken Maustaste auf dem unteren Rand der Titelleiste verändern kann
            const static __int64 MinBr = 0x000400000; // Legt fest, ob es eine Minimale Breite des Fensters gibt
            const static __int64 MaxBr = 0x000800000; // Legt fest, ob es eine Maximale Breite des Fensters gibt
            const static __int64 MinHi = 0x001000000; // Legt fest, ob es eine Minimale Höhe des Fensters gibt
            const static __int64 MaxHi = 0x002000000; // Legt fest, ob es eine Maximale Höhe des Fensters gibt
            const static __int64 BodyMinBr = 0x004000000; // Legt fest, ob es eine Minimale Breite des Körpers gibt
            const static __int64 BodyMaxBr = 0x008000000; // Legt fest, ob es eine Maximale Breite des Körpers gibt
            const static __int64 BodyMinHi = 0x010000000; // Legt fest, ob es eine Minimale Höhe des Körpers gibt
            const static __int64 BodyMaxHi = 0x020000000; // Legt fest, ob es eine Maximale Höhe des Körpers gibt
            const static __int64 VScroll = 0x040000000; // Legt fest, ob eine ScrollBar am rechten Fensterrand erscheinen soll
            const static __int64 HScroll = 0x080000000; // Legt fest, ob eine ScrollBar am unteren Fensterrand erscheinen soll
            const static __int64 METransparenz = 0x100000000; // Legt fest, ob die Mausereignisse auch noch von Zeichnungen hinter dem Fenster verarbeitet werden sollen
            const static __int64 Rahmen = 0x200000000; // Legt fest, ob das Fenster einen Rahmen haben soll

            const static __int64 min_max = MinHi | MaxHi | MaxBr | MaxHi; // Vereint die Flags MinHö, MaxHö, MaxBr, MaxHö
            const static __int64 body_min_max = BodyMinBr | BodyMaxBr | BodyMinHi | BodyMaxBr; // Vereint die Flags Körper_minBr, Körper_maxBr, Körper_minHö, Körper_maxBr
            const static __int64 scroll = VScroll | HScroll; // Vereint die Flags VScroll, HScroll
            const static __int64 nichtfixiert = TitelHeightChangeable | HeightChangeable | BreiteChangeable | Beweglich; // Vereint die Flags TitelHöheÄnderbar, HöheÄnderbar, BreiteÄnderbar, Beweglich

            const static __int64 normal = Sichtbar | Erlaubt | Rahmen | Titel | TitelBuffered | Closable | ClosingHBild | ClosingKlickBuffer | Beweglich; // Vereint die Flags Sichtbar, Erlaubt, Rahmen, Titel, TitelBuffered, Schließbar, SchließHBild, SchließKlickBuffer, Beweglich
        };
    private:
        bool( *closingMe )( void *, void *, MausEreignis );
        void *closingMeParam;
        LRahmen *rahmen;
        TextFeld *titel;
        ZeichnungArray *members;
        int bgBodyColor;
        Bild *bgBodyPicture;
        AlphaFeld *bodyBuffer;
        int bgClosingFarbe;
        Bild *bgClosingBild;
        AlphaFeld *closeBuffer;
        AlphaFeld *closeKlickBuffer;
        VScrollBar *vScroll;
        HScrollBar *hScroll;
        Punkt min, max;
        Punkt kMin, kMax;
        bool closeKlick, klick;
        int mx, my;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) Fenster();
        // Destruktor 
        __declspec( dllexport ) virtual ~Fenster();
        // Setzt einen Zeiger auf den Rahmen des Fensters
        //  ram: Der Rahmen
        __declspec( dllexport ) void setRahmenZ( LRahmen *ram );
        // Setzt die Farbe des Fensterrahmens
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setRFarbe( int f );
        // Setzt die Breite des Fensterrahmens
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setRBreite( int br );
        // Setzt den Titel des Fensters
        //  txt: Der Text
        __declspec( dllexport ) void setTitel( Text *txt );
        // Setzt einen Zeiger auf den Titel Text
        //  txt: Der neue Text
        __declspec( dllexport ) void setTitelZ( Text *txt );
        // Setzt den Titel des Fensters
        //  txt: Der Textv
        __declspec( dllexport ) void setTitel( const char *txt );
        // Setzt einen Zeiger auf das TextFeld, das den Titeltext zeichnet
        //  tf: Das TextFeld
        __declspec( dllexport ) void setTTextFeldZ( TextFeld *tf );
        // Setzt die Schrift, die für den Titel verwendet werden soll
        //  schrift: Die Schrift
        __declspec( dllexport ) void setTSchriftZ( Schrift *schrift );
        // Setzt die Farbe der Schrift, die für den Titel verwendet werden soll
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setTSFarbe( int f );
        // Setzt die Größe der Schrift, die für den Titel verwendet werden soll
        //  gr: Die Höhe einer zeile in Pixeln
        __declspec( dllexport ) void setTSSize( int gr );
        // Setzt die Hintergrund Farbe des Titels
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setTBgFarbe( int f );
        // Setzt einen Zeiger auf den Farbübergang des Titels
        //  af: Der Farbübergang
        __declspec( dllexport ) void setTAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs des Titels
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setTAfFarbe( int f );
        // Setzt die Stärke des Farbübergangs des Titels
        //  st: Die Stärke
        __declspec( dllexport ) void setTAfStrength( int st );
        // Setzt das Hintergrund Bild des Titels durch kopieren
        //  b: Das Bild, was kopiert werden soll
        __declspec( dllexport ) void setTBgBild( Bild *b );
        // Setzt einen Zeiger auf das Hintergrund Bild des Titels
        //  b: Das Bild
        __declspec( dllexport ) void setTBgBildZ( Bild *b );
        // Setzt einen Zeiger auf den Rahmen des Titels
        //  ram: Der Rahmen
        __declspec( dllexport ) void setTRahmenZ( LRahmen *ram );
        // Setzt die Farbe des Rahmens des Titels
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setTRFarbe( int f );
        // Setzt die Breite des Rahmens des Titels
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setTRBreite( int br );
        // Setzt die Hintergrundfarbe des Körpers 
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKBgFarbe( int f );
        // Setzt das Hintergrund Bild des Körpers durch kopieren
        //  b: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setKBgBild( Bild *b );
        // Setzt einen Zeiger auf das Hintergrund Bild des Körpers
        //  b: Das Bild
        __declspec( dllexport ) void setKBgBildZ( Bild *b );
        // Setzt einen Zeiger auf den Farbübergang des Körpers
        //  af: Der Farbübergang
        __declspec( dllexport ) void setKAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs des Körpers
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKAfFarbe( int f );
        // Setzt die Stärke des Farbübergangs des Körpers
        //  st: Die Stärke
        __declspec( dllexport ) void setKAfStrength( int st );
        // Setzt den Parameter der Rückruffunktion, die aufgerufen wird, wenn der Schließen Knopf ein MausEreignis erhält
        //  param: Der Parameter
        __declspec( dllexport ) void setClosingMeParam( void *param );
        // Setzt die Rückruffunktion, die Aufgerufen wird, wenn der Schließen Knopf ein MausEreignis erhält
        // Wenn die Rückruffunktion 0 zurückgiebt, oder nicht gesetzt wurde, wird ein Maus Ereignis von der Zeichnung nicht weiter beachtet
        // Das Fenster wird nicht von selbst geschlossen, sondern sollte in der Rückruffunktion durch den aufruf von löscheStyle( Fenster::Style::Sichtbar ); geschlossen werden
        //  ak: Ein Zeiger auf die Rückruffunktion
        __declspec( dllexport ) void setClosingMe( bool( *closingMe )( void *, void *, MausEreignis ) );
        // Setzt die Hintergrund Farbe des Schließen Knopfes
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSBgFarbe( int f );
        // Setzt das Hintergrund Bild des Schließen Knopfes durch kopieren
        //  b: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setSBgBild( Bild *b );
        // Setzt einen Zeiger auf das Hintergrund Bild des Schließen Knopfes
        //  b: Das Bild
        __declspec( dllexport ) void setSBgBildZ( Bild *b );
        // Setzt einen Zeiger auf den Farbübergang des Schließen Knopfes
        //  af: Der Farbübergang
        __declspec( dllexport ) void setSAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs des Schließen Knopfes
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSAfFarbe( int f );
        // Setzt die Stärke des Farbübergangs des Schließen Knopfes
        //  st: Die Stärke
        __declspec( dllexport ) void setSAfStrength( int st );
        // Setzt einen Zeiger auf den Farbübergang, der beim Klicken des Schließen Knopfes verwendet wird
        //  af: Der Farbübergnag
        __declspec( dllexport ) void setSKAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs, der beim Klicken des Schließen Knopfes verwendet wird
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSKAfFarbe( int f );
        // Setzt die Stärke des Farbübergangs, der beim Klicken des Schließen Knopfes verwendet wird
        //  st: Die Stärke
        __declspec( dllexport ) void setSKAfStrength( int st );
        // Setzt die Minimale Größe des Fensters
        //  mx: Die Minimale Breite in Pixeln
        //  my: Die Minimale Höhe in Pixeln
        __declspec( dllexport ) void setMin( int mx, int my );
        // Setzt die Minimale Größe des Fensters
        //  min: Die Minimale Breite und Höhe in Pixeln
        __declspec( dllexport ) void setMin( const Punkt &min );
        // Setzt die Maximale Größe des Fensters
        //  mx: Die Maximale Breite in Pixeln
        //  my: Die Maximale Höhe in Pixeln
        __declspec( dllexport ) void setMax( int mx, int my );
        // Setzt die Maximale Größe des Fensters
        //  min: Die Maximale Breite und Höhe in Pixeln
        __declspec( dllexport ) void setMax( const Punkt &max );
        // Setzt die Minimale Größe des Fenster Körpers
        //  mx: Die Minimale Breite in Pixeln
        //  my: Die Minimale Höhe in Pixeln
        __declspec( dllexport ) void setKMin( int mx, int my );
        // Setzt die Minimale Größe des Fenster Körpers
        //  min: Die Minimale Breite und Höhe in Pixeln
        __declspec( dllexport ) void setKMin( const Punkt &min );
        // Setzt die Maximale Größe des Fenster Körpers
        //  mx: Die Maximale Breite in Pixeln
        //  my: Die Maximale Höhe in Pixeln
        __declspec( dllexport ) void setKMax( int mx, int my );
        // Setzt die Maximale Größe des Fenster Körpers
        //  min: Die Maximale Breite und Höhe in Pixeln
        __declspec( dllexport ) void setKMax( const Punkt &max );
        // Setzt einen Zeiger auf die Scrollbar am unteren Rand des Fensters
        //  hScroll: Die Scrollbar
        __declspec( dllexport ) void setHScrollBarZ( HScrollBar *hScroll );
        // Setzt einen Zeiger auf die Scrollbar am rechten Rand des Fensters
        //  vScroll: Die Scrollbar
        __declspec( dllexport ) void setVScrollBarZ( VScrollBar *vScroll );
        // Setzt die Maximale Scroll breite des Fenster Körpers
        //  max: Die Breite in Pixeln
        __declspec( dllexport ) void setHSBMax( int max );
        // Setzt die Maximale Scroll höhe des Fenster Körpers
        //  max: Die Höhe in Pixeln
        __declspec( dllexport ) void setVSBMax( int max );
        // Scrollt zu einer bestimmten x Position im Fenster Körper
        //  scroll: Die Anzahl der Pixel, die der Inhalt nach links verschoben werden soll
        __declspec( dllexport ) void setHSBScroll( int scroll );
        // Scrollt zu einer bestimmten y Position im Fenster Körper
        //  scroll: Die Anzahl der Pixel, die der Inhalt nach oben verschoben werden soll
        __declspec( dllexport ) void setVSBScroll( int scroll );
        // Fügt dem Fenster eine Zeichnung hinzu
        //  zObj: Die Zeichnung
        __declspec( dllexport ) void addMember( Zeichnung *zObj );
        // Entfernt eine Zeichnung aus dem Fenster
        //  zObj: Die Zeichnung
        __declspec( dllexport ) void removeMember( Zeichnung *zObj );
        // Aktualisiert das Objekt. Wird vom Framework aufgerufen
        //  tickVal: Die Zeit in sekunden, die seit dem lezten Aufruf dieser Funktion vergangen ist
        //  return: 1, wenn sich etwas verändert hat und das Bild neu gezeichnet werden muss. 0 sonst
        __declspec( dllexport ) bool tick( double tickval ) override;
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Verarbeitet Tastatur Nachrichten
        //  me: Das Ereignis, was durch die Tastatureingabe ausgelößt wurde
        __declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
        // Zeichnet das Fentster nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt den Rahmen des Fensters zurück
        __declspec( dllexport ) LRahmen *getRahmen() const;
        // Gibt den Rahmen des Fensters ohne erhöhen Reference Counter zurück
        __declspec( dllexport ) LRahmen *zRahmen() const;
        // Gibt die Farbe des Rahmens des Fensters im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getRFarbe() const;
        // Gibt die Breite des Rahmens des Fensters in Pixeln zurück
        __declspec( dllexport ) int getRBreite() const;
        // Gibt den Titel des Fensters zurück
        __declspec( dllexport ) Text *getTitel() const;
        // Gibt den Titel des Fensters ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Text *zTitel() const;
        // Gibt das TextFeld zurück, das zum Zeichnen des Titels verwendet wird
        __declspec( dllexport ) TextFeld *getTTextFeld() const;
        // Gibt das TextFeld ohne erhöhten Reference Counter zurück, das zum Zeichnen des Titels verwendet wird
        __declspec( dllexport ) TextFeld *zTTextFeld() const;
        // Gibt die Schrift zurück, die für den Titel verwendet wird
        __declspec( dllexport ) Schrift *getTSchrift() const;
        // Gibt die Schrift ohne erhöten Reference Counter zurück, die für den Titel verwendet wird
        __declspec( dllexport ) Schrift *zTSchrift() const;
        // Gibt die Schrift Farbe des Titels im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getTSFarbe() const;
        // Gibt die Höhe einer Zeile des Titels in Pixeln zurück
        __declspec( dllexport ) int getTSSize() const;
        // Gibt die Hintergrundfarbe des Titels im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getTBgFarbe() const;
        // Gibt den Farbübergang des Titels zurück
        __declspec( dllexport ) AlphaFeld *getTAlphaFeld() const;
        // Gibt den Farbübergang des Titels ohne erhöhten Reference COunter zurück
        __declspec( dllexport ) AlphaFeld *zTAlphaFeld() const;
        // Gibt die Farbe des Farbübergangs des Titels im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getTAfFarbe() const;
        // Gibt die Stärke des Farbübergangs des Titels zurück
        __declspec( dllexport ) int getTAfStrength() const;
        // Gibt das Hintergrund Bild des titels zurück
        __declspec( dllexport ) Bild *getTBgBild() const;
        // Gibt das Hintergrund Bild des titels ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Bild *zTBgBild() const;
        // Gibt den Rahmen des Titels zurück
        __declspec( dllexport ) LRahmen *getTRahmen() const;
        // Gibt den Rahmen des Titels ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) LRahmen *zTRahmen() const;
        // Gibt die Farbe des Rahmens des Titels im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getTRFarbe() const;
        // Gibt die Stärke des Rahmens des Titels zurück
        __declspec( dllexport ) int getTRBreite() const;
        // Gibt die Hintergrund Farbe des Körpers zurück
        __declspec( dllexport ) int getKBgFarbe() const;
        // Gibt das Hintergrund Bild des Körpers zurück
        __declspec( dllexport ) Bild *getKBgBild() const;
        // Gibt das Hintergrund Bild des Körpers ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Bild *zKBgBild() const;
        // Gibt den Farbübergang des Körpers zurück
        __declspec( dllexport ) AlphaFeld *getKAlphaFeld() const;
        // Gibt den Farbübergang des Körpers ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) AlphaFeld *zKAlphaFeld() const;
        // Gibt die Farbe des Farbübergangs des Körpers im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getKAfFarbe() const;
        // Gibt die Stärke des Farbübergangs des Körpers zurück
        __declspec( dllexport ) int getKAfStrength() const;
        // Gibt die Hintergrund Farbe des Schließen Knopfes im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getSBgFarbe() const;
        // Gibt das Hintergrund Bild des Schließen Knopfes zurück
        __declspec( dllexport ) Bild *getSBgBild() const;
        // Gibt das Hintergrund Bild des Schließen Knopfes ohne erhöhten Reference COunter zurück
        __declspec( dllexport ) Bild *zSBgBild() const;
        // Gibt den Farbübergang des Schließen Knopfes zurück
        __declspec( dllexport ) AlphaFeld *getSAlphaFeld() const;
        // Gibt den Farbübergang des Schließen Knopfes ohne erhöhten Reference COunter zurück
        __declspec( dllexport ) AlphaFeld *zSAlphaFeld() const;
        // Gibt die Farbe des Farbübergangs des Schließen Knopfes im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getSAfFarbe() const;
        // Gibt die Stärke des Farbübergangs des Schließen Knopfes zurück
        __declspec( dllexport ) int getSAfStrength() const;
        // Gibt den Farbübergang zurück, der verwendet wird, wenn auf den Schließen Knopf gedrückt wird
        __declspec( dllexport ) AlphaFeld *getSKAlphaFeld() const;
        // Gibt den Farbübergang ohne erhöhten Reference Counter zurück, der verwendet wird, wenn auf den Schließen Knopf gedrückt wird
        __declspec( dllexport ) AlphaFeld *zSKAlphaFeld() const;
        // Gibt die Farbe des Farbübergangs im A8R8G8B8 Format zurück, der verwendet wird, wenn auf den Schließen Knopf gedrückt wird
        __declspec( dllexport ) int getSKAfFarbe() const;
        // Gibt die Stärke des Farbübergangs zurück, der verwendet wird, wenn auf den Schließen Knopf gedrückt wird
        __declspec( dllexport ) int getSKAfStrength() const;
        // Gibt die minimale Fenstergröße in Pixeln zurück
        __declspec( dllexport ) const Punkt &getMin() const;
        // Gibt die maximale Fenstergröße in Pixeln zurück
        __declspec( dllexport ) const Punkt &getMax() const;
        // Gibt die minimale Körpergröße in Pixeln zurück
        __declspec( dllexport ) const Punkt &getKMin() const;
        // Gibt die maximale Körpergröße in Pixeln zurück
        __declspec( dllexport ) const Punkt &getKMax() const;
        // Gibt die Scrollbar vom rechten Fensterrand zurück
        __declspec( dllexport ) VScrollBar *getVScrollBar() const;
        // Gibt die Scrollbar vom rechten Fensterrand ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) VScrollBar *zVScrollBar() const;
        // Gibt die Scrollbar vom unteren Fensterrand zurück
        __declspec( dllexport ) HScrollBar *getHScrollBar() const;
        // Gibt die Scrollbar vom unteren Fensterrand ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) HScrollBar *zHScrollBar() const;
        // Gibt eine Liste mit Zeichnungen zurück, die im Fenster sind
        __declspec( dllexport ) ZeichnungArray *getMembers() const;
        // Erzeugt eine Kopie des Fensters, die ohne Auswirkungen auf das Original verändert werden kann
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Fenster *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Fenster *release();
    };
}
#endif