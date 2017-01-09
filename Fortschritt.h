#ifndef Fortschritt_H
#define Fortschritt_H

#include "Zeichnung.h"

namespace Framework
{
    class LRahmen; // Rahmen.h
    class AlphaFeld; // AlphaFeld.h
    class Bild; // Bild.h
    class Schrift; // Schrift.h
    class FBalken; // aus dieser Datei

    // Eine Zeichnung des 2D GuI Frameworks, die einen Fortschrittsbalken zeichnet
    class FBalken : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Prozent = 0x001000; // Legt fest, ob eine Prozentangebe gezechnet wird

            const static __int64 FRahmen = 0x002000; // Legt fest, ob der vom fortschritt erfasste Teil des Balkens einen eigenen Rahmen hat
            const static __int64 FFarbe = 0x004000; // Legt fest, ob der vom fortschritt erfasste Teil des Balkens eine eigene Hintergrund Farbe hat
            const static __int64 FBild = 0x008000; // Legt fest, ob der vom fortschritt erfasste Teil des Balkens ein eigenes Hintergrund Bild hat
            const static __int64 FAlpha = 0x10000; // Legt fest, ob der vom fortschritt erfasste Teil des Balkens zum zeichnen seines Hintergrundes alpha blending verwendet
            const static __int64 FBuffered = 0x20000; // Legt fest, ob der vom fortschritt erfasste Teil des Balkens einen eigenen Frbübergang hat

            const static __int64 L_R = 0x0100000; // Legt fest, ob sich der Fortschritt von lenks nach rechts ausbreitet
            const static __int64 R_L = 0x0200000; // Legt fest, ob sich der Fortschritt von rechts nach links ausbreitet
            const static __int64 U_O = 0x0400000; // Legt fest, ob sich der Fortschritt von unten nach oben ausbreitet
            const static __int64 O_U = 0x0800000; // Legt fest, ob sich der Fortschritt von oben nach unten ausbreitet

            const static __int64 normal = Sichtbar | Rahmen | Hintergrund | HBild | FRahmen | FBild | L_R | Prozent; // Vereint die Flags Sichtbar, Rahmen, Hintergrund, HBild, FRahmen, FBild, L_R, Prozent
        };
    private:
        __int64 maxAk;
        __int64 ak;
        LRahmen *fRahmen;
        AlphaFeld *fBuffer;
        int fBgF;
        Bild *fBgBild;
        Schrift *schrift;
        int schriftFarbe;
        unsigned char schriftSize;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) FBalken();
        // Destructor 
        __declspec( dllexport ) virtual ~FBalken();
        // Setzt die Anzahl an benötigten Operationen um 100% Fortschritt zu erreichen
        //  ak: Die Anzahl an Operationen
        __declspec( dllexport ) void setAktionAnzahl( __int64 ak );
        // Legt fest, dass eine bestimmte Anzahl an Operationen seit dem letzten Aufruf der Funktion durchgeführt wurden
        //  aktionen: Die durchgeführten Operationen. Standartmäßig auf 1
        __declspec( dllexport ) void aktionPlus( __int64 aktionen = 1 );
        // Setzt alle fertigen Operationen zurück, so dass der Fortschritt wieder von 0% beginnt
        __declspec( dllexport ) void reset();
        // Setzt einen Zeiger auf den Rahmen, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  ram: Der Rahmen
        __declspec( dllexport ) void setFRahmenZ( LRahmen *ram );
        // Setzt die Farbe des Rahmens, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setFRFarbe( int f );
        // Setzt die Breite des Rahmens, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setFRBreite( int br );
        // Setzt einen Zeiger auf den Farbverlauf, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  af: Der Farbverlauf
        __declspec( dllexport ) void setFAlphaFeldZ( AlphaFeld *af );
        // Setzt die Farbe des Farbverlaufs, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setFAFFarbe( int f );
        // Setzt die Stärke des Farbverlaufs, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  st: Die Stärke
        __declspec( dllexport ) void setFAFStrength( int st );
        // Setzt die Hintergrund Farbe, die in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setFBgFarbe( int f );
        // Setzt einen Zeiger auf das Hintergrund Bild, das in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  b: Das Bild
        __declspec( dllexport ) void setFBgBildZ( Bild *b );
        // Setzt das Hintergrund Bild durch kopieren, das in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        //  b: Das Bild, das kopiert werden soll
        __declspec( dllexport ) void setFBgBild( Bild *b );
        // Setzt die Schrift, die verwendet wird, um die Prozentanzeige zu zeichnen
        __declspec( dllexport ) void setSchriftZ( Schrift *b );
        // Setzt die Farbe der Prozentanzeige
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setSFarbe( int f );
        // Setzt die Schriftgröße der Prozentanzeige
        //  gr: Die Größe einer Zeile in Pixeln
        __declspec( dllexport ) void setSSize( unsigned char gr );
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die Anzahl an benötigten Operationen zurück, die für 100% benötigt werden
        __declspec( dllexport ) __int64 getAktionAnzahl() const;
        // Gibt die momentan erreichte Prozentzahl des Fortschritts zurück
        __declspec( dllexport ) double getProzent() const;
        // Gibt die Anzahl an fertigen Operationen zurück
        __declspec( dllexport ) __int64 getAktion() const;
        // Gibt den Rahmen zurück, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) LRahmen *getFRahmen() const;
        // Gibt den Rahmen ohne erhöhten Reference Counter zurück, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) LRahmen *zFRahmen() const;
        // Gibt den Farbübergang zurück, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) AlphaFeld *getFAlphaFeld() const;
        // Gibt den Farbübergang ohne erhöhten Reference Counter zurück, der in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) AlphaFeld *zFAlphaFeld() const;
        // Gibt die Hintergrund Farbe im A8R8G8B8 Format zurück, die in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) int getFBgFarbe() const;
        // Gibt das Hintergrund Bild zurück, das in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) Bild *getFBgBild() const;
        // Gibt das Hintergrund Bild ohne erhöhten Reference COunter zurück, das in dem Teil des Balkens gezeichnet wird, der vom Fortschritt erfasst wurde
        __declspec( dllexport ) Bild *zFBgBild() const;
        // Gibt die Schrift zurück, die für die Prozentanzeige verwendet wird
        __declspec( dllexport ) Schrift *getSchrift() const;
        // Gibt die Schrift ohne erhöhten Reference Counter zurück, die für die Prozentanzeige verwendet wird
        __declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt die Farbe der Prozentanzeige im A8R8G8B8 Format zurück
        __declspec( dllexport ) int getSFarbe() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) FBalken *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) FBalken *release();
    };
}

#endif