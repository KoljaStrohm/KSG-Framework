#include "AuswahlBox.h"
#include "AlphaFeld.h"
#include "Bild.h"
#include "MausEreignis.h"
#include "TastaturEreignis.h"
#include "Rahmen.h"
#include "Scroll.h"
#include "TextFeld.h"
#include "Knopf.h"
#include "Text.h"
#include "Schrift.h"
#include "ToolTip.h"

using namespace Framework;

// Inhalt der AuswahlBox Klasse aus AuswahlBox.h
// Konstruktor 
AuswahlBox::AuswahlBox()
    : ZeichnungHintergrund(),
    schrift( 0 ),
    msStyle( 0 ),
    members( new RCArray< TextFeld >() ),
    ausfahren( new Knopf() ),
    auswRahmen( new LRahmen() ),
    auswBgF( 0xFF000000 ),
    auswBgB( 0 ),
    auswAf( new AlphaFeld() ),
    msAuswRahmen( 0 ),
    msAuswBgF( 0 ),
    msAuswBgB( 0 ),
    msAuswAf( 0 ),
    mausRahmen( new LRahmen() ),
    mausBgF( 0xFF000000 ),
    mausBgB( 0 ),
    mausAf( new AlphaFeld() ),
    msMausRahmen( 0 ),
    msMausBgF( 0 ),
    msMausBgB( 0 ),
    msMausAf( 0 ),
    anzahl( 0 ),
    auswahl( 0 ),
    ausgeklappt( 0 ),
    ausklappHeight( 0 ),
    ausklapMaxHeight( 200 ),
    eintragHeight( 15 ),
    tickval( 0 ),
    mausEintrag( 0 ),
    scrollAnzeigen( 0 ),
    eAkP( 0 ),
    eAk( 0 ),
    ref( 1 )
{
    vertikalScrollBar = new VScrollBar();
    style = Style::Normal;
    rahmen = new LRahmen();
    rahmen->setFarbe( 0xFFFFFFFF );
    rahmen->setRamenBreite( 1 );
    ausfahren->setStyle( Knopf::Style::Sichtbar | Knopf::Style::Erlaubt | Knopf::Style::KlickBuffer );
    ausfahren->setKBFarbe( 0xA0000000 );
    ausfahren->setKBStrength( 10 );
    ausfahren->setSize( 18, 18 );
    ausfahren->setSchriftFarbe( 0xFFFFFFFF );
    ausfahren->setText( "\\/" );
    auswRahmen->setFarbe( 0xFF00FF00 );
    auswRahmen->setRamenBreite( 1 );
    auswAf->setFarbe( 0xA000FF00 );
    auswAf->setStrength( 7 );
    mausRahmen->setFarbe( 0xFF00FF00 );
    mausRahmen->setRamenBreite( 1 );
    mausAf->setFarbe( 0x5000FF00 );
    mausAf->setStrength( 7 );
    gr.x = 20;
    gr.y = 20;
}

// Destruktor 
AuswahlBox::~AuswahlBox()
{
    if( schrift )
        schrift->release();
    if( msStyle )
        msStyle->release();
    if( members )
        members->release();
    if( ausfahren )
        ausfahren->release();
    if( auswRahmen )
        auswRahmen->release();
    if( auswBgB )
        auswBgB->release();
    if( auswAf )
        auswAf->release();
    if( msAuswRahmen )
        msAuswRahmen->release();
    if( msAuswAf )
        msAuswAf->release();
    if( msAuswBgB )
        msAuswBgB->release();
    if( msAuswBgF )
        msAuswBgF->release();
    if( mausRahmen )
        mausRahmen->release();
    if( mausAf )
        mausAf->release();
    if( mausBgB )
        mausBgB->release();
    if( msMausRahmen )
        msMausRahmen->release();
    if( msMausAf )
        msMausAf->release();
    if( msMausBgB )
        msMausBgB->release();
    if( msMausBgF )
        msMausBgF->release();
}

// nicht constant
void AuswahlBox::setEventParam( void *p ) // setzt den Event Parameter
{
    eAkP = p;
}

void AuswahlBox::setEventAktion( void( *eAk )( void *p, AuswahlBox *, int, int ) ) // setzt die Event Funktion
{
    this->eAk = eAk;
}

void AuswahlBox::setSchriftZ( Schrift *schrift ) // setzt die schrift
{
    if( this->schrift )
        this->schrift->release();
    this->schrift = schrift;
    ausfahren->setSchriftZ( schrift->getThis() );
    rend = 1;
}

void AuswahlBox::addEintrag( const char *txt ) // Eintrag hinzufügen
{
    TextFeld *tf = new TextFeld();
    if( schrift )
        tf->setSchriftZ( schrift->getThis() );
    tf->addStyle( TextFeld::Style::Sichtbar | TextFeld::Style::Center | TextFeld::Style::Rahmen );
    tf->setText( txt );
    tf->setSchriftFarbe( 0xFFFFFFFF );
    tf->setLinienRahmenFarbe( 0xFFFFFFFF );
    tf->setSize( 0, eintragHeight );
    members->add( tf, anzahl );
    ++anzahl;
    rend = 1;
}

void AuswahlBox::addEintrag( Text *txt )
{
    TextFeld *tf = new TextFeld();
    if( schrift )
        tf->setSchriftZ( schrift->getThis() );
    tf->addStyle( TextFeld::Style::Sichtbar | TextFeld::Style::Center | TextFeld::Style::Rahmen );
    tf->setText( txt );
    tf->setSchriftFarbe( 0xFFFFFFFF );
    tf->setLinienRahmenFarbe( 0xFFFFFFFF );
    tf->setSize( 0, eintragHeight );
    members->add( tf, anzahl );
    ++anzahl;
    rend = 1;
}

void AuswahlBox::addEintragZ( TextFeld *txt )
{
    members->add( txt, anzahl );
    ++anzahl;
    rend = 1;
}

void AuswahlBox::setEintrag( int i, const char *txt ) // Eintrag setzen
{
    if( members->z( i ) )
        members->z( i )->setText( txt );
    rend = 1;
}

void AuswahlBox::setEintrag( int i, Text *txt )
{
    if( members->z( i ) )
        members->z( i )->setText( txt );
    else
        txt->release();
    rend = 1;
}

void AuswahlBox::setEintragZ( int i, TextFeld *txt )
{
    if( i < anzahl )
        members->set( txt, i );
    else
        txt->release();
    rend = 1;
}

void AuswahlBox::removeEintrag( int i ) // Eintrag entfernen
{
    if( i < anzahl )
    {
        members->remove( i );
        if( msStyle )
            msStyle->remove( i );
        if( msAuswRahmen )
            msAuswRahmen->remove( i );
        if( msAuswBgF )
            msAuswBgF->remove( i );
        if( msAuswBgB )
            msAuswBgB->remove( i );
        if( msAuswAf )
            msAuswAf->remove( i );
        if( msMausRahmen )
            msMausRahmen->remove( i );
        if( msMausBgF )
            msMausBgF->remove( i );
        if( msMausBgB )
            msMausBgB->remove( i );
        if( msMausAf )
            msMausAf->remove( i );
        if( auswahl > i )
            --auswahl;
        if( mausEintrag > i )
            --mausEintrag;
        --anzahl;
        rend = 1;
    }
}

void AuswahlBox::setAusklappKnopfZ( Knopf *ausK ) // Ausklapp Knopf setzen
{
    if( ausfahren )
        ausfahren->release();
    ausfahren = ausK;
    rend = 1;
}

void AuswahlBox::setEintragRahmenZ( int i, LRahmen *rahmen ) // Eintrag Rahmen setzen
{
    if( members->z( i ) )
        members->z( i )->setLinienRahmenZ( rahmen );
    else
        rahmen->release();
    rend = 1;
}

void AuswahlBox::setEintragRahmenFarbe( int i, int f ) // Eintrag Rahmen Farbe setzen
{
    if( members->z( i ) )
        members->z( i )->setLinienRahmenFarbe( f );
    rend = 1;
}

void AuswahlBox::setEintragRahmenBreite( int i, int rbr ) // Eintrag Rahmen Breite setzen
{
    if( members->z( i ) )
        members->z( i )->setLinienRahmenBreite( rbr );
    rend = 1;
}

void AuswahlBox::setEintragHintergrundFarbe( int i, int f ) // Eintrag Hintergrund farbe setzen
{
    if( members->z( i ) )
        members->z( i )->setHintergrundFarbe( f );
    rend = 1;
}

