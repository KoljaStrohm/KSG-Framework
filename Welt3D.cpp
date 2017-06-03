#include "Welt3D.h"
#include "Zeichnung3D.h"
#include "MausEreignis.h"
#ifdef WIN32
#include "Render3D.h"
#endif

using namespace Framework;

// Inhalt der Welt3D Klasse aus Welt3D.h
// Konstructor
Welt3D::Welt3D()
{
    arraySize = 100;
    arraySizeAlpha = 100;
    members = new Zeichnung3D*[ arraySize ];
    membersAlpha = new Zeichnung3D*[ arraySizeAlpha ];
    distSq = new float[ arraySizeAlpha + arraySize ];
    distSqSort = new float[ arraySizeAlpha + arraySize ];
    alphaVS = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
    elementsSort = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
    for( int i = 0; i < arraySize; i++ )
        members[ i ] = 0;
    for( int i = 0; i < arraySizeAlpha; i++ )
        membersAlpha[ i ] = 0;
    ref = 1;
    rend = 0;
    upd = 1;
}

// Destruktor
Welt3D::~Welt3D()
{
    delete[] members;
    delete[] membersAlpha;
    delete[] distSq;
    delete[] distSqSort;
    delete[] alphaVS;
    delete[] elementsSort;
}

// Fügt der Welt ein Objekt hinzu
//  obj: Das Objekt, was hinzugefügt werden soll
void Welt3D::addZeichnung( Zeichnung3D *obj )
{
    cs.lock();
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
            cs.unlock();
            return;
        }
        tmp++;
    }
    rend = 1;
    if( obj->hatAlpha() )
    {
        arraySizeAlpha += 100;
        Zeichnung3D **nm = new Zeichnung3D*[ arraySizeAlpha ];
        memcpy( nm, membersAlpha, sizeof( Zeichnung3D * ) * ( arraySizeAlpha - 100 ) );
        memset( &nm[ arraySizeAlpha - 100 ], 0, sizeof( Zeichnung3D * ) * 100 );
        delete[] membersAlpha;
        membersAlpha = nm;
        membersAlpha[ arraySizeAlpha - 100 ] = obj;
        delete[] distSq;
        delete[] distSqSort;
        delete[] alphaVS;
        delete[] elementsSort;
        distSq = new float[ arraySizeAlpha + arraySize ];
        distSqSort= new float[ arraySizeAlpha + arraySize ];
        alphaVS = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
        elementsSort = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
        cs.unlock();
        return;
    }
    arraySize += 100;
    Zeichnung3D **nm = new Zeichnung3D*[ arraySize ];
    memcpy( nm, members, sizeof( Zeichnung3D * ) * ( arraySize - 100 ) );
    memset( &nm[ arraySize - 100 ], 0, sizeof( Zeichnung3D * ) * 100 );
    delete[] members;
    members = nm;
    members[ arraySize - 100 ] = obj;
    delete[] distSq;
    delete[] distSqSort;
    delete[] alphaVS;
    delete[] elementsSort;
    distSq = new float[ arraySizeAlpha + arraySize ];
    distSqSort = new float[ arraySizeAlpha + arraySize ];
    alphaVS = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
    elementsSort = new Zeichnung3D*[ arraySizeAlpha + arraySize ];
    cs.unlock();
}

// Entfernt ein Objekt aus der Welt
//  obj: Das Objekt, das entwernt werden soll
void Welt3D::removeZeichnung( Zeichnung3D *obj )
{
    cs.lock();
    int index = 0;
    if( !obj->hatAlpha() )
    {
        for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
        {
            if( *i == obj )
            {
                *i = 0;
                rend = 1;
                cs.unlock();
                return;
            }
        }
        cs.unlock();
        return;
    }
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        if( *i == obj )
        {
            *i = 0;
            rend = 1;
            cs.unlock();
            return;
        }
    }
    cs.unlock();
}

