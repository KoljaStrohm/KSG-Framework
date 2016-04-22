#include "Liste.h"
#include "Array.h"
#include "Rahmen.h"
#include "Bild.h"
#include "AlphaFeld.h"
#include "TextFeld.h"
#include "Text.h"
#include "MausEreignis.h"
#include "TastaturEreignis.h"
#include "Scroll.h"
#include "Globals.h"
#include "Schrift.h"

using namespace Framework;

// Inhalt der AuswahlListe Klasse aus Liste.h
// Konstruktor 
AuswahlListe::AuswahlListe()
    : ZeichnungHintergrund(),
    tfListe( 0 ),
    einträge( 0 ),
    auswahl( -1 ),
    ahFarbe( 0xFF000000 ),
    ahBild( 0 ),
    aBuffer( 0 ),
    aRahmen( 0 ),
    styles( 0 ),
    ahFarbeListe( 0 ),
    ahBildListe( 0 ),
    aBufferListe( 0 ),
    aRahmenListe( 0 ),
    schrift( 0 ),
    ref( 1 )
{
    style = 0;
    this->setMausEreignis( _ret1ME );
    this->setTastaturEreignis( _ret1TE );
    ref = 1;
}

// Destruktor 
AuswahlListe::~AuswahlListe()
{
    if( tfListe )
        tfListe->release();
    if( ahBild )
        ahBild->release();
    if( aBuffer )
        aBuffer->release();
    if( aRahmen )
        aRahmen->release();
    if( styles )
        styles->release();
    if( ahFarbeListe )
        ahFarbeListe->release();
    if( ahBildListe )
        ahBildListe->release();
    if( aBufferListe )
        aBufferListe->release();
    if( aRahmenListe )
        aRahmenListe->release();
    if( schrift )
        schrift->release();
}

// nicht constant 
void AuswahlListe::update() // aktualisiert die Auswahl Liste
{
    int rbr = 0;
    if( rahmen )
    {
        rbr = rahmen->getRBreite();
        rahmen->setPosition( 0, 0 );
        rahmen->setGröße( gr.x, gr.y );
    }
    if( hintergrundFeld )
    {
        hintergrundFeld->setPosition( rbr, rbr );
        hintergrundFeld->setGröße( gr.x - rbr * 2, gr.y - rbr * 2 );
    }
    if( hatStyleNicht( Style::MultiStyled ) && tfListe )
    {
        bool FeldRahmen = hatStyle( Style::FeldRahmen );
        bool FeldHintergrund = hatStyle( Style::FeldHintergrund );
        bool FeldHBild = hatStyle( Style::FeldHBild );
        bool FeldHAlpha = hatStyle( Style::FeldHAlpha );
        bool FeldBuffer = hatStyle( Style::FeldBuffer );
        for( int i = 0; i < einträge; ++i )
        {
            TextFeld *tf = tfListe->z( i );
            tf->setStyle( TextFeld::Style::Rahmen, FeldRahmen );
            tf->setStyle( TextFeld::Style::Hintergrund, FeldHintergrund );
            tf->setStyle( TextFeld::Style::HBild, FeldHBild );
            tf->setStyle( TextFeld::Style::HAlpha, FeldHAlpha );
            tf->setStyle( TextFeld::Style::Buffered, FeldBuffer );
            if( schrift )
                tf->setSchriftZ( schrift->getThis() );
        }
    }
    if( hatStyle( Style::MultiStyled ) && tfListe && styles )
    {
        for( int i = 0; i < einträge; ++i )
        {
            TextFeld *tf = tfListe->z( i );
            tf->setStyle( TextFeld::Style::Rahmen, hatMsStyle( i, Style::FeldRahmen ) );
            tf->setStyle( TextFeld::Style::Hintergrund, hatMsStyle( i, Style::FeldHintergrund ) );
            tf->setStyle( TextFeld::Style::HBild, hatMsStyle( i, Style::FeldHBild ) );
            tf->setStyle( TextFeld::Style::HAlpha, hatMsStyle( i, Style::FeldHAlpha ) );
            tf->setStyle( TextFeld::Style::Buffered, hatMsStyle( i, Style::FeldBuffer ) );
        }
    }
    rend = 1;
}