void AuswahlBox::setEintragHintergrundBildZ( int i, Bild *bgB ) // Eintrag Hintergrund Bild setzen
{
    if( members->z( i ) )
        members->z( i )->setHintergrundBildZ( bgB );
    else
        bgB->release();
    rend = 1;
}

void AuswahlBox::setEintragHintergrundBild( int i, Bild *bgB )
{
    if( members->z( i ) )
        members->z( i )->setHintergrundBild( bgB );
    else
        bgB->release();
    rend = 1;
}

void AuswahlBox::setEintragAlphaFeldZ( int i, AlphaFeld *af ) // Eintrag AlphaFeld setzen
{
    if( members->z( i ) )
        members->z( i )->setAlphaFeldZ( af );
    rend = 1;
}

void AuswahlBox::setEintragAlphaFeldFarbe( int i, int afF ) // Eintrag AlphaFeld Farbe setzen
{
    if( members->z( i ) )
        members->z( i )->setAlphaFeldFarbe( afF );
    rend = 1;
}

void AuswahlBox::setEintragAlphaFeldStrength( int i, int afSt ) // Eintrag AlphaFeld Stärke setzen
{
    if( members->z( i ) )
        members->z( i )->setAlphaFeldStrength( afSt );
    rend = 1;
}

void AuswahlBox::setAuswRahmenZ( LRahmen *rahmen ) // Auswahl Rahmen setzen
{
    if( auswRahmen )
        auswRahmen->release();
    auswRahmen = rahmen;
    rend = 1;
}

void AuswahlBox::setAuswRahmenFarbe( int f ) // Auswahl Rahmen Farbe setzen
{
    if( !auswRahmen )
        auswRahmen = new LRahmen();
    auswRahmen->setFarbe( f );
    rend = 1;
}

void AuswahlBox::setAuswRahmenBreite( int rbr ) // Auswahl Rahmen Breite setzen
{
    if( !auswRahmen )
        auswRahmen = new LRahmen();
    auswRahmen->setRamenBreite( rbr );
    rend = 1;
}

void AuswahlBox::setAuswHintergrundFarbe( int f ) // Auswahl Hintergrund Farbe setzen
{
    auswBgF = f;
    rend = 1;
}

void AuswahlBox::setAuswHintergrundBildZ( Bild *bgB ) // Auswahl Hintergrund Bild setzen
{
    if( auswBgB )
        auswBgB->release();
    auswBgB = bgB;
    rend = 1;
}

void AuswahlBox::setAuswHintergrundBild( Bild *bgB )
{
    if( !auswBgB )
        auswBgB = new Bild();
    auswBgB->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
    auswBgB->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
    bgB->release();
    rend = 1;
}

void AuswahlBox::setAuswAlphaFeldZ( AlphaFeld *af ) // Auswahl AlphaFeld setzen
{
    if( auswAf )
        auswAf->release();
    auswAf = af;
    rend = 1;
}

void AuswahlBox::setAuswAlphaFeldFarbe( int afF ) // Auswahl AlphaFeld Farbe setzen
{
    if( !auswAf )
        auswAf = new AlphaFeld();
    auswAf->setFarbe( afF );
    rend = 1;
}

void AuswahlBox::setAuswAlphaFeldStrength( int afSt ) // Auswahl Alpha Feld stärke setzen
{
    if( !auswAf )
        auswAf = new AlphaFeld();
    auswAf->setStrength( afSt );
    rend = 1;
}

void AuswahlBox::setMsAuswRahmenZ( int i, LRahmen *rahmen ) // Multistyle Auswahl Rahmen setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        rahmen->release();
        return;
    }
    if( !msAuswRahmen )
        msAuswRahmen = new RCArray< LRahmen >();
    msAuswRahmen->set( rahmen, i );
    rend = 1;
}

void AuswahlBox::setMsAuswRahmenFarbe( int i, int f ) // Multistyle Auswahl Rahmen Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msAuswRahmen )
        msAuswRahmen = new RCArray< LRahmen >();
    if( !msAuswRahmen->z( i ) )
        msAuswRahmen->set( new LRahmen(), i );
    msAuswRahmen->z( i )->setFarbe( f );
    rend = 1;
}

void AuswahlBox::setMsAuswRahmenBreite( int i, int rbr ) // Multistyle Auswahl Breite setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msAuswRahmen )
        msAuswRahmen = new RCArray< LRahmen >();
    if( !msAuswRahmen->z( i ) )
        msAuswRahmen->set( new LRahmen(), i );
    msAuswRahmen->z( i )->setRamenBreite( rbr );
    rend = 1;
}

void AuswahlBox::setMsAuswHintergrundFarbe( int i, int f ) // Multistyle Auswahl Hintergrund Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msAuswBgF )
        msAuswBgF = new Array< int >();
    msAuswBgF->set( f, i );
    rend = 1;
}

void AuswahlBox::setMsAuswHintergrundBildZ( int i, Bild *bgB ) // Multistyle Auswahl Hintergrund Bild setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        bgB->release();
        return;
    }
    if( !msAuswBgB )
        msAuswBgB = new RCArray< Bild >();
    msAuswBgB->set( bgB, i );
    rend = 1;
}

void AuswahlBox::setMsAuswHintergrundBild( int i, Bild *bgB )
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        bgB->release();
        return;
    }
    if( !msAuswBgB )
        msAuswBgB = new RCArray< Bild >();
    if( !msAuswBgB->z( i ) )
    {
        Bild *z = new Bild;
        z->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
        z->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
        msAuswBgB->set( z, i );
    }
    else
    {
        msAuswBgB->z( i )->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
        msAuswBgB->z( i )->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
    }
    bgB->release();
    rend = 1;
}

void AuswahlBox::setMsAuswAlphaFeldZ( int i, AlphaFeld *af ) // Multistyle Auswahl AlphaFeld setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        af->release();
        return;
    }
    if( !msAuswAf )
        msAuswAf = new RCArray< AlphaFeld >();
    msAuswAf->set( af, i );
    rend = 1;
}

void AuswahlBox::setMsAuswAlphaFeldFarbe( int i, int afF ) // Multistyle Auswahl AlphaFeld Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msAuswAf )
        msAuswAf = new RCArray< AlphaFeld >();
    if( !msAuswAf->z( i ) )
        msAuswAf->set( new AlphaFeld(), i );
    msAuswAf->z( i )->setFarbe( afF );
    rend = 1;
}

void AuswahlBox::setMsAuswAlphaFeldStrength( int i, int afSt ) // Multistyle Auswahl AlphaFeld stärke setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msAuswAf )
        msAuswAf = new RCArray< AlphaFeld >();
    if( !msAuswAf->z( i ) )
        msAuswAf->set( new AlphaFeld(), i );
    msAuswAf->z( i )->setStrength( afSt );
    rend = 1;
}

void AuswahlBox::setMausRahmenZ( LRahmen *rahmen ) // Maus Rahmen setzen
{
    if( mausRahmen )
        mausRahmen->release();
    mausRahmen = rahmen;
    rend = 1;
}

void AuswahlBox::setMausRahmenFarbe( int f ) // Maus Rahmen Farbe setzen
{
    if( !mausRahmen )
        mausRahmen = new LRahmen();
    mausRahmen->setFarbe( f );
    rend = 1;
}

void AuswahlBox::setMausRahmenBreite( int rbr ) // Maus Rahmen breite setzen
{
    if( !mausRahmen )
        mausRahmen = new LRahmen();
    mausRahmen->setRamenBreite( rbr );
    rend = 1;
}

void AuswahlBox::setMausHintergrundFarbe( int f ) // Maus Hintergrund Farbe setzen
{
    mausBgF = f;
    rend = 1;
}

void AuswahlBox::setMausHintergrundBildZ( Bild *bgB ) // Maus Hintergrund Bild setzen
{
    if( mausBgB )
        mausBgB->release();
    mausBgB = bgB;
    rend = 1;
}

void AuswahlBox::setMausHintergrundBild( Bild *bgB )
{
    if( !mausBgB )
        mausBgB = new Bild();
    mausBgB->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
    mausBgB->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
    bgB->release();
    rend = 1;
}

void AuswahlBox::setMausAlphaFeldZ( AlphaFeld *af ) // Maus AlphaFeld setzen
{
    if( mausAf )
        mausAf->release();
    mausAf = af;
    rend = 1;
}

void AuswahlBox::setMausAlphaFeldFarbe( int afF ) // Maus AlphaFeld Farbe setzen
{
    if( !mausAf )
        mausAf = new AlphaFeld();
    mausAf->setFarbe( afF );
    rend = 1;
}

