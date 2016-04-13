#include "Punkt.h"
#ifdef WIN32
#include "Fenster.h"
#endif

using namespace Framework;

// andere Funktionen 
#ifdef WIN32
inline Punkt Framework::BildschirmGröße() // Gibt die Größe des Bildschirms zurück
{
	RECT r;
	GetWindowRect( GetDesktopWindow(), &r );
	return Punkt( r.right, r.bottom );
}

inline Punkt Framework::Bildschirmmitte() // Giebt die Mitte des Bildschirms zurück
{
	RECT r;
	GetWindowRect( GetDesktopWindow(), &r ); // Bildschirmgröße herausfinden
	return Punkt( r.right / 2, r.bottom / 2 );
}

inline Punkt Framework::Bildschirmmitte( WFenster *f ) // Giebt die Mitte des Bildschirms zurück
{
	Punkt p = Bildschirmmitte();
	Punkt p2 = f->getGröße();
	f->release();
	return { p.x - p2.x / 2, p.y - p2.y / 2 };
}
#endif
bool Framework::operator >( const Punkt &a, const Punkt &b ) // Gibt an, ob a > als b ist
{
	return a.x > b.x && a.y > b.y;
}

bool Framework::operator <( const Punkt &a, const Punkt &b ) // Gibt an, ob a < als b ist
{
	return a.x < b.x && a.y < b.y;
}

bool Framework::operator <=( const Punkt &a, const Punkt &b ) // Gibt an, ob a <= als b ist
{
	return a.x <= b.x && a.y <= b.y;
}

bool Framework::operator >=( const Punkt &a, const Punkt &b ) // Gibt an, ob a >= als b ist
{
	return a.x >= b.x && a.y >= b.y;
}