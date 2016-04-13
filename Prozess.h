#ifndef Prozess_H
#define Prozess_H

#include "Betriebssystem.h"

#ifdef WIN32
#pragma comment( lib, "Psapi.lib" )
#endif

namespace Framework
{
#ifdef WIN32
	struct MemoryInfo; // aus dieser Datei
#endif
	class Prozess; // aus dieser Datei

#ifdef WIN32
	struct MemoryInfo
	{
		unsigned long ausgelagerteFehler;
		__int64 höchsteAusgelagerterSpeicher;
		__int64 ausgelagerterSpeicher;
		__int64 höchsterAusgelagerterPool;
		__int64 ausgelagerterPool;
		__int64 höchsterNichtAusgelagerterPool;
		__int64 nichtAusgelagerterPool;
		__int64 vorreservierterSpeicher;
		__int64 höchsterVorreservierterSpeicher;
	};
#endif
    // Diese Klasse findet informationen über einen laufenden Prozess heraus (CPU, MEM)
    // Bei Ubuntu immer der eigene Prozess
	class Prozess
	{
	private:
#ifdef WIN32
		int numProcessors;
		ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
		void *pHandle;
#endif
		int ref;

	public:
		// Konstruktor 
		__declspec( dllexport ) Prozess();
		// nicht constant 
#ifdef WIN32
        // Setzt den Prozess, der überwacht werden soll (Nur für Windows)
		__declspec( dllexport ) void setProcess( void *pHandle );
#endif
		// Gibt den CPU verbrauch des Prozesses zurück
		__declspec( dllexport ) double getCPU() const;
        // Gibt den Speicherverbrauch des Prozesses zurück
		__declspec( dllexport ) __int64 getMem() const;
#ifdef WIN32
        // Gibt detailierte Informationen über den Speicherverbrauch des Prozesses zurück (Nur für Windows)
		__declspec( dllexport ) MemoryInfo getMemInfo() const;
        // Gibt die Anzahl der Threads zurück (Nur für Windows)
		__declspec( dllexport ) int getThreadAnzahl() const;
#endif
        // Erhöht den Reference Counting Zähler.
        //  return: this.
		__declspec( dllexport ) Prozess *getThis();
        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
		__declspec( dllexport ) Prozess *release();
	};
}

#endif