void AuswahlListe::addEintrag( Text *txt ) // fügt einen Eintrag hinzu
{
    TextFeld *tf = new TextFeld();
    tf->setStyle( TextFeld::Style::Center | TextFeld::Style::Sichtbar | TextFeld::Style::Mehrfarbig | TextFeld::Style::Rahmen );
    tf->setSchriftFarbe( 0xFFFFFFFF );
    tf->setLinienRahmenBreite( 1 );
    tf->setLinienRahmenFarbe( 0xFFFFFFFF );
    tf->setTextZ( txt );
    tf->setGröße( 0, 20 );
    addEintrag( tf );
    rend = 1;
}

void AuswahlListe::addEintrag( const char *txt )
{
    Text *tx = new Text( txt );
    addEintrag( tx );
    rend = 1;
}

void AuswahlListe::addEintragZ( TextFeld *tf )
{
    if( !tfListe )
        tfListe = new RCArray< TextFeld >();
    if( schrift && ( !tf->zSchrift() || hatStyleNicht( Style::MultiStyled ) ) )
        tf->setSchriftZ( schrift->getThis() );
    tfListe->set( tf, einträge );
    ++einträge;
    rend = 1;
}

void AuswahlListe::addEintrag( int pos, Text *txt ) // fügt einen Eintrag bei position pos ein
{
    TextFeld *tf = new TextFeld();
    tf->setStyle( TextFeld::Style::Center | TextFeld::Style::Sichtbar | TextFeld::Style::Mehrfarbig | TextFeld::Style::Rahmen );
    tf->setSchriftFarbe( 0xFFFFFFFF );
    tf->setLinienRahmenBreite( 1 );
    tf->setLinienRahmenFarbe( 0xFFFFFFFF );
    tf->setTextZ( txt );
    tf->setGröße( 0, 20 );
    addEintrag( pos, tf );
    rend = 1;
}

void AuswahlListe::addEintrag( int pos, const char *txt )
{
    Text *tx = new Text( txt );
    addEintrag( pos, tx );
    rend = 1;
}

void AuswahlListe::addEintragZ( int pos, TextFeld *tf )
{
    if( !tfListe )
        tfListe = new RCArray< TextFeld >();
    if( schrift && ( !tf->zSchrift() || hatStyleNicht( Style::MultiStyled ) ) )
        tf->setSchriftZ( schrift->getThis() );
    tfListe->add( tf, pos );
    ++einträge;
    rend = 1;
}

void AuswahlListe::setEintrag( int pos, Text *txt ) // ändert den pos - ten Eintrag
{
    TextFeld *tf = 0;
    if( tfListe )
        tf = tfListe->z( pos );
    if( !tf )
    {
        tf = new TextFeld();
        tf->setStyle( TextFeld::Style::Center | TextFeld::Style::Sichtbar | TextFeld::Style::Mehrfarbig | TextFeld::Style::Rahmen );
        tf->setSchriftFarbe( 0xFFFFFFFF );
        tf->setLinienRahmenFarbe( 0xFFFFFFFF );
        tf->setLinienRahmenBreite( 1 );
        tf->setTextZ( txt );
        tf->setGröße( 0, 20 );
        setEintrag( pos, tf );
        rend = 1;
        return;
    }
    tf->setTextZ( txt );
    rend = 1;
}

void AuswahlListe::setEintrag( int pos, unsigned char *txt )
{
    Text *tx = new Text( (const char*)txt );
    setEintrag( pos, tx );
    rend = 1;
}

void AuswahlListe::setEintragZ( int pos, TextFeld *tf )
{
    if( !tfListe )
        tfListe = new RCArray< TextFeld >();
    if( schrift && ( !tf->zSchrift() || hatStyleNicht( Style::MultiStyled ) ) )
        tf->setSchriftZ( schrift->getThis() );
    tfListe->set( tf, pos );
    rend = 1;
}

void AuswahlListe::tauschEintragPos( int vpos, int npos ) // taucht den Eintrag vpos mit dem Eintrag npos
{
    if( tfListe )
    {
        tfListe->tausch( vpos, npos );
        if( styles )
            styles->tausch( vpos, npos );
        if( ahFarbeListe )
            ahFarbeListe->tausch( vpos, npos );
        if( ahBildListe )
            ahBildListe->tausch( vpos, npos );
        if( aBufferListe )
            aBufferListe->tausch( vpos, npos );
        if( aRahmenListe )
            aRahmenListe->tausch( vpos, npos );
        rend = 1;
    }
}

void AuswahlListe::löscheEintrag( int pos ) // löscht den Eintrag pos
{
    tfListe->lösche( pos );
    --einträge;
    rend = 1;
}

