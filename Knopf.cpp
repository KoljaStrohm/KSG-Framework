#include "Knopf.h"
#include "MausEreignis.h"
#include "TastaturEreignis.h"
#include "Bild.h"
#include "AlphaFeld.h"
#include "Text.h"
#include "Schrift.h"
#include "Rahmen.h"
#include "Scroll.h"
#include "Datei.h"
#include "DateiSystem.h"
#include "Globals.h"
#include "ToolTip.h"

using namespace Framework;

// Inhalt der Knopf Klasse aus Knopf.h
// Konstruktor 
Knopf::Knopf()
    : TextFeld(),
    klickFarbe( 0xFF000000 ),
    klickBild( 0 ),
    klickBuffer( 0 ),
    klickIndex( 0 ),
    ref( 1 )
{
    style = Style::Erlaubt | Style::Buffered | Style::KlickBuffer | Style::Rahmen;
    this->setMausEreignis( _ret1ME );
    setSchriftFarbe( 0xFFFFFFFF );
    setSchriftSize( 12 );
    setLinienRahmenBreite( 2 );
    setLinienRahmenFarbe( 0xFF00FF00 );
    setAlphaFeldFarbe( 0x5500FF00 );
    setAlphaFeldStrength( -5 );
    setKBFarbe( 0xFF000000 );
    setKBStrength( 20 );
    addStyle( TextFeld::Style::Center | TextFeld::Style::Mehrfarbig );
}

// Destruktor 
Knopf::~Knopf()
{
    if( klickBuffer )
        klickBuffer->release();
    if( klickBild )
        klickBild->release();
}

// nicht constant 
void Knopf::setKlickFarbe( int fc ) // setzt die Klick Farbe
{
    klickFarbe = fc;
    rend = 1;
}

void Knopf::setKlickBild( Bild *bild ) // setzt das Klick Bild
{
    if( bild )
    {
        if( !klickBild )
            klickBild = new Bild();
        klickBild->neuBild( bild->getBreite(), bild->getHeight(), 0 );
        int *buff1 = klickBild->getBuffer();
        int *buff2 = bild->getBuffer();
        for( int i = 0; i < bild->getBreite() * bild->getHeight(); ++i )
            buff1[ i ] = buff2[ i ];
        bild->release();
        rend = 1;
    }
}

void Knopf::setKlickBildZ( Bild *bild ) // setzt einen Zeiger zum Klick Bild
{
    if( klickBild )
        klickBild->release();
    klickBild = bild;
    rend = 1;
}

void Knopf::setKBZ( AlphaFeld *af )
{
    if( klickBuffer )
        klickBuffer->release();
    klickBuffer = af;
    rend = 1;
}

void Knopf::setKBStrength( int st ) // setzt die Stärke des Klick Buffers
{
    if( !klickBuffer )
        klickBuffer = new AlphaFeld();
    klickBuffer->setStrength( st );
    rend = 1;
}

void Knopf::setKBFarbe( int fc ) // setzt die Farbe des Klick Buffers
{
    if( !klickBuffer )
        klickBuffer = new AlphaFeld();
    klickBuffer->setFarbe( fc );
    rend = 1;
}

void Knopf::doMausEreignis( MausEreignis &me ) // Maus Ereignis
{
    bool nmakc = !me.verarbeitet;
    if( hatStyleNicht( Style::Erlaubt ) || hatStyleNicht( Style::Sichtbar ) )
    {
        mausIn = 0;
        if( toolTip )
            toolTip->setMausIn( 0 );
        return;
    }
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) )
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
            return;
        }
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Leaves )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
        return;
    }
    if( !mausIn && me.id != ME_Leaves && !me.verarbeitet )
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
    ++ref;
    me.mx -= pos.x, me.my -= pos.y;
    if( mak && ( me.verarbeitet || mak( makParam, this, me ) ) )
    {
        if( me.id == ME_Betritt )
        {
            klickIndex = MausStand[ M_Links ];
            if( klickIndex )
                rend = 1;
        }
        if( me.id == ME_RLinks || me.id == ME_Leaves )
        {
            if( klickIndex )
                rend = 1;
            klickIndex = 0;
        }
        if( !me.verarbeitet )
        {
            if( me.id == ME_PLinks )
            {
                klickIndex = 1;
                rend = 1;
            }
            me.verarbeitet = 1;
        }
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
    release();
}

