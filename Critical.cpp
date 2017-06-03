#include "Critical.h"
#include "Globals.h"
#include "Thread.h"

using namespace Framework;

// Inhalt der Critical class aus Critical.h
// Konstructor
Critical::Critical()
{
    InitializeCriticalSection( &cs );
}

// Destructor
Critical::~Critical()
{
    DeleteCriticalSection( &cs );
}

// sperrt das Objekt
void Critical::lock()
{
    EnterCriticalSection( &cs );
    if( !owner )
        owner = getThreadRegister()->zThread( GetCurrentThread() );
    if( owner )
        owner->addCriticalLock();
    lockCount++;
}

// versucht das Objekt zu sperren
bool Critical::tryLock()
{
    if( lockCount > 0 )
        return false;
    EnterCriticalSection( &cs );
    if( !owner )
        owner = getThreadRegister()->zThread( GetCurrentThread() );
    if( owner )
        owner->addCriticalLock();
    lockCount++;
    return true;
}

// entsperrt das Objekt
void Critical::unlock()
{
    if( owner && owner->getThreadHandle() != GetCurrentThread() )
        throw std::runtime_error( "A Thread that does not own a Critical Object trys to unlock it" );
    if( owner )
        owner->removeCriticalLock();
    if( !--lockCount )
        owner = 0;
    LeaveCriticalSection( &cs );
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