void AuswahlListe::setSchriftZ( Schrift *schrift ) // legt die Schrift der Einträge fest
{
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    rend = 1;
}

void AuswahlListe::setVScrollZuEintrag( int eintrag ) // scrollt zum Eintrag
{
    if( vertikalScrollBar )
    {
        if( eintrag < einträge )
            eintrag = einträge - 1;
        int y = 0;
        for( int i = 0; i < eintrag; i++ )
            y += tfListe->z( i ) ? tfListe->z( i )->getHöhe() : 0;
        vertikalScrollBar->scroll( y );
    }
}

void AuswahlListe::updateVScroll() // scrollt zur Curser Position oder nach Unten
{
    if( vertikalScrollBar )
    {
        int y = 0;
        for( int i = 0; i < einträge; i++ )
            y += tfListe->z( i ) ? tfListe->z( i )->getHöhe() : 0;
        vertikalScrollBar->update( y, gr.y - ( ( rahmen && hatStyle( TextFeld::Style::Rahmen ) ) ? rahmen->getRBreite() : 0 ) );
    }
}

void AuswahlListe::setALRZ( LRahmen *rahmen ) // setzt einen Zeiger zum Auswahl Rahmen (nur ohne MulitStyled)
{
    if( aRahmen )
        aRahmen->release();
    aRahmen = rahmen;
    rend = 1;
}

void AuswahlListe::setALRBreite( int br ) // setzt die Breite des Auswahl Rahmens (nur ohne MultiStyled)
{
    if( !aRahmen )
        aRahmen = new LRahmen();
    aRahmen->setRamenBreite( br );
    rend = 1;
}

void AuswahlListe::setALRFarbe( int fc ) // setzt die Farbe des Auswahl Rahmens (nur ohne MultiStyled)
{
    if( !aRahmen )
        aRahmen = new LRahmen();
    aRahmen->setFarbe( fc );
    rend = 1;
}

void AuswahlListe::setAAFZ( AlphaFeld *buffer ) // setzt einen Zeiger zum Auswahl AlpaFeld (nur ohne MultiStyled)
{
    if( aBuffer )
        aBuffer->release();
    aBuffer = buffer;
    rend = 1;
}

void AuswahlListe::setAAFStärke( int st ) // setzt die Stärke des Auswahl Hintergrund Buffers (nur ohne MultiStyled)
{
    if( !aBuffer )
        aBuffer = new AlphaFeld();
    aBuffer->setStärke( st );
    rend = 1;
}

void AuswahlListe::setAAFFarbe( int fc ) // setzt die Farbe des Auswahl Hintergrund Buffers (nur ohne MultiStyled)
{
    if( !aBuffer )
        aBuffer = new AlphaFeld();
    aBuffer->setFarbe( fc );
    rend = 1;
}

void AuswahlListe::setAHBild( Bild *bild ) // setzt das Auswahl Hintergrund Bild (nur ohne MultiStyled)
{
    if( !ahBild )
        ahBild = new Bild();
    ahBild->neuBild( bild->getBreite(), bild->getHöhe(), 0 );
    int *buff1 = ahBild->getBuffer();
    int *buff2 = bild->getBuffer();
    for( int i = 0; i < bild->getBreite() * bild->getHöhe(); ++i )
        buff1[ i ] = buff2[ i ];
    bild->release();
    rend = 1;
}

void AuswahlListe::setAHFarbe( int f ) // setzt einen Zeiger zur Auswahl Hintergrund Farbe (nur ohne MultiStyled)
{
    ahFarbe = f;
    rend = 1;
}

void AuswahlListe::setAHBildZ( Bild *b ) // setzt einen Zeiger zum Hintergrund Bild (nur ohne MultiStyled)
{
    if( ahBild )
        ahBild->release();
    ahBild = b;
    rend = 1;
}

void AuswahlListe::setALRZ( int pos, LRahmen *rahmen ) // setzt einen Zeiger zum Auswahl Rahmen (nur mit MulitStyled)
{
    if( !aRahmenListe )
        aRahmenListe = new RCArray< LRahmen >();
    aRahmenListe->set( rahmen, pos );
    rend = 1;
}

