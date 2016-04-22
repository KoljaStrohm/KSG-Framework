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

    // Eine Zeichnung des 2D GUI Frameworks, die eine Liste darstellt, aus der der Benutzer Elemente aus und abwählen kann
    class AuswahlListe : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 FeldRahmen = 0x0001000; // Legt fest, ob die Einträge der Liste einen Rahmen haben
            const static __int64 FeldHintergrund = 0x0002000; // Legt fest, ob die Einträge einen Hintergrund haben
            const static __int64 FeldHBild = 0x0004000; // Legt fest, ob die Einträge ein Hintergrund Bild haben
            const static __int64 FeldHAlpha = 0x0008000; // Legt fest, ob beim zeichnen der Eintrag Hintergründe alpha blending verwendet werden soll
            const static __int64 FeldBuffer = 0x0010000; // Legt fest, ob die Einträge einen Farbübergang besitzen
            const static __int64 AuswahlHintergrund = 0x0020000; // Legt fest, ob die ausgewählten Einträge einen Hintergrund haben
            const static __int64 AuswahlHBild = 0x0040000; // Legt fest, ob die ausgewählten einträge ein Bild als Hintergrund haben
            const static __int64 AuswahlHAlpha = 0x0080000; // Legt fest, ob für das Zeichnen der Hintergründe der ausgewählten Einträge alpha blending verwendet wird
            const static __int64 AuswahlBuffer = 0x0100000; // Legt fest, ob die ausgewählten Einträge einen Farbübergang besitzen
            const static __int64 AuswahlRahmen = 0x0200000; // Legt fest, ob die ausgewählten Einträge einen Rahmen besitzen
            const static __int64 MultiStyled = 0x0400000; // Legt fest, ob jeder Eintrag seine eigenen Hintergrund, Farbübergang und Rahmen hat, fals er ausgewählt ist
            const static __int64 MultiSelect = 0x0800000; // Legt fest, das mehrere Einträge gleichzeitig ausgewählt sein können
            const static __int64 Ausgewählt = 0x1000000; // Legt fest, ob ein bestimmter Eintrag ausgewählt ist, falls MultiSelect gesetzt wurde.

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | FeldHAlpha | FeldHintergrund | FeldRahmen | AuswahlBuffer | AuswahlRahmen; // Vereint die Flags Sichtbar, Erlaubt, Rahmen, FeldHAlpha, FeldHintergrund, FeldRahmen, AuswahlBuffer, AuswahlRahmen
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
        // Aktualisiert die Styles, größe und Position der Einträge
        __declspec( dllexport ) void update();
        // Fügt einen Eintrag hinzu
        //  txt: Der Text des Eintrags
        __declspec( dllexport ) void addEintrag( Text *txt );
        // Fügt einen Eintrag hinzu
        //  txt: Der Text des Eintrags
        __declspec( dllexport ) void addEintrag( const char *txt );
        // Fügt einen zeiger auf einen Eintrag hinzu
        //  tf: Das TextFeld, mit dem der Eintrag gezeichnet wird
        __declspec( dllexport ) void addEintragZ( TextFeld *tf );
        // Fügt einen Eintrag an einer bestimmten Position hinzu
        //  pos: Der Index des neuen Eintrags
        //  txt: Der Text des Eintrags
        __declspec( dllexport ) void addEintrag( int pos, Text *txt );
        // Fügt einen Eintrag an einer bestimmten Position hinzu
        //  pos: Der Index des neuen Eintrags
        //  txt: Der Text des Eintrags
        __declspec( dllexport ) void addEintrag( int pos, const char *txt );
        // Fügt einen zeiger auf einen Eintrag an einer bestimmten Position hinzu
        //  pos: Der Index des neuen Eintrags
        //  tf: Das TextFeld, mit dem der Eintrag gezeichnet wird
        __declspec( dllexport ) void addEintragZ( int pos, TextFeld *tf );
        // Ändert einen Eintrag
        //  pos: Der Index des Eintrags
        //  txt: Der neue Text des Eintrags
        __declspec( dllexport ) void setEintrag( int pos, Text *txt );
        // Ändert einen Eintrag
        //  pos: Der Index des Eintrags
        //  txt: Der neue Text des Eintrags
        __declspec( dllexport ) void setEintrag( int pos, unsigned char *txt );
        // Ändert den Zeiger eines Eintrags
        //  pos: Der Index des Eintrags
        //  tf: Der neue Eintrag
        __declspec( dllexport ) void setEintragZ( int pos, TextFeld *tf );
        // Vertauscht die Positionen zweier Einträge
        //  vpos: Der Index des ersten Eintrags
        //  npos: Der Index des zweiten Eintrags
        __declspec( dllexport ) void tauschEintragPos( int vpos, int npos );
        // Löscht einen Eintrag
        // pos: Der Index des Eintrags
        __declspec( dllexport ) void löscheEintrag( int pos );
        // Setzt die verwendete Schrift
        //  schrift: Die Schrift
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        // Scrollt zu einem bestimmen Eintrag
        //  eintrag: Der Index des Eintrags
        __declspec( dllexport ) void setVScrollZuEintrag( int eintrag );
        // Aktualisiert die maximale Scroll Höhe indem die Höhe aller Einträge addiert wird
        __declspec( dllexport ) void updateVScroll();
        // Setzt den Zeiger auf den Rahmen, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  rahmen: Der Rahmen
        __declspec( dllexport ) void setALRZ( LRahmen *rahmen );
        // Setzt die Breite des Rahmens, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setALRBreite( int br );
        // Setzt die Farbe des Rahmens, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setALRFarbe( int fc );
        // Setzt den Zeiger auf den Farbübergnag, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  buffer: Der Farbübergang
        __declspec( dllexport ) void setAAFZ( AlphaFeld *buffer );
        // Setzt die Stärke des Farbübergnags, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  st: Die Stärke
        __declspec( dllexport ) void setAAFStärke( int st );
        // Setzt die Farbe des Farbübergnags, der bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setAAFFarbe( int fc );
        // Setzt das Hintergrund Bild durch kopieren, das bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  bild: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setAHBild( Bild *bild );
        // Setzt einen Zeiger auf das Hintergrund Bild, das bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  bild: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setAHBildZ( Bild *bild );
        // Setzt die Hintergrund Farbe, die bei ausgewählten Einträgen verwendet wird, fals MulitStyled nicht gesetzt wurde
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setAHFarbe( int fc );
        // Setzt den Zeiger auf den Rahmen, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  rahmen: Der Rahmen
        __declspec( dllexport ) void setALRZ( int pos, LRahmen *rahmen );
        // Setzt die Breite des Rahmens, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setALRBreite( int pos, int br );
        // Setzt die Farbe des Rahmens, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setALRFarbe( int pos, int fc );
        // Setzt den Zeiger auf den Farbübergnag, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  buffer: Der Farbübergang
        __declspec( dllexport ) void setAAFZ( int pos, AlphaFeld *buffer );
        // Setzt die Stärke des Farbübergnags, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  st: Die Stärke
        __declspec( dllexport ) void setAAFStärke( int pos, int st );
        // Setzt die Farbe des Farbübergnags, der bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setAAFFarbe( int pos, int fc );
        // Setzt das Hintergrund Bild durch kopieren, das bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  bild: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setAHBild( int pos, Bild *bild );
        // Setzt einen Zeiger auf das Hintergrund Bild, das bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  bild: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setAHBildZ( int pos, Bild *bild );
        // Setzt die Hintergrund Farbe, die bei einem ausgewählten Eintrag verwendet wird, fals MulitStyled gesetzt wurde
        //  pos: Der Index des Eintrags
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setAHFarbe( int pos, int fc );
        // Setzt den Style eines Eintrags, falls MulitStyled gesetzt wurde und legt fest, ob ein Eintrag ausgewählt ist, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Der neue Style
        __declspec( dllexport ) void setMsStyle( int pos, __int64 style );
        // Ändert den Style eines Eintrags, falls MulitStyled gesetzt wurde und legt fest, ob ein Eintrag ausgewählt ist, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Der Style
        //  add_löschen: 1, falls der Style hinzugefügt werden soll. 0, falls der Style entfernt weden soll
        __declspec( dllexport ) void setMsStyle( int pos, __int64 style, bool add_löschen );
        // Fügt Styles zu einem Eintrag hinzu, falls MulitStyled gesetzt wurde und legt fest, ob ein Eintrag ausgewählt ist, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Der Style, der hinzugefügt werden soll
        __declspec( dllexport ) void addMsStyle( int pos, __int64 style );
        // Entfernt Styles von einem Eintrag, falls MulitStyled gesetzt wurde und legt fest, ob ein Eintrag ausgewählt ist, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Der Style, der entfernt werden soll
        __declspec( dllexport ) void löscheMsStyle( int pos, __int64 style );
        // Verarbeitet Tastatur Nachrichten
        //  me: Das Ereignis, was durch die Tastatureingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Verarbeitet ein Tastatur Ereignis. Wird vom Framework automatisch aufgerufen
        //  te: Das Ereignis
        __declspec( dllexport ) void doTastaturEreignis( TastaturEreignis &te ) override;
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt den Index eines Eintrags zurück, auf den die Maus zeigt
        //  my: Die Position der Maus auf der Y Achse basierend auf dem oberend Rand der Liste
        __declspec( dllexport ) int getKlickEintrag( int my );
        // Wählt einen Eintrag aus
        //  ausw: Der Index des Eintrags
        __declspec( dllexport ) void setAuswahl( int ausw );
        // Wählt alle ausgewählten Einträge ab
        __declspec( dllexport ) void deSelect();
        // Gibt die Anzahl an Einträgen zurück
        __declspec( dllexport ) int getEintragAnzahl() const;
        // Gibt den Index des ausgewählten Eintrags zurück, fals MultiSelect nicht gesetzt ist. Wenn MultiSelect gesetzt wurde, kann die Auswahl eines Eintrags mit hatMsStyle( Eintrag Index, AuswahlListe::Style::Ausgewählt ) geprüft werden
        __declspec( dllexport ) int getAuswahl() const;
        // Gibt den Index eines Eintrags zurück
        //  eintragText: Der Text des Eintrags
        __declspec( dllexport ) int getEintragPos( Text *eintragText );
        // Gibt einen Eintrag zurück
        //  pos: Der Index des Eintrags
        __declspec( dllexport ) TextFeld *getEintrag( int pos ) const;
        // Gibt einen Eintrag ohne erhöhten reference Counter zurück
        //  pos: Der Index des Eintrags
        __declspec( dllexport ) TextFeld *zEintrag( int pos ) const;
        // Gibt den Rahmen zurück, der für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) LRahmen *getARahmen() const;
        // Gibt den Rahmen ohne erhöhten Reference Counter zurück, der für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) LRahmen *zARahmen() const;
        // Gibt die Hintergrund Farbe im A8R8G8B8 Format zurück, die für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) int getAHFarbe() const;
        // Gibt das Hintergrund Bild zurück, das für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) Bild *getAHBild() const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference Counter zurück, das für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) Bild *zAHBild() const;
        // Gibt den Farbverlauf zurück, der für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) AlphaFeld *getABuffer() const;
        // Gibt den Farbverlauf ohne erhöhten Reference Counter zurück, der für ausgewählte Einträge verwendet wird, falls MultiStyled nicht gesetzt wurde
        __declspec( dllexport ) AlphaFeld *zABuffer() const;
        // Gibt den Rahmen zurück, der für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) LRahmen *getARahmen( int pos ) const;
        // Gibt den Rahmen ohne erhöhten Reference Counter zurück, der für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) LRahmen *zARahmen( int pos ) const;
        // Gibt die Hintegrrund Farbe im A8R8G8B8 Format zurück, die für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) int getAHFarbe( int pos ) const;
        // Gibt das Hintergrund Bild zurück, das für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) Bild *getAHBild( int pos ) const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference Counter zurück, das für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) Bild *zAHBild( int pos ) const;
        // Gibt den Farbübergang zurück, der für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) AlphaFeld *getABuffer( int pos ) const;
        // Gibt den Farbübergang ohne erhöhten Reference Counter zurück, der für einen ausgewählten Eintrag verwendet wird, falls MultiStyled gesetzt wurde
        __declspec( dllexport ) AlphaFeld *zABuffer( int pos ) const;
        // Überprüft, ob für einen Bestimmten Eintarg betimmte Styles gesetzt wurden, falls MultiStyled gesetzt wurde. Überprüft auch, ob ein Eintrag ausgewählt wurde, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Die zu prüfenden Styles
        __declspec( dllexport ) inline bool hatMsStyle( int pos, __int64 style ) const;
        // Überprüft, ob für einen Bestimmten Eintarg betimmte Styles nicht gesetzt wurden, falls MultiStyled gesetzt wurde. Überprüft auch, ob ein Eintrag nicht ausgewählt wurde, falls MultiSelect gesetzt wurde
        //  pos: Der Index des Eintrags
        //  style: Die zu prüfenden Styles
        __declspec( dllexport ) inline bool hatMsStyleNicht( int pos, __int64 style ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) AuswahlListe *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) AuswahlListe *release();
    };
}

#endif