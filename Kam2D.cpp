#include "Kam2D.h"
#include "Bild.h"
#include "Welt2D.h"
#include "MausEreignis.h"
#include "ToolTip.h"
#include "TastaturEreignis.h"
#include "Globals.h"

using namespace Framework;

// Inhalt der Kam2D Klasse aus Kam2D.h
// Konstruktor
Kam2D::Kam2D()
{
    welt = 0;
    wGr.x = 0;
    wGr.y = 0;
    maxWGr.x = 0;
    maxWGr.y = 0;
    wPos.x = 0;
    wPos.y = 0;
    animations = new Array< KamAnimation* >();
    ref = 1;
}

// Destruktor
Kam2D::~Kam2D()
{
    if( welt )
        welt->release();
    animations->release();
}

// nicht constant
void Kam2D::setWelt( Welt2D *welt )
{
    if( this->welt )
        this->welt->release();
    this->welt = welt;
}

void Kam2D::setMaxWeltGröße( int br, int hö )
{
    maxWGr.x = br;
    maxWGr.y = hö;
}

void Kam2D::setWeltGröße( int x, int y, int sek )
{
    animations->add( new KamAnimation{ WeltGröße, (double)x, (double)y, (double)sek } );
}

void Kam2D::scrollIn( int scroll, int sek )
{
    animations->add( new KamAnimation{ ScrollIn, (double)scroll, 0, (double)sek } );
}

void Kam2D::scrollOut( int scroll, int sek )
{
    animations->add( new KamAnimation{ ScrollOut, (double)scroll, 0, (double)sek } );
}

void Kam2D::moveKam( int x, int y, int sek )
{
    animations->add( new KamAnimation{ MoveKam, (double)x, (double)y, (double)sek } );
}

void Kam2D::finishAnimation()
{
    while( animations->getEintragAnzahl() > 0 )
    {
        KamAnimation *a = animations->get( 0 );
        switch( a->typ )
        {
        case WeltGröße:
            wGr.x = a->w1;
            wGr.y = a->w2;
            break;
        case ScrollIn:
        {
            double tmp = wGr.x;
            wGr.x -= a->w1;
            wGr.y -= ( wGr.y * a->w1 ) / tmp;
            break;
        }
        case ScrollOut:
        {
            double tmp = wGr.x;
            wGr.x += a->w1;
            wGr.y += ( wGr.y * a->w1 ) / tmp;
            break;
        }
        case MoveKam:
            wPos.x = a->w1;
            wPos.y = a->w1;
            break;
        }
        animations->lösche( 0 );
        delete a;
    }
}

void Kam2D::clearAnimation()
{
    while( animations->getEintragAnzahl() > 0 )
    {
        delete animations->get( 0 );
        animations->lösche( 0 );
    }
}

bool Kam2D::tick( double t )
{
    int num = 0;
    for( auto i = animations->getArray(); i.set; i++ )
    {
        double time = t;
        switch( i.var->typ )
        {
        case WeltGröße:
            if( i.var->sek < t )
                time = i.var->sek;
            wGr.x += ( i.var->w1 - wGr.x ) * time;
            wGr.y = ( i.var->w2 - wGr.y ) * time;
            break;
        case ScrollIn:
        {
            double tmp = wGr.x;
            wGr.x -= i.var->w1 * time;
            wGr.y -= ( wGr.y * i.var->w1 * time ) / tmp;
            break;
        }
        case ScrollOut:
        {
            double tmp = wGr.x;
            wGr.x += i.var->w1 * time;
            wGr.y += ( wGr.y * i.var->w1 * time ) / tmp;
            break;
        }
        case MoveKam:
            wPos.x += ( i.var->w1 - wPos.x ) * time;
            wPos.y += ( i.var->w1 - wPos.y ) * time;
            break;
        }
        i.var->sek -= time;
        if( i.var->sek == 0 )
        {
            delete i.var;
            animations->lösche( num );
            num--;
        }
        num++;
        rend = 1;
    }
    if( hatStyle( Style::MAUS_MOVE ) )
    {
        if( mausPos.x > 0 && mausPos.y < 30 )
            wPos.x -= 50 * t;
        if( mausPos.x > gr.x - 30 && mausPos.x < gr.x )
            wPos.x += 50 * t;
        if( mausPos.y > 0 && mausPos.y < 30 )
            wPos.y -= 50 * t;
        if( mausPos.y > gr.y - 30 && mausPos.y < gr.y )
            wPos.y += 50 * t;
    }
    if( hatStyle( Style::TASTATUR_MOVE ) )
    {
        if( getTastenStand( T_Links ) )
            wPos.x -= 50 * t;
        if( getTastenStand( T_Rechts ) )
            wPos.x += 50 * t;
        if( getTastenStand( T_Oben ) )
            wPos.y -= 50 * t;
        if( getTastenStand( T_Unten ) )
            wPos.y += 50 * t;
    }
    if( welt && hatStyle( Style::WELT_TICK ) )
        rend |= welt->tick( t );
    else
        rend |= welt->tick( 0 );
    return __super::tick( t );
}

