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

    // Eine Zeichnung de 2D GUI Frameworks, die ein live Diagramm von werten zeichnet
    class SLDiag : public ZeichnungHintergrund
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Gitter = 0x01000; // Es wird ein Gitter im Diagramm zur Orientierung gezeichnet
            const static __int64 LinienName = 0x02000; // Es werden die Namen der Graphen angezeigt

            const static __int64 normal = Sichtbar | Hintergrund | Rahmen | Gitter | LinienName; // Vereinigung der Flags Sichtbar, Hintergrund, Rahmen, Gitter, LinienName
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
        __declspec( dllexport ) virtual ~SLDiag();
        // Setzt einen Zeiger zur verwendeten Schrift
        //  schrift: Die Schrift
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        // Setzt die Abstände zwischen den Linien des Gitters
        //  gr: Für x den Abstand zwischen jeder vertikalen Linie in Pixeln und für y den Abstand zwischen jeder Horizontalen Linie in den dargestellen y Werten
        __declspec( dllexport ) void setGSize( Punkt &gr );
        // Setzt die Farbe der Raster Linien
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setGFarbe( int f );
        // Fügt dem Diagramm einen neuen Graphen hinzu
        //  name: Der Name des Graphen. Er wird bei dem Flag LinienName im Diagramm angezeigt
        __declspec( dllexport ) void addLinie( const char *name );
        // Fügt dem Diagramm einen neuen Graphen hinzu
        //  name: Der Name des Graphen. Er wird bei dem Flag LinienName im Diagramm angezeigt
        __declspec( dllexport ) void addLinie( Text *txt );
        // Setzt die Farbe eines Graphen
        //  lNum: Der Index des Graphen
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setLFarbe( int lNum, int f );
        // Fügt einem Graphen einen Punkt hinzu. Hierdurch verschieben sich alle Punkte des Graphen so weit nach Links, dass der neue Punkt rechts auftaucht. Geht ein Punkt links aus dem Diagramm raus, wird er gelöscht.
        // Auf diese Weise erscheint das Diagramm wie eine Abbildung einer Live Messung
        //  x: Der Abstand zum Letzten Punkt in Pixeln
        //  y: Den Wert der abgebildeten Funktion in dem Punkt
        __declspec( dllexport ) void addPunkt( int lNum, int x, int h );
        // Entfernt einen Graphen
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) void removeLinie( int lNum );
        // Zeichnet das Diagramm
        //  zRObj: Das Bild, in das gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die verwendete Schrift zurück
        __declspec( dllexport ) Schrift *getSchrift() const;
        // Gibt die verwendete Schrift ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt die Größe eines Quadrats des Gitters zurück, wobei die Braite in Pixeln und die Höhe in Werten der y Achse ist
        __declspec( dllexport ) const Punkt &getGSize() const;
        // Gibt die Farbe des Gitters in A8R8G8B8 Format zurück
        __declspec( dllexport ) int getGFarbe() const;
        // Gibt den Index eines Graphen zurück
        //  name: Der Name des Graphen
        __declspec( dllexport ) int getLinienNummer( const char *name ) const;
        // Gibt den Index eines Graphen zurück
        //  name: Der Name des Graphen
        __declspec( dllexport ) int getLinienNummer( Text *name ) const;
        // Gibt den Namen eines Graphen zurück
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) Text *getLinienName( int lNum ) const;
        // Gibt den Namen eines Graphen ohne erhöhten Reference Counter zurück
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) Text *zLinienNamen( int lNum ) const;
        // Gibt den Höchsten y Wert von allen Graphen zurück
        __declspec( dllexport ) int getHighestValue() const;
        // Gibt den Höchsten y Wert von einem Bestimmten Graphen zurück
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) int getHighestValue( int lNum ) const;
        // Gibt den Durchschnittswert aller im Diagramm gespeicherten Punkten eines Graphen zurück
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) int getMedian( int lNum ) const;
        // Gibt die Anzahl an Graphen zurück
        __declspec( dllexport ) int getLAnzahl() const;
        // Gibt den y Wert des letzten Punktes eines Graphen zurück
        //  lNum: Der Index des Graphen
        __declspec( dllexport ) int getLastValue( int lNum ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) SLDiag *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
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
            const static int Sichtbar = 0x01; // Legt fest, ob der Wert angezeigt werden soll
            const static int Alpha = 0x02; // Legt fest, ob beim zeichnen des Wertes Alphablending verwendet werden soll
            const static int Hintergrund = 0x04; // Legt fest, ob der Wert einen Hintergrund hat
            const static int HAlpha = 0x08; // Legt fest, ob beim zeichnen des Hintergrundes Alphablending verwendet werden soll
            const static int Name = 0x10; // Legt fest, ob der Name des Wertes gezeichnet werden soll
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
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) DiagWert *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
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
            const static int Sichtbar = 0x0001; // Legt Fest, ob die Daten sichtbar sein sollen
            const static int Raster = 0x0002; // Legt fest, ob ein Raster zur Orientierung angezeigt werden soll
            const static int RasterAlpha = 0x0004; // Legt fest, ob zum Zeichnen des Rasters Alphablending verwendet werden soll
            const static int AutoIntervallHeight = 0x0008; // Legt fest, dass alle Werte automatisch so auf der y Achse Skalliert werden, dass immer die komplette Höhe des Diagramms genutzt wird
            const static int AutoIntervallBreite = 0x0010; // Legt fest, dass alle Wert automatisch so auf der x Achse Skalliert werden, dass immer die komplette Breite des Diagramms genutzt wird
            const static int HIntervall = 0x0020; // Legt fest, ob die X Achse gezeichnet werden soll
            const static int VIntervall = 0x0040; // Legt fest, ob die Y Achse gezeichnet werden soll
            const static int HIntervallTexte = 0x0200; // Legt fest, ob spezielle Texte für die Werte der X Achse gezeichnet werden sollen
            const static int VIntervallTexte = 0x0400; // Left fest, ob spezielle Texte für die Werte der Y Achse gezeichnet werden sollen
            const static int AutoRasterBreite = 0x0800; // Legt fest, ob der Abstand zwischen den Vertikalen Linien des Rasters automatisch gewählt werden soll
            const static int AutoRasterHeight = 0x1000; // Legt fest, ob der Abstand zwischen den Horizontalen Rasterlinien automatisch gewählt werden soll

            const static int intervalle = HIntervall | VIntervall; // Vereiniegt die Flags HIntervall, VIntervall
            const static int intervallTexte = HIntervallTexte | VIntervallTexte; // Vereint die Flags HIntervallTexte, VIntervallTexte
            const static int autoRaster = AutoRasterHeight | AutoRasterBreite | Raster; // Vereint die Flags AutoRasterHöhe, AutoRasterBreite, Raster
            const static int autoIntervall = AutoIntervallHeight | AutoIntervallBreite; // Vereint die Flags AutoIntervallHöhe, AutoIntervallBreite
        };
        // Style eines Diagramms
        int style;
        // Breite einer Rasterlinie
        int rasterDicke;
        // Breite eines Rasterkästchens
        double rasterBreite;
        // Höhe eines Rasterkästchens
        double rasterHeight;
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
        double vIntervallHeight;
        // Werte, die in dem Diagramm visualisiert werden
        RCArray< DiagWert > *werte;

        // Konstruktor
        __declspec( dllexport ) DiagDaten();
        // Destruktor
        __declspec( dllexport ) ~DiagDaten();
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) DiagDaten *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
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
        //  lock: Ein Zeiger zur CRITICAL_SECTION, mit der die Diagramm Klasse, die von dieser Klasse erbt Multithread sicher gemacht wird
        __declspec( dllexport ) BaseDiag( CRITICAL_SECTION *lock );
        // Destruktor
        __declspec( dllexport ) virtual ~BaseDiag();
        // Setzt einen Zeiger auf die Daten des Diagramms
        //  dd: Die Daten
        __declspec( dllexport ) void setDiagDatenZ( DiagDaten *dd );
        // Kopiert die Daten eines Diagramms
        //  dd: Die Daten
        __declspec( dllexport ) void setDiagDaten( DiagDaten *dd );
        // Rasterdicke setzen
        //  d: Die Breite einer Rasterlinie in Pixeln
        __declspec( dllexport ) void setRasterDicke( int d );
        // Rasterbreite setzen
        //  br: Der Abstand zwischen zwei Vertikalen Rasterlinien in Pixeln
        __declspec( dllexport ) void setRasterBreite( int br );
        // Rasterhöhe setzen
        //  hi: Der Abstand zwischen zwei Horizontalen Rasterlinien in Pixeln
        __declspec( dllexport ) void setRasterHeight( int hi );
        // Rasterfarbe setzen
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setRasterFarbe( int f );
        // Setzt die Intervall Breite
        //  br: Der Abstand zwischen den Werten 0 und 1 auf der X Achse in Pixeln
        __declspec( dllexport ) void setHIntervallBreite( double br );
        // Setzt die Intervall Höhe
        //  hi: Der Abstand zwischen den Werten 0 und 1 auf der Y Achse in Pixeln
        __declspec( dllexport ) void setVIntervallHeight( double hi );
        // Setzt die Farbe der X Achse
        //  f: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setHIntervallFarbe( int f );
        // Setzt die Farbe der Y Achse
        //  f: Die Farbe im A8R8G8B8 Formats
        __declspec( dllexport ) void setVIntervallFarbe( int f );
        // Setzt den Namen der X Achse
        //  name: Der Name
        __declspec( dllexport ) void setHIntervallName( char *name );
        // Setzt den Namen der X Achse
        //  name: Der Name
        __declspec( dllexport ) void setHIntervallName( Text *name );
        // Setzt den Namen der Y Achse
        //  name: Der Name
        __declspec( dllexport ) void setVIntervallName( char *name );
        // Setzt den Namen der Y Achse
        //  name: Der Name
        __declspec( dllexport ) void setVIntervallName( Text *name );
        // Beschriftet einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void addHIntervallText( double hIntervall, char *text );
        // Beschriftet einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void addHIntervallText( double hIntervall, Text *text );
        // setzt den Zeiger auf eine Beschriftung in einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setHIntervallTextZ( double hIntervall, Text *text );
        // Ändert eine Beschriftung in einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setHIntervallText( double hIntervall, Text *text );
        // Ändert eine Beschriftung in einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setHIntervallText( double hIntervall, char *text );
        // Entfernt eine Beschriftung in einen Betimmten Wert der X Achse, falls der Flag HIntervallTexte in den DiagDaten gesetzt wurde
        //  hIntervall: Der Wert, der nicht mehr beschriftet sein soll
        __declspec( dllexport ) void removeHIntervallText( double hIntervall );
        // Beschriftet einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void addVIntervallText( double vIntervall, char *text );
        // Beschriftet einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void addVIntervallText( double vIntervall, Text *text );
        // setzt den Zeiger auf eine Beschriftung in einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setVIntervallTextZ( double vIntervall, Text *text );
        // Ändert eine Beschriftung in einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setVIntervallText( double vIntervall, Text *text );
        // Ändert eine Beschriftung in einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der beschriftet werden soll
        //  text: Die Beschriftung
        __declspec( dllexport ) void setVIntervallText( double vIntervall, char *text );
        // Entfernt eine Beschriftung in einen Betimmten Wert der Y Achse, falls der Flag VIntervallTexte in den DiagDaten gesetzt wurde
        //  vIntervall: Der Wert, der nicht mehr beschriftet sein soll
        __declspec( dllexport ) void removeVIntervallText( double vIntervall );
        // Fügt einen im Diagramm dargestellten Wert (Graph) hinzu
        //  w: Der neue Wert
        __declspec( dllexport ) void addWertZ( DiagWert *w );
        // Fügt einen im Diagramm dargestellten Wert (Graph) hinzu, indem ein anderer Kopiert wird
        //  w: Der Wert, der Kopiert werden soll
        __declspec( dllexport ) void addWert( DiagWert *w );
        // Fügt einen im Diagramm dargestellten Wert (Graph) hinzu
        //  name: Der Name des Wertes
        __declspec( dllexport ) void addWert( const char *name );
        // Fügt einen im Diagramm dargestellten Wert (Graph) hinzu
        //  name: Der Name des Wertes
        __declspec( dllexport ) void addWert( Text *txt );
        // setzt die Farbe eines Wertes
        //  wNum: Der Index des Wertes
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setWertFarbe( int wNum, int fc );
        // fügt einem Wert einen Punkt hinzu
        //  wNum: Der Index des Wertes
        //  p: Der neue Punkt
        __declspec( dllexport ) void addPunktZ( int wNum, DiagPunkt *p );
        // fügt einem Wert einen Punkt hinzu, indem ein Punkt kopiert wird
        //  wNum: Der Index des Wertes
        //  p: Der Punkt, der kopiert werden soll
        __declspec( dllexport ) void addPunkt( int wNum, DiagPunkt *p );
        // Fügt einem Wert einen Punkt hinzu
        //  wNum: Der Index des Wertes
        //  hI: Der Wert des Punktes auf der X Achse
        //  vI: Der Wert des Punktes auf der Y Achse
        __declspec( dllexport ) void addPunkt( int wNum, double hI, double vI );
        // Setzt einen Zeiger auf einen vorhandenen Punkt eines Wertes
        //  wNum: Der Index des Wertes
        //  hI: Der Wert des Punktes auf der X Achse, der ersetzt werden soll
        //  p: Der neue Punkt
        __declspec( dllexport ) void setPunktZ( int wNum, double hI, DiagPunkt *p );
        // Setzt einen vorhandenen Punkt eines Wertes, indem ein Punkt kopiert wird
        //  wNum: Der Index des Wertes
        //  hI: Der Wert des Punktes auf der X Achse, der ersetzt werden soll
        //  p: Der neue Punkt
        __declspec( dllexport ) void setPunkt( int wNum, double hI, DiagPunkt *p );
        // Setzt einen vorhandenen Punkt eines Wertes
        //  wNum: Der Index des Wertes
        //  hI: Der Wert des Punktes auf der X Achse, der ersetzt werden soll
        //  h: Der Wert auf der X Achse des neuen Punktes
        //  v: Der Wert auf der Y Achse des neuen Punktes
        __declspec( dllexport ) void setPunkt( int wNum, double hI, double h, double v );
        // Setzt einen Zeiger auf einen vorhandenen Punkt eines Wertes
        //  wNum: Der Index des Wertes
        //  pNum: Der Index des Punktes im Wert
        //  p: Der neue Punkt
        __declspec( dllexport ) void setPunktZ( int wNum, int pNum, DiagPunkt *p );
        // Setzt einen vorhandenen Punkt eines Wertes, indem ein Punkt kopiert wird
        //  wNum: Der Index des Wertes
        //  pNum: Der Index des Punktes im Wert
        //  p: Der neue Punkt
        __declspec( dllexport ) void setPunkt( int wNum, int pNum, DiagPunkt *p );
        // Setzt einen vorhandenen Punkt eines Wertes
        //  wNum: Der Index des Wertes
        //  pNum: Der Index des Punktes im Wert
        //  h: Der Wert auf der X Achse des neuen Punktes
        //  v: Der Wert auf der Y Achse des neuen Punktes
        __declspec( dllexport ) void setPunkt( int wNum, int pNum, double h, double v );
        // Löscht einen vorhandenen Punkt
        //  wNum: Der Index des Wertes
        //  hI: Der Wert auf der X Achse des Punktes, der gelöscht werden soll
        __declspec( dllexport ) void removePunkt( int wNum, double hI );
        // Löscht einen vorhandenen Punkt
        //  wNum: Der Index des Wertes
        //  pNum: Der Index des Punktes im Wert
        __declspec( dllexport ) void removePunkt( int wNum, int pNum );
        // entfernt einen Wert
        //  wNum: Der Index des Wertes
        __declspec( dllexport ) void removeWert( int wNum );
        // entfernt einen Wert
        //  name: Der Name des Wertes
        __declspec( dllexport ) void removeWert( char *name );
        // entfernt einen Wert
        //  name: Der Name des Wertes
        __declspec( dllexport ) void removeWert( Text *name );
        // Fügt den DiagDaten bestimmte Styles hinzu
        //  style: Die neuen Styles
        __declspec( dllexport ) void addDatenStyle( int style );
        // Setzt die Styles der DiagDaten
        //  style: Die neuen Styles
        __declspec( dllexport ) void setDatenStyle( int style );
        // Setzt die Styles der DiagDaten
        //  style: Die Styles
        //  addRemove: 1, falls die Styles hinzugefügt werden sollem. 0, falls die Styles entfernt werden sollen
        __declspec( dllexport ) void setDatenStyle( int style, bool addRemove );
        // Entfernt Styles der DiagDaten
        //  style: Die Styles, die entfernt werden sollen
        __declspec( dllexport ) void removeDatenStyle( int style );
        // Fügt einem bestimmten Wert bestimmte Styles hinzu
        //  wNum: Der Index des Wertes
        //  style: Die neuen Styles
        __declspec( dllexport ) void addWertStyle( int wNum, int style );
        // Setzt die Styles eines Bestimmten Wertes
        //  wNum: Der Index des Wertes
        //  style: Die neuen Styles
        __declspec( dllexport ) void setWertStyle( int wNum, int style );
        // Setzt die Styles eines Bestimmten Wertes
        //  wNum: Der Index des Wertes
        //  style: Die Styles
        //  addRemove: 1, falls die Styles hinzugefügt werden sollem. 0, falls die Styles entfernt werden sollen
        __declspec( dllexport ) void setWertStyle( int wNum, int style, bool addRemove );
        // Entfernt Styles eines Bestimmten Wertes
        //  wNum: Der Index des Wertes
        //  style: Die Styles, die entfernt werden sollen
        __declspec( dllexport ) void removeWertStyle( int wNum, int style );
        // Gibt die Daten des Diagramms zurück
        __declspec( dllexport ) DiagDaten *getDiagDaten() const;
        // Gibt die Daten des Diagramms ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) DiagDaten *zDiagDaten() const;
        // Gibt die Daten eines Wertes zurück
        //  wNum: Der Index des Wertes
        __declspec( dllexport ) DiagWert *getDiagWert( int wNum ) const;
        // Gibt die Daten eines Wertes ohne erhöhten Reference Counter zurück
        //  wNum: Der Index des Wertes
        __declspec( dllexport ) DiagWert *zDiagWert( int wNum ) const;
        // Gibt die Daten eines Wertes zurück
        //  name: Der Name des Wertes
        __declspec( dllexport ) DiagWert *getDiagWert( char *name ) const;
        // Gibt die Daten eines Wertes ohne erhöhten Reference Counter zurück
        //  name: Der Name des Wertes
        __declspec( dllexport ) DiagWert *zDiagWert( char *name ) const;
        // Gibt den Index eines Wertes zurück
        //  name: Der Name des Wertes
        __declspec( dllexport ) int getDiagWertPos( char *name ) const;
        // Gibt den Index eines Punktes von einem Wert zurück
        //  wNum: Der Index des Wertes
        //  hI: Der Wert auf der X Achse des Punktes
        __declspec( dllexport ) int getDiagPunktPos( int wNum, double hI ) const;
        // Gibt den Index eines Punktes von einem Wert zurück
        //  wName: Der Name des Wertes
        //  hI: Der Wert auf der X Achse des Punktes
        __declspec( dllexport ) int getDiagPunktPos( char *wName, double hI ) const;
        // Prüft, ob bestimmte Styles in den DiagDaten gesetzt wurden
        //  style: Die Styles
        __declspec( dllexport ) inline bool hatDatenStyle( int style ) const;
        // Prüft, ob bestimmte Styles in den DiagDaten nicht gesetzt wurden
        //  style: Die Styles
        __declspec( dllexport ) inline bool hatDatenStyleNicht( int style ) const;
        // Prüft, ob bestimmte Styles für einen bestimmten Wert gesetzt wurden
        //  wNum: Der Index des Wertes
        //  style: Die Styles
        __declspec( dllexport ) inline bool hatWertStyle( int wNum, int style ) const;
        // Prüft, ob bestimmte Styles für einen bestimmten Wert nicht gesetzt wurden
        //  wNum: Der Index des Wertes
        //  style: Die Styles
        __declspec( dllexport ) inline bool hatWertStyleNicht( int wNum, int style ) const;
        // Erhöht den Reference Counting Zähler.
        //  return: this.
        __declspec( dllexport ) BaseDiag *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) virtual BaseDiag *release();
    };

    // Eine Zeichnung des 2D GUI Frameworks, die die Diagrammdaten als Liniengraphen darstellt
    class LDiag : public ZeichnungHintergrund, public BaseDiag
    {
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 DatenRahmen = 0x0001000; // Legt fest, ob um die daten ein Rahmen gezeichnet wird
            const static __int64 DatenHintergrund = 0x02000; // legt fest, ob die Daten einen Hintergrund haben
            const static __int64 DatenHAlpha = 0x04000; // legt fest, ob beim zeichnen des Hintergrunds der Daten alpha blending benutzt wird
            const static __int64 DatenHBild = 0x008000; // Legt fest, ob für den Hintergrund der Daten ein Bild verwendet werden soll
            const static __int64 DatenBuffered = 0x010000; // Legt fest, ob die Daten einen Farbübergang haben

            const static __int64 scroll = VScroll | HScroll; // Vereint die Flags VScroll und HScroll
            const static __int64 normal = Hintergrund | scroll | DatenRahmen; // Vereint die Flags Hintergrund, scroll, DatenRahmen
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
        __declspec( dllexport ) virtual ~LDiag();
        // Setzt die Schrift
        //  schrift: Die Schrift
        __declspec( dllexport ) void setSchriftZ( Schrift *schrift );
        // Setzt die größe der Schrift
        //  gr: Die Höhe einer Zeile in Pixeln
        __declspec( dllexport ) void setSchriftSize( int gr );
        // Setzt den inneren Rahmen um das eigentliche Diagramm (Rahmen um die Daten)
        //  ram: Der Rahmen
        __declspec( dllexport ) void setDatenRahmenZ( LRahmen *ram );
        // Setzt den inneren Rahmen um das eigentliche Diagramm (Rahmen um die Daten), indem ein Rahmen kopiert wird
        //  ram: Der Rahmen, der kopiert werden soll
        __declspec( dllexport ) void setDatenRahmen( LRahmen *ram );
        // Setzt die Breite des inneren Rahmen um das eigentliche Diagramm (Rahmen um die Daten)
        //  br: Die Breite in Pixeln
        __declspec( dllexport ) void setDatenRahmenBreite( int br );
        // Setzt die Farbe des inneren Rahmen um das eigentliche Diagramm (Rahmen um die Daten)
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setDatenRahmenFarbe( int fc );
        // Setzt den Hintergrund des eigentlichen Diagramms
        __declspec( dllexport ) void setDatenHintergrundFarbe( int fc );
        __declspec( dllexport ) void setDatenHintergrundBildZ( Bild *b );
        __declspec( dllexport ) void setDatenHintergrundBild( Bild *b );
        // Setzt den Farbübergang des eigentlichen Diagramms (Farbübergang der Daten)
        //  af: Der Farbübergang
        __declspec( dllexport ) void setDatenAlphaFeldZ( AlphaFeld *af );
        // Setzt den Farbübergang des eigentlichen Diagramms (Farbübergang der Daten), indem ein Farbübergang kopiert wird
        //  af: Der Farbübergang, der Kopiert werden soll
        __declspec( dllexport ) void setDatenAlphaFeld( AlphaFeld *af );
        // Setzt die Farbe des Farbübergangs des eigentlichen Diagramms (Farbübergang der Daten)
        //  fc: Die Farbe im A8R8G8B8 Format
        __declspec( dllexport ) void setDatenAlphaFeldFarbe( int fc );
        // Setzt die FaStärkerbe des Farbübergangs des eigentlichen Diagramms (Farbübergang der Daten)
        //  st: Die Stärke
        __declspec( dllexport ) void setDatenAlphaFeldStrength( int st );
        // Verarbeitet Maus Nachrichten
        //  me: Das Ereignis, was durch die Mauseingabe ausgelößt wurde
        __declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override;
        // Zeichnet das Objekt nach zRObj, falls es sichtbar ist
        //  zRObj: Das Bild, in welches gezeichnet werden soll
        __declspec( dllexport ) void render( Bild &zRObj ) override;
        // Gibt die Schrift zurück
        __declspec( dllexport ) Schrift *getSchrift() const;
        // Gibt die Schrift ohne erhöhten Reference Counter zurück
        __declspec( dllexport ) Schrift *zSchrift() const;
        // Gibt den inneren Rahmen um das eigentliche Diagramm zurück (Rahmen um die Daten)
        __declspec( dllexport ) LRahmen *getDatenRahmen() const;
        // Gibt den inneren Rahmen um das eigentliche Diagramm ohne erhöhten Reference Counter zurück (Rahmen um die Daten)
        __declspec( dllexport ) LRahmen *zDatenRahmen() const;
        // Gibt die Farbe des inneren Rahmens um das eigentliche Diagramm im A8R8G8B8 Format zurück (Rahmen um die Daten)
        __declspec( dllexport ) int getDatenRahmenFarbe() const;
        // Gibt die Breite des inneren Rahmens um das eigentliche Diagramm in Pixeln zurück (Rahmen um die Daten)
        __declspec( dllexport ) int getDatenRahmenBreite() const;
        // Gibt die Farbe des Hintergrunds des eigentlichen Diagramms im A8R8G8B8 Format zurück (Hintergrund der Daten)
        __declspec( dllexport ) int getDatenHintergrundFarbe() const;
        // Gibt das Hintergrund Bild des eigentlichen Diagramms zurück (Hintergrund Bild der Daten)
        __declspec( dllexport ) Bild *getDatenHintergrundBild() const;
        // Gibt das Hintergrund Bild des eigentlichen Diagramms ohne erhöhten Reference Counter zurück (Hintergrund Bild der Daten)
        __declspec( dllexport ) Bild *zDatenHintergrundBild() const;
        // Gibt den Farbübergang des eigentlichen Diagramms zurück (Farbübergang der Daten)
        __declspec( dllexport ) AlphaFeld *getDatenAlphaFeld() const;
        // Gibt den Farbübergang des eigentlichen Diagramms ohne erhöhten Reference Counter zurück (Farbübergang der Daten)
        __declspec( dllexport ) AlphaFeld *zDatenAlphaFeld() const;
        // Gibt die Farbe des Farbübergangs des eigentlichen Diagramms im A8R8G8B8 Format zurück (Farbübergang der Daten)
        __declspec( dllexport ) int getDatenAlphaFeldFarbe() const;
        // Gibt die Stärke des Farbübergangs des eigentlichen Diagramms zurück (Farbübergang der Daten)
        __declspec( dllexport ) int getDatenAlphaFeldStrength() const;
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
        __declspec( dllexport ) BaseDiag *release() override;
    };
}

#endif