#ifndef Vec2_H
#define Vec2_H

#include "FrameworkMath.h"

namespace Framework
{
    // Ein zweidimensionaler Vektor
    template< typename T >
    class Vec2
    {
    public:
        T x; // x Komponente des Vektors
        T y; // y Komponente des Vektors
        // Konstruktor
        inline Vec2()
        {}
        // Konstruktor
        //  x: X Komponente des Vektors
        //  y: Y Komponente des Vektors
        inline Vec2( T x, T y )
            : x( x ),
            y( y )
        {}
        // Konstruktor
        //  vect: Ein Vektor, dessen Komponenten übernommen werden
        inline Vec2( const Vec2 &vect )
            : x( (T)vect.x ),
            y( (T)vect.y )
        {}
        // Skalliert den Vektor, so dass er die Länge 1 hat
        inline Vec2 &normalize()
        {
            const T length = getLength();
            x /= length;
            y /= length;
            return *this;
        }
        // Dreht den Vektor um 90 Grad gegen den Uhrzeigersinn
        inline Vec2 &CCW90()
        {
            T temp = y;
            y = -x;
            x = temp;
            return *this;
        }
        // Dreht den Vektor um 90 Grad im Uhrzeigersinn
        inline Vec2 &CW90()
        {
            T temp = y;
            y = x;
            x = -temp;
            return *this;
        }
        // Tauscht die Werte der Vektoren aus
        //  vect: Der Vektor, mit dem getauscht werden soll
        inline Vec2 &Swap( Vec2 &vect )
        {
            const Vec2 tmp = vect;
            vect = *this;
            *this = tmp;
            return *this;
        }
        // nimmt die Werte eines anderen Vektors an
        //  r: Der andere Vektor
        inline Vec2 operator=( const Vec2 &r )
        {
            x = r.x;
            y = r.y;
            return *this;
        }
        // addiert einen anderen Vektor zu diesem hinzu
        //  r: Der andere Vektor
        inline Vec2 operator+=( const Vec2 &r )
        {
            x += r.x;
            y += r.y;
            return *this;
        }
        // Zieht einen anderen Vektor von diesem ab
        //  r: Der andere Vektor
        inline Vec2 operator-=( const Vec2 &r )
        {
            x -= r.x;
            y -= r.y;
            return *this;
        }
        // Skalliert diesen Vektor mit einem anderen Vektor
        //  r: Der andere Vektor
        inline Vec2 operator*=( const T &r )
        {
            x *= r;
            y *= r;
            return *this;
        }
        // Verkürtzt diesen Vektor durch einen anderen Vektor
        //  r: Der andere Vektor
        inline Vec2 operator/=( const T &r )
        {
            x /= r;
            y /= r;
            return *this;
        }
        // Bildet die Negation des Vektors
        inline Vec2 operator-() const
        {
            return Vec2< T >( -x, -y );
        }
        // Konvertiert den Vektor in ein Vektor eines anderen Typs
        template< typename T2 >
        inline operator Vec2< T2 >() const
        {
            return Vec2< T2 >( (T2)x, (T2)y );
        }
        // Errechnet das Quadrat der Länge desVektors
        inline T getLengthSq() const
        {
            return *this * *this;
        }
        // Errechnet die Länge des Vektors
        inline T getLength() const
        {
            return sqrt( getLengthSq() );
        }
        // Errechnet das Skalarprodukt zwischen zwei Vektoren
        //  r: Der andere Vektor
        inline T operator*( const Vec2 &r ) const
        {
            return x * r.x + y * r.y;
        }
        // Addiert zwei Vektoren
        //  r: Der andere Vektor
        inline Vec2 operator+( const Vec2 &r ) const
        {
            return Vec2( *this ) += r;
        }
        // Subtrahiert zwei Vektoren
        //  r: Der andere Vektor
        inline Vec2 operator-( const Vec2 &r ) const
        {
            return Vec2( *this ) -= r;
        }
        // Multipliziert die Komponenten zweier Vektoren
        //  r: Der andere Vektor
        inline Vec2 operator*( const T &r ) const
        {
            return Vec2( *this ) *= r;
        }
        // Dividiert die Komponenten zweier Vektoren
        //  r: Der andere Vektor
        inline Vec2 operator/( const T &r ) const
        {
            return Vec2( *this ) /= r;
        }
        // Prüft, ob sich der Vektor in dem Rechteck zwischen zwei Vektoren befindet
        //  p1: Ein Vektor zur einen Ecke des Rechtecks
        //  p2: Ein Vektor zur gegenüberliegenden Ecke des Rechtecks
        inline bool istInRegion( const Vec2 &p1, const Vec2 &p2 ) const
        {
            const T medianX = (T)( ( p1.x + p2.x ) / 2.0 );
            const T medianY = (T)( ( p1.y + p2.y ) / 2.0 );
            return abs< T >( medianX - x ) <= abs< T >( medianX - p1.x ) &&
                abs< T >( medianY - y ) <= abs< T >( medianY - p1.y );
        }
        // Überprüft zwei Vektoren auf Gleichheit
        //  r: Der andere Vektor
        inline bool operator==( const Vec2 &r ) const
        {
            return x == r.x && y == r.y;
        }
        // Überprüft zwei Vektoren auf Ungleichheit
        //  r: Der andere Vektor
        inline bool operator!=( const Vec2 &r ) const
        {
            return !( *this == r );
        }
        // Errechnet den Mittelpunkt zwischen zwei Vektoren
        //  p2: Der andere Vektor
        inline Vec2 mittelpunktMit( const Vec2 &p2 ) const
        {
            return Vec2( (T)( ( x + p2.x ) / 2.0 ), (T)( ( y + p2.y ) / 2.0 ) );
        }
        // Rotiert den Vektor gegen den Uhrzeigersinn
        //  angle: Der Winkel in Bogenmas
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