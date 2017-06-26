#include "Critical.h"
#include "Globals.h"
#include "Thread.h"
#include <iostream>
#include <time.h>

using namespace Framework;

// Inhalt der Critical class aus Critical.h
// Konstructor
Critical::Critical()
{
    InitializeCriticalSection( &cs );
    owner = 0;
    lockCount = 0;
    id = (int)time( 0 );
}

// Destructor
Critical::~Critical()
{
    DeleteCriticalSection( &cs );
}

// sperrt das Objekt
void Critical::lock()
{
    pthread_t t = GetCurrentThread();
    Thread *tmp = getThreadRegister()->zThread( t );
    if( tmp )
        tmp->addCriticalLock();
    EnterCriticalSection( &cs );
    if( !owner )
        owner = tmp;
    lockCount++;
}

// versucht das Objekt zu sperren
bool Critical::tryLock()
{
    if( lockCount > 0 )
        return false;
    Thread *tmp = getThreadRegister()->zThread( GetCurrentThread() );
    if( tmp )
        tmp->addCriticalLock();
    EnterCriticalSection( &cs );
    if( !owner )
        owner = tmp;
    lockCount++;
    return true;
}

// entsperrt das Objekt
void Critical::unlock()
{
    if( owner && GetThreadId( owner->getThreadHandle() ) != GetThreadId( GetCurrentThread() ) )
        throw std::runtime_error( "A Thread that does not own a Critical Object trys to unlock it" );
    Thread *tmp = owner;
    if( !--lockCount )
        owner = 0;
    LeaveCriticalSection( &cs );
    if( tmp )
        tmp->removeCriticalLock();
}


// gibt true zurück, wenn das Objekt gesperrt ist
bool Critical::isLocked() const
{
    return lockCount > 0;
}

// gibt einen Zeiger auf den Thread zurück, der das Objekt gesperrt hat
const Thread *Critical::zOwner() const
{
    return owner;
}