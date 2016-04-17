#ifndef Mat3_H
#define Mat3_H

#include "Vec2.h"

namespace Framework
{
	template< typename T >
    // Eine 3x3 Matrix
	class Mat3
	{
	public:
		T elements[ 3 ][ 3 ]; // Die Elemente der Matrix
		// Kopiert die Elemente einer anderen Matrix in diese
        //  r: Die andere Matrix
		Mat3 &operator=( const Mat3 &r )
		{
			memcpy( elements, r.elements, sizeof( elements ) );
			return *this;
		}
        // Multipliziert alle Werte mit einem Skalar und speichert das Ergebnis in dieser
        //  r: Der Skalar
		Mat3 &operator*=( const T r )
		{
			for( T &e : elements )
				e *= r;
			return *this;
		}
        // Multipliziert die Matrix mit einer anderen und speichert das Ergebnis in dieser
        //  r: Die andere Matrix
		Mat3 &operator*=( const Mat3 &r )
		{
			return *this = *this * r;
		}
        // Multipliziert alle Werte mit einem Skalar ohne diese zu verändern
        //  r: Der Skalar
		Mat3 operator*( const T r ) const
		{
			Mat3 result = *this;
			return result *= r;
		}
        // Multipliziert die Matrix mit einer anderen ohne diese zu verändern
        //  r: Die andere Matrix
		Mat3 operator*( const Mat3 &r ) const
		{
			Mat3 result;
			for( int j = 0; j < 3; j++ )
			{
				for( int k = 0; k < 3; k++ )
				{
					T sum = 0;
					for( int i = 0; i < 3; i++ )
						sum += elements[ j ][ i ] * r.elements[ i ][ k ];
					result.elements[ j ][ k ] = sum;
				}
			}
			return result;
		}
        // Multipliziert die Matrix mit einem Vektor ohne diese zu verändern
        //  r: Der Vektor
		Vec2< T > operator*( const Vec2< T > r ) const
		{
			Vec2< T > result;
			result.x = elements[ 0 ][ 0 ] * r.x + elements[ 0 ][ 1 ] * r.y + elements[ 0 ][ 2 ];
			result.y = elements[ 1 ][ 0 ] * r.x + elements[ 1 ][ 1 ] * r.y + elements[ 1 ][ 2 ];
			return  result;
		}
        // Erzeugt eine Matrix, die verwendet wird, um einen Vektor zu drehen, wenn sie mit ihm multipliziert wird
        //  radian: Der Winkel im Bogenmas, um dem der Vektor gedreht werden soll
		static Mat3 rotation( T radian )
		{
			const T cosTheta = (T)cos( radian );
			const T sinTheta = (T)sin( radian );
			Mat3 r = { cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1 };
			return r;
		}
        // Erzeugt eine Matrix, die einen Vektor skalliert. wenn sie mit ihm multipliziert wird
        //  faktor: Der Faktor, um den Skalliert werden soll
		static Mat3 scaling( T faktor )
		{
			Mat3 s = { faktor, 0, 0, 0, faktor, 0, 0, 0, 1 };
			return s;
		}
        // Erzeugt eine Matrix, die einen Vektor verschiebt, wenn sie mit ihm multipliziert wird
        //  offset: Die x und y Werte, die zu dem Vektor dazu addiert werden sollen
		static Mat3 translation( const Vec2< T > offset )
		{
			Mat3 t = { 1, 0, offset.x, 0, 1, offset.y, 0, 0, 1 };
			return t;
		}
        // Erzeugt eine MAtrix, die mit allem multiplizert werden kann, ohne es zu verändern
		static Mat3 identity()
		{
			Mat3 i = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
			return i;
		}
	};
}

#endif