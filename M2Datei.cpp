#include "M2Datei.h"
#include "Text.h"
#include "Datei.h"
#include "Model2D.h"

using namespace Framework;

// Inhalt der M2Datei Klasse aus M2Datei.h
// Konstruktor
M2Datei::M2Datei()
{
	pfad = new Text();
	modelName = new RCArray< Text >();
	modelPos = new Array< __int64 >();
	ref = 1;
}

M2Datei::M2Datei( const char *pfad )
{
	this->pfad = new Text( pfad );
	modelName = new RCArray< Text >();
	modelPos = new Array< __int64 >();
	ref = 1;
}

M2Datei::M2Datei( Text *pfad )
{
	this->pfad = pfad;
	modelName = new RCArray< Text >();
	modelPos = new Array< __int64 >();
	ref = 1;
}

// Destruktor
M2Datei::~M2Datei()
{
	pfad->release();
	modelName->release();
	modelPos->release();
}

// nicht constant
void M2Datei::setPfad( const char *pfad )
{
	this->pfad->setText( pfad );
}

void M2Datei::setPfadZ( Text *pfad )
{
	if( this->pfad )
		this->pfad->release();
	this->pfad = pfad;
}

void M2Datei::leseDaten()
{
	Datei d;
	d.setDatei( pfad->getText() );
	d.öffnen( Datei::Style::lesen );
	char anz = 0;
	d.lese( &anz, 1 );
	modelName->leeren();
	modelPos->leeren();
	for( int i = 0; i < anz; i++ )
	{
		char län = 0;
		d.lese( &län, 1 );
		char *txt = new char[ län + 1 ];
		d.lese( txt, län );
		txt[ (int)län ] = 0;
		modelName->set( new Text( txt ), i );
		delete[] txt;
		__int64 pos = 0;
		d.lese( (char*)&pos, 8 );
		modelPos->set( pos, i );
	}
	d.schließen();
}

bool M2Datei::saveModel( Model2DData *zMdr, Text *name )
{
	bool ret = saveModel( zMdr, name->getText() );
	name->release();
	return ret;
}

bool M2Datei::saveModel( Model2DData *zMdr, const char *name )
{
	int anz = modelName->getEintragAnzahl();
	for( int i = 0; i < anz; i++ )
	{
		if( modelName->z( i )->istGleich( name ) )
		{
			if( !löscheModel( name ) )
				return 0;
			break;
		}
	}
	anz = modelName->getEintragAnzahl();
	Datei d;
	d.setDatei( pfad->getText() );
	d.öffnen( Datei::Style::lesen );
	Datei neu;
	neu.setDatei( pfad->getText() );
	neu.zPfad()->anhängen( "0" );
	while( neu.existiert() )
		neu.zPfad()->anhängen( "0" );
	if( !neu.öffnen( Datei::Style::schreiben ) )
	{
		if( d.istOffen() )
			d.schließen();
		return 0;
	}
	modelName->add( new Text( name ) );
	int offs = textLänge( name ) + 9;
	for( int i = 0; i < anz; i++ )
		modelPos->set( modelPos->get( i ) + offs, i );
	if( d.getGröße() < 0 )
		modelPos->add( offs + 1 );
	else
		modelPos->add( d.getGröße() + offs );
	anz++;
	char tmp = (char)anz;
	neu.schreibe( &tmp, 1 );
	for( int i = 0; i < anz; i++ )
	{
		char län = (char)modelName->z( i )->getLänge();
		neu.schreibe( &län, 1 );
		neu.schreibe( modelName->z( i )->getText(), län );
		__int64 pos = modelPos->get( i );
		neu.schreibe( (char*)&pos, 8 );
	}
	if( d.existiert() )
	{
		d.setLPosition( modelPos->get( 0 ) - offs, 0 );
		__int64 dl = d.getGröße() - d.getLPosition();
		char bytes[ 2048 ];
		while( dl )
		{
			int l = dl > 2048 ? 2048 : (int)dl;
			d.lese( bytes, l );
			neu.schreibe( bytes, l );
			dl -= l;
		}
	}
	d.schließen();
	char pAnz = zMdr->polygons->getEintragAnzahl();
	neu.schreibe( &pAnz, 1 );
	for( int p = 0; p < pAnz; p++ )
	{
		int vAnz = zMdr->polygons->get( p ).vertex->getEintragAnzahl();
		char textur = 1;
		for( int i = 0; i < vAnz; i++ )
			textur &= (char)zMdr->polygons->get( p ).tKordinaten->hat( i );
		neu.schreibe( &textur, 1 );
		neu.schreibe( (char*)&vAnz, 4 );
		for( int i = 0; i < vAnz; i++ )
		{
			float v = zMdr->polygons->get( p ).vertex->get( i ).x;
			neu.schreibe( (char*)&v, 4 );
			v = zMdr->polygons->get( p ).vertex->get( i ).y;
			neu.schreibe( (char*)&v, 4 );
			if( textur )
			{
				int t = zMdr->polygons->get( p ).tKordinaten->get( i ).x;
				neu.schreibe( (char*)&t, 4 );
				t = zMdr->polygons->get( p ).tKordinaten->get( i ).y;
				neu.schreibe( (char*)&t, 4 );
			}
		}
	}
	d.löschen();
	neu.schließen();
	neu.umbenennen( pfad->getText() );
	leseDaten();
	return 1;
}

