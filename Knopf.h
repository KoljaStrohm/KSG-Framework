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

    // Eine Zeichnung des 2D GUI Frameworks, die einen Knopf zeichnet, den der Benutzer drücken kann
    class Knopf : public TextFeld
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 MehrfarbigText = 0x0010000; // Legt fest, ob der Text des Knopfes mehrere Farben haben darf
            const static __int64 KlickFarbe = 0x0020000; // Legt fest, ob der Knopf während er gedrückt wird eine Hintergrund Farbe hat
            const static __int64 KlickBild = 0x0040000; // Legt fest, ob der Knopf während er gedrückt wird ein Bild als Hintergrund hat
            const static __int64 KlickAlpha = 0x0080000; // Legt fest, ob der Knopf während er gedrückt wird für das Zeichnen des Hintergrundes alpha blending verwendet
            const static __int64 KlickBuffer = 0x0100000; // Legt fest, ob der Knopf während er gedrückt wird einen Farbübergang besitzt

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | Buffered | MehrfarbigText | KlickBuffer; // Vereint di Flags Sichtbar, Erlaubt, Rahmen, Buffered, MehrfarbigText, KlickBuffer
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
        __declspec( dllexport ) virtual ~Knopf();
        // Setzt die Farbe, die als Hintergrund verwendet wird, wenn der Knopf gedrückt wird
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKlickFarbe( int fc );
        // Setzt das Hintergrund Bild durch kopieren, das verwendet wird, wenn der Knopf gedrückt wird
        //  bild: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setKlickBild( Bild *bild );
        // Setzt einen Zeiger auf das Hintergrund Bild, das verwendet wird, wenn der Knopf gedrückt wird
        //  bild: Das neue Bild
        __declspec( dllexport ) void setKlickBildZ( Bild *bild );
        // Setzt einen Zeiger auf den Farbübergang, der verwendet wird, wenn der Knopf gedrückt wird
        //  af: Der neue Farbübergang
        __declspec( dllexport ) void setKBZ( AlphaFeld *af );
        // Setzt die Stärke des Farbübergangs, der verwendet wird, wenn der Knopf gedrückt wird
        //  st: Die Stärke
        __declspec( dllexport ) void setKBStrength( int st );
        // Setzt die Farbe des Farbübergangs, der verwendet wird, wenn der Knopf gedrückt wird
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKBFarbe( int fc );
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die Hintergrund Farbe im A8R8G8B8 Format zurück, die verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) int getKlickFarbe() const;
        // Gibt das Hintergrund Bild zurück, das verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) Bild *getKlickBild() const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference Counter zurück, das verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) Bild *zKlickBild() const;
        // Gibt den Farbübergang zurück, der verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) AlphaFeld *getKB() const;
        // Gibt den Farbübergang ohne erhöhten Reference Counter zurück, der verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) AlphaFeld *zKB() const;
        // Gibt die Farbe des Farbübergangs im A8R8G8B8 Format zurück, der verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) int getKBFarbe() const;
        // Gibt die Stärke des Farbübergangs zurück, der verwendet wird, wenn der Knopf gedrückt wird
        __declspec( dllexport ) int getKBStrength() const;
        // Kopiert den Knopf, so das er ohne auswirkungen auf das Original verändert werden kann
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Knopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Knopf *release();
    };

    // Eine Zeichnung des 2D GUI Frameworks, die einen Auswählbares Kästchen mit beschriftung zeichnet
    class KontrollKnopf : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Selected = 0x00004; // Legt fest, ob das Kästchen ausgewählt ist

            const static __int64 MehrfarbigText = 0x001000; // Legt Fest, ob der Beschriftungs Text mehrfarbig sein kann
            const static __int64 KlickFarbe = 0x002000; // Legt fest, ob das Kästchen während ed gedrückt wird eine Hintergrund Farbe hat
            const static __int64 KlickBild = 0x004000; // Legt fest, ob das Kästchen beim Klicken ein Hintergrund Bild hat
            const static __int64 KlickAlpha = 0x008000; // Legt fest, ob für den Hintergrund des Kästchens beim Klicken alpha blending verwendet wird
            const static __int64 KlickBuffer = 0x010000; // Legt fest, ob das Kästchen beim Klicken einen Farbübergang hat
            const static __int64 SelectFarbe = 0x020000; // Legt fest, ob das Kästchen wärhend es ausgewählt ist eine Hintergrund Farbe hat
            const static __int64 SelectBild = 0x040000; // Legt fest, ob das Kästchen wärhend es ausgewählt ist ein Hintergrund Bild hat
            const static __int64 SelectAlpha = 0x080000; // Legt fest, ob das Kästchen wärhend es ausgewählt ist für den Hintergrund alpha blending verwendet
            const static __int64 SelectBuffer = 0x100000; // Legt fest, ob das Kästchen wärhend es ausgewählt ist einen Farbübergang hat
            const static __int64 SelectText = 0x200000; // Legt fest, ob das Kästchen wärhend es ausgewählt ist eine andere Beschriftung hat
            const static __int64 MehrzeiligText = 0x400000; // Legt fest, ob das Kästchen eine Mehrzeilige Beschriftung haben kann

            const static __int64 MausKlick = 0x800000; // Legt fest, ob gerade mit der Maus auf das Kästchen geklickt wird. (Wird vom Kästchen selbst verwaltet)

            const static __int64 Normal = Sichtbar | Erlaubt | Rahmen | KlickBuffer; // Vereint die Flags Sichtbar, Erlaubt, Rahmen, KlickBuffer
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
        __declspec( dllexport ) virtual ~KontrollKnopf();
        // Setzt den Zeiger auf den Beschriftungs Text
        //  txt: Der Text
        __declspec( dllexport ) void setTextZ( Text *txt );
        // Setzt den Beschriftungs Text
        //  txt: Der Text
        __declspec( dllexport ) void setText( Text *txt );
        // Setzt den Beschriftungs Text
        //  txt: Der Text
        __declspec( dllexport ) void setText( const char *txt );
        // Setzt den Zeiger auf den Beschriftungs Text, der angezeigt wird, während das Kästchen ausgewählt ist
        //  txt: Der Text
        __declspec( dllexport ) void setSTextZ( Text *txt );
        // Setzt den Beschriftungs Text, der angezeigt wird, während das Kästchen ausgewählt ist
        //  txt: Der Text
        __declspec( dllexport ) void setSText( Text *txt );
        // Setzt den Beschriftungs Text, der angezeigt wird, während das Kästchen ausgewählt ist
        //  txt: Der Text
        __declspec( dllexport ) void setSText( const char *txt );
        // Setzt die Schrift, die für die Beschriftung verwendet werden soll
        //  schrift: Die Schrift
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        // Setzt die Farbe der Schrift, die für die Beschriftung verwendet werden soll
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSFarbe( int f );
        // Setzt die Größe der Schrift, die für die Beschriftung verwendet werden soll
        //  f: Die Höhe einer Zeile in Pixeln
        __declspec( dllexport ) void setSSize( int gr );
        // Setzt die Hintergrund Farbe, die verwendet wird, während das Kästchen ausgewählt ist
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSBgFarbe( int f );
        // Setzt die Hintergrund Farbe, die verwendet wird, während auf das Kästchen geklickt wird
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKBgFarbe( int f );
        // Setzt einen Zeiger auf das Hintergrund Bild, das verwendet wird, während das Kästchen ausgewählt ist
        //  b: Das Bild
        __declspec( dllexport ) void setSBgBildZ( Bild *b );
        // Setzt das Hintergrund Bild durch kopieren, das verwendet wird, während das Kästchen ausgewählt ist
        //  b: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setSBgBild( Bild *b );
        // Setzt einen Zeiger auf das Hintergrund Bild, das verwendet wird, während auf das Kästchen geklickt wird
        //  b: Das Bild
        __declspec( dllexport ) void setKBgBildZ( Bild *b );
        // Setzt das Hintergrund Bild durch kopieren, das verwendet wird, während auf das Kästchen geklickt wird
        //  b: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setKBgBild( Bild *b );
        // Setzt einen Zeiger auf den Farbübergang, der verwendet wird, während das Kästchen ausgewählt ist
        //  af: Der Farbübergang
        __declspec( dllexport ) void setSAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs, der verwendet wird, während das Kästchen ausgewählt ist
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSAFFarbe( int f );
        // Setzt die Stärke des Farbübergangs, der verwendet wird, während das Kästchen ausgewählt ist
        //  st: Die Stärke
        __declspec( dllexport ) void setSAFStrength( int st );
        // Setzt einen Zeiger auf den Farbübergang, der verwendet wird, während auf das Kästchen geklickt wird
        //  af: Der Farbübergang
        __declspec( dllexport ) void setKAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs, der verwendet wird, während auf das Kästchen geklickt wird
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setKAFFarbe( int f );
        // Setzt die Stärke des Farbübergangs, der verwendet wird, während auf das Kästchen geklickt wird
        //  st: Die Stärke
        __declspec( dllexport ) void setKAFStrength( int st );
        // Lädt die Bilder "kasten.gif" und "skasten.gif" aus einer LTDB Datei
        // Das Bild kasten.gif wird gezeichnet, wenn das Kästchen nicht ausgewählt ist. Das Bild skasten.gif wird gezeichnet, wenn das Kästchen ausgewählt wird
        //  zDat: Die LTDB Datei
        __declspec( dllexport ) void loadData( LTDBDatei *zDat );
        // Lädt die Bilder "kasten.gif" und "skasten.gif" aus einer LTDB Datei
        // Das Bild kasten.gif wird gezeichnet, wenn das Kästchen nicht ausgewählt ist. Das Bild skasten.gif wird gezeichnet, wenn das Kästchen ausgewählt wird
        //  ltdb: Der Pfad zur LTDB Datei
        __declspec( dllexport ) void loadData( const char *ltdb );
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die beschriftung zurück
        __declspec( dllexport ) Text *getText() const;
        // Gibt die beschriftung ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Text *zText() const;
        // Gibt die beschriftung zurück, die verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) Text *getSText() const;
        // Gibt die beschriftung ohne erhöhten Reference Counter zurück, die verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) Text *zSText() const;
        // Gibt die verwendete Schrift zurück
        __declspec( dllexport ) Schrift *getSchrift() const;
        // Gibt die verwendete Schrift ohne Reference Counter zurück
        __declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt die Schrift Farbe im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getSFarbe() const;
        // Gibt die Schrift Größe zurück
        __declspec( dllexport ) int getSSize() const;
        // Gibt die Hintergrund Farbe im A8R8G8B8 Format zurück, die verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) int getSBgFarbe() const;
        // Gibt die Hintergrund Farbe im A8R8G8B8 Format zurück, die verwendet wird, während auf das Kästchen geklickt wird
        __declspec( dllexport ) int getKBgFarbe() const;
        // Gibt das Hintergrund Bild zurück, das verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) Bild *getSBgBild() const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference Counter zurück, das verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) Bild *zSBgBild() const;
        // Gibt das Hintergrund Bild zurück, das verwendet wird, während auf das Kästchen geklickt wird
        __declspec( dllexport ) Bild *getKBgBild() const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference Counter zurück, das verwendet wird, während auf das Kästchen geklickt wird
        __declspec( dllexport ) Bild *zKBgBild() const;
        // Gibt den Farbübergang zurück, der verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) AlphaFeld *getSAlphaFeld() const;
        // Gibt den Farbübergang ohne erhöhten Reference COunter zurück, der verwendet wird, während das Kästchen ausgewählt ist
        __declspec( dllexport ) AlphaFeld *zSAlphaFeld() const;
        // Gibt den Farbübergang zurück, der verwendet wird, während auf das Kästchen geklickt wird
        __declspec( dllexport ) AlphaFeld *getKAlphaFeld() const;
        // Gibt den Farbübergang ohne erhöhten Reference COunter zurück, der verwendet wird, während auf das Kästchen geklickt wird
        __declspec( dllexport ) AlphaFeld *zKAlphaFeld() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) KontrollKnopf *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) KontrollKnopf *release();
    };
}

#endif