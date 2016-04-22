//---Include---
#ifdef WIN32

#include <Windows.h>
#include <GdiPlus.h>
#pragma comment( lib, "gdiplus.lib" )

#endif
#include "Bild.h"
#ifdef WIN32
#include "DateiSystem.h"
#include "Scroll.h"
#include "Rahmen.h"
#include "MausEreignis.h"
#include "Globals.h"
#include "ToolTip.h"
#include "Text.h"
#include "AlphaFeld.h"
#else
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifndef max
#define max( a, b ) ( ( (a) > (b) ) ? (a) : (b) )
#endif
#ifndef min
#define min( a, b ) ( ( (a) < (b) ) ? (a) : (b) )
#endif
#endif

using namespace Framework;

// Inhalt der Bild Klasse aus Bild.h
// Konstruktor 
Bild::Bild( bool options )
    : fc( 0 ),
    delFc( 1 ),
    größe( 0, 0 ),
    ref( 1 ),
    drawOff( options ? new Punkt[ 2000 ] : new Punkt[ 1 ] ),
    dPosA( options ? new Punkt[ 2000 ] : new Punkt[ 1 ] ),
    dGrößeA( options ? new Punkt[ 2000 ] : new Punkt[ 1 ] ),
    doa( 0 ),
    alpha( options ? new unsigned char[ 1000 ] : new unsigned char[ 1 ] ),
    alphaAnzahl( 0 ),
    rend( 0 ),
    alpha3D( 0 )
{
    alpha[ 0 ] = 0;
}

// Destruktor 
Bild::~Bild()
{
    if( delFc )
    {
        delete[] fc;
        fc = 0;
    }
    delete[] dPosA;
    delete[] dGrößeA;
    delete[] alpha;
    delete[] drawOff;
}

// privat
inline void Bild::alphaPixelP( int x, int y, int f )
{
    alphaPixelP( fc[ x + y * größe.x ], f );
}

inline void Bild::alphaPixelP( int &fc, int colorb )
{
    int alpha = ( ( colorb >> 24 ) & 0xFF );
    int na = ( 0x100 - alpha );
    fc = ( ( ( ( ( na * ( fc & 0xFF00FF ) ) >> 8 ) + ( ( alpha * ( colorb & 0xFF00FF ) ) >> 8 ) ) & 0xFF00FF ) |
        ( ( ( ( na * ( fc & 0x00FF00 ) ) >> 8 ) + ( ( alpha * ( colorb & 0x00FF00 ) ) >> 8 ) ) & 0x00FF00 ) |
           ( ( fc & 0xFF000000 ) ) ) * ( fc != 0 || !alpha3D ) | ( fc == 0 && alpha3D ) * colorb;
    //unsigned char *fc1 = (unsigned char*)&fc[ i ];
    //unsigned char *fc2 = (unsigned char*)&colorb;
    //unsigned char na = ~fc2[ 3 ];
    //fc1[ 3 ] = fc2[ 3 ];
    //fc1[ 2 ] = (unsigned char)( ( fc2[ 2 ] * fc2[ 3 ] + fc1[ 2 ] * na ) / 255 );
    //fc1[ 1 ] = (unsigned char)( ( fc2[ 1 ] * fc2[ 3 ] + fc1[ 1 ] * na ) / 255 );
    //fc1[ 0 ] = (unsigned char)( ( fc2[ 0 ] * fc2[ 3 ] + fc1[ 0 ] * na ) / 255 );
}

char Bild::getOutCode( Punkt& p ) const
{
    char ret = 0;
    if( p.x < dPosA[ doa ].x )
        ret |= 1;
    else if( p.x >= dGrößeA[ doa ].x )
        ret |= 2;
    if( p.y < dPosA[ doa ].y )
        ret |= 4;
    else if( p.y >= dGrößeA[ doa ].y )
        ret |= 8;
    return ret;
}

void Bild::drawFlatDreieck( int y1, int y2, float m1, float b1, float m2, float b2, int farbe )
{
    const int yStart = max( y1, dPosA[ doa ].y );
    const int yEnd = min( y2, dGrößeA[ doa ].y );
    for( int y = yStart; y < yEnd; y++ )
    {
        const int xStart = max( (int)( m1 * y + b1 + 0.5f ), dPosA[ doa ].x );
        const int xEnd = min( (int)( m2 * y + b2 + 0.5f ), dGrößeA[ doa ].x );
        for( int x = xStart; x < xEnd; x++ )
            fc[ x + y * größe.x ] = farbe;
    }
}

void Bild::drawFlatDreieckTextur( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
                                  double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur )
{
    const double yStart = max( y1, dPosA[ doa ].y );
    const double yEnd = min( y2, dGrößeA[ doa ].y );
    double tx_1 = tx1 + tx_1o * ( yStart - y1 ), ty_1 = ty1 + ty_1o * ( yStart - y1 ), tx_2 = tx2 + tx_2o * ( yStart - y1 ), ty_2 = ty2 + ty_2o * ( yStart - y1 );
    for( double y = yStart; y < yEnd; y++, tx_1 += tx_1o, ty_1 += ty_1o, tx_2 += tx_2o, ty_2 += ty_2o )
    {
        const double xStart = m1 * y + b1;
        const double xEnd = m2 * y + b2;
        drawLinieHTextur( Vec2< double >( xStart, y ), xEnd - xStart, Vec2< double >( tx_1, ty_1 ), Vec2< double >( tx_2, ty_2 ), txf, tyf, textur );
    }
}

void Bild::drawFlatDreieckAlpha( int y1, int y2, float m1, float b1, float m2, float b2, int farbe )
{
    const int yStart = max( (int)( y1 + 0.5 ), dPosA[ doa ].y );
    const int yEnd = min( (int)( y2 + 0.5 ), dGrößeA[ doa ].y );
    for( int y = yStart; y < yEnd; y++ )
    {
        const int xStart = max( (int)( m1 * ( (float)y + 0.5f ) + b1 + 0.5f ), dPosA[ doa ].x );
        const int xEnd = min( (int)( m2 * ( (float)y + 0.5 ) + b2 + 0.5f ), dGrößeA[ doa ].x );
        for( int x = xStart; x < xEnd; x++ )
            alphaPixelP( fc[ x + y * größe.x ], farbe );
    }
}

void Bild::drawFlatDreieckTexturAlpha( int y1, int y2, double m1, double b1, double m2, double b2, double tx1, double ty1, double tx2, double ty2,
                                       double tx_1o, double ty_1o, double tx_2o, double ty_2o, double txf, double tyf, Bild &textur )
{
    const double yStart = max( y1, dPosA[ doa ].y );
    const double yEnd = min( y2, dGrößeA[ doa ].y );
    double tx_1 = tx1 + tx_1o * ( yStart - y1 ), ty_1 = ty1 + ty_1o * ( yStart - y1 ), tx_2 = tx2 + tx_2o * ( yStart - y1 ), ty_2 = ty2 + ty_2o * ( yStart - y1 );
    for( double y = yStart; y < yEnd; y++, tx_1 += tx_1o, ty_1 += ty_1o, tx_2 += tx_2o, ty_2 += ty_2o )
    {
        const double xStart = m1 * y + b1;
        const double xEnd = m2 * y + b2;
        drawLinieHTexturAlpha( Vec2< double >( xStart, y ), xEnd - xStart, Vec2< double >( tx_1, ty_1 ), Vec2< double >( tx_2, ty_2 ), txf, tyf, textur );
    }
}

void Bild::drawLinieHTextur( Vec2< double > p, double län, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur ) // zeichnet eine horizontale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawLinieHTexturAlpha( p, län, ta, tb, txo, tyo, textur );
        return;
    }
    if( län < 0 )
    {
        p.x += län;
        län = -län;
        ta.Swap( tb );
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    if( p.y < dpy || p.y >= dgy )
        return;
    double off = 0;
    if( p.x < dpx )
    {
        off = dpx - p.x;
        län -= dpx - p.x;
        if( län <= 0 )
            return;
        p.x = dpx;
    }
    if( p.x + län >= dgx )
    {
        län -= p.x - dgx + län;
        if( län <= 0 )
            return;
    }
    int br = größe.x;
    int *fc = this->fc + (int)( p.x + (int)p.y * br );
    double x = ta.x + txo * off, y = ta.y + tyo * off;
    int *buffer = textur.getBuffer();
    int txtBr = textur.getBreite();
    for( int i = 0; i < län; ++i, ++fc )
    {
        *fc = buffer[ (int)( (int)( x + 0.5 ) + (int)( y + 0.5 ) * txtBr ) ];
        x += txo, y += tyo;
    }
    rend = 1;
}