void AuswahlBox::setMausAlphaFeldStrength( int afSt ) // Maus AlphaFeld stärke setzen
{
    if( !mausAf )
        mausAf = new AlphaFeld();
    mausAf->setStrength( afSt );
    rend = 1;
}

void AuswahlBox::setMsMausRahmenZ( int i, LRahmen *rahmen ) // Multistyle Maus Rahmen setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        rahmen->release();
        return;
    }
    if( !msMausRahmen )
        msMausRahmen = new RCArray< LRahmen >();
    msMausRahmen->set( rahmen, i );
    rend = 1;
}

void AuswahlBox::setMsMausRahmenFarbe( int i, int f ) // Multistyle Maus Rahmen Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msMausRahmen )
        msMausRahmen = new RCArray< LRahmen >();
    if( !msMausRahmen->z( i ) )
        msMausRahmen->set( new LRahmen(), i );
    msMausRahmen->z( i )->setFarbe( f );
    rend = 1;
}

void AuswahlBox::setMsMausRahmenBreite( int i, int rbr ) // Multistyle Maus Rahmen breite setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msMausRahmen )
        msMausRahmen = new RCArray< LRahmen >();
    if( !msMausRahmen->z( i ) )
        msMausRahmen->set( new LRahmen(), i );
    msMausRahmen->z( i )->setRamenBreite( rbr );
    rend = 1;
}

void AuswahlBox::setMsMausHintergrundFarbe( int i, int f ) // Multistyle Maus Hintergrund Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msMausBgF )
        msMausBgF = new Array< int >();
    msMausBgF->set( f, i );
    rend = 1;
}

void AuswahlBox::setMsMausHintergrundBildZ( int i, Bild *bgB ) // Multistyle Maus Hintergrund Bild setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        bgB->release();
        return;
    }
    if( !msMausBgB )
        msMausBgB = new RCArray< Bild >();
    msMausBgB->set( bgB, i );
    rend = 1;
}

void AuswahlBox::setMsMausHintergrundBild( int i, Bild *bgB )
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        bgB->release();
        return;
    }
    if( !msMausBgB )
        msMausBgB = new RCArray< Bild >();
    if( !msMausBgB->z( i ) )
    {
        Bild *z = new Bild;
        z->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
        z->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
        msMausBgB->set( z, i );
    }
    else
    {
        msMausBgB->z( i )->neuBild( bgB->getBreite(), bgB->getHeight(), 0 );
        msMausBgB->z( i )->drawBild( 0, 0, bgB->getBreite(), bgB->getHeight(), *bgB );
    }
    bgB->release();
    rend = 1;
}

void AuswahlBox::setMsMausAlphaFeldZ( int i, AlphaFeld *af ) // Multistyle Maus AlphaFeld setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
    {
        af->release();
        return;
    }
    if( !msMausAf )
        msMausAf = new RCArray< AlphaFeld >();
    msMausAf->set( af, i );
    rend = 1;
}

void AuswahlBox::setMsMausAlphaFeldFarbe( int i, int afF ) // Multistyle Maus AlphaFeld Farbe setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msMausAf )
        msMausAf = new RCArray< AlphaFeld >();
    if( !msMausAf->z( i ) )
        msMausAf->set( new AlphaFeld(), i );
    msMausAf->z( i )->setFarbe( afF );
    rend = 1;
}

void AuswahlBox::setMsMausAlphaFeldStrength( int i, int afSt ) // Multistyle Maus AlphaFeld stärke setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msMausAf )
        msMausAf = new RCArray< AlphaFeld >();
    if( !msMausAf->z( i ) )
        msMausAf->set( new AlphaFeld(), i );
    msMausAf->z( i )->setStrength( afSt );
    rend = 1;
}

void AuswahlBox::setAuswahl( int i ) // Eintrag auswählen
{
    if( i < anzahl )
    {
        auswahl = i;
        if( eAk )
            eAk( eAkP, this, 0, auswahl );
        rend = 1;
    }
}

void AuswahlBox::ausklappen() // liste ausklappen
{
    ausgeklappt = 1;
}

void AuswahlBox::einklappen() // liste einklappen
{
    ausgeklappt = 0;
}

void AuswahlBox::scrollZuEintrag( int i ) // liste scrollen
{
    if( hatStyle( Style::VScroll ) && vertikalScrollBar && i < anzahl )
    {
        int scrollPos = 0;
        if( hatStyle( Style::MultiStyled ) )
        {
            for( int j = 0; j < i; ++j )
                scrollPos += members->z( j ) ? members->z( j )->getHeight() : 0;
        }
        else
            scrollPos += i * eintragHeight;
        vertikalScrollBar->scroll( scrollPos );
        rend = 1;
    }
}

void AuswahlBox::setMaxAuskappHeight( int maxHeight ) // höhe der Liste beim ausklappen
{
    ausklapMaxHeight = maxHeight;
}

void AuswahlBox::setEintragHeight( int height ) // setzt die Höhe der Einträge
{
    eintragHeight = height;
}

void AuswahlBox::addMsStyle( int i, __int64 abStyle ) // Multistyle style hinzufügen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msStyle )
        msStyle = new Array< __int64 >();
    msStyle->set( msStyle->get( i ) | abStyle, i );
    rend = 1;
}

void AuswahlBox::setMsStyle( int i, __int64 abStyle, bool add ) // Multistyle style setzen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msStyle )
        msStyle = new Array< __int64 >();
    if( add )
        msStyle->set( msStyle->get( i ) | abStyle, i );
    else
        msStyle->set( msStyle->get( i ) & ~abStyle, i );
    rend = 1;
}

void AuswahlBox::setMsStyle( int i, __int64 abStyle )
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msStyle )
        msStyle = new Array< __int64 >();
    msStyle->set( abStyle, i );
    rend = 1;
}

void AuswahlBox::removeMsStyle( int i, __int64 abStyle ) // Multistyle style entfernen
{
    if( hatStyleNicht( Style::MultiStyled ) || i >= anzahl )
        return;
    if( !msStyle )
        msStyle = new Array< __int64 >();
    msStyle->set( msStyle->get( i ) & ~abStyle, i );
    rend = 1;
}

bool AuswahlBox::tick( double tickVal ) // tick
{
    if( ausgeklappt && ausfahren && !ausfahren->zText()->istGleich( "/\\" ) )
    {
        ausfahren->setText( "/\\" );
        rend = 1;
    }
    else if( !ausgeklappt && ausfahren && !ausfahren->zText()->istGleich( "\\/" ) )
    {
        ausfahren->setText( "\\/" );
        rend = 1;
    }
    if( hatStyleNicht( Style::Sichtbar ) || hatStyleNicht( Style::Erlaubt ) )
        ausgeklappt = 0;
    rend |= ausfahren->tick( tickVal );
    this->tickval += tickVal * 300;
    int val = ( int )this->tickval;
    if( val < 1 )
        return ZeichnungHintergrund::tick( tickVal );
    this->tickval -= val;
    int maxHeight = rahmen ? rahmen->getRBreite() : 0;
    if( hatStyleNicht( Style::MultiStyled ) )
        maxHeight += anzahl * eintragHeight;
    else
        for( int i = 0; i < anzahl; ++i )
            maxHeight += members->z( i ) ? members->z( i )->getHeight() : 0;
    if( maxHeight > ausklapMaxHeight )
    {
        if( hatStyle( Style::VScroll ) && vertikalScrollBar )
        {
            scrollAnzeigen = 1;
            vertikalScrollBar->update( maxHeight, ausklapMaxHeight );
        }
        maxHeight = ausklapMaxHeight;
    }
    else
        scrollAnzeigen = 0;
    if( ausgeklappt )
    {
        if( ausklappHeight < maxHeight )
        {
            ausklappHeight += val;
            if( ausklappHeight > maxHeight )
                ausklappHeight = maxHeight;
            rend = 1;
        }
    }
    else
    {
        if( ausklappHeight > 0 )
        {
            ausklappHeight -= val;
            if( ausklappHeight < 0 )
                ausklappHeight = 0;
            rend = 1;
        }
    }
    for( int i = 0; i < anzahl; ++i )
    {
        if( i != auswahl )
            rend |= members->z( i )->tick( tickVal );
        else
            members->z( i )->tick( tickVal );
    }
    return ZeichnungHintergrund::tick( tickVal );
}

