#include "TextFeld.h"
#include "Schrift.h"
#include "Text.h"
#include "AlphaFeld.h"
#include "Rahmen.h"
#include "Bild.h"
#include "TastaturEreignis.h"
#include "MausEreignis.h"
#include "Fenster.h"
#include "Scroll.h"
#include <math.h>
#include "Globals.h"
#include "ToolTip.h"
#include "Scroll.h"

using namespace Framework;

// Inhalt der TextFeld Klasse aus TextFeld.h 
// Konstruktor 
TextFeld::TextFeld()
    : ZeichnungHintergrund(),
    schriftGröße( 12 ),
    schrift( 0 ),
    text( 0 ),
    sF( 0xFF000000 ),
    showChar( 0 ),
    begf( 0 ),
    cpos( 0 ),
    tickVal( 0 ),
    mausKlick( 0 ),
    ref( 1 )
{
    horizontalScrollBar = new HScrollBar();
    vertikalScrollBar = new VScrollBar();
    style = 0;
    this->setMausEreignis( _ret1ME );
    this->setTastaturEreignis( _ret1TE );
}

// Destruktor 
TextFeld::~TextFeld()
{
    if( schrift )
        schrift->release();
    if( text )
        text->release();
}

// nicht constant 
void TextFeld::setText( Text *txt ) // setzt den angezeigten Text
{
    lockZeichnung();
    if( !text )
        text = new Text();
    text->setText( txt );
    if( hatStyle( Style::VScroll ) )
        updateVScroll();
    if( hatStyle( Style::HScroll ) )
        updateHScroll();
    unlockZeichnung();
    rend = 1;
}

void TextFeld::setTextZ( Text *txt ) // setzt einen Zeiger zum angezeigten Text
{
    lockZeichnung();
    if( text )
        text->release();
    text = txt;
    if( hatStyle( Style::VScroll ) )
        updateVScroll();
    if( hatStyle( Style::HScroll ) )
        updateHScroll();
    rend = 1;
    unlockZeichnung();
}

void TextFeld::setText( const char *txt ) // setzt den angezeigten Text
{
    lockZeichnung();
    if( !text )
        text = new Text();
    text->setText( txt );
    if( hatStyle( Style::VScroll ) )
        updateVScroll();
    if( hatStyle( Style::HScroll ) )
        updateHScroll();
    rend = 1;
    unlockZeichnung();
}

void TextFeld::addZeile( const char *zeile ) // fügt Zeile An
{
    if( text )
    {
        Text *txt = new Text( zeile );
        txt->anhängen( "\n" );
        if( schrift )
        {
            bool vs = vertikalScrollBar && hatStyle( Style::VScroll );
            int rbr = ( rahmen && hatStyle( Style::Rahmen ) ) ? rahmen->getRBreite() : 0;
            schrift->lock();
            schrift->setSchriftGröße( schriftGröße );
            schrift->textFormatieren( txt, gr.x - ( (int)vs * 15 ) - rbr * 2, schriftGröße );
            schrift->unlock();
        }
        lockZeichnung();
        text->anhängen( txt->getText() );
        unlockZeichnung();
        txt->release();
        if( hatStyle( Style::VScroll ) )
            updateVScroll();
        if( hatStyle( Style::HScroll ) )
            updateHScroll();
        rend = 1;
    }
}

void TextFeld::setAuswahl( int pos1, int pos2 ) // setzt den Ausgewählten Text
{
    cpos = pos1;
    begf = pos2;
    rend = 1;
}

void TextFeld::setAuswahl( Punkt &auswahl )
{
    cpos = auswahl.x;
    begf = auswahl.y;
    rend = 1;
}

void TextFeld::setSchriftZ( Schrift *schrift ) // setzt einen Zeiger zur Schrift
{
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    rend = 1;
}

void TextFeld::setSchriftGröße( unsigned char gr ) // setzt die Schriftgröße
{
    schriftGröße = gr;
    rend = 1;
}

void TextFeld::setSchriftFarbe( int fc ) // setzt die Schrift Farbe
{
    sF = fc;
    rend = 1;
}

void TextFeld::setSchowChar( unsigned char c ) // bei Passwortfeld *
{
    showChar = c;
    rend = 1;
}

