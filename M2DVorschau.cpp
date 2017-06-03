#include "M2DVorschau.h"
#include "Model2D.h"
#include "MausEreignis.h"
#include "Rahmen.h"
#include "AlphaFeld.h"
#include "ToolTip.h"
#include "Globals.h"
#include "Bild.h"


using namespace Framework;


// Inhalt der M2DVorschau Klasse aus M2DVorschau.h
// Konstruktor
M2DVorschau::M2DVorschau()
    : ZeichnungHintergrund()
{
    mdl = 0;
    mx = -1;
    my = -1;
    af = 0;
    ram = 0;
    ref = 1;
}

// Destruktor
M2DVorschau::~M2DVorschau()
{
    if( mdl )
        mdl->release();
    if( ram )
        ram->release();
    if( af )
        af->release();
}

// nicht constant
void M2DVorschau::setModel2DZ( Model2D *mdl )
{
    if( this->mdl )
        this->mdl->release();
    this->mdl = mdl;
    rend = 1;
}

void M2DVorschau::setModel2D( Model2DData *mdl )
{
    if( !this->mdl )
        this->mdl = new Model2D();
    this->mdl->setModel( mdl );
    rend = 1;
}

void M2DVorschau::doMausEreignis( MausEreignis &me )
{
    if( hatStyleNicht( Style::Erlaubt ) || hatStyleNicht( Style::Sichtbar ) )
    {
        mausIn = 0;
        if( toolTip )
            toolTip->setMausIn( 0 );
        return;
    }
    if( me.verarbeitet || ( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Leaves ) )
    {
        if( mausIn )
        {
            mausIn = 0;
            if( toolTip )
                toolTip->setMausIn( 0 );
            MausEreignis me2;
            me2.id = ME_Leaves;
            me2.mx = me.mx;
            me2.my = me.my;
            me2.verarbeitet = 0;
            doMausEreignis( me2 );
        }
        return;
    }
    if( !mausIn && me.id != ME_Leaves )
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
        if( hatStyle( Style::UsrScale ) )
        {
            if( mdl && me.id == ME_UScroll )
                mdl->addSize( 0.01f );
            if( mdl && me.id == ME_DScroll )
                mdl->addSize( -0.01f );
        }
        if( me.id == ME_RLinks || me.id == ME_RRechts || me.id == ME_Leaves )
        {
            mx = -1;
            my = -1;
        }
        if( mdl && me.id == ME_Bewegung )
        {
            if( mx != -1 && my != -1 )
            {
                if( getMausStand( M_Links ) && hatStyle( Style::UsrMove ) )
                    mdl->setPosition( mdl->getPosition() + Punkt( me.mx - mx, me.my - my ) );
                if( getMausStand( M_Rechts ) && hatStyle( Style::UsrRot ) )
                {
                    if( me.mx > gr.x / 2 )
                        mdl->addDrehung( 0.01f * (float)( me.my - my ) );
                    else
                        mdl->addDrehung( -0.01f * (float)( me.my - my ) );
                    if( me.my > gr.y / 2 )
                        mdl->addDrehung( -0.01f * (float)( me.mx - mx ) );
                    else
                        mdl->addDrehung( 0.01f * (float)( me.mx - mx ) );
                }
                mx = me.mx;
                my = me.my;
            }
        }
        if( me.id == ME_PLinks || me.id == ME_PRechts )
        {
            mx = me.mx;
            my = me.my;
        }
        if( nMak && me.verarbeitet )
            me.verarbeitet = nMak( nmakParam, this, me );
    }
    me.mx += pos.x, me.my += pos.y;
}

bool M2DVorschau::tick( double tv )
{
    rend |= mdl ? mdl->tick( tv ) : 0;
    rend |= af ? af->tick( tv ) : 0;
    rend |= ram ? ram->tick( tv ) : 0;
    return ZeichnungHintergrund::tick( tv );
}

void M2DVorschau::render( Bild &rb )
{
    removeStyle( Style::VScroll | Style::HScroll );
    if( hatStyleNicht( Style::Sichtbar ) )
        return;
	ZeichnungHintergrund::render( rb );
    if( !rb.setDrawOptions( innenPosition, innenSize ) )
        return;
    if( mdl )
    {
        int rbr = rahmen && hatStyle( Style::Rahmen ) ? rahmen->getRBreite() : 0;
        rb.addScrollOffset( -gr.x / 2 + rbr, -gr.y / 2 + rbr );
        mdl->render( rb );
    }
    rb.releaseDrawOptions();
}

// constant
Model2D *M2DVorschau::zModel() const
{
    return mdl;
}

Model2D *M2DVorschau::getModel() const
{
    return mdl ? mdl->getThis() : 0;
}

Zeichnung *M2DVorschau::dublizieren() const
{
    M2DVorschau *ret = new M2DVorschau();
    if( mdl )
        ret->setModel2D( mdl->getModel() );
    if( rahmen )
        ret->setLinienRahmenZ( (LRahmen*)rahmen->dublizieren() );
    if( hintergrundFeld )
        ret->setAlphaFeldZ( (AlphaFeld*)hintergrundFeld->dublizieren() );
    ret->setHintergrundFarbe( bgF );
    return ret;
}

// Reference counting
M2DVorschau *M2DVorschau::getThis()
{
    ref++;
    return this;
}

M2DVorschau *M2DVorschau::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}