void AuswahlBox::doMausEreignis( MausEreignis &me ) // Maus
{
    mausEintrag = -1;
    if( hatStyleNicht( Style::Sichtbar ) || hatStyleNicht( Style::Erlaubt ) )
    {
        if( toolTip )
            toolTip->setMausIn( 0 );
        ausgeklappt = 0;
        return;
    }
    bool removeFokus = 0;
    bool nmakc = me.verarbeitet == 0;
    if( me.verarbeitet || !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y + ausklappHeight ) )
    {
        if( mausIn )
        {
            if( toolTip )
                toolTip->setMausIn( 0 );
            mausIn = 0;
            MausEreignis me2;
            me2.id = ME_Leaves;
            me2.mx = me.mx;
            me2.my = me.my;
            me2.verarbeitet = 0;
            doMausEreignis( me2 );
            return;
        }
        removeFokus = 1;
    }
    if( !( me.mx >= pos.x && me.mx <= pos.x + gr.x && me.my >= pos.y && me.my <= pos.y + gr.y + ausklappHeight ) && me.id != ME_Leaves )
    {
        if( removeFokus && me.id == ME_RLinks )
        {
            if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
                removeStyle( Style::Fokus );
            ausgeklappt = 0;
            me.mx -= pos.x, me.my -= pos.y;
            if( nmakc && me.verarbeitet && nMak )
                me.verarbeitet = nMak( nmakParam, this, me );
            me.mx += pos.x, me.my += pos.y;
        }
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
    me.mx -= pos.x, me.my -= pos.y;
    if( Mak && ( me.verarbeitet || Mak( makParam, this, me ) ) )
    {
        bool vera = me.verarbeitet;
        if( ausfahren )
        {
            int tmpMx = me.mx;
            int tmpMy = me.my;
            if( me.mx > 0 && me.my > 0 && me.mx < gr.x && me.my < gr.y )
                me.mx = ausfahren->getX() + 1, me.my = ausfahren->getY() + 1;
            else
                me.mx = ausfahren->getX() - 1, me.my = ausfahren->getY() - 1;
            ausfahren->doMausEreignis( me );
            me.mx = tmpMx, me.my = tmpMy;
        }
        if( me.verarbeitet && !vera && me.id == ME_RLinks )
        {
            ausgeklappt = !ausgeklappt;
            if( ausgeklappt )
                mausEintrag = auswahl;
            if( scrollAnzeigen )
                scrollZuEintrag( mausEintrag );
        }
        if( removeFokus && me.id == ME_RLinks )
            removeStyle( Style::Fokus );
        if( !me.verarbeitet && hatStyleNicht( Style::Fokus ) && me.id == ME_RLinks )
            addStyle( Style::Fokus );
        if( hatStyle( Style::VScroll ) && vertikalScrollBar && ausgeklappt && scrollAnzeigen )
        {
            int rbr = 0;
            if( rahmen && hatStyle( Style::Rahmen ) )
                rbr = rahmen->getRBreite();
            if( ( ( me.mx > gr.x - 15 - rbr && me.my > gr.y ) || me.id == ME_UScroll || me.id == ME_DScroll ) && me.id != ME_Betritt && me.id != ME_Leaves )
            {
                vertikalScrollBar->doMausMessage( gr.x - rbr - 15, gr.y, 15, vertikalScrollBar->getScrollData()->anzeige, me );
                me.verarbeitet = 1;
            }
        }
        if( !me.verarbeitet )
        {
            int eintr = -1;
            int tmp = me.my - gr.y + ( vertikalScrollBar ? vertikalScrollBar->getScroll() : 0 );
            if( hatStyle( Style::MultiStyled ) )
            {
                for( int i = 0; i < anzahl; ++i )
                {
                    if( tmp > 0 && tmp < ( members->z( i ) ? members->z( i )->getHeight() : 0 ) )
                    {
                        eintr = i;
                        break;
                    }
                    tmp -= members->z( i ) ? members->z( i )->getHeight() : 0;
                }
            }
            else
            {
                for( int i = 0; i < anzahl; ++i )
                {
                    if( tmp > 0 && tmp < eintragHeight )
                    {
                        eintr = i;
                        break;
                    }
                    tmp -= eintragHeight;
                }
            }
            if( ausgeklappt && me.mx > 0 && me.mx < gr.x && me.my > gr.y && me.my < gr.y + ausklappHeight )
            {
                if( eintr >= 0 )
                {
                    if( me.id == ME_RLinks )
                    {
                        if( auswahl != eintr )
                            rend = 1;
                        auswahl = eintr;
                        if( eAk )
                            eAk( eAkP, this, 0, auswahl );
                    }
                    if( mausEintrag != eintr )
                        rend = 1;
                    mausEintrag = eintr;
                }
            }
        }
        me.verarbeitet = 1;
    }
    if( nmakc && me.verarbeitet && nMak )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
}

void AuswahlBox::doTastaturEreignis( TastaturEreignis &te ) // Tastatur
{
    if( te.verarbeitet || hatStyleNicht( Style::Fokus ) || hatStyleNicht( Style::Erlaubt ) )
        return;
    if( te.id == TE_Release )
    {
        switch( te.taste )
        {
        case T_Oben:
            if( auswahl > 0 )
                --auswahl;
            else
                auswahl = anzahl - 1;
            if( eAk )
                eAk( eAkP, this, 0, auswahl );
            scrollZuEintrag( auswahl );
            rend = 1;
            break;
        case T_Unten:
            if( auswahl < anzahl - 1 )
                ++auswahl;
            else
                auswahl = 0;
            if( eAk )
                eAk( eAkP, this, 0, auswahl );
            scrollZuEintrag( auswahl );
            rend = 1;
            break;
        }
    }
    if( te.verarbeitet && nTak )
        te.verarbeitet = nTak( ntakParam, this, te );
}

