#include "Schrift.h"
#include "Bild.h"
#include "Text.h"
#include "Scroll.h"
#include "Globals.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "FrameworkMath.h"

using namespace Framework;

// Inhalt der Buchstabe Klasse aus Schrift.h
// Konstruktor 
Buchstabe::Buchstabe()
    : size( 0, 0 ),
    pos( 0, 0 ),
    alpha( 0 ),
    schriftSize( 0 ),
    drawSg( 0 ),
    ref( 1 )
{}

// Destruktor 
Buchstabe::~Buchstabe()
{
    if( alpha )
        delete[]alpha;
}

// nicht constant 
void Buchstabe::NeuBuchstabe( Punkt &size ) // Initialisierung
{
    this->size = size;
    if( alpha )
        delete[]alpha;
    alpha = new unsigned char[ size.x * size.y ];
    ZeroMemory( alpha, size.x * size.y );
}

void Buchstabe::setPixel( Punkt &pos, unsigned char alpha ) // setzt den alphawert des Pixels
{
    this->alpha[ pos.x + pos.y * size.x ] = alpha;
}

void Buchstabe::setPixel( int x, int y, unsigned char alpha )
{
    this->alpha[ x + y * size.x ] = alpha;
}

void Buchstabe::setPixel( int i, unsigned char alpha )
{
    this->alpha[ i ] = alpha;
}

void Buchstabe::setPosition( Punkt &pos ) // setzt die Buchstabenposition
{
    this->pos = pos;
}

void Buchstabe::setPosition( int x, int y )
{
    pos.x = x;
    pos.y = y;
}

void Buchstabe::setSchriftSize( int sg ) // setzt die Schriftgröße des Buchstaben
{
    schriftSize = sg;
}

void Buchstabe::setDrawSchriftSize( int dsg ) // setzt die Zeichengröße des Buchstaben
{
    drawSg = dsg;
}

// constant
const Punkt &Buchstabe::getSize() const // gibt die Buchstabenbildgröße zurück
{
    return size;
}

int Buchstabe::getBreite() const // Buchstabenbreite
{
    return (int)( ( (double)size.x / (double)schriftSize ) * (double)drawSg + 0.5 );
}

int Buchstabe::getHeight() const // Buchstabenhöhe
{
    return (int)( ( (double)size.y / (double)schriftSize ) *(double)drawSg + 0.5 );
}

int Buchstabe::getNormHeight() const // Buchstabenhöhe
{
    return size.y;
}

unsigned char *Buchstabe::getBuff() const // gibt den Alphabuffer zurück
{
    return alpha;
}

