#include "Thread.h"
#include "Globals.h"

using namespace Framework;

// inhalt der Thread Klasse aus Thread.h
// Konstruktor 
Thread::Thread()
{
    thRegister->add( this );
#ifdef WIN32
    threadHandle = 0;
    threadId = 0;
#endif
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
bool Thread::läuft() const // prüft, ob der Thrad aktiv ist
{
    return run;
}

int Thread::warteAufThread( int zeit ) const // wartet zeit lang auf den Thread
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
    return pthread_join( threadHandle, 0 );
#endif
}

#ifdef WIN32
void *Thread::getThreadHandle() const
{
    return threadHandle;
}
#endif

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
    pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, 0 );
    ( (Thread *)param )->thread();
    ( (Thread *)param )->threadEnd();
    pthread_exit( 0 );
    return 0;
}
#endif

// Inhalt der ThreadRegister Klasse aus Thread.h
void ThreadRegister::add( Thread *t )
{
    threads.add( t );
}

void ThreadRegister::remove( Thread *t )
{
    threads.lösche( threads.getWertIndex( t ) );
}

bool ThreadRegister::isThread( Thread *t ) const
{
    return threads.hat( threads.getWertIndex( t ) );
}