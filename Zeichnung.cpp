#include "Zeichnung.h"
#include "MausEreignis.h"
#include "TastaturEreignis.h"
#include "Globals.h"
#include "ToolTip.h"
#include "Scroll.h"
#include "Text.h"
#include "Rahmen.h"
#include "AlphaFeld.h"
#include "Bild.h"
#ifdef WIN32
#include <Windows.h>
#endif

using namespace Framework;

// Inhalt der Zeichnung Klasse aus Zeichnung.h
// Konstruktor 
Zeichnung::Zeichnung()
    : pos( 0, 0 ),
    gr( 0, 0 ),
    makParam( 0 ),
    takParam( 0 ),
    mak( 0 ),
    tak( 0 ),
    nmakParam( 0 ),
    ntakParam( 0 ),
    nMak( 0 ),
    nTak( 0 ),
    mausIn( 0 ),
    toolTip( 0 ),
    rend( 0 )
{}

// Destruktor 
Zeichnung::~Zeichnung()
{
    if( toolTip )
        toolTip->release();
}

// �bergibt einen Void Funktionspointer auf eine Aktion die einmalig vom Hauptthread ausgef�hrt werden soll. (Passiert nach dem Tick)
void Zeichnung::postAction( std::function< void() > action )
{
    actions.push( action );
}

// nicht constant 
void Zeichnung::setRender()
{
    rend = 1;
}

void Zeichnung::setToolTipText( const char *txt, Bildschirm *zScreen )
{
    if( !txt )
        toolTip = (ToolTip*)toolTip->release();
    else
    {
        if( !toolTip )
            toolTip = new ToolTip( zScreen );
        toolTip->setText( txt );
    }
}

void Zeichnung::lockZeichnung()
{
    cs.lock();
}

void Zeichnung::unlockZeichnung()
{
    cs.unlock();
}

void Zeichnung::setMausEreignisParameter( void *p ) // setzt den Parameter vom Maus Ereignis
{
    makParam = p;
}

void Zeichnung::setTastaturEreignisParameter( void *p ) // setzt den Parameter vom Tastatur Ereignis
{
    takParam = p;
}

void Zeichnung::setMausEreignis( std::function< bool( void*, void*, MausEreignis ) > ak ) // setzt das Maus Ereignis
{
    mak = ak;
}

void Zeichnung::setTastaturEreignis( std::function< bool( void*, void*, TastaturEreignis ) > ak ) // setzt das TastaturEreignis
{
    tak = ak;
}

void Zeichnung::setNMausEreignisParameter( void *p ) // setzt den Parameter vom Maus Ereignis
{
    nmakParam = p;
}

void Zeichnung::setNTastaturEreignisParameter( void *p ) // setzt den Parameter vom Tastatur Ereignis
{
    ntakParam = p;
}

void Zeichnung::setNMausEreignis( std::function< bool( void*, void*, MausEreignis ) > ak ) // setzt das Maus Ereignis
{
    nMak = ak;
}

void Zeichnung::setNTastaturEreignis( std::function< bool( void*, void*, TastaturEreignis ) > ak ) // setzt das TastaturEreignis
{
    nTak = ak;
}

void Zeichnung::doMausEreignis( MausEreignis &me ) // ruft Mak auf
{
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
    if( mak )
        me.verarbeitet |= mak( makParam, this, me );
    if( nMak && me.verarbeitet )
        me.verarbeitet = nMak( nmakParam, this, me );
    me.mx += pos.x, me.my += pos.y;
}

void Zeichnung::doTastaturEreignis( TastaturEreignis &te ) // ruft Tak auf
{
    if( te.verarbeitet )
        return;
    if( tak )
        te.verarbeitet |= tak( takParam, this, te );
    if( nTak && te.verarbeitet )
        te.verarbeitet = nTak( ntakParam, this, te );
}