void Buchstabe::render( Bild &zRObj, int f ) const // Zeichnet nach zRObj
{
    if( alpha )
    {
        const Punkt &zRObjGr = zRObj.getDrawGr();
        const Punkt &zRObjPos = zRObj.getDrawPos();
        const Punkt &zRObjOff = zRObj.getDrawOff();
        int xp = pos.x + zRObjOff.x, yp = pos.y + zRObjOff.y;
        int xs = xp < zRObjPos.x ? ( zRObjPos.x - xp ) : 0, ys = yp < zRObjPos.y ? ( zRObjPos.y - yp ) : 0;
        int b = size.x, h = size.y;
        unsigned char a2 = (unsigned char)( 255 - ( f >> 24 ) );
        f &= 0x00FFFFFF;
        if( schriftSize == drawSg )
        {
            if( xp >= zRObjGr.x || yp >= zRObjGr.y || xp + b < zRObjPos.x || yp + h < zRObjPos.y )
                return;
            b = ( xp + b ) > zRObjGr.x ? ( zRObjGr.x - xp ) : b;
            h = ( yp + h ) > zRObjGr.y ? ( zRObjGr.y - yp ) : h;
            if( !a2 )
            {
                int xx, ygr = ( ys - 1 ) * size.x, ygr2 = ( yp + ys - 1 ) * zRObj.getBreite();
                for( int yy = ys; yy < h; ++yy )
                {
                    ygr += size.x;
                    ygr2 += zRObj.getBreite();
                    for( xx = xs; xx < b; ++xx )
                        zRObj.alphaPixel( xp + xx + ygr2, f | ( alpha[ xx + ygr ] << 24 ) );
                }
            }
            else
            {
                int a;
                int xx, ygr = ( ys - 1 ) * size.x, ygr2 = ( yp + ys - 1 ) * zRObj.getBreite();
                for( int yy = ys; yy < h; ++yy )
                {
                    ygr += size.x;
                    ygr2 += zRObj.getBreite();
                    for( xx = xs; xx < b; ++xx )
                    {
                        a = alpha[ xx + ygr ] - a2;
                        if( a > 0 )
                            zRObj.alphaPixel( xp + xx + ygr2, f | ( a << 24 ) );
                    }
                }
            }
        }
        else
        {
            double xoff = (double)schriftSize / (double)drawSg,
                yoff = (double)schriftSize / (double)drawSg;
            double x = xs * xoff, y = ys * yoff;
            int maxX = getBreite(), maxY = getHeight();
            maxX = ( xp + maxX ) >= zRObjGr.x ? ( zRObjGr.x - xp ) : maxX;
            maxY = ( yp + maxY ) >= zRObjGr.y ? ( zRObjGr.y - yp ) : maxY;
            if( !a2 )
            {
                int dx, ygr, ygr2;
                for( int dy = ys; dy < maxY; ++dy )
                {
                    ygr2 = ( yp + dy ) * zRObj.getBreite();
                    ygr = (int)y * b;
                    for( dx = xs; dx < maxX; ++dx )
                    {
                        zRObj.alphaPixel( xp + dx + ygr2, f | ( alpha[ (int)x + ygr ] << 24 ) );
                        x += xoff;
                    }
                    x = xs;
                    y += yoff;
                }
            }
            else
            {
                int a, dx, ygr, ygr2;
                for( int dy = ys; dy < maxY; ++dy )
                {
                    ygr2 = ( yp + dy ) * zRObj.getBreite();
                    ygr = (int)y * b;
                    for( dx = xs; dx < maxX; ++dx )
                    {
                        a = alpha[ (int)x + ygr ] - a2;
                        zRObj.alphaPixel( xp + dx + ygr2, f | ( a << 24 ) );
                        x += xoff;
                    }
                    x = xs;
                    y += yoff;
                }
            }
        }
    }
}

// Reference Counting 
Buchstabe *Buchstabe::getThis()
{
    ++ref;
    return this;
}

Buchstabe *Buchstabe::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der Alphabet Klasse aus Schrift.h
// Konstruktor 
Alphabet::Alphabet()
    : zeichen( new Buchstabe*[ 256 ] ),
    schriftSize( 12 ),
    drawSchriftSize( 12 ),
    pos( 0, 0 ),
    zeilenHeight( 0 ),
    zeilenAbstand( 5 ),
    ref( 1 )
{
    for( int i = 0; i < 256; ++i )
        zeichen[ i ] = 0;
}

// Destruktor 
Alphabet::~Alphabet()
{
    for( int i = 0; i < 256; ++i )
    {
        if( zeichen[ i ] )
            zeichen[ i ]->release();
    }
    delete[]zeichen;
}

// nicht constant 
void Alphabet::NeuAlphabet() // Initialisierung 
{
    for( int i = 0; i < 256; ++i )
    {
        if( zeichen[ i ] )
            zeichen[ i ]->release();
    }
    for( int i = 0; i < 256; ++i )
        zeichen[ i ] = 0;
    zeilenHeight = 0;
}

void Alphabet::setBuchstabe( unsigned char i, Buchstabe *buchstabe ) // setzt einen Buchstaben
{
    if( zeichen[ i ] )
        zeichen[ i ]->release();
    zeichen[ i ] = buchstabe;
    if( zeichen[ i ] )
    {
        zeichen[ i ]->setSchriftSize( schriftSize );
        zeichen[ i ]->setDrawSchriftSize( drawSchriftSize );
    }
    zeilenHeight = 0;
    for( int i = 0; i < 256; ++i )
    {
        if( zeichen[ i ] != 0 )
            zeilenHeight = maxInt( zeichen[ i ]->getHeight(), zeilenHeight );
    }
}

void Alphabet::setSchriftSize( int gr ) // setzt die Schriftgröße
{
    schriftSize = gr;
    for( int i = 0; i < 256; ++i )
    {
        if( zeichen[ i ] )
            zeichen[ i ]->setSchriftSize( gr );
    }
}

void Alphabet::setDrawSchriftSize( int gr ) // setzt die Zeichengröße
{
    drawSchriftSize = gr;
    for( int i = 0; i < 256; ++i )
    {
        if( zeichen[ i ] )
            zeichen[ i ]->setDrawSchriftSize( gr );
    }
}

