#include "Fenster.h"
#include "Text.h"
#include "MausEreignis.h"
#include "TastaturEreignis.h"
#include "Bildschirm.h"
#include "AlphaFeld.h"
#include "Bild.h"
#include "Rahmen.h"
#include "TextFeld.h"
#include "Scroll.h"
#include "Maus.h"
#include "ToolTip.h"
#include "Globals.h"

using namespace Framework;

// Fensterklasse erzeugen
WNDCLASS Framework::F_Normal( HINSTANCE hInst )// Erzeugen einer normalen Fensterklasse
{
	WNDCLASS ret;
	ret.cbClsExtra = 0;
	ret.cbWndExtra = 0;
	ret.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	ret.hCursor = LoadCursor( NULL, IDC_ARROW );
	ret.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	ret.hInstance = hInst;
	ret.lpszMenuName = "";
	ret.lpfnWndProc = WindowProc;
	ret.style = CS_HREDRAW | CS_VREDRAW;
	return ret;
}

WNDCLASSEX Framework::F_NormalEx( HINSTANCE hInst )
{
	WNDCLASSEX ret;
	ret.cbSize = sizeof( WNDCLASSEX );
	ret.cbClsExtra = 0;
	ret.cbWndExtra = 0;
	ret.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	ret.hCursor = LoadCursor( NULL, IDC_ARROW );
	ret.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	ret.hInstance = hInst;
	ret.lpszMenuName = "";
	ret.lpfnWndProc = WindowProc;
	ret.style = CS_HREDRAW | CS_VREDRAW;
	ret.hIconSm = 0;
	return ret;
}

