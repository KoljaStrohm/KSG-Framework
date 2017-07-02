#include "Animation3D.h"
#include "Model3D.h"

using namespace Framework;

// löscht eine KnochenData Struktur
void Animation3D::deleteKnochenData( KnochenData *d )
{
    if( d->next != 0 )
        deleteKnochenData( d->next );
    delete d;
}

// Inhalt der Animation3D Klasse aus Animation3D.h
// Konstruktor
Animation3D::Animation3D()
{
    for( int i = 0; i < MAX_KNOCHEN_ANZ; i++ )
        kd[ i ] = 0;
    maxTime = 0;
    ref = 1;
}

// Destructor
Animation3D::~Animation3D()
{
    for( int i = 0; i < MAX_KNOCHEN_ANZ; i++ )
        deleteKnochenData( kd[ i ] );
}

// Fügt für einen bestimmten Knochen ein Keyframe hinzu
//  kId: Die Id des Knochens
//  time: Die Zeit, die vergehen soll bis der Knochen die Position und die Rotation des Keyframes follständig erreicht hat
//  pos: Die Position des Knochens die erreicht werden soll in Modellkoordinaten
//  rot: Die Rotation des Knochens um seine Position die erreicht werden soll
void Animation3D::addKeyFrame( int kId, double time, Vec3<float> pos, Vec3<float> rot )
{
    KnochenData *d = kd[ kId ];
    if( !d )
    {
        kd[ kId ] = new KnochenData();
        kd[ kId ]->next = 0;
        kd[ kId ]->time = time;
        kd[ kId ]->pos = pos;
        kd[ kId ]->rot = rot;
        return;
    }
    while( d->next )
        d = d->next;
    d->next = new KnochenData();
    d->next->next = 0;
    d->next->time = time;
    d->next->pos = pos;
    d->next->rot = rot;    
}

// Wendet die Animation auf ein bestimmtes Skelett an
// zS: Das Skelett
// timeOffset: zeit in sekunden, die diese Animation bereits auf dem Skelett angewendet wurde. Wird automatisch von der Animation aktualisiert.
// sec: zeit in Sekunden, die vergangen ist seit dem diese Methode zuletzt für das Skelett aufgerufen wurde
void Animation3D::apply( Skelett *zS, double &timeOffset, double sec ) const
{
    if( zS->k )
        apply( zS->k, timeOffset, sec );
    timeOffset += sec;
    if( timeOffset > maxTime )
        timeOffset -= maxTime;
}

// Wendet die Animation auf ein bestimmten Knochen an
// zS: Der Knochen
// timeOffset: zeit in sekunden, die diese Animation bereits auf dem Knochen angewendet wurde.
// sec: zeit in Sekunden, die vergangen ist seit dem diese Methode zuletzt für dem Knochen aufgerufen wurde
void Animation3D::apply( Knochen *zK, double timeOffset, double sec ) const
{
    if( zK->geschwister )
        apply( zK->geschwister, timeOffset, sec );
    if( zK->kinder )
        apply( zK->kinder, timeOffset, sec );
    KnochenData *d = kd[ zK->id ];
    if( d )
    {
        while( timeOffset > d->time )
        {
            timeOffset -= d->time;
            d = d->next;
            if( !d )
                d = kd[ zK->id ];
        }
        while( sec > 0 )
        {
            double left = d->time - timeOffset;
            if( left > 0 )
            {
                float proc = (float)MIN( sec / left, 1 );
                zK->pos += ( d->pos - zK->pos ) * proc;
                zK->winkel += ( d->rot - zK->winkel ) * proc;
                sec -= left;
            }
            else
            {
                zK->pos = d->pos;
                zK->winkel = d->rot;
                sec -= d->time;
            }
            timeOffset = 0;
            d = d->next;
            if( !d )
                d = kd[ zK->id ];
        }
    }
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Animation3D *Animation3D::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Animation3D *Animation3D::release()
{
    if( --ref == 0 )
        delete this;
    return 0;
}