void AuswahlListe::setALRBreite( int pos, int br ) // setzt die Breite des Auswahl Rahmens (nur mit MultiStyled)
{
    if( !aRahmenListe )
        aRahmenListe = new RCArray< LRahmen >();
    if( !aRahmenListe->z( pos ) )
        aRahmenListe->set( new LRahmen(), pos );
    aRahmenListe->z( pos )->setRamenBreite( br );
    rend = 1;
}

void AuswahlListe::setALRFarbe( int pos, int fc ) // setzt die Farbe des Auswahl Rahmens (nur mit MultiStyled)
{
    if( !aRahmenListe )
        aRahmenListe = new RCArray< LRahmen >();
    if( !aRahmenListe->z( pos ) )
        aRahmenListe->set( new LRahmen(), pos );
    aRahmenListe->z( pos )->setFarbe( fc );
    rend = 1;
}

void AuswahlListe::setAAFZ( int pos, AlphaFeld *buffer ) // setzt einen Zeiger zum Auswahl AlpaFeld (nur mit MultiStyled)
{
    if( !aBufferListe )
        aBufferListe = new RCArray< AlphaFeld >();
    aBufferListe->set( buffer, pos );
    rend = 1;
}

void AuswahlListe::setAAFStärke( int pos, int st ) // setzt die Stärke des Auswahl Hintergrund Buffers (nur mit MultiStyled)
{
    if( !aBufferListe )
        aBufferListe = new RCArray< AlphaFeld >();
    if( !aBufferListe->z( pos ) )
        aBufferListe->set( new AlphaFeld(), pos );
    aBufferListe->z( pos )->setStärke( st );
    rend = 1;
}

void AuswahlListe::setAAFFarbe( int pos, int fc ) // setzt die Farbe des Auswahl Hintergrund Buffers (nur mit MultiStyled)
{
    if( !aBufferListe )
        aBufferListe = new RCArray< AlphaFeld >();
    if( !aBufferListe->z( pos ) )
        aBufferListe->set( new AlphaFeld(), pos );
    aBufferListe->z( pos )->setFarbe( fc );
    rend = 1;
}

void AuswahlListe::setAHBild( int pos, Bild *bild ) // setzt das Auswahl Hintergrund Bild (nur mit MultiStyled)
{
    if( ahBildListe )
        ahBildListe = new RCArray< Bild >();
    if( !ahBildListe->z( pos ) )
        ahBildListe->set( new Bild(), pos );
    ahBildListe->z( pos )->neuBild( bild->getBreite(), bild->getHöhe(), 0 );
    int *buff1 = ahBildListe->z( pos )->getBuffer();
    int *buff2 = bild->getBuffer();
    for( int i = 0; i < bild->getBreite() * bild->getHöhe(); ++i )
        buff1[ i ] = buff2[ i ];
    bild->release();
    rend = 1;
}

void AuswahlListe::setAHFarbe( int pos, int f ) // setzt einen Zeiger zur Auswahl Hintergrund Farbe (nur miz MultiStyled)
{
    if( ahFarbeListe )
        ahFarbeListe = new Array< int >();
    ahFarbeListe->set( f, pos );
    rend = 1;
}

void AuswahlListe::setAHBildZ( int pos, Bild *b ) // setzt einen Zeiger zum Hintergrund Bild (nur mit MultiStyled)
{
    if( ahBildListe )
        ahBildListe = new RCArray< Bild >();
    ahBildListe->set( b, pos );
    rend = 1;
}

void AuswahlListe::setMsStyle( int pos, __int64 style ) // setzt den Style des Eintrags (nur mit MultiStyled)
{
    if( !styles )
        styles = new Array< __int64 >();
    styles->set( style, pos );
    rend = 1;
}

void AuswahlListe::setMsStyle( int pos, __int64 style, bool add_löschen )
{
    if( !styles )
        styles = new Array< __int64 >();
    if( add_löschen )
        styles->set( styles->get( pos ) | style, pos );
    else
        styles->set( styles->get( pos ) & ~style, pos );
    rend = 1;
}

void AuswahlListe::addMsStyle( int pos, __int64 style )
{
    if( !styles )
        styles = new Array< __int64 >();
    styles->set( styles->get( pos ) | style, pos );
    rend = 1;
}

void AuswahlListe::löscheMsStyle( int pos, __int64 style )
{
    if( !styles )
        styles = new Array< __int64 >();
    styles->set( styles->get( pos ) & ~style, pos );
    rend = 1;
}

