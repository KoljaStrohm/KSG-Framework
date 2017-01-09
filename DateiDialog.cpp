#include "DateiDialog.h"
#include "Text.h"
#include <ShObjIdl.h>
#pragma comment( lib, "Ole32.lib" )
#pragma comment( linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )	

void Framework::InitDialog()
{
    CoInitialize( 0 );
}

using namespace Framework;

// Inhalt der DateiDialog Klasse aus DateiDialog.h
// Konstruktor
DateiDialog::DateiDialog()
{
    typeName = new RCArray< Text >();
    type = new RCArray< Text >();
    fileIndex = 1;
    ref = 1;
}

// Destruktor
DateiDialog::~DateiDialog()
{
    typeName->release();
    type->release();
}

// nicht constant
void DateiDialog::removeDateiTypen()
{
    typeName->leeren();
    type->leeren();
}

void DateiDialog::addDateiTyp( char *name, char *typ )
{
    addDateiTyp( new Text( name ), new Text( typ ) );
}

void DateiDialog::addDateiTyp( Text *name, Text *typ )
{
    typeName->add( name );
    type->add( typ );
}

void DateiDialog::setDateiTypAuswahl( int i )
{
    fileIndex = i + 1;
}

// constant
Text *DateiDialog::anzeigen( bool open ) const
{
    IFileDialog *pfd = NULL;
    CoInitialize( NULL );
    HRESULT hr = 0;
    if( open )
    {
        CoCreateInstance( CLSID_FileOpenDialog,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS( &pfd ) );
    }
    else
    {
        CoCreateInstance( CLSID_FileSaveDialog,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS( &pfd ) );
    }
    if( SUCCEEDED( hr ) )
    {
        DWORD dwFlags;
        hr = pfd->GetOptions( &dwFlags );
        if( SUCCEEDED( hr ) )
        {
            hr = pfd->SetOptions( dwFlags | FOS_FORCEFILESYSTEM );
            if( SUCCEEDED( hr ) )
            {
                int anz = type->getEintragAnzahl();
                COMDLG_FILTERSPEC *c_rgSaveTypes = 0;
                if( !anz )
                {
                    c_rgSaveTypes = new COMDLG_FILTERSPEC[ 1 ];
                    c_rgSaveTypes[ 0 ] = { L"Alle Dateien", L"*.*" };
                    anz = 1;
                }
                else
                {
                    c_rgSaveTypes = new COMDLG_FILTERSPEC[ anz ];
                    for( int i = 0; i < anz; i++ )
                    {
                        wchar_t *n = new wchar_t[ typeName->z( i )->getLength() + 1 ];
#pragma warning( disable : 4996 )
                        mbstowcs( n, typeName->z( i )->getText(), typeName->z( i )->getLength() + 1 );
                        wchar_t *t = new wchar_t[ type->z( i )->getLength() + 1 ];
#pragma warning( disable : 4996 )
                        mbstowcs( t, type->z( i )->getText(), type->z( i )->getLength() + 1 );
                        c_rgSaveTypes[ i ].pszName = n;
                        c_rgSaveTypes[ i ].pszSpec = t;
                    }
                }
                hr = pfd->SetFileTypes( anz, c_rgSaveTypes );
                if( SUCCEEDED( hr ) )
                {
                    hr = pfd->SetFileTypeIndex( fileIndex );
                    if( SUCCEEDED( hr ) )
                    {
                        Text txt = "";
                        for( int i = 0; i < anz; i++ )
                        {
                            if( !type->z( i )->hat( ".*" ) )
                            {
                                txt.append( type->z( i )->getTeilText( type->z( i )->positionVon( "." ) + 1 ) );
                                txt += ";";
                            }
                        }
                        if( txt.getLength() > 0 )
                            txt.remove( txt.getLength() - 1 );
                        wchar_t *defEnd = new wchar_t[ txt.getLength() + 1 ];
#pragma warning( disable : 4996 )
                        mbstowcs( defEnd, txt, txt.getLength() + 1 );
                        hr = pfd->SetDefaultExtension( defEnd );
                        if( SUCCEEDED( hr ) )
                        {
                            hr = pfd->Show( NULL );
                            if( SUCCEEDED( hr ) )
                            {
                                IShellItem *psiResult;
                                hr = pfd->GetResult( &psiResult );
                                if( SUCCEEDED( hr ) )
                                {
                                    PWSTR pszFilePath = NULL;
                                    hr = psiResult->GetDisplayName( SIGDN_FILESYSPATH,
                                                                    &pszFilePath );
                                    if( SUCCEEDED( hr ) )
                                    {
                                        char *txt = new char[ wcslen( pszFilePath ) + 1 ];
#pragma warning( disable : 4996 )
                                        wcstombs( txt, pszFilePath, wcslen( pszFilePath ) + 1 );
                                        Text *ret = new Text( txt );
                                        delete[] txt;
                                        psiResult->Release();
                                        delete[] defEnd;
                                        for( int i = 0; i < anz; i++ )
                                        {
                                            delete[] c_rgSaveTypes[ i ].pszName;
                                            delete[] c_rgSaveTypes[ i ].pszSpec;
                                        }
                                        delete[] c_rgSaveTypes;
                                        pfd->Release();
                                        return ret;
                                    }
                                    psiResult->Release();
                                }
                            }
                        }
                        delete[] defEnd;
                    }
                }
                for( int i = 0; i < anz; i++ )
                {
                    delete[] c_rgSaveTypes[ i ].pszName;
                    delete[] c_rgSaveTypes[ i ].pszSpec;
                }
                delete[] c_rgSaveTypes;
            }
        }
        pfd->Release();
    }
    return 0;
}

// Reference Counting
DateiDialog *DateiDialog::getThis()
{
    ref++;
    return this;
}

DateiDialog *DateiDialog::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der DateiDialogTh Klasse aus DateiDialog.h
// Konstruktor
DateiDialogTh::DateiDialogTh()
{
    dialog = new DateiDialog();
    ret = 0;
    open = 0;
    ref = 1;
}

// Destruktor
DateiDialogTh::~DateiDialogTh()
{
    if( run )
    {
        warteAufThread( 5000 );
        if( run )
            ende();
    }
    dialog->release();
    if( ret )
        ret->release();
}

// nicht constant
void DateiDialogTh::setOpen( bool b )
{
    open = b;
}

void DateiDialogTh::removeDateiTypen()
{
    dialog->removeDateiTypen();
}

void DateiDialogTh::addDateiTyp( char *name, char *typ )
{
    dialog->addDateiTyp( name, typ );
}

void DateiDialogTh::addDateiTyp( Text *name, Text *typ )
{
    dialog->addDateiTyp( name, typ );
}

void DateiDialogTh::setDateiTypAuswahl( int i )
{
    dialog->setDateiTypAuswahl( i );
}

void DateiDialogTh::thread()
{
    if( ret )
        ret = ret->release();
    ret = dialog->anzeigen( open );
}

// constant
Text *DateiDialogTh::getPfad() const
{
    return ret ? ret->getThis() : 0;
}

Text *DateiDialogTh::zPfad() const
{
    return ret;
}

// Refernece Counting
DateiDialogTh *DateiDialogTh::getThis()
{
    ref++;
    return this;
}

DateiDialogTh *DateiDialogTh::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}