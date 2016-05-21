#ifndef Thread_H
#define Thread_H

#include "Array.h"

namespace Framework
{
    class Thread;

    // Ein neuer Thread wie die Thread Klasse aus Java
    class Thread
    {
    private:
        pthread_t *threadHandleSys;
    protected:
        unsigned long threadId;
        pthread_t threadHandle;
        bool run;

    public:
        // Konstruktor 
        __declspec( dllexport ) Thread();
        // Destruktor 
        __declspec( dllexport ) virtual ~Thread();
        // Startet den neuen Thread 
        __declspec( dllexport ) void start();
#ifdef WIN32
        // Pausiert den Thread (Nur für Windows)
        __declspec( dllexport ) void pause();
        // Setzt den Thread Fort (Nur für Windows)
        __declspec( dllexport ) void fortsetzen();
#endif
        // Beendet den Thread
        __declspec( dllexport ) void ende();
        // Diese Funktion wird von dem neuen Thread ausgeführt.
        __declspec( dllexport ) virtual void thread();
        // Diese Funktion wird eufgerufen, nachdem die thread Funktion zuende ist
        __declspec( dllexport ) virtual void threadEnd();
        // prüft, ob der Thrad aktiv ist
        // return: true, falls der Thread läuft. 
        //         false, wenn der Thread beendet, pausiert oder noch nicht gestartet wurde.
        __declspec( dllexport ) bool läuft() const;
        // wartet zeit lang auf den Thread
        //  zeit: Die Zeit, die auf den Thread gewartet werden soll. 1000 = 1 Sekunde
        __declspec( dllexport ) int warteAufThread( int zeit );
        // Legt einen Frameworkpointer auf ein Threadhandle fest, der auf 0 gesetzt wird, falls die Ressourcen des Threads bereits follstänfig aufgeräumt wurden
        //  ths: Ein Zeiger auf ein Threadhandle, das verändert werden soll
        void setSystemHandlePointer( pthread_t *ths );
        // Gibt ein Handle auf den Thread zurück
        __declspec( dllexport ) pthread_t getThreadHandle() const;
    };

#ifdef WIN32
    // Diese Funktion wird fon der Thread Klasse benutzt um einen thread zu starten
    __declspec( dllexport ) unsigned long __stdcall threadStart( void *param );
#else
    // Diese Funktion wird fon der Thread Klasse benutzt um einen thread zu starten
    void *threadStart( void *param ); // startet thread
#endif

    // Eine Klasse, die alle aktuell laufenden Thread Objekte speichert
    class ThreadRegister
    {
    private:
        Array< Thread* > threads;
        CRITICAL_SECTION cs;
        Array< pthread_t > closedThreads;

    public:
        // Konstruktor
        ThreadRegister();
        // Destruktor
        ~ThreadRegister();
        // Fügt einen neuen Thread hinzu
        //  t: Der Thread, der hinzugefügt werden soll
        void add( Thread *t );
        // Entfernt einen neuen Thread
        //  t: Der Thread, der entfernt werden soll
        void remove( Thread *t );
        // Überprüft, ob ein Zeiger auf ein gültiges Thread Objekt zeigt, oder ob es schon gelöscht wurde
        //  t: Der Zeiger, der geprüft werden soll
        bool isThread( Thread *t );
        // Setzt Wird automatisch aufgerufen, wenn ein Thread beendet wird. Die Reccourcen werden daraufhin in cleanUpClosedThreads freigegeben.
        //  handle: Das Handle des Threads
        void addClosedThread( pthread_t handle );
        // Löscht die bereits beendetetn Threads und gibt ihre Reccourcen wieder frei
        __declspec( dllexport ) void cleanUpClosedThreads();
    };
}

#endif