void AuswahlListe::doMausEreignis( MausEreignis &me )
{
    bool nmakc = !me.verarbeitet;
    if( hatStyleNicht( Style::Sichtbar ) || hatStyleNicht( Style::Erlaubt ) )
        return;
    bool removeFokus = 0;
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
        removeFokus = 1;
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Verlässt )
    {
        if( removeFokus && me.id == ME_RLinks )
            löscheStyle( Style::Fokus );
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
    me.mx -= pos.x, me.my -= pos.y;
    if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
    {
        if( removeFokus && me.id == ME_RLinks )
            löscheStyle( Style::Fokus );
        if( !me.verarbeitet && hatStyleNicht( Style::Fokus ) && me.id == ME_RLinks )
            addStyle( Style::Fokus );
        if( hatStyle( Style::VScroll ) && vertikalScrollBar )
        {
            int rbr = 0;
            if( rahmen && hatStyle( Style::Rahmen ) )
                rbr = rahmen->getRBreite();
            if( ( ( me.mx > gr.x - 15 - rbr ) || me.id == ME_UScroll || me.id == ME_DScroll ) && me.id != ME_Betritt && me.id != ME_Verlässt )
            {
                vertikalScrollBar->doMausMessage( gr.x - rbr - 15, rbr, 15, gr.y - rbr * 2, me );
                me.verarbeitet = 1;
            }
        }
        if( !me.verarbeitet && me.id == ME_RLinks )
        {
            int eintr = getKlickEintrag( me.my );
            if( eintr >= 0 )
            {
                if( hatStyleNicht( Style::MultiSelect ) )
                {
                    auswahl = eintr;
                    rend = 1;
                }
                else
                {
                    bool shift = TastenStand[ T_Shift ];
                    bool strg = TastenStand[ T_Strg ];
                    if( strg )
                    {
                        setMsStyle( eintr, Style::Ausgewählt, hatMsStyleNicht( eintr, Style::Ausgewählt ) );
                        auswahl = eintr;
                    }
                    else if( shift && auswahl != -1 )
                    {
                        deSelect();
                        int beg = auswahl, end = eintr;
                        if( beg > end )
                        {
                            int tmp = end;
                            end = beg;
                            beg = tmp;
                        }
                        for( int i = beg; i <= end; ++i )
                        {
                            addMsStyle( i, Style::Ausgewählt );
                        }
                    }
                    else
                    {
                        deSelect();
                        addMsStyle( eintr, Style::Ausgewählt );
                        auswahl = eintr;
                    }
                }
            }
            else
                deSelect();
        }
        me.verarbeitet = 1;
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
}

void AuswahlListe::doTastaturEreignis( TastaturEreignis &te )
{
    bool ntakc = !te.verarbeitet;
    if( hatStyleNicht( Style::Fokus ) || !Tak || te.verarbeitet )
        return;
    ++ref;
    if( Tak( takParam, this, te ) )
    {
        if( te.id == TE_Press )
        {
            if( hatStyleNicht( Style::MultiSelect ) )
            {
                switch( te.taste )
                {
                case T_Unten:
                    ++auswahl;
                    if( auswahl > einträge )
                        auswahl = einträge;
                    rend = 1;
                    break;
                case T_Oben:
                    --auswahl;
                    if( auswahl < -1 )
                        auswahl = -1;
                    rend = 1;
                    break;
                }
            }
            else
            {
                switch( te.taste )
                {
                case T_Unten:
                    deSelect();
                    ++auswahl;
                    if( auswahl > einträge )
                        auswahl = einträge;
                    if( auswahl >= 0 )
                        addMsStyle( auswahl, Style::Ausgewählt );
                    rend = 1;
                    break;
                case T_Oben:
                    deSelect();
                    --auswahl;
                    if( auswahl < -1 )
                        auswahl = -1;
                    if( auswahl >= 0 )
                        addMsStyle( auswahl, Style::Ausgewählt );
                    rend = 1;
                    break;
                }
            }
        }
        te.verarbeitet = 1;
    }
    --ref;
    if( ntakc && te.verarbeitet && nTak )
        te.verarbeitet = nTak( ntakParam, this, te );
    if( !ref )
        delete this;
}

void AuswahlListe::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( !hatStyle( Style::Sichtbar ) )
        return;
    löscheStyle( Style::HScroll );
    __super::render( zRObj );
    lockZeichnung();
    if( !zRObj.setDrawOptions( innenPosition, innenGröße ) )
    {
        unlockZeichnung();
        return;
    }
    int rbr = 0;
    if( rahmen && hatStyle( Style::Rahmen ) )
        rbr = rahmen->getRBreite();
    if( tfListe )
    {
        einträge = tfListe->getEintragAnzahl();
        int maxHöhe = 0;
        int dx = 0, dy = 0;
        if( vertikalScrollBar && hatStyle( Style::VScroll ) )
            dy -= vertikalScrollBar->getScroll();
        int mdy = innenGröße.y + rbr;
        for( int i = 0; i < einträge; ++i )
        {
            TextFeld *tf = tfListe->z( i );
            if( dy + tf->getHöhe() > mdy && !(vertikalScrollBar && hatStyle( Style::VScroll ) ) )
                break;
            tf->setPosition( dx, dy );
            tf->setGröße( innenGröße.x, tf->getHöhe() );
            maxHöhe += tf->getHöhe();
            bool selected = 0;
            if( hatStyle( Style::MultiSelect ) && styles )
                selected = hatMsStyle( i, Style::Ausgewählt );
            else
                selected = auswahl == i;
            AlphaFeld *tmpBuffer = 0;
            bool tmpB = 0;
            int tmpHFarbe = 0;
            bool tmpH = 0;
            Bild *tmpHBild = 0;
            bool tmpHB = 0;
            bool tmpHAlpha = 0;
            LRahmen *tmpRahmen = 0;
            bool tmpR = 0;
            if( selected )
            {
                if( hatStyleNicht( Style::MultiStyled ) || !styles )
                {
                    if( hatStyle( Style::AuswahlBuffer ) && aBuffer )
                    {
                        tmpBuffer = tf->getAlphaFeld();
                        tf->setAlphaFeldZ( aBuffer->getThis() );
                        tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                        tf->setStyle( TextFeld::Style::Buffered, hatStyle( Style::AuswahlBuffer ) );
                    }
                    if( hatStyle( Style::AuswahlHintergrund ) )
                    {
                        tmpH = tf->hatStyle( TextFeld::Style::Hintergrund );
                        tmpHFarbe = tf->getHintergrundFarbe();
                        tf->setHintergrundFarbe( ahFarbe );
                        tf->setStyle( TextFeld::Style::Hintergrund, hatStyle( Style::Hintergrund ) );
                        if( hatStyle( Style::AuswahlHBild ) && ahBild )
                        {
                            tmpHBild = tf->getHintergrundBild();
                            tf->setHintergrundBildZ( ahBild->getThis() );
                            tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                            tf->setStyle( TextFeld::Style::HBild, hatStyle( Style::HBild ) );
                        }
                        if( hatStyle( Style::AuswahlHAlpha ) )
                        {
                            tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                            tf->setStyle( TextFeld::Style::HAlpha, hatStyle( Style::AuswahlHAlpha ) );
                        }
                    }
                    if( hatStyle( Style::AuswahlRahmen ) && aRahmen )
                    {
                        tmpRahmen = tf->getLinienRahmen();
                        tf->setLinienRahmenZ( aRahmen->getThis() );
                        tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, hatStyle( Style::AuswahlRahmen ) );
                    }
                }
                else
                {
                    if( hatMsStyle( i, Style::AuswahlBuffer ) && aBufferListe )
                    {
                        tmpBuffer = tf->getAlphaFeld();
                        tf->setAlphaFeldZ( aBufferListe->get( i ) );
                        tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                        tf->setStyle( TextFeld::Style::Buffered, hatMsStyle( i, Style::AuswahlBuffer ) );
                    }
                    if( hatMsStyle( i, Style::AuswahlHintergrund ) )
                    {
                        tmpH = tf->hatStyle( Style::Hintergrund );
                        tf->setStyle( TextFeld::Style::Hintergrund, hatMsStyle( i, Style::AuswahlHintergrund ) );
                        if( ahFarbeListe && ahFarbeListe->hat( i ) )
                        {
                            tmpHFarbe = tf->getHintergrundFarbe();
                            tf->setHintergrundFarbe( ahFarbeListe->get( i ) );
                        }
                        if( hatMsStyle( i, Style::AuswahlHBild ) && ahBildListe )
                        {
                            tmpHBild = tf->getHintergrundBild();
                            tf->setHintergrundBildZ( ahBildListe->get( i ) );
                            tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                            tf->setStyle( TextFeld::Style::HBild, hatMsStyle( i, Style::HBild ) );
                        }
                        if( hatMsStyle( i, Style::AuswahlHAlpha ) )
                        {
                            tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                            tf->setStyle( TextFeld::Style::HAlpha, hatMsStyle( i, Style::AuswahlHAlpha ) );
                        }
                    }
                    if( hatMsStyle( i, Style::AuswahlRahmen ) && aRahmenListe )
                    {
                        tmpRahmen = tf->getLinienRahmen();
                        tf->setLinienRahmenZ( aRahmenListe->get( i ) );
                        tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, hatMsStyle( i, Style::AuswahlRahmen ) );
                    }
                }
            }
            tf->render( zRObj );
            if( selected )
            {
                if( hatStyleNicht( Style::MultiStyled ) || !styles )
                {
                    if( hatStyle( Style::AuswahlBuffer ) )
                    {
                        tf->setAlphaFeldZ( tmpBuffer );
                        tf->setStyle( TextFeld::Style::Buffered, tmpB );
                    }
                    if( hatStyle( Style::AuswahlHintergrund ) )
                    {
                        tf->setHintergrundFarbe( tmpHFarbe );
                        tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                        if( hatStyle( Style::AuswahlHBild ) )
                        {
                            tf->setHintergrundBildZ( tmpHBild );
                            tf->setStyle( TextFeld::Style::HBild, tmpHB );
                        }
                        if( hatStyle( Style::AuswahlHAlpha ) )
                            tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                    }
                    if( hatStyle( Style::AuswahlRahmen ) )
                    {
                        tf->setLinienRahmenZ( tmpRahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                    }
                }
                else
                {
                    if( hatMsStyle( i, Style::AuswahlBuffer ) && aBufferListe )
                    {
                        tf->setAlphaFeldZ( tmpBuffer );
                        tf->setStyle( TextFeld::Style::Buffered, tmpB );
                    }
                    if( hatMsStyle( i, Style::AuswahlHintergrund ) )
                    {
                        tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                        if( ahFarbeListe && ahFarbeListe->hat( i ) )
                            tf->setHintergrundFarbe( tmpHFarbe );
                        if( hatMsStyle( i, Style::AuswahlHBild ) && ahBildListe )
                        {
                            tf->setHintergrundBildZ( tmpHBild );
                            tf->setStyle( TextFeld::Style::HBild, tmpHB );
                        }
                        if( hatMsStyle( i, Style::AuswahlHAlpha ) )
                            tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                    }
                    if( hatMsStyle( i, Style::AuswahlRahmen ) && aRahmenListe )
                    {
                        tf->setLinienRahmenZ( tmpRahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                    }
                }
            }
            dy += tf->getHöhe();
        }
        if( vertikalScrollBar )
            vertikalScrollBar->getScrollData()->max = maxHöhe;
    }
    zRObj.releaseDrawOptions();
    unlockZeichnung();
}

