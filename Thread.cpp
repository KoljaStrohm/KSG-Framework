#include "Thread.h"
#include "Globals.h"

using namespace Framework;

// inhalt der Thread Klasse aus Thread.h
// Konstruktor 
Thread::Thread()
{
    threadHandleSys = 0;
    threadHandle = 0;
    threadId = 0;
    thRegister->add( this );
    run = 0;
}

// Destruktor 
Thread::~Thread()
{
    thRegister->remove( this );
#ifdef WIN32
    if( GetCurrentThreadId() == GetThreadId( threadHandle ) )
        return;
#else
    if( pthread_self() == threadHandle )
        return;
#endif
    if( run )
        warteAufThread( 100000 );
    if( run )
        ende();
}

// nicht constant 
void Thread::start() // startet den Thread
{
    if( !run )
    {
#ifdef WIN32
        threadHandle = CreateThread( 0, 0, threadStart, this, 0, &threadId );
#else
        pthread_create( &threadHandle, NULL, threadStart, this );
#endif
    }
    run = 1;
}
#ifdef WIN32
void Thread::pause() // pausiert den Thread
{
    if( run )
        SuspendThread( threadHandle );
    run = 0;
}

void Thread::fortsetzen() // pausiert den Thread
{
    if( !run )
        ResumeThread( threadHandle );
    run = 1;
}
#endif
void Thread::ende() // beendet den Thread
{
    if( run )
    {
#ifdef WIN32
#pragma warning(suppress: 6258)
        TerminateThread( threadHandle, 0 );
#else
        if( pthread_self() == threadHandle )
        {
            thRegister->addClosedThread( threadHandle );
            run = 0;
        }
        pthread_cancel( threadHandle );
#endif
    }
    run = 0;
}

void Thread::thread() // Thread
{}

void Thread::threadEnd()
{
    run = 0;
}

// constant 
bool Thread::isRunning() const // prüft, ob der Thrad aktiv ist
{
    return run;
}

int Thread::warteAufThread( int zeit ) // wartet zeit lang auf den Thread
{
#ifdef WIN32
    if( !run )
        return WAIT_OBJECT_0;
    if( GetCurrentThreadId() == GetThreadId( threadHandle ) )
        return WAIT_OBJECT_0;
    return WaitForSingleObject( threadHandle, zeit );
#else
    if( !run )
        return 0;
    if( pthread_self() == threadHandle )
        return 0;
    if( threadHandleSys )
        *threadHandleSys = threadHandle;
    int ret = pthread_join( threadHandle, 0 );
    threadHandle = 0;
    return ret;
#endif
}

// Legt einen Frameworkpointer auf ein Threadhandle fest, der auf 0 gesetzt wird, falls die Ressourcen des Threads bereits follstänfig aufgeräumt wurden
//  ths: Ein Zeiger auf ein Threadhandle, das verändert werden soll
void Thread::setSystemHandlePointer( pthread_t *ths )
{
    threadHandleSys = ths;
    *threadHandleSys = threadHandle;
}

pthread_t Thread::getThreadHandle() const
{
    return threadHandle;
}

// funktionen 
#ifdef WIN32
unsigned long __stdcall Framework::threadStart( void *param )
{
    if( istThreadOk( (Thread *)param ) )
        ( (Thread *)param )->thread();
    if( istThreadOk( (Thread *)param ) )
        ( (Thread *)param )->threadEnd();
    return 0;
}
#else
void *Framework::threadStart( void *param )
{
    pthread_t handle = 0;
    pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, 0 );
    if( istThreadOk( (Thread *)param ) )
    {
        ( (Thread *)param )->setSystemHandlePointer( &handle );
        ( (Thread *)param )->thread();
    }
    if( istThreadOk( (Thread *)param ) )
        ( (Thread *)param )->threadEnd();
    thRegister->addClosedThread( handle );
    pthread_exit( 0 );
    return 0;
}
#endif

// Konstruktor
ThreadRegister::ThreadRegister()
{
    InitializeCriticalSection( &cs );
}

// Destruktor
ThreadRegister::~ThreadRegister()
{
    DeleteCriticalSection( &cs );
}

// Inhalt der ThreadRegister Klasse aus Thread.h
void ThreadRegister::add( Thread *t )
{
    EnterCriticalSection( &cs );
    threads.add( t );
    LeaveCriticalSection( &cs );
}

void ThreadRegister::remove( Thread *t )
{
    EnterCriticalSection( &cs );
    threads.remove( threads.getWertIndex( t ) );
    LeaveCriticalSection( &cs );
}

bool ThreadRegister::isThread( Thread *t )
{
    EnterCriticalSection( &cs );
    bool ret = threads.hat( threads.getWertIndex( t ) );
    LeaveCriticalSection( &cs );
    return ret;
}

void ThreadRegister::addClosedThread( pthread_t handle )
{
    EnterCriticalSection( &cs );
    if( handle )
        closedThreads.add( handle );
    LeaveCriticalSection( &cs );
}

// Löscht die bereits beendetetn Threads und gibt ihre Reccourcen wieder frei
void ThreadRegister::cleanUpClosedThreads()
{
    EnterCriticalSection( &cs );
    while( closedThreads.getEintragAnzahl() > 0 )
    {
#ifndef WIN32
        pthread_join( closedThreads.get( 0 ), 0 );
#endif
        closedThreads.remove( 0 );
    }
    LeaveCriticalSection( &cs );
}