void Knopf::render( Bild &zRObj ) // zeichenet nach zRObj
{
    if( !hatStyle( Style::Sichtbar ) )
        return;
    addStyle( TextFeld::Style::Center );
    removeStyle( Style::VScroll | Style::HScroll );
    bool rA = 0;
    if( !hatStyle( Style::Erlaubt ) )
    {
        zRObj.setAlpha( 0x20 );
        rA = 1;
    }
	TextFeld::render( zRObj );
    if( !zRObj.setDrawOptions( innenPosition, innenSize ) )
    {
        if( rA )
            zRObj.releaseAlpha();
        return;
    }
    if( hatStyle( Style::Sichtbar ) && klickIndex == 1 )
    {
        if( hatStyle( Style::KlickFarbe ) )
        {
            if( hatStyle( Style::KlickAlpha ) )
                zRObj.alphaRegion( 0, 0, innenSize.x, innenSize.y, klickFarbe );
            else
                zRObj.fillRegion( 0, 0, innenSize.x, innenSize.y, klickFarbe );
        }
        if( hatStyle( Style::KlickBild ) && klickBild )
        {
            if( hatStyle( Style::KlickAlpha ) )
                zRObj.alphaBild( 0, 0, innenSize.x, innenSize.y, *klickBild );
            else
                zRObj.drawBild( 0, 0, innenSize.x, innenSize.y, *klickBild );
        }
        if( hatStyle( Style::KlickBuffer ) && klickBuffer )
        {
            klickBuffer->setSize( innenSize.x, innenSize.y );
            klickBuffer->render( zRObj );
        }
    }
    if( rA )
        zRObj.releaseAlpha();
    zRObj.releaseDrawOptions();
}

// constant 
int Knopf::getKlickFarbe() const // gibt getThis der Klick Farbe zurück
{
    return klickFarbe;
}

Bild *Knopf::getKlickBild() const // gibt getThis des Klick Bildes zurück
{
    if( !klickBild )
        return 0;
    return klickBild->getThis();
}

Bild *Knopf::zKlickBild() const // gibt einen Zeiger zum Klick Bild zurück
{
    return klickBild;
}

AlphaFeld *Knopf::getKB() const // gibt getThis des Klick Buffers zurück
{
    if( !klickBuffer )
        return 0;
    return klickBuffer->getThis();
}

AlphaFeld *Knopf::zKB() const // gibt den Klick Buffer zurück
{
    return klickBuffer;
}

int Knopf::getKBFarbe() const // gibt getThis der Farbe des Klick Buffers zurück
{
    if( !klickBuffer )
        return 0;
    return klickBuffer->getFarbe();
}

int Knopf::getKBStrength() const // gibt die Stärke des Klickbuffers zurück
{
    if( !klickBuffer )
        return 0;
    return klickBuffer->getStrength();
}

