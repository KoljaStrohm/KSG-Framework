#ifndef Vec2_H
#define Vec2_H

#include "FrameworkMath.h"

namespace Framework
{
	template< typename T >
	class Vec2
	{
	public:
		T x;
		T y;
		// Konstruktor
		inline Vec2()
		{
		}
		inline Vec2( T x, T y )
			: x( x ),
			  y( y )
		{
		}
		inline Vec2( const Vec2 &vect )
			: x( (T)vect.x ),
			  y( (T)vect.y )
		{
		}
		// nicht constant
		inline Vec2 &normalize()
		{
			const T länge = län();
			x /= länge;
			y /= länge;
			return *this;
		}
		inline Vec2 &CCW90()
		{
			T temp = y;
			y = -x;
			x = temp;
			return *this;
		}
		inline Vec2 &CW90()
		{
			T temp = y;
			y = x;
			x = -temp;
			return *this;
		}
		inline Vec2 &Swap( Vec2 &vect )
		{
			const Vec2 tmp = vect;
			vect = *this;
			*this = tmp;
			return *this;
		}
		inline Vec2 operator=( const Vec2 &r )
		{
			x = r.x;
			y = r.y;
			return *this;
		}
		inline Vec2 operator+=( const Vec2 &r )
		{
			x += r.x;
			y += r.y;
			return *this;
		}
		inline Vec2 operator-=( const Vec2 &r )
		{
			x -= r.x;
			y -= r.y;
			return *this;
		}
		inline Vec2 operator*=( const T &r )
		{
			x *= r;
			y *= r;
			return *this;
		}
		inline Vec2 operator/=( const T &r )
		{
			x /= r;
			y /= r;
			return *this;
		}
		// constant
		inline Vec2 operator-( ) const
		{
			return Vec2< T >( -x, -y );
		}
		template< typename T2 >
		inline operator Vec2< T2 >() const
		{
			return Vec2< T2 >( (T2)x, (T2)y );
		}
		inline T länSq() const
		{
			return *this * *this;
		}
		inline T län() const
		{
			return sqrt( länSq() );
		}
		inline T operator*( const Vec2 &r ) const
		{
			return x * r.x + y * r.y;
		}
		inline Vec2 operator+( const Vec2 &r ) const
		{
			return Vec2( *this ) += r;
		}
		inline Vec2 operator-( const Vec2 &r ) const
		{
			return Vec2( *this ) -= r;
		}
		inline Vec2 operator*( const T &r ) const
		{
			return Vec2( *this ) *= r;
		}
		inline Vec2 operator/( const T &r ) const
		{
			return Vec2( *this ) /= r;
		}
		inline bool istInRegion( const Vec2 &p1, const Vec2 &p2 ) const
		{
			const T medianX = (T)( ( p1.x + p2.x ) / 2.0 );
			const T medianY = (T)( ( p1.y + p2.y ) / 2.0 );
			return abs< T >( medianX - x ) <= abs< T >( medianX - p1.x ) &&
				   abs< T >( medianY - y ) <= abs< T >( medianY - p1.y );
		}
		inline bool operator==( const Vec2 &r ) const
		{
			return x == r.x && y == r.y;
		}
		inline bool operator!=( const Vec2 &r ) const
		{
			return !( *this == r );
		}
		inline Vec2 mittelpunktMit( const Vec2 &p2 ) const
		{
			return Vec2( (T)( ( x + p2.x ) / 2.0 ), (T)( ( y + p2.y ) / 2.0 ) );
		}
		inline Vec2 rotation( const float angle ) const
		{
			Vec2 result;
			float cosine = cosf( angle );
			float sine = sinf( angle );
			result.x = (T)( x * cosine - y * sine );
			result.y = (T)( x * sine + y * cosine );
			return result;
		}
	};
}

#endif