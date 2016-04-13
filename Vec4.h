#pragma once

#include "Vec3.h"

namespace Framework
{
    template< typename T >
    class Vec4
    {
    public:
        T x;
        T y;
        T z;
        T w;
        // Konstruktor
        inline Vec4()
        {}
        inline Vec4( T x, T y, T z, T w )
            : x( x ),
            y( y ),
            z( z ),
            w( w )
        {}
        inline Vec4( const Vec4 &vect )
            : Vec4( vect.x, vect.y, vect.z, vect.w )
        {}
        // nicht constant
        inline Vec4 &normalize()
        {
            const T länge = län();
            x /= länge;
            y /= länge;
            z /= länge;
            w /= länge;
            return *this;
        }
        inline Vec4 &Swap( Vec4 &vect )
        {
            const Vec4 tmp = vect;
            vect = *this;
            *this = tmp;
            return *this;
        }
        inline Vec4 operator=( const Vec4 &r )
        {
            x = r.x;
            y = r.y;
            z = r.z;
            w = r.w;
            return *this;
        }
        inline Vec4 operator+=( const Vec4 &r )
        {
            x += r.x;
            y += r.y;
            z += r.z;
            w += r.w;
            return *this;
        }
        inline Vec4 operator-=( const Vec4 &r )
        {
            x -= r.x;
            y -= r.y;
            z -= r.z;
            w -= r.w;
            return *this;
        }
        inline Vec4 operator*=( const T &r )
        {
            x *= r;
            y *= r;
            z *= r;
            w *= r;
            return *this;
        }
        inline Vec4 operator/=( const T &r )
        {
            x /= r;
            y /= r;
            z /= r;
            w /= r;
            return *this;
        }
        // constant
        inline Vec4 operator-() const
        {
            return{ -x, -y, -z, -w };
        }
        template< typename T2 >
        inline operator Vec4< T2 >() const
        {
            return{ (T2)x, (T2)y, (T2)z, (T2)w };
        }
        inline T länSq() const
        {
            return *this * *this;
        }
        inline T län() const
        {
            return sqrt( länSq() );
        }
        inline T operator*( const Vec4 &r ) const
        {
            return x * r.x + y * r.y + z * r.z;// +w * r.w;
        }
        inline T operator*( const Vec3< T > &r ) const
        {
            return x * r.x + y * r.y + z * r.z + w;
        }
        inline Vec4 operator+( const Vec4 &r ) const
        {
            return Vec4( *this ) += r;
        }
        inline Vec4 operator-( const Vec4 &r ) const
        {
            return Vec4( *this ) -= r;
        }
        inline Vec4 operator*( const T &r ) const
        {
            return Vec4( *this ) *= r;
        }
        inline Vec4 operator/( const T &r ) const
        {
            return Vec4( *this ) /= r;
        }
        inline bool operator==( const Vec4 &r ) const
        {
            return x == r.x && y == r.y && z == r.z && w == r.w;
        }
        inline bool operator!=( const Vec4 &r ) const
        {
            return !( *this == r );
        }
    };
}