void Alphabet::setZeilenAbstand( int za ) // setzt die Zeilenhöhe( Zeilenabstand )
{
    zeilenAbstand = za;
}

void Alphabet::setDrawPosition( Punkt &pos ) // setzt die Draw Position
{
    this->pos = pos;
}

void Alphabet::setDrawPosition( int x, int y )
{
    pos.x = x;
    pos.y = y;
}

// constant 
Buchstabe *Alphabet::getBuchstabe( unsigned char i ) const // gibt einen Buchstaben zurück
{
    if( zeichen[ i ] )
        return zeichen[ i ]->getThis();
    return 0;
}

Buchstabe *Alphabet::zBuchstabe( unsigned char i ) const
{
    return zeichen[ i ];
}

bool Alphabet::hatBuchstabe( unsigned char b ) const
{
    return zeichen[ b ] != 0;
}

int Alphabet::getSchriftSize() const // gibt die Schriftgröße zurück
{
    return schriftSize;
}

int Alphabet::getDrawSchriftSize() const // gibt die Zeichengröße zurück
{
    return drawSchriftSize;
}

int Alphabet::getZeilenAbstand() const // gibt den Zeilenabstand zurück
{
    return zeilenAbstand;
}

int Alphabet::getZeilenHeight() const // gibt die Höhe des höchsten Zeichens zurück
{
    return (int)( (double)zeilenHeight / schriftSize * drawSchriftSize + 0.5 );
}

const Punkt &Alphabet::getPosition() const // gibt die DrawPosition zurück
{
    return pos;
}

int Alphabet::getTextBreite( Text *zTxt ) const // gibt die Breite des Textes zurück
{
    int ret = 0;
    int len = zTxt->getLength();
    char *buff = zTxt->getText();
    unsigned char c = 0;
    int tmp = 0;
    for( int i = 0; i < len; ++i )
    {
        c = (unsigned char)buff[ i ];
        if( buff[ i ] == '\n' )
        {
            if( tmp > ret )
                ret = tmp;
            tmp = 0;
        }
        else if( buff[ i ] == '\r' )
        {
            i += 10;
            continue;
        }
        else if( buff[ i ] == '\t' )
            tmp += drawSchriftSize;
        else if( buff[ i ] == ' ' )
            tmp += drawSchriftSize / 2;
        else if( zeichen[ c ] )
            tmp += zeichen[ c ]->getBreite();
    }
    if( tmp > ret )
        ret = tmp;
    return ret;
}

int Alphabet::getTextHeight( Text *zTxt ) const // gibt die Höhe des Textes zurück
{
    int hi = getZeilenHeight();
    return hi + ( ( hi + zeilenAbstand ) * zTxt->anzahlVon( '\n' ) );
}

int Alphabet::textPos( Text *zText, int mausX, int mausY ) const // gibt den Buchstaben zurück, auf den die Maus zeigt
{
    char *buffer = zText->getText();
    int len = zText->getLength();
    int tx = 0;
    int ty = 0;
    int sh = getZeilenHeight();
    if( mausX < 0 || mausY < 0 )
        return -1;
    for( int i = 0; i < len; ++i )
    {
        if( buffer[ i ] == '\n' )
        {
            ty += sh + zeilenAbstand;
            tx = 0;
            if( mausY < ty )
                return i;
        }
        if( buffer[ i ] == '\t' )
            tx += drawSchriftSize;
        if( buffer[ i ] == ' ' )
            tx += drawSchriftSize / 2;
        if( zeichen[ (unsigned char)buffer[ i ] ] )
            tx += zeichen[ (unsigned char)buffer[ i ] ]->getBreite();
        int txpl = 0;
        if( zeichen[ (unsigned char)buffer[ i + 1 ] ] )
            txpl = zeichen[ (unsigned char)buffer[ i + 1 ] ]->getBreite() / 2;
        if( mausX < tx - txpl && mausY < ty + sh + zeilenAbstand )
            return i;
    }
    if( mausY < ty + sh + zeilenAbstand )
        return len;
    return -1;
}

