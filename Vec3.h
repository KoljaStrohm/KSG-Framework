#ifndef Vec3_H
#define Vec3_H

#include "FrameworkMath.h"

namespace Framework
{
    template< typename T >
    // Ein 3D Vektor
    class Vec3
    {
    public:
        T x; // X Komponente des Vektors
        T y; // y Komponente des Vektors
        T z; // z Komponente des Vektors
        // Konstruktor
        inline Vec3()
        {}
        // Konstruktor
        //  x: Die X Komponente des neuen Vektors
        //  y: Die Y Komponente des neuen Vektors
        //  z: Die Z Komponente des neuen Vektors
        inline Vec3( T x, T y, T z )
            : x( x ),
            y( y ),
            z( z )
        {}
        // Konstruktor
        //  vect: Ein Vektor, dessen Werte kopiert werden sollen
        inline Vec3( const Vec3 &vect )
            : Vec3( vect.x, vect.y, vect.z )
        {}
        // Skalliert den Vektor, so dass er die Länge 1 hat
        inline Vec3 &normalize()
        {
            const T länge = län();
            x /= länge;
            y /= länge;
            z /= länge;
            return *this;
        }
        // Vertaucht die Werte des Vektors mit denen eines anderen Vektor
        //  vect: Der andere Vektor
        inline Vec3 &Swap( Vec3 &vect )
        {
            const Vec3 tmp = vect;
            vect = *this;
            *this = tmp;
            return *this;
        }
        // Kopiert die Werte eines anderen Vektors
        //  r: Der andere Vektor
        inline Vec3 operator=( const Vec3 &r )
        {
            x = r.x;
            y = r.y;
            z = r.z;
            return *this;
        }
        // Addiert einen anderen Vektor zu diesem Hinzu
        //  r: Der andere Vektor
        inline Vec3 operator+=( const Vec3 &r )
        {
            x += r.x;
            y += r.y;
            z += r.z;
            return *this;
        }
        // Zieht einen anderen Vektor von diesem ab
        //  r: Der andere Vektor
        inline Vec3 operator-=( const Vec3 &r )
        {
            x -= r.x;
            y -= r.y;
            z -= r.z;
            return *this;
        }
        // Skalliert diesen Vektor
        //  r: Der Faktor
        inline Vec3 operator*=( const T &r )
        {
            x *= r;
            y *= r;
            z *= r;
            return *this;
        }
        // Skalliert diesen Vektor mit 1/Faktor
        //  r: Der Faktor
        inline Vec3 operator/=( const T &r )
        {
            x /= r;
            y /= r;
            z /= r;
            return *this;
        }
        // Errechnet das Quadrat des Abstands zwischen zewi Vektoren
        //  p: Der andere Vektor
        inline T abstandSq( const Vec3 &p ) const
        {
            return ( x - p.x ) * ( x - p.x ) + ( y - p.y ) * ( y - p.y ) + ( z - p.z ) * ( z - p.z );
        }
        // Errechnet den Abstand zwischen zwei Vektoren
        //  p: Der andere Vektor
        inline T abstand( const Vec3 &p ) const
        {
            return sqrt( abstandSq( p ) );
        }
        // Gibt einen neuen Vektor zurück, der die negation von diesem ist
        inline Vec3 operator-() const
        {
            return{ -x, -y, -z };
        }
        template< typename T2 >
        // Konvertiert den Typ des Vektors in einen anderen
        inline operator Vec3< T2 >() const
        {
            return{ (T2)x, (T2)y, (T2)z };
        }
        // Errechnet das Quadrat der Länge des Vektors
        inline T länSq() const
        {
            return *this * *this;
        }
        // Errechnet due Länge des Vektors
        inline T län() const
        {
            return sqrt( länSq() );
        }
        // Bildet das Skalarprodukt zwischen zwei Vektoren
        //  r: Der andere Vektor
        inline T operator*( const Vec3 &r ) const
        {
            return x * r.x + y * r.y + z * r.z;
        }
        // Errechnet die Summe zweier Vektoren
        //  r: Der andere Vektor
        inline Vec3 operator+( const Vec3 &r ) const
        {
            return Vec3( *this ) += r;
        }
        // Zieht zwei Vektoren von einander ab
        //  r: Der andere Vektor
        inline Vec3 operator-( const Vec3 &r ) const
        {
            return Vec3( *this ) -= r;
        }
        // Skalliert den Vektor ohne ihn zu verändern
        //  r: Der Faktor
        inline Vec3 operator*( const T &r ) const
        {
            return Vec3( *this ) *= r;
        }
        // Skalliert den Vektor mit 1/Faktor ohne ihn zu Verändern
        //  r: Der Faktor
        inline Vec3 operator/( const T &r ) const
        {
            return Vec3( *this ) /= r;
        }
        // Überprüft zwei Vektoren auf Gleichheit
        //  r: Der andere Vektor
        inline bool operator==( const Vec3 &r ) const
        {
            return x == r.x && y == r.y && z == r.z;
        }
        // Überprüft zwei Vektoren auf Ungleichheit
        //  r: Der andere Vektor
        inline bool operator!=( const Vec3 &r ) const
        {
            return !( *this == r );
        }
    };
}

#endif