void AuswahlBox::render( Bild &zRObj ) // zeichnet nach zRObj
{
    if( hatStyle( Style::Sichtbar ) )
    {
        lockZeichnung();
        int br = gr.x;
        int hi = gr.y + ausklappHeight;
        if( ( ausklappHeight && !zRObj.setDrawOptionsErzwingen( pos.x, pos.y, br, hi ) ) || ( !ausklappHeight && !zRObj.setDrawOptions( pos.x, pos.y, br, hi ) ) )
        {
            unlockZeichnung();
            return;
        }
        int rbr = 0;
        if( hatStyle( Style::Rahmen ) && rahmen ) // Rahmen zeichnen
        {
            rahmen->setSize( br, hi );
            rahmen->render( zRObj );
            rbr = rahmen->getRBreite();
        }
        if( ( ausklappHeight && !zRObj.setDrawOptionsErzwingen( rbr, rbr, br - rbr * 2, hi - rbr * 2 ) ) || ( !ausklappHeight && !zRObj.setDrawOptions( rbr, rbr, br - rbr * 2, hi - rbr * 2 ) ) )
        {
            zRObj.releaseDrawOptions();
            unlockZeichnung();
            return;
        }
        if( hatStyle( Style::Hintergrund ) )
        {
            if( hatStyle( Style::HAlpha ) )
                zRObj.alphaRegion( 0, 0, br, hi, hintergrundFarbe );
            else
                zRObj.fillRegion( 0, 0, br, hi, hintergrundFarbe );
            if( hatStyle( Style::HBild ) && hintergrundBild )
            {
                if( hatStyle( Style::HAlpha ) )
                    zRObj.alphaBild( 0, 0, br, hi, *hintergrundBild );
                else
                    zRObj.drawBild( 0, 0, br, hi, *hintergrundBild );
            }
        }
        if( hatStyle( Style::Buffered ) && hintergrundFeld )
        {
            hintergrundFeld->setSize( br - rbr * 2, hi - rbr * 2 );
            hintergrundFeld->render( zRObj );
        }
        if( ausfahren ) // Ausklapp Knopf zeichnen
        {
            ausfahren->setSize( gr.y - rbr * 2, gr.y - rbr * 2 );
            ausfahren->setPosition( gr.x - rbr - ausfahren->getBreite(), rbr );
            ausfahren->render( zRObj );
        }
        if( members ) // Ausgewähtes TextFeld zeichnen
        {
            if( auswahl < 0 )
            {
                auswahl = 0;
                if( eAk )
                    eAk( eAkP, this, 0, auswahl );
            }
            if( auswahl >= anzahl )
            {
                auswahl = anzahl - 1;
                if( eAk )
                    eAk( eAkP, this, 0, auswahl );
            }
            TextFeld *tf = auswahl >= 0 ? members->z( auswahl ) : 0;
            if( tf )
            {
                AlphaFeld *tmpBuffer = 0;
                bool tmpB = 0;
                int tmpHFarbe = 0;
                bool tmpH = 0;
                Bild *tmpHBild = 0;
                bool tmpHB = 0;
                bool tmpHAlpha = 0;
                LRahmen *tmpRahmen = 0;
                bool tmpR = 0;
                if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
                {
                    if( hatStyle( Style::AuswahlBuffer ) )
                    {
                        tmpBuffer = tf->getAlphaFeld();
                        tf->setAlphaFeldZ( auswAf->getThis() );
                        tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                        tf->setStyle( TextFeld::Style::Buffered, hatStyle( Style::AuswahlBuffer ) );
                    }
                    if( hatStyle( Style::AuswahlHintergrund ) )
                    {
                        tmpH = tf->hatStyle( TextFeld::Style::Hintergrund );
                        tmpHFarbe = tf->getHintergrundFarbe();
                        tf->setHintergrundFarbe( auswBgF );
                        tf->setStyle( TextFeld::Style::Hintergrund, hatStyle( Style::Hintergrund ) );
                        if( hatStyle( Style::AuswahlHBild ) )
                        {
                            tmpHBild = tf->getHintergrundBild();
                            tf->setHintergrundBildZ( auswBgB->getThis() );
                            tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                            tf->setStyle( TextFeld::Style::HBild, hatStyle( Style::HBild ) );
                        }
                        if( hatStyle( Style::AuswahlHAlpha ) )
                        {
                            tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                            tf->setStyle( TextFeld::Style::HAlpha, hatStyle( Style::AuswahlHAlpha ) );
                        }
                    }
                    if( hatStyle( Style::AuswahlRahmen ) )
                    {
                        tmpRahmen = tf->getLinienRahmen();
                        tf->setLinienRahmenZ( auswRahmen->getThis() );
                        tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, hatStyle( Style::AuswahlRahmen ) );
                    }
                }
                else
                {
                    if( hatMsStyle( auswahl, Style::AuswahlBuffer ) && msAuswAf )
                    {
                        tmpBuffer = tf->getAlphaFeld();
                        tf->setAlphaFeldZ( msAuswAf->get( auswahl ) );
                        tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                        tf->setStyle( TextFeld::Style::Buffered, hatMsStyle( auswahl, Style::AuswahlBuffer ) );
                    }
                    if( hatMsStyle( auswahl, Style::AuswahlHintergrund ) )
                    {
                        tmpH = tf->hatStyle( Style::Hintergrund );
                        tf->setStyle( TextFeld::Style::Hintergrund, hatMsStyle( auswahl, Style::AuswahlHintergrund ) );
                        if( msAuswBgF && msAuswBgF->hat( auswahl ) )
                        {
                            tmpHFarbe = tf->getHintergrundFarbe();
                            tf->setHintergrundFarbe( msAuswBgF->get( auswahl ) );
                        }
                        if( hatMsStyle( auswahl, Style::AuswahlHBild ) && msAuswBgB )
                        {
                            tmpHBild = tf->getHintergrundBild();
                            tf->setHintergrundBildZ( msAuswBgB->get( auswahl ) );
                            tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                            tf->setStyle( TextFeld::Style::HBild, hatMsStyle( auswahl, Style::HBild ) );
                        }
                        if( hatMsStyle( auswahl, Style::AuswahlHAlpha ) )
                        {
                            tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                            tf->setStyle( TextFeld::Style::HAlpha, hatMsStyle( auswahl, Style::AuswahlHAlpha ) );
                        }
                    }
                    if( hatMsStyle( auswahl, Style::AuswahlRahmen ) && msAuswRahmen )
                    {
                        tmpRahmen = tf->getLinienRahmen();
                        tf->setLinienRahmenZ( msAuswRahmen->get( auswahl ) );
                        tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, hatMsStyle( auswahl, Style::AuswahlRahmen ) );
                    }
                }
                int tmpHi = tf->getHeight();
                tf->setPosition( 0, 0 );
                tf->setSize( gr.x - rbr * 2 - ( ausfahren ? ausfahren->getBreite() : 0 ), gr.y - rbr * 2 );
                tf->render( zRObj );
                tf->setSize( tf->getBreite(), tmpHi );
                if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
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
                    if( hatMsStyle( auswahl, Style::AuswahlBuffer ) && msAuswAf )
                    {
                        tf->setAlphaFeldZ( tmpBuffer );
                        tf->setStyle( TextFeld::Style::Buffered, tmpB );
                    }
                    if( hatMsStyle( auswahl, Style::AuswahlHintergrund ) )
                    {
                        tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                        if( msAuswBgF && msAuswBgF->hat( auswahl ) )
                            tf->setHintergrundFarbe( tmpHFarbe );
                        if( hatMsStyle( auswahl, Style::AuswahlHBild ) && msAuswBgB )
                        {
                            tf->setHintergrundBildZ( tmpHBild );
                            tf->setStyle( TextFeld::Style::HBild, tmpHB );
                        }
                        if( hatMsStyle( auswahl, Style::AuswahlHAlpha ) )
                            tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                    }
                    if( hatMsStyle( auswahl, Style::AuswahlRahmen ) && msAuswRahmen )
                    {
                        tf->setLinienRahmenZ( tmpRahmen );
                        tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                    }
                }
            }
        }
        bool vsb = hatStyle( Style::VScroll ) && vertikalScrollBar; // Scroll bar zeichnen
        if( auswahl >= anzahl )
        {
            auswahl = 0;
            if( eAk )
                eAk( eAkP, this, 0, auswahl );
        }
        if( members )
        {
            if( vsb && ausklappHeight )
            {
                br -= 15;
                vertikalScrollBar->getScrollData()->anzeige = ausklappHeight - rbr;
                vertikalScrollBar->render( br - rbr, gr.y, 15, ausklappHeight - rbr, zRObj );
            }
            if( ( ausklappHeight && !zRObj.setDrawOptionsErzwingen( 0, gr.y, br - rbr, hi - rbr - gr.y ) ) || ( !ausklappHeight && !zRObj.setDrawOptions( 0, gr.y, br - rbr, hi - rbr - gr.y ) ) )
            {
                zRObj.releaseDrawOptions();
                zRObj.releaseDrawOptions();
                unlockZeichnung();
                return;
            }
            int maxHeight = 0;
            int dy = 0;
            if( vsb )
                dy -= vertikalScrollBar->getScroll();
            int mdy = hi - rbr;
            anzahl = members->getEintragAnzahl();
            for( int i = 0; i < anzahl; ++i )
            {
                TextFeld *tf = members->z( i );
                if( dy >= mdy && !vsb )
                    break;
                tf->setPosition( 0, dy );
                tf->setSize( br - rbr * 2, tf->getHeight() );
                maxHeight += tf->getHeight();
                bool selected = auswahl == i;
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
                    if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
                    {
                        if( hatStyle( Style::AuswahlBuffer ) )
                        {
                            tmpBuffer = tf->getAlphaFeld();
                            tf->setAlphaFeldZ( auswAf->getThis() );
                            tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                            tf->setStyle( TextFeld::Style::Buffered, hatStyle( Style::AuswahlBuffer ) );
                        }
                        if( hatStyle( Style::AuswahlHintergrund ) )
                        {
                            tmpH = tf->hatStyle( TextFeld::Style::Hintergrund );
                            tmpHFarbe = tf->getHintergrundFarbe();
                            tf->setHintergrundFarbe( auswBgF );
                            tf->setStyle( TextFeld::Style::Hintergrund, hatStyle( Style::Hintergrund ) );
                            if( hatStyle( Style::AuswahlHBild ) )
                            {
                                tmpHBild = tf->getHintergrundBild();
                                tf->setHintergrundBildZ( auswBgB->getThis() );
                                tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                                tf->setStyle( TextFeld::Style::HBild, hatStyle( Style::HBild ) );
                            }
                            if( hatStyle( Style::AuswahlHAlpha ) )
                            {
                                tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                                tf->setStyle( TextFeld::Style::HAlpha, hatStyle( Style::AuswahlHAlpha ) );
                            }
                        }
                        if( hatStyle( Style::AuswahlRahmen ) )
                        {
                            tmpRahmen = tf->getLinienRahmen();
                            tf->setLinienRahmenZ( auswRahmen->getThis() );
                            tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, hatStyle( Style::AuswahlRahmen ) );
                        }
                    }
                    else
                    {
                        if( hatMsStyle( i, Style::AuswahlBuffer ) && msAuswAf )
                        {
                            tmpBuffer = tf->getAlphaFeld();
                            tf->setAlphaFeldZ( msAuswAf->get( i ) );
                            tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                            tf->setStyle( TextFeld::Style::Buffered, hatMsStyle( i, Style::AuswahlBuffer ) );
                        }
                        if( hatMsStyle( i, Style::AuswahlHintergrund ) )
                        {
                            tmpH = tf->hatStyle( Style::Hintergrund );
                            tf->setStyle( TextFeld::Style::Hintergrund, hatMsStyle( i, Style::AuswahlHintergrund ) );
                            if( msAuswBgF && msAuswBgF->hat( i ) )
                            {
                                tmpHFarbe = tf->getHintergrundFarbe();
                                tf->setHintergrundFarbe( msAuswBgF->get( i ) );
                            }
                            if( hatMsStyle( i, Style::AuswahlHBild ) && msAuswBgB )
                            {
                                tmpHBild = tf->getHintergrundBild();
                                tf->setHintergrundBildZ( msAuswBgB->get( i ) );
                                tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                                tf->setStyle( TextFeld::Style::HBild, hatMsStyle( i, Style::HBild ) );
                            }
                            if( hatMsStyle( i, Style::AuswahlHAlpha ) )
                            {
                                tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                                tf->setStyle( TextFeld::Style::HAlpha, hatMsStyle( i, Style::AuswahlHAlpha ) );
                            }
                        }
                        if( hatMsStyle( i, Style::AuswahlRahmen ) && msAuswRahmen )
                        {
                            tmpRahmen = tf->getLinienRahmen();
                            tf->setLinienRahmenZ( msAuswRahmen->get( i ) );
                            tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, hatMsStyle( i, Style::AuswahlRahmen ) );
                        }
                    }
                }
                else if( mausEintrag == i )
                {
                    if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
                    {
                        if( hatStyle( Style::MausBuffer ) )
                        {
                            tmpBuffer = tf->getAlphaFeld();
                            tf->setAlphaFeldZ( mausAf->getThis() );
                            tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                            tf->setStyle( TextFeld::Style::Buffered, hatStyle( Style::MausBuffer ) );
                        }
                        if( hatStyle( Style::MausHintergrund ) )
                        {
                            tmpH = tf->hatStyle( TextFeld::Style::Hintergrund );
                            tmpHFarbe = tf->getHintergrundFarbe();
                            tf->setHintergrundFarbe( mausBgF );
                            tf->setStyle( TextFeld::Style::Hintergrund, hatStyle( Style::Hintergrund ) );
                            if( hatStyle( Style::MausHBild ) )
                            {
                                tmpHBild = tf->getHintergrundBild();
                                tf->setHintergrundBildZ( mausBgB->getThis() );
                                tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                                tf->setStyle( TextFeld::Style::HBild, hatStyle( Style::HBild ) );
                            }
                            if( hatStyle( Style::MausHAlpha ) )
                            {
                                tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                                tf->setStyle( TextFeld::Style::HAlpha, hatStyle( Style::MausHAlpha ) );
                            }
                        }
                        if( hatStyle( Style::MausRahmen ) )
                        {
                            tmpRahmen = tf->getLinienRahmen();
                            tf->setLinienRahmenZ( mausRahmen->getThis() );
                            tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, hatStyle( Style::MausRahmen ) );
                        }
                    }
                    else
                    {
                        if( hatMsStyle( i, Style::MausBuffer ) && msAuswAf )
                        {
                            tmpBuffer = tf->getAlphaFeld();
                            tf->setAlphaFeldZ( msMausAf->get( i ) );
                            tmpB = tf->hatStyle( TextFeld::Style::Buffered );
                            tf->setStyle( TextFeld::Style::Buffered, hatMsStyle( i, Style::MausBuffer ) );
                        }
                        if( hatMsStyle( i, Style::MausHintergrund ) )
                        {
                            tmpH = tf->hatStyle( Style::Hintergrund );
                            tf->setStyle( TextFeld::Style::Hintergrund, hatMsStyle( i, Style::MausHintergrund ) );
                            if( msMausBgF && msMausBgF->hat( i ) )
                            {
                                tmpHFarbe = tf->getHintergrundFarbe();
                                tf->setHintergrundFarbe( msMausBgF->get( i ) );
                            }
                            if( hatMsStyle( i, Style::MausHBild ) && msMausBgB )
                            {
                                tmpHBild = tf->getHintergrundBild();
                                tf->setHintergrundBildZ( msMausBgB->get( i ) );
                                tmpHB = tf->hatStyle( TextFeld::Style::HBild );
                                tf->setStyle( TextFeld::Style::HBild, hatMsStyle( i, Style::HBild ) );
                            }
                            if( hatMsStyle( i, Style::MausHAlpha ) )
                            {
                                tmpHAlpha = tf->hatStyle( TextFeld::Style::HAlpha );
                                tf->setStyle( TextFeld::Style::HAlpha, hatMsStyle( i, Style::MausHAlpha ) );
                            }
                        }
                        if( hatMsStyle( i, Style::MausRahmen ) && msMausRahmen )
                        {
                            tmpRahmen = tf->getLinienRahmen();
                            tf->setLinienRahmenZ( msMausRahmen->get( i ) );
                            tmpR = tf->hatStyle( TextFeld::Style::Rahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, hatMsStyle( i, Style::MausRahmen ) );
                        }
                    }
                }
                tf->render( zRObj );
                if( selected )
                {
                    if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
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
                        if( hatMsStyle( i, Style::AuswahlBuffer ) && msAuswAf )
                        {
                            tf->setAlphaFeldZ( tmpBuffer );
                            tf->setStyle( TextFeld::Style::Buffered, tmpB );
                        }
                        if( hatMsStyle( i, Style::AuswahlHintergrund ) )
                        {
                            tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                            if( msAuswBgF && msAuswBgF->hat( i ) )
                                tf->setHintergrundFarbe( tmpHFarbe );
                            if( hatMsStyle( i, Style::AuswahlHBild ) && msAuswBgB )
                            {
                                tf->setHintergrundBildZ( tmpHBild );
                                tf->setStyle( TextFeld::Style::HBild, tmpHB );
                            }
                            if( hatMsStyle( i, Style::AuswahlHAlpha ) )
                                tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                        }
                        if( hatMsStyle( i, Style::AuswahlRahmen ) && msAuswRahmen )
                        {
                            tf->setLinienRahmenZ( tmpRahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                        }
                    }
                }
                else if( mausEintrag == i )
                {
                    if( hatStyleNicht( Style::MultiStyled ) || !msStyle )
                    {
                        if( hatStyle( Style::MausBuffer ) )
                        {
                            tf->setAlphaFeldZ( tmpBuffer );
                            tf->setStyle( TextFeld::Style::Buffered, tmpB );
                        }
                        if( hatStyle( Style::MausHintergrund ) )
                        {
                            tf->setHintergrundFarbe( tmpHFarbe );
                            tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                            if( hatStyle( Style::MausHBild ) )
                            {
                                tf->setHintergrundBildZ( tmpHBild );
                                tf->setStyle( TextFeld::Style::HBild, tmpHB );
                            }
                            if( hatStyle( Style::MausHAlpha ) )
                                tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                        }
                        if( hatStyle( Style::MausRahmen ) )
                        {
                            tf->setLinienRahmenZ( tmpRahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                        }
                    }
                    else
                    {
                        if( hatMsStyle( i, Style::MausBuffer ) && msAuswAf )
                        {
                            tf->setAlphaFeldZ( tmpBuffer );
                            tf->setStyle( TextFeld::Style::Buffered, tmpB );
                        }
                        if( hatMsStyle( i, Style::MausHintergrund ) )
                        {
                            tf->setStyle( TextFeld::Style::Hintergrund, tmpH );
                            if( msAuswBgF && msAuswBgF->hat( i ) )
                                tf->setHintergrundFarbe( tmpHFarbe );
                            if( hatMsStyle( i, Style::MausHBild ) && msAuswBgB )
                            {
                                tf->setHintergrundBildZ( tmpHBild );
                                tf->setStyle( TextFeld::Style::HBild, tmpHB );
                            }
                            if( hatMsStyle( i, Style::MausHAlpha ) )
                                tf->setStyle( TextFeld::Style::HAlpha, tmpHAlpha );
                        }
                        if( hatMsStyle( i, Style::MausRahmen ) && msAuswRahmen )
                        {
                            tf->setLinienRahmenZ( tmpRahmen );
                            tf->setStyle( TextFeld::Style::Rahmen, tmpR );
                        }
                    }
                }
                dy += tf->getHeight();
            }
            if( vertikalScrollBar )
                vertikalScrollBar->getScrollData()->max = maxHeight;
            zRObj.releaseDrawOptions();
        }
        zRObj.releaseDrawOptions();
        zRObj.releaseDrawOptions();
        unlockZeichnung();
    }
}