void Alphabet::textFormatieren( Text *zText, int maxBreite, int schriftSize ) // fügt zeilenumbrüche ein 
{
    int sg = drawSchriftSize;
    setDrawSchriftSize( schriftSize );
    int lastPos = -1;
    int len = zText->getLength();
    char *txt = zText->getText();
    int x = 0;
    Text result = zText->getText();
    for( int i = 0; i < len; ++i )
    {
        char c = txt[ i ];
        if( c == ' ' )
        {
            lastPos = i;
            x += schriftSize / 2;
            continue;
        }
        if( c == '\t' )
        {
            lastPos = i;
            x += schriftSize;
            continue;
        }
        if( c == '\n' )
        {
            x = 0;
            lastPos = -1;
            continue;
        }
        if( c == '\r' && len - i >= 11 )
        {
            i += 10;
            continue;
        }
        Buchstabe *b = getBuchstabe( (unsigned)c );
        if( b )
        {
            x += b->getBreite();
            if( x > maxBreite && lastPos > -1 )
            {
                result.ersetzen( lastPos, lastPos + 1, "\n" );
                x = 0;
                i = lastPos;
                lastPos = -1;
            }
            b = b->release();
        }
    }
    zText->setText( result );
    setDrawSchriftSize( sg );
}

void Alphabet::render( Text *zTxt, Bild &rendezRObj, int f ) const // Zeichnet txt nach rendezRObj
{
    int zRObjBr = rendezRObj.getBreite();
    int zRObjHi = rendezRObj.getHeight();
    int xp = pos.x;
    int yp = pos.y;
    int zh = getZeilenHeight();
    if( yp + ( zh + zeilenAbstand ) * zTxt->anzahlVon( '\n' ) + zh < 0 || xp >= zRObjBr || yp >= zRObjHi )
        return;
    char *text = zTxt->getText();
    int len = zTxt->getLength();
    for( int i = 0; i < len; ++i )
    {
        unsigned char c = text[ i ];
        if( c == ' ' )
        {
            xp += drawSchriftSize / 2;
            continue;
        }
        if( c == '\t' )
        {
            xp += drawSchriftSize;
            continue;
        }
        if( c == '\n' )
        {
            yp += zh + zeilenAbstand;
            xp = pos.x;
            continue;
        }
        if( c == '\r' && len - i >= 11 )
        {
            i += 3;
            Text *hex1 = zTxt->getTeilText( i, i + 6 );
            Text *hex2 = zTxt->getTeilText( i + 6, i + 8 );
            f = ( TextZuInt( hex1->getText(), 16 ) << 8 ) |
                ( TextZuInt( hex2->getText(), 16 ) );
            hex1->release();
            hex2->release();
            i += 7;
            continue;
        }
        if( zeichen[ c ] )
        {
            if( xp >= zRObjBr )
                continue;
            zeichen[ c ]->setPosition( xp, yp );
            zeichen[ c ]->render( rendezRObj, f );
            xp += zeichen[ c ]->getBreite();
        }
    }
}

void Alphabet::render( Text *zTxt, Bild &rendezRObj, int cpos, int cf, int fbeg, int ff, int f ) const
{
    int zRObjBr = rendezRObj.getBreite();
    int zRObjHi = rendezRObj.getHeight();
    int xp = pos.x;
    int yp = pos.y;
    int zh = getZeilenHeight();
    if( yp + ( zh + zeilenAbstand ) * zTxt->anzahlVon( '\n' ) + zh < 0 || xp >= zRObjBr || yp >= zRObjHi )
        return;
    char *text = zTxt->getText();
    int len = zTxt->getLength();
    bool faerb = 0;
    for( int i = 0; i < len; ++i )
    {
        unsigned char c = text[ i ];
        if( i == fbeg )
            faerb = !faerb;
        if( i == cpos )
        {
            rendezRObj.drawLinieVAlpha( xp, yp, zh, cf );
            faerb = !faerb;
        }
        if( c == ' ' )
        {
            if( faerb )
                rendezRObj.alphaRegion( xp, yp, drawSchriftSize / 2, zh, ff );
            xp += drawSchriftSize / 2;
            continue;
        }
        if( c == '\t' )
        {
            if( faerb )
                rendezRObj.alphaRegion( xp, yp, drawSchriftSize, zh, ff );
            xp += drawSchriftSize;
            continue;
        }
        if( c == '\n' )
        {
            yp += zh + zeilenAbstand;
            xp = pos.x;
            continue;
        }
        if( c == '\r' && len - i >= 11 )
        {
            i += 3;
            Text *hex1 = zTxt->getTeilText( i, i + 6 );
            Text *hex2 = zTxt->getTeilText( i + 6, i + 8 );
            f = ( TextZuInt( hex1->getText(), 16 ) << 8 ) |
                ( TextZuInt( hex2->getText(), 16 ) );
            hex1->release();
            hex2->release();
            i += 7;
            continue;
        }
        if( zeichen[ c ] )
        {
            if( xp >= zRObjBr )
                continue;
            if( faerb )
            {
                int br = zeichen[ c ]->getBreite();
                rendezRObj.alphaRegion( xp, yp, br, zh, ff );
            }
            zeichen[ c ]->setPosition( xp, yp );
            zeichen[ c ]->render( rendezRObj, f );
            xp += zeichen[ c ]->getBreite();
        }
    }
    if( len == cpos )
        rendezRObj.drawLinieVAlpha( xp, yp, zh, cf );
}

