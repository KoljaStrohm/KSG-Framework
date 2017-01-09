#include "Animation.h"
#include "Bild.h"
#include "DateiSystem.h"
#include "Text.h"
#include "InitDatei.h"
#include "ToolTip.h"
#include "Rahmen.h"

using namespace Framework;

// Inhalt der Animation2DData Klasse aus Animation.h
// Konstruktor
Animation2DData::Animation2DData()
    : bilder( 0 ),
    bildAnzahl( 0 ),
    fps( 0 ),
    wiederhohlen( 0 ),
    transparent( 0 ),
    ref( 1 )
{
    InitializeCriticalSection( &cs );
}

// Destruktor
Animation2DData::~Animation2DData()
{
    reset();
    DeleteCriticalSection( &cs );
}

// nicht constant
void Animation2DData::lock()
{
    EnterCriticalSection( &cs );
}

void Animation2DData::unlock()
{
    LeaveCriticalSection( &cs );
}

void Animation2DData::ladeAnimation( InitDatei *datei )
{
    if( !datei )
        return;
    reset();
    int anz = datei->getWertAnzahl();
    lock();
    if( datei->wertExistiert( "fps" ) )
    {
        --anz;
        fps = TextZuInt( datei->zWert( "fps" )->getText(), 10 );
    }
    if( datei->wertExistiert( "wiederhohlen" ) )
    {
        --anz;
        wiederhohlen = datei->zWert( "wiederhohlen" )->istGleich( "true" );
    }
    if( datei->wertExistiert( "transparent" ) )
    {
        --anz;
        transparent = datei->zWert( "transparent" )->istGleich( "true" );
    }
    Bild **bilder = new Bild*[ anz ];
    int j = 0;
    for( int i = 0; i < anz; ++i )
    {
        if( datei->zName( i )->istGleich( "fps" ) ||
            datei->zName( i )->istGleich( "wiederhohlen" ) ||
            datei->zName( i )->istGleich( "transparent" ) )
            continue;
        bilder[ j ] = 0;
        Text pfad = datei->zWert( i )->getText();
        if( pfad.hat( ".ltdb/" ) && pfad.getLength() > 7 )
        {
            Text *name = pfad.getTeilText( pfad.positionVon( ".ltdb/", pfad.anzahlVon( ".ltdb/" ) - 1 ) + 6 );
            pfad.setText( pfad.getTeilText( 0, pfad.getLength() - name->getLength() - 1 ) );
            LTDBDatei *dat = new LTDBDatei();
            dat->setDatei( pfad.getThis() );
            dat->leseDaten( 0 );
            bilder[ j ] = dat->laden( 0, name );
            dat->release();
        }
        ++j;
    }
    this->bilder = new Bild*[ bildAnzahl ];
    j = 0;
    for( int i = 0; i < anz; ++i )
    {
        if( !bilder[ i ] )
            ++j;
        else
            this->bilder[ i - j ] = bilder[ i ];
    }
    delete[] bilder;
    unlock();
    datei->release();
}

void Animation2DData::ladeAnimation( LTDBDatei *datei )
{
    if( !datei )
        return;
    reset();
    datei->leseDaten( 0 );
    int anz = datei->getBildAnzahl();
    RCArray< Text > *list = datei->zBildListe();
    lock();
    Bild **bilder = new Bild*[ anz ];
    for( int i = 0; i < anz; ++i )
    {
        bilder[ i ] = datei->laden( 0, list->get( i ) );
        if( bilder[ i ] )
            ++bildAnzahl;
    }
    this->bilder = new Bild*[ bildAnzahl ];
    int j = 0;
    for( int i = 0; i < anz; ++i )
    {
        if( !bilder[ i ] )
            ++j;
        else
            this->bilder[ i - j ] = bilder[ i ];
    }
    delete[] bilder;
    unlock();
    datei->release();
}

void Animation2DData::setFPS( int fps )
{
    this->fps = fps;
}

void Animation2DData::setWiederhohlend( bool wh )
{
    wiederhohlen = wh;
}

void Animation2DData::setTransparent( bool trp )
{
    transparent = trp;
}

void Animation2DData::reset()
{
    lock();
    for( int i = 0; i < bildAnzahl; ++i )
        bilder[ i ] = bilder[ i ]->release();
    delete[] bilder;
    bilder = 0;
    bildAnzahl = 0;
    fps = 30;
    wiederhohlen = 0;
    transparent = 0;
    unlock();
}

// constant
Bild *Animation2DData::getBild( int i ) const
{
    return ( i >= 0 && i < bildAnzahl ) ? bilder[ i ]->getThis() : 0;
}

Bild *Animation2DData::zBild( int i ) const
{
    return ( i >= 0 && i < bildAnzahl ) ? bilder[ i ] : 0;
}

int Animation2DData::getBildAnzahl() const
{
    return bildAnzahl;
}

int Animation2DData::getFPS() const
{
    return fps;
}

bool Animation2DData::istWiederhohlend() const
{
    return wiederhohlen;
}

bool Animation2DData::istTransparent() const
{
    return transparent;
}

// Reference Counting
Animation2DData *Animation2DData::getThis()
{
    ++ref;
    return this;
}