// constant
int AuswahlBox::getEintragPos( const char *txt ) const // gibt die Eintrag Position zurück
{
    for( int i = 0; i < anzahl; ++i )
        if( members->z( i ) && members->z( i )->zText()->istGleich( txt ) )
            return i;
    return -1;
}

int AuswahlBox::getEintragPos( Text *txt ) const
{
    for( int i = 0; i < anzahl; ++i )
    {
        if( members->z( i ) && members->z( i )->zText()->istGleich( txt->getText() ) )
        {
            txt->release();
            return i;
        }
    }
    txt->release();
    return -1;
}

Text *AuswahlBox::getEintragText( int i ) const // gibt den Eintrag Text zurück
{
    if( i >= anzahl )
        return 0;
    return members->z( i ) ? members->z( i )->getText() : 0;
}

Text *AuswahlBox::zEintragText( int i ) const
{
    if( i >= anzahl )
        return 0;
    return members->z( i ) ? members->z( i )->zText() : 0;
}

TextFeld *AuswahlBox::getEintrag( int i ) const // gibt den Eintrag zurück
{
    if( i >= anzahl )
        return 0;
    return members->get( i );
}

TextFeld *AuswahlBox::zEintrag( int i ) const
{
    if( i >= anzahl )
        return 0;
    return members->z( i );
}

