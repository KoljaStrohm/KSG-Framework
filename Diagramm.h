#ifndef Diagramm_H
#define Diagramm_H

#include "Zeichnung.h"
#include "Array.h"

namespace Framework
{
    class LRahmen; // Rahmen.h
    class AlphaFeld; // AlphaFeld.h
    class Text; // Text.h
    class Schrift; // Schrift.h
    class HScrollBar; // Scroll.h
    class VScrollBar; // Scroll.h
    class SLDiag; // aus dieser Datei
    class LDiag; // aus dieser Datei

    class SLDiag : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Gitter = 0x01000;
            const static __int64 LinienName = 0x02000;

            const static __int64 normal = Sichtbar | Hintergrund | Rahmen | Gitter | LinienName;
        };
    private:
        int gF;
        Array< int > *lFarbe;
        RCArray< Text > *lName;
        RCArray< Array< int > > *ph;
        RCArray< Array< int > > *pb;
        Array< int > *lastValue;
        Schrift *schrift;
        Punkt gitterGr;
        int lines;
        int ref;

    public:
        // Konstruktor 
        __declspec( dllexport ) SLDiag();
        // Destruktor 
        __declspec( dllexport ) ~SLDiag();
        // nicht constant 
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift ); // setzt die Schrift
        __declspec( dllexport ) void setGGröße( Punkt &gr ); // setzt die Größe des Gitters
        __declspec( dllexport ) void setGFarbe( int f ); // setzt die Gitter Farbe
        __declspec( dllexport ) void addLinie( const char *name ); // fügt eine Linie hinzu
        __declspec( dllexport ) void addLinie( Text *txt );
        __declspec( dllexport ) void setLFarbe( int lNum, int f ); // setzt die Linienfarbe
        __declspec( dllexport ) void addPunkt( int lNum, int x, int h ); // fügt einen Punkt hinzu
        __declspec( dllexport ) void removeLinie( int lNum ); // entfernt eine Linie
        __declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
        // constant
        __declspec( dllexport ) Schrift *getSchrift() const; // gibt die Schrift zurück
        __declspec( dllexport ) Schrift *zSchrift() const;
        __declspec( dllexport ) const Punkt &getGGröße() const; // gibt die Gitter Größe zurück
        __declspec( dllexport ) int getGFarbe() const; // gibt die Gitter Farbe zurück
        __declspec( dllexport ) int getLinienNummer( const char *name ) const; // gibt die Linien Nummer zurück
        __declspec( dllexport ) int getLinienNummer( Text *name ) const;
        __declspec( dllexport ) Text *getLinienName( int lNum ) const; // gibt den Linien Namen zurück
        __declspec( dllexport ) Text *zLinienNamen( int lNum ) const;
        __declspec( dllexport ) int getHöchstValue() const; // gibt den Höchsten Wert zurück
        __declspec( dllexport ) int getHöchstValue( int lNum ) const;
        __declspec( dllexport ) int getMedian( int lNum ) const; // gibt den durchschnittswert zurück
        __declspec( dllexport ) int getLAnzahl() const; // gibt die Linien Anzahl zurück
        __declspec( dllexport ) int getLastValue( int lNum ) const; // gibt den letzten Wert zurück
        // Reference Counting 
        __declspec( dllexport ) SLDiag *getThis();
        __declspec( dllexport ) SLDiag *release();
    };

    // Punkt eines Wertes eines Diagramms
    struct DiagPunkt
    {
        // Position des Punktes auf dem Horizontalen Intervall des Diagramms
        double hIntervall;
        // Position des punktes auf dem Vertikalen Intervall des Diagramms
        double vIntervall;
    };

    // Wert der in einem Diagramm visualisiert wird
    struct DiagWert
    {
        // Style eines Diagramm Wertes
        class Style
        {
        public:
            const static int Sichtbar = 0x01;
            const static int Alpha = 0x02;
            const static int Hintergrund = 0x04;
            const static int HAlpha = 0x08;
            const static int Name = 0x10;
        };
        // Style des Wertes
        int style;
        // Farbe des Wertes
        int farbe;
        // Hintergrundfarbe des Wertes
        int hintergrund;
        // Name des Wertes
        Text *name;
        // Punkte des Wertes
        Array< DiagPunkt* > *punkte;

        // Konstruktor
        __declspec( dllexport ) DiagWert();
        // Destruktor
        __declspec( dllexport ) ~DiagWert();
        // Referenc Counting
        __declspec( dllexport ) DiagWert *getThis();
        __declspec( dllexport ) DiagWert *release();

    private:
        // Reference Counting
        int ref;
    };

    // Daten für ein Diagramm
    struct DiagDaten
    {
        class Style
        {
        public:
            const static int Sichtbar = 0x0001;
            const static int Raster = 0x0002;
            const static int RasterAlpha = 0x0004;
            const static int AutoIntervallHöhe = 0x0008;
            const static int AutoIntervallBreite = 0x0010;
            const static int HIntervall = 0x0020;
            const static int VIntervall = 0x0040;
            const static int HIntervallTexte = 0x0200;
            const static int VIntervallTexte = 0x0400;
            const static int AutoRasterBreite = 0x0800;
            const static int AutoRasterHöhe = 0x1000;

            const static int intervalle = 0x0060;
            const static int intervallTexte = 0x0600;
            const static int autoRaster = 0x1802;
            const static int autoIntervall = 0x0018;
        };
        // Style eines Diagramms
        int style;
        // Breite einer Rasterlinie
        int rasterDicke;
        // Breite eines Rasterkästchens
        double rasterBreite;
        // Höhe eines Rasterkästchens
        double rasterHöhe;
        // Farbe des Rasters
        int rasterFarbe;
        // Schriftfarbe des Horizontalen Intervalls
        int hIntervallFarbe;
        // Schriftfarbe des Vertikalen Intervalls
        int vIntervallFarbe;
        // Name des Horizontalen Intervalls
        Text *hIntervallName;
        // Name des Vertikalen Intervalls
        Text *vIntervallName;
        // Anzeigen des Horizontalen Intervalls
        RCArray< Text > *hIntervallTexte;
        // Anzeigen des Vertikalen Intervalls
        RCArray< Text > *vIntervallTexte;
        // einzelne Horizontale Intervalle
        Array< double > *hIntervallWerte;
        // einzelne Vertikale Intervalle
        Array< double > *vIntervallWerte;
        // Breite des Wertes 1 in Pixeln bei dem horizontalen Intervall ( wird durch das setzen von AutoIntervallBreite überschrieben )
        double hIntervallBreite;
        // Höhe des Wertes 1 in Pixeln bei dem vertikalen Intervall ( wird durch das setzen von AutoIntervallHöhe überschrieben )
        double vIntervallHöhe;
        // Werte, die in dem Diagramm visualisiert werden
        RCArray< DiagWert > *werte;

        // Konstruktor
        __declspec( dllexport ) DiagDaten();
        // Destruktor
        __declspec( dllexport ) ~DiagDaten();
        // Referenc Counting
        __declspec( dllexport ) DiagDaten *getThis();
        __declspec( dllexport ) DiagDaten *release();

    private:
        // Reference Counting
        int ref;
    };

    // Basisklasse der verschiedenen Diagramm Arten
    class BaseDiag
    {
    protected:
        DiagDaten *daten;
        bool changed;
        CRITICAL_SECTION *lock;
        int ref;

    public:
        // Konstruktor
        __declspec( dllexport ) BaseDiag( CRITICAL_SECTION *lock );
        // Destruktor
        __declspec( dllexport ) ~BaseDiag();
        // nicht constant
        // Setzt einen Zeiger auf die Daten des Diagramms
        __declspec( dllexport ) void setDiagDatenZ( DiagDaten *dd );
        // Kopiert die Daten eines Diagramms
        __declspec( dllexport ) void setDiagDaten( DiagDaten *dd );
        // Rasterdicke setzen
        __declspec( dllexport ) void setRasterDicke( int d );
        // Rasterbreite setzen
        __declspec( dllexport ) void setRasterBreite( int br );
        // Rasterhöhe setzen
        __declspec( dllexport ) void setRasterHöhe( int hö );
        // Rasterfarbe setzen
        __declspec( dllexport ) void setRasterFarbe( int f );
        // Intervall Breite
        __declspec( dllexport ) void setHIntervallBreite( double br );
        // Intervall Höhe
        __declspec( dllexport ) void setVIntervallHöhe( double hö );
        // Farbe des Horizontalen Intervalls
        __declspec( dllexport ) void setHIntervallFarbe( int f );
        // Farbe des Vertikalen Intervalls
        __declspec( dllexport ) void setVIntervallFarbe( int f );
        // Setzt den Namen des Horizontalen Intervalls
        __declspec( dllexport ) void setHIntervallName( char *name );
        __declspec( dllexport ) void setHIntervallName( Text *name );
        // Setzt den Namen des Vertikalen Intervalls
        __declspec( dllexport ) void setVIntervallName( char *name );
        __declspec( dllexport ) void setVIntervallName( Text *name );
        // Text eines Horizontalen Intervalls hinzufügen
        __declspec( dllexport ) void addHIntervallText( double hIntervall, char *text );
        __declspec( dllexport ) void addHIntervallText( double hIntervall, Text *text );
        // Setzt den Text eines Horizontalen Intervalls
        __declspec( dllexport ) void setHIntervallTextZ( double hIntervall, Text *text );
        __declspec( dllexport ) void setHIntervallText( double hIntervall, Text *text );
        __declspec( dllexport ) void setHIntervallText( double hIntervall, char *text );
        // Text eines Horizontalen Intervalls entfernen
        __declspec( dllexport ) void löscheHIntervallText( double hIntervall );
        // Text eines Vertikalen Intervalls hinzufügen
        __declspec( dllexport ) void addVIntervallText( double vIntervall, char *text );
        __declspec( dllexport ) void addVIntervallText( double vIntervall, Text *text );
        // Setzt den Text eines Vertikalen Intervalls
        __declspec( dllexport ) void setVIntervallTextZ( double vIntervall, Text *text );
        __declspec( dllexport ) void setVIntervallText( double vIntervall, Text *text );
        __declspec( dllexport ) void setVIntervallText( double vIntervall, char *text );
        // Text eines Vertikalen Intervalls entfernen
        __declspec( dllexport ) void löscheVIntervallText( double vIntervall );
        // Fügt einen Wert hinzu
        __declspec( dllexport ) void addWertZ( DiagWert *w );
        __declspec( dllexport ) void addWert( DiagWert *w );
        __declspec( dllexport ) void addWert( const char *name );
        __declspec( dllexport ) void addWert( Text *txt );
        // setzt die Farbe eines Wertes
        __declspec( dllexport ) void setWertFarbe( int wNum, int fc );
        // fügt einem Wert einen Punkt hinzu
        __declspec( dllexport ) void addPunktZ( int wNum, DiagPunkt *p );
        __declspec( dllexport ) void addPunkt( int wNum, DiagPunkt *p );
        __declspec( dllexport ) void addPunkt( int wNum, double hI, double vI );
        // Ändert einen vorhandenen Punkt eines Wertes
        __declspec( dllexport ) void setPunktZ( int wNum, double hI, DiagPunkt *p );
        __declspec( dllexport ) void setPunkt( int wNum, double hI, DiagPunkt *p );
        __declspec( dllexport ) void setPunkt( int wNum, double hI, double h, double v );
        __declspec( dllexport ) void setPunktZ( int wNum, int pNum, DiagPunkt *p );
        __declspec( dllexport ) void setPunkt( int wNum, int pNum, DiagPunkt *p );
        __declspec( dllexport ) void setPunkt( int wNum, int pNum, double h, double v );
        // Löscht einen vorhandenen Punkt
        __declspec( dllexport ) void löschePunkt( int wNum, double hI );
        __declspec( dllexport ) void löschePunkt( int wNum, int pNum );
        // entfernt einen Wert
        __declspec( dllexport ) void removeWert( int wNum );
        __declspec( dllexport ) void removeWert( char *name );
        __declspec( dllexport ) void removeWert( Text *name );
        // Setzt den Style der Daten
        __declspec( dllexport ) void addDatenStyle( int style );
        __declspec( dllexport ) void setDatenStyle( int style );
        __declspec( dllexport ) void setDatenStyle( int style, bool addRemove );
        __declspec( dllexport ) void löscheDatenStyle( int style );
        // Setzt den Style eines Wertes
        __declspec( dllexport ) void addWertStyle( int wNum, int style );
        __declspec( dllexport ) void setWertStyle( int wNum, int style );
        __declspec( dllexport ) void setWertStyle( int wNum, int style, bool addRemove );
        __declspec( dllexport ) void löscheWertStyle( int wNum, int style );
        // constant
        // Gibt die Daten des Diagramms zurück
        __declspec( dllexport ) DiagDaten *getDiagDaten() const;
        __declspec( dllexport ) DiagDaten *zDiagDaten() const;
        // Gibt die Daten eines Wertes zurück
        __declspec( dllexport ) DiagWert *getDiagWert( int wNum ) const;
        __declspec( dllexport ) DiagWert *zDiagWert( int wNum ) const;
        __declspec( dllexport ) DiagWert *getDiagWert( char *name ) const;
        __declspec( dllexport ) DiagWert *zDiagWert( char *name ) const;
        // Gibt die Position eines Wertes zurück
        __declspec( dllexport ) int getDiagWertPos( char *name ) const;
        // Gibt die Position eines Punktes von einem Wert zurück
        __declspec( dllexport ) int getDiagPunktPos( int wNum, double hI ) const;
        __declspec( dllexport ) int getDiagPunktPos( char *wName, double hI ) const;
        // Prüft den Style der Daten
        __declspec( dllexport ) inline bool hatDatenStyle( int style ) const;
        __declspec( dllexport ) inline bool hatDatenStyleNicht( int style ) const;
        // Prüft den Style eines Wertes
        __declspec( dllexport ) inline bool hatWertStyle( int wNum, int style ) const;
        __declspec( dllexport ) inline bool hatWertStyleNicht( int wNum, int style ) const;
        // Reference Counting
        __declspec( dllexport ) BaseDiag *getThis();
        __declspec( dllexport ) virtual BaseDiag *release();
    };

    // Linien Diagramm
    class LDiag : public ZeichnungHintergrund, public BaseDiag
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 DatenRahmen = 0x0001000;
            const static __int64 DatenHintergrund = 0x02000;
            const static __int64 DatenHAlpha = 0x04000;
            const static __int64 DatenHBild = 0x008000;
            const static __int64 DatenBuffered = 0x010000;

            const static __int64 scroll = VScroll | HScroll;
            const static __int64 normal = Hintergrund | scroll | DatenRahmen;
        };
    private:
        Schrift *schrift;
        LRahmen *dRam;
        int dBgF;
        Bild *dBgB;
        AlphaFeld *dAf;
        Bild *vIntervallRB;
        Bild *hIntervallRB;
        int schriftGr;

    public:
        // Konstruktor
        __declspec( dllexport ) LDiag();
        // Destruktor
        __declspec( dllexport ) ~LDiag();
        // nicht constant
        // Setzt die Schrift
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        __declspec( dllexport ) void setSchriftGröße( int gr );
        // Setzt den inneren Rahmen um das eigentliche Diagramm
        __declspec( dllexport ) void setDatenRahmenZ( LRahmen *ram );
        __declspec( dllexport ) void setDatenRahmen( LRahmen *ram );
        __declspec( dllexport ) void setDatenRahmenBreite( int br );
        __declspec( dllexport ) void setDatenRahmenFarbe( int fc );
        // Setzt den Hintergrund des eigentlichen Diagramms
        __declspec( dllexport ) void setDatenHintergrundFarbe( int fc );
        __declspec( dllexport ) void setDatenHintergrundBildZ( Bild *b );
        __declspec( dllexport ) void setDatenHintergrundBild( Bild *b );
        // Setzt das AlphaFeld des eigentlichen Diagramms
        __declspec( dllexport ) void setDatenAlphaFeldZ( AlphaFeld *af );
        __declspec( dllexport ) void setDatenAlphaFeld( AlphaFeld *af );
        __declspec( dllexport ) void setDatenAlphaFeldFarbe( int fc );
        __declspec( dllexport ) void setDatenAlphaFeldStärke( int st );
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // constant
        // Gibt die Schrift zurück
        __declspec( dllexport ) Schrift *getSchrift() const;
        __declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt den inneren Rahmen um das eigentliche Diagramm zurück
        __declspec( dllexport ) LRahmen *getDatenRahmen() const;
        __declspec( dllexport ) LRahmen *zDatenRahmen() const;
        __declspec( dllexport ) int getDatenRahmenFarbe() const;
        __declspec( dllexport ) int getDatenRahmenBreite() const;
        // Gibt den Hintergrund des eigentlichen Diagramms zurück
        __declspec( dllexport ) int getDatenHintergrundFarbe() const;
        __declspec( dllexport ) Bild *getDatenHintergrundBild() const;
        __declspec( dllexport ) Bild *zDatenHintergrundBild() const;
        // Gibt das AlphaFeld des eigentlichen Diagramms zurück
        __declspec( dllexport ) AlphaFeld *getDatenAlphaFeld() const;
        __declspec( dllexport ) AlphaFeld *zDatenAlphaFeld() const;
        __declspec( dllexport ) int getDatenAlphaFeldFarbe() const;
        __declspec( dllexport ) int getDatenAlphaFeldStärke() const;
        // Reference Counting
        __declspec( dllexport ) BaseDiag *release() override;
    };
}

#endif