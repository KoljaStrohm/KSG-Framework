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
	class BildO; // aus dieser Datei
    class Text; // Text.h
#endif

	class Bild
	{
	private:
		int *fc;
		bool delFc;
		Punkt größe;
		int ref;
		Punkt *drawOff;
		Punkt *dPosA;
		Punkt *dGrößeA;
		int doa;
		unsigned char *alpha;
		int alphaAnzahl;
		bool rend;
        bool alpha3D;
		// privat
		inline void alphaPixelP( int x, int y, int f );
		inline void alphaPixelP( int &fc, int f );
		inline char getOutCode( Punkt& p ) const;
		void drawFlatDreieck( int y1, int y2, float m1, float b1, float m2, float b2, int farbe );
		void drawFlatDreieckTextur( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
									double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur );
		void drawFlatDreieckAlpha( int y1, int y2, float m1, float b1, float m2, float b2, int farbe );
		void drawFlatDreieckTexturAlpha( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
										 double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur );
		void drawLinieHTextur( Vec2< double > p, double län, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur );
		void drawLinieHTexturAlpha( Vec2< double > p, double län, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur );
	
	public:
		// Konstruktor 
		__declspec( dllexport ) Bild( bool options = 0 );
		// Destruktor 
		__declspec( dllexport ) ~Bild(); 
        // Wird dieser Flag gesetzt, so wird beim Alpha Blending wenn die vorheriege Farbe 0 ist nur die neue mit ihrem Alpha Wert kopiert.
        // Das ist sinnvoll für die Verwendung im 3DBildschirm, wo das Gezeichnette Bild später mittels Alpha Blending angezeigt wird
        __declspec( dllexport ) void setAlpha3D( bool erlaubt );
 		__declspec( dllexport ) void setAlpha( unsigned char alpha ); // setzt die Transparenz der nachfolgenden Zeichnunge
		__declspec( dllexport ) void releaseAlpha(); // Löscht alpha
		__declspec( dllexport ) void setPixelBuffer( int *buffer, bool deleteBuffer, int breite, int höhe ); // setzt den Zeiger auf die Pixel des Bildes
		__declspec( dllexport ) void neuBild( int breite, int höhe, int füllFarbe ); // erzeugt ein neues Bild mit der Hintergrundfarbe füllFarbe
		__declspec( dllexport ) void alphaPixel( int x, int y, int f );
		__declspec( dllexport ) void alphaPixel( int i, int f );
		__declspec( dllexport ) void alphaPixelDP( int x, int y, int f );
		__declspec( dllexport ) void alphaPixelDP( int i, int f );
		__declspec( dllexport ) void setPixelDP( int x, int y, int f );
		__declspec( dllexport ) void setPixelDP( int i, int f );
		__declspec( dllexport ) void setFarbe( int f );
		__declspec( dllexport ) void füllRegion( int x, int y, int b, int h, int fc );
		__declspec( dllexport ) void alphaRegion( int x, int y, int b, int h, int fc );
		__declspec( dllexport ) void drawLinieH( int x, int y, int län, int fc ); // zeichnet eine horizontale Linie
		__declspec( dllexport ) void drawLinieV( int x, int y, int län, int fc ); // zeichnet eine vertikale Linie
		__declspec( dllexport ) void drawLinieHAlpha( int x, int y, int län, int fc ); // zeichnet eine horizontale Linie
		__declspec( dllexport ) void drawLinieVAlpha( int x, int y, int län, int fc ); // zeichnet eine vertikale Linie
		__declspec( dllexport ) void drawLinie( Punkt a, Punkt b, int fc ); // zeichnet eine Linie von Punkt( x1, y1 ) nach Punke( x2, y2 )
		__declspec( dllexport ) void drawLinieAlpha( Punkt a, Punkt b, int fc );
        __declspec( dllexport ) void füllKreis( int xOff, int yOff, int r, int fc ); // zeichnet einen Kreis um Punkt( xOff, yOff ) mit radius r
		__declspec( dllexport ) void drawKreis( int xOff, int yOff, int r, int fc ); // zeichnet einen Kreis um Punkt( xOff, yOff ) mit radius r
		__declspec( dllexport ) void drawKreisAlpha( int xOff, int yOff, int r, int fc );
		__declspec( dllexport ) void drawBild( int x, int y, int br, int hö, Bild &zBild ); // zeichet zBild
		__declspec( dllexport ) void alphaBild( int x, int y, int br, int hö, Bild &zBild );
		__declspec( dllexport ) void drawBild90( int x, int y, int br, int hö, Bild &zBild ); // Zeichnet ein um 90 Grad nach rchts gedrehtes Bild
		__declspec( dllexport ) void alphaBild90( int x, int y, int br, int hö, Bild &zBild );
		__declspec( dllexport ) void drawBild180( int x, int y, int br, int hö, Bild &zBild ); // Zeichnet ein um 180 Grad nach rchts gedrehtes Bild
		__declspec( dllexport ) void alphaBild180( int x, int y, int br, int hö, Bild &zBild );
		__declspec( dllexport ) void drawBild270( int x, int y, int br, int hö, Bild &zBild ); // Zeichnet ein um 270 Grad nach rchts gedrehtes Bild
		__declspec( dllexport ) void alphaBild270( int x, int y, int br, int hö, Bild &zBild );
		__declspec( dllexport ) void drawBildSkall( int x, int y, int br, int hö, Bild &zBild ); // zeichet zBild Skalliert
		__declspec( dllexport ) void alphaBildSkall( int x, int y, int br, int hö, Bild &zBild );
		__declspec( dllexport ) void drawDreieck( Punkt a, Punkt b, Punkt c, int farbe ); // füllt eine Dreieck aus
		__declspec( dllexport ) void drawDreieckTextur( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur );
		__declspec( dllexport ) void drawDreieckAlpha( Punkt a, Punkt b, Punkt c, int farbe );
		__declspec( dllexport ) void drawDreieckTexturAlpha( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur );
		__declspec( dllexport ) bool setDrawOptions( const Punkt &pos, const Punkt &gr ); // setzt die Drawoptionen
		__declspec( dllexport ) bool setDrawOptions( int x, int y, int br, int hö );
		__declspec( dllexport ) bool setDrawOptionsErzwingen( const Punkt &pos, const Punkt &gr ); // setzt die Drawoptionen
		__declspec( dllexport ) bool setDrawOptionsErzwingen( int x, int y, int br, int hö ); // setzt die Drawoptionen
		__declspec( dllexport ) void addScrollOffset( int xOff, int yOff ); // setzt ScrollOffset
		__declspec( dllexport ) void releaseDrawOptions(); // setzt die Drawoptionen zurück
		__declspec( dllexport ) bool getRend();
		// constant 
		__declspec( dllexport ) int *getBuffer()const; // gibt buffer zurück
		__declspec( dllexport ) int getPixel( int x, int y ) const; // gibt die Farbe des Pixels(x, y) zurück
		__declspec( dllexport ) const Punkt &getGröße() const; // gibt die Größe zurück
		__declspec( dllexport ) int getBreite() const; // gibt die Breite zurück
		__declspec( dllexport ) int getHöhe() const; // gibt die Höhe zurück
		__declspec( dllexport ) unsigned char getAlpha() const; // gibt den Alpha wert zurück
		__declspec( dllexport ) const Punkt &getDrawPos() const;
		__declspec( dllexport ) const Punkt &getDrawGr() const;
		__declspec( dllexport ) const Punkt &getDrawOff() const;
		// Reference Counting 
		__declspec( dllexport ) Bild *getThis();
		__declspec( dllexport ) Bild *release();
	};