int AuswahlBox::getAuswahl() const // gibt die Position des ausgewählten Eintrages zurück
{
    return auswahl;
}

int AuswahlBox::getEintragAnzahl() const // gibt die Anzahl der Einträge zurück
{
    return anzahl;
}

bool AuswahlBox::istAusgeklappt() const // prüft, ob die Liste ausgeklappt ist
{
    return ausgeklappt;
}

int AuswahlBox::getMaxHeight() const // gibt die maximale Höhe der Liste zurück
{
    if( !hatStyle( Style::MaxHeight ) )
        return 0;
    return ausklapMaxHeight;
}

int AuswahlBox::getEintragHeight() const // gibt die Höhe der Einträge zurück
{
    return eintragHeight;
}

Knopf *AuswahlBox::getAusklappKnopf() const // gibt den aus-/einklapp Knopf zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return ausfahren ? ausfahren->getThis() : 0;
}

Knopf *AuswahlBox::zAusklappKnopf() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return ausfahren;
}

LRahmen *AuswahlBox::getEintragRahmen( int i ) const // gibt den Eintrag Rahmen zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getLinienRahmen();
}

LRahmen *AuswahlBox::zEintragRahmen( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->zLinienRahmen();
}

int AuswahlBox::getEintragRahmenFarbe( int i ) const // gibt die Eintrag Rahmen Frabe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getLinienRahmenFarbe();
}

int AuswahlBox::getEintragRahmenBreite( int i ) const // gibt die Eintrag Rahmen Breite zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getLinienRahmenBreite();
}

AlphaFeld *AuswahlBox::getEintragAlphaFeld( int i ) const // gibt das Eintrag AlphaFeld zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getAlphaFeld();
}

AlphaFeld *AuswahlBox::zEintragAlphaFeld( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->zAlphaFeld();
}

int AuswahlBox::getEintragAlphaFeldFarbe( int i ) const // gibt die Eintrag AlphaFeld Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getAlphaFeldFarbe();
}

int AuswahlBox::getEintragAlphaFeldStrength( int i ) const // gibt die Eintrag AlphaFeld stärke zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getAlphaFeldStrength();
}

int AuswahlBox::getEintragHintergrundFarbe( int i ) const // gibt die Eintrag Hintergrund Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getHintergrundFarbe();
}

Bild *AuswahlBox::getEintragHintergrundBild( int i ) const // gibt das Eintrag Hintergrund Bild zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->getHintergrundBild();
}

Bild *AuswahlBox::zEintragHintergrundBild( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !members->z( i ) )
        return 0;
    return members->z( i )->zHintergrundBild();
}

LRahmen *AuswahlBox::getAuswRahmen() const // gibt den Auswahl Rahmen zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswRahmen ? auswRahmen->getThis() : 0;
}

LRahmen *AuswahlBox::zAuswRahmen() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswRahmen;
}

int AuswahlBox::getAuswRahmenFarbe() const // gibt die Auswahl Rahmen Frabe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswRahmen ? auswRahmen->getFarbe() : 0;
}

int AuswahlBox::getAuswRahmenBreite() const // gibt die Auswahl Rahmen Breite zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswRahmen ? auswRahmen->getRBreite() : 0;
}

AlphaFeld *AuswahlBox::getAuswAlphaFeld() const // gibt das Auswahl AlphaFeld zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswAf ? auswAf->getThis() : 0;
}

AlphaFeld *AuswahlBox::zAuswAlphaFeld() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswAf;
}

int AuswahlBox::getAuswAlphaFeldFarbe() const // gibt die Auswahl AlphaFeld Farbe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswAf ? auswAf->getFarbe() : 0;
}

int AuswahlBox::getAuswAlphaFeldStrength() const // gibt die Auswahl AlphaFeld stärke zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswAf ? auswAf->getStrength() : 0;
}

int AuswahlBox::getAuswHintergrundFarbe() const // gibt die Auswahl Hintergrund Farbe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswBgF;
}

Bild *AuswahlBox::getAuswHintergrundBild() const // gibt das Auswahl Hintergrund Bild zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswBgB ? auswBgB->getThis() : 0;
}

Bild *AuswahlBox::zAuswHintergrundBild() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return auswBgB;
}

LRahmen *AuswahlBox::getMsAuswRahmen( int i ) const // gibt den Multistyle Auswahl Rahmen zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswRahmen )
        return 0;
    return msAuswRahmen->z( i ) ? msAuswRahmen->z( i )->getThis() : 0;
}

LRahmen *AuswahlBox::zMsAuswRahmen( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswRahmen )
        return 0;
    return msAuswRahmen->z( i );
}

int AuswahlBox::getMsAuswRahmenFarbe( int i ) const // gibt die Multistyle Auswahl Rahmen Frabe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswRahmen )
        return 0;
    return msAuswRahmen->z( i ) ? msAuswRahmen->z( i )->getFarbe() : 0;
}

int AuswahlBox::getMsAuswRahmenBreite( int i ) const // gibt die Multistyle Auswahl Rahmen Breite zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswRahmen )
        return 0;
    return msAuswRahmen->z( i ) ? msAuswRahmen->z( i )->getRBreite() : 0;
}

AlphaFeld *AuswahlBox::getMsAuswAlphaFeld( int i ) const // gibt das Multistyle Auswahl AlphaFeld zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswAf )
        return 0;
    return msAuswAf->z( i ) ? msAuswAf->z( i )->getThis() : 0;
}

AlphaFeld *AuswahlBox::zMsAuswAlphaFeld( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswAf )
        return 0;
    return msAuswAf->z( i );
}

int AuswahlBox::getMsAuswAlphaFeldFarbe( int i ) const // gibt die Multistyle Auswahl AlphaFeld Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswAf )
        return 0;
    return msAuswAf->z( i ) ? msAuswAf->z( i )->getFarbe() : 0;
}

int AuswahlBox::getMsAuswAlphaFeldStrength( int i ) const // gibt die Multistyle Auswahl AlphaFeld stärke zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswAf )
        return 0;
    return msAuswAf->z( i ) ? msAuswAf->z( i )->getStrength() : 0;
}

int AuswahlBox::getMsAuswHintergrundFarbe( int i ) const // gibt die Multistyle Auswahl Hintergrund Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswBgF || !msAuswBgF->hat( i ) )
        return 0;
    return msAuswBgF->get( i );
}

Bild *AuswahlBox::getMsAuswHintergrundBild( int i ) const // gibt das Multistyle Auswahl Hintergrund Bild zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswBgB )
        return 0;
    return msAuswBgB->get( i );
}

Bild *AuswahlBox::zMsAuswHintergrundBild( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msAuswBgB )
        return 0;
    return msAuswBgB->z( i );
}