Animation2DData *Animation2DData::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der Animation2D Klasse aus Animation.h
// Konstruktor
Animation2D::Animation2D()
    : Zeichnung(),
    data( 0 ),
    jetzt( 0 ),
    ausgleich( 0 ),
    alpha( 0 ),
    maxAlpha( 255 ),
    rahmen( 0 ),
    ram( 0 ),
    aps( 255 * 60 ),
    sichtbar( 0 ),
    ref( 1 )
{}

// Destruktor
Animation2D::~Animation2D()
{
    if( data )
        data->release();
    if( ram )
        ram->release();
}

// nicht constant
void Animation2D::setRahmen( bool ram )
{
    rahmen = ram;
}

void Animation2D::setRahmenZ( LRahmen *ram )
{
    if( this->ram )
        this->ram->release();
    this->ram = ram;
}

void Animation2D::setRahmenBreite( int br )
{
    if( !ram )
        ram = new LRahmen();
    ram->setRamenBreite( br );
}

void Animation2D::setRahmenFarbe( int f )
{
    if( !ram )
        ram = new LRahmen();
    ram->setFarbe( f );
}

void Animation2D::setAnimationDataZ( Animation2DData *data )
{
    lockZeichnung();
    if( this->data )
        this->data->release();
    this->data = data;
    if( alpha )
        rend = 1;
    unlockZeichnung();
}

void Animation2D::setAlphaMaske( unsigned char alpha )
{
    maxAlpha = alpha;
}

void Animation2D::setAPS( int aps )
{
    this->aps = aps;
}

void Animation2D::setSichtbar( bool sichtbar )
{
    this->sichtbar = sichtbar;
}

bool Animation2D::tick( double zeit )
{
    lockZeichnung();
    if( !data || ( !alpha && !sichtbar ) )
    {
        bool ret = rend;
        rend = 0;
        unlockZeichnung();
        return ret;
    }
    if( sichtbar && alpha < maxAlpha )
    {
        if( alpha + aps * zeit >= maxAlpha )
            alpha = maxAlpha;
        else
            alpha = (unsigned char)( alpha + aps * zeit );
        rend = 1;
    }
    else if( !sichtbar && alpha > 0 )
    {
        if( alpha - aps * zeit <= 0 )
            alpha = 0;
        else
            alpha = (unsigned char)( alpha - aps * zeit );
        rend = 1;
    }
    ausgleich += zeit;
    int tmp = jetzt;
    data->lock();
    if( ausgleich >= 1.0 / data->getFPS() )
    {
        ausgleich -= 1.0 / data->getFPS();
        ++jetzt;
        if( jetzt >= data->getBildAnzahl() )
        {
            if( data->istWiederhohlend() )
                jetzt = 0;
            else
                jetzt = data->getBildAnzahl();
        }
    }
    data->unlock();
    if( tmp != jetzt )
        rend = 1;
    bool ret = rend;
    rend = 0;
    unlockZeichnung();
    return ret;
}

void Animation2D::render( Bild &zRObj )
{
    lockZeichnung();
    if( !data )
    {
        unlockZeichnung();
        return;
    }
	Zeichnung::render( zRObj );
    data->lock();
    if( data->zBild( jetzt ) )
    {
        zRObj.setAlpha( alpha );
        if( data->istTransparent() )
            zRObj.alphaBild( pos.x, pos.y, gr.x, gr.y, *data->zBild( jetzt ) );
        else
            zRObj.drawBild( pos.x, pos.y, gr.x, gr.y, *data->zBild( jetzt ) );
        if( ram && rahmen )
        {
            ram->setPosition( pos );
            ram->setSize( gr );
            ram->render( zRObj );
        }
        zRObj.releaseAlpha();
    }
    data->unlock();
    unlockZeichnung();
}

// constant
Animation2DData *Animation2D::getAnimationData() const
{
    return data ? data->getThis() : 0;
}

Animation2DData *Animation2D::zAnimationData() const
{
    return data;
}

bool Animation2D::istSichtbar() const
{
    return sichtbar;
}

int Animation2D::getJetzt() const
{
    return jetzt;
}

unsigned char Animation2D::getAlphaMaske() const
{
    return maxAlpha;
}

bool Animation2D::hatRahmen() const
{
    return rahmen;
}

LRahmen *Animation2D::getRahmen() const
{
    return ram ? ram->getThis() : 0;
}

LRahmen *Animation2D::zRahmen() const
{
    return ram;
}

int Animation2D::getRahmenBreite() const
{
    return ram ? ram->getRBreite() : 0;
}

int Animation2D::getRahmenFarbe() const
{
    return ram ? ram->getFarbe() : 0;
}

Zeichnung *Animation2D::dublizieren() const
{
    Animation2D *ret = new Animation2D();
    ret->setPosition( pos );
    ret->setSize( gr );
    ret->setMausEreignisParameter( makParam );
    ret->setTastaturEreignisParameter( takParam );
    ret->setMausEreignis( Mak );
    ret->setTastaturEreignis( Tak );
    if( toolTip )
        ret->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    if( data )
        ret->setAnimationDataZ( data->getThis() );
    ret->setAPS( aps );
    ret->setSichtbar( sichtbar );
    ret->setAlphaMaske( maxAlpha );
    ret->setRahmen( rahmen );
    if( ram )
    {
        ret->setRahmenBreite( ram->getRBreite() );
        ret->setRahmenFarbe( ram->getFarbe() );
    }
    return ret;
}

// Reference Counting
Animation2D *Animation2D::getThis()
{
    ++ref;
    return this;
}

Animation2D *Animation2D::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}