Zeichnung *Knopf::dublizieren() const // Erzeugt eine Kopie des Zeichnungs
{
    Knopf *obj = new Knopf();
    obj->setPosition( pos );
    obj->setSize( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( mak );
    obj->setTastaturEreignis( tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    obj->setStyle( style );
    obj->setSchriftSize( getSchriftSize() );
    if( zSchrift() )
        obj->setSchriftZ( getSchrift() );
    if( zText() )
        obj->setText( zText()->getText() );
    obj->setHintergrundFarbe( hintergrundFarbe );
    obj->setSchriftFarbe( getSchriftFarbe() );
    if( hintergrundFeld )
        obj->setAlphaFeldZ( (AlphaFeld*)hintergrundFeld->dublizieren() );
    if( rahmen )
        obj->setLinienRahmenZ( (LRahmen*)rahmen->dublizieren() );
    if( hintergrundBild )
        obj->setHintergrundBild( hintergrundBild->getThis() );
    if( vertikalScrollBar )
    {
        obj->setVertikalKlickScroll( vertikalScrollBar->getKlickScroll() );
        obj->setVertikalScrollPos( vertikalScrollBar->getScroll() );
        obj->setVertikalScrollFarbe( vertikalScrollBar->getFarbe(), vertikalScrollBar->getBgFarbe() );
    }
    if( horizontalScrollBar )
    {
        obj->setHorizontalKlickScroll( horizontalScrollBar->getKlickScroll() );
        obj->setHorizontalScrollPos( horizontalScrollBar->getScroll() );
        obj->setHorizontalScrollFarbe( horizontalScrollBar->getFarbe(), horizontalScrollBar->getBgFarbe() );
    }
    obj->setSchowChar( getShowChar() );
    obj->setAuswahl( getSelectionPos(), getCursorPos() );
    obj->setKlickFarbe( klickFarbe );
    if( klickBild )
        obj->setKlickBild( klickBild->getThis() );
    if( klickBuffer )
        obj->setKBZ( (AlphaFeld*)klickBuffer->dublizieren() );
    return obj;
}

// Reference Counting 
Knopf *Knopf::getThis()
{
    ++ref;
    return this;
}

Knopf *Knopf::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der KontrollKnopf Klasse aus Knopf.h
// Konstruktor 
KontrollKnopf::KontrollKnopf()
    : ZeichnungHintergrund(),
    txt( 0 ),
    sTxt( 0 ),
    sBgF( 0 ),
    kBgF( 0 ),
    sBgB( 0 ),
    kBgB( 0 ),
    sAf( 0 ),
    kAf( 0 ),
    kasten( 0 ),
    sKasten( 0 ),
    schrift( 0 ),
    sF( 0xFFFFFFFF ),
    sGr( 12 ),
    ref( 1 )
{
    style = Style::Erlaubt | Style::KlickBuffer;
    setKAFFarbe( 0x00007500 );
    setKAFStrength( -30 );
}

// Destruktor 
KontrollKnopf::~KontrollKnopf()
{
    if( txt )
        txt->release();
    if( sTxt )
        sTxt->release();
    if( sBgB )
        sBgB->release();
    if( kBgB )
        kBgB->release();
    if( sAf )
        sAf->release();
    if( kAf )
        kAf->release();
    if( kasten )
        kasten->release();
    if( sKasten )
        sKasten->release();
    if( schrift )
        schrift->release();
}

// nicht constant 
void KontrollKnopf::setTextZ( Text *txt ) // setzt den Text
{
    if( this->txt )
        this->txt->release();
    this->txt = txt;
    rend = 1;
}

void KontrollKnopf::setText( Text *txt )
{
    if( !this->txt )
        this->txt = new Text();
    this->txt->setText( txt->getText() );
    txt->release();
    rend = 1;
}

void KontrollKnopf::setText( const char *txt )
{
    if( !this->txt )
        this->txt = new Text();
    this->txt->setText( txt );
    rend = 1;
}

void KontrollKnopf::setSTextZ( Text *txt ) // setzt den Text bei Selectiert
{
    if( sTxt )
        sTxt->release();
    sTxt = txt;
    rend = 1;
}

void KontrollKnopf::setSText( Text *txt )
{
    if( !sTxt )
        sTxt = new Text();
    sTxt->setText( txt->getText() );
    txt->release();
    rend = 1;
}

void KontrollKnopf::setSText( const char *txt )
{
    if( !sTxt )
        sTxt = new Text();
    sTxt->setText( txt );
    rend = 1;
}

void KontrollKnopf::setSchriftZ( Schrift *schrift ) // setzt die Schrift
{
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    rend = 1;
}

void KontrollKnopf::setSFarbe( int f ) // setzt die Schrift Farbe
{
    sF = f;
    rend = 1;
}

void KontrollKnopf::setSSize( int gr ) // setzt die Schrift Größe
{
    sGr = gr;
    rend = 1;
}

void KontrollKnopf::setSBgFarbe( int f ) // setzt die Select Hintergrundfarbe
{
    sBgF = f;
    rend = 1;
}

void KontrollKnopf::setKBgFarbe( int f ) // setzt die Klick Hintergrundfarbe
{
    kBgF = f;
    rend = 1;
}

void KontrollKnopf::setSBgBildZ( Bild *b ) // setzt das Select Hintergrundbild
{
    if( sBgB )
        sBgB->release();
    sBgB = b;
    rend = 1;
}

void KontrollKnopf::setSBgBild( Bild *b )
{
    if( !sBgB )
        sBgB = new Bild();
    sBgB->neuBild( b->getBreite(), b->getHeight(), 0 );
    sBgB->drawBild( 0, 0, b->getBreite(), b->getHeight(), *b );
    b->release();
    rend = 1;
}

void KontrollKnopf::setKBgBildZ( Bild *b ) // setzt das Klick Hintergrundbild
{
    if( kBgB )
        kBgB->release();
    kBgB = b;
    rend = 1;
}

void KontrollKnopf::setKBgBild( Bild *b )
{
    if( !kBgB )
        kBgB = new Bild();
    kBgB->neuBild( b->getBreite(), b->getHeight(), 0 );
    kBgB->drawBild( 0, 0, b->getBreite(), b->getHeight(), *b );
    b->release();
    rend = 1;
}

void KontrollKnopf::setSAlphaFeldZ( AlphaFeld *af ) // setzt das Select Alpha Feld
{
    if( sAf )
        sAf->release();
    sAf = af;
    rend = 1;
}

void KontrollKnopf::setSAFFarbe( int f ) // setzt die Select Alphafeld Farbe
{
    if( !sAf )
        sAf = new AlphaFeld();
    sAf->setFarbe( f );
    rend = 1;
}

void KontrollKnopf::setSAFStrength( int st ) // setzt die Select AlphaFeld Stärke
{
    if( !sAf )
        sAf = new AlphaFeld();
    sAf->setStrength( st );
    rend = 1;
}

void KontrollKnopf::setKAlphaFeldZ( AlphaFeld *af ) // setzt das Klick Alpha Feld
{
    if( kAf )
        kAf->release();
    kAf = af;
    rend = 1;
}

void KontrollKnopf::setKAFFarbe( int f ) // setzt die Klick Alphafeld Farbe
{
    if( !kAf )
        kAf = new AlphaFeld();
    kAf->setFarbe( f );
    rend = 1;
}

void KontrollKnopf::setKAFStrength( int st ) // setzt die Klick AlphaFeld Stärke
{
    if( !kAf )
        kAf = new AlphaFeld();
    kAf->setStrength( st );
    rend = 1;
}

void KontrollKnopf::loadData( LTDBDatei *dat ) // läht die Systembilder
{
    if( DateiExistiert( new Text( "data/normal.ltdb" ) ) && ( !dat || !dat->istOffen() ) )
    {
        LTDBDatei *dat = new LTDBDatei();
        dat->setDatei( new Text( "data/normal.ltdb" ) );
        dat->leseDaten( 0 );
        kasten = dat->laden( 0, new Text( "kasten.gif" ) );
        sKasten = dat->laden( 0, new Text( "skasten.gif" ) );
        dat->release();
    }
    else
    {
        kasten = dat->laden( 0, new Text( "kasten.gif" ) );
        sKasten = dat->laden( 0, new Text( "skasten.gif" ) );
    }
    rend = 1;
}

void KontrollKnopf::loadData( const char *ltdb ) // läht die Systembilder
{
    if( DateiExistiert( new Text( ltdb ) ) )
    {
        LTDBDatei *dat = new LTDBDatei();
        dat->setDatei( new Text( ltdb ) );
        dat->leseDaten( 0 );
        kasten = dat->laden( 0, new Text( "kasten.gif" ) );
        sKasten = dat->laden( 0, new Text( "skasten.gif" ) );
        dat->release();
        rend = 1;
    }
}

void KontrollKnopf::doMausEreignis( MausEreignis &me ) // Nachrichten verarbeitung
{
    bool nmakc = !me.verarbeitet;
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) )
    {
        style &= ~Style::MausKlick;
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
            return;
        }
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Leaves )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
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
    if( hatStyleNicht( TextFeld::Style::Erlaubt ) || hatStyleNicht( TextFeld::Style::Sichtbar ) )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
        return;
    }
    me.mx -= pos.x;
    me.my -= pos.y;
    if( mak && ( me.verarbeitet || mak( makParam, this, me ) ) )
    {
        if( me.id == ME_Betritt && hatStyle( Style::MausKlick ) != MausStand[ M_Links ] )
            setStyle( Style::MausKlick, MausStand[ M_Links ] );
        if( !me.verarbeitet && me.id == ME_PLinks )
            addStyle( Style::MausKlick );
        if( me.id == ME_RLinks )
        {
            removeStyle( Style::MausKlick );
            setStyle( Style::Selected, !hatStyle( Style::Selected ) );
        }
        me.verarbeitet = 1;
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x;
    me.my += pos.y;
}

