#ifndef Bild_H
#define Bild_H

#include "Array.h"
#ifdef WIN32
#include "Zeichnung.h"
#else
#include "Punkt.h"
#endif

namespace Framework
{
    class Bild; // aus dieser Datei
#ifdef WIN32
    class VScrollBar; // Scroll.h
    class HScrollBar; // Scroll.h
    struct VScrollData; // Scroll.h
    struct HScrollData; // Scroll.h
    class LRahmen; // Rahmen.h
    struct MausEreignis; // Mausereignis.h
    class Text; // Text.h
#endif

    // Verwaltet ein Bild als Array von Pixel Farben im A8R8G8B8 Format, in den gezeichnet werden kann
    class Bild
    {
    private:
        int *fc;
        bool delFc;
        Punkt size;
        int ref;
        Punkt *drawOff;
        Punkt *dPosA;
        Punkt *dSizeA;
        int doa;
        unsigned char *alpha;
        int alphaAnzahl;
        bool rend;
        bool alpha3D;

        // privat
        inline void alphaPixelP( int x, int y, int f );
        inline void alphaPixelP( int &fc, int f );
        inline char getOutCode( Punkt p ) const;
        void drawFlatDreieck( int y1, int y2, float m1, float b1, float m2, float b2, int farbe );
        void drawFlatDreieckTextur( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
                                    double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur );
        void drawFlatDreieckAlpha( int y1, int y2, float m1, float b1, float m2, float b2, int farbe );
        void drawFlatDreieckTexturAlpha( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
                                         double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur );
        void drawLinieHTextur( Vec2< double > p, double length, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur );
        void drawLinieHTexturAlpha( Vec2< double > p, double length, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur );

