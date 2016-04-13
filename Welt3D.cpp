#include "Welt3D.h"
#include "Zeichnung3D.h"
#include "Render3D.h"

using namespace Framework;

// Inhalt der Welt3D Klasse aus Welt3D.h
// Konstructor
Welt3D::Welt3D()
{
    arraySize = 100;
    arraySizeAlpha = 100;
    members = new Zeichnung3D*[ arraySize ];
    membersAlpha = new Zeichnung3D*[ arraySizeAlpha ];
    used = new bool[ arraySizeAlpha ];
    distSq = new float[ arraySizeAlpha ];
    alphaVS = new Zeichnung3D*[ arraySizeAlpha ];
    for( int i = 0; i < arraySize; i++ )
        members[ i ] = 0;
    for( int i = 0; i < arraySizeAlpha; i++ )
        membersAlpha[ i ] = 0;
    ref = 1;
}

// Destruktor
Welt3D::~Welt3D()
{
    delete[] members;
    delete[] membersAlpha;
    delete[] used;
    delete[] distSq;
    delete[] alphaVS;
}

// Fügt der Welt ein Objekt hinzu
//  obj: Das Objekt, was hinzugefügt werden soll
void Welt3D::addZeichnung( Zeichnung3D *obj )
{
    Zeichnung3D **tmp = members;
    int max = arraySize;
    if( obj->hatAlpha() )
    {
        tmp = membersAlpha;
        max = arraySizeAlpha;
    }
    for( int i = 0; i < max; i++ )
    {
        if( !*tmp )
        {
            *tmp = obj;
            return;
        }
        tmp++;
    }
    if( obj->hatAlpha() )
    {
        arraySizeAlpha += 100;
        Zeichnung3D **nm = new Zeichnung3D*[ arraySizeAlpha ];
        memcpy( nm, membersAlpha, sizeof( Zeichnung3D * ) * ( arraySizeAlpha - 100 ) );
        memset( &nm[ arraySizeAlpha - 100 ], 0, sizeof( Zeichnung3D * ) * 100 );
        delete[] membersAlpha;
        membersAlpha = nm;
        membersAlpha[ arraySizeAlpha - 100 ] = obj;
        delete[] used;
        delete[] distSq;
        delete[] alphaVS;
        used = new bool[ arraySizeAlpha ];
        distSq = new float[ arraySizeAlpha ];
        alphaVS = new Zeichnung3D*[ arraySizeAlpha ];
        return;
    }
    arraySize += 100;
    Zeichnung3D **nm = new Zeichnung3D*[ arraySize ];
    memcpy( nm, members, sizeof( Zeichnung3D * ) * ( arraySize - 100 ) );
    memset( &nm[ arraySize - 100 ], 0, sizeof( Zeichnung3D * ) * 100 );
    delete[] members;
    members = nm;
    members[ arraySize - 100 ] = obj;
}

// Entfernt ein Objekt aus der Welt
//  obj: Das Objekt, das entwernt werden soll
void Welt3D::removeZeichnung( Zeichnung3D *obj )
{
    int index = 0;
    if( !obj->hatAlpha() )
    {
        for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
        {
            if( *i == obj )
            {
                *i = 0;
                return;
            }
        }
        return;
    }
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        if( *i == obj )
        {
            *i = 0;
            return;
        }
    }
}
// Verarbeitet die vergangene Zeit
//  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
//  return: true, wenn sich das Objekt verändert hat, false sonnst.
bool Welt3D::tick( double tickval )
{
    int index = 0;
    bool ret = 0;
    for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
    {
        if( *i && ( *i )->hatAlpha() )
        {
            addZeichnung( *i );
            *i = 0;
            continue;
        }
        ret |= *i ? ( *i )->tick( tickval ) : 0;
    }
    index = 0;
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        ret |= *i ? ( *i )->tick( tickval ) : 0;
        if( *i && !( *i )->hatAlpha() )
        {
            addZeichnung( *i );
            *i = 0;
            continue;
        }
    }
    return ret;
}

// Zeichnet einen ausschnitt der Welt
//  zRObj: Enthällt alle Werkzeuge, die zum Zeichnen verwendet werden
void Welt3D::render( Render3D *zRObj )
{
    int index = 0;
    for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
    {
        if( *i && zRObj->isInFrustrum( (*i)->getPos(), (*i)->getRadius() ) )
            ( *i )->render( zRObj );
    }
    memset( used, 0, arraySizeAlpha * sizeof( bool ) );
    memset( alphaVS, 0, arraySizeAlpha * sizeof( Zeichnung3D * ) );
    index = 0;
    int index2 = 0;
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        if( *i && zRObj->isInFrustrum( ( *i )->getPos(), ( *i )->getRadius(), &distSq[ index2 ] ) )
        {
            alphaVS[ index2 ] = *i;
            index2++;
        }
    }
    float maxEntf;
    int ind;
    do
    {
        maxEntf = -1;
        ind = -1;
        for( int i = 0; i < index2; i++ )
        {
            if( !used[ i ] && distSq[ i ] > maxEntf )
            {
                maxEntf = distSq[ i ];
                ind = i;
            }
        }
        if( ind >= 0 )
        {
            alphaVS[ ind ]->render( zRObj );
            used[ ind ] = 1;
        }
    }
    while( ind >= 0 );
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Welt3D *Welt3D::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Welt3D *Welt3D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}