void TextFeld::setVScrollZuZeile( int zeile ) // scrollt zur Zeile
{
    if( vertikalScrollBar && schrift && text && hatStyle( Style::Mehrzeilig ) )
    {
        schrift->lock();
        schrift->setSchriftGröße( schriftGröße );
        Text t = "a";
        vertikalScrollBar->scroll( zeile * ( schrift->getZeilenabstand() + schrift->getTextHöhe( &t ) ) );
        schrift->unlock();
        rend = 1;
    }
}

void TextFeld::updateVScroll( int pos ) // scrollt nach unten
{
    if( pos == -1 )
        pos = cpos;
    if( vertikalScrollBar )
    {
        int sPos = 0;
        int hö = 0;
        if( text && schrift )
        {
            if( hatStyleNicht( Style::Mehrzeilig ) )
                text->löschen( '\n' );
            schrift->setSchriftGröße( schriftGröße );
            hö = gr.y;
            if( hatStyle( Style::Rahmen ) && rahmen )
                hö -= rahmen->getRBreite() * 2;
            if( hatStyle( Style::HScroll ) && horizontalScrollBar )
                hö -= 15;
            vertikalScrollBar->update( schrift->getTextHöhe( text ) + schriftGröße + schrift->getTextHöhe( &Text( "a" ) ), hö );
            Text t;
            int zh = schrift->getTextHöhe( &t ) + schrift->getZeilenabstand();
            int l = text->getLänge();
            for( int i = 0; i < l && ( i < pos || hatStyleNicht( Style::Erlaubt ) ); ++i )
            {
                if( text->getText()[ i ] == '\n' )
                    sPos += zh;
            }
        }
        if( schrift )
        {
            if( sPos - schrift->getZeilenabstand() - schrift->getTextHöhe( &Text( "a" ) ) < vertikalScrollBar->getScroll() )
                vertikalScrollBar->scroll( sPos - schrift->getZeilenabstand() - schrift->getTextHöhe( &Text( "a" ) ) );
            if( sPos + schrift->getZeilenabstand() + schrift->getTextHöhe( &Text( "a" ) ) > vertikalScrollBar->getScroll() + vertikalScrollBar->getScrollData()->anzeige )
                vertikalScrollBar->scroll( sPos + ( schrift->getZeilenabstand() + schrift->getTextHöhe( &Text( "a" ) ) ) * 2 - hö );
        }
        rend = 1;
    }
}

void TextFeld::updateHScroll( int pos ) // scrollt zur Curser Position
{
    if( pos == -1 )
        pos = cpos;
    if( horizontalScrollBar && text && schrift )
    {
        schrift->lock();
        schrift->setSchriftGröße( schriftGröße );
        int br = gr.x;
        if( hatStyle( Style::Rahmen ) && rahmen )
            br -= rahmen->getRBreite() * 2;
        if( hatStyle( Style::VScroll ) && vertikalScrollBar )
            br -= 15;
        int maxBr = schrift->getTextBreite( text ) + schriftGröße;
        horizontalScrollBar->update( maxBr, br );
        if( hatStyle( Style::Erlaubt ) && maxBr > br && pos > 0 && pos < text->getLänge() )
        {
            int l = text->getLänge();
            int p1 = 0;
            char *tmp = text->getText();
            for( int i = 0; i < pos; i++, tmp++ )
            {
                if( *tmp == '\n' )
                    p1 = i + 1;
            }
            Text *t = text->getTeilText( p1, pos );
            int cbr = schrift->getTextBreite( t );
            t->release();
            if( cbr + schriftGröße > horizontalScrollBar->getScroll() + horizontalScrollBar->getScrollData()->anzeige )
                horizontalScrollBar->scroll( cbr + schriftGröße - br );
            if( cbr - schriftGröße < horizontalScrollBar->getScroll() )
                horizontalScrollBar->scroll( cbr - schriftGröße );
        }
        schrift->unlock();
    }
}

bool TextFeld::tick( double tickval ) // tick
{
    if( hatStyle( Style::Fokus ) )
    {
        if( tickVal < 0.5 && tickVal + tickval >= 0.5 )
            rend = 1;
        if( tickVal >= 0.5 && tickVal + tickval >= 1 )
            rend = 1;
        tickVal += tickval;
        if( tickVal >= 1 )
            tickVal -= 1;
    }
    return __super::tick( tickval );
}