void KontrollKnopf::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( !hatStyle( Style::Sichtbar ) )
        return;
    removeStyle( Style::VScroll | Style::HScroll );
	ZeichnungHintergrund::render( zRObj );
    lockZeichnung();
    if( !zRObj.setDrawOptions( innenPosition, innenSize ) )
    {
        unlockZeichnung();
        return;
    }
    if( hatStyle( Style::MausKlick ) )
    {
        if( hatStyle( Style::KlickFarbe ) )
        {
            if( hatStyle( Style::KlickAlpha ) )
                zRObj.alphaRegion( 0, 0, innenSize.x, innenSize.y, kBgF );
            else
                zRObj.fillRegion( 0, 0, innenSize.x, innenSize.y, kBgF );
        }
        if( hatStyle( Style::KlickBild ) && kBgB )
        {
            if( hatStyle( Style::KlickAlpha ) )
                zRObj.alphaBild( 0, 0, innenSize.x, innenSize.y, *kBgB );
            else
                zRObj.drawBild( 0, 0, innenSize.x, innenSize.y, *kBgB );
        }
        if( hatStyle( Style::KlickBuffer ) && kAf )
        {
            kAf->setSize( innenSize.x, innenSize.y );
            kAf->render( zRObj );
        }
        int kbr = 0;
        if( hatStyle( Style::Selected ) && sKasten )
        {
            if( sTxt && schrift )
                zRObj.drawBild( 0, ( gr.y / 2 - sKasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - sKasten->getHeight() / 2 ), innenSize.x, innenSize.y, *sKasten );
            else
                zRObj.drawBild( gr.x / 2 - sKasten->getBreite() / 2, ( gr.y / 2 - sKasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - sKasten->getHeight() / 2 ), innenSize.x, innenSize.y, *sKasten );
            kbr = sKasten->getBreite();
            if( sTxt && schrift )
            {
                schrift->setSchriftSize( sGr );
                schrift->setDrawPosition( kbr + 5, gr.y / 2 - schrift->getTextHeight( sTxt ) / 2 );
                schrift->renderText( sTxt, zRObj, sF );
            }
        }
        else if( kasten )
        {
            if( txt && schrift )
                zRObj.drawBild( 0, ( gr.y / 2 - kasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - kasten->getHeight() / 2 ), innenSize.x, innenSize.y, *kasten );
            else
                zRObj.drawBild( gr.x / 2 - kasten->getBreite() / 2, ( gr.y / 2 - kasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - kasten->getHeight() / 2 ), innenSize.x, innenSize.y, *kasten );
            kbr = kasten->getBreite();
            if( txt && schrift )
            {
                schrift->setSchriftSize( sGr );
                schrift->setDrawPosition( kbr + 5, gr.y / 2 - schrift->getTextHeight( txt ) / 2 );
                schrift->renderText( txt, zRObj, sF );
            }
        }
    }
    else if( hatStyle( Style::Selected ) )
    {
        if( hatStyle( Style::SelectFarbe ) )
        {
            if( hatStyle( Style::SelectAlpha ) )
                zRObj.alphaRegion( 0, 0, innenSize.x, innenSize.y, sBgF );
            else
                zRObj.fillRegion( 0, 0, innenSize.x, innenSize.y, sBgF );
        }
        if( hatStyle( Style::SelectBild ) && sBgB )
        {
            if( hatStyle( Style::SelectAlpha ) )
                zRObj.alphaBild( 0, 0, innenSize.x, innenSize.y, *sBgB );
            else
                zRObj.drawBild( 0, 0, innenSize.x, innenSize.y, *sBgB );
        }
        if( hatStyle( Style::SelectBuffer ) && sAf )
        {
            sAf->setSize( innenSize.x, innenSize.y );
            sAf->render( zRObj );
        }
        int kbr = 0;
        if( sKasten )
        {
            if( sTxt && schrift )
                zRObj.drawBild( 0, ( gr.y / 2 - sKasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - sKasten->getHeight() / 2 ), innenSize.x, innenSize.y, *sKasten );
            else
                zRObj.drawBild( gr.x / 2 - sKasten->getBreite() / 2, ( gr.y / 2 - sKasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - sKasten->getHeight() / 2 ), innenSize.x, innenSize.y, *sKasten );
            kbr = sKasten->getBreite();
        }
        if( sTxt && schrift )
        {
            schrift->setSchriftSize( sGr );
            schrift->setDrawPosition( kbr + 5, gr.y / 2 - schrift->getTextHeight( sTxt ) / 2 );
            schrift->renderText( sTxt, zRObj, sF );
        }
    }
    else
    {
        int kbr = 0;
        if( kasten )
        {
            if( txt && schrift )
                zRObj.drawBild( 0, ( gr.y / 2 - kasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - kasten->getHeight() / 2 ), innenSize.x, innenSize.y, *kasten );
            else
                zRObj.drawBild( gr.x / 2 - kasten->getBreite() / 2, ( gr.y / 2 - kasten->getHeight() / 2 ) < 0 ? 0 : ( gr.y / 2 - kasten->getHeight() / 2 ), innenSize.x, innenSize.y, *kasten );
            kbr = kasten->getBreite();
        }
        if( txt && schrift )
        {
            schrift->setSchriftSize( sGr );
            schrift->setDrawPosition( kbr + 5, gr.y / 2 - schrift->getTextHeight( txt ) / 2 );
            schrift->renderText( txt, zRObj, sF );
        }
    }
    zRObj.releaseDrawOptions();
    unlockZeichnung();
}