// Reference Counting 
Alphabet *Alphabet::getThis()
{
    ++ref;
    return this;
}

Alphabet *Alphabet::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der AlphabetArray Klasse aus Schrift.h
// Konstruktor 
AlphabetArray::AlphabetArray()
    : next( 0 ),
    This( 0 )
{}

// Destruktor 
AlphabetArray::~AlphabetArray()
{
    if( This )
        This->release();
    delete next;
}

// nicht constant 
bool AlphabetArray::addAlphabet( Alphabet *alphabet ) // Fügt ein Alphabet hinzu
{
    if( This )
    {
        if( This->getSchriftSize() == alphabet->getSchriftSize() )
        {
            alphabet->release();
            return false;
        }
    }
    else
    {
        This = alphabet;
        return true;
    }
    if( !next )
        next = new AlphabetArray();
    return next->addAlphabet( alphabet );
}

bool AlphabetArray::removeAlphabet( int sg ) // entfernt ein Alphabet
{
    if( This )
    {
        if( This->getSchriftSize() == sg )
            This = This->release();
        return 1;
    }
    if( !next )
        return 0;
    if( next->removeAlphabet( sg ) )
    {
        AlphabetArray *tmp = next->getNext();
        next->setNext0();
        delete next;
        next = tmp;
    }
    return 0;
}

void AlphabetArray::setDrawSchriftSize( int sg ) // Setzt die Draw Schriftgröße aller Alphabete
{
    if( This )
        This->setDrawSchriftSize( sg );
    if( next )
        next->setDrawSchriftSize( sg );
}

void AlphabetArray::setZeilenAbstand( int za ) // setzt den Zeilenabstant aller Alphabete
{
    if( This )
        This->setZeilenAbstand( za );
    if( next )
        next->setZeilenAbstand( za );
}

void AlphabetArray::setNext0() // setzt den next Zeiger zu 0
{
    next = 0;
}

// constant 
Alphabet *AlphabetArray::getAlphabet( unsigned char sg ) const // gibt getThis von einem Alphabet zurück
{
    if( !This )
        return 0;
    if( This->getSchriftSize() == sg )
        return This->getThis();
    if( next )
        return next->getAlphabet( sg );
    return 0;
}

Alphabet *AlphabetArray::zAlphabet( unsigned char sg ) const // gibt ein Alphabet zurück
{
    if( !This )
        return 0;
    if( This->getSchriftSize() == sg )
        return This;
    if( next )
        return next->zAlphabet( sg );
    return 0;
}

Alphabet *AlphabetArray::getAlphabetI( int index, int count ) const
{
    if( count == index )
        return This->getThis();
    if( next )
        return next->getAlphabetI( index, count + 1 );
    return 0;
}

Alphabet *AlphabetArray::zAlphabetI( int index, int count ) const
{
    if( count == index )
        return This;
    if( next )
        return next->zAlphabetI( index, count + 1 );
    return 0;
}

AlphabetArray *AlphabetArray::getNext() const // gibt das nächste Alphabet zurück
{
    return next;
}

// Inhalt der Schrift Klasse aus Schrift.h
// Konstruktor 
Schrift::Schrift()
    : alphabetAnzahl( 0 ),
    alphabet( new AlphabetArray() ),
    schriftSize( 12 ),
    zeilenAbstand( 5 ),
    drawPos( 0, 0 ),
    ref( 1 )
{
}

// Destruktor 
Schrift::~Schrift()
{
    delete alphabet;
}

// nicht constant 
void Schrift::lock() // lockt die Schrift
{
    cs.lock();
}

void Schrift::unlock() // unlockt die Schrift
{
    cs.unlock();
}

bool Schrift::addAlphabet( Alphabet *alphabet ) // Fügt der Schrift ein Alphabet hinzu
{
    lock();
    if( this->alphabet->addAlphabet( alphabet ) )
    {
        ++alphabetAnzahl;
        alphabet->setDrawSchriftSize( schriftSize );
        unlock();
        return true;
    }
    unlock();
    return false;
}

