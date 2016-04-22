#pragma once

#include "Vec3.h"

namespace Framework
{
    template< typename T >
    // Eine 4x4 Matrix
    class Mat4
    {
    public:
        T elements[ 4 ][ 4 ]; // Die Elemente der Matrix
        // Kopiert alle Werte einer anderen Matrix
        //  r: Die andere Matrix
        Mat4 &operator=( const Mat4 &r )
        {
            memcpy( elements, r.elements, sizeof( elements ) );
            return *this;
        }
        // Skalliert die Matrix
        //  r: der Faktor
        Mat4 &operator*=( const T r )
        {
            for( T &e : elements )
                e *= r;
            return *this;
        }
        // Multipliziert die MAtrix mit einer anderen
        //  r: Die andere Matrix
        Mat4 &operator*=( const Mat4 &r )
        {
            return *this = *this * r;
        }
        // Skalliert die Matrix ohne sie zu verändern
        //  r: der Faktor
        Mat4 operator*( const T r ) const
        {
            Mat4 result = *this;
            return result *= r;
        }
        // Multipliziert zwei Matrizen
        //  r: Die andere Matrix
        Mat4 operator*( const Mat4 &r ) const
        {
            Mat4 result;
            for( int j = 0; j < 4; j++ )
            {
                for( int k = 0; k < 4; k++ )
                {
                    T sum = 0;
                    for( int i = 0; i < 4; i++ )
                        sum += elements[ j ][ i ] * r.elements[ i ][ k ];
                    result.elements[ j ][ k ] = sum;
                }
            }
            return result;
        }
        // Multiplziert die Matrix mit einem Vektor
        //  r: Der Vektor
        Vec3< T > operator*( const Vec3< T > &r ) const
        {
            Vec3< T > result;
            result.x = elements[ 0 ][ 0 ] * r.x + elements[ 0 ][ 1 ] * r.y + elements[ 0 ][ 2 ] * r.z + elements[ 0 ][ 3 ];
            result.y = elements[ 1 ][ 0 ] * r.x + elements[ 1 ][ 1 ] * r.y + elements[ 1 ][ 2 ] * r.z + elements[ 1 ][ 3 ];
            result.z = elements[ 2 ][ 0 ] * r.x + elements[ 2 ][ 1 ] * r.y + elements[ 2 ][ 2 ] * r.z + elements[ 2 ][ 3 ];
            return  result;
        }
        // Erzeugt eine Matrix, die einen Vektor um die Z Achse dreht, wenn sie mit ihm multipliziert wird
        //  radian: Der Winkel in Bogenmas
        static Mat4 rotationZ( T radian )
        {
            const T cosTheta = (T)cos( radian );
            const T sinTheta = (T)sin( radian );
            Mat4 r = { cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            return r;
        }
        // Erzeugt eine Matrix, die einen Vektor um die X Achse dreht, wenn sie mit ihm multipliziert wird
        //  radian: Der Winkel in Bogenmas
        static Mat4 rotationX( T radian )
        {
            const T cosTheta = (T)cos( radian );
            const T sinTheta = (T)sin( radian );
            Mat4 r = { 1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1 };
            return r;
        }
        // Erzeugt eine Matrix, die einen Vektor um die Y Achse dreht, wenn sie mit ihm multipliziert wird
        //  radian: Der Winkel in Bogenmas
        static Mat4 rotationY( T radian )
        {
            const T cosTheta = (T)cos( radian );
            const T sinTheta = (T)sin( radian );
            Mat4 r = { cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta, 0, cosTheta, 0, 0, 0, 0, 1 };
            return r;
        }
        // Erzeugt eine Matrix, die einen Vektor Skalliert, wenn sie mit ihm multipliziert wird
        //  faktor: Der Faktor
        static Mat4 scaling( T faktor )
        {
            Mat4 s = { faktor, 0, 0, 0, 0, faktor, 0, 0, 0, 0, faktor, 0, 0, 0, 1 };
            return s;
        }
        // Erzeugt eine Matrix, die einen Vektor Skalliert, wenn sie mit ihm multipliziert wird
        //  faktorX: Der Faktor für die X Komponente des Vektors
        //  faktorY: Der Faktor für die Y Komponente des Vektors
        //  faktorZ: Der Faktor für die Z Komponente des Vektors
        static Mat4 scaling( T faktorX, T faktorY, T faktorZ )
        {
            Mat4 s = { faktorX, 0, 0, 0, 0, faktorY, 0, 0, 0, 0, faktorZ, 0, 0, 0, 1 };
            return s;
        }
        // Erzeugt eine Matrix, die einen Vektor verchiebt, wenn sie mit ihm multipliziert wird
        //  offset: Die Koordinaten, um die der Vektor verschoben werden soll
        static Mat4 translation( const Vec3< T > offset )
        {
            Mat4 t = { 1, 0, 0, offset.x, 0, 1, 0, offset.y, 0, 0, 1, offset.z, 0, 0, 0, 1 };
            return t;
        }
        // Erzeugt eine Matrix, die einen Vektor auf den Bildschirm Projeziert
        //  öffnungswinkel: Der Öffnungswinkel der Kamera im Bogenmas
        //  bildschirmXY: Das Seitenverhältnis des Rechtecks auf dem Bildschirm, in dem gezeichnet werden soll. (Breite / Höhe)
        //  minz: Der Mindestabstand zur Kamera, ab dem gezeichnet wird
        //  maxZ: Der Maximalabstand zur Kamera, ab dem nicht mehr gezeichnet wird
        static Mat4 projektion( float öffnungswinkel, float bildschirmXY, float minZ, float maxZ )
        {
            Mat4 p = { (float)( 1 / tan( öffnungswinkel / 2 ) ) / bildschirmXY, 0, 0, 0,
                0, (float)( 1 / tan( öffnungswinkel / 2 ) ), 0, 0,
                0, 0, maxZ / ( maxZ - minZ ), -( minZ * maxZ ) / ( maxZ - minZ ),
                0, 0, 1, 0 };
            return p;
        }
        // Erzeugt eine Matrix, die mit allem Multipliziert werden kann ohne es zu ändern
        static Mat4 identity()
        {
            Mat4 i = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            return i;
        }
    };
}