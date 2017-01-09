#pragma once

#include "Vec3.h"

namespace Framework
{
    template< typename T >
    // Speichert eine 3D Ebene in Koordinaten Form (x*?+y*?+z*?-w=0)
    class Ebene3D
    {
    public:
        T x; // Die X Komponente des Normalen Vektors der Ebene
        T y; // Die Y Komponente des Normalen Vektors der Ebene
        T z; // Die Z Komponente des Normalen Vektors der Ebene
        T w; // Der Wert, den das Skalarprodukt eines Vektors mit dem Normalen Vektor haben muss um in der Ebene zu liegen
        // Konstruktor
        inline Ebene3D()
        {}
        // Knstruktor
        //  x: Die X Komponente des Normalen Vektors der Ebene
        //  y: Die Y Komponente des Normalen Vektors der Ebene
        //  z: Die Z Komponente des Normalen Vektors der Ebene
        //  w: Der Wert, den das Skalarprodukt eines Vektors mit dem Normalen Vektor haben muss um in der Ebene zu liegen
        inline Ebene3D( T x, T y, T z, T w )
            : x( x ),
            y( y ),
            z( z ),
            w( w )
        {}
        // Knstruktor
        //  vect: Die Ebene, deren Werte kopiert werden sollen
        inline Ebene3D( const Ebene3D &vect )
            : Ebene3D( vect.x, vect.y, vect.z, vect.w )
        {}
        // Bringt den Normalen Vektor der Ebene auf die Länge 1
        inline Ebene3D &normalize()
        {
            const T length = len();
            x /= length;
            y /= length;
            z /= length;
            w /= length;
            return *this;
        }
        // Tauscht die Werte zweier Ebenen
        //  vect: die zweite Ebene
        inline Ebene3D &Swap( Ebene3D &vect )
        {
            const Ebene3D tmp = vect;
            vect = *this;
            *this = tmp;
            return *this;
        }
        // Kopiert die Werte einer anderen Ebene
        //  r: Die andere Ebene
        inline Ebene3D operator=( const Ebene3D &r )
        {
            x = r.x;
            y = r.y;
            z = r.z;
            w = r.w;
            return *this;
        }
        // Addiert eine andere Ebene dazu
        //  r: Die andere Ebene
        inline Ebene3D operator+=( const Ebene3D &r )
        {
            x += r.x;
            y += r.y;
            z += r.z;
            w += r.w;
            return *this;
        }
        // Subtrahiert eine andere Ebene
        //  r: Die andere Ebene
        inline Ebene3D operator-=( const Ebene3D &r )
        {
            x -= r.x;
            y -= r.y;
            z -= r.z;
            w -= r.w;
            return *this;
        }
        // Multipliziert die Ebene mit einem Faktor
        //  r: Der Faktor
        inline Ebene3D operator*=( const T &r )
        {
            x *= r;
            y *= r;
            z *= r;
            w *= r;
            return *this;
        }
        // Teilt die Ebene durch einen Faktor
        //  r: Der Faktor
        inline Ebene3D operator/=( const T &r )
        {
            x /= r;
            y /= r;
            z /= r;
            w /= r;
            return *this;
        }
        template< typename T2 >
        // Konvertiert die Ebene in einen anderen Typ
        inline operator Ebene3D< T2 >() const
        {
            return{ (T2)x, (T2)y, (T2)z, (T2)w };
        }
        // Errehnet das Quadrat der Länge des Normalen Vektors der Ebene
        inline T lenSq() const
        {
            return x * x + y * y + z * z;
        }
        // Errechnet die Länge des Normalen Vektors
        inline T len() const
        {
            return sqrt( lenSq() );
        }
        // Berechnet die Entfernung der Ebene zu einem Vektor
        //  r: Der Vektor
        inline T operator*( const Vec3< T > &r ) const
        {
            return x * r.x + y * r.y + z * r.z + w;
        }
        // Addiert zwei Ebenen
        //  r: Die andere Ebene
        inline Ebene3D operator+( const Ebene3D &r ) const
        {
            return Vec4( *this ) += r;
        }
        // Subtrahiert zwei Ebenen
        //  r: Die andere Ebene
        inline Ebene3D operator-( const Ebene3D &r ) const
        {
            return Ebene3D( *this ) -= r;
        }
        // Multiliziert die Ebene mit einem Faktor
        // r: Der Faktor
        inline Ebene3D operator*( const T &r ) const
        {
            return Ebene3D( *this ) *= r;
        }
        // Dividiert die Ebene durch einen Faktor
        //  r: Der Faktor
        inline Ebene3D operator/( const T &r ) const
        {
            return Ebene3D( *this ) /= r;
        }
        // Überprüft zwei Ebenen auf Gleichheit
        //  r: Die andere Ebene
        inline bool operator==( const Ebene3D &r ) const
        {
            return x == r.x && y == r.y && z == r.z && w == r.w;
        }
        // Überprüft zwei Ebenen auf Ungleichheit
        //  r: Die andere Ebene
        inline bool operator!=( const Ebene3D &r ) const
        {
            return !( *this == r );
        }
    };
}