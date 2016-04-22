#include "RenderThread.h"
#include "Bildschirm.h"
#include "Zeit.h"

using namespace Framework;

// Inhalt der RenderTh Klasse aus RenderThread.h
// Konstruktor 
RenderTh::RenderTh()
    : stoppen( 0 ),
    bildschirm( 0 ),
    zeit( new ZeitMesser() ),
    renderTickZeit( 1 / 60 ),
    renderParameter( 0 ),
    tickParameter( 0 ),
    renderFunktion( 0 ),
    tickFunktion( 0 ),
    pause( 0 ),
    maxFps( 30 ),
    ref( 1 )
{
    InitializeCriticalSection( &cs );
}

// Destruktor 
RenderTh::~RenderTh()
{
    if( run )
        beenden();
    if( bildschirm )
        bildschirm->release();
    zeit->release();
    DeleteCriticalSection( &cs );
}

// nicht constant 
void RenderTh::lock()
{
    EnterCriticalSection( &cs );
}

void RenderTh::unlock()
{
    LeaveCriticalSection( &cs );
}

void RenderTh::setBildschirm( Bildschirm *bildschirm ) // setzt den Bildschirm
{
    lock();
    if( this->bildschirm )
        this->bildschirm->release();
    this->bildschirm = bildschirm;
    unlock();
}

void RenderTh::thread() // Render Schleife
{
    zeit->messungStart();
    double ausgleich = 0;
    while( !stoppen )
    {
        lock();
        if( bildschirm && !pause )
        {
            if( renderFunktion )
                renderFunktion( renderParameter, this, bildschirm->zRenderBild() );
            bildschirm->render();
            if( tickFunktion )
                tickFunktion( tickParameter, this, renderTickZeit );
            bildschirm->tick( renderTickZeit );
            unlock();
        }
        else
        {
            unlock();
            Sleep( 100 );
        }
        ausgleich += 1.0 / maxFps - renderTickZeit;
        if( ausgleich > 0 )
            Sleep( (int)( ausgleich * 1000 ) );
        zeit->messungEnde();
        zeit->messungStart();
        renderTickZeit = zeit->getSekunden();
    }
    zeit->messungEnde();
}

void RenderTh::beginn() // beginnt rendering
{
    stoppen = 0;
    start();
}

void RenderTh::beenden() // beendet den Thread
{
    stoppen = 1;
    warteAufThread( 2000 );
    if( run )
        ende();
}

void RenderTh::setMaxFps( int fps ) // setzt die Anzahl der Bilder pro Sekunde
{
    maxFps = fps;
}

void RenderTh::setPause( bool p ) // Renderpause
{
    pause = p;
}

void RenderTh::setRenderFunktion( void( *rF )( void *, void *, Bild * ) ) // setzt die Rückruf Funktion beim Rendern
{
    renderFunktion = rF;
}

void RenderTh::setTickFunktion( void( *tF )( void *, void *, double ) ) // setzt die Rückruf Funktion beim Tick
{
    tickFunktion = tF;
}

void RenderTh::setRenderFunktionParameter( void *p ) // setzt den Parameter der Rückruf Funktion beim Rendern
{
    renderParameter = p;
}

void RenderTh::setTickFunktionParameter( void *p ) // setzt den Parameter der Rückruf Funktion beim Tick
{
    tickParameter = p;
}

// constant
Bildschirm *RenderTh::getBildschirm() const // gibt den Bildschirm zurück
{
    return bildschirm ? bildschirm->getThis() : 0;
}

Bildschirm *RenderTh::zBildschirm() const
{
    return bildschirm;
}

double RenderTh::getRenderTickZeit() const // gibt die Zeit zurück, die zum Rendern und zum Tick benötigt wird
{
    return renderTickZeit;
}

// Reference Counting
RenderTh *RenderTh::getThis()
{
    ++ref;
    return this;
}

RenderTh *RenderTh::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}