void TextFeld::doMausEreignis( MausEreignis &me ) // Maus Ereignis
{
    bool nmakc = !me.verarbeitet;
    if( hatStyleNicht( Style::Erlaubt ) || hatStyleNicht( Style::Sichtbar ) )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
        me.mx -= pos.x, me.my -= pos.y;
        int rbr = 0;
        if( rahmen )
            rbr = rahmen->getRBreite();
        if( ( vertikalScrollBar && hatStyle( Style::VScroll ) ||
              horizontalScrollBar && hatStyle( Style::HScroll ) ) &&
            me.mx > rbr && me.mx < gr.x - rbr &&
            me.my > rbr && me.my < gr.y - rbr )
        {
            vertikalScrollBar->doMausMessage( gr.x - rbr - 15, rbr, 15, gr.y - rbr * 2, me );
            horizontalScrollBar->doMausMessage( rbr, gr.y - rbr * 2 - 15, gr.x - rbr * 2 - ( ( vertikalScrollBar && hatStyle( Style::VScroll ) ) ? 15 : 0 ), 15, me );
            me.verarbeitet = 1;
        }
        me.mx += pos.x, me.my += pos.y;
        mausKlick = 0;
        return;
    }
    bool removeFokus = 0;
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) )
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
            return;
        }
        removeFokus = 1;
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y ) && me.id != ME_Verlässt )
    {
        if( removeFokus && me.id == ME_RLinks )
        {
            me.mx -= pos.x, me.my -= pos.y;
            if( hatStyle( Style::Fokus ) && Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
                löscheStyle( Style::Fokus );
            if( nmakc && me.verarbeitet && nMak )
                me.verarbeitet = nMak( nmakParam, this, me );
            me.mx += pos.x, me.my += pos.y;
        }
        if( toolTip )
            toolTip->setMausIn( 0 );
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
    if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
    {
        if( removeFokus && me.id == ME_RLinks )
            löscheStyle( Style::Fokus );
        if( !me.verarbeitet )
        {
            if( hatStyleNicht( Style::Fokus ) )
            {
                mausKlick = 0;
                if( me.id == Framework::ME_PLinks )
                    addStyle( Style::Fokus );
            }
            int rbr = 0;
            if( rahmen )
                rbr = rahmen->getRBreite();
            if( vertikalScrollBar && hatStyle( Style::VScroll ) )
            {
                if( vertikalScrollBar->doMausMessage( gr.x - rbr - 15, rbr, 15, gr.y - rbr * 2, me ) )
                {
                    if( nmakc && me.verarbeitet && nMak )
                        me.verarbeitet = nMak( nmakParam, this, me );
                    me.mx += pos.x, me.my += pos.y;
                    return;
                }
            }
            if( horizontalScrollBar && hatStyle( Style::HScroll ) )
            {
                if( horizontalScrollBar->doMausMessage( rbr, gr.y - rbr - 15, gr.x - rbr * 2 - ( ( vertikalScrollBar && hatStyle( Style::VScroll ) ) ? 15 : 0 ), 15, me ) )
                {
                    if( nmakc && me.verarbeitet && nMak )
                        me.verarbeitet = nMak( nmakParam, this, me );
                    me.mx += pos.x, me.my += pos.y;
                    return;
                }
            }
            if( me.mx < gr.x - rbr - 15 )
            {
                if( schrift )
                {
                    schrift->setSchriftGröße( schriftGröße );
                    bool shift = TastenStand[ T_Shift ];
                    if( me.id == Framework::ME_PLinks )
                    {
                        int tbr = schrift->getTextBreite( text );
                        int thö = schrift->getTextHöhe( text );
                        int scrollHö = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) ? vertikalScrollBar->getScroll() : 0;
                        int scrollBr = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) ? horizontalScrollBar->getScroll() : 0;
                        int xxx = me.mx - rbr + scrollBr;
                        int yyy = me.my - rbr + scrollHö;
                        int scrollBreite = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) * 15;
                        int scrollHöhe = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) * 15;
                        if( hatStyle( Style::HCenter ) )
                            xxx -= ( ( ( gr.x - scrollBreite ) / 2 ) - tbr / 2 ) - rbr;
                        if( hatStyle( Style::VCenter ) && hatStyleNicht( Style::VScroll ) )
                            yyy -= ( ( ( gr.y - scrollHö ) / 2 ) - thö / 2 ) - rbr;
                        int pos = schrift->textPos( text, xxx, yyy );
                        if( pos != -1 )
                        {
                            if( shift )
                                begf = pos;
                            else
                            {
                                cpos = pos;
                                begf = pos;
                            }
                            rend = 1;
                            if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                                updateVScroll( begf );
                            if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                                updateHScroll( begf );
                        }
                        mausKlick = 1;
                    }
                    if( me.id == ME_Bewegung && mausKlick )
                    {
                        int tbr = schrift->getTextBreite( text );
                        int thö = schrift->getTextHöhe( text );
                        int scrollHö = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) ? vertikalScrollBar->getScroll() : 0;
                        int scrollBr = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) ? horizontalScrollBar->getScroll() : 0;
                        int xxx = me.mx - rbr + scrollBr;
                        int yyy = me.my - rbr + scrollHö;
                        int scrollBreite = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) * 15;
                        int scrollHöhe = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) * 15;
                        if( hatStyle( Style::HCenter ) )
                            xxx -= ( ( ( gr.x - scrollBreite ) / 2 ) - tbr / 2 ) - rbr;
                        if( hatStyle( Style::VCenter ) && hatStyleNicht( Style::VScroll ) )
                            yyy -= ( ( ( gr.y - scrollHöhe ) / 2 ) - thö / 2 ) - rbr;
                        int pos = schrift->textPos( text, xxx, yyy );
                        if( pos != -1 )
                        {
                            if( begf != pos )
                                rend = 1;
                            begf = pos;
                            if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                                updateVScroll( begf );
                            if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                                updateHScroll( begf );
                        }
                    }
                    if( me.id == ME_RLinks )
                    {
                        if( !shift )
                        {
                            int tbr = schrift->getTextBreite( text );
                            int thö = schrift->getTextHöhe( text );
                            int scrollHö = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) ? vertikalScrollBar->getScroll() : 0;
                            int scrollBr = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) ? horizontalScrollBar->getScroll() : 0;
                            int xxx = me.mx - rbr + scrollBr;
                            int yyy = me.my - rbr + scrollHö;
                            int scrollBreite = ( vertikalScrollBar && hatStyle( Style::VScroll ) ) * 15;
                            int scrollHöhe = ( horizontalScrollBar && hatStyle( Style::HScroll ) ) * 15;
                            if( hatStyle( Style::HCenter ) )
                                xxx -= ( ( ( gr.x - scrollBreite ) / 2 ) - tbr / 2 ) - rbr;
                            if( hatStyle( Style::VCenter ) && hatStyleNicht( Style::VScroll ) )
                                yyy -= ( ( ( gr.y - scrollHöhe ) / 2 ) - thö / 2 ) - rbr;
                            int pos = schrift->textPos( text, xxx, yyy );
                            if( pos != -1 )
                            {
                                begf = pos;
                                if( vertikalScrollBar && hatStyle( Style::VScroll ) )
                                    updateVScroll( begf );
                                if( horizontalScrollBar && hatStyle( Style::HScroll ) )
                                    updateHScroll( begf );
                            }
                            rend = 1;
                        }
                        mausKlick = 0;
                    }
                }
            }
        }
        me.verarbeitet = 1;
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
}