bool M2Datei::löscheModel( Text *name )
{
	bool ret = löscheModel( name->getText() );
	name->release();
	return ret;
}

bool M2Datei::löscheModel( const char *name )
{
	int anz = modelName->getEintragAnzahl();
	int p = -1;
	for( int i = 0; i < anz; i++ )
	{
		if( modelName->z( i )->istGleich( name ) )
		{
			p = i;
			break;
		}
	}
	if( p < 0 )
		return 0;
	Datei d;
	d.setDatei( pfad->getText() );
	if( !d.öffnen( Datei::Style::lesen ) )
		return 0;
	Datei neu;
	neu.setDatei( pfad->getText() );
	neu.zPfad()->anhängen( "0" );
	while( neu.existiert() )
		neu.zPfad()->anhängen( "0" );
	if( !neu.öffnen( Datei::Style::schreiben ) )
	{
		if( d.istOffen() )
			d.schließen();
		return 0;
	}
	char nAnz = (char)anz - 1;
	neu.schreibe( &nAnz, 1 );
	int offs = modelName->z( p )->getLänge() + 9;
	__int64 startP = 0, endP = 0, start2P = 0;
	for( int i = 0; i < anz; i++ )
	{
		char nLän = (char)modelName->z( i )->getLänge();
		char *n = modelName->z( i )->getText();
		__int64 pos = modelPos->get( i );
		if( !startP )
			startP = pos;
		if( i == p + 1 )
			start2P = pos;
		if( i == p )
		{
			if( !endP )
				endP = pos;
			if( i < anz - 1 )
				offs += (int)( modelPos->get( i + 1 ) - pos );
		}
		if( i != p )
		{
			pos -= offs;
			neu.schreibe( &nLän, 1 );
			neu.schreibe( n, nLän );
			neu.schreibe( (char*)&pos, 8 );
		}
	}
	if( d.istOffen() )
	{
		d.setLPosition( startP, 0 );
		__int64 bLän = endP - startP;
		char bytes[ 2048 ];
		while( bLän > 0 )
		{
			int l = 2048 > bLän ? (int)bLän : 2048;
			d.lese( bytes, l );
			neu.schreibe( bytes, l );
			bLän -= l;
		}
		if( start2P )
		{
			d.setLPosition( start2P, 0 );
			bLän = d.getGröße() - start2P;
			while( bLän > 0 )
			{
				int l = 2048 > bLän ? (int)bLän : 2048;
				d.lese( bytes, l );
				neu.schreibe( bytes, l );
				bLän -= l;
			}
		}
		d.schließen();
	}
	d.löschen();
	neu.schließen();
	neu.umbenennen( pfad->getText() );
	leseDaten();
	return 1;
}

// constant
Model2DData *M2Datei::ladeModel( Text *name ) const
{
	Model2DData *ret = ladeModel( name->getText() );
	name->release();
	return ret;
}

Model2DData *M2Datei::ladeModel( const char *name ) const
{
	Datei d;
	d.setDatei( pfad->getText() );
	if( !d.öffnen( Datei::Style::lesen ) )
		return 0;
	int anz = modelName->getEintragAnzahl();
	for( int i = 0; i < anz; i++ )
	{
		if( modelName->z( i )->istGleich( name ) )
		{
			d.setLPosition( modelPos->get( i ), 0 );
			break;
		}
	}
	if( !d.getLPosition() )
	{
		d.schließen();
		return 0;
	}
	char pAnz = 0;
	d.lese( &pAnz, 1 );
	Array< Polygon2D > *polygons = new Array< Polygon2D >();
	for( int p = 0; p < pAnz; p++ )
	{
		char textur = 0;
		d.lese( &textur, 1 );
		int vAnz = 0;
		d.lese( (char*)&vAnz, 4 );
		if( polygons->hat( p ) )
		{
			if( polygons->get( p ).vertex )
				polygons->get( p ).vertex->release();
			if( polygons->get( p ).tKordinaten )
				polygons->get( p ).tKordinaten->release();
		}
		Polygon2D polygon;
		polygon.vertex = new Array< Vertex >();
		if( textur )
			polygon.tKordinaten = new Array< Punkt >();
		else
			polygon.tKordinaten = 0;
		for( int v = 0; v < vAnz; v++ )
		{
			Vertex p;
			d.lese( (char*)&p.x, 4 );
			d.lese( (char*)&p.y, 4 );
			polygon.vertex->add( p );
			if( textur )
			{
				Punkt tp;
				d.lese( (char*)&tp.x, 4 );
				d.lese( (char*)&tp.y, 4 );
				polygon.tKordinaten->add( tp );
			}
		}
		polygons->add( polygon );
	}
	d.schließen();
	Model2DData *ret = new Model2DData();
	ret->erstelleModell( polygons );
	return ret;
}

bool M2Datei::hatModel( const char *name ) const
{
	int anz = modelName->getEintragAnzahl();
	for( int i = 0; i < anz; i++ )
	{
		if( modelName->z( i )->istGleich( name ) )
			return 1;
	}
	return 0;
}

int M2Datei::getModelAnzahl() const
{
	return modelName->getEintragAnzahl();
}

Text *M2Datei::zModelName( int i ) const
{
	return modelName->z( i );
}

// Reference Counting
M2Datei *M2Datei::getThis()
{
	++ref;
	return this;
}

M2Datei *M2Datei::release()
{
	if( !--ref )
		delete this;
	return 0;
}