#include "Prozess.h"
#ifdef WIN32
#include <Psapi.h>
#include <tlHelp32.h>
#else
#include <sys/time.h>
#include <fstream>
#include <string>
#endif

using namespace Framework;

// Inhalt der Prozess Klasse aus Prozess.h
// Konstruktor 
Prozess::Prozess()
{
#ifdef WIN32
    pHandle = GetCurrentProcess();
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo( &sysInfo );
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime( &ftime );
    memcpy( &lastCPU, &ftime, sizeof( FILETIME ) );

    GetProcessTimes( pHandle, &ftime, &ftime, &fsys, &fuser );
    memcpy( &lastSysCPU, &fsys, sizeof( FILETIME ) );
    memcpy( &lastUserCPU, &fuser, sizeof( FILETIME ) );
#endif
    ref = 1;
}

// nicht constant 
#ifdef WIN32
void Prozess::setProcess( void *pHandle )
{
    this->pHandle = pHandle;
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo( &sysInfo );
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime( &ftime );
    memcpy( &lastCPU, &ftime, sizeof( FILETIME ) );

    GetProcessTimes( pHandle, &ftime, &ftime, &fsys, &fuser );
    memcpy( &lastSysCPU, &fsys, sizeof( FILETIME ) );
    memcpy( &lastUserCPU, &fuser, sizeof( FILETIME ) );
}
#endif
// constant
double Prozess::getCPU() const
{
#ifdef WIN32
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime( &ftime );
    memcpy( &now, &ftime, sizeof( FILETIME ) );

    GetProcessTimes( pHandle, &ftime, &ftime, &fsys, &fuser );
    memcpy( &sys, &fsys, sizeof( FILETIME ) );
    memcpy( &user, &fuser, sizeof( FILETIME ) );
    percent = (double)( ( sys.QuadPart - lastSysCPU.QuadPart ) +
        ( user.QuadPart - lastUserCPU.QuadPart ) );
    percent /= ( now.QuadPart - lastCPU.QuadPart );
    percent /= numProcessors;
    memcpy( (void*)&lastCPU, (void*)&now, sizeof( now ) );
    memcpy( (void*)&lastUserCPU, (void*)&user, sizeof( now ) );
    memcpy( (void*)&lastSysCPU, (void*)&sys, sizeof( now ) );

    return percent * 100;
#else
    return 0; //usage.ru_stime usage.ru_utime;
#endif
}

__int64 Prozess::getMem() const
{
#ifdef WIN32
    PROCESS_MEMORY_COUNTERS pMemCountr;
    pMemCountr = PROCESS_MEMORY_COUNTERS();
    if( GetProcessMemoryInfo( pHandle, &pMemCountr, sizeof( pMemCountr ) ) )
        return pMemCountr.WorkingSetSize;
    return 0;
#else
    using std::ios_base;
    using std::ifstream;
    using std::string;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream( "/proc/self/stat", ios_base::in );

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
        >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
        >> utime >> stime >> cutime >> cstime >> priority >> nice
        >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf( _SC_PAGE_SIZE ) / 1024; // in case x86-64 is configured to use 2MB pages
    return rss * page_size_kb;
#endif
}
#ifdef WIN32
MemoryInfo Prozess::getMemInfo() const
{
    PROCESS_MEMORY_COUNTERS pMemCountr;
    pMemCountr = PROCESS_MEMORY_COUNTERS();
    int result = GetProcessMemoryInfo( pHandle, &pMemCountr, sizeof( pMemCountr ) );
    MemoryInfo ret;
    ZeroMemory( &ret, sizeof( ret ) );
    if( result )
    {
        ret.ausgelagerteFehler = pMemCountr.PageFaultCount;
        ret.ausgelagerterPool = pMemCountr.QuotaPagedPoolUsage;
        ret.ausgelagerterSpeicher = pMemCountr.WorkingSetSize;
        ret.höchsteAusgelagerterSpeicher = pMemCountr.PeakWorkingSetSize;
        ret.höchsterAusgelagerterPool = pMemCountr.QuotaPeakPagedPoolUsage;
        ret.höchsterNichtAusgelagerterPool = pMemCountr.QuotaPeakNonPagedPoolUsage;
        ret.höchsterVorreservierterSpeicher = pMemCountr.PeakPagefileUsage;
        ret.nichtAusgelagerterPool = pMemCountr.QuotaNonPagedPoolUsage;
        ret.vorreservierterSpeicher = pMemCountr.PagefileUsage;
        return ret;
    }
    return ret;
}

int Prozess::getThreadAnzahl() const
{
    int ret = 0;
    DWORD processID = GetCurrentProcessId();
    HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPALL, processID );
    if( snap != INVALID_HANDLE_VALUE )
    {
        PROCESSENTRY32 entry = { 0 };
        entry.dwSize = sizeof( PROCESSENTRY32 );
        if( Process32First( snap, &entry ) )
        {
            do
            {
                if( entry.th32ProcessID == processID )
                {
                    ret = entry.cntThreads;
                    break;
                }
            } while( Process32Next( snap, &entry ) );
        }
        CloseHandle( snap );
    }
    return ret;
}
#endif
Prozess *Prozess::getThis()
{
    ++ref;
    return this;
}

Prozess *Prozess::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}