void TextFeld::doTastaturEreignis( TastaturEreignis &te )
{
    bool ntakc = !te.verarbeitet;
    if( te.verarbeitet || hatStyleNicht( Style::Fokus ) )
        return;
    if( !Tak )
        return;
    ++ref;
    if( Tak( takParam, this, te ) )
    {
        if( hatStyleNicht( Style::Erlaubt ) )
        {
            --ref;
            if( !ref )
                delete this;
            return;
        }
        if( te.id == TE_Press )
        {
            bool shift = TastenStand[ T_Shift ];
            bool strg = TastenStand[ T_Strg ];
            switch( te.taste )
            {
            case T_Entf:
                if( cpos != begf )
                    text->löschen( cpos, begf );
                else
                    text->löschen( cpos, cpos + 1 );
                begf = cpos;
                rend = 1;
                break;
            case T_BackSpace:
                if( cpos != begf )
                {
                    text->löschen( cpos, begf );
                    if( cpos > begf )
                        cpos -= cpos - begf;
                }
                else
                {
                    text->löschen( cpos - 1, cpos );
                    --cpos;
                }
                begf = cpos;
                rend = 1;
                break;
            case T_Enter:
                if( cpos != begf )
                {
                    text->löschen( begf, cpos );
                    if( cpos > begf )
                        cpos -= cpos - begf;
                }
                text->einfügen( cpos, '\n' );
                ++cpos;
                begf = cpos;
                rend = 1;
                break;
            case T_Links:
                if( shift )
                {
                    if( strg )
                        begf = text->getLKick( begf );
                    else
                        --begf;
                }
                else
                {
                    if( strg )
                        cpos = text->getLKick( cpos );
                    else
                        --cpos;
                    begf = cpos;
                }
                rend = 1;
                break;
            case T_Oben:
                if( shift )
                {
                    begf = text->getOKick( begf );
                }
                else
                {
                    cpos = text->getOKick( cpos );
                    begf = cpos;
                }
                rend = 1;
                break;
            case T_Rechts:
                if( shift )
                {
                    if( strg )
                        begf = text->getRKick( begf );
                    else
                        ++begf;
                }
                else
                {
                    if( strg )
                        cpos = text->getRKick( cpos );
                    else
                        ++cpos;
                    begf = cpos;
                }
                rend = 1;
                break;
            case T_Unten:
                if( shift )
                {
                    begf = text->getUKick( begf );
                }
                else
                {
                    cpos = text->getUKick( cpos );
                    begf = cpos;
                }
                rend = 1;
                break;
            default:
                if( strg && te.id == TE_Press )
                {
                    if( te.taste == 'c' || te.taste == 'C' )
                    {
                        if( begf != cpos )
                        {
                            int län = begf - cpos;
                            if( län < 0 )
                                län = -län;
                            char *txt = new char[ län + 1 ];
                            txt[ län ] = 0;
                            int beg = begf < cpos ? begf : cpos;
                            for( int i = beg; i < beg + län; ++i )
                                txt[ i - beg ] = text->getText()[ i ];
                            TextKopieren( txt );
                            delete[] txt;
                        }
                        else
                            TextKopieren( text->getText() );
                    }
                    if( te.taste == 'v' || te.taste == 'V' )
                    {
                        if( begf != cpos )
                        {
                            text->löschen( begf, cpos );
                            if( cpos > begf )
                                cpos = begf;
                        }
                        char *txt = TextEinfügen();
                        text->einfügen( cpos, txt );
                        cpos += textLänge( txt );
                        begf = cpos;
                        rend = 1;
                    }
                    break;
                }
                if( istSchreibbar( te.taste ) )
                {
                    if( begf != cpos )
                    {
                        text->löschen( begf, cpos );
                        if( cpos > begf )
                            cpos = begf;
                    }
                    text->einfügen( cpos, (char)te.taste );
                    ++cpos;
                    begf = cpos;
                    rend = 1;
                }
                break;
            }
        }
        if( cpos < 0 )
            cpos = 0;
        if( cpos > text->getLänge() )
            cpos = text->getLänge();
        if( begf < 0 )
            begf = 0;
        if( begf > text->getLänge() )
            begf = text->getLänge();
        if( hatStyle( Style::VScroll ) )
            updateVScroll( begf );
        if( hatStyle( Style::HScroll ) )
            updateHScroll( begf );
        te.verarbeitet = 1;
    }
    --ref;
    if( ntakc && te.verarbeitet && nTak )
        te.verarbeitet = nTak( ntakParam, this, te );
    if( !ref )
        delete this;
}

