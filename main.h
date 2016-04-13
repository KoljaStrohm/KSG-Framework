#ifndef main_H
#define main_H

#ifdef _DEBUG
#ifndef _LTMDB
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _LTMDB
#endif
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define KSGStart __stdcall

namespace Framework
{
    // Speichert die dem Programm vom Betriebssystem beim Start übergebenen Parameter
	struct Startparam
	{
		HINSTANCE hinst, hpinst;
		LPSTR cmd;
		int show;
	};

    // Überschreibe diese Funktion. Sie wird vom Framework automatisch beim Start des Programmes aufgerufen
    //  p: Die Parameter, die dem Programm beim Start vom Betriebssystem übergeben wurden
	int KSGStart Start( Startparam p );
    // Initialisiert das Framework
    // Wird in der (WinMain) des Frameworks automatisch aufgerufen
	__declspec( dllexport ) void initFramework();
    // Gibt den duch (initFramework) benutzten Arbeitsspeicher wieder frei
    // Wird in der (WinMain) des Frameworks automatisch aufgerufen
	__declspec( dllexport ) void releaseFramework();
}

int WINAPI WinMain( _In_ HINSTANCE hinst, _In_opt_ HINSTANCE hpinst, _In_ LPSTR cmd, int _In_ show )
{
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	Framework::initFramework();
	Framework::Startparam stp;
	stp.hinst = hinst;
	stp.hpinst = hpinst;
	stp.cmd = cmd;
	stp.show = show;
	int ret = Framework::Start( stp );
	Framework::releaseFramework();
	return ret;
}

#endif