void Kam2D::doMausEreignis( MausEreignis &me )
{
    if( me.verarbeitet || ( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Verlässt ) )
    {
        if( mausIn )
        {
            mausIn = 0;
            if( toolTip )
                toolTip->setMausIn( 0 );
            MausEreignis me2;
            me2.id = ME_Verlässt;
            me2.mx = me.mx;
            me2.my = me.my;
            me2.verarbeitet = 0;
            doMausEreignis( me2 );
        }
        return;
    }
    if( !mausIn && me.id != ME_Verlässt )
    {
        mausIn = 1;
        if( toolTip )
            toolTip->setMausIn( 1 );
        MausEreignis me2;
        me2.id = ME_Betritt;
        me2.mx = me.mx;
        me2.my = me.my;
        me2.verarbeitet = 0;
        doMausEreignis( me2 );
    }
    me.mx -= pos.x, me.my -= pos.y;
    if( Mak && Mak( makParam, this, me ) )
    {
        if( welt && hatStyle( Style::WELT_EREIGNISSE ) )
        {
            int tmpx = me.mx;
            int tmpy = me.my;
            me.mx = (int)( wPos.x + me.mx * ( wGr.x / gr.x ) );
            me.my = (int)( wPos.y + me.my * ( wGr.y / gr.y ) );
            welt->doMausEreignis( me );
            me.mx = tmpx;
            me.my = tmpy;
        }
        if( hatStyle( Style::MAUS_MOVE ) )
        {
            mausPos.x = me.mx;
            mausPos.y = me.my;
        }
        me.verarbeitet = 1;
    }
    if( nMak && me.verarbeitet )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
}

void Kam2D::doTastaturEreignis( TastaturEreignis &te )
{
    if( te.verarbeitet )
        return;
    if( Tak && Tak( takParam, this, te ) )
    {
        if( welt && hatStyle( Style::WELT_EREIGNISSE ) )
            welt->doTastaturEreignis( te );
        te.verarbeitet = 1;
    }
    if( nTak && te.verarbeitet )
        te.verarbeitet = nTak( ntakParam, this, te );
}

void Kam2D::render( Bild &rObj )
{
    if( !hatStyle( Style::Sichtbar ) )
        return;
    löscheStyle( Style::HScroll | Style::VScroll );
    __super::render( rObj );
    if( !rObj.setDrawOptions( innenPosition, innenGröße ) )
        return;
    if( welt )
        welt->render( rObj, (Punkt)wPos, (Punkt)wGr, innenGröße );
    rObj.releaseDrawOptions();
}

// constant
Welt2D *Kam2D::zWelt() const
{
    return welt;
}

Welt2D *Kam2D::getWelt() const
{
    return welt ? welt->getThis() : 0;
}

double Kam2D::getWeltX() const
{
    return wPos.x;
}

double Kam2D::getWeltY() const
{
    return wPos.y;
}

double Kam2D::getWeltBr() const
{
    return wGr.x;
}

double Kam2D::getWeltHö() const
{
    return wGr.y;
}

// Reference Counting
Kam2D *Kam2D::getThis()
{
    ref++;
    return this;
}

Kam2D *Kam2D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}