void TextFeld::render( Bild &zRObj ) // zeichenet nach zRObj
{
    if( hatStyleNicht( Style::Sichtbar ) )
        return;
    __super::render( zRObj );
    if( !text || !schrift )
        return;
    lockZeichnung();
    if( !zRObj.setDrawOptions( innenPosition, innenGröße ) )
    {
        unlockZeichnung();
        return;
    }
    if( hatStyleNicht( Style::Mehrzeilig ) )
        text->löschen( '\n' );
    if( hatStyleNicht( Style::Mehrfarbig ) )
    {
        while( text->hat( '\r' ) )
            text->löschen( text->positionVon( '\r' ), text->positionVon( '\r' ) + 11 );
    }
    schrift->setSchriftGröße( schriftGröße );
    int tbr = schrift->getTextBreite( text );
    int thö = schrift->getTextHöhe( text );
    int xxx = 0;
    int yyy = 0;
    int breite = innenGröße.x;
    int höhe = innenGröße.y;
    bool hs = horizontalScrollBar && hatStyle( Style::HScroll );
    bool vs = vertikalScrollBar && hatStyle( Style::VScroll );
    if( vs )
        yyy -= vertikalScrollBar->getScroll();
    if( hs )
        xxx -= horizontalScrollBar->getScroll();
    if( hatStyle( Style::HCenter ) && !hs )
        xxx = ( breite / 2 ) - tbr / 2;
    if( hatStyle( Style::VCenter ) && !vs )
        yyy = ( höhe / 2 ) - thö / 2;
    schrift->setDrawPosition( xxx, yyy );
    if( hatStyle( Style::Fokus ) && hatStyle( Style::Erlaubt ) )
    {
        if( istSchreibbar( showChar ) )
        {
            Text rt;
            int län = text->getLänge() - text->anzahlVon( '\n' ) - text->anzahlVon( '\r' ) * 11;
            rt.füllText( showChar, län );
            if( tickVal <= 0.5 )
                schrift->renderText( &rt, zRObj, cpos, 0xFFFF5555, begf, 0xFF0000FF, sF );
            else
                schrift->renderText( &rt, zRObj, cpos, 0x00000000, begf, 0xFF0000FF, sF );
        }
        else
        {
            if( tickVal <= 0.5 )
                schrift->renderText( text, zRObj, cpos, 0xFFFF5555, begf, 0xFF0000FF, sF );
            else
                schrift->renderText( text, zRObj, cpos, 0x00000000, begf, 0xFF0000FF, sF );
        }
    }
    else
    {
        if( istSchreibbar( showChar ) )
        {
            Text rt;
            int län = text->getLänge() - text->anzahlVon( '\n' ) - text->anzahlVon( '\r' ) * 11;
            rt.füllText( showChar, län );
            schrift->renderText( &rt, zRObj, sF );
        }
        else
            schrift->renderText( text, zRObj, sF );
    }
    zRObj.releaseDrawOptions();
    unlockZeichnung();
}