int AuswahlListe::getKlickEintrag( int my )
{
    if( !tfListe )
        return -1;
    einträge = tfListe->getEintragAnzahl();
    int y = 0;
    if( hatStyle( Style::VScroll ) && vertikalScrollBar )
        y -= vertikalScrollBar->getScroll();
    for( int i = 0; i < einträge; ++i )
    {
        y += tfListe->z( i )->getHöhe();
        if( y > my )
            return i;
    }
    return -1;
}

void AuswahlListe::setAuswahl( int ausw ) // setzt die Auswahl
{
    if( hatStyleNicht( Style::MultiSelect ) )
        auswahl = ausw;
    else if( styles )
    {
        for( int i = 0; i < einträge; ++i )
            löscheMsStyle( i, Style::Ausgewählt );
        addMsStyle( ausw, Style::Ausgewählt );
    }
}

void AuswahlListe::deSelect()
{
    if( hatStyleNicht( Style::MultiSelect ) )
        auswahl = -1;
    else if( styles )
    {
        for( int i = 0; i < einträge; ++i )
        {
            löscheMsStyle( i, Style::Ausgewählt );
        }
    }
}

// constant 
int AuswahlListe::getEintragAnzahl() const // gibt die Anzahl der Einträge zurück
{
    return einträge;
}

