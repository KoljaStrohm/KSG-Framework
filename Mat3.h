#ifndef Mat3_H
#define Mat3_H

#include "Vec2.h"

namespace Framework
{
	template< typename T >
	class Mat3
	{
	public:
		// [ Zeile ][ Spalte ] ( [ y ][ x ] )
		T elements[ 3 ][ 3 ];
		// nicht constant
		Mat3 &operator=( const Mat3 &r )
		{
			memcpy( elements, r.elements, sizeof( elements ) );
			return *this;
		}
		Mat3 &operator*=( const T r )
		{
			for( T &e : elements )
				r *= r;
			return *this;
		}
		Mat3 &operator*=( const Mat3 &r )
		{
			return *this = *this * r;
		}
		// constant
		Mat3 operator*( const T r ) const
		{
			Mat3 result = *this;
			return result *= r;
		}
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
		Vec2< T > operator*( const Vec2< T > r ) const
		{
			Vec2< T > result;
			result.x = elements[ 0 ][ 0 ] * r.x + elements[ 0 ][ 1 ] * r.y + elements[ 0 ][ 2 ];
			result.y = elements[ 1 ][ 0 ] * r.x + elements[ 1 ][ 1 ] * r.y + elements[ 1 ][ 2 ];
			return  result;
		}
		// static
		static Mat3 rotation( T radian )
		{
			const T cosTheta = (T)cos( radian );
			const T sinTheta = (T)sin( radian );
			Mat3 r = { cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1 };
			return r;
		}
		static Mat3 scaling( T faktor )
		{
			Mat3 s = { faktor, 0, 0, 0, faktor, 0, 0, 0, 1 };
			return s;
		}
		static Mat3 translation( const Vec2< T > offset )
		{
			Mat3 t = { 1, 0, offset.x, 0, 1, offset.y, 0, 0, 1 };
			return t;
		}
		static Mat3 identity()
		{
			Mat3 i = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
			return i;
		}
	};
}

#endif