    public:
        // Konstruktor
        //  options: 1, wenn das Bild Buffer zum Zeichnen anlegen soll, wie zum Beispiel für temporäre Zeichnenflächen Begrenzungen und Transparenz Filter
        //  Verbraucht etwa 50 kb mehr Arbeitsspeicher pro Bild
        __declspec( dllexport ) Bild( bool options = 0 );
        // Destruktor 
        __declspec( dllexport ) ~Bild();
        // Wird dieser Flag gesetzt, so wird beim Alpha Blending wenn die vorheriege Farbe 0 ist nur die neue mit ihrem Alpha Wert kopiert.
        // Das ist sinnvoll für die Verwendung im 3DBildschirm, wo das Gezeichnette Bild später mittels Alpha Blending angezeigt wird.
        // Der Flag wird im 3DBildschirm automatisch gesetzt
        __declspec( dllexport ) void setAlpha3D( bool erlaubt );
        // Setzt die mindest Transparenz von allem was als nächstes gezeichnet wird, bis der zugehörige releaseAlpha() aufruf erfolgt.
        __declspec( dllexport ) void setAlpha( unsigned char alpha );
        // Entfernt die als letztes mit setAlpha() gesetzte Alpha grenze
        __declspec( dllexport ) void releaseAlpha();
        // Setzt einen Zeiger auf die Pixel, in die gezeichnet werden soll
        //  buffer: Ein Array mit den A8R8G8B8 Farbwerten
        //  deleteBuffer: 1, fals der Array vom Bild gelöscht werden soll
        //  breite: Die Breite in Pixeln
        //  height: Die Höhe in Pixeln
        __declspec( dllexport ) void setPixelBuffer( int *buffer, bool deleteBuffer, int breite, int height );
        // Erstellt ein neues Bild
        //  breite: Die Breite des Bildes in Pixeln
        //  höhe: Die Höhe des Bildes in Pixeln
        //  fillColor: Die startfarbe, auf die alle Pixel gesetzt werden
        __declspec( dllexport ) void neuBild( int breite, int height, int fillColor );
        // Blendet mittels Alphablending eine Farbe auf einen bestimmten Pixel
        // Beachtet nicht die begrenzung der Zeichnenfläche und das Skroll Offset
        //  x: Die X Koordinate des Pixels
        //  y: Die Y Koordinate des Pixels
        //  f: Die Farbe in A8R8G8B8 Format, die geblendet werden soll
        __declspec( dllexport ) void alphaPixel( int x, int y, int f );
        // Blendet mittels Alphablending eine Farbe auf einen bestimmten Pixel
        // Beachtet nicht die begrenzung der Zeichnenfläche und das Skroll Offset
        //  i: Der Index des Pixels im Pixel Array
        //  f: Die Farbe in A8R8G8B8 Format, die geblendet werden soll
        __declspec( dllexport ) void alphaPixel( int i, int f );
        // Blendet mittels Alphablending eine Farbe auf einen bestimmten Pixel
        //  x: Die X Koordinate des Pixels
        //  y: Die Y Koordinate des Pixels
        //  f: Die Farbe in A8R8G8B8 Format, die geblendet werden soll
        __declspec( dllexport ) void alphaPixelDP( int x, int y, int f );
        // Blendet mittels Alphablending eine Farbe auf einen bestimmten Pixel
        //  i: Der Index des Pixels im Pixel Array
        //  f: Die Farbe in A8R8G8B8 Format, die geblendet werden soll
        __declspec( dllexport ) void alphaPixelDP( int i, int f );
        // Setzt die Farbe eines besimmten Pixels
        //  x: Die X Koordinate des Pixels
        //  y: Die Y Koordinate des Pixels
        //  f: Die neue Farbe in A8R8G8B8 Format
        __declspec( dllexport ) void setPixelDP( int x, int y, int f );
        // Setzt die Farbe eines besimmten Pixels
        //  i: Der Index des Pixels im Pixel Array
        //  f: Die neue Farbe in A8R8G8B8 Format
        __declspec( dllexport ) void setPixelDP( int i, int f );
        // Setzt die Farbe aller Pixel des Bildes
        //  f: Die neue Farbe
        __declspec( dllexport ) void setFarbe( int f );
        // Setzt die Farben aller Pixel in einem Rechteck
        //  x: Die X Koordinate
        //  y: Die Y Koordinate
        //  b: Die Breite des Rechtecks
        //  h: Die Höhe des Rechtecks
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void fillRegion( int x, int y, int b, int h, int fc );
        // Blendet eine Farbe mit Alphablending in einem Rechteck
        //  x: Die X Koordinate
        //  y: Die Y Koordinate
        //  b: Die Breite des Rechtecks
        //  h: Die Höhe des Rechtecks
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void alphaRegion( int x, int y, int b, int h, int fc );
        // Zeichnet eine horizontale Linie
        //  x: Die X Koordinate des Startpunktes der Linie
        //  y: Die Y Koordinate des Startpunktes der Linie
        //  length: Die Länge der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinieH( int x, int y, int length, int fc );
        // Zeichnet eine vertikale Linie
        //  x: Die X Koordinate des Startpunktes der Linie
        //  y: Die Y Koordinate des Startpunktes der Linie
        //  length: Die Länge der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinieV( int x, int y, int length, int fc );
        // Zeichnet eine horizontale Linie mit Alpha blending
        //  x: Die X Koordinate des Startpunktes der Linie
        //  y: Die Y Koordinate des Startpunktes der Linie
        //  length: Die Länge der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinieHAlpha( int x, int y, int length, int fc );
        // Zeichnet eine vertikale Linie mit Alpha Blending
        //  x: Die X Koordinate des Startpunktes der Linie
        //  y: Die Y Koordinate des Startpunktes der Linie
        //  length: Die Länge der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinieVAlpha( int x, int y, int length, int fc );
        // Zeichnet eine Linie
        //  a: Der Startpunkt der Linie
        //  b: der Endpunkt der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinie( Punkt a, Punkt b, int fc );
        // Zeichnet eine Linie mit Alpha Blending
        //  a: Der Startpunkt der Linie
        //  b: der Endpunkt der Linie
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawLinieAlpha( Punkt a, Punkt b, int fc );
        // Füllt einen Kreis mit einer Farbe. (Unfertig)
        //  xOff: Die X Koordinate des Kreismittelpunktes
        //  yOff: Die Y Koordinate des Kreismittelpunktes
        //  r: Der Radius des Kreises in Pixeln
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void fillCircle( int xOff, int yOff, int r, int fc );
        // Zeichnet den Umriss eines Kreises
        //  xOff: Die X Koordinate des Kreismittelpunktes
        //  yOff: Die Y Koordinate des Kreismittelpunktes
        //  r: Der Radius des Kreises in Pixeln
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawKreis( int xOff, int yOff, int r, int fc );
        // Zeichnet den Umriss eines Kreises mit Alpha Blending
        //  xOff: Die X Koordinate des Kreismittelpunktes
        //  yOff: Die Y Koordinate des Kreismittelpunktes
        //  r: Der Radius des Kreises in Pixeln
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawKreisAlpha( int xOff, int yOff, int r, int fc );
        // Zeichnet ein Bild in ein bestimmtes Feld ohne Skallierung
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void drawBild( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild in ein bestimmtes Feld ohne Skallierung mit alpha blending
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void alphaBild( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 90 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void drawBild90( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 90 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung mit alpha Blending
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void alphaBild90( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 180 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void drawBild180( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 180 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung mit alpha Blending
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void alphaBild180( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 270 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void drawBild270( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild um 270 grad nach rechts gedreht in ein bestimmtes Feld ohne Skallierung mit alpha Blending
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void alphaBild270( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild in ein bestimmtes Feld mit Skallierung
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void drawBildSkall( int x, int y, int br, int hi, Bild &zBild );
        // Zeichnet ein Bild in ein bestimmtes Feld mit Skallierung mit Alpha Blending
        //  x: Die X Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  y: Die Y Koordinate der linken oberen Ecke des Rechtecks in dem das Bild gezeichnet werden soll
        //  br: Die Breite des Rechecks in dem das Bild gezeichnet werden soll
        //  hi: Die Höhe des Rechecks in dem das Bild gezeichnet werden soll
        //  zBild: Das Bild, was gezeichnet werden soll
        __declspec( dllexport ) void alphaBildSkall( int x, int y, int br, int hi, Bild &zBild );
        // Füllt ein Dreieck mit einer bestimmten Farbe
        //  a: Eine Ecke des Dreiecks
        //  b: Eine Ecke des Dreiecks
        //  c: Eine Ecke des Dreiecks
        //  farbe: die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawDreieck( Punkt a, Punkt b, Punkt c, int farbe );
        // Füllt ein Dreieck mit einer bestimmten Textur
        //  a: Eine Ecke des Dreiecks
        //  b: Eine Ecke des Dreiecks
        //  c: Eine Ecke des Dreiecks
        //  ta: Die Koordinaten von a in der Textur
        //  tb: Die Koordinaten von b in der Textur
        //  tc: Die Koordinaten von c in der Textur
        //  textur: Das Bild, was als Textur verwendet werden soll
        __declspec( dllexport ) void drawDreieckTextur( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur );
        // Füllt ein Dreieck mit einer bestimmten Farbe mit alpha blending
        //  a: Eine Ecke des Dreiecks
        //  b: Eine Ecke des Dreiecks
        //  c: Eine Ecke des Dreiecks
        //  farbe: die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void drawDreieckAlpha( Punkt a, Punkt b, Punkt c, int farbe );
        // Füllt ein Dreieck mit einer bestimmten Textur mit alpha blending
        //  a: Eine Ecke des Dreiecks
        //  b: Eine Ecke des Dreiecks
        //  c: Eine Ecke des Dreiecks
        //  ta: Die Koordinaten von a in der Textur
        //  tb: Die Koordinaten von b in der Textur
        //  tc: Die Koordinaten von c in der Textur
        //  textur: Das Bild, was als Textur verwendet werden soll
        __declspec( dllexport ) void drawDreieckTexturAlpha( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur );
        // Begrenzt die Zeichenfläche in die bis zum nächsten aufruf von releaseDrawOptions() gezeichnet werden kann. Alles außerhalb der Fläche wird automatisch ignoriert.
        // Wenn die Zeichenfläche über den Rand einer bestehenden Zeichenfläche hinausragt, wird sie automatisch auf die bestehende Zeichnenfläche beschnitten.
        //  pos: Die Koordinaten der linken oberen Ecke der Zeichenfläche
        //  gr: Die Größe der Zeichenfläche
        //  return: 1, falls die neue Zeichenfläche ganz oder teilweise innerhalb der bestehenden Zeichenfläche ligt. Wenn 0 zurückgegeben wird, wurde die Zeichenfläche nicht gesetzt und es braucht nicht releaseDrawOptions() aufgerufen werden
        __declspec( dllexport ) bool setDrawOptions( const Punkt &pos, const Punkt &gr );
        // Begrenzt die Zeichenfläche in die bis zum nächsten aufruf von releaseDrawOptions() gezeichnet werden kann. Alles außerhalb der Fläche wird automatisch ignoriert.
        // Wenn die Zeichenfläche über den Rand einer bestehenden Zeichenfläche hinausragt, wird sie automatisch auf die bestehende Zeichnenfläche beschnitten.
        //  x: Die X Koordinate der linken oberen Ecke der Zeichenfläche
        //  y: Die X Koordinate der linken oberen Ecke der Zeichenfläche
        //  br: Die Breite der Zeichenfläche
        //  hi: Die Höhe der Zeichenfläche
        //  return: 1, falls die neue Zeichenfläche ganz oder teilweise innerhalb der bestehenden Zeichenfläche ligt. Wenn 0 zurückgegeben wird, wurde die Zeichenfläche nicht gesetzt und es braucht nicht releaseDrawOptions() aufgerufen werden
        __declspec( dllexport ) bool setDrawOptions( int x, int y, int br, int hi );
        // Begrenzt die Zeichenfläche in die bis zum nächsten aufruf von releaseDrawOptions() gezeichnet werden kann. Alles außerhalb der Fläche wird automatisch ignoriert.
        // Wenn die Zeichenfläche darf über den Rand einer bestehenden Zeichenfläche hinausragen
        //  pos: Die Koordinaten der linken oberen Ecke der Zeichenfläche
        //  gr: Die Größe der Zeichenfläche
        //  return: 1, falls die neue Zeichenfläche ganz oder teilweise innerhalb des Bildes ligt. Wenn 0 zurückgegeben wird, wurde die Zeichenfläche nicht gesetzt und es braucht nicht releaseDrawOptions() aufgerufen werden
        __declspec( dllexport ) bool setDrawOptionsErzwingen( const Punkt &pos, const Punkt &gr );
        // Begrenzt die Zeichenfläche in die bis zum nächsten aufruf von releaseDrawOptions() gezeichnet werden kann. Alles außerhalb der Fläche wird automatisch ignoriert.
        // Wenn die Zeichenfläche darf über den Rand einer bestehenden Zeichenfläche hinausragen
        //  x: Die X Koordinate der linken oberen Ecke der Zeichenfläche
        //  y: Die X Koordinate der linken oberen Ecke der Zeichenfläche
        //  br: Die Breite der Zeichenfläche
        //  hi: Die Höhe der Zeichenfläche
        //  return: 1, falls die neue Zeichenfläche ganz oder teilweise innerhalb des Bildes ligt. Wenn 0 zurückgegeben wird, wurde die Zeichenfläche nicht gesetzt und es braucht nicht releaseDrawOptions() aufgerufen werden
        __declspec( dllexport ) bool setDrawOptionsErzwingen( int x, int y, int br, int hi );
        // Setzt Koordinaten, die bei den Zeichenfunktionen von den Positionen abgezogen werden
        // Vorher sollte eine Zeichenfläche mit SetDrawOptions gesetzt worden sein. Die Werte werden mit dem aufruf von releaseDrawOptions() wieder zurückgesetzt.
        //  xOff: Ein Wert, der von allen X Koordinaten abgezogen wird
        //  yOff: Ein Wert, der von allen Y Koordinaten abgezogen wird
        __declspec( dllexport ) void addScrollOffset( int xOff, int yOff );
        // Entfernt die als letztes gesetzte begrenzung der Zeichenfläche und alle seit dem gesetzten Scroll Offsets
        // Stellt die vorherige Zeichenfläche und die zugehörigen Scroll Offsets wieder her
        __declspec( dllexport ) void releaseDrawOptions();
        // Gibt zurück, ob sich das Bild seit dem letzten Aufruf dieser Funktion verändert hat
        __declspec( dllexport ) bool getRend();
        // Gibt den Array mit A8R8G8B8 Farbwerten der Pixel zurück
        // Der Index eines Pixels berechnet sich durch x + y * Bildbreite
        __declspec( dllexport ) int *getBuffer() const;
        // Gibt den A8R8G8B8 Farbwert eines Pixels zurück
        //  x: Die X Koordinate des Pixels
        //  y: Die Y Koordinate des Pixels
        __declspec( dllexport ) int getPixel( int x, int y ) const;
        // Gibt die Größe des Bildes zurück
        __declspec( dllexport ) const Punkt &getSize() const;
        // Gibt die Breite des Bildes zurück
        __declspec( dllexport ) int getBreite() const;
        // Gibt die Höhe des Bildes zurück
        __declspec( dllexport ) int getHeight() const;
        // Gibt den aktuellen mindest Transparenz Wert zurück
        __declspec( dllexport ) unsigned char getAlpha() const;
        // Gibt die Koordinaten der linken oberen Ecke der aktuellen Zeichenfläche zurück
        __declspec( dllexport ) const Punkt &getDrawPos() const;
        // Gibt die Größe der aktuelen Zeichenfläche zurück
        __declspec( dllexport ) const Punkt &getDrawGr() const;
        // Gibt die Koordinaten zurück, die zu allen Positionen vor dem Zeichnen hinzugefügt werden
        __declspec( dllexport ) const Punkt &getDrawOff() const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) Bild *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) Bild *release();
    };
#ifdef WIN32

    // Eine Zeichnung des 2d GUI Frameworks, die ein Bild anzeigt.
    class BildZ : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Alpha = 0x1000; // Wenn dieser Flag gesetzt ist, wird zum Zeichnen des Bildes Alpha Blending verwendet

            const static __int64 normal = HScroll | Sichtbar | Erlaubt | Rahmen | VScroll; // Der Normale Style einer Bild Zeichnung Bestehend aus HScroll, Sichtbar, Erlaubt, Rahmen, VScroll
        };
    private:
        Bild *bild;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) BildZ();
        // Destruktor 
        __declspec( dllexport ) ~BildZ();
        // Setzt einen Zeiger auf das Bild, was angezeigt werden soll
        //  b: Das Bild
        __declspec( dllexport ) void setBildZ( Bild *b );
        // Setzt das Bild, was angezeigt werden soll. Der Inhalt des Bildes wird Kopiert
        //  b: Das Bild
        __declspec( dllexport ) void setBild( Bild *b );
        // Updated den Zeichenhintergrund
        //  tickVal: Die vergangene Zeit in Sekunden, die seit dem Letzten Aufruf dieser Funktion verstrichen ist
        //  return: 1, wenn das Bild neu gezeichnet werden muss. 0 sonnst
        __declspec( dllexport ) bool tick( double tickVal ) override;
        // Verarbeitet ein Maus Ereignis. Wird vom Framework automatisch aufgerufen.
        //  me: Das Ereignis
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Zeichnet die Zeihnung in ein bestimmtes Bild
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt das angezeigte Bild zurück
        __declspec( dllexport ) Bild *getBild() const;
        // Gbt das angezeigte Bild ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Bild *zBild() const;
        // Kopiert die Komplette Zeichnung, so dass sie ohne Effekt auf das Original verändert werden kann
        __declspec( dllexport ) Zeichnung *dublizieren() const override;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) BildZ *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) BildZ *release();
    };

    // Lädt ein Bild aus einer .bmp, .jpg, .gif oder .png Datei
    //  pfad: Der Pfad zur Bilddatei
    //  zError: Ein Zeiger auf ein Text Objekt, in dem ein möglicher Fehler zurückgegeben wird
    //  return: Das geladene Bild
    __declspec( dllexport ) Bild *ladeBild( char *pfad, Text *zError );
#endif
}
#endif