void Schrift::removeAlphabet( int sg ) // Entfernt ein Alphabet
{
    lock();
    if( alphabet->removeAlphabet( sg ) )
        --alphabetAnzahl;
    unlock();
}

void Schrift::setDrawPosition( int x, int y ) // setzt die Zeichenposition
{
    lock();
    drawPos.x = x;
    drawPos.y = y;
    unlock();
}

void Schrift::setDrawPosition( Punkt &pos )
{
    lock();
    drawPos = pos;
    unlock();
}

void Schrift::setSchriftSize( int sg ) // setzt die Schriftgröße
{
    lock();
    schriftSize = sg;
    alphabet->setDrawSchriftSize( sg );
    unlock();
}

void Schrift::setZeilenAbstand( int za ) // setzt den Zeilenabstand
{
    lock();
    zeilenAbstand = za;
    alphabet->setZeilenAbstand( za );
    unlock();
}

void Schrift::textFormatieren( Text *zText, int maxBreite, int schriftSize ) // fügt zeilenumbrüche ein
{
    lock();
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( drawAlphabet )
        drawAlphabet->textFormatieren( zText, maxBreite, schriftSize );
    unlock();
}

void Schrift::renderText( Text *zTxt, Bild &zRObj, int f ) // zeichnet txt nach zRObj
{
    lock();
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( drawAlphabet )
    {
        drawAlphabet->setDrawPosition( drawPos.x, drawPos.y );
        drawAlphabet->render( zTxt, zRObj, f );
    }
    unlock();
}

void Schrift::renderText( Text *zTxt, Bild &zRObj, int cpos, int cf, int fbeg, int ff, int f )
{
    lock();
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( drawAlphabet )
    {
        drawAlphabet->setDrawPosition( drawPos.x, drawPos.y );
        drawAlphabet->render( zTxt, zRObj, cpos, cf, fbeg, ff, f );
    }
    unlock();
}

// constant 
Alphabet *Schrift::getAlphabet( int sg ) const // gibt einen Alphaberarray zurück
{
    return alphabet->getAlphabet( (unsigned char)sg );
}

Alphabet *Schrift::zAlphabet( int sg ) const
{
    return alphabet->zAlphabet( (unsigned char)sg );
}

Alphabet *Schrift::getAlphabetI( int index ) const
{
    return alphabet->getAlphabetI( index, 0 );
}

Alphabet *Schrift::zAlphabetI( int index ) const
{
    return alphabet->zAlphabetI( index, 0 );
}

unsigned char Schrift::getAlphabetAnzahl() const // gibt die anzahl von in der Schrift enthaltenen Alphabeten zurück
{
    return alphabetAnzahl;
}

int Schrift::getSchriftSize() const // gibt die Schriftgröße zurück
{
    return schriftSize;
}

int Schrift::getZeilenabstand() const // gibt den Zeilenabstand zurück
{
    return zeilenAbstand;
}

int Schrift::getDrawX() const // gibt die Zeichenposition zurück
{
    return drawPos.x;
}

int Schrift::getDrawY() const
{
    return drawPos.y;
}

const Punkt &Schrift::getDrawPosition() const
{
    return drawPos;
}

int Schrift::getTextBreite( Text *zTxt ) const // gibt die Breite des Textes zurück
{
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( !drawAlphabet )
        return 0;
    return drawAlphabet->getTextBreite( zTxt );
}

int Schrift::getTextHeight( Text *zTxt ) const // gibt die Höhe des Textes zurück
{
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( !drawAlphabet )
        return 0;
    return drawAlphabet->getTextHeight( zTxt );
}

int Schrift::textPos( Text *zTxt, int mausX, int mausY ) const // gibt den Buchstaben zurück, auf den die Maus zeigt
{
    Alphabet *drawAlphabet = alphabet->zAlphabet( (unsigned char)schriftSize );
    if( !drawAlphabet )
    {
        for( int i = 0; i < 256; ++i )
        {
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize - i ) );
            if( drawAlphabet )
                break;
            drawAlphabet = alphabet->zAlphabet( (unsigned char)( schriftSize + i ) );
            if( drawAlphabet )
                break;
        }
    }
    if( !drawAlphabet )
        return 0;
    return drawAlphabet->textPos( zTxt, mausX, mausY );
}

// Reference Counting 
Schrift *Schrift::getThis()
{
    ++ref;
    return this;
}

Schrift *Schrift::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}