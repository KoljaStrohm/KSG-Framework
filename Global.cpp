#ifdef WIN32

#include <Windows.h>
#include <GdiPlus.h>
#pragma comment( lib, "gdiplus.lib" )
#include "Fenster.h"
#include "Maus.h"
#include "Model3DList.h"
#include "TexturList.h"

#endif
#define Global
#include "Globals.h"
#include "Thread.h"

void Framework::initFramework()
{
    if( istInitialisiert )
        return;
#ifdef WIN32
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, 0 );
    msgExit = 0;
    MausTrack = 1;
    for( int i = 0; i < 255; ++i )
        TastenStand[ i ] = 0;
    for( int i = 0; i < 3; ++i )
        MausStand[ i ] = 0;
    Model3DList::init();
    m3dRegister = new Model3DList();
    TexturList::init();
    texturRegister = new TexturList();
#endif
    istInitialisiert = 1;
    thRegister = new ThreadRegister();
}

void Framework::releaseFramework()
{
    if( !istInitialisiert )
        return;
    thRegister->cleanUpClosedThreads();
    delete thRegister;
#ifdef WIN32
    m3dRegister->release();
    Model3DList::destroy();
    texturRegister->release();
    TexturList::destroy();
#endif
    istInitialisiert = 0;
}

bool Framework::istThreadOk( Thread *t )
{
    return thRegister->isThread( t );
}

// Gibt das Thread Register des Frameworks zurück
Framework::ThreadRegister *Framework::getThreadRegister()
{
    return thRegister;
}

#ifdef WIN32

const Framework::Punkt &Framework::getMausPos()
{
    return mausPos;
}

bool Framework::getMausStand( int taste )
{
    return MausStand[ taste ];
}

void Framework::setTastenStand( unsigned char taste, bool st )
{
    TastenStand[ taste ] = st;
}

bool Framework::getTastenStand( unsigned char taste )
{
    return TastenStand[ taste ];
}

// Gibt das Model3DData Register des Frameworks ohne erhöhten reference Counter zurück
Framework::Model3DList *Framework::zM3DRegister()
{
    return m3dRegister;
}

// Gibt das Textur Register des Frameworks ohne erhöhten reference Counter zurück
Framework::TexturList *Framework::zTexturRegister()
{
    return texturRegister;
}

#endif