// Konstant 
Text *TextFeld::getText() const // gibt vom Text zurück
{
    if( !text )
        return 0;
    return text->getThis();
}

Text *TextFeld::zText() const // gibt den Text zurück
{
    return text;
}

Schrift *TextFeld::getSchrift() const// gint getThis der Schrift Zurück
{
    if( !schrift )
        return 0;
    return schrift->getThis();
}

Schrift *TextFeld::zSchrift() const// gibt die Schrift zurück
{
    return schrift;
}

unsigned char TextFeld::getSchriftGröße() const // gibt die Schriftgröße zurück
{
    return schriftGröße;
}

int TextFeld::getSchriftFarbe() const// gibt getThis der Schriftfarbe zurück
{
    return sF;
}

unsigned char TextFeld::getShowChar() const // gibt den Anzeige Char zurück
{
    return showChar;
}

int TextFeld::getCursorPos() const
{
    return cpos;
}

int TextFeld::getFärbungPos() const
{
    return begf;
}

Zeichnung *TextFeld::dublizieren() const // Erzeugt eine Kopie des Zeichnungs
{
    TextFeld *obj = new TextFeld();
    obj->setPosition( pos );
    obj->setGröße( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( Mak );
    obj->setTastaturEreignis( Tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    obj->setStyle( style );
    obj->setSchriftGröße( schriftGröße );
    if( schrift )
        obj->setSchriftZ( schrift->getThis() );
    if( text )
        obj->setText( text->getText() );
    obj->setHintergrundFarbe( hintergrundFarbe );
    obj->setSchriftFarbe( sF );
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
    obj->setSchowChar( showChar );
    obj->setAuswahl( begf, cpos );
    return obj;
}

// Reference Counting 
TextFeld *TextFeld::getThis()
{
    ++ref;
    return this;
}

TextFeld *TextFeld::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}