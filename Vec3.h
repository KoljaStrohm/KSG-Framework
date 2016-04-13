#ifndef Vec3_H
#define Vec3_H

#include "FrameworkMath.h"

namespace Framework
{
	template< typename T >
	class Vec3
	{
	public:
		T x;
		T y;
		T z;
		// Konstruktor
		inline Vec3()
		{
		}
		inline Vec3( T x, T y, T z )
			: x( x ),
			  y( y ),
			  z( z )
		{
		}
		inline Vec3( const Vec3 &vect )
			: Vec3( vect.x, vect.y, vect.z )
		{
		}
		// nicht constant
		inline Vec3 &normalize()
		{
			const T länge = län();
			x /= länge;
			y /= länge;
			z /= länge;
			return *this;
		}
		inline Vec3 &Swap( Vec3 &vect )
		{
			const Vec3 tmp = vect;
			vect = *this;
			*this = tmp;
			return *this;
		}
		inline Vec3 operator=( const Vec3 &r )
		{
			x = r.x;
			y = r.y;
			z = r.z;
			return *this;
		}
		inline Vec3 operator+=( const Vec3 &r )
		{
			x += r.x;
			y += r.y;
			z += r.z;
			return *this;
		}
		inline Vec3 operator-=( const Vec3 &r )
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			return *this;
		}
		inline Vec3 operator*=( const T &r )
		{
			x *= r;
			y *= r;
			z *= r;
			return *this;
		}
		inline Vec3 operator/=( const T &r )
		{
			x /= r;
			y /= r;
			z /= r;
			return *this;
		}
		// constant
        inline T abstandSq( const Vec3 &p ) const
        {
            return ( x - p.x ) * ( x - p.x ) + ( y - p.y ) * ( y - p.y ) + ( z - p.z ) * ( z - p.z );
        }
        inline T abstand( const Vec3 &p ) const
        {
            return sqrt( abstandSq( p ) );
        }
		inline Vec3 operator-( ) const
		{
			return{ -x, -y, -z };
		}
		template< typename T2 >
		inline operator Vec3< T2 >() const
		{
			return{ (T2)x, (T2)y, (T2)z };
		}
		inline T länSq() const
		{
			return *this * *this;
		}
		inline T län() const
		{
			return sqrt( länSq() );
		}
		inline T operator*( const Vec3 &r ) const
		{
			return x * r.x + y * r.y + z * r.z;
		}
		inline Vec3 operator+( const Vec3 &r ) const
		{
			return Vec3( *this ) += r;
		}
		inline Vec3 operator-( const Vec3 &r ) const
		{
			return Vec3( *this ) -= r;
		}
		inline Vec3 operator*( const T &r ) const
		{
			return Vec3( *this ) *= r;
		}
		inline Vec3 operator/( const T &r ) const
		{
			return Vec3( *this ) /= r;
		}
		inline bool operator==( const Vec3 &r ) const
		{
			return x == r.x && y == r.y && z == r.z;
		}
		inline bool operator!=( const Vec3 &r ) const
		{
			return !( *this == r );
		}
	};
}

#endif