// constant 
Text *KontrollKnopf::getText() const // gibt den Text zurück
{
    return txt ? txt->getThis() : 0;
}

Text *KontrollKnopf::zText() const
{
    return txt;
}

Text *KontrollKnopf::getSText() const // gibt den Select Text zurück
{
    return sTxt ? sTxt->getThis() : 0;
}

Text *KontrollKnopf::zSText() const
{
    return sTxt;
}

Schrift *KontrollKnopf::getSchrift() const // gibt die Schrift zurück
{
    return schrift ? schrift->getThis() : 0;
}

Schrift *KontrollKnopf::zSchrift() const
{
    return schrift;
}

int KontrollKnopf::getSFarbe() const // gibt die Schrift Farbe zurück
{
    return sF;
}

int KontrollKnopf::getSSize() const // gibt die Schrift Größe zurück
{
    return sGr;
}

int KontrollKnopf::getSBgFarbe() const// gibt die Select Hintergrundfarbe zurück
{
    return sBgF;
}

int KontrollKnopf::getKBgFarbe() const // gibt die Klick Hintergrundfarbe zurück
{
    return kBgF;
}

Bild *KontrollKnopf::getSBgBild() const // gibt das Select Hintergrundbild zurück
{
    return sBgB ? sBgB->getThis() : 0;
}

Bild *KontrollKnopf::zSBgBild() const
{
    return sBgB;
}

Bild *KontrollKnopf::getKBgBild() const // gibt das Klick Hintergrundbild zurück
{
    return kBgB ? kBgB->getThis() : 0;
}

Bild *KontrollKnopf::zKBgBild() const
{
    return kBgB;
}

AlphaFeld *KontrollKnopf::getSAlphaFeld() const // gibt das Select AlphaFeld zurück
{
    return sAf ? sAf->getThis() : 0;
}

AlphaFeld *KontrollKnopf::zSAlphaFeld() const
{
    return sAf;
}

AlphaFeld *KontrollKnopf::getKAlphaFeld() const // gibt das Klick AlphaFeld zurück
{
    if( !kAf )
        return 0;
    return kAf->release();
}

AlphaFeld *KontrollKnopf::zKAlphaFeld() const
{
    return kAf;
}

// Reference Counting 
KontrollKnopf *KontrollKnopf::getThis()
{
    ++ref;
    return this;
}

KontrollKnopf *KontrollKnopf::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}