int AuswahlListe::getAuswahl() const // gibt den ersten ausgewählten Eintrag zurück
{
    return auswahl;
}

int AuswahlListe::getEintragPos( Text *eintragText ) // gibt die Position des eintrages mit dem entsprechenden Textes zurück
{
    for( int i = 0; i < einträge; ++i )
    {
        if( tfListe->z( i )->zText()->istGleich( eintragText->getText() ) )
        {
            eintragText->release();
            return i;
        }
    }
    return -1;
}

TextFeld *AuswahlListe::getEintrag( int pos ) const // gibt den pos- ten Eintrag zurück
{
    if( !tfListe )
        return 0;
    TextFeld *ret = (TextFeld*)tfListe->get( pos );
    if( ret )
        return ret->getThis();
    return 0;
}

TextFeld *AuswahlListe::zEintrag( int pos ) const
{
    if( !tfListe )
        return 0;
    return (TextFeld*)tfListe->z( pos );
}

LRahmen *AuswahlListe::getARahmen() const // gibt den Auswahl Rahmen zurück (ohne MultiStyled)
{
    if( aRahmen )
        return aRahmen->getThis();
    return 0;
}

LRahmen *AuswahlListe::zARahmen() const
{
    return aRahmen;
}

int AuswahlListe::getAHFarbe() const // gibt die Auswahl Hintergrund Farbe zurück (ohne MultiStyled)
{
    return ahFarbe;
}