void Zeichnung::setPosition( const Punkt &pos ) // setzt die position
{
    lockZeichnung();
    if( this->pos != pos )
        rend = 1;
    this->pos = pos;
    unlockZeichnung();
}

void Zeichnung::setX( int xPos )
{
    lockZeichnung();
    if( pos.x != xPos )
    {
        rend = 1;
        pos.x = xPos;
    }
    unlockZeichnung();
}

void Zeichnung::setY( int yPos )
{
    lockZeichnung();
    if( pos.y != yPos )
    {
        rend = 1;
        pos.y = yPos;
    }
    unlockZeichnung();
}

void Zeichnung::setSize( const Punkt &gr ) // setzt die Gr��e
{
    lockZeichnung();
    if( this->gr != gr )
        rend = 1;
    this->gr = gr;
    unlockZeichnung();
}

void Zeichnung::setPosition( int x, int y ) // setzt die position
{
    setPosition( Punkt( x, y ) );
}

void Zeichnung::setSize( int x, int y ) // setzt die Gr��e
{
    setSize( Punkt( x, y ) );
}

bool Zeichnung::tick( double tickval )
{
    while( !actions.empty() )
    {
        actions.front()();
        actions.pop();
    }
    bool r = rend;
    rend = 0;
    return r;
}

void Zeichnung::setStyle( __int64 style ) // setzt den Style des Text Feldes
{
    if( this->style != style )
    {
        this->style = style;
        rend = 1;
    }
}

void Zeichnung::setStyle( __int64 style, bool add_remove )
{
    if( add_remove && ( this->style | style ) != this->style )
    {
        this->style |= style;
        rend = 1;
    }
    else if( !add_remove && ( this->style & ~style ) != this->style )
    {
        if( toolTip && ( style | Style::Sichtbar ) == style )
            toolTip->setMausIn( 0 );
        this->style &= ~style;
        rend = 1;
    }
}

void Zeichnung::addStyle( __int64 style )
{
    if( ( this->style | style ) != this->style )
    {
        this->style |= style;
        rend = 1;
    }
}

void Zeichnung::removeStyle( __int64 style )
{
    if( ( this->style & ~style ) != this->style )
    {
        if( toolTip && ( style | Style::Sichtbar ) == style )
            toolTip->setMausIn( 0 );
        this->style &= ~style;
        rend = 1;
    }
}

void Zeichnung::render( Bild &zRObj )
{
    if( toolTip && ( style | Style::Sichtbar ) == style )
        toolTip->setZeichnen();
}

// constant 
bool Zeichnung::hatMausEreignis() const // pr�ft, ob Mak gesetzt ist
{
    return mak != 0;
}

bool Zeichnung::hatTastaturEreignis() const // pr�ft, ob Tak gesetzt ist
{
    return tak != 0;
}

const Punkt &Zeichnung::getPosition() const // gibt die Position zur�ck
{
    return pos;
}

const Punkt &Zeichnung::getSize() const // gibt die Gr��e zur�ck
{
    return gr;
}

int Zeichnung::getBreite() const // gibt die Breite zur�ck
{
    return gr.x;
}

int Zeichnung::getHeight() const // gibt die H�he zur�ck
{
    return gr.y;
}

int Zeichnung::getX() const // gibt X zur�ck
{
    return pos.x;
}

int Zeichnung::getY() const // gibt Y zur�ck
{
    return pos.y;
}

ToolTip *Zeichnung::getToolTip() const // gibt den ToolTip Text
{
    return (ToolTip*)toolTip->getThis();
}

ToolTip *Zeichnung::zToolTip() const
{
    return toolTip;
}

bool Zeichnung::hatStyle( __int64 style ) const // pr�ft, ob style vorhanden
{
    return ( this->style | style ) == this->style;
}

bool Zeichnung::hatStyleNicht( __int64 style ) const // pr�ft, ob style nicht vorhanden
{
    return ( this->style | style ) != this->style;
}