// Verarbeitet ein Mausereignis
//  me: Das Mausereignis, das verarbeitet werden soll
void Welt3D::doMausEreignis( MausEreignis3D &me )
{
    //cs.lock()
    //int anz = 0;
    //int index = 0;
    //for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
    //{
    //    if( *i )
    //    {
    //        distSq[ anz ] = me.pos.abstandSq( ( *i )->getPos() );
    //        alphaVS[ anz ] = *i;
    //        anz++;
    //    }
    //}
    //index = 0;
    //for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    //{
    //    if( *i )
    //    {
    //        distSq[ anz ] = me.pos.abstandSq( ( *i )->getPos() );
    //        alphaVS[ anz ] = *i;
    //        anz++;
    //    }
    //}
    //float maxEntf;
    //int ind;
    //do
    //{
    //    maxEntf = -1;
    //    ind = -1;
    //    for( int i = 0; i < anz; i++ )
    //    {
    //        if( !used[ i ] && distSq[ i ] > maxEntf )
    //        {
    //            maxEntf = distSq[ i ];
    //            ind = i;
    //        }
    //    }
    //    if( ind >= 0 )
    //    {
    //        alphaVS[ ind ]->doMausEreignis( me );
    //        if( me.verarbeitet )
    //        {
    //            cs.unlock();
    //            return;
    //        }
    //        used[ ind ] = 1;
    //    }
    //} while( ind >= 0 );
    //cs.unlock();
}

// Verarbeitet die vergangene Zeit
//  tickval: Die zeit in sekunden, die seit dem letzten Aufruf der Funktion vergangen ist
//  return: true, wenn sich das Objekt verändert hat, false sonnst.
bool Welt3D::tick( double tickval )
{
    if( !upd )
        return rend;
    rend = 0;
    upd = 0;
    int index = 0;
    cs.lock();
    for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
    {
        if( *i && ( *i )->hatAlpha() )
        {
            addZeichnung( *i );
            *i = 0;
            continue;
        }
        rend |= *i ? ( *i )->tick( tickval ) : 0;
    }
    index = 0;
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        rend |= *i ? ( *i )->tick( tickval ) : 0;
        if( *i && !( *i )->hatAlpha() )
        {
            addZeichnung( *i );
            *i = 0;
            continue;
        }
    }
    cs.unlock();
    return rend;
}

// Zeichnet einen ausschnitt der Welt
//  zRObj: Enthällt alle Werkzeuge, die zum Zeichnen verwendet werden
void Welt3D::render( Render3D *zRObj )
{
#ifdef WIN32
    upd = 1;
    cs.lock();
    int index = 0;
    for( Zeichnung3D **i = members; index < arraySize; i++, index++ )
    {
        if( *i && zRObj->isInFrustrum( ( *i )->getPos(), ( *i )->getRadius() ) )
            ( *i )->render( zRObj );
    }
    index = 0;
    int index2 = 0;
    for( Zeichnung3D **i = membersAlpha; index < arraySizeAlpha; i++, index++ )
    {
        if( *i && zRObj->isInFrustrum( ( *i )->getPos(), ( *i )->getRadius(), &distSq[ index2 ] ) )
        {
            alphaVS[ index2 ] = *i;
            elementsSort[ index2 ] = *i;
            distSqSort[ index2 ] = distSq[ index2 ];
            index2++;
        }
    }
    int K;
    int L = 1;
    while( L < index2 )
    {
        K = 0;
        while( K + 2 * L - 1 < index2 )
        {
            //merge
            int I = K;
            int J = K + L;
            int N = K;
            while( I < K + L || J < K + 2 * L )
            {
                if( J == K + 2 * L || ( I < K + L && distSq[ I ] < distSq[ J ] ) )
                {
                    distSqSort[ N ] = distSq[ I ];
                    elementsSort[ N ] = alphaVS[ I ];
                    I++;
                }
                else
                {
                    distSqSort[ N ] = distSq[ J ];
                    elementsSort[ N ] = alphaVS[ J ];
                    J++;
                }
                N++;
            }
            K += 2 * L;
        }
        if( K + L - 1 < index2 - 1 )
        {
            //merge
            int I = K;
            int J = K + L;
            int N = K;
            while( I < K + L || J < index2 - 1 )
            {
                if( J == index2 || ( I < K + L && distSq[ I ] < distSq[ J ] ) )
                {
                    distSqSort[ N ] = distSqSort[ I ];
                    elementsSort[ N ] = alphaVS[ I ];
                    I++;
                }
                else
                {
                    distSqSort[ N ] = distSq[ J ];
                    elementsSort[ N ] = alphaVS[ J ];
                    J++;
                }
                N++;
            }
        }
        float *tmpF = distSq;
        distSq = distSqSort;
        distSqSort = tmpF;
        Zeichnung3D **tmpZ = alphaVS;
        alphaVS = elementsSort;
        elementsSort = tmpZ;
        L *= 2;
    }
    for( int i = index2 - 1; i >= 0; i-- )
        alphaVS[ i ]->render( zRObj );
    cs.unlock();
#endif
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
