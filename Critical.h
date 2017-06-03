#pragma once

#include "Betriebssystem.h"

namespace Framework
{
    class Thread;

    class Critical
    {
    private:
        CRITICAL_SECTION cs;
        Thread *owner;
        int lockCount;

    public:
        // Konstructor
        __declspec( dllexport ) Critical();
        // Destructor
        __declspec( dllexport ) ~Critical();
        // sperrt das Objekt
        __declspec( dllexport ) void lock();
        // versucht das Objekt zu sperren
        __declspec( dllexport ) bool tryLock();
        // entsperrt das Objekt
        __declspec( dllexport ) void unlock();

        // gibt true zurück, wenn das Objekt gesperrt ist
        __declspec( dllexport ) bool isLocked() const;
        // gibt einen Zeiger auf den Thread zurück, der das Objekt gesperrt hat
        __declspec( dllexport ) const Thread *zOwner() const;
    };
}