void Bild::drawLinieHTexturAlpha( Vec2< double > p, double län, Vec2< double > ta, Vec2< double > tb, double txo, double tyo, Bild &textur ) // zeichnet eine horizontale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( län < 0 )
    {
        p.x += län;
        län = -län;
        ta.Swap( tb );
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    if( p.y < dpy || p.y >= dgy )
        return;
    double off = 0;
    if( p.x < dpx )
    {
        off = dpx - p.x;
        län -= dpx - p.x;
        if( län <= 0 )
            return;
        p.x = dpx;
    }
    if( p.x + län >= dgx )
    {
        län -= p.x - dgx + län;
        if( län <= 0 )
            return;
    }
    int br = größe.x;
    int *fc = this->fc + (int)( p.x + (int)p.y * br );
    double x = ta.x + txo * off, y = ta.y + tyo * off;
    int *buffer = textur.getBuffer();
    int txtBr = textur.getBreite();
    int f;
    for( int i = 0; i < län; ++i, ++fc )
    {
        f = buffer[ (int)( (int)( x + 0.5 ) + (int)( y + 0.5 ) * txtBr ) ];
        if( alpha[ alphaAnzahl ] )
        {
            unsigned char *cf = (unsigned char*)&f;
            cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
        }
        alphaPixelP( *fc, f );
        x += txo, y += tyo;
    }
    rend = 1;
}

// nicht constant

// Wird dieser Flag gesetzt, so wird beim Alpha Blending wenn die vorheriege Farbe 0 ist nur die neue mit ihrem Alpha Wert kopiert.
// Das ist sinnvoll für die Verwendung im 3DBildschirm, wo das Gezeichnette Bild später mittels Alpha Blending angezeigt wird
void Bild::setAlpha3D( bool erlaubt )
{
    alpha3D = erlaubt;
}

void Bild::setAlpha( unsigned char alpha ) // setzt die Transparenz der nachfolgenden Zeichnunge
{
    int last = this->alpha[ alphaAnzahl ];
    ++alphaAnzahl;
    assert( alphaAnzahl < 1000 );
    this->alpha[ alphaAnzahl ] = ( 255 - alpha ) > last ? ( 255 - alpha ) : last;
}

void Bild::releaseAlpha() // Löscht alpha
{
    --alphaAnzahl;
}

void Bild::setPixelBuffer( int *buffer, bool deleteBuffer, int breite, int höhe ) // setzt den Zeiger auf die Pixel des Bildes
{
    if( delFc )
        delete[]fc;
    fc = buffer;
    delFc = deleteBuffer;
    größe.x = breite;
    größe.y = höhe;
    rend = 1;
}

void Bild::neuBild( int breite, int höhe, int füllFarbe )
{
    if( fc && delFc )
        delete[] fc;
    größe.x = breite;
    größe.y = höhe;
    fc = new int[ größe.x * größe.y ];
    setFarbe( füllFarbe );
    drawOff[ 0 ].x = 0;
    drawOff[ 0 ].y = 0;
    dPosA[ 0 ].x = 0;
    dPosA[ 0 ].y = 0;
    dGrößeA[ 0 ] = größe;
    alphaAnzahl = 0;
    alpha[ 0 ] = 0;
    doa = 0;
    rend = 1;
}

void Bild::setFarbe( int f )
{
    if( ( f & 0xFF ) == ( ( f >> 8 ) & 0xFF ) && ( f & 0xFF ) == ( ( f >> 16 ) & 0xFF ) && ( f & 0xFF ) == ( ( f >> 24 ) & 0xFF ) )
        memset( fc, f, größe.x * größe.y * 4 );
    else
    {
        for( int *i = fc, *end = i + größe.x * größe.y; i < end; i++ )
            *i = f;
    }
    rend = 1;
}

void Bild::füllRegion( int x, int y, int b, int h, int ff )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaRegion( x, y, b, h, ff );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + b < dpx || y + h < dpy || x > dgx || y > dgy )
        return;
    if( x < dpx )
    {
        b -= dpx - x;
        x = dpx;
    }
    if( y < dpy )
    {
        h -= dpy - y;
        y = dpy;
    }
    b = ( x + b ) >= dgx ? ( dgx - x ) : b;
    h = ( y + h ) >= dgy ? ( dgy - y ) : h;
    int *pixel = fc + y * größe.x + x;
    int *rowEnd = pixel + b;
    for( int i = 0; i < h; pixel += größe.x - b, ++i, rowEnd += größe.x )
    {
        for( ; pixel < rowEnd; ++pixel )
            *pixel = ff;
    }
    rend = 1;
}

void Bild::alphaRegion( int x, int y, int b, int h, int ff )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + b < dpx || y + h < dpy || x > dgx || y > dgy )
        return;
    if( x < dpx )
    {
        b -= dpx - x;
        x = dpx;
    }
    if( y < dpy )
    {
        h -= dpy - y;
        y = dpy;
    }
    b = ( x + b ) >= dgx ? ( dgx - x ) : b;
    h = ( y + h ) >= dgy ? ( dgy - y ) : h;
    if( alpha[ alphaAnzahl ] )
    {
        unsigned char *cf = (unsigned char*)&ff;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    int *pixel = fc + y * größe.x + x;
    int *rowEnd = pixel + b;
    int alpha = ( ( ff >> 24 ) & 0xFF );
    int na = ( 0x100 - alpha );
    int i1 = ( alpha * ( ff & 0xFF00FF ) ) >> 8;
    int i2 = ( alpha * ( ff & 0x00FF00 ) ) >> 8;
    for( int i = 0; i < h; pixel += größe.x - b, ++i, rowEnd += größe.x )
    {
        for( ; pixel < rowEnd; ++pixel )
        {
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( ( *pixel & 0xFF000000 ) ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * ff;
        }
    }
    rend = 1;
}

void Bild::alphaPixel( int i, int f )
{
    if( !alpha[ alphaAnzahl ] )
        alphaPixelP( fc[ i ], f );
    if( alpha[ alphaAnzahl ] < 0xFF )
    {
        unsigned char *cf = (unsigned char*)&f;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
        alphaPixelP( fc[ i ], f );
        rend = 1;
    }
}

void Bild::alphaPixel( int x, int y, int f )
{
    if( !alpha[ alphaAnzahl ] )
        alphaPixelP( fc[ x + y * größe.x ], f );
    if( alpha[ alphaAnzahl ] < 0xFF )
    {
        unsigned char *cf = (unsigned char*)&f;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
        alphaPixelP( fc[ x + y * größe.x ], f );
        rend = 1;
    }
}

void Bild::alphaPixelDP( int x, int y, int f )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    if( x < dpx || y < dpy || x > dgx || y > dgy )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        unsigned char *cf = (unsigned char*)&f;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    alphaPixelP( fc[ x + y * größe.x ], f );
    rend = 1;
}

void Bild::alphaPixelDP( int i, int f )
{
    int x = i % größe.x;
    int y = i / größe.x;
    alphaPixelDP( x, y, f );
    rend = 1;
}

void Bild::setPixelDP( int x, int y, int f )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaPixelDP( x, y, f );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    if( x < dpx || y < dpy || x > dgx || y > dgy )
        return;
    fc[ x + y * größe.x ] = f;
    rend = 1;
}

void Bild::setPixelDP( int i, int f )
{
    int x = i % größe.x;
    int y = i / größe.x;
    setPixelDP( x, y, f );
    rend = 1;
}

void Bild::drawLinieH( int x, int y, int län, int f ) // zeichnet eine horizontale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawLinieHAlpha( x, y, län, f );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( y < dpy || y >= dgy )
        return;
    if( x < dpx )
    {
        län -= dpx - x;
        if( län <= 0 )
            return;
        x = dpx;
    }
    if( x + län >= dgx )
    {
        län -= x - dgx + län;
        if( län <= 0 )
            return;
    }
    int br = größe.x;
    int *fc = this->fc + x + y * br;
    int pval = län < 0 ? -1 : 1;
    län = län > 0 ? län : -län;
    for( int i = 0; i < län; ++i, fc += pval )
        *fc = f;
    rend = 1;
}

