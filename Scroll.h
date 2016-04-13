#ifndef Scroll_H
#define Scroll_H

#include "Betriebssystem.h"

namespace Framework
{
	class Bild; // Bild.h
	struct MausEreignis; // MausEreignis.h
	struct VScrollData; // aus dieser Datei
	struct HScrollData; // aus dieser Datei
	class VScrollBar; // aus dieser Datei
	class HScrollBar; // aus dieser Datei

	struct VScrollData
	{
		int anzeigeHöhe;
		int maxHöhe;
		int anzeigeBeginn;
	};

	struct HScrollData
	{
		int anzeigeBreite;
		int maxBreite;
		int anzeigeBeginn;
	};

	class VScrollBar
	{
	private:
		VScrollData *data;
		int knopfdruck;
		int farbe;
		int bgFarbe;
		bool bg;
		int klickScroll;
		int mx, my;
		bool mp;
		bool rend;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) VScrollBar();
		// Destruktor 
		__declspec( dllexport ) ~VScrollBar();
		// nicht constant
		__declspec( dllexport ) void setFarbe( int fc );
		__declspec( dllexport ) void setBgFarbe( int fc, bool bgF );
		__declspec( dllexport ) void update( int maxHöhe, int anzeigeHöhe );
		__declspec( dllexport ) void setKlickScroll( int klickScroll );
		__declspec( dllexport ) void scroll( int höhe );
		__declspec( dllexport ) bool doMausMessage( int x, int y, int br, int hö, MausEreignis &me );
		__declspec( dllexport ) bool getRend();
		// constant 
		__declspec( dllexport ) void render( int x, int y, int br, int hö, Bild &zRObj ) const;
		__declspec( dllexport ) VScrollData *getScrollData() const;
		__declspec( dllexport ) int getKlickScroll() const;
		__declspec( dllexport ) int getFarbe() const;
		__declspec( dllexport ) int getBgFarbe() const;
        __declspec( dllexport ) int getScroll() const;
		// Reference Counting
		__declspec( dllexport ) VScrollBar *getThis();
		__declspec( dllexport ) VScrollBar *release();
	};

	class HScrollBar
	{
	private:
		HScrollData *data;
		int knopfdruck;
		int farbe;
		int bgFarbe;
		bool bg;
		int klickScroll;
		int mx, my;
		bool mp;
		bool rend;
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) HScrollBar();
		// Destruktor 
		__declspec( dllexport ) ~HScrollBar();
		// nicht constant
		__declspec( dllexport ) void setFarbe( int fc );
		__declspec( dllexport ) void setBgFarbe( int fc, bool bgF );
		__declspec( dllexport ) void update( int maxBreite, int anzeigeBreite );
		__declspec( dllexport ) void setKlickScroll( int klickScroll );
		__declspec( dllexport ) void scroll( int breite );
		__declspec( dllexport ) bool doMausMessage( int x, int y, int br, int hö, MausEreignis &me );
		__declspec( dllexport ) bool getRend();
		// constant 
		__declspec( dllexport ) void render( int x, int y, int br, int hö, Bild &zRObj ) const;
		__declspec( dllexport ) HScrollData *getScrollData() const;
		__declspec( dllexport ) int getKlickScroll() const;
		__declspec( dllexport ) int getFarbe() const;
		__declspec( dllexport ) int getBgFarbe() const;
        __declspec( dllexport ) int getScroll() const;
		// Reference Counting
		__declspec( dllexport ) HScrollBar *getThis();
		__declspec( dllexport ) HScrollBar *release();
	};
}

#endif