Bild *AuswahlListe::getAHBild() const // gibt das Auswahl Hintergrund Bild zurück (ohne MultiStyled)
{
    if( ahBild )
        return ahBild->getThis();
    return 0;
}

Bild *AuswahlListe::zAHBild() const
{
    return ahBild;
}

AlphaFeld *AuswahlListe::getABuffer() const // gibt den Auswahl Buffer zurück (ohne MultiStyled)
{
    if( aBuffer )
        return aBuffer->getThis();
    return 0;
}

AlphaFeld *AuswahlListe::zABuffer() const
{
    return aBuffer;
}

LRahmen *AuswahlListe::getARahmen( int pos ) const // gibt den Auswahl Rahmen zurück (mit MultiStyled)
{
    LRahmen *ret = 0;
    if( aRahmenListe )
        ret = (LRahmen*)aRahmenListe->get( pos );
    if( ret )
        return ret->getThis();
    return 0;
}

LRahmen *AuswahlListe::zARahmen( int pos ) const
{
    LRahmen *ret = 0;
    if( aRahmenListe )
        ret = (LRahmen*)aRahmenListe->z( pos );
    return ret;
}

int AuswahlListe::getAHFarbe( int pos ) const // gibt die Auswahl Hintergrund Farbe zurück (mit MultiStyled)
{
    if( ahFarbeListe && ahFarbeListe->hat( pos ) )
        return ahFarbeListe->get( pos );
    return 0;
}

Bild *AuswahlListe::getAHBild( int pos ) const // gibt das Auswahl Hintergrund Bild zurück (mit MultiStyled)
{
    Bild *ret = 0;
    if( ahBildListe )
        ret = (Bild*)ahBildListe->get( pos );
    if( ret )
        return ret->getThis();
    return 0;
}

Bild *AuswahlListe::zAHBild( int pos ) const
{
    Bild *ret = 0;
    if( ahBildListe )
        ret = (Bild*)ahBildListe->z( pos );
    return ret;
}

AlphaFeld *AuswahlListe::getABuffer( int pos ) const // gibt den Auswahl Buffer zurück (mit MultiStyled)
{
    AlphaFeld *ret = 0;
    if( aBufferListe )
        ret = (AlphaFeld*)aBufferListe->get( pos );
    if( ret )
        return ret->getThis();
    return 0;
}

AlphaFeld *AuswahlListe::zABuffer( int pos ) const
{
    AlphaFeld *ret = 0;
    if( aBufferListe )
        ret = (AlphaFeld*)aBufferListe->z( pos );
    return ret;
}
bool AuswahlListe::hatMsStyle( int pos, __int64 style ) const // prüft ob style vorhanden (mit MultiStyled)
{
    __int64 st = 0;
    if( styles )
        st = styles->get( pos );
    return ( st | style ) == st;
}

bool AuswahlListe::hatMsStyleNicht( int pos, __int64 style ) const // prüft obt style nicht vorhanden (mit MultiStyled)
{
    __int64 st = 0;
    if( styles )
        st = styles->get( pos );
    return ( st | style ) != st;
}

// Reference Counting 
AuswahlListe *AuswahlListe::getThis()
{
    ++ref;
    return this;
}

AuswahlListe *AuswahlListe::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}