LRahmen *AuswahlBox::getMausRahmen() const // gibt den Maus Rahmen zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausRahmen ? mausRahmen->getThis() : 0;
}

LRahmen *AuswahlBox::zMausRahmen() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausRahmen;
}

int AuswahlBox::getMausRahmenFarbe() const // gibt die Maus Rahmen Frabe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausRahmen ? mausRahmen->getFarbe() : 0;
}

int AuswahlBox::getMausRahmenBreite() const // gibt die Maus Rahmen Breite zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausRahmen ? mausRahmen->getRBreite() : 0;
}

AlphaFeld *AuswahlBox::getMausAlphaFeld() const // gibt das Maus AlphaFeld zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausAf ? mausAf->getThis() : 0;
}

AlphaFeld *AuswahlBox::zMausAlphaFeld() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausAf;
}

int AuswahlBox::getMausAlphaFeldFarbe() const // gibt die Maus AlphaFeld Farbe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausAf ? mausAf->getFarbe() : 0;
}

int AuswahlBox::getMausAlphaFeldStrength() const // gibt die Maus AlphaFeld stärke zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausAf ? mausAf->getStrength() : 0;
}

int AuswahlBox::getMausHintergrundFarbe() const // gibt die Maus Hintergrund Farbe zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausBgF;
}

Bild *AuswahlBox::getMausHintergrundBild() const // gibt das Maus Hintergrund Bild zurück
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausBgB ? mausBgB->getThis() : 0;
}

Bild *AuswahlBox::zMausHintergrundBild() const
{
    if( hatStyle( Style::MultiStyled ) )
        return 0;
    return mausBgB;
}

LRahmen *AuswahlBox::getMsMausRahmen( int i ) const // gibt den Multistyle Maus Rahmen zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausRahmen )
        return 0;
    return msMausRahmen->get( i );
}

LRahmen *AuswahlBox::zMsMausRahmen( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausRahmen )
        return 0;
    return msMausRahmen->z( i );
}

int AuswahlBox::getMsMausRahmenFarbe( int i ) const // gibt die Multistyle Maus Rahmen Frabe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausRahmen )
        return 0;
    return msMausRahmen->z( i ) ? msMausRahmen->z( i )->getFarbe() : 0;
}

int AuswahlBox::getMsMausRahmenBreite( int i ) const // gibt die Multistyle Maus Rahmen Breite zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausRahmen )
        return 0;
    return msMausRahmen->z( i ) ? msMausRahmen->z( i )->getRBreite() : 0;
}

AlphaFeld *AuswahlBox::getMsMausAlphaFeld( int i ) const // gibt das Multistyle Maus AlphaFeld zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausAf )
        return 0;
    return msMausAf->get( i );
}

AlphaFeld *AuswahlBox::zMsMausAlphaFeld( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausAf )
        return 0;
    return msMausAf->z( i );
}

int AuswahlBox::getMsMausAlphaFeldFarbe( int i ) const // gibt die Multistyle Maus AlphaFeld Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausAf )
        return 0;
    return msMausAf->z( i ) ? msMausAf->z( i )->getFarbe() : 0;
}

int AuswahlBox::getMsMausAlphaFeldStrength( int i ) const // gibt die Multistyle Maus AlphaFeld stärke zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausAf )
        return 0;
    return msMausAf->z( i ) ? msMausAf->z( i )->getStrength() : 0;
}

int AuswahlBox::getMsMausHintergrundFarbe( int i ) const // gibt die Multistyle Maus Hintergrund Farbe zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausBgF || !msMausBgF->hat( i ) )
        return 0;
    return msMausBgF->get( i );
}

Bild *AuswahlBox::getMsMausHintergrundBild( int i ) const // gibt das Multistyle Maus Hintergrund Bild zurück
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausBgB )
        return 0;
    return msMausBgB->get( i );
}

Bild *AuswahlBox::zMsMausHintergrundBild( int i ) const
{
    if( !hatStyle( Style::MultiStyled ) )
        return 0;
    if( !msMausBgB )
        return 0;
    return msMausBgB->z( i );
}

bool AuswahlBox::hatMsStyle( int i, __int64 abStyle ) const // prüft ob Multistyle style vorhanden
{
    if( ( style | Style::MultiStyled ) != style || !msStyle || !msStyle->hat( i ) )
        return 0;
    return ( msStyle->get( i ) | abStyle ) == msStyle->get( i );
}

bool AuswahlBox::hatMsStyleNicht( int i, __int64 abStyle ) const // prüft ob Multistyle style nicht vorhanden
{
    if( ( style | Style::MultiStyled ) != style || !msStyle || !msStyle->hat( i ) )
        return 1;
    return ( msStyle->get( i ) | abStyle ) != msStyle->get( i );
}

Zeichnung *AuswahlBox::dublizieren() const // Erzeugt eine Kopie des Zeichnungs
{
    AuswahlBox *obj = new AuswahlBox();
    obj->setPosition( pos );
    obj->setSize( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( Mak );
    obj->setTastaturEreignis( Tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    obj->setStyle( style );
    if( schrift )
        obj->setSchriftZ( schrift->getThis() );
    if( rahmen )
        obj->setLinienRahmenZ( (LRahmen*)rahmen->dublizieren() );
    if( ausfahren )
        obj->setAusklappKnopfZ( (Knopf*)ausfahren->dublizieren() );
    obj->setHintergrundFarbe( hintergrundFarbe );
    if( hintergrundBild )
        obj->setHintergrundBild( hintergrundBild->getThis() );
    if( hintergrundFeld )
        obj->setAlphaFeldZ( (AlphaFeld*)hintergrundFeld->dublizieren() );
    if( auswRahmen )
        obj->setAuswRahmenZ( (LRahmen*)auswRahmen->dublizieren() );
    obj->setAuswHintergrundFarbe( auswBgF );
    if( auswBgB )
        obj->setAuswHintergrundBild( auswBgB->getThis() );
    if( auswAf )
        obj->setAuswAlphaFeldZ( (AlphaFeld*)auswAf->dublizieren() );
    if( mausRahmen )
        obj->setMausRahmenZ( (LRahmen*)mausRahmen->dublizieren() );
    obj->setMausHintergrundFarbe( mausBgF );
    if( mausBgB )
        obj->setMausHintergrundBild( mausBgB->getThis() );
    if( mausAf )
        obj->setMausAlphaFeldZ( (AlphaFeld*)mausAf->dublizieren() );
    obj->setMaxAuskappHeight( ausklapMaxHeight );
    obj->setEintragHeight( eintragHeight );
    for( int i = 0; i < anzahl; ++i )
    {
        if( members->z( i ) )
        {
            obj->addEintrag( "a" );
            obj->setEintragZ( i, (TextFeld*)members->z( i )->dublizieren() );
            if( msStyle && msStyle->hat( i ) )
                obj->setMsStyle( i, msStyle->get( i ) );
            if( msAuswRahmen && msAuswRahmen->z( i ) )
                obj->setMsAuswRahmenZ( i, (LRahmen*)msAuswRahmen->z( i )->dublizieren() );
            if( msAuswBgF && msAuswBgF->hat( i ) )
                obj->setMsAuswHintergrundFarbe( i, msAuswBgF->get( i ) );
            if( msAuswBgB && msAuswBgB->z( i ) )
                obj->setMsAuswHintergrundBild( i, msAuswBgB->get( i ) );
            if( msAuswAf && msAuswAf->z( i ) )
                obj->setMsAuswAlphaFeldZ( i, (AlphaFeld*)msAuswAf->z( i )->dublizieren() );
            if( msMausRahmen && msMausRahmen->z( i ) )
                obj->setMsMausRahmenZ( i, (LRahmen*)msMausRahmen->z( i )->dublizieren() );
            if( msMausBgF && msMausBgF->hat( i ) )
                obj->setMsMausHintergrundFarbe( i, msMausBgF->get( i ) );
            if( msMausBgB && msMausBgB->z( i ) )
                obj->setMsMausHintergrundBild( i, msMausBgB->get( i ) );
            if( msMausAf && msMausAf->z( i ) )
                obj->setMsMausAlphaFeldZ( i, (AlphaFeld*)msMausAf->z( i )->dublizieren() );
        }
    }
    obj->setAlphaFeldFarbe( auswahl );
    return obj;
}

// Reference Counting
AuswahlBox *AuswahlBox::getThis()
{
    ++ref;
    return this;
}

AuswahlBox *AuswahlBox::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}