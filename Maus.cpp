#include "Maus.h"
#include "Bild.h"
#include "Punkt.h"

using namespace Framework;

// Inhalt der Maus Klasse aus Maus.h
// Konstruktor 
Maus::Maus()
    : hMaus( LoadCursor( 0, IDC_ARROW ) ),
    ref( 1 )
{}

// nicht constant
void Maus::ladeMaus( int mausId )
{
    if( mausId == MausId::nichts )
        hMaus = 0;
    if( mausId == MausId::normal )
        hMaus = LoadCursor( 0, IDC_ARROW );
    if( mausId == MausId::hand )
        hMaus = LoadCursor( 0, IDC_HAND );
    if( mausId == MausId::warten )
        hMaus = LoadCursor( 0, IDC_APPSTARTING );
    if( mausId == MausId::verschieben )
        hMaus = LoadCursor( 0, IDC_SIZEALL );
    if( mausId == MausId::text )
        hMaus = LoadCursor( 0, IDC_IBEAM );
    if( mausId == MausId::wahgerecht )
        hMaus = LoadCursor( 0, IDC_SIZEWE );
    if( mausId == MausId::senkrecht )
        hMaus = LoadCursor( 0, IDC_SIZENS );
    if( mausId == MausId::diagonal1 )
        hMaus = LoadCursor( 0, IDC_SIZENWSE );
    if( mausId == MausId::diagonal2 )
        hMaus = LoadCursor( 0, IDC_SIZENESW );
    if( mausId == MausId::verboten )
        hMaus = LoadCursor( 0, IDC_NO );
    SetCursor( hMaus );
}

void Maus::ladeMaus( Bild *maus )
{
    HBITMAP hAndMaskBitmap;
    HBITMAP hXorMaskBitmap;
    HDC hDC = GetDC( 0 );
    HDC hAndMaskDC = CreateCompatibleDC( hDC );
    HDC hXorMaskDC = CreateCompatibleDC( hDC );

    hAndMaskBitmap = CreateCompatibleBitmap( hDC, maus->getBreite(), maus->getHöhe() );
    hXorMaskBitmap = CreateCompatibleBitmap( hDC, maus->getBreite(), maus->getHöhe() );

    //Select the bitmaps to DC
    HBITMAP hOldAndMaskBitmap = (HBITMAP)SelectObject( hAndMaskDC, hAndMaskBitmap );
    HBITMAP hOldXorMaskBitmap = (HBITMAP)SelectObject( hXorMaskDC, hXorMaskBitmap );

    //Scan each pixel of the souce bitmap and create the masks
    int y;
    for( int x = 0; x < maus->getBreite(); ++x )
    {
        for( y = 0; y < maus->getHöhe(); ++y )
        {
            int pixel = maus->getPixel( x, y );
            if( ( ( pixel >> 24 ) & 0xFF ) == 0 )
            {
                SetPixel( hAndMaskDC, x, y, RGB( 255, 255, 255 ) );
                SetPixel( hXorMaskDC, x, y, RGB( 0, 0, 0 ) );
            }
            else
            {
                SetPixel( hAndMaskDC, x, y, RGB( 0, 0, 0 ) );
                SetPixel( hXorMaskDC, x, y, RGB( ( pixel >> 16 ) & 0xFF, ( pixel >> 8 ) & 0xFF, pixel & 0xFF ) );
            }
        }
    }
    SelectObject( hAndMaskDC, hOldAndMaskBitmap );
    SelectObject( hXorMaskDC, hOldXorMaskBitmap );

    DeleteDC( hXorMaskDC );
    DeleteDC( hAndMaskDC );
    ReleaseDC( 0, hDC );

    ICONINFO iconinfo = { 0 };
    iconinfo.fIcon = 0;
    iconinfo.xHotspot = 0;
    iconinfo.yHotspot = 0;
    iconinfo.hbmMask = hAndMaskBitmap;
    iconinfo.hbmColor = hXorMaskBitmap;
    hMaus = CreateIconIndirect( &iconinfo );

    SetCursor( hMaus );
}

void Maus::update()
{
    SetCursor( hMaus );
}

// constant
HCURSOR Maus::getMausHandle()
{
    return hMaus;
}

// Reference Counting
Maus *Maus::getThis()
{
    ++ref;
    return this;
}

Maus *Maus::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}