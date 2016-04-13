#include "InitDatei.h"
#include "Text.h"
#include "Datei.h"

using namespace Framework;

// Inhalt der InitDatei Klasse aus InitDatei.h
// Konstruktor
InitDatei::InitDatei()
	: pfad( new Text() ),
	  feldAnzahl( 0 ),
	  name( new RCArray< Text >() ),
	  wert( new RCArray< Text >() ),
	  ref( 1 )
{
}

InitDatei::InitDatei( Text *pfad )
	: pfad( new Text() ),
	feldAnzahl( 0 ),
	name( new RCArray< Text >() ),
	wert( new RCArray< Text >() ),
	ref( 1 )
{
	setPfad( pfad );
}

InitDatei::InitDatei( const char *pfad )
	: pfad( new Text() ),
	feldAnzahl( 0 ),
	name( new RCArray< Text >() ),
	wert( new RCArray< Text >() ),
	ref( 1 )
{
	setPfad( pfad );
}

// Destruktor
InitDatei::~InitDatei()
{
	pfad->release();
	name->release();
	wert->release();
}

// nicht constant
void InitDatei::setPfad( Text *pfad )
{
	this->pfad->setText( pfad );
}

void InitDatei::setPfad( const char *pfad )
{
	this->pfad->setText( pfad );
}

bool InitDatei::laden()
{
	Datei *dat = new Datei();
	dat->setDatei( pfad->getText() );
	if( !dat->öffnen( Datei::Style::lesen ) )
	{
		dat->release();
		return 0;
	}
	löscheAlle();
	Text *zeile = dat->leseZeile();
	for( int i = 0; zeile; ++i )
	{
		zeile->löschen( "\r\n" );
		zeile->löschen( "\n" );
		Text *n = zeile->getTeilText( 0, zeile->positionVon( '=' ) );
		Text *w = zeile->getTeilText( zeile->positionVon( '=' ) + 1 );
		name->set( n, i );
		wert->set( w, i );
		zeile->release();
		zeile = dat->leseZeile();
		feldAnzahl = i + 1;
	}
	dat->schließen();
	dat->release();
	return 1;
}

bool InitDatei::addWert( Text *name, Text *wert )
{
	if( !wertExistiert( name->getText() ) )
	{
		this->name->set( new Text( name->getText() ), feldAnzahl );
		this->wert->set( new Text( wert->getText() ), feldAnzahl );
		++feldAnzahl;
		name->release();
		wert->release();
		return 1;
	}
	name->release();
	wert->release();
	return 0;
}

bool InitDatei::addWert( const char *name, const char *wert )
{
	if( !wertExistiert( name ) )
	{
		this->name->set( new Text( name ), feldAnzahl );
		this->wert->set( new Text( wert ), feldAnzahl );
		++feldAnzahl;
		return 1;
	}
	return 0;
}

bool InitDatei::setWert( Text *name, Text *wert )
{
	if( !wertExistiert( name->getText() ) )
	{
		name->release();
		wert->release();
		return 0;
	}
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			this->wert->z( i )->setText( wert->getText() );
			name->release();
			wert->release();
			return 1;
		}
	}
	name->release();
	wert->release();
	return 0;
}

bool InitDatei::setWert( const char *name, const char *wert )
{
	if( !wertExistiert( name ) )
		return 0;
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
		{
			this->wert->z( i )->setText( wert );
			return 1;
		}
	}
	return 0;
}

bool InitDatei::setWert( int num, Text *wert )
{
	if( num >= feldAnzahl )
	{
		wert->release();
		return 0;
	}
	this->wert->z( num )->setText( wert->getText() );
	wert->release();
	return 1;
}

bool InitDatei::setWert( int num, const char *wert )
{
	if( num >= feldAnzahl )
		return 0;
	this->wert->z( num )->setText( wert );
	return 1;
}

bool InitDatei::löscheWert( Text *name )
{
	if( !wertExistiert( name->getText() ) )
	{
		name->release();
		return 0;
	}
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			this->name->lösche( i );
			this->wert->lösche( i );
			--feldAnzahl;
			name->release();
			return 1;
		}
	}
	name->release();
	return 0;
}

bool InitDatei::löscheWert( const char *name )
{
	if( !wertExistiert( name ) )
		return 0;
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
		{
			this->name->lösche( i );
			this->wert->lösche( i );
			--feldAnzahl;
			return 1;
		}
	}
	return 0;
}

bool InitDatei::löscheWert( int num )
{
	if( num >= feldAnzahl )
		return 0;
	this->name->lösche( num );
	this->wert->lösche( num );
	--feldAnzahl;
	return 1;
}

void InitDatei::löscheAlle()
{
	for( ; feldAnzahl > 0; --feldAnzahl )
	{
		this->name->lösche( 0 );
		this->wert->lösche( 0 );
	}
}

bool InitDatei::speichern()
{
	Datei *dat = new Datei();
	dat->setDatei( pfad->getText() );
	if( !dat->existiert() )
		dat->erstellen();
	if( !dat->öffnen( Datei::Style::schreiben ) )
	{
		dat->release();
		return 0;
	}
	for( int i = 0; i < feldAnzahl; ++i )
	{
		Text *zeile = new Text( "" );
		zeile->anhängen( name->z( i )->getText() );
		zeile->anhängen( "=" );
		zeile->anhängen( wert->z( i )->getText() );
		zeile->anhängen( "\n" );
		dat->schreibe( zeile->getText(), zeile->getLänge() );
		zeile->release();
	}
	dat->schließen();
	dat->release();
	return 1;
}

// constant
int InitDatei::getWertAnzahl() const
{
	return feldAnzahl;
}

bool InitDatei::wertExistiert( Text *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			name->release();
			return 1;
		}
	}
	name->release();
	return 0;
}

bool InitDatei::wertExistiert( const char *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
			return 1;
	}
	return 0;
}

int InitDatei::getWertNummer( Text *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			name->release();
			return i;
		}
	}
	name->release();
	return -1;
}

int InitDatei::getWertNummer( const char *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
			return i;
	}
	return -1;
}

Text *InitDatei::getWert( Text *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			name->release();
			return wert->get( i );
		}
	}
	name->release();
	return 0;
}

Text *InitDatei::getWert( const char *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
			return wert->get( i );
	}
	return 0;
}

Text *InitDatei::getWert( int num )
{
	if( num >= feldAnzahl )
		return 0;
	return wert->get( num );
}

Text *InitDatei::zWert( Text *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name->getText() ) )
		{
			name->release();
			return wert->z( i );
		}
	}
	name->release();
	return 0;
}

Text *InitDatei::zWert( const char *name )
{
	for( int i = 0; i < feldAnzahl; ++i )
	{
		if( this->name->z( i )->istGleich( name ) )
			return wert->z( i );
	}
	return 0;
}

Text *InitDatei::zWert( int num )
{
	if( num >= feldAnzahl )
		return 0;
	return wert->z( num );
}

Text *InitDatei::getName( int num )
{
	if( num >= feldAnzahl )
		return 0;
	return name->get( num );
}

Text *InitDatei::zName( int num )
{
	if( num >= feldAnzahl )
		return 0;
	return name->z( num );
}

Text *InitDatei::getPfad() const
{
	return pfad->getThis();
}

Text *InitDatei::zPfad() const
{
	return pfad;
}

// reference Counting
InitDatei *InitDatei::getThis()
{
	++ref;
	return this;
}

InitDatei *InitDatei::release()
{
	--ref;
	if( !ref )
		delete this;
	return 0;
}