void Bild::drawLinieV( int x, int y, int län, int f ) // zeichnet eine vertikale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawLinieVAlpha( x, y, län, f );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x < dpx || x >= dgx )
        return;
    if( y < dpy )
    {
        län -= dpy - y;
        if( län <= 0 )
            return;
        y = dpy;
    }
    if( y + län >= dgy )
    {
        län -= y - dgy + län;
        if( län < 0 )
            return;
    }
    int br = größe.x;
    int *fc = this->fc + x + y * br;
    int pval = län < 0 ? -br : br;
    län = län > 0 ? län : -län;
    for( int i = 0; i < län; ++i, fc += pval )
        *fc = f;
    rend = 1;
}

void Bild::drawLinieHAlpha( int x, int y, int län, int f ) // zeichnet eine horizontale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( y < dpy || y >= dgy )
        return;
    if( x < dpx )
    {
        län -= dpx - x;
        if( län <= 0 )
            return;
        x = dpx;
    }
    if( x + län >= dgx )
    {
        län -= x - dgx + län;
        if( län <= 0 )
            return;
    }
    int br = größe.x;
    int pval = län < 0 ? -1 : 1;
    län = län > 0 ? län : -län;
    int end = 0;
    if( alpha[ alphaAnzahl ] )
    {
        unsigned char *cf = (unsigned char*)&f;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    int alpha = ( ( f >> 24 ) & 0xFF );
    int na = ( 0x100 - alpha );
    int i1 = ( alpha * ( f & 0xFF00FF ) ) >> 8;
    int i2 = ( alpha * ( f & 0x00FF00 ) ) >> 8;
    for( int i = x + y * br; end < län; ++end, i += pval )
    {
        fc[ i ] = ( ( ( ( ( na * ( fc[ i ] & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
            ( ( ( ( na * ( fc[ i ] & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                    ( ( fc[ i ] & 0xFF000000 ) ) ) * ( fc[ i ] != 0 || !alpha3D ) | ( fc[ i ] == 0 && alpha3D ) * f;
    }
    rend = 1;
}

void Bild::drawLinieVAlpha( int x, int y, int län, int f ) // zeichnet eine vertikale Linie
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x < dpx || x >= dgx )
        return;
    if( y < dpy )
    {
        län -= dpy - y;
        if( län <= 0 )
            return;
        y = dpy;
    }
    if( y + län >= dgy )
    {
        län -= y - dgy + län;
        if( län < 0 )
            return;
    }
    int br = größe.x;
    int pval = län < 0 ? -br : br;
    län = län > 0 ? län : -län;
    int end = 0;
    if( alpha[ alphaAnzahl ] )
    {
        unsigned char *cf = (unsigned char*)&f;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    int alpha = ( ( f >> 24 ) & 0xFF );
    int na = ( 0x100 - alpha );
    int i1 = ( alpha * ( f & 0xFF00FF ) ) >> 8;
    int i2 = ( alpha * ( f & 0x00FF00 ) ) >> 8;
    for( int i = x + y * br; end < län; ++end, i += pval )
    {
        fc[ i ] = ( ( ( ( ( na * ( fc[ i ] & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
            ( ( ( ( na * ( fc[ i ] & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                    ( fc[ i ] & 0xFF000000 ) ) * ( fc[ i ] != 0 || !alpha3D ) | ( fc[ i ] == 0 && alpha3D ) * f;
    }
    rend = 1;
}

void Bild::drawLinie( Punkt a, Punkt b, int fc ) // zeichnet eine Linie von Punkt( x1, y1 ) nach Punke( x2, y2 )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawLinieAlpha( a, b, fc );
        return;
    }
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    char outCode1 = getOutCode( a );
    char outCode2 = getOutCode( b );
    bool ok = 0;
    while( 1 )
    {
        int xMax = dGrößeA[ doa ].x - 1;
        int yMax = dGrößeA[ doa ].y - 1;
        if( !( outCode1 | outCode2 ) )
        {
            ok = 1;
            break;
        }
        else if( outCode1 & outCode2 )
            break;
        else
        {
            int x, y;
            char outCodeOut = outCode1 ? outCode1 : outCode2;
            if( outCodeOut & 8 )
            {
                x = (int)( a.x + ( b.x - a.x ) * ( yMax - a.y ) / ( b.y - a.y ) + 0.5 );
                y = yMax;
            }
            else if( outCodeOut & 4 )
            {
                x = (int)( a.x + ( b.x - a.x ) * ( dPosA[ doa ].y - a.y ) / ( b.y - a.y ) + 0.5 );
                y = dPosA[ doa ].y;
            }
            else if( outCodeOut & 2 )
            {
                y = (int)( a.y + ( b.y - a.y ) * ( xMax - a.x ) / ( b.x - a.x ) + 0.5 );
                x = xMax;
            }
            else if( outCodeOut & 1 )
            {
                y = (int)( a.y + ( b.y - a.y ) * ( dPosA[ doa ].x - a.x ) / ( b.x - a.x ) + 0.5 );
                x = dPosA[ doa ].x;
            }
            if( outCodeOut == outCode1 )
            {
                a.x = x;
                a.y = y;
                outCode1 = getOutCode( a );
            }
            else
            {
                b.x = x;
                b.y = y;
                outCode2 = getOutCode( b );
            }
        }
    }
    if( ok )
    {
        int xlän = b.x - a.x, axlän = abs( xlän );
        int ylän = b.y - a.y, aylän = abs( ylän );
        double xf = (double)xlän / ( aylän ? aylän : 1 );
        double yf = (double)ylän / ( axlän ? axlän : 1 );
        if( axlän > aylän )
            xf = xf < 0 ? -1 : 1;
        else
            yf = yf < 0 ? -1 : 1;
        double x = (double)a.x, y = (double)a.y;
        int maxP = (int)( sqrt( xlän * xlän + ylän * ylän ) + 0.5 );
        int count = 0;
        while( !( (int)( x + 0.5 ) == b.x && (int)( y + 0.5 ) == b.y ) && count < maxP )
        {
            ++count;
            this->fc[ (int)( (int)( x + 0.5 ) + (int)( y + 0.5 ) * größe.x ) ] = fc;
            x += xf, y += yf;
        }
        rend = 1;
    }
}

void Bild::drawLinieAlpha( Punkt a, Punkt b, int fc )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    char outCode1 = getOutCode( a );
    char outCode2 = getOutCode( b );
    bool ok = 0;
    while( 1 )
    {
        int xMax = dGrößeA[ doa ].x - 1;
        int yMax = dGrößeA[ doa ].y - 1;
        if( !( outCode1 | outCode2 ) )
        {
            ok = 1;
            break;
        }
        else if( outCode1 & outCode2 )
            break;
        else
        {
            int x, y;
            char outCodeOut = outCode1 ? outCode1 : outCode2;
            if( outCodeOut & 8 )
            {
                x = (int)( a.x + ( b.x - a.x ) * ( yMax - a.y ) / ( b.y - a.y ) + 0.5 );
                y = yMax;
            }
            else if( outCodeOut & 4 )
            {
                x = (int)( a.x + ( b.x - a.x ) * ( dPosA[ doa ].y - a.y ) / ( b.y - a.y ) + 0.5 );
                y = dPosA[ doa ].y;
            }
            else if( outCodeOut & 2 )
            {
                y = (int)( a.y + ( b.y - a.y ) * ( xMax - a.x ) / ( b.x - a.x ) + 0.5 );
                x = xMax;
            }
            else if( outCodeOut & 1 )
            {
                y = (int)( a.y + ( b.y - a.y ) * ( dPosA[ doa ].x - a.x ) / ( b.x - a.x ) + 0.5 );
                x = dPosA[ doa ].x;
            }
            if( outCodeOut == outCode1 )
            {
                a.x = x;
                a.y = y;
                outCode1 = getOutCode( a );
            }
            else
            {
                b.x = x;
                b.y = y;
                outCode2 = getOutCode( b );
            }
        }
    }
    if( ok )
    {
        int xlän = b.x - a.x, axlän = abs( xlän );
        int ylän = b.y - a.y, aylän = abs( ylän );
        double xf = (double)xlän / ( aylän ? aylän : 1 );
        double yf = (double)ylän / ( axlän ? axlän : 1 );
        if( axlän > aylän )
            xf = xf < 0 ? -1 : 1;
        else
            yf = yf < 0 ? -1 : 1;
        double x = (double)a.x, y = (double)a.y;
        if( alpha[ alphaAnzahl ] )
        {
            unsigned char *cf = (unsigned char*)&fc;
            cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
        }
        int maxP = (int)( sqrt( xlän * xlän + ylän * ylän ) + 0.5 );
        int count = 0;
        int alpha = ( ( fc >> 24 ) & 0xFF );
        int na = ( 0x100 - alpha );
        int i1 = ( alpha * ( fc & 0xFF00FF ) ) >> 8;
        int i2 = ( alpha * ( fc & 0x00FF00 ) ) >> 8;
        while( !( (int)( x + 0.5 ) == b.x && (int)( y + 0.5 ) == b.y ) && count < maxP )
        {
            ++count;
            int &pixel = this->fc[ (int)( x + 0.5 ) + (int)( y + 0.5 ) * größe.x ];
            pixel = ( ( ( ( ( na * ( pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                      ( pixel & 0xFF000000 ) ) * ( pixel != 0 || !alpha3D ) | ( pixel == 0 && alpha3D ) * fc;
            x += xf, y += yf;
        }
        rend = 1;
    }
}

void Bild::füllKreis( int xOff, int yOff, int r, int fc ) // zeichnet einen Kreis um Punkt( xOff, yOff ) mit radius r
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    for( int i = r; i > 0; i-- )
        drawKreis( xOff, yOff, i, fc );
}

void Bild::drawKreis( int xOff, int yOff, int r, int fc ) // zeichnet einen Kreis um Punkt( xOff, yOff ) mit radius r
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawKreisAlpha( xOff, yOff, r, fc );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    xOff += drawOff[ doa ].x;
    yOff += drawOff[ doa ].y;
    if( xOff + r < dpx || xOff - r >= dgx || yOff + r < dpy || yOff - r >= dgy )
        return;
    for( int a = 0; a < r; ++a )
    {
        int b = (int)( sqrt( (long)( r * r - a * a ) ) + 0.5 );
        if( xOff + a < dgx && xOff + a > dpx && yOff + b < dgy && yOff + b > dpy )
            this->fc[ xOff + a + ( yOff + b ) * größe.x ] = fc;
        if( xOff - a < dgx && xOff - a > dpx && yOff + b < dgy && yOff + b > dpy )
            this->fc[ xOff - a + ( yOff + b ) * größe.x ] = fc;
        if( xOff + a < dgx && xOff + a > dpx && yOff - b < dgy && yOff - b > dpy )
            this->fc[ xOff + a + ( yOff - b ) * größe.x ] = fc;
        if( xOff - a < dgx && xOff - a > dpx && yOff - b < dgy && yOff - b > dpy )
            this->fc[ xOff - a + ( yOff - b ) * größe.x ] = fc;
        if( xOff + b < dgx && xOff + b > dpx && yOff + a < dgy && yOff + a > dpy )
            this->fc[ xOff + b + ( yOff + a ) * größe.x ] = fc;
        if( xOff - b < dgx && xOff - b > dpx && yOff + a < dgy && yOff + a > dpy )
            this->fc[ xOff - b + ( yOff + a ) * größe.x ] = fc;
        if( xOff + b < dgx && xOff + b > dpx && yOff - a < dgy && yOff - a > dpy )
            this->fc[ xOff + b + ( yOff - a ) * größe.x ] = fc;
        if( xOff - b < dgx && xOff - b > dpx && yOff - a < dgy && yOff - a > dpy )
            this->fc[ xOff - b + ( yOff - a ) * größe.x ] = fc;
    }
    rend = 1;
}

void Bild::drawKreisAlpha( int xOff, int yOff, int r, int fc )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    xOff += drawOff[ doa ].x;
    yOff += drawOff[ doa ].y;
    if( xOff + r < dpx || xOff - r >= dgx || yOff + r < dpy || yOff - r >= dgy )
        return;
    if( alpha[ alphaAnzahl ] < 0xFF )
    {
        unsigned char *cf = (unsigned char*)&fc;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    int alpha = ( ( fc >> 24 ) & 0xFF );
    int na = ( 0x100 - alpha );
    int i1 = ( alpha * ( fc & 0xFF00FF ) ) >> 8;
    int i2 = ( alpha * ( fc & 0x00FF00 ) ) >> 8;
    for( int a = 0; a < r; ++a )
    {
        int b = (int)( sqrt( (long)( r * r - a * a ) ) + 0.5 );
        int *pixel = 0;
        if( xOff + a < dgx && xOff + a > dpx && yOff + b < dgy && yOff + b > dpy )
        {
            pixel = &this->fc[ xOff + a + ( yOff + b ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff - a < dgx && xOff - a > dpx && yOff + b < dgy && yOff + b > dpy )
        {
            pixel = &this->fc[ xOff - a + ( yOff + b ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff + a < dgx && xOff + a > dpx && yOff - b < dgy && yOff - b > dpy )
        {
            pixel = &this->fc[ xOff + a + ( yOff - b ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff - a < dgx && xOff - a > dpx && yOff - b < dgy && yOff - b > dpy )
        {
            pixel = &this->fc[ xOff - a + ( yOff - b ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff + b < dgx && xOff + b > dpx && yOff + a < dgy && yOff + a > dpy )
        {
            pixel = &this->fc[ xOff + b + ( yOff + a ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff - b < dgx && xOff - b > dpx && yOff + a < dgy && yOff + a > dpy )
        {
            pixel = &this->fc[ xOff - b + ( yOff + a ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff + b < dgx && xOff + b > dpx && yOff - a < dgy && yOff - a > dpy )
        {
            pixel = &this->fc[ xOff + b + ( yOff - a ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
        if( xOff - b < dgx && xOff - b > dpx && yOff - a < dgy && yOff - a > dpy )
        {
            pixel = &this->fc[ xOff - b + ( yOff - a ) * größe.x ];
            *pixel = ( ( ( ( ( na * ( *pixel & 0xFF00FF ) ) >> 8 ) + i1 ) & 0xFF00FF ) |
                ( ( ( ( na * ( *pixel & 0x00FF00 ) ) >> 8 ) + i2 ) & 0x00FF00 ) |
                       ( *pixel & 0xFF000000 ) ) * ( *pixel != 0 || !alpha3D ) | ( *pixel == 0 && alpha3D ) * fc;
        }
    }
    rend = 1;
}

void Bild::drawBild( int x, int y, int br, int hö, Bild &zBild ) // zeichet zBild
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaBild( x, y, br, hö, zBild );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int xx, ygr, ygr2;
    for( int yy = yst2; yy < dgy; ++yy )
    {
        ygr = yy * größe.x;
        ygr2 = ( yy - yst2 + yst ) * bb;
        for( xx = xst2; xx < dgx; ++xx )
            fc[ xx + ygr ] = ff[ ( xx - xst2 + xst ) + ygr2 ];
    }
    rend = 1;
}

void Bild::alphaBild( int x, int y, int br, int hö, Bild &zBild )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    if( !alpha[ alphaAnzahl ] )
    {
        int xx, ygr, ygr2;
        for( int yy = yst2; yy < dgy; ++yy )
        {
            ygr = yy * größe.x;
            ygr2 = ( yy - yst2 + yst ) * bb;
            for( xx = xst2; xx < dgx; ++xx )
                alphaPixelP( fc[ xx + ygr ], ff[ ( xx - xst2 + xst ) + ygr2 ] );
        }
    }
    else
    {
        int xx, ygr, ygr2;
        for( int yy = yst2; yy < dgy; ++yy )
        {
            ygr = yy * größe.x;
            ygr2 = ( yy - yst2 + yst ) * bb;
            for( xx = xst2; xx < dgx; ++xx )
            {
                int fc = ff[ ( xx - xst2 + xst ) + ygr2 ];
                unsigned char *cf = (unsigned char*)&fc;
                cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
                alphaPixelP( this->fc[ xx + ygr ], fc );
            }
        }
    }
    rend = 1;
}

void Bild::drawBild90( int x, int y, int br, int hö, Bild &zBild ) // Zeichnet ein um 90 Grad nach rchts gedrehtes Bild
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaBild90( x, y, br, hö, zBild );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + hö < dpx || y + br < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + hö, dgx );
    dgy = minInt( y + br, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int yy, xbb;
    for( int xx = xst2; xx < dgx; ++xx )
    {
        xbb = ( zBild.getHöhe() - ( xx - xst2 + xst + 1 ) ) * bb;
        for( yy = yst2; yy < dgy; ++yy )
            fc[ xx + yy * größe.x ] = ff[ ( yy - yst2 + yst ) + xbb ];
    }
    rend = 1;
}

void Bild::alphaBild90( int x, int y, int br, int hö, Bild &zBild )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + hö < dpx || y + br < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + hö, dgx );
    dgy = minInt( y + br, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    if( !alpha[ alphaAnzahl ] )
    {
        int yy, xbb;
        for( int xx = xst2; xx < dgx; ++xx )
        {
            xbb = ( zBild.getHöhe() - ( xx - xst2 + xst + 1 ) ) * bb;
            for( yy = yst2; yy < dgy; ++yy )
                alphaPixelP( xx, yy, ff[ ( yy - yst2 + yst ) + xbb ] );
        }
    }
    else
    {
        int yy, xbb;
        for( int xx = xst2; xx < dgx; ++xx )
        {
            xbb = ( zBild.getHöhe() - ( xx - xst2 + xst + 1 ) ) * bb;
            for( yy = yst2; yy < dgy; ++yy )
            {
                int fc = ff[ ( yy - yst2 + yst ) + xbb ];
                unsigned char *cf = (unsigned char*)&fc;
                cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
                alphaPixelP( xx, yy, fc );
            }
        }
    }
    rend = 1;
}

void Bild::drawBild180( int x, int y, int br, int hö, Bild &zBild ) // Zeichnet ein um 180 Grad nach rchts gedrehtes Bild
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaBild180( x, y, br, hö, zBild );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int xx, ygr, ybb;
    for( int yy = yst2; yy < dgy; ++yy )
    {
        ygr = yy * größe.x;
        ybb = ( zBild.getHöhe() - ( yy - yst2 + yst + 1 ) ) * bb;
        for( xx = xst2; xx < dgx; ++xx )
            fc[ xx + ygr ] = ff[ ( bb - ( xx - xst2 + xst + 1 ) ) + ybb ];
    }
    rend = 1;
}

void Bild::alphaBild180( int x, int y, int br, int hö, Bild &zBild )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    if( !alpha[ alphaAnzahl ] )
    {
        int xx, ygr, ybb;
        for( int yy = yst2; yy < dgy; ++yy )
        {
            ygr = yy * größe.x;
            ybb = ( zBild.getHöhe() - ( yy - yst2 + yst + 1 ) ) * bb;
            for( xx = xst2; xx < dgx; ++xx )
                alphaPixelP( fc[ xx + ygr ], ff[ ( bb - ( xx - xst2 + xst + 1 ) ) + ybb ] );
        }
    }
    else
    {
        int xx, ygr, ybb;
        for( int yy = yst2; yy < dgy; ++yy )
        {
            ygr = yy * größe.x;
            ybb = ( zBild.getHöhe() - ( yy - yst2 + yst + 1 ) ) * bb;
            for( xx = xst2; xx < dgx; ++xx )
            {
                int fc = ff[ ( bb - ( xx - xst2 + xst + 1 ) ) + ybb ];
                unsigned char *cf = (unsigned char*)&fc;
                cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
                alphaPixelP( this->fc[ xx + ygr ], fc );
            }
        }
    }
    rend = 1;
}

void Bild::drawBild270( int x, int y, int br, int hö, Bild &zBild ) // Zeichnet ein um 270 Grad nach rchts gedrehtes Bild
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaBild270( x, y, br, hö, zBild );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + hö < dpx || y + br < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getHöhe() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + hö, dgx );
    dgy = minInt( y + br, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int yy, xbb;
    for( int xx = xst2; xx < dgx; ++xx )
    {
        xbb = ( xx - xst2 + xst ) * bb;
        for( yy = yst2; yy < dgy; ++yy )
            fc[ xx + yy * größe.x ] = ff[ ( bb - ( yy - yst2 + yst + 1 ) ) + xbb ];
    }
    rend = 1;
}

void Bild::alphaBild270( int x, int y, int br, int hö, Bild &zBild )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + hö < dpx || y + br < dpy || x > dgx || y > dgy )
        return;
    br = minInt( br, zBild.getBreite() );
    hö = minInt( hö, zBild.getBreite() );
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + hö, dgx );
    dgy = minInt( y + br, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    if( !alpha[ alphaAnzahl ] )
    {
        int yy, xbb;
        for( int xx = xst2; xx < dgx; ++xx )
        {
            xbb = ( xx - xst2 + xst ) * bb;
            for( yy = yst2; yy < dgy; ++yy )
                alphaPixelP( xx, yy, ff[ ( bb - ( yy - yst2 + yst + 1 ) ) + xbb ] );
        }
    }
    else
    {
        int yy, xbb;
        for( int xx = xst2; xx < dgx; ++xx )
        {
            xbb = ( xx - xst2 + xst ) * bb;
            for( yy = yst2; yy < dgy; ++yy )
            {
                int fc = ff[ ( bb - ( yy - yst2 + yst + 1 ) ) + xbb ];
                unsigned char *cf = (unsigned char*)&fc;
                cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
                alphaPixelP( xx, yy, fc );
            }
        }
    }
    rend = 1;
}

void Bild::drawBildSkall( int x, int y, int br, int hö, Bild &zBild ) // zeichet zBild Skalliert
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        alphaBildSkall( x, y, br, hö, zBild );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    double xo = zBild.getBreite() / (double)br;
    double yo = zBild.getHöhe() / (double)hö;
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int xx, ygr, ygr2;
    double xb = 0, yb = yst * yo;
    for( int yy = yst2; yy < dgy; ++yy, yb += yo )
    {
        ygr = yy * größe.x;
        ygr2 = (int)( ( yy - yst2 + yst ) * yo ) * bb;
        for( xx = xst2, xb = xst * xo; xx < dgx; ++xx, xb += xo )
            fc[ xx + ygr ] = ff[ (int)xb + ygr2 ];
    }
    rend = 1;
}

void Bild::alphaBildSkall( int x, int y, int br, int hö, Bild &zBild )
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    x += drawOff[ doa ].x;
    y += drawOff[ doa ].y;
    if( x + br < dpx || y + hö < dpy || x > dgx || y > dgy )
        return;
    double xo = zBild.getBreite() / (double)br;
    double yo = zBild.getHöhe() / (double)hö;
    int xst = maxInt( dpx - x, 0 );
    int yst = maxInt( dpy - y, 0 );
    int xst2 = maxInt( x, dpx );
    int yst2 = maxInt( y, dpy );
    dgx = minInt( x + br, dgx );
    dgy = minInt( y + hö, dgy );
    int bb = zBild.getBreite();
    int *ff = zBild.getBuffer();
    int xx, ygr, ygr2;
    double xb = 0;
    for( int yy = yst2; yy < dgy; ++yy )
    {
        ygr = yy * größe.x;
        ygr2 = (int)( ( yy - yst2 + yst ) * yo ) * bb;
        for( xx = xst2, xb = xst * xo; xx < dgx; ++xx, xb += xo )
        {
            int f = ff[ (int)xb + ygr2 ];
            unsigned char *cf = (unsigned char*)&f;
            cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
            alphaPixelP( fc[ xx + ygr ], f );
        }
    }
    rend = 1;
}

void Bild::drawDreieck( Punkt a, Punkt b, Punkt c, int farbe ) // füllt eine Dreieck aus
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawDreieckAlpha( a, b, c, farbe );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    c += drawOff[ doa ];
    if( ( a.x < dpx && b.x < dpx && c.x < dpx ) || ( a.y < dpy && b.y < dpy && c.y < dpy ) ||
        ( a.x > dgx && b.x > dgx && c.x > dgx ) || ( a.y > dgy && b.y > dgy && c.y > dgy ) )
        return;
    if( b.y < a.y )
        a.Swap( b );
    if( c.y < b.y )
        b.Swap( c );
    if( b.y < a.y )
        a.Swap( b );
    if( a.y == b.y )
    {
        if( b.x < a.x )
            a.Swap( b );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        const float m3 = (float)( b.x - c.x ) / ( b.y - c.y );
        float b2 = a.x - m2 * a.y;
        float b3 = b.x - m3 * b.y;
        drawFlatDreieck( b.y, c.y, m2, b2, m3, b3, farbe );
    }
    else if( b.y == c.y )
    {
        if( c.x < b.x )
            b.Swap( c );
        const float m1 = (float)( a.x - b.x ) / ( a.y - b.y );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        float b1 = a.x - m1 * a.y;
        float b2 = a.x - m2 * a.y;
        drawFlatDreieck( a.y, b.y, m1, b1, m2, b2, farbe );
    }
    else
    {
        const float m1 = (float)( a.x - b.x ) / ( a.y - b.y );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        const float m3 = (float)( b.x - c.x ) / ( b.y - c.y );
        float b1 = a.x - m1 * a.y;
        float b2 = a.x - m2 * a.y;
        float b3 = b.x - m3 * b.y;
        const float qx = m2 * b.y + b2;
        if( qx < b.x )
        {
            drawFlatDreieck( a.y, b.y, m2, b2, m1, b1, farbe );
            drawFlatDreieck( b.y, c.y, m2, b2, m3, b3, farbe );
        }
        else
        {
            drawFlatDreieck( a.y, b.y, m1, b1, m2, b2, farbe );
            drawFlatDreieck( b.y, c.y, m3, b3, m2, b2, farbe );
        }
    }
    rend = 1;
}

void Bild::drawDreieckTextur( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur ) // füllt eine Dreieck aus
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        drawDreieckTexturAlpha( a, b, c, ta, tb, tc, textur );
        return;
    }
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    c += drawOff[ doa ];
    if( ( a.x < dpx && b.x < dpx && c.x < dpx ) || ( a.y < dpy && b.y < dpy && c.y < dpy ) ||
        ( a.x > dgx && b.x > dgx && c.x > dgx ) || ( a.y > dgy && b.y > dgy && c.y > dgy ) )
        return;
    if( b.y < a.y )
    {
        a.Swap( b );
        ta.Swap( tb );
    }
    if( c.y < b.y )
    {
        b.Swap( c );
        tb.Swap( tc );
    }
    if( b.y < a.y )
    {
        a.Swap( b );
        ta.Swap( tb );
    }
    const double m1 = (double)( a.x - b.x ) / ( a.y - b.y );
    const double m2 = (double)( a.x - c.x ) / ( a.y - c.y );
    const double m3 = (double)( b.x - c.x ) / ( b.y - c.y );
    double b1 = a.x - m1 * a.y;
    double b2 = a.x - m2 * a.y;
    double b3 = b.x - m3 * b.y;
    const double qx = m2 * b.y + b2;
    if( qx < b.x )
    {
        double tx1o, ty1o, tx2o, ty2o;
        if( c.y - a.y )
        {
            tx1o = (double)( tc.x - ta.x ) / ( c.y - a.y );
            ty1o = (double)( tc.y - ta.y ) / ( c.y - a.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        if( b.y - a.y )
        {
            tx2o = (double)( tb.x - ta.x ) / ( b.y - a.y );
            ty2o = (double)( tb.y - ta.y ) / ( b.y - a.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        Vec2< double > q( ta.x + tx1o * ( b.y - a.y ), ta.y + ty1o * ( b.y - a.y ) );
        double txf, tyf;
        if( b.x - qx )
        {
            txf = ( tb.x - q.x ) / ( b.x - qx );
            tyf = ( tb.y - q.y ) / ( b.x - qx );
        }
        else
        {
            txf = 0;
            tyf = 0;
        }
        drawFlatDreieckTextur( a.y, b.y, m2, b2, m1, b1, ta.x, ta.y, ta.x, ta.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
        if( c.y - b.y )
        {
            tx2o = (double)( tc.x - tb.x ) / ( c.y - b.y );
            ty2o = (double)( tc.y - tb.y ) / ( c.y - b.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        drawFlatDreieckTextur( b.y, c.y, m2, b2, m3, b3, q.x, q.y, tb.x, tb.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
    }
    else
    {
        double tx1o, ty1o, tx2o, ty2o;
        if( b.y - a.y )
        {
            tx1o = (double)( tb.x - ta.x ) / ( b.y - a.y );
            ty1o = (double)( tb.y - ta.y ) / ( b.y - a.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        if( c.y - a.y )
        {
            tx2o = (double)( tc.x - ta.x ) / ( c.y - a.y );
            ty2o = (double)( tc.y - ta.y ) / ( c.y - a.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        Vec2< double > q( ta.x + tx2o * ( b.y - a.y ), ta.y + ty2o * ( b.y - a.y ) );
        double txf, tyf;
        if( qx - b.x )
        {
            txf = ( q.x - tb.x ) / ( qx - b.x );
            tyf = ( q.y - tb.y ) / ( qx - b.x );
        }
        else
        {
            txf = 0;
            tyf = 0;
        }
        drawFlatDreieckTextur( a.y, b.y, m1, b1, m2, b2, ta.x, ta.y, ta.x, ta.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
        if( c.y - b.y )
        {
            tx1o = (double)( tc.x - tb.x ) / ( c.y - b.y );
            ty1o = (double)( tc.y - tb.y ) / ( c.y - b.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        drawFlatDreieckTextur( b.y, c.y, m3, b3, m2, b2, tb.x, tb.y, q.x, q.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
    }
    rend = 1;
}

void Bild::drawDreieckAlpha( Punkt a, Punkt b, Punkt c, int farbe ) // füllt eine Dreieck aus
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    c += drawOff[ doa ];
    if( ( a.x < dpx && b.x < dpx && c.x < dpx ) || ( a.y < dpy && b.y < dpy && c.y < dpy ) ||
        ( a.x > dgx && b.x > dgx && c.x > dgx ) || ( a.y > dgy && b.y > dgy && c.y > dgy ) )
        return;
    if( alpha[ alphaAnzahl ] )
    {
        unsigned char *cf = (unsigned char*)&farbe;
        cf[ 3 ] = ( cf[ 3 ] > alpha[ alphaAnzahl ] ) * ( cf[ 3 ] - alpha[ alphaAnzahl ] );
    }
    if( b.y < a.y )
        a.Swap( b );
    if( c.y < b.y )
        b.Swap( c );
    if( b.y < a.y )
        a.Swap( b );
    if( a.y == b.y )
    {
        if( b.x < a.x )
            a.Swap( b );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        const float m3 = (float)( b.x - c.x ) / ( b.y - c.y );
        float b2 = a.x - m2 * a.y;
        float b3 = b.x - m3 * b.y;
        drawFlatDreieckAlpha( b.y, c.y, m2, b2, m3, b3, farbe );
    }
    else if( b.y == c.y )
    {
        if( c.x < b.x )
            b.Swap( c );
        const float m1 = (float)( a.x - b.x ) / ( a.y - b.y );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        float b1 = a.x - m1 * a.y;
        float b2 = a.x - m2 * a.y;
        drawFlatDreieckAlpha( a.y, b.y, m1, b1, m2, b2, farbe );
    }
    else
    {
        const float m1 = (float)( a.x - b.x ) / ( a.y - b.y );
        const float m2 = (float)( a.x - c.x ) / ( a.y - c.y );
        const float m3 = (float)( b.x - c.x ) / ( b.y - c.y );
        float b1 = a.x - m1 * a.y;
        float b2 = a.x - m2 * a.y;
        float b3 = b.x - m3 * b.y;
        const float qx = m2 * b.y + b2;
        if( qx < b.x )
        {
            drawFlatDreieckAlpha( a.y, b.y, m2, b2, m1, b1, farbe );
            drawFlatDreieckAlpha( b.y, c.y, m2, b2, m3, b3, farbe );
        }
        else
        {
            drawFlatDreieckAlpha( a.y, b.y, m1, b1, m2, b2, farbe );
            drawFlatDreieckAlpha( b.y, c.y, m3, b3, m2, b2, farbe );
        }
    }
    rend = 1;
}

void Bild::drawDreieckTexturAlpha( Punkt a, Punkt b, Punkt c, Punkt ta, Punkt tb, Punkt tc, Bild &textur ) // füllt eine Dreieck aus
{
    if( alpha[ alphaAnzahl ] == 0xFF )
        return;
    int dpx = dPosA[ doa ].x;
    int dpy = dPosA[ doa ].y;
    int dgx = dGrößeA[ doa ].x;
    int dgy = dGrößeA[ doa ].y;
    a += drawOff[ doa ];
    b += drawOff[ doa ];
    c += drawOff[ doa ];
    if( ( a.x < dpx && b.x < dpx && c.x < dpx ) || ( a.y < dpy && b.y < dpy && c.y < dpy ) ||
        ( a.x > dgx && b.x > dgx && c.x > dgx ) || ( a.y > dgy && b.y > dgy && c.y > dgy ) )
        return;
    if( b.y < a.y )
    {
        a.Swap( b );
        ta.Swap( tb );
    }
    if( c.y < b.y )
    {
        b.Swap( c );
        tb.Swap( tc );
    }
    if( b.y < a.y )
    {
        a.Swap( b );
        ta.Swap( tb );
    }
    const double m1 = (double)( a.x - b.x ) / ( a.y - b.y );
    const double m2 = (double)( a.x - c.x ) / ( a.y - c.y );
    const double m3 = (double)( b.x - c.x ) / ( b.y - c.y );
    double b1 = a.x - m1 * a.y;
    double b2 = a.x - m2 * a.y;
    double b3 = b.x - m3 * b.y;
    const double qx = m2 * b.y + b2;
    if( qx < b.x )
    {
        double tx1o, ty1o, tx2o, ty2o;
        if( c.y - a.y )
        {
            tx1o = (double)( tc.x - ta.x ) / ( c.y - a.y );
            ty1o = (double)( tc.y - ta.y ) / ( c.y - a.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        if( b.y - a.y )
        {
            tx2o = (double)( tb.x - ta.x ) / ( b.y - a.y );
            ty2o = (double)( tb.y - ta.y ) / ( b.y - a.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        Vec2< double > q( ta.x + tx1o * ( b.y - a.y ), ta.y + ty1o * ( b.y - a.y ) );
        double txf, tyf;
        if( b.x - qx )
        {
            txf = ( tb.x - q.x ) / ( b.x - qx );
            tyf = ( tb.y - q.y ) / ( b.x - qx );
        }
        else
        {
            txf = 0;
            tyf = 0;
        }
        drawFlatDreieckTexturAlpha( a.y, b.y, m2, b2, m1, b1, ta.x, ta.y, ta.x, ta.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
        if( c.y - b.y )
        {
            tx2o = (double)( tc.x - tb.x ) / ( c.y - b.y );
            ty2o = (double)( tc.y - tb.y ) / ( c.y - b.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        drawFlatDreieckTexturAlpha( b.y, c.y, m2, b2, m3, b3, q.x, q.y, tb.x, tb.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
    }
    else
    {
        double tx1o, ty1o, tx2o, ty2o;
        if( b.y - a.y )
        {
            tx1o = (double)( tb.x - ta.x ) / ( b.y - a.y );
            ty1o = (double)( tb.y - ta.y ) / ( b.y - a.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        if( c.y - a.y )
        {
            tx2o = (double)( tc.x - ta.x ) / ( c.y - a.y );
            ty2o = (double)( tc.y - ta.y ) / ( c.y - a.y );
        }
        else
        {
            tx2o = 0;
            ty2o = 0;
        }
        Vec2< double > q( ta.x + tx2o * ( b.y - a.y ), ta.y + ty2o * ( b.y - a.y ) );
        double txf, tyf;
        if( qx - b.x )
        {
            txf = ( q.x - tb.x ) / ( qx - b.x );
            tyf = ( q.y - tb.y ) / ( qx - b.x );
        }
        else
        {
            txf = 0;
            tyf = 0;
        }
        drawFlatDreieckTexturAlpha( a.y, b.y, m1, b1, m2, b2, ta.x, ta.y, ta.x, ta.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
        if( c.y - b.y )
        {
            tx1o = (double)( tc.x - tb.x ) / ( c.y - b.y );
            ty1o = (double)( tc.y - tb.y ) / ( c.y - b.y );
        }
        else
        {
            tx1o = 0;
            ty1o = 0;
        }
        drawFlatDreieckTexturAlpha( b.y, c.y, m3, b3, m2, b2, tb.x, tb.y, q.x, q.y, tx1o, ty1o, tx2o, ty2o, txf, tyf, textur );
    }
    rend = 1;
}

bool Bild::setDrawOptions( const Punkt &pos, const Punkt &gr ) // setzt die Drawoptionen
{
    int dx = drawOff[ doa ].x, dy = drawOff[ doa ].y;
    int xx = dPosA[ doa ].x, yy = dPosA[ doa ].y;
    int bb = dGrößeA[ doa ].x, hh = dGrößeA[ doa ].y;
    if( dx + pos.x + gr.x < 0 || dy + pos.y + gr.y < 0 || dx + pos.x >= größe.x || dy + pos.y >= größe.y )
        return 0;
    if( pos.x + gr.x + dx < xx || pos.y + gr.y + dy < yy || dx + pos.x >= bb || dy + pos.y >= hh )
        return 0;
    ++doa;
    assert( doa < 2000 );
    dPosA[ doa ].x = maxInt( pos.x + dx, xx );
    dPosA[ doa ].y = maxInt( pos.y + dy, yy );
    dGrößeA[ doa ].x = minInt( pos.x + gr.x + dx, bb );
    dGrößeA[ doa ].y = minInt( pos.y + gr.y + dy, hh );
    drawOff[ doa ].x = dx + pos.x;
    drawOff[ doa ].y = dy + pos.y;
    return 1;
}

bool Bild::setDrawOptions( int x, int y, int br, int hö )
{
    int dx = drawOff[ doa ].x, dy = drawOff[ doa ].y;
    int xx = dPosA[ doa ].x, yy = dPosA[ doa ].y;
    int bb = dGrößeA[ doa ].x, hh = dGrößeA[ doa ].y;
    if( dx + x + br < 0 || dy + y + hö < 0 || dx + x >= größe.x || dy + y >= größe.y )
        return 0;
    if( x + br + dx < xx || y + hö + dy < yy || dx + x >= bb || dy + y >= hh )
        return 0;
    ++doa;
    assert( doa < 2000 );
    dPosA[ doa ].x = maxInt( x + dx, xx );
    dPosA[ doa ].y = maxInt( y + dy, yy );
    dGrößeA[ doa ].x = minInt( x + br + dx, bb );
    dGrößeA[ doa ].y = minInt( y + hö + dy, hh );
    drawOff[ doa ].x = dx + x;
    drawOff[ doa ].y = dy + y;
    return 1;
}

bool Bild::setDrawOptionsErzwingen( const Punkt &pos, const Punkt &gr ) // setzt die Drawoptionen
{
    int dx = drawOff[ doa ].x, dy = drawOff[ doa ].y;
    if( dx + pos.x + gr.x < 0 || dy + pos.y + gr.y < 0 || dx + pos.x >= größe.x || dy + pos.y >= größe.y )
        return 0;
    ++doa;
    assert( doa < 2000 );
    dPosA[ doa ].x = maxInt( pos.x + dx, 0 );
    dPosA[ doa ].y = maxInt( pos.y + dy, 0 );
    dGrößeA[ doa ].x = minInt( pos.x + gr.x + dx, größe.x );
    dGrößeA[ doa ].y = minInt( pos.y + gr.y + dy, größe.y );
    drawOff[ doa ].x = dx + pos.x;
    drawOff[ doa ].y = dy + pos.y;
    return 1;
}

bool Bild::setDrawOptionsErzwingen( int x, int y, int br, int hö ) // setzt die Drawoptionen
{
    int dx = drawOff[ doa ].x, dy = drawOff[ doa ].y;
    if( dx + x + br < 0 || dy + y + hö < 0 || dx + x >= größe.x || dy + y >= größe.y )
        return 0;
    ++doa;
    assert( doa < 2000 );
    dPosA[ doa ].x = maxInt( x + dx, 0 );
    dPosA[ doa ].y = maxInt( y + dy, 0 );
    dGrößeA[ doa ].x = minInt( x + br + dx, größe.x );
    dGrößeA[ doa ].y = minInt( y + hö + dy, größe.y );
    drawOff[ doa ].x = dx + x;
    drawOff[ doa ].y = dy + y;
    return 1;
}

void Bild::addScrollOffset( int xOff, int yOff ) // setzt ScrollOffset
{
    drawOff[ doa ].x -= xOff;
    drawOff[ doa ].y -= yOff;
}

void Bild::releaseDrawOptions() // setzt die Drawoptionen zurück
{
    --doa;
}

bool Bild::getRend()
{
    bool ret = rend;
    rend = 0;
    return ret;
}

// constant 
int *Bild::getBuffer()const // gibt buffer zurück
{
    return fc;
}

int Bild::getPixel( int x, int y ) const
{
    if( x < 0 || y < 0 || x >= größe.x || y >= größe.y )
        return 0;
    return fc[ x + y * größe.x ];
}

const Punkt &Bild::getGröße() const // gibt die Größe zurück
{
    return größe;
}

int Bild::getBreite() const // gibt die Breite zurück
{
    return größe.x;
}

int Bild::getHöhe() const // gibt die Höhe zurück
{
    return größe.y;
}

unsigned char Bild::getAlpha() const // gibt den Alpha wert zurück
{
    return 255 - alpha[ alphaAnzahl ];
}

const Punkt &Bild::getDrawPos() const
{
    return dPosA[ doa ];
}

const Punkt &Bild::getDrawGr() const
{
    return dGrößeA[ doa ];
}

const Punkt &Bild::getDrawOff() const
{
    return drawOff[ doa ];
}

// Reference Counting 
Bild *Bild::getThis()
{
    ++ref;
    return this;
}

Bild *Bild::release()
{
    --ref;
    if( ref < 1 )
        delete this;
    return 0;
}


#ifdef WIN32
// Inhalt der BildZ Klasse aus Bild.h
// Konstruktor 
BildZ::BildZ()
    : ZeichnungHintergrund(),
    bild( 0 ),
    ref( 1 )
{
    style = 0;
    Mak = _ret1ME;
}

// Destruktor 
BildZ::~BildZ()
{
    if( bild )
        bild->release();
}

// nicht constant 
void BildZ::setBildZ( Bild *b ) // setzt das Bild
{
    if( bild )
        bild->release();
    bild = b;
    if( !vertikalScrollBar )
        vertikalScrollBar = new VScrollBar();
    if( !horizontalScrollBar )
        horizontalScrollBar = new HScrollBar();
    horizontalScrollBar->getScrollData()->max = b->getBreite();
    vertikalScrollBar->getScrollData()->max = b->getHöhe();
    rend = 1;
}

void BildZ::setBild( Bild *b )
{
    if( !bild )
        bild = new Bild();
    bild->neuBild( b->getBreite(), b->getHöhe(), 0 );
    bild->drawBild( 0, 0, b->getBreite(), b->getHöhe(), *b );
    if( !vertikalScrollBar )
        vertikalScrollBar = new VScrollBar();
    if( !horizontalScrollBar )
        horizontalScrollBar = new HScrollBar();
    horizontalScrollBar->getScrollData()->max = b->getBreite();
    vertikalScrollBar->getScrollData()->max = b->getHöhe();
    b->release();
    rend = 1;
}

bool BildZ::tick( double tickVal ) // tick
{
    return __super::tick( tickVal );
}

void BildZ::doMausEreignis( MausEreignis &me ) // ruft Mak auf
{
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) )
    {
        if( mausIn )
        {
            mausIn = 0;
            MausEreignis me2;
            me2.id = ME_Verlässt;
            me2.mx = me.mx;
            me2.my = me.my;
            me2.verarbeitet = 0;
            doMausEreignis( me2 );
            return;
        }
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Verlässt )
    {
        return;
    }
    if( !mausIn && me.id != ME_Verlässt )
    {
        mausIn = 1;
        MausEreignis me2;
        me2.id = ME_Betritt;
        me2.mx = me.mx;
        me2.my = me.my;
        me2.verarbeitet = 0;
        doMausEreignis( me2 );
    }
    me.mx -= pos.x;
    me.my -= pos.y;
    if( hatStyle( Style::Sichtbar ) )
    {
        if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
        {
            if( me.id != ME_Betritt && me.id != ME_Verlässt )
            {
                lockZeichnung();
                int rbr = 0;
                if( hatStyle( Style::Rahmen ) && rahmen )
                    rbr = rahmen->getRBreite();
                bool vs = hatStyle( Style::VScroll ) && vertikalScrollBar;
                bool hs = hatStyle( Style::HScroll ) && horizontalScrollBar;
                if( vs )
                {
                    if( hs )
                        horizontalScrollBar->doMausMessage( rbr, gr.y - rbr - 15, gr.x - rbr * 2 - 15, 15, me );
                    vertikalScrollBar->doMausMessage( gr.x - rbr - 15, rbr, 15, gr.y - rbr * 2, me );
                }
                else if( hs )
                    horizontalScrollBar->doMausMessage( rbr, gr.y - rbr - 15, gr.x - rbr * 2, 15, me );
                unlockZeichnung();
                if( vs || hs )
                    me.verarbeitet = 1;
            }
        }
    }
    me.mx += pos.x;
    me.my += pos.y;
}

void BildZ::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( hatStyle( Style::Sichtbar ) )
    {
        __super::render( zRObj );
        lockZeichnung();
        if( !zRObj.setDrawOptions( innenPosition, innenGröße ) )
        {
            unlockZeichnung();
            return;
        }
        if( bild )
        {
            int x = 0;
            int y = 0;
            int br = innenGröße.x;
            int hö = innenGröße.y;
            if( !( vertikalScrollBar && hatStyle( Style::VScroll ) ) && !( horizontalScrollBar && hatStyle( Style::HScroll ) ) )
            {
                if( hatStyle( Style::Alpha ) )
                    zRObj.alphaBild( x, y, br, hö, *bild );
                else
                    zRObj.drawBild( x, y, br, hö, *bild );
            }
            else
            {
                if( !zRObj.setDrawOptions( x, y, br, hö ) )
                {
                    zRObj.releaseDrawOptions();
                    unlockZeichnung();
                    return;
                }
                if( hatStyle( Style::Alpha ) )
                    zRObj.alphaBild( -horizontalScrollBar->getScroll(), -vertikalScrollBar->getScroll(), bild->getBreite(), bild->getHöhe(), *bild );
                else
                    zRObj.drawBild( -horizontalScrollBar->getScroll(), -vertikalScrollBar->getScroll(), bild->getBreite(), bild->getHöhe(), *bild );
                zRObj.releaseDrawOptions();
            }
        }
        zRObj.releaseDrawOptions();
        unlockZeichnung();
    }
}

// constant 
Bild *BildZ::getBild() const // gibt das Bild zurück
{
    if( bild )
        return bild->getThis();
    return 0;
}

Bild *BildZ::zBild() const
{
    return bild;
}

Zeichnung *BildZ::dublizieren() const // erstellt eine Kopie des Zeichnungs
{
    BildZ *obj = new BildZ();
    obj->setPosition( pos );
    obj->setGröße( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( Mak );
    obj->setTastaturEreignis( Tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    obj->setStyle( style );
    obj->setHintergrundFarbe( hintergrundFarbe );
    if( hintergrundFeld )
        obj->setAlphaFeldZ( (AlphaFeld*)hintergrundFeld->dublizieren() );
    if( rahmen )
        obj->setLinienRahmenZ( (LRahmen*)rahmen->dublizieren() );
    if( hintergrundBild )
        obj->setHintergrundBild( hintergrundBild->getThis() );
    if( bild )
        obj->setBild( bild->getThis() );
    obj->setStyle( style );
    return obj;
}

// Reference Counting 
BildZ *BildZ::getThis()
{
    ++ref;
    return this;
}

BildZ *BildZ::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}


Bild *Framework::ladeBild( char *pfad, Text *zError )
{
    Text p = pfad;
    Text *txt = p.getTeilText( p.positionVon( '.', p.anzahlVon( '.' ) - 1 ) );
    if( !( txt->istGleich( ".bmp" ) || txt->istGleich( ".jpg" ) || txt->istGleich( ".gif" ) || txt->istGleich( ".png" ) ) )
    {
        zError->setText( "Die Angegebene Datei ist keine gueltige Bilddatei!" );
        txt->release();
        return 0;
    }
    txt->release();
    wchar_t *name = new wchar_t[ p.getLänge() + 1 ];
    for( int i = 0; i < p.getLänge(); i++ )
        name[ i ] = (wchar_t)p.getText()[ i ];
    name[ p.getLänge() ] = '\0';

    Gdiplus::Bitmap bitmap( name );
    Gdiplus::Color pix;
    delete[]name;

    Bild *ret = new Bild();
    ret->neuBild( bitmap.GetWidth(), bitmap.GetHeight(), 0 );
    int *buff = ret->getBuffer();
    for( unsigned int i = 0; i < bitmap.GetWidth() * bitmap.GetHeight(); i++ )
    {
        bitmap.GetPixel( i % bitmap.GetWidth(), i / bitmap.GetWidth(), &pix );
        buff[ i ] = pix.GetValue();
    }
    return ret;
}

#endif