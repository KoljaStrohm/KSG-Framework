#ifdef WIN32

#include <Windows.h>
#include <GdiPlus.h>
#pragma comment( lib, "gdiplus.lib" )

#endif
#define Global
#include "Fenster.h"
#include "Maus.h"
#include "Globals.h"
#include "Thread.h"
#include "Model3DList.h"
#include "TexturList.h"

void Framework::initFramework()
{
    if( istInitialisiert )
        return;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, 0 );
    msgExit = 0;
    MausTrack = 1;
    for( int i = 0; i < 255; ++i )
        TastenStand[ i ] = 0;
    for( int i = 0; i < 3; ++i )
        MausStand[ i ] = 0;
    thRegister = new ThreadRegister();
    Model3DList::init();
    m3dRegister = new Model3DList();
    TexturList::init();
    texturRegister = new TexturList();
    istInitialisiert = 1;
}

void Framework::releaseFramework()
{
    if( !istInitialisiert )
        return;
    delete thRegister;
    m3dRegister->release();
    Model3DList::destroy();
    texturRegister->release();
    TexturList::destroy();
    istInitialisiert = 0;
}

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

bool Framework::istThreadOk( Thread *t )
{
    return thRegister->isThread( t );
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