Zeichnung *Zeichnung::dublizieren() const // Erzeugt eine Kopie des Zeichnungs
{
    Zeichnung *obj = new Zeichnung();
    obj->setPosition( pos );
    obj->setSize( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( mak );
    obj->setTastaturEreignis( tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    return obj;
}

// Inhalt der ZeichnungArray Klasse aus Zeichnung.h
// Konstruktor 
ZeichnungArray::ZeichnungArray()
    : This( 0 ),
    next( 0 )
{}

// Destruktor 
ZeichnungArray::~ZeichnungArray()
{
    delete next;
}

// nicht const 
bool ZeichnungArray::addZeichnung( Zeichnung *obj ) // F�gt ein Zeichnung hinzu
{
    if( obj == This )
        return 0;
    if( !This )
    {
        This = obj;
        return 1;
    }
    if( !next )
        next = new ZeichnungArray();
    return next->addZeichnung( obj );
}

bool ZeichnungArray::removeZeichnung( Zeichnung *obj ) // Entfernt ein Zeichnung
{
    if( obj == This )
    {
        if( index == 0 )
        {
            if( next )
            {
                This = next->getZeichnung();
                ZeichnungArray *tmp = next->getNext();
                next->setNext0();
                delete next;
                next = tmp;
            }
            else
                This = 0;
            return 0;
        }
        return 1;
    }
    if( !next )
        return 0;
    if( next->removeZeichnung( obj ) )
    {
        ZeichnungArray *tmp = next->getNext();
        next->setNext0();
        delete next;
        next = tmp;
    }
    return 0;
}

bool ZeichnungArray::removeZeichnung( int i ) // Entfernt das von diesem aus i-te Zeichnung
{
    if( i == index )
    {
        if( index == 0 )
        {
            This = next->getZeichnung();
            ZeichnungArray *tmp = next->getNext();
            next->setNext0();
            delete next;
            next = tmp;
            return 0;
        }
        return 1;
    }
    if( !next )
        return 0;
    if( next->removeZeichnung( i ) )
    {
        ZeichnungArray *tmp = next->getNext();
        next->setNext0();
        delete next;
        next = tmp;
    }
    return 0;
}

void ZeichnungArray::setNext0() // Setzt das n�chste Zeichnung zu 0
{
    next = 0;
}

void ZeichnungArray::updateIndex( int i ) // aktualisiert die Index variable
{
    index = i;
    if( next )
        next->updateIndex( i + 1 );
}

// constant 
ZeichnungArray *ZeichnungArray::getNext() const // gibt das n�chste Zeichnung zur�ck
{
    return next;
}

Zeichnung *ZeichnungArray::getZeichnung( int i ) const // gibt das von diesem aus i-te Zeichnung zur�ck
{
    if( i == index )
        return This;
    if( !next )
        return 0;
    return next->getZeichnung( i );
}

Zeichnung *ZeichnungArray::getZeichnung() const // gibt das von diesem aus i-te Zeichnung zur�ck
{
    return This;
}

int ZeichnungArray::getIndex() const // Gibt den Index zur�ck
{
    return index;
}

void ZeichnungArray::sendMausAll( MausEreignis &me ) const // sendet me an alle volgenden Zeichnunge
{
    if( next )
        next->sendMausAll( me );
    if( This )
        This->doMausEreignis( me );
}

void ZeichnungArray::sendTastaturAll( TastaturEreignis &te ) const // sendet te an alle volgenden Zeichnunge
{
    if( next )
        next->sendTastaturAll( te );
    if( This )
        This->doTastaturEreignis( te );
}

void ZeichnungArray::render( Bild &zRObj )
{
    if( This )
        This->render( zRObj );
    if( next )
        next->render( zRObj );
}

bool ZeichnungArray::tick( double tickval )
{
    return ( This && This->tick( tickval ) ) | ( next && next->tick( tickval ) );
}


// Inhalt der ZeichnungHintergrund Klasse aus Zeichnung.h
// Konstruktor 
ZeichnungHintergrund::ZeichnungHintergrund()
    : Zeichnung()
{
    hintergrundFarbe = 0xFF000000;
    rahmen = 0;
    hintergrundBild = 0;
    hintergrundFeld = 0;
    horizontalScrollBar = 0;
    vertikalScrollBar = 0;
    innenPosition.x = 0;
    innenPosition.y = 0;
    innenSize.x = 0;
    innenSize.y = 0;
}

// Destruktor 
ZeichnungHintergrund::~ZeichnungHintergrund()
{
    if( rahmen )
        rahmen->release();
    if( hintergrundBild )
        hintergrundBild->release();
    if( hintergrundFeld )
        hintergrundFeld->release();
    if( horizontalScrollBar )
        horizontalScrollBar->release();
    if( vertikalScrollBar )
        vertikalScrollBar->release();
}

void ZeichnungHintergrund::setHintergrundBild( Bild *bild ) // setzt das Hintergrund Bild
{
    if( !hintergrundBild )
        hintergrundBild = new Bild();
    hintergrundBild->neuBild( bild->getBreite(), bild->getHeight(), 0 );
    int *buff1 = hintergrundBild->getBuffer();
    int *buff2 = bild->getBuffer();
    for( int i = 0; i < bild->getBreite() * bild->getHeight(); ++i )
        buff1[ i ] = buff2[ i ];
    bild->release();
    rend = 1;
}

void ZeichnungHintergrund::setHintergrundBildZ( Bild *bild ) // setzt einen Zeiger zum Hintergrund Bild
{
    if( hintergrundBild != bild )
    {
        if( hintergrundBild )
            hintergrundBild->release();
        hintergrundBild = bild;
        rend = 1;
    }
}

void ZeichnungHintergrund::setHintergrundFarbe( int fc ) // setzt die Hintergrundfarbe
{
    if( hintergrundFarbe != fc )
    {
        hintergrundFarbe = fc;
        rend = 1;
    }
}

void ZeichnungHintergrund::setAlphaFeldZ( AlphaFeld *buff ) // setzt einen Zeiger zum Hintergrund Buffer
{
    if( hintergrundFeld != buff )
    {
        if( hintergrundFeld )
            hintergrundFeld->release();
        hintergrundFeld = buff;
        rend = 1;
    }
}

void ZeichnungHintergrund::setAlphaFeldStrength( int st ) // setzt die St�rke des Hintergrund Buffers
{
    if( !hintergrundFeld )
    {
        hintergrundFeld = new AlphaFeld();
        rend = 1;
    }
    if( hintergrundFeld->getStrength() != st )
    {
        hintergrundFeld->setStrength( st );
        rend = 1;
    }
}

void ZeichnungHintergrund::setAlphaFeldFarbe( int fc ) // setzt die Farbe des Hintergrund Buffers
{
    if( !hintergrundFeld )
    {
        hintergrundFeld = new AlphaFeld();
        rend = 1;
    }
    if( hintergrundFeld->getFarbe() != fc )
    {
        hintergrundFeld->setFarbe( fc );
        rend = 1;
    }
}

void ZeichnungHintergrund::setLinienRahmenZ( LRahmen *ram ) // setzt einen Zeiger zum Rahmen
{
    if( rahmen != ram )
    {
        if( rahmen )
            rahmen->release();
        rahmen = ram;
        rend = 1;
    }
}

void ZeichnungHintergrund::setLinienRahmenBreite( int br ) // setzt die Breite des Rahmens
{
    if( !rahmen )
    {
        rahmen = new LRahmen();
        rend = 1;
    }
    if( rahmen->getRBreite() != br )
    {
        rahmen->setRamenBreite( br );
        rend = 1;
    }
}

void ZeichnungHintergrund::setLinienRahmenFarbe( int fc ) // setzt die Farbe des Rahmens
{
    if( !rahmen )
    {
        rahmen = new LRahmen();
        rend = 1;
    }
    if( rahmen->getFarbe() != fc )
    {
        rahmen->setFarbe( fc );
        rend = 1;
    }
}

void ZeichnungHintergrund::setVertikalKlickScroll( int ks ) // setzt die vertikale Scroll geschwindigkeit
{
    if( !vertikalScrollBar )
    {
        vertikalScrollBar = new VScrollBar();
        rend = 1;
    }
    if( vertikalScrollBar->getKlickScroll() != ks )
    {
        vertikalScrollBar->setKlickScroll( ks );
        rend = 1;
    }
}

void ZeichnungHintergrund::setVertikalScrollPos( int pos ) // setzt die vertikale Scroll Position
{
    if( !vertikalScrollBar )
    {
        vertikalScrollBar = new VScrollBar();
        rend = 1;
    }
    if( vertikalScrollBar && vertikalScrollBar->getScroll() != pos )
    {
        vertikalScrollBar->scroll( pos );
        rend = 1;
    }
}

void ZeichnungHintergrund::setVertikalScrollFarbe( int f, int bgF ) // setzt die scroll Farbe
{
    if( !vertikalScrollBar )
    {
        vertikalScrollBar = new VScrollBar();
        rend = 1;
    }
    if( vertikalScrollBar && ( vertikalScrollBar->getFarbe() != f || vertikalScrollBar->getBgFarbe() != bgF ) )
    {
        vertikalScrollBar->setFarbe( f );
        vertikalScrollBar->setBgFarbe( bgF, bgF != 0 );
        rend = 1;
    }
}

void ZeichnungHintergrund::setHorizontalKlickScroll( int ks ) // setzt die horizontale Scroll geschwindigkeit
{
    if( !horizontalScrollBar )
    {
        horizontalScrollBar = new HScrollBar();
        rend = 1;
    }
    if( horizontalScrollBar && horizontalScrollBar->getKlickScroll() != ks )
    {
        horizontalScrollBar->setKlickScroll( ks );
        rend = 1;
    }
}

void ZeichnungHintergrund::setHorizontalScrollPos( int pos ) // setzt die horizontale Scroll Position
{
    if( !horizontalScrollBar )
    {
        horizontalScrollBar = new HScrollBar();
        rend = 1;
    }
    if( horizontalScrollBar && horizontalScrollBar->getScroll() != pos )
    {
        horizontalScrollBar->scroll( pos );
        rend = 1;
    }
}

void ZeichnungHintergrund::setHorizontalScrollFarbe( int f, int bgF ) // setzt die scroll Farbe
{
    if( !horizontalScrollBar )
    {
        horizontalScrollBar = new HScrollBar();
        rend = 1;
    }
    if( horizontalScrollBar && ( horizontalScrollBar->getFarbe() != f || horizontalScrollBar->getBgFarbe() != bgF ) )
    {
        horizontalScrollBar->setFarbe( f );
        horizontalScrollBar->setBgFarbe( bgF, bgF != 0 );
        rend = 1;
    }
}

bool ZeichnungHintergrund::tick( double tickVal )
{
    if( vertikalScrollBar && hatStyle( Style::VScroll ) )
        rend |= vertikalScrollBar->getRend();
    if( horizontalScrollBar && hatStyle( Style::HScroll ) )
        rend |= horizontalScrollBar->getRend();
    return Zeichnung::tick( tickVal );
}

void ZeichnungHintergrund::render( Bild &rObj )
{
    innenPosition.x = pos.x;
    innenPosition.y = pos.y;
    innenSize.x = gr.x;
    innenSize.y = gr.y;
    if( hatStyleNicht( Style::Sichtbar ) )
        return;
    lockZeichnung();
    if( !rObj.setDrawOptions( pos.x, pos.y, gr.x, gr.y ) )
    {
        unlockZeichnung();
        return;
    }
    Zeichnung::render( rObj );
    int rbr = 0;
    if( hatStyle( Style::Rahmen ) && rahmen )
    {
        rahmen->setSize( gr );
        rahmen->render( rObj );
        rbr = rahmen->getRBreite();
    }
    innenPosition.x += rbr;
    innenPosition.y += rbr;
    innenSize.x -= rbr * 2;
    innenSize.y -= rbr * 2;
    if( !rObj.setDrawOptions( rbr, rbr, gr.x - rbr * 2, gr.y - rbr * 2 ) )
    {
        rObj.releaseDrawOptions();
        unlockZeichnung();
        return;
    }
    bool vs = vertikalScrollBar && hatStyle( Style::VScroll );
    bool hs = horizontalScrollBar && hatStyle( Style::HScroll );
    if( vs )
    {
        vertikalScrollBar->render( gr.x - rbr * 2 - 15, 0, 15, gr.y - rbr * 2, rObj );
        innenSize.x -= 15;
        if( hs )
        {
            horizontalScrollBar->render( 0, gr.y - rbr * 2 - 15, gr.x - rbr * 2 - 15, 15, rObj );
            innenSize.y -= 15;
            if( !rObj.setDrawOptions( 0, 0, gr.x - rbr * 2 - 15, gr.y - rbr * 2 - 15 ) )
            {
                rObj.releaseDrawOptions();
                rObj.releaseDrawOptions();
                unlockZeichnung();
                return;
            }
            horizontalScrollBar->update( horizontalScrollBar->getScrollData()->max, innenSize.x );
        }
        else
        {
            if( !rObj.setDrawOptions( 0, 0, gr.x - rbr * 2 - 15, gr.y - rbr * 2 ) )
            {
                rObj.releaseDrawOptions();
                rObj.releaseDrawOptions();
                unlockZeichnung();
                return;
            }
        }
        vertikalScrollBar->update( vertikalScrollBar->getScrollData()->max, innenSize.y );
    }
    else if( hs )
    {
        horizontalScrollBar->render( rbr, gr.y - rbr * 2 - 15, gr.x - rbr * 2, 15, rObj );
        innenSize.y -= 15;
        if( !rObj.setDrawOptions( 0, 0, gr.x - rbr * 2, gr.y - rbr * 2 - 15 ) )
        {
            rObj.releaseDrawOptions();
            rObj.releaseDrawOptions();
            unlockZeichnung();
            return;
        }
    }
    if( hatStyle( Style::Hintergrund ) )
    {
        if( hatStyle( Style::HAlpha ) )
            rObj.alphaRegion( 0, 0, gr.x - rbr * 2, gr.y - rbr * 2, hintergrundFarbe );
        else
            rObj.fillRegion( 0, 0, gr.x - rbr * 2, gr.y - rbr * 2, hintergrundFarbe );
        if( hatStyle( Style::HBild ) && hintergrundBild )
        {
            if( hatStyle( Style::HAlpha ) )
                rObj.alphaBildSkall( 0, 0, gr.x - rbr * 2, gr.y - rbr * 2, *hintergrundBild );
            else
                rObj.drawBildSkall( 0, 0, gr.x - rbr * 2, gr.y - rbr * 2, *hintergrundBild );
        }
    }
    if( hatStyle( Style::Buffered ) && hintergrundFeld )
    {
        hintergrundFeld->setSize( gr.x - rbr * 2, gr.y - rbr * 2 );
        hintergrundFeld->render( rObj );
    }
    if( vs || hs )
        rObj.releaseDrawOptions();
    rObj.releaseDrawOptions();
    rObj.releaseDrawOptions();
    unlockZeichnung();
}

Bild *ZeichnungHintergrund::getHintergrundBild() const // gibt getThis vom Hintergrund Bild zur�ck
{
    if( !hintergrundBild )
        return 0;
    return hintergrundBild->getThis();
}

Bild *ZeichnungHintergrund::zHintergrundBild() const // gibt das Hintergrund Bild zur�ck
{
    return hintergrundBild;
}

int ZeichnungHintergrund::getHintergrundFarbe() const // giebt getThis der Hintergrundfarbe zur�ck
{
    return hintergrundFarbe;
}

AlphaFeld *ZeichnungHintergrund::getAlphaFeld() const // gibt getThir vom Hintergrund Buffer zur�ck
{
    if( !hintergrundFeld )
        return 0;
    return hintergrundFeld->getThis();
}

AlphaFeld *ZeichnungHintergrund::zAlphaFeld() const // gibt den Hintergrund Buffer zur�ck
{
    return hintergrundFeld;
}

int ZeichnungHintergrund::getAlphaFeldStrength() const // gibt die St�rke des Hintergrund Buffers zur�ck
{
    if( !hintergrundFeld )
        return 0;
    return hintergrundFeld->getStrength();
}

int ZeichnungHintergrund::getAlphaFeldFarbe() const // gibt getThis von der Farbe des Hintergrund Buffers zur�ck
{
    return hintergrundFeld->getFarbe();
}

LRahmen *ZeichnungHintergrund::getLinienRahmen() const // gibt getThis des Rahmens zur�ck
{
    if( !rahmen )
        return 0;
    return rahmen->getThis();
}

LRahmen *ZeichnungHintergrund::zLinienRahmen() const // gibt den Rahmen zur�ck
{
    return rahmen;
}

int ZeichnungHintergrund::getLinienRahmenBreite() const // gibt die Breite des Rahmens zur�ck
{
    if( !rahmen || hatStyleNicht( Style::Rahmen ) )
        return 0;
    return rahmen->getRBreite();
}

int ZeichnungHintergrund::getLinienRahmenFarbe() const // gibt getThis der Farbe des Rahmens zur�ck
{
    return rahmen->getFarbe();
}

int ZeichnungHintergrund::getVertikalKlickScroll() const
{
    return vertikalScrollBar ? vertikalScrollBar->getKlickScroll() : 0;
}

int ZeichnungHintergrund::getVertikalScrollPos() const
{
    return vertikalScrollBar ? vertikalScrollBar->getScroll() : 0;
}

int ZeichnungHintergrund::getVertikalScrollFarbe() const
{
    return vertikalScrollBar ? vertikalScrollBar->getFarbe() : 0;
}

int ZeichnungHintergrund::getVertikalScrollHintergrund() const
{
    return vertikalScrollBar ? vertikalScrollBar->getBgFarbe() : 0;
}

int ZeichnungHintergrund::getHorizontalKlickScroll() const
{
    return horizontalScrollBar ? horizontalScrollBar->getKlickScroll() : 0;
}

int ZeichnungHintergrund::getHorizontalScrollPos() const
{
    return horizontalScrollBar ? horizontalScrollBar->getScroll() : 0;
}

int ZeichnungHintergrund::getHorizontalScrollFarbe() const
{
    return horizontalScrollBar ? horizontalScrollBar->getFarbe() : 0;
}

int ZeichnungHintergrund::getHorizontalScrollHintergrund() const
{
    return horizontalScrollBar ? horizontalScrollBar->getBgFarbe() : 0;
}

Zeichnung *ZeichnungHintergrund::dublizieren() const // Erzeugt eine Kopie des Zeichnungs
{
    ZeichnungHintergrund *obj = new ZeichnungHintergrund();
    obj->setPosition( pos );
    obj->setSize( gr );
    obj->setMausEreignisParameter( makParam );
    obj->setTastaturEreignisParameter( takParam );
    obj->setMausEreignis( mak );
    obj->setTastaturEreignis( tak );
    if( toolTip )
        obj->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
    obj->setStyle( style );
    obj->setHintergrundFarbe( hintergrundFarbe );
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
    return obj;
}