#include "Key.h"

using namespace Framework::Encryption;

// Inhalt der Bytes Klasse aus Schlüssel.h
// Konstruktor
Bytes::Bytes()
    : bytes( 0 ),
    del( 1 ),
    length( 0 ),
    ref( 1 )
{}

Bytes::Bytes( int len )
    : bytes( new char[ len ] ),
    del( 1 ),
	length( len ),
    ref( 1 )
{}

Bytes::Bytes( const char *daten, int len )
    : bytes( new char[ len ] ),
    del( 1 ),
	length( len ),
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
    char *end = bytes + length;
    for( char *c = bytes; c < end; c++, ++daten )
        *c = *daten;
}

void Bytes::setBytes( const char *daten, int len )
{
    if( !daten || !len )
        return;
    if( del )
        delete[] bytes;
    del = 1;
    bytes = new char[ len ];
    this->length = len;
    setBytes( daten );
}

void Bytes::setBytesZ( char *daten, int len )
{
    if( del )
        delete[] bytes;
    del = 0;
    bytes = daten;
    this->length = len;
}

void Bytes::fill( const char c )
{
    if( !bytes )
        return;
    char *end = bytes + length;
    for( char *b = bytes; b < end; ++b )
        *b = c;
}

void Bytes::fill( const char c, int len )
{
    if( !bytes )
        bytes = new char[ len ];
    len = len > this->length ? this->length : len;
    char *end = bytes + len;
    for( char *b = bytes; b < end; ++b )
        *b = c;
}

void Bytes::fill( const char c, int beg, int end )
{
    if( beg >= length )
        return;
    if( !bytes )
        bytes = new char[ end - beg ];
    end = end > this->length ? this->length : end;
    char *endB = bytes + end;
    for( char *b = bytes + beg; b < endB; ++b )
        *b = c;
}

void Bytes::fill( const char *c, int cLen )
{
    if( !c || !cLen || !bytes )
        return;
    char *endB = bytes + length;
    const char *endC = c + cLen;
    const char *d = c;
    for( char *b = bytes; b < endB; b++, d = d < endC - 1 ? d + 1 : c )
        *b = *d;
}

void Bytes::set( const char c, int pos )
{
    if( !bytes || pos >= length )
        return;
    bytes[ pos ] = c;
}

// constant
int Bytes::getLength() const
{
    return length;
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
Key::Key()
    : key( 0 ),
    length( 0 ),
    pos( 0 ),
    ref( 1 )
{}

Key::Key( const char *s, int len )
    : key( new unsigned char[ len ] ),
    length( len ),
    pos( 0 ),
    ref( 1 )
{
    for( int i = 0; i < len; ++i )
        key[ i ] = s[ i ];
}

// Destruktor
Key::~Key()
{
    delete[] key;
}

// nicht constant
void Key::setPos( __int64 p )
{
    if( p < 0 )
        p = 0;
    pos = (int)(p % length);
}

void Key::setKey( const char *s, int len )
{
    delete[] key;
    key = new unsigned char[ len ];
    for( int i = 0; i < len; ++i )
        key[ i ] = s[ i ];
    pos = 0;
    this->length = len;
}

void Key::codieren( Bytes *daten )
{
    if( !key || !length )
    {
        daten->release();
        return;
    }
    int dLen = daten->getLength();
    char *bytes = daten->getBytes();
    char *bEnd = bytes + dLen;
    for( char *c = bytes; c < bEnd; ++c )
    {
        *c = (char)( *c + key[ pos ] );
        ++pos;
        if( pos >= length )
            pos = 0;
    }
    daten->release();
}

void Key::decodieren( Bytes *daten )
{
    if( !key || !length )
    {
        daten->release();
        return;
    }
    int dLen = daten->getLength();
    char *bytes = daten->getBytes();
    char *bEnd = bytes + dLen;
    for( char *c = bytes; c < bEnd; ++c )
    {
        *c = (char)( *c - key[ pos ] );
        ++pos;
        if( pos >= length )
            pos = 0;
    }
    daten->release();
}

// Reference Counting
Key *Key::getThis()
{
    ++ref;
    return this;
}

Key *Key::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}