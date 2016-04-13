#include "Schluessel.h"

using namespace Framework::Verschlüsselung;

// Inhalt der Bytes Klasse aus Schlüssel.h
// Konstruktor
Bytes::Bytes()
	: bytes( 0 ),
	  del( 1 ),
	  län( 0 ),
	  ref( 1 )
{
}

Bytes::Bytes( int län )
	: bytes( new char[ län ] ),
	  del( 1 ),
	  län( län ),
	  ref( 1 )
{
}

Bytes::Bytes( const char *daten, int län )
	: bytes( new char[ län ] ),
	  del( 1 ),
	  län( län ),
	  ref( 1 )
{
	setBytes( daten );
}

// Destruktor
Bytes::~Bytes()
{
	if( del )
	    delete[] bytes;
}

// nicht constant
void Bytes::setBytes( const char *daten )
{
	if( !bytes || !daten )
		return;
	char *end = bytes + län;
	for( char *c = bytes; c < end; c++, ++daten )
		*c = *daten;
}

void Bytes::setBytes( const char *daten, int län )
{
	if( !daten || !län )
		return;
	if( del )
	    delete[] bytes;
	del = 1;
	bytes = new char[ län ];
	this->län = län;
	setBytes( daten );
}

void Bytes::setBytesZ( char *daten, int län )
{
	if( del )
	    delete[] bytes;
	del = 0;
	bytes = daten;
	this->län = län;
}

void Bytes::füll( const char c )
{
	if( !bytes )
		return;
	char *end = bytes + län;
	for( char *b = bytes; b < end; ++b )
		*b = c;
}

void Bytes::füll( const char c, int län )
{
	if( !bytes )
		bytes = new char[ län ];
	län = län > this->län ? this->län : län;
	char *end = bytes + län;
	for( char *b = bytes; b < end; ++b )
		*b = c;
}

void Bytes::füll( const char c, int beg, int end )
{
	if( beg >= län )
		return;
	if( !bytes )
		bytes = new char[ end - beg ];
	end = end > this->län ? this->län : end;
	char *endB = bytes + end;
	for( char *b = bytes + beg; b < endB; ++b )
		*b = c;
}

void Bytes::füll( const char *c, int cLän )
{
	if( !c || !cLän || !bytes )
		return;
	char *endB = bytes + län;
	const char *endC = c + cLän;
	const char *d = c;
	for( char *b = bytes; b < endB; b++, d = d < endC - 1 ? d + 1 : c )
		*b = *d;
}

void Bytes::set( const char c, int pos )
{
	if( !bytes || pos >= län )
		return;
	bytes[ pos ] = c;
}

// constant
int Bytes::getLänge() const
{
	return län;
}

char *Bytes::getBytes() const
{
	return bytes;
}

// Reference Counting
Bytes *Bytes::getThis()
{
	++ref;
	return this;
}

Bytes *Bytes::release()
{
	--ref;
	if( !ref )
		delete this;
	return 0;
}


// Inhalt der Schlüssel Klasse aus Schlüssel.h
// Konstruktor
Schlüssel::Schlüssel()
	: schlüssel( 0 ),
	  län( 0 ),
	  pos( 0 ),
	  ref( 1 )
{
}

Schlüssel::Schlüssel( const char *s, int län )
	: schlüssel( new unsigned char[ län ] ),
	  län( län ),
	  pos( 0 ),
	  ref( 1 )
{
	for( int i = 0; i < län; ++i )
		schlüssel[ i ] = s[ i ];
}

// Destruktor
Schlüssel::~Schlüssel()
{
	delete[] schlüssel;
}

// nicht constant
void Schlüssel::setPos( int p )
{
	if( p < 0 )
		p = 0;
	pos = p > län ? 0 : p;
}

void Schlüssel::setSchlüssel( const char *s, int län )
{
	delete[] schlüssel;
	schlüssel = new unsigned char[ län ];
	for( int i = 0; i < län; ++i )
		schlüssel[ i ] = s[ i ];
	pos = 0;
	this->län = län;
}

void Schlüssel::codieren( Bytes *daten )
{
	if( !schlüssel || !län )
	{
		daten->release();
		return;
	}
	int dLän = daten->getLänge();
	char *bytes = daten->getBytes();
	char *bEnd = bytes + dLän;
	for( char *c = bytes; c < bEnd; ++c )
	{
		*c = *c + schlüssel[ pos ];
		++pos;
		if( pos >= län )
			pos = 0;
	}
	daten->release();
}

void Schlüssel::decodieren( Bytes *daten )
{
	if( !schlüssel || !län )
	{
		daten->release();
		return;
	}
	int dLän = daten->getLänge();
	char *bytes = daten->getBytes();
	char *bEnd = bytes + dLän;
	for( char *c = bytes; c < bEnd; ++c )
	{
		*c = *c - schlüssel[ pos ];
		++pos;
		if( pos >= län )
			pos = 0;
	}
	daten->release();
}

// Reference Counting
Schlüssel *Schlüssel::getThis()
{
	++ref;
	return this;
}

Schlüssel *Schlüssel::release()
{
	--ref;
	if( !ref )
		delete this;
	return 0;
}