#ifdef WIN32

	class BildO : public ZeichnungHintergrund
	{
    public:
        class Style : public ZeichnungHintergrund::Style
        {
        public:
            const static __int64 Alpha = 0x1000;

            const static __int64 normal = HScroll | Sichtbar | Erlaubt | Rahmen | VScroll;
        };
	private:
		Bild *bild;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) BildO();
		// Destruktor 
		__declspec( dllexport ) ~BildO();
		// nicht constant 
		__declspec( dllexport ) void setBildZ( Bild *b ); // setzt das Bild
		__declspec( dllexport ) void setBild( Bild *b );
		__declspec( dllexport ) bool tick( double tickVal ) override; // tick
		__declspec( dllexport ) void doMausEreignis( MausEreignis &me ) override; // ruft Mak auf
		__declspec( dllexport ) void render( Bild &zRObj ) override; // zeichnet nach zRObj
		// constant 
		__declspec( dllexport ) Bild *getBild() const; // gibt das Bild zurück
		__declspec( dllexport ) Bild *zBild() const;
		__declspec( dllexport ) Zeichnung *dublizieren() const override; // erstellt eine Kopie des Zeichnungs
		// Reference Counting 
		__declspec( dllexport ) BildO *getThis();
		__declspec( dllexport ) BildO *release();
	};

    __declspec( dllexport ) Bild *ladeBild( char *pfad, Text *zError );
#endif
}
#endif