// WinAPI
LRESULT CALLBACK Framework::WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if( msgExit )
		return ( DefWindowProc( hwnd, message, wparam, lparam ) );
	switch( message )
	{
		// Maus Zeiger 
	case WM_SETCURSOR:
		MausZeiger.update();
		return 0;
		// Fenster 
	case WM_SIZE:
		if( wparam == SIZE_RESTORED )
			WFensterA.sendRestoreMessage( hwnd );
		break;
		// Schließen 
	case WM_CLOSE:
		if( WFensterA.sendVSchließMessage( hwnd ) )
			return 0;
		else
			break;
	case WM_DESTROY:
		if( WFensterA.sendNSchließMessage( hwnd ) )
			return 0;
		else
			break;
		// Maus 
	case WM_LBUTTONDOWN: // Linksklick
		if( 1 )
		{
			MausStand[ M_Links ] = 1;
			MausEreignis me = { ME_PLinks, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_RBUTTONDOWN: // Rechtsklick
		if( 1 )
		{
			MausStand[ M_Rechts ] = 1;
			MausEreignis me = { ME_PRechts, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MBUTTONDOWN: // Mittelklick
		if( 1 )
		{
			MausStand[ M_Mitte ] = 1;
			MausEreignis me = { ME_PMitte, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_LBUTTONUP: // Linksrelease
		if( 1 )
		{
			MausStand[ M_Links ] = 0;
			MausEreignis me = { ME_RLinks, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_RBUTTONUP: // Rechtsrelease
		if( 1 )
		{
			MausStand[ M_Rechts ] = 0;
			MausEreignis me = { ME_RRechts, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MBUTTONUP: // Mittelrelease
		if( 1 )
		{
			MausStand[ M_Mitte ] = 0;
			MausEreignis me = { ME_RMitte, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_LBUTTONDBLCLK: // Linksdoppelklick
		if( 1 )
		{
			MausEreignis me = { ME_DKLinks, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_RBUTTONDBLCLK: // Rechtsdoppelklick
		if( 1 )
		{
			MausEreignis me = { ME_DKRechts, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MBUTTONDBLCLK: // Mitteldoppelklick
		if( 1 )
		{
			MausEreignis me = { ME_DKMitte, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MOUSEHOVER: // Maus betritt Fenster
		if( 1 )
		{
			MausTrack = 1;
			MausEreignis me = { ME_Betritt, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MOUSELEAVE: // Maus verlässt Fenster
		if( 1 )
		{
			MausTrack = 1;
			MausEreignis me = { ME_Verlässt, (int)LOWORD( lparam ), (int)HIWORD( lparam ), 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MOUSEMOVE: // Maus wird bewegt
		if( 1 )
		{
			if( mausPos.x == (int)LOWORD( lparam ) && mausPos.y == (int)HIWORD( lparam ) )
				break;
			mausPos.x = (int)LOWORD( lparam );
			mausPos.y = (int)HIWORD( lparam );
			if( MausTrack )
			{
				TRACKMOUSEEVENT lptme;
				lptme.cbSize = sizeof( TRACKMOUSEEVENT );
				lptme.dwFlags = TME_HOVER | TME_LEAVE;
				lptme.dwHoverTime = 0;
				lptme.hwndTrack = hwnd;
				TrackMouseEvent( &lptme );
				MausTrack = 0;
			}
			MausEreignis me = { ME_Bewegung, mausPos.x, mausPos.y, 0, 0, 0 };
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
	case WM_MOUSEWHEEL: // Maus scroll
		if( 1 )
		{
			MausEreignis me = { 0, mausPos.x, mausPos.y, 0, 0, 0 };
			if( (int)(short)HIWORD( wparam ) < 0 )
				me.id = !getTastenStand( T_Shift ) ? ME_DScroll : ME_RScroll;
			else
				me.id = !getTastenStand( T_Shift ) ? ME_UScroll : ME_LScroll;
			me.rmx = me.mx;
			me.rmy = me.my;
			WFensterA.sendMausMessage( hwnd, me );
			break;
		}
		// Tastatur 
	case WM_KEYDOWN:
		if( 1 )
		{
			TastaturEreignis te = { TE_Press, VirtualZuChar( (int)wparam ), 0 };
			if( te.taste == T_Alt_Gr )
				TastenStand[ T_Strg ] = 0;
			TastenStand[ te.taste ] = 1;
			WFensterA.sendTastaturMessage( hwnd, te );
			break;
		} // Taste wird gedrückt
	case WM_KEYUP:
		if( 1 )
		{
			TastaturEreignis te = { TE_Release, VirtualZuChar( (int)wparam ), 0 };
			TastenStand[ te.taste ] = 0;
			WFensterA.sendTastaturMessage( hwnd, te );
			break;
		} // Taste wird losgelassen
	}
	return ( DefWindowProc( hwnd, message, wparam, lparam ) );
}

void Framework::StartNachrichtenSchleife()
{
	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ) > 0 && !msgExit )
	{
		if( !msgExit )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
	msgExit = 0;
}

void Framework::StopNachrichtenSchleife( HWND hwnd )
{
	msgExit = 1;
	PostMessage( hwnd, WM_MOUSELEAVE, 0, 0 );
}

unsigned char Framework::VirtualZuChar( int Virtual )
{
	UINT vk = MapVirtualKey( Virtual, 2 );
	if( !vk )
	{
		switch( Virtual )
		{
		case VK_CAPITAL:
			return T_Caps_Lock;
		case VK_SHIFT:
			return T_Shift;
		case VK_CONTROL:
			return T_Strg;
		case VK_F1:
			return T_F1;
		case VK_F2:
			return T_F2;
		case VK_F3:
			return T_F3;
		case VK_F4:
			return T_F4;
		case VK_F5:
			return T_F5;
		case VK_F6:
			return T_F6;
		case VK_F7:
			return T_F7;
		case VK_F8:
			return T_F8;
		case VK_F9:
			return T_F9;
		case VK_F10:
			return T_F10;
		case VK_F11:
			return T_F11;
		case VK_F12:
			return T_F12;
		case VK_PAUSE:
			return T_Pause;
		case VK_SNAPSHOT:
			return T_Druck;
		case VK_INSERT:
			return T_Einfg;
		case VK_DELETE:
			return T_Entf;
		case VK_LEFT:
			return T_Links;
		case VK_UP:
			return T_Oben;
		case VK_RIGHT:
			return T_Rechts;
		case VK_DOWN:
			return T_Unten;
		case VK_MENU:
			return T_Alt_Gr;
		case VK_NUMLOCK:
			return T_Num;
		case VK_HOME:
			return T_Pos1;
		case VK_PRIOR:
			return T_BildO;
		case VK_NEXT:
			return T_BildU;
		case VK_END:
			return T_Ende;
		case VK_TAB:
			return T_Tab;
		}
	}
	if( vk == VK_TAB )
		return T_Tab;
	char ret = (char)(short)LOWORD( vk );
	short SHIFT = GetKeyState( VK_SHIFT );
	short CAPSLOCK = GetKeyState( VK_CAPITAL );
	short ALTGR = TastenStand[ T_Alt_Gr ];
	if( ALTGR )
	{
		switch( ret )
		{
		case 'q':
			return '@';
		case 'Q':
			return '@';
		case '<':
			return '|';
		case '>':
			return '|';
		case '7':
			return '{';
		case '/':
			return '{';
		case '8':
			return '[';
		case '(':
			return '[';
		case '9':
			return ']';
		case ')':
			return ']';
		case '0':
			return '}';
		case '=':
			return '}';
		case 'ß':
			return '\\';
		case '?':
			return '\\';
		case '+':
			return '~';
		case '*':
			return '~';
		case 'e':
			return '€';
		case 'E':
			return '€';
		case 'm':
			return 'µ';
		case 'M':
			return 'µ';
		case '2':
			return '²';
		case '"':
			return '²';
		case '3':
			return '³';
		case '§':
			return '³';
		}
	}
	SHIFT = HIWORD( SHIFT );
	CAPSLOCK = LOWORD( CAPSLOCK );
	bool up = 0;
	if( SHIFT && !CAPSLOCK )
		up = 1;
	if( CAPSLOCK && !SHIFT )
		up = 1;
	ret = KleinOrGroß( ret, up );
	return ret;
}

// Inhalt der WFenster Klasse aus Fenster.h
// Konstruktor 
WFenster::WFenster()
	: hWnd( 0 ),
	  style( 0 ),
	  makParam( 0 ),
	  sakParam( 0 ),
	  takParam( 0 ),
	  MausAktion( 0 ),
	  VSchließAktion( 0 ),
	  NSchließAktion( 0 ),
	  TastaturAktion( 0 ),
	  screen( 0 ),
	  mx( -1 ),
	  my( -1 ),
	  verschiebbar( 0 ),
	  ref( 1 ),
	  rahmen( 0 ),
	  bitmap( 0 ),
	  hdc( 0 )
{
	WFensterA.addFenster( this );
}

WFenster::WFenster( HWND hwnd )
	: WFenster()
{
	hWnd = hwnd;
}

// Destruktor 
WFenster::~WFenster()
{
	if( WFensterA.removeFenster( this ) )
	{
		WFensterA.del();
	}
    if( hWnd )
        zerstören();
	if( screen )
		screen->release();
}

// nicht constant 
void WFenster::erstellen( int style, WNDCLASS wc ) // Erstellt ein Fenster in Windows
{
	if( !GetClassInfo( wc.hInstance, wc.lpszClassName, &wc ) )
	{
#pragma warning(suppress: 6102)
		if( !RegisterClass( &wc ) ) // Register Fensterklasse
		{
			MessageBox( hWnd, "Fehler beim Registrieren der Fensterklasse!", "Error", MB_ICONERROR ); // Fehlermeldung bei Fehler
			return;
		}
	}

	hWnd = CreateWindow( wc.lpszClassName, wc.lpszClassName, style, 0, 0, 0, 0, 0, 0, wc.hInstance, 0 ); // Fenster Erstellen

	if( hWnd == 0 )
	{
		MessageBox( hWnd, "Fehler beim erstellen des Fensters!", "Error", MB_ICONERROR ); // Fehlermeldung bei Fehler
		return;
	}
	this->style = style;
}

void WFenster::erstellenEx( int exStyle, int style, WNDCLASSEX wc ) // Das Fenster Erstellen
{
	if( !GetClassInfoEx( wc.hInstance, wc.lpszClassName, &wc ) )
	{
#pragma warning(suppress: 6102)
		if( !RegisterClassEx( &wc ) ) // Register Fensterklasse
		{
			MessageBox( hWnd, "Fehler beim Registrieren der Fensterklasse!", "Error", MB_ICONERROR ); // Fehlermeldung bei Fehler
			return;
		}
	}

	hWnd = CreateWindowEx( exStyle, wc.lpszClassName, wc.lpszClassName, style, 0, 0, 0, 0, 0, 0, wc.hInstance, 0 ); // Fenster Erstellen

	if( hWnd == 0 )
	{
		MessageBox( hWnd, "Fehler beim erstellen des Fensters!", "Error", MB_ICONERROR ); // Fehlermeldung bei Fehler
		return;
	}
	this->style = style;
}

void WFenster::setAnzeigeModus( int mod )// Fenster Anzeigen
{
	if( rahmen )
	{
		if( mod == 2 )
			ShowWindow( rahmen, 0 );
		else
			ShowWindow( rahmen, mod );
	}
	ShowWindow( hWnd, mod );
}

bool WFenster::setFokus() // Setzt den Fokus auf das Fenster
{
	DWORD dwCurrentThread = GetCurrentThreadId();
	DWORD dwFGThread = GetWindowThreadProcessId( GetForegroundWindow(), NULL );
	AttachThreadInput( dwCurrentThread, dwFGThread, TRUE );
	SetFocus( hWnd );
	AttachThreadInput( dwCurrentThread, dwFGThread, FALSE );
	SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
	SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
	return GetFocus() == hWnd;
}

void WFenster::setPosition( Punkt &p )// Fenster Position
{
	RECT r;
	GetWindowRect( hWnd, &r ); // Vorherige Position herausfinden
	RECT res;
	res.left = p.x, res.top = p.y, res.right = r.right - r.left, res.bottom = r.bottom - r.top;
	AdjustWindowRect( &res, style, 0 );
	if( res.top < 0 )
	{
		res.bottom -= res.top;
		res.top = 0;
	}
	if( res.left < 0 )
	{
		res.right -= res.left;
		res.left = 0;
	}
	SetWindowPos( hWnd, 0, res.left, res.top, res.right, res.bottom, 0 ); // Position ändern
}

void WFenster::setGröße( Punkt &g )// Fenster Größe
{
	RECT r;
	GetWindowRect( hWnd, &r ); // vorherige Größe herausfinden
	RECT res;
	res.left = r.left, res.top = r.top, res.right = g.x, res.bottom = g.y;
	AdjustWindowRect( &res, style, 0 );
	if( res.top < 0 )
	{
		res.bottom -= res.top;
		res.top = 0;
	}
	if( res.left < 0 )
	{
		res.right -= res.left;
		res.left = 0;
	}
	SetWindowPos( hWnd, 0, res.left, res.top, res.right, res.bottom, 0 ); // Größe ändern
}

void WFenster::setGröße( int breite, int höhe )
{
	RECT r;
	GetWindowRect( hWnd, &r ); // vorherige Größe herausfinden
	RECT res;
	res.left = r.left, res.top = r.top, res.right = breite, res.bottom = höhe;
	AdjustWindowRect( &res, style, 0 );
	if( res.top < 0 )
	{
		res.bottom -= res.top;
		res.top = 0;
	}
	if( res.left < 0 )
	{
		res.right -= res.left;
		res.left = 0;
	}
	SetWindowPos( hWnd, 0, res.left, res.top, res.right, res.bottom, 0 ); // Größe ändern
}

void WFenster::setBounds( Punkt &p, Punkt &g ) // setzt größe und Position
{
	SetWindowPos( hWnd, 0, p.x, p.y, g.x, g.y, 0 ); // Größe und Position ändern
}

void WFenster::setBildschirm( Bildschirm *screen )
{
	if( this->screen )
		this->screen->release();
	this->screen = screen;
}

void WFenster::zerstören()
{
	DestroyWindow( hWnd );
    hWnd = 0;
	if( rahmen )
		DestroyWindow( rahmen );
    rahmen = 0;
	if( bitmap )
		DeleteObject( bitmap );
    bitmap = 0;
	if( hdc )
		DeleteDC( hdc );
    hdc = 0;
}

void WFenster::doMausAktion( MausEreignis &me )
{
	if( !MausAktion || !MausAktion( makParam, this, me ) )
		return;
	if( screen && me.id != ME_Betritt && me.id != ME_Verlässt )
	{
		screen->doMausEreignis( me );
		if( !me.verarbeitet && verschiebbar )
		{
			if( mx != -1 && my != -1 ) // verschieben
			{
				RECT r;
				if( rahmen )
				{
					GetWindowRect( rahmen, &r );
					r.right -= r.left, r.bottom -= r.top;
					r.left += me.mx - mx;
					r.top += me.my - my;
					SetWindowPos( rahmen, 0, r.left, r.top, r.right, r.bottom, 0 ); // Position setzen
				}
				GetWindowRect( hWnd, &r ); // Vorherige Position herausfinden
				r.right -= r.left, r.bottom -= r.top; // Position updaten
				r.left += me.mx - mx;
				r.top += me.my - my;
				SetWindowPos( hWnd, 0, r.left, r.top, r.right, r.bottom, 0 ); // Position setzen

			}
			else if( me.id == ME_PLinks ) // verschieben starten
				mx = me.mx, my = me.my;
			if( me.id == ME_RLinks ) // verschieben beenden
				mx = -1, my = -1;
			me.verarbeitet = 1;
		}
	}
}

void WFenster::doVSchließAktion()
{
	if( !VSchließAktion )
		return;
	VSchließAktion( sakParam, this );
}

void WFenster::doNSchließAktion()
{
	if( !NSchließAktion )
		return;
	NSchließAktion( sakParam, this );
}

void WFenster::doTastaturAktion( TastaturEreignis &te )
{
	if( !TastaturAktion || !TastaturAktion( takParam, this, te ) )
		return;
	if( screen )
		screen->doTastaturEreignis( te );
}

void WFenster::doRestoreMessage() // macht den Rahmen sichtbar
{
	if( rahmen )
		ShowWindow( rahmen, 1 );
	ShowWindow( hWnd, 1 );
}

void WFenster::setMausEreignisParameter( void *p ) // setzt den Parameter vom Maus Ereignis
{
	makParam = p;
}

void WFenster::setSchließEreignisParameter( void *p ) // setzt den Parameter vom Schließ Ereignis
{
	sakParam = p;
}

void WFenster::setTastaturEreignisParameter( void *p ) // setzt den Parameter vom Tastatur Ereignis
{
	takParam = p;
}

void WFenster::setMausAktion( bool( *MausAk )( void *, void *, MausEreignis ) )
{
	MausAktion = MausAk;
}

void WFenster::setVSchließAktion( void( *vSchließAk )( void *, void * ) )
{
	VSchließAktion = vSchließAk;
}

void WFenster::setNSchließAktion( void( *nSchließAk )( void *, void * ) )
{
	NSchließAktion = nSchließAk;
}

void WFenster::setTastaturAktion( bool( *TastaturAk )( void *, void *, TastaturEreignis ) )
{
	TastaturAktion = TastaturAk;
}

void WFenster::setFensterHandle( HWND hWnd ) // setzt das operationsfenster
{
	this->hWnd = hWnd;
}

void WFenster::setVerschiebbar( bool verschiebbar ) // legt fest, ob das Fenster durch ziehen mit Maus verschoben werden kann
{
	this->verschiebbar = verschiebbar;
}

void WFenster::ladeRahmenFenster( Bild *zBild, HINSTANCE hinst ) // setzt einen Transpatenten Rahmen um das Fenster
{
	if( !zBild )
		return;
	// Fenster erstellen
	WNDCLASSEX wcl = { 0 };
	wcl.cbSize = sizeof( wcl );
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hinst;
	wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcl.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcl.hbrBackground = NULL;
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = TEXT( "LayeredWindowClass" );
	wcl.hIconSm = NULL;

	// Bitmap erstellen
	hdc = CreateCompatibleDC( NULL );
	if( !hdc )
		return;
	BITMAPINFO info;
	info.bmiHeader.biSize = sizeof( info.bmiHeader );
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = zBild->getBreite();
	info.bmiHeader.biHeight = -zBild->getHöhe();
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biPlanes = 1;
	unsigned char *pPixels = 0;
	bitmap = CreateDIBSection( hdc, &info, DIB_RGB_COLORS, (void**)&pPixels, 0, 0 );
	if( !bitmap )
		DeleteDC( hdc );
	GdiFlush();
	// bitmap füllen
	int pitch = ( ( zBild->getBreite() * 32 + 31 ) & ~31 ) >> 3;
	unsigned char *pRow = 0;
	int *buffer = zBild->getBuffer();
	for( int i = 0; i < zBild->getHöhe(); ++i )
	{
		pRow = &pPixels[ i * pitch ];
		for( int i2 = 0; i2 < zBild->getBreite(); ++i2 )
		{
			pRow[ i2 * 4 ] = (unsigned char)( ( buffer[ i2 + i * zBild->getBreite() ] >> 16 ) & 0xFF );
			pRow[ i2 * 4 + 1 ] = (unsigned char)( ( buffer[ i2 + i * zBild->getBreite() ] >> 8 ) & 0xFF );
			pRow[ i2 * 4 + 2 ] = (unsigned char)( ( buffer[ i2 + i * zBild->getBreite() ] ) & 0xFF );
			pRow[ i2 * 4 + 3 ] = (unsigned char)( ( buffer[ i2 + i * zBild->getBreite() ] >> 24 ) & 0xFF );
		}
	}
	// alpha berechnen
	unsigned char *pPixel = 0;
	if( zBild->getBreite() * 4 == pitch )
	{
		int i = 0;
		int totalBytes = zBild->getBreite() * zBild->getHöhe() * 4;

		for( i = 0; i < totalBytes; i += 4 )
		{
			pPixel = &pPixels[ i ];
			pPixel[ 0 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
			pPixel[ 1 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
			pPixel[ 2 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
		}
	}
	else
	{
		int x = 0;
		int y = 0;

		for( y = 0; y < zBild->getHöhe(); ++y )
		{
			for( x = 0; x < zBild->getBreite(); ++x )
			{
				pPixel = &pPixels[ ( y * pitch ) + ( x * 4 ) ];
				pPixel[ 0 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
				pPixel[ 1 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
				pPixel[ 2 ] *= (unsigned char)( (float)pPixel[ 3 ] / 255.0f );
			}
		}
	}
	// Fenster erstellen fortsetzen
	if( RegisterClassEx( &wcl ) )
	{
		rahmen = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_LAYERED,
								 wcl.lpszClassName,
								 TEXT( "Transparentes Fenster" ),
								 WS_POPUP,
								 0,
								 0,
								 zBild->getBreite(),
								 zBild->getHöhe(),
								 0,
								 0,
								 wcl.hInstance,
								 0
								 );
		if( rahmen )
		{
			// zeichnen
			HDC h = 0;
			if( ( h = GetDC( rahmen ) ) && bitmap )
			{
				HGDIOBJ hPrevObj = NULL;
				POINT ptDest = { 0, 0 };
				POINT ptSrc = { 0, 0 };
				SIZE client = { zBild->getBreite(), zBild->getHöhe() };
				BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

				hPrevObj = SelectObject( hdc, bitmap );
				ClientToScreen( rahmen, &ptDest );

				UpdateLayeredWindow( rahmen, h, &ptDest, &client, hdc, &ptSrc, 0, &blendFunc, ULW_ALPHA );

				SelectObject( hdc, hPrevObj );
				ReleaseDC( rahmen, h );
			}
			UpdateWindow( rahmen );
			Punkt pos = getPosition() + ( getGröße() - zBild->getGröße() ) / 2;
			SetWindowPos( rahmen, 0, pos.x, pos.y, zBild->getBreite(), zBild->getHöhe(), 0 );
		}
	}
}

// constant 
Punkt WFenster::getPosition() const // gibt die Position zurück
{
	RECT r;
	GetWindowRect( hWnd, &r ); // Position herausfinden
	return{ r.left, r.top };
}

Punkt WFenster::getGröße() const // gibt die Größe zurück
{
	RECT r;
	GetWindowRect( hWnd, &r ); // Größe herausfinden
	return{ r.right - r.left, r.bottom - r.top };
}

Punkt WFenster::getKörperGröße() const // gibt die Fenster Körpergröße zurück
{
	RECT r;
	GetClientRect( hWnd, &r ); // Größe herausfinden
	return{ r.right - r.left, r.bottom - r.top };
}

int WFenster::getKörperBreite() const // gibt die Fenster Körperbreite zurück
{
	RECT r;
	GetClientRect( hWnd, &r ); // Größe herausfinden
	return r.right;
}

int WFenster::getKörperHöhe() const // gibt die Fenster Körperhöhe zurück
{
	RECT r;
	GetClientRect( hWnd, &r ); // Größe herausfinden
	return r.bottom;
}

HWND WFenster::getFensterHandle() const // gibt ein Handle zum Fenster zurück
{
	return hWnd;
}

bool WFenster::hatMausAktion() const
{
	return MausAktion != 0;
}

bool WFenster::hatVSchließAktion() const
{
	return VSchließAktion != 0;
}

bool WFenster::hatNSchließAktion() const
{
	return NSchließAktion != 0;
}

bool WFenster::hatTastaturAktion() const
{
	return TastaturAktion != 0;
}

Bildschirm *WFenster::getBildschirm() const
{
	if( !screen )
		return 0;
	return screen->getThis();
}

Bildschirm *WFenster::zBildschirm() const
{
	return screen;
}

bool WFenster::istVerschiebbar() const // prüft, ob das Fenster durch ziehen mit Maus verschoben werden kann
{
	return verschiebbar;
}

// Reference Counting
WFenster *WFenster::getThis()
{
	++ref;
	return this;
}

WFenster *WFenster::release()
{
	--ref;
	if( ref < 1 )
		delete this;
	return 0;
}

// Inhalt der WFensterArray Klasse aus Fenster.h
// Konstruktor 
WFensterArray::WFensterArray()
	: next( 0 ),
	  This( 0 )
{
}

// Destruktor 
WFensterArray::~WFensterArray()
{
	if( next )
		delete next;
}

// add und remove 
bool WFensterArray::addFenster( WFenster *fenster )
{
	if( fenster == This )
		return 0;
	if( !This )
	{
		This = fenster;
		return 1;
	}
	if( !next )
		next = new WFensterArray();
	return next->addFenster( fenster );
}

bool WFensterArray::removeFenster( WFenster *fenster )
{
	if( fenster == This )
		return 1;
	if( !next )
		return 0;
	if( next->removeFenster( fenster ) )
	{
		WFensterArray *tmp = next->getNext();
		next->setNext0();
		delete next;
		next = tmp;
	}
	return 0;
}

WFensterArray *WFensterArray::getNext()
{
	return next;
}

void WFensterArray::setNext0()
{
	next = 0;
}

void WFensterArray::del()
{
	if( next )
	{
		This = next->getThis();
		WFensterArray *tmp = next->getNext();
		next->setNext0();
		delete next;
		next = tmp;
	}
	else
		This = 0;
}

// Messages 
bool WFensterArray::sendVSchließMessage( HWND hwnd )
{
	if( !This )
		return 0;
	bool ret = 0;
	if( This->getFensterHandle() == hwnd && This->hatVSchließAktion() )
	{
		This->doVSchließAktion();
		ret = 1;
	}
	if( !next )
		return ret;
	return ret | next->sendVSchließMessage( hwnd );
}

bool WFensterArray::sendNSchließMessage( HWND hwnd )
{
	if( !This )
		return 0;
	bool ret = 0;
	if( This->getFensterHandle() == hwnd && This->hatNSchließAktion() )
	{
		This->doNSchließAktion();
		ret = 1;
	}
	if( !next )
		return ret;
	return ret | next->sendNSchließMessage( hwnd );
}

bool WFensterArray::sendMausMessage( HWND hwnd, MausEreignis &me )
{
	if( !This )
		return 0;
	bool ret = 0;
	if( This->getFensterHandle() == hwnd && This->hatMausAktion() )
	{
		This->doMausAktion( me );
		ret = 1;
	}
	if( !next )
		return ret;
	return ret | next->sendMausMessage( hwnd, me );
}

bool WFensterArray::sendTastaturMessage( HWND hwnd, TastaturEreignis &te )
{
	if( !This )
		return 0;
	bool ret = 0;
	if( This->getFensterHandle() == hwnd && This->hatTastaturAktion() )
	{
		This->doTastaturAktion( te );
		ret = 1;
	}
	if( !next )
		return ret;
	return ret | next->sendTastaturMessage( hwnd, te );
}

bool WFensterArray::sendRestoreMessage( HWND hwnd )
{
	if( !This )
		return 0;
	bool ret = 0;
	if( This->getFensterHandle() == hwnd && This->hatMausAktion() )
	{
		This->doRestoreMessage();
		ret = 1;
	}
	if( !next )
		return ret;
	return ret | next->sendRestoreMessage( hwnd );
}

WFenster *WFensterArray::getThis()
{
	return This;
}

// WMessageBox
void Framework::WMessageBox( HWND hWnd, Text *titel, Text *meldung, UINT style )
{
	MessageBox( hWnd, meldung->getText(), titel->getText(), style ); // Message Box
	titel->release();
	meldung->release();
}

// Inhalt der Fenster Klasse aus Fenster.h
// Konstruktor 
Fenster::Fenster()
	: Zeichnung(),
	schließenMe( 0 ),
	schließenMeParam( 0 ),
	rahmen( 0 ),
	titel( 0 ),
	members( 0 ),
	bgKörperFarbe( 0xFF000000 ),
	bgKörperBild( 0 ),
	körperBuffer( 0 ),
	bgSchließFarbe( 0xFF000000 ),
	bgSchließBild( 0 ),
	schließBuffer( 0 ),
	schließKlickBuffer( 0 ),
	vScroll( 0 ),
	hScroll( 0 ),
	kMin( 0, 0 ),
	kMax( 0, 0 ),
	schließKlick( 0 ),
	klick( 0 ),
	mx( 0 ),
	my( 0 ),
	ref( 1 )
{
    style = 0;
	this->setMausEreignis( _ret1ME );
	this->setTastaturEreignis( _ret1TE );
	min = Punkt( 0, 0 ), max = Punkt( 0, 0 );
}

// Destruktor 
Fenster::~Fenster()
{
	if( rahmen )
		rahmen->release();
	if( titel )
		titel->release();
	if( members )
		delete members;
	if( körperBuffer )
		körperBuffer->release();
	if( bgKörperBild )
		bgKörperBild->release();
	if( bgSchließBild )
		bgSchließBild->release();
	if( schließBuffer )
		schließBuffer->release();
	if( schließKlickBuffer )
		schließKlickBuffer->release();
	if( vScroll )
		vScroll->release();
	if( hScroll )
		hScroll->release();
}

// nicht const 
// -- Rahmen -- 
void Fenster::setRahmenZ( LRahmen *ram ) // setzt den rahmen
{
	if( rahmen )
		rahmen->release();
	rahmen = ram;
	rend = 1;
}

void Fenster::setRFarbe( int f ) // setzt die Rahmen Farbe
{
	if( !rahmen )
		rahmen = new LRahmen();
	rahmen->setFarbe( f );
	rend = 1;
}

void Fenster::setRBreite( int br ) // setzt die Breite des Rahmens
{
	if( !rahmen )
		rahmen = new LRahmen();
	rahmen->setRamenBreite( br );
	rend = 1;
}

// -- Titel -- 
void Fenster::setTitel( Text *txt ) // setzt den Titel
{
	if( !titel )
		titel = new TextFeld();
	titel->setText( txt );
	rend = 1;
}

void Fenster::setTitelZ( Text *txt )
{
	if( !titel )
		titel = new TextFeld();
	titel->setTextZ( txt );
	rend = 1;
}

void Fenster::setTitel( const char *txt )
{
	if( !titel )
		titel = new TextFeld();
	titel->setText( txt );
	rend = 1;
}

void Fenster::setTTextFeldZ( TextFeld *tf ) // setzt das Titel TextFeld
{
	if( titel )
		titel->release();
	titel = tf;
	rend = 1;
}

// -- Schrift -- 
void Fenster::setTSchriftZ( Schrift *schrift ) // setzt die Titel Schrift
{
	if( !titel )
		titel = new TextFeld();
	titel->setSchriftZ( schrift );
	rend = 1;
}

void Fenster::setTSFarbe( int f ) // setzt die Titel Schrift Farbe
{
	if( !titel )
		titel = new TextFeld();
	titel->setSchriftFarbe( f );
	rend = 1;
}

void Fenster::setTSGröße( int gr ) // setzt die Titel Schrift Größe
{
	if( !titel )
		titel = new TextFeld();
	titel->setSchriftGröße( gr );
	rend = 1;
}

// -- Titel Hintergrund -- 
void Fenster::setTBgFarbe( int f ) // setzt Titel Hintergrund farbe
{
	if( !titel )
		titel = new TextFeld();
	titel->setHintergrundFarbe( f );
	rend = 1;
}

// -- Titel AlphaFeld -- 
void Fenster::setTAlphaFeldZ( AlphaFeld *af ) // setzt das Titel AlphaFeld
{
	if( !titel )
		titel = new TextFeld();
	titel->setAlphaFeldZ( af );
	rend = 1;
}

void Fenster::setTAfFarbe( int f ) // setzt die Titel AlphFeld Farbe
{
	if( !titel )
		titel = new TextFeld();
	titel->setAlphaFeldFarbe( f );
	rend = 1;
}

void Fenster::setTAfStärke( int st ) // setzt die Stärke des Titel AlphaFeldes
{
	if( !titel )
		titel = new TextFeld();
	titel->setAlphaFeldStärke( st );
	rend = 1;
}

// -- Titel Hintergrund Bild -- 
void Fenster::setTBgBild( Bild *b ) // setzt das Titel Hintergrund Bild
{
	if( !titel )
		titel = new TextFeld();
	titel->setHintergrundBild( b );
	rend = 1;
}

void Fenster::setTBgBildZ( Bild *b )
{
	if( !titel )
		titel = new TextFeld();
	titel->setHintergrundBildZ( b );
	rend = 1;
}

// -- Titel Rahmen -- 
void Fenster::setTRahmenZ( LRahmen *ram ) // set Titel Rahmen
{
	if( !titel )
		titel = new TextFeld();
	titel->setLinienRahmenZ( ram );
	rend = 1;
}

void Fenster::setTRFarbe( int f ) // setzt die Titel Rahmen Farbe
{
	if( !titel )
		titel = new TextFeld();
	titel->setLinienRahmenFarbe( f );
	rend = 1;
}

void Fenster::setTRBreite( int br ) // setzt die Titel Rahmen Breite
{
	if( !titel )
		titel = new TextFeld();
	titel->setLinienRahmenBreite( br );
	rend = 1;
}

// -- Körper Hintergrund -- 
void Fenster::setKBgFarbe( int f ) // setzt die Körper Hintergrund Farbe
{
	bgKörperFarbe = f;
	rend = 1;
}

// -- Körper Hintergrund Bild -- 
void Fenster::setKBgBild( Bild *b ) // setzt das Körper Hintergrund Bild
{
	if( !bgKörperBild )
		bgKörperBild = new Bild();
	bgKörperBild->neuBild( b->getBreite(), b->getHöhe(), 0 );
	int *buff1 = bgKörperBild->getBuffer();
	int *buff2 = b->getBuffer();
	int gr = bgKörperBild->getBreite() * bgKörperBild->getHöhe();
	for( int i = 0; i < gr; ++i )
		buff1[ i ] = buff2[ i ];
	b->release();
	rend = 1;
}

void Fenster::setKBgBildZ( Bild *b )
{
	if( bgKörperBild )
		bgKörperBild->release();
	bgKörperBild = b;
	rend = 1;
}

// -- Körper AlphaFeld -- 
void Fenster::setKAlphaFeldZ( AlphaFeld *af ) // setzt das Körper AlphaFeld
{
	if( körperBuffer )
		körperBuffer->release();
	körperBuffer = af;
	rend = 1;
}

void Fenster::setKAfFarbe( int f ) // setzt Körper AlphaFeld Farbe
{
	if( !körperBuffer )
		körperBuffer = new AlphaFeld();
	körperBuffer->setFarbe( f );
	rend = 1;
}

void Fenster::setKAfStärke( int st ) // setzt die Stärke des Körper AlphaFeldes
{
	if( !körperBuffer )
		körperBuffer = new AlphaFeld();
	körperBuffer->setStärke( st );
	rend = 1;
}

// -- Schließen --
void Fenster::setSchließenMeParam( void *param )
{
	schließenMeParam = param;
}

void Fenster::setSchließenMe( bool( *schließenMe )( void *, void *, MausEreignis ) ) // setzt das Schließen Mausereignis
{
	this->schließenMe = schließenMe;
}

// -- Schließen Hintergrund -- 
void Fenster::setSBgFarbe( int f ) // setzt die Schließ Hintergrund Farbe
{
	bgSchließFarbe = f;
	rend = 1;
}

// -- Schließen Hintergrund Bild -- 
void Fenster::setSBgBild( Bild *b ) // setzt das Schließ Hintergrund Bild
{
	if( !bgSchließBild )
		bgSchließBild = new Bild();
	bgSchließBild->neuBild( b->getBreite(), b->getHöhe(), 0 );
	int *buff1 = bgSchließBild->getBuffer();
	int *buff2 = b->getBuffer();
	int gr = bgSchließBild->getBreite() * bgSchließBild->getHöhe();
	for( int i = 0; i < gr; ++i )
		buff1[ i ] = buff2[ i ];
	b->release();
	rend = 1;
}

void Fenster::setSBgBildZ( Bild *b )
{
	if( bgSchließBild )
		bgSchließBild->release();
	bgSchließBild = b;
	rend = 1;
}

// -- Schließen AlphaFeld -- 
void Fenster::setSAlphaFeldZ( AlphaFeld *af ) // setzt das Schließ AlphaFeld
{
	if( schließBuffer )
		schließBuffer->release();
	schließBuffer = af;
	rend = 1;
}

void Fenster::setSAfFarbe( int f ) // setzt die Farbe des Schließ AlphaFeldes
{
	if( !schließBuffer )
		schließBuffer = new AlphaFeld();
	schließBuffer->setFarbe( f );
	rend = 1;
}

void Fenster::setSAfStärke( int st ) // setzt die Stärke des Schließ AlphaFeldes
{
	if( !schließBuffer )
		schließBuffer = new AlphaFeld();
	schließBuffer->setStärke( st );
	rend = 1;
}

// -- Schließen Klick AlphaFeld -- 
void Fenster::setSKAlphaFeldZ( AlphaFeld *af ) // setzt das Schließ klick AlphaFeld
{
	if( schließKlickBuffer )
		schließKlickBuffer->release();
	schließKlickBuffer = af;
	rend = 1;
}

void Fenster::setSKAfFarbe( int f ) // setzt die Farbe des Schließ klick AlphaFeldes
{
	if( !schließKlickBuffer )
		schließKlickBuffer = new AlphaFeld();
	schließKlickBuffer->setFarbe( f );
	rend = 1;
}

void Fenster::setSKAfStärke( int st ) // setzt die Stärke des Schließ klick AlphaFeldes
{
	if( !schließKlickBuffer )
		schließKlickBuffer = new AlphaFeld();
	schließKlickBuffer->setStärke( st );
	rend = 1;
}

// -- min max -- 
void Fenster::setMin( int mx, int my ) // setzt die Mindest Fenster Größe
{
	min.x = mx;
	min.y = my;
}

void Fenster::setMin( const Punkt &min )
{
	this->min = min;
}

void Fenster::setMax( int mx, int my ) // setzt die Maximale Fenster Größe
{
	max.x = mx;
	max.y = my;
}

void Fenster::setMax( const Punkt &max )
{
	this->max = max;
}

void Fenster::setKMin( int mx, int my ) // setzt die Mindest Körper Größe
{
	kMin.x = mx;
	kMin.y = my;
}

void Fenster::setKMin( const Punkt &min )
{
	kMin = min;
}

void Fenster::setKMax( int mx, int my ) // setzt die Maximale Körper Größe
{
	kMax.x = mx;
	kMax.y = my;
}

void Fenster::setKMax( const Punkt &max )
{
	kMax = max;
}

// -- scroll -- 
void Fenster::setHScrollBarZ( HScrollBar *hScroll ) // setzt die Horizontale Scroll Bar
{
	if( this->hScroll )
		this->hScroll->release();
	this->hScroll = hScroll;
	rend = 1;
}

void Fenster::setVScrollBarZ( VScrollBar *vScroll ) // setzt die Vertikale Scroll BAr
{
	if( this->vScroll )
		this->vScroll->release();
	this->vScroll = vScroll;
	rend = 1;
}

void Fenster::setHSBMax( int max ) // setzt das Scroll Maximum
{
	if( !hScroll )
		hScroll = new HScrollBar();
	int rbr = 0;
	if( hatStyle( Style::Rahmen ) && rahmen )
		rbr = rahmen->getRBreite();
	int vsh = 0;
	if( hatStyle( Style::VScroll ) && vScroll )
		vsh = 15;
	hScroll->update( max, gr.x - rbr * 2 - vsh );
	rend = 1;
}

void Fenster::setVSBMax( int max )
{
	if( !vScroll )
		vScroll = new VScrollBar();
	int rbr = 0;
	int th = 0;
	if( hatStyle( Style::Rahmen ) && rahmen )
		rbr = rahmen->getRBreite();
	if( hatStyle( Style::Titel ) && titel )
		th = titel->getHöhe();
	int hsh = 0;
	if( hatStyle( Style::HScroll ) && hScroll )
		hsh = 15;
	vScroll->update( max, gr.y - rbr * 2 - th - hsh );
	rend = 1;
}

void Fenster::setHSBScroll( int scroll ) // setzt die momentane Scroll Position
{
	if( !hScroll )
		hScroll = new HScrollBar();
	hScroll->scroll( scroll );
	rend = 1;
}

void Fenster::setVSBScroll( int scroll )
{
	if( !vScroll )
		vScroll = new VScrollBar();
	vScroll->scroll( scroll );
	rend = 1;
}

// -- Members -- 
void Fenster::addMember( Zeichnung *obj ) // fügt einen Member hinzu
{
	if( !members )
		members = new ZeichnungArray();
	members->addZeichnung( obj );
	members->updateIndex( 0 );
	rend = 1;
}

void Fenster::removeMember( Zeichnung *obj ) // entfernt einen Member
{
	if( !members )
		return;
	members->removeZeichnung( obj );
	members->updateIndex( 0 );
	rend = 1;
}

// -- Messages -- 
bool Fenster::tick( double tickval ) // tick
{
	if( members && hatStyle( Style::Sichtbar ) )
		rend |= members->tick( tickval );
	else if( members )
		members->tick( tickval );
	if( vScroll && hatStyle( Style::VScroll ) )
		rend |= vScroll->getRend();
	if( hScroll && hatStyle( Style::HScroll ) )
		rend |= hScroll->getRend();
	bool ret = rend;
	rend = 0;
	return ret;
}

void Fenster::doMausEreignis( MausEreignis &me )
{
    bool nmakc = !me.verarbeitet;
	if( me.verarbeitet || ( me.mx < pos.x || me.mx > pos.x + gr.x || me.my < pos.y || me.my > pos.y + gr.y ) )
	{
		if( mausIn )
		{
			mausIn = 0;
			MausEreignis me2;
			me2.id = ME_Verlässt;
			me2.mx = me.mx;
			me2.my = me.my;
			me2.verarbeitet = 0;
			doMausEreignis( me2 );
			return;
		}
	}
	else if( !mausIn && me.id != ME_Verlässt )
	{
		mausIn = 1;
		MausEreignis me2;
		me2.id = ME_Betritt;
		me2.mx = me.mx;
		me2.my = me.my;
		me2.verarbeitet = 0;
		doMausEreignis( me2 );
	}
	if( hatStyle( Style::Sichtbar ) )
	{
		bool mvtmp = me.verarbeitet;
		if( !Mak )
			return;
		if( hatStyleNicht( Style::Erlaubt ) )
			me.verarbeitet = 1;
		bool mpr = 0;
		me.mx -= pos.x;
		me.my -= pos.y;
		int rbr = 0;
		if( hatStyle( Style::Rahmen ) && rahmen )
			rbr = rahmen->getRBreite();
		int th = 0;
		if( hatStyle( Style::Titel ) && titel )
			th = titel->getHöhe();
		bool hSc = hatStyle( Style::HScroll ) && hScroll;
		bool vSc = hatStyle( Style::VScroll ) && vScroll;
		if( !me.verarbeitet && me.id == ME_Bewegung && klick )
		{
			if( hatStyle( Style::Beweglich ) || hatStyle( Style::HöheÄnderbar ) || hatStyle( Style::BreiteÄnderbar ) || hatStyle( Style::TitelHöheÄnderbar ) )
			{
				if( Mak( makParam, this, me ) )
				{
					mpr = 1;
					bool ret1 = 0;
					bool mset = 0;
					int schö = 0;
					int scbr = 0;
					if( hSc )
					{
						schö = 15;
						scbr = 40;
					}
					if( vSc )
					{
						scbr += 15;
						schö = 40;
					}
					bool minXb, maxXb, minYb, maxYb;
					bool kMinXb, kMaxXb, kMinYb, kMaxYb;
					minXb = hatStyle( Style::MinBr );
					maxXb = hatStyle( Style::MaxBr );
					minYb = hatStyle( Style::MinHö );
					maxYb = hatStyle( Style::MaxHö );
					kMinXb = hatStyle( Style::Körper_minBr );
					kMaxXb = hatStyle( Style::Körper_maxBr );
					kMinYb = hatStyle( Style::Körper_minHö );
					kMaxYb = hatStyle( Style::Körper_maxHö );
					int fMinBr = rbr * 2 + scbr;
					if( minXb )
						fMinBr = fMinBr < min.x ? min.x : fMinBr;
					if( kMinXb )
						fMinBr = fMinBr < ( rbr * 2 + kMin.x + scbr ) ? ( rbr * 2 + kMin.x + scbr ) : fMinBr;
					int fMinHö = rbr * 2 + th + schö;
					if( minYb )
						fMinHö = fMinHö < min.y ? min.y : fMinHö;
					if( kMinYb )
						fMinHö = fMinHö < ( rbr * 2 + kMin.y + th + schö ) ? ( rbr * 2 + kMin.y + th + schö ) : fMinHö;
					int fMaxBr = 0;
					if( maxXb )
						fMaxBr = max.x;
					if( kMaxXb )
						fMaxBr = fMaxBr < ( rbr * 2 + kMax.x + scbr ) ? ( rbr * 2 + kMax.x + scbr ) : fMaxBr;
					int fMaxHö = 0;
					if( maxYb )
						fMaxHö = max.y;
					if( kMaxYb )
						fMaxHö = fMaxHö < ( rbr * 2 + kMax.y + th + schö ) ? ( rbr * 2 + kMax.y + th + schö ) : fMaxHö;
					minXb |= kMinXb, maxXb |= kMaxXb, minYb |= kMinYb, maxYb |= kMaxYb;
					if( hatStyle( Style::HöheÄnderbar ) )
					{
						if( mx > -5 && mx < gr.x + 5 && my > -5 && my < rbr )
						{
							pos.y -= my - me.my;
							gr.y += my - me.my;
							if( gr.y < fMinHö )
							{
								pos.y += fMinHö - gr.y;
								gr.y = fMinHö;
							}
							else if( maxYb && gr.y > fMaxHö )
							{
								pos.y += gr.y - fMaxHö;
								gr.y = fMaxHö;
							}
							else if( vSc )
								vScroll->getScrollData()->anzeige = gr.y;
							rend = 1;
							ret1 = 1;
						}
						else if( mx > -5 && mx < gr.x + 5 && my > gr.y - rbr && my < gr.y + 5 )
						{
							gr.y += me.my - my;
							if( gr.y < fMinHö )
								gr.y = fMinHö;
							else if( maxYb && gr.y > fMaxHö )
								gr.y = fMaxHö;
							else
							{
								mset = 1;
								if( vSc )
									vScroll->getScrollData()->anzeige = gr.y;
							}
							rend = 1;
							ret1 = 1;
						}
					}
					if( hatStyle( Style::BreiteÄnderbar ) )
					{
						if( mx > -5 && mx < rbr && my > -5 && my < gr.y + 5 )
						{
							pos.x -= mx - me.mx;
							gr.x += mx - me.mx;
							if( gr.x < fMinBr )
							{
								pos.x += fMinBr - gr.x;
								gr.x = fMinBr;
							}
							else if( maxXb && gr.x > fMaxBr )
							{
								pos.x += gr.x - fMaxBr;
								gr.x = fMaxBr;
							}
							else if( hSc )
								hScroll->getScrollData()->anzeige = gr.x;
							rend = 1;
							ret1 = 1;
						}
						else if( mx > gr.x - rbr && mx < gr.x + 5 && my > -5 && my < gr.y + 5 )
						{
							gr.x += me.mx - mx;
							if( gr.x < rbr * 2 + fMinBr )
								gr.x = rbr * 2 + fMinBr;
							else if( maxXb && gr.x > fMaxBr )
								gr.x = fMaxBr;
							else
							{
								mset = 1;
								if( hSc )
									hScroll->getScrollData()->anzeige = gr.x;
							}
							rend = 1;
							ret1 = 1;
						}
					}
					if( hatStyle( Style::TitelHöheÄnderbar ) && titel && mx > -5 && mx < gr.x + 5 && my < rbr + th + 5 && my > rbr + th )
					{
						int maxTh = gr.y - rbr * 2 - schö;
						if( kMaxYb )
							maxTh = maxTh < ( gr.x - rbr * 2 - kMin.y ) ? maxTh : ( gr.x - rbr * 2 - kMin.y );
						if( hatStyle( Style::Schließbar ) )
							maxTh = ( gr.x - th - 5 - rbr * 2 - me.my + my ) < 0 ? th : maxTh;
						titel->setGröße( titel->getBreite(), titel->getHöhe() + me.my - my );
						if( titel->getHöhe() > maxTh )
							titel->setGröße( titel->getBreite(), maxTh );
						else if( titel->getHöhe() < 5 )
							titel->setGröße( titel->getBreite(), 5 );
						else
							mset = 1;
						rend = 1;
						ret1 = 1;
					}
					if( ret1 )
					{
						if( mset )
							mx = me.mx, my = me.my;
						me.verarbeitet = 1;
					}
					if( hatStyle( Style::Beweglich ) && mx > rbr && mx < gr.x - th - rbr && my > rbr && my < rbr + th )
					{
						pos.x += me.mx - mx;
						pos.y += me.my - my;
						rend = 1;
						ret1 = 1;
					}
					if( ret1 )
						me.verarbeitet = 1;
				}
			}
		}
		bool inside = me.mx >= 0 && me.mx <= gr.x && me.my >= 0 && me.my <= gr.y;
		if( mpr || me.verarbeitet || ( !inside || Mak( makParam, this, me ) ) )
		{
			if( me.id == ME_RLinks )
			{
				if( schließKlick )
					rend = 1;
				schließKlick = 0, klick = 0;
				mx = -1, my = -1;
			}
			if( me.id == ME_Verlässt )
			{
				if( schließKlick != 0 )
					rend = 1;
				schließKlick = 0, klick = 0;
				mx = -1, my = -1;
			}
			if( !me.verarbeitet )
			{
				if( me.id == ME_PLinks )
				{
					klick = 1;
					mx = me.mx, my = me.my;
				}
				if( hatStyle( Style::Schließbar ) && me.my <= th + rbr && me.mx >= gr.x + rbr - th && me.my >= rbr && me.mx <= gr.x - rbr )
				{
					if( !schließenMe || schließenMe( schließenMeParam, this, me ) )
					{
						if( me.id == ME_PLinks )
						{
							schließKlick = 1;
							rend = 1;
						}
						if( !schließKlick && MausStand[ M_Links ] )
						{
							schließKlick = 1;
							rend = 1;
						}
						me.verarbeitet = 1;
					}
				}
				else if( schließKlick )
				{
					schließKlick = 0;
					rend = 1;
				}
			}
			if( members && me.id != ME_Betritt && me.id != ME_Verlässt )
			{
				if( vSc )
				{
					vScroll->doMausMessage( gr.x - rbr - 16, rbr + th, 15, gr.y - rbr * 2 - th - 1, me );
					if( hSc )
						hScroll->doMausMessage( rbr, gr.y - rbr - 16, gr.x - rbr * 2 - 16, 15, me );
				}
				else
				if( hSc )
					hScroll->doMausMessage( rbr, gr.y - rbr - 16, gr.x - rbr * 2 - 1, 15, me );
				me.mx -= rbr;
				me.my -= rbr + th;
				if( hatStyle( Style::VScroll ) && vScroll )
					me.my += vScroll->getScroll();
				if( hatStyle( Style::HScroll ) && hScroll )
					me.mx += hScroll->getScroll();
				members->sendMausAll( me );
				me.mx += rbr;
				me.my += rbr + th;
                if( hatStyle( Style::VScroll ) && vScroll )
                    me.my -= vScroll->getScroll();
				if( hatStyle( Style::HScroll ) && hScroll )
					me.mx -= hScroll->getScroll();
			}
			if( inside && hatStyleNicht( Style::METransparenz ) )
				me.verarbeitet = 1;
		}
        if( nmakc && me.verarbeitet && nMak )
            me.verarbeitet = nMak( nmakParam, this, me );
		me.mx += pos.x;
		me.my += pos.y;
		if( hatStyleNicht( Style::Erlaubt ) )
			me.verarbeitet = mvtmp;
	}
	else if( members )
	{
		MausEreignis me;
		me.verarbeitet = 1;
		me.id = 0;
		me.mx = -1000000;
		me.my = -1000000;
		members->sendMausAll( me );
	}
}

void Fenster::doTastaturEreignis( TastaturEreignis &te )
{
    bool ntakc = !te.verarbeitet;
	if( hatStyle( Style::Sichtbar ) )
	{
		if( te.verarbeitet )
		{
			if( members )
				members->sendTastaturAll( te );
		}
		else
		{
			if( Tak && Tak( takParam, this, te ) )
			{
				if( members )
					members->sendTastaturAll( te );
			}
		}
	}
    if( ntakc && te.verarbeitet && nTak )
        te.verarbeitet = nTak( ntakParam, this, te );
}

// -- Render -- 
void Fenster::render( Bild &zRObj ) // zeichent nach zRObj
{
	if( hatStyle( Style::Sichtbar ) )
	{
		lockZeichnung();
		if( !zRObj.setDrawOptions( pos, gr ) )
		{
			unlockZeichnung();
			return;
		}
        __super::render( zRObj );
		int rbr = 0;
		if( hatStyle( Style::Rahmen ) && rahmen )
		{
			rahmen->setGröße( gr );
			rahmen->render( zRObj );
			rbr = rahmen->getRBreite();
		}
		int th = 0;
		if( hatStyle( Style::Titel ) && titel )
		{
			titel->setStyle( TextFeld::Style::Hintergrund, hatStyle( Style::TitelHintergrund ) );
			titel->setStyle( TextFeld::Style::HAlpha, hatStyle( Style::TitelHAlpha ) );
			titel->setStyle( TextFeld::Style::HBild, hatStyle( Style::TitelHBild ) );
			titel->setStyle( TextFeld::Style::Buffered, hatStyle( Style::TitelBuffered ) );
			th = titel->getHöhe();
			if( !zRObj.setDrawOptions( rbr, rbr, gr.x - rbr * 2, th ) )
			{
				zRObj.releaseDrawOptions();
				unlockZeichnung();
				return;
			}
			int sbr = 0;
			if( hatStyle( Style::Schließbar ) )
			{
				sbr = th;
				if( hatStyle( Style::SchließHintergrund ) )
				{
					if( hatStyle( Style::SchließHAlpha ) )
						zRObj.alphaRegion( gr.x - th - rbr * 2, 0, th, th, bgSchließFarbe );
					else
						zRObj.füllRegion( gr.x - th - rbr * 2, 0, th, th, bgSchließFarbe );
					if( hatStyle( Style::SchließHBild ) && bgSchließBild )
					{
						if( hatStyle( Style::SchließHAlpha ) )
							zRObj.alphaBild( gr.x - th - rbr * 2, 0, th, th, *bgSchließBild );
						else
							zRObj.drawBild( gr.x - th - rbr * 2, 0, th, th, *bgSchließBild );
					}
				}
				if( !hatStyle( Style::SchließHBild ) || !bgSchließBild )
				{
					zRObj.drawLinie( Punkt( gr.x - th - rbr * 2, 0 ), Punkt( gr.x - rbr * 2, th ), 0xFFFFFFFF );
					zRObj.drawLinie( Punkt( gr.x - rbr * 2, 0 ), Punkt( gr.x - th - rbr * 2, th ), 0xFFFFFFFF );
				}
				if( hatStyle( Style::SchließBuffer ) && schließBuffer )
				{
					schließBuffer->setPosition( gr.x - th - rbr * 2, 0 );
					schließBuffer->setGröße( th, th );
					schließBuffer->render( zRObj );
				}
				if( hatStyle( Style::SchließKlickBuffer ) && schließKlickBuffer && schließKlick )
				{
					schließKlickBuffer->setPosition( gr.x - th - rbr * 2, 0 );
					schließKlickBuffer->setGröße( th, th );
					schließKlickBuffer->render( zRObj );
				}
			}
			titel->setGröße( gr.x - rbr * 2 - sbr, th );
			titel->render( zRObj );
			zRObj.releaseDrawOptions();
		}
		bool vSc = hatStyle( Style::VScroll ) && vScroll;
		bool hSc = hatStyle( Style::HScroll ) && hScroll;
		if( vSc )
		{
			vScroll->render( gr.x - rbr - 16, rbr + th, 15, gr.y - rbr * 2 - th - 1, zRObj );
			if( hSc )
				hScroll->render( rbr, gr.y - rbr - 16, gr.x - rbr * 2 - 16, 15, zRObj );
		}
		else
		if( hSc )
			hScroll->render( rbr, gr.y - rbr - 16, gr.x - rbr * 2 - 1, 15, zRObj );
		int x = rbr;
		int y = rbr + th;
		int br = gr.x - rbr * 2;
		int hö = gr.y - rbr * 2 - th;
		if( vSc )
			br -= 16;
		if( hSc )
			hö -= 16;
		if( !zRObj.setDrawOptions( x, y, br, hö ) )
		{
			zRObj.releaseDrawOptions();
			unlockZeichnung();
			return;
		}
		if( hatStyle( Style::KörperHintergrund ) )
		{
			if( hatStyle( Style::KörperHAlpha ) )
				zRObj.alphaRegion( 0, 0, br, hö, bgKörperFarbe );
			else
				zRObj.füllRegion( 0, 0, br, hö, bgKörperFarbe );
			if( hatStyle( Style::KörperHBild ) && bgKörperBild )
			{
				int *bgBuff = bgKörperBild->getBuffer();
				int bgBr = bgKörperBild->getBreite();
				if( hatStyle( Style::KörperHAlpha ) )
					zRObj.alphaBild( 0, 0, br, hö, *bgKörperBild );
				else
					zRObj.drawBild( 0, 0, br, hö, *bgKörperBild );
			}
		}
		if( hatStyle( Style::KörperBuffered ) && körperBuffer )
		{
			körperBuffer->setGröße( br, hö );
			körperBuffer->render( zRObj );
		}
		if( members )
		{
			if( !vSc && !hSc )
				members->render( zRObj );
			else
			{
				zRObj.addScrollOffset( hSc ? hScroll->getScroll() : 0, vSc ? vScroll->getScroll() : 0 );
				members->render( zRObj );
			}
		}
		zRObj.releaseDrawOptions();
		zRObj.releaseDrawOptions();
		unlockZeichnung();
	}
}

// constant 
// -- Rahmen -- 
LRahmen *Fenster::getRahmen() const // gibt den Rahmen zurück
{
	if( !rahmen )
		return 0;
	return rahmen->getThis();
}

LRahmen *Fenster::zRahmen() const
{
	return rahmen;
}

int Fenster::getRFarbe() const // gibt die Farbe des Rahmens zurück
{
	if( !rahmen )
		return 0;
	return rahmen->getFarbe();
}

int Fenster::getRBreite() const // gibt die breite des Rahmens zurück
{
	if( !rahmen )
		return 0;
	return rahmen->getRBreite();
}

// -- Titel  -- 
Text *Fenster::getTitel() const // gibt den Titel zurück
{
	if( !titel )
		return 0;
	return titel->getText();
}

Text *Fenster::zTitel() const
{
	if( !titel )
		return 0;
	return titel->zText();
}

TextFeld *Fenster::getTTextFeld() const // gibt das Titel TextFeld zurück
{
	if( !titel )
		return 0;
	return titel->getThis();
}

TextFeld *Fenster::zTTextFeld() const
{
	return titel;
}

// -- Titel Schrift -- 
Schrift *Fenster::getTSchrift() const // gibt die Titel Schrift zurück
{
	if( !titel )
		return 0;
	return titel->getSchrift();
}

Schrift *Fenster::zTSchrift() const
{
	if( !titel )
		return 0;
	return titel->zSchrift();
}

int Fenster::getTSFarbe() const // gibt die Titel Schrift Farbe zurück
{
	if( !titel )
		return 0;
	return titel->getSchriftFarbe();
}

int Fenster::getTSGröße() const // gibt die Titel Schrift Größe zurück
{
	if( !titel )
		return 0;
	return titel->getSchriftGröße();
}

// -- Titel Hintergrund -- 
int Fenster::getTBgFarbe() const // gibt die Titel Hintergrund Farbe zurück
{
	if( !titel )
		return 0;
	return titel->getHintergrundFarbe();
}

// -- Titel AlphaFeld -- 
AlphaFeld *Fenster::getTAlphaFeld() const // gibt das Titel AlphaFeld zurück
{
	if( !titel )
		return 0;
	return titel->getAlphaFeld();
}

AlphaFeld *Fenster::zTAlphaFeld() const
{
	if( !titel )
		return 0;
	return titel->zAlphaFeld();
}

int Fenster::getTAfFarbe() const // gibt die Farbe des Titel AlphaFeldes zurück
{
	if( !titel )
		return 0;
	return titel->getAlphaFeldFarbe();
}

int Fenster::getTAfStärke() const // gibt die Stärke des TitelAlphaFeldes zurück
{
	if( !titel )
		return 0;
	return titel->getAlphaFeldStärke();
}

// -- Titel Hintergrund Bild -- 
Bild *Fenster::getTBgBild() const // gibt das Titel Hintergrund Bild zurück
{
	if( !titel )
		return 0;
	return titel->getHintergrundBild();
}

Bild *Fenster::zTBgBild() const
{
	if( !titel )
		return 0;
	return titel->zHintergrundBild();
}

// -- Titel Rahmen -- 
LRahmen *Fenster::getTRahmen() const // gibt den Titel Rahmen zurück
{
	if( !titel )
		return 0;
	return titel->getLinienRahmen();
}

LRahmen *Fenster::zTRahmen() const
{
	if( !titel )
		return 0;
	return titel->zLinienRahmen();
}

int Fenster::getTRFarbe() const // gibt die Farbe des Titel Rahmens zurück
{
	if( !titel )
		return 0;
	return titel->getLinienRahmenFarbe();
}

int Fenster::getTRBreite() const // gibt die Breite des Titel Rahmens zurück
{
	if( !titel )
		return 0;
	return titel->getLinienRahmenBreite();
}

// -- Körper Hintergrund -- 
int Fenster::getKBgFarbe() const // gibt die Körper Hintergrund Farbe zurück
{
	return bgKörperFarbe;
}

// -- Körper Hintergrund Bild -- 
Bild *Fenster::getKBgBild() const // gibt das Körper Hintergrund Bild zurück
{
	if( !bgKörperBild )
		return 0;
	return bgKörperBild->getThis();
}

Bild *Fenster::zKBgBild() const
{
	return bgKörperBild;
}

// -- Körper AlphaFeld -- 
AlphaFeld *Fenster::getKAlphaFeld() const // gibt das Körper AlphaFeld zurück
{
	if( !körperBuffer )
		return 0;
	return körperBuffer->getThis();
}

AlphaFeld *Fenster::zKAlphaFeld() const
{
	return körperBuffer;
}

int Fenster::getKAfFarbe() const // gibt die Farbe des Körper AlphaFeldes zurück
{
	if( !körperBuffer )
		return 0;
	return körperBuffer->getFarbe();
}

int Fenster::getKAfStärke() const // gibt die Stärke des Körper AlphaFeldes zurück
{
	if( !körperBuffer )
		return 0;
	return körperBuffer->getStärke();
}

// -- Schließen Hintergrund -- 
int Fenster::getSBgFarbe() const // gibt die Schließ Hintergrund Farbe zurück
{
	return bgSchließFarbe;
}

// -- Schließen Hintergrund Bild -- 
Bild *Fenster::getSBgBild() const // gibt das Schließ Hintergrund Bild zurück
{
	if( !bgSchließBild )
		return 0;
	return bgSchließBild->getThis();
}

Bild *Fenster::zSBgBild() const
{
	return bgSchließBild;
}

// -- Schließen AlphaFeld -- 
AlphaFeld *Fenster::getSAlphaFeld() const // gibt das Schließ AlphaFeld zurück
{
	if( !schließBuffer )
		return 0;
	return schließBuffer->getThis();
}

AlphaFeld *Fenster::zSAlphaFeld() const
{
	return schließBuffer;
}

int Fenster::getSAfFarbe() const // gibt die Farbe des Schließ AlphaFeldes zurück
{
	if( !schließBuffer )
		return 0;
	return schließBuffer->getFarbe();
}

int Fenster::getSAfStärke() const // gibt die Stärke des Schließ AlphaFeldes zurück
{
	if( !schließBuffer )
		return 0;
	return schließBuffer->getStärke();
}

// -- Schließen Klick AlphaFeld -- 
AlphaFeld *Fenster::getSKAlphaFeld() const // gibt das Schließ Klick AlphaFeld zurück
{
	if( !schließKlickBuffer )
		return 0;
	return schließKlickBuffer->getThis();
}

AlphaFeld *Fenster::zSKAlphaFeld() const
{
	return schließKlickBuffer;
}

int Fenster::getSKAfFarbe() const // gibt die Farbe des Schließ Klick AlphaFeldes zurück
{
	if( !schließKlickBuffer )
		return 0;
	return schließKlickBuffer->getFarbe();
}

int Fenster::getSKAfStärke() const // gibt die Stärke des Schließ Klick AlphaFeldes zurück
{
	if( !schließKlickBuffer )
		return 0;
	return schließKlickBuffer->getStärke();
}

// -- min max -- 
const Punkt &Fenster::getMin() const // gibt die minimale Fenstergröße zurück
{
	return min;
}

const Punkt &Fenster::getMax() const // gibt die maximale Fenstergröße zurück
{
	return max;
}

const Punkt &Fenster::getKMin() const // gibt die minimale Fenstergröße zurück
{
	return kMin;
}

const Punkt &Fenster::getKMax() const // gibt die maximale Fenstergröße zurück
{
	return kMax;
}

// -- scroll -- 
VScrollBar *Fenster::getVScrollBar() const // gibt die Vertikale Scroll Bar zurück
{
	if( !vScroll )
		return 0;
	return (VScrollBar*)vScroll->getThis();
}

VScrollBar *Fenster::zVScrollBar() const
{
	return vScroll;
}

HScrollBar *Fenster::getHScrollBar() const // gibt die Horizontale Scroll Bar zurück
{
	if( !hScroll )
		return 0;
	return (HScrollBar*)hScroll->getThis();
}

HScrollBar *Fenster::zHScrollBar() const
{
	return hScroll;
}

// -- Members -- 
ZeichnungArray *Fenster::getMembers() const // gibt die Members zurück
{
	return members;
}

// -- Kopie --
Zeichnung *Fenster::dublizieren() const // Erzeugt eine Kopie des Fensters
{
	Fenster *ret = new Fenster();
	ret->setPosition( pos );
	ret->setGröße( gr );
	ret->setMausEreignisParameter( makParam );
	ret->setTastaturEreignisParameter( takParam );
	ret->setMausEreignis( Mak );
	ret->setTastaturEreignis( Tak );
	if( toolTip )
		ret->setToolTipText( toolTip->zText()->getText(), toolTip->zBildschirm() );
	ret->setStyle( style );
	ret->setSchließenMeParam( schließenMeParam );
	ret->setSchließenMe( schließenMe );
	if( rahmen )
	{
		ret->setRBreite( rahmen->getRBreite() );
		ret->setRFarbe( rahmen->getFarbe() );
	}
	if( titel )
		ret->setTTextFeldZ( (TextFeld*)titel->dublizieren() );
	ret->setKBgFarbe( bgKörperFarbe );
	if( bgKörperBild )
		ret->setKBgBild( bgKörperBild->getThis() );
	if( körperBuffer )
	{
		ret->setKAfFarbe( körperBuffer->getFarbe() );
		ret->setKAfStärke( körperBuffer->getStärke() );
	}
	ret->setSBgFarbe( bgSchließFarbe );
	if( bgSchließBild )
		ret->setSBgBild( bgSchließBild->getThis() );
	if( schließBuffer )
	{
		ret->setSAfFarbe( schließBuffer->getFarbe() );
		ret->setSAfStärke( schließBuffer->getStärke() );
	}
	if( schließKlickBuffer )
	{
		ret->setSKAfFarbe( schließKlickBuffer->getFarbe() );
		ret->setSKAfStärke( schließKlickBuffer->getStärke() );
	}
	if( vScroll )
	{
		ret->setVSBMax( vScroll->getScrollData()->max );
		ret->setVSBScroll( vScroll->getScroll() );
	}
	if( hScroll )
	{
		ret->setHSBMax( hScroll->getScrollData()->max );
		ret->setHSBScroll( hScroll->getScroll() );
	}
	ret->setMin( min );
	ret->setMax( max );
	ret->setKMin( kMin );
	ret->setKMax( kMax );
	return ret;
}

// Reference Counting 
Fenster *Fenster::getThis()
{
	++ref;
	return this;
}

Fenster *Fenster::release()
{
	--ref;
	if( ref == 0 )
		delete this;
	return 0;
}