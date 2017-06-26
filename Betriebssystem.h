#ifndef Betriebssystem_H
#define Betriebssystem_H
#define MAX_KNOCHEN_ANZ 128
#define _NOHEAP
#ifdef _WIN32

#ifdef _DEBUG

#ifndef _NOHEAP
#ifndef _LTMDB

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _LTMDB

#endif
#endif

#include <assert.h>

#else

#define assert( x )

#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define pthread_t                      void*

#else

#define __stdcall
#define __declspec( x )
#define __int64                        long long
#define __int32                        int
#define __int16                        short
#define __int8                         char
#define assert( x )
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#ifndef CRITICAL_SECTION_CLASS
#define CRITICAL_SECTION_CLASS
class CriticalSection
{
public:
    CriticalSection()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
        pthread_mutex_init( &mutex, &attr );
    } // TODO dsfgdslgkjhlas
	// HACK dsgdshgah
    ~CriticalSection()
    {
        pthread_mutex_destroy( &mutex );
    }
    void Enter()
    {
        pthread_mutex_lock( &mutex );
    }
    void Leave()
    {
        pthread_mutex_unlock( &mutex );
    }
private:
    pthread_mutex_t mutex;
};
#else
class CriticalSection;
#endif
#define GetCurrentThread               pthread_self
#define GetThreadId( x )               x
#define CRITICAL_SECTION               CriticalSection*
#define InitializeCriticalSection( x ) ( *( x ) ) = new CriticalSection()
#define DeleteCriticalSection( x )     delete ( *( x ) )
#define EnterCriticalSection( x )      ( *( x ) )->Enter()
#define LeaveCriticalSection( x )      ( *( x ) )->Leave()
#include <unistd.h>
#define Sleep( x )   usleep( (x) * 1000 )
#define ZeroMemory( Destination, Length ) memset( ( Destination ), 0, ( Length ) )

#endif

#endif