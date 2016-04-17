#ifndef FrameworkMath_H
#define FrameworkMath_H

#include <math.h>
#include "Betriebssystem.h"

#define PI 3.14159265

// Gibt die größere Zahl zurück ohne if zu verwenden
// Funktioniert nur, wenn die Zahlen nicht mehr als 16 bits verwenden
//  a: Eine der beiden Zahlen
//  b: Eine der beiden Zahlen
inline int maxInt( int a, int b )
{
	return ( ( ( a - b ) >> 16 ) & b ) | ( ~( ( a - b ) >> 16 ) & a );
}

// Gibt die kleinere Zahl zurück ohne if zu verwenden
// Funktioniert nur, wenn die Zahlen nicht mehr als 16 bits verwenden
//  a: Eine der beiden Zahlen
//  b: Eine der beiden Zahlen
inline int minInt( int a, int b )
{
	return ( ( ( a - b ) >> 16 ) & a ) | ( ~( ( a - b ) >> 16 ) & b );
}

// Gibt den positiven Wert eines Zeichnunges zurück.
// Klappt nur, wenn der - und der < 0 operator definiert ist
template< typename T>
inline T abs( T t )
{
    if( t < 0 )
        return -t;
    return t;
}

#endif