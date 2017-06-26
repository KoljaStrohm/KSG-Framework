//---Include---
#include "DateiSystem.h"
#include "Bild.h"
#include "Text.h"
#include <iostream>
#ifdef WIN32
#include "Fenster.h"
#include "Schrift.h"
#include "Fortschritt.h"
#include "Globals.h"
#endif
#include "Datei.h"

using namespace Framework;
// LTDB Dateivormat 
// Inhalt der LTDBPixel Klasse aus Dateisystem.h
// Konstruktor 
LTDBPixel::LTDBPixel( LTDBPixel *davor )
    : davor( davor ),
    index( 0 ),
    iR( 0 ),
    iG( 0 ),
    iB( 0 ),
    iA( 0 ),
    miR( 8 ),
    miG( 8 ),
    miB( 8 ),
    miA( 8 ),
    maxIndex( 1 ),
    change( 0 ),
    changeR( 0 ),
    changeG( 0 ),
    changeB( 0 ),
    changeA( 0 ),
    komp( 0 ),
    R( 0 ),
    G( 0 ),
    B( 0 ),
    A( 0 ),
    ref( 1 )
{}

// Destruktor 
LTDBPixel::~LTDBPixel()
{
    if( davor )
        davor->release();
}

// nicht constant 
bool LTDBPixel::addBitZuFarbe( unsigned char bit ) // Fügt den Farbwerten ein Bit hinzu
{
    if( changeR && iR != miR ) // Das Bit gehört zu Rot
    {
        R = (unsigned char)( R | ( ( bit & 0x1 ) << ( 7 - komp - iR ) ) );
        ++iR;
    }
    else if( changeG && iG != miG ) // Das Bit gehört zu Grün
    {
        G = (unsigned char)( G | ( ( bit & 0x1 ) << ( 7 - komp - iG ) ) );
        ++iG;
    }
    else if( changeB && iB != miB ) // Das Bit gehört zu Blau
    {
        B = (unsigned char)( B | ( ( bit & 0x1 ) << ( 7 - komp - iB ) ) );
        ++iB;
    }
    else if( changeA && iA != miA ) // Das Bit gehört zu Alpha
    {
        A = (unsigned char)( A | ( ( bit & 0x1 ) << ( 7 - komp - iA ) ) );
        ++iA;
    }
    else // Das Bit gehört zum nächsten Pixel
        return false;
    return true;
}

char LTDBPixel::addByte( char byte, char begin ) // gibt ende des Pixels zurück, -1 wenn nicht zu ende
{
    if( begin >= 8 || begin < 0 )
        return -1;
    for( int i = begin; i < 8; ++i )
    {
        switch( index )
        {
        case 0:
            // Das erste Bit eines Pixels speichert, ob sich an der Komprimierung etwas änderte
            change = (bool)( ( byte >> ( 7 - i ) ) & 0x1 );
            if( !change ) // Ändert sich nichts an der Komprimierung, so werden die Werte vom vorherigen Pixel übernommen
            {
                if( !davor ) // Die Datei ist beschädigt ( Der erste Pixel kann nicht von dem davor Übernemen )
                {
#ifdef WIN32
                    MessageBox( NULL, "Fehler, die Bilddatei ist beschädigt", "Fehler", MB_ICONERROR );
#endif
                    exit( 0 );
                }
                changeR = davor->getChangeR();
                changeG = davor->getChangeG();
                changeB = davor->getChangeB();
                changeA = davor->getChangeA();
                komp = davor->getKomp() & 7;
                miR = (char)( miR - komp ), miG = (char)( miG - komp ), miB = (char)( miB - komp ), miA = (char)( miA - komp );
                if( !changeR )
                    R = davor->getR();
                if( !changeG )
                    G = davor->getG();
                if( !changeB )
                    B = davor->getB();
                if( !changeA )
                    A = davor->getA();
                maxIndex = (char)( maxIndex + ( changeR + changeG + changeB + changeA ) * ( 8 - komp ) ); // Bestimmung der Länge
                // des Pixels in Bits. Jede Farbe hat von grund auf 8 Bits, durch komprimierung kanns kleiner sein
            }
            else
                maxIndex = (char)( maxIndex + 7 ); // Da der Pixel nicht die Werte des vorherigen übernimmt, wird er um 7 Bits größer
            break;
        case 1: // Das zweite Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                changeR = (bool)( ( byte >> ( 7 - i ) ) & 0x1 );
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 2: // Das dritte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                changeG = (bool)( ( byte >> ( 7 - i ) ) & 0x1 );
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 3: // Das vierte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                changeB = (bool)( ( byte >> ( 7 - i ) ) & 0x1 );
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 4: // Das fünfte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                changeA = (bool)( ( byte >> ( 7 - i ) ) & 0x1 );
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 5: // Das sechste Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                komp = (unsigned char)( komp | ( (unsigned char)( ( byte >> ( 7 - i ) ) & 0x1 ) << 2 ) ) & 7;
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 6: // Das siebte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
                komp = (unsigned char)( komp | ( (unsigned char)( ( byte >> ( 7 - i ) ) & 0x1 ) << 1 ) ) & 7;
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        case 7: // Das achte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( change ) // Das Bit gehört zu den 7 Komprimierungsbits
            {
                komp = (unsigned char)( komp | ( ( byte >> ( 7 - i ) ) & 0x1 ) ) & 7;
                // Das war das letzte Komprimierungsbit
                // Komprimierung auswerten 
                miR = (char)( miR - komp ), miG = (char)( miG - komp ), miB = (char)( miB - komp ), miA = (char)( miA - komp );
                if( !changeR )
                    R = davor->getR();
                if( !changeG )
                    G = davor->getG();
                if( !changeB )
                    B = davor->getB();
                if( !changeA )
                    A = davor->getA();
                maxIndex = (char)( maxIndex + ( changeR + changeG + changeB + changeA ) * ( 8 - komp ) ); // Bitlänge des Pixels
            }
            else
            {
                if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                    return (unsigned char)i;
            }
            break;
        default: // Die restlichen Bits speichern alle die Farbwerte des Pixels
            if( !addBitZuFarbe( (unsigned char)( byte >> ( 7 - i ) ) ) ) // Das Bit gehört zur Farbe
                return (unsigned char)i;
            break;
        }
        ++index;
        if( index >= maxIndex )
        {
            if( davor )
            {
                if( changeR )
                    R = (unsigned char)( davor->getR() + R );
                if( changeG )
                    G = (unsigned char)( davor->getG() + G );
                if( changeB )
                    B = (unsigned char)( davor->getB() + B );
                if( changeA )
                    A = (unsigned char)( davor->getA() + A );
                davor = davor->release();
            }
            return (char)( i + 1 );
        }
    }
    return -1;
}

void LTDBPixel::setFarbe( int f ) // setzt die Farbe des Pixels
{   // Da diese Funktion aufgerufen wird, möchte man die Klasse nun zum speichern verwenden
    // Werte zurücksetzen, fals mit der Klasse schon geladen oder gespeichert wurde
    index = 0, maxIndex = 1;
    change = 0, changeR = 0, changeG = 0, changeB = 0, changeA = 0, komp = 0;
    iR = 0, iG = 0, iB = 0, iA = 0;
    miR = 8, miG = 8, miB = 8, miA = 8;
    // Farbwerte setzen
    R = (unsigned char)( f >> 16 );
    G = (unsigned char)( f >> 8 );
    B = (unsigned char)f;
    A = (unsigned char)( f >> 24 );
}

void LTDBPixel::komprimieren() // Komprimiert den Pixel
{
    maxIndex = 1;
    if( !davor )
    {// Das ist der erste Pixel
        change = 1;
        changeR = 1;
        changeG = 1;
        changeB = 1;
        changeA = 1;
        maxIndex = (char)( maxIndex + 7 );
        miR = (char)getBits( R );
        miG = (char)getBits( G );
        miB = (char)getBits( B );
        miA = (char)getBits( A );
    }
    else
    { // Es wird die differenz zum vorrigen Pixel gespeichert
        miR = (char)getBits( (char)( R - davor->getR() ) );
        miG = (char)getBits( (char)( G - davor->getG() ) );
        miB = (char)getBits( (char)( B - davor->getB() ) );
        miA = (char)getBits( (char)( A - davor->getA() ) );
        changeR = R != davor->getR();
        changeG = G != davor->getG();
        changeB = B != davor->getB();
        changeA = A != davor->getA();
    }// Prüfen ob sich etwas ändert
    if( !miR && changeR )
        ++miR;
    if( !miG && changeG )
        ++miG;
    if( !miB && changeB )
        ++miB;
    if( !miA && changeA )
        ++miA;
    int k = ( miR > miG ? miR : miG );
    k = ( k > miB ? k : miB );
    k = ( k > miA ? k : miA );
    miR = (char)k, miG = (char)k, miB = (char)k, miA = (char)k;
    komp = (unsigned char)( 8 - k ) & 7;
    maxIndex = (char)( maxIndex + ( changeR + changeG + changeB + changeA ) * k );
    if( davor )
    {
        if( changeR != davor->getChangeR() ||
            changeG != davor->getChangeG() ||
            changeB != davor->getChangeB() ||
            changeA != davor->getChangeA() ||
            komp != davor->getKomp() )
        { // Es ändert sich etwas
            change = 1;
            maxIndex = (char)( maxIndex + 7 );
        }
        else
        { // Es ändert sich nichts
            change = 0;
        }
    }
}

bool LTDBPixel::getNextFarbeBit( char &byte, int i ) // Speichert das nächste Farbbit in byte
{
    unsigned char RR = R;
    unsigned char GG = G;
    unsigned char BB = B;
    unsigned char AA = A;
    if( davor )
    {
        RR = (unsigned char)( RR - davor->getR() );
        GG = (unsigned char)( GG - davor->getG() );
        BB = (unsigned char)( BB - davor->getB() );
        AA = (unsigned char)( AA - davor->getA() );
    }
    if( changeR && iR != miR ) // Das Bit gehört zu Rot
    {
        byte |= (char)( ( ( RR >> ( 7 - komp - iR ) ) & 0x1 ) << ( 7 - i ) );
        ++iR;
    }
    else if( changeG && iG != miG ) // Das Bit gehört zu Grün
    {
        byte |= (char)( ( ( GG >> ( 7 - komp - iG ) ) & 0x1 ) << ( 7 - i ) );
        ++iG;
    }
    else if( changeB && iB != miB ) // Das Bit gehört zu Blau
    {
        byte |= (char)( ( ( BB >> ( 7 - komp - iB ) ) & 0x1 ) << ( 7 - i ) );
        ++iB;
    }
    else if( changeA && iA != miA ) // Das Bit gehört zu Alpha
    {
        byte |= (char)( ( ( AA >> ( 7 - komp - iA ) ) & 0x1 ) << ( 7 - i ) );
        ++iA;
    }
    else // Der Pixel ist bereits zu ende
        return false;
    return true;
}

char LTDBPixel::getNextByte( char &byte, int begin ) // Gibt die nächsten Bits Zurück, -1 wenn der Pixel nicht zu ende ist
{
    // bbegin gibt an wohin in die byte-variable geschrieben werden soll
    // die Funktion gibt das ende des Pixels in der byte-variable zurück
    // -1 heißt, dass der Pixel nicht zu ende ist
    for( int i = begin; i < 8; ++i )
    {
        switch( index )
        {
        case 0: // Das erste Bit des Pixels speichert, ob sich etwas an der Komprimierung ändert
            byte |= (char)( ( (int)change & 0x1 ) << ( 7 - i ) );
            break;
        case 1: // Das zweite Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( (int)changeR & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 2: // Das dritte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( (int)changeG & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 3: // Das vierte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( (int)changeB & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 4: // Das fünfte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( (int)changeA & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 5: // Das sechste Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( ( komp >> 2 ) & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 6: // Das siebte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( ( komp >> 1 ) & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        case 7: // Das sechste Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( change ) // Komprimierung
                byte |= (char)( ( komp & 0x1 ) << ( 7 - i ) );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return (char)i;
            }
            break;
        default: // Die restlichen Bits speichern die Farbe des Pixels
            if( !getNextFarbeBit( byte, i ) )
                return (char)i;
            break;
        }
        ++index;
        if( index >= maxIndex )
        {
            if( davor )
                davor = davor->release();
            return (char)( i + 1 );
        }
    }
    return -1;
}

// constant 
int LTDBPixel::zuFarbe() const // gibt den Pixel als Farbe zurück
{
    return ( ( (int)R << 16 ) | ( (int)G << 8 ) | (int)B | ( (int)A << 24 ) );
}

bool LTDBPixel::getChangeR() const
{
    return changeR;
}

bool LTDBPixel::getChangeG() const
{
    return changeG;
}

bool LTDBPixel::getChangeB() const
{
    return changeB;
}

bool LTDBPixel::getChangeA() const
{
    return changeA;
}

unsigned char LTDBPixel::getKomp() const
{
    return komp;
}

unsigned char LTDBPixel::getR() const // gibt Rot zurück 
{
    return R;
}

unsigned char LTDBPixel::getG() const // gibt Grün zurück
{
    return G;
}

unsigned char LTDBPixel::getB() const // gibt Blau zurück
{
    return B;
}

unsigned char LTDBPixel::getA() const // gibt Alpha zurück
{
    return A;
}

// Reference Counting 
LTDBPixel *LTDBPixel::getThis()
{
    ++ref;
    return this;
}

LTDBPixel *LTDBPixel::release()
{
    --ref;
    if( ref < 1 )
        delete this;
    return 0;
}

// Inhalt der LTDBDateiKopf Klasse aus Dateisystem.h
// konstructor
LTDBDateiKopf::LTDBDateiKopf()
    : bilder( new RCArray< Text >() ),
    pos( new Array< __int64 >() ),
    bAnzahl( 0 ),
    ref( 1 )
{}

// destructor 
LTDBDateiKopf::~LTDBDateiKopf()
{
    bilder->release();
    pos->release();
}

// nicht constant 
void LTDBDateiKopf::removeBild( int i )
{
    if( i >= bAnzahl )
        return;
    bilder->remove( i );
    pos->remove( i );
    --bAnzahl;
}

void LTDBDateiKopf::removeBild( Text *txt )
{
    int i = -1;
    for( int ii = 0; ii < bAnzahl; ++ii )
    {
        Text *b = bilder->z( ii );
        if( b->istGleich( txt->getText() ) )
        {
            i = ii;
            break;
        }
    }
    txt->release();
    if( i == -1 )
        return;
    bilder->remove( i );
    pos->remove( i );
    --bAnzahl;
}

void LTDBDateiKopf::addBild( Text *txt )
{
    bilder->add( txt, bAnzahl );
    pos->add( 0, bAnzahl );
    ++bAnzahl;
}

void LTDBDateiKopf::setBildPos( int i, __int64 pos )
{
    this->pos->set( pos, i );
}

void LTDBDateiKopf::setBildPos( Text *txt, __int64 pos )
{
    int i = -1;
    for( int ii = 0; ii < bAnzahl; ++ii )
    {
        Text *b = bilder->z( ii );
        if( b->istGleich( txt->getText() ) )
        {
            i = ii;
            break;
        }
    }
    txt->release();
    if( i == -1 )
        return;
    this->pos->set( pos, i );
}

void LTDBDateiKopf::laden( FBalken *f, std::ifstream *inF )
{
    if( inF->is_open() && inF->good() )
    {
        char b = 0;
        inF->read( &b, 1 );
        bAnzahl = b << 8;
        inF->read( &b, 1 );
        bAnzahl |= b & 0xFF;
#ifdef WIN32
        if( f )
        {
            f->reset();
            f->setAktionAnzahl( bAnzahl );
        }
#endif
        bilder->leeren();
        pos->leeren();
        for( int i = 0; i < bAnzahl; ++i )
        {
            LTDBKopf *kpf = new LTDBKopf();
            kpf->laden( inF ); // bildname und halbe datei position
            bilder->set( kpf->getTitel(), i ); // setzt titel
            Punkt gr = kpf->getSize();
            kpf->release();
            char p[ 5 ];
            inF->read( (char *)p, 5 ); // andere hälfte der Dateiposition
            unsigned __int64 position = ( ( (__int64)gr.x << 52 ) & 0xFFF0000000000000 ) |
                ( ( (__int64)gr.y << 40 ) & 0xFFF0000000000 ) |
                ( ( (__int64)p[ 0 ] << 32 ) & 0xFF00000000 ) |
                ( ( (__int64)p[ 1 ] << 24 ) & 0xFF000000 ) |
                ( ( (__int64)p[ 2 ] << 16 ) & 0xFF0000 ) |
                ( ( (__int64)p[ 3 ] << 8 ) & 0xFF00 ) |
                ( (__int64)p[ 4 ] & 0xFF );
            pos->set( position, i ); // setzt position
#ifdef WIN32
            if( f )
                f->aktionPlus();
#endif
        }
    }
}

// constant
void LTDBDateiKopf::speichern( std::ofstream *outF ) const
{
    if( outF->is_open() && outF->good() )
    {
        char b = (char)( bAnzahl >> 8 );
        outF->write( &b, 1 );
        b = (char)bAnzahl;
        outF->write( &b, 1 );
        for( int i = 0; i < bAnzahl; ++i )
        {
            LTDBKopf *kpf = new LTDBKopf();
            __int64 position = pos->get( i );
            kpf->Init( bilder->get( i ), Punkt( (int)( position >> 52 ), (int)( position >> 40 ) ) );
            kpf->speichern( outF );
            kpf->release();
            char p[] = { (char)( position >> 32 ), (char)( position >> 24 ), (char)( position >> 16 ), (char)( position >> 8 ), (char)( position ) };
            outF->write( (char *)p, 5 );
        }
    }
}

Text *LTDBDateiKopf::getBild( int i ) const
{
    return bilder->get( i );
}

Text *LTDBDateiKopf::zBild( int i ) const
{
    return bilder->z( i );
}

__int64 LTDBDateiKopf::getBildPosition( Text *txt ) const
{
    int i = -1;
    for( int ii = 0; ii < bAnzahl; ++ii )
    {
        Text *b = bilder->z( ii );
        if( b->istGleich( txt->getText() ) )
        {
            i = ii;
            break;
        }
    }
    txt->release();
    if( i == -1 )
        return -1;
    return pos->get( i );
}

__int64 LTDBDateiKopf::getBildPosition( int index ) const
{
    return pos->get( index );
}

int LTDBDateiKopf::getBildIndex( Text *txt ) const
{
    int i = -1;
    for( int ii = 0; ii < bAnzahl; ++ii )
    {
        Text *b = bilder->z( ii );
        if( b->istGleich( txt->getText() ) )
        {
            i = ii;
            break;
        }
    }
    txt->release();
    return i;
}

int LTDBDateiKopf::getbAnzahl() const
{
    return bAnzahl;
}

RCArray< Text > *LTDBDateiKopf::zBildListe() const
{
    return bilder;
}

// Reference Counting 
LTDBDateiKopf *LTDBDateiKopf::getThis()
{
    ++ref;
    return this;
}

LTDBDateiKopf *LTDBDateiKopf::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDBKopf Klasse aus DateiSystem.h
// Konstruktor 
LTDBKopf::LTDBKopf()
    : ref( 1 )
{}

// nicht constant 
void LTDBKopf::laden( std::ifstream *f ) // Lät die Daten aus einer Datei
{
    if( f->is_open() )
    {
        a = 0;
        b = 0;
        c = 0;
        char aa = 0;
        f->read( &aa, 1 );
        int tl = ( aa >> 4 ) & Bits( 4 );
        int BitAnzahl = 4 + tl * 5 + 24;
        f->seekg( -1, std::ios::cur );
        int Bytes = BitAnzahl / 8;
        if( ( (float)BitAnzahl / 8.0f ) != Bytes )
            ++Bytes;
        char byte = 0;
        for( int i = 0; i < Bytes; ++i )
        {
            f->read( &byte, 1 );
            setBits( i * 8, i * 8 + 8, byte );
        }
    }
}

int LTDBKopf::Init( Text *t, const Punkt &g ) // Befüllt die Daten
{
    a = 0;
    b = 0;
    c = 0;
    int tl = t->getLength();
    if( tl > 15 )
        tl = 15;
    char *titel = new char[ tl ];
    int skipped = 0;
    for( int i = 0; i < tl; ++i )
    {
        titel[ i - skipped ] = t->getText()[ i ];
        if( titel[ i - skipped ] > 96 && titel[ i - skipped ] < 123 )
            titel[ i - skipped ] = (char)( titel[ i - skipped ] - 96 );
        else if( titel[ i - skipped ] > 64 && titel[ i - skipped ] < 91 )
            titel[ i - skipped ] = (char)( titel[ i - skipped ] - 64 );
        else if( titel[ i - skipped ] == 'ü' || titel[ i - skipped ] == 'Ü' )
            titel[ i - skipped ] = 27;
        else if( titel[ i - skipped ] == 'ö' || titel[ i - skipped ] == 'Ö' )
            titel[ i - skipped ] = 28;
        else if( titel[ i - skipped ] == 'ä' || titel[ i - skipped ] == 'Ä' )
            titel[ i - skipped ] = 29;
        else if( titel[ i - skipped ] == 'ß' )
            titel[ i - skipped ] = 30;
        else if( titel[ i - skipped ] == '.' )
            titel[ i - skipped ] = 31;
        else
            ++skipped;
    }
    a = (__int64)( ( tl - skipped ) & Bits( 4 ) ) << 60;
    int BeginBit = 4;
    for( int i = 0; i < tl - skipped; ++i )
    {
        BeginBit += 5;
        switch( i )
        {
        case 0:
            a |= (__int64)( titel[ i ] & 31 ) << 55; // ersten Buchstaben speichern
            break;
        case 1:
            a |= (__int64)( titel[ i ] & 31 ) << 50; // zweiten Buchstaben speichern
            break;
        case 2:
            a |= (__int64)( titel[ i ] & 31 ) << 45; // dritten Buchstaben speichern
            break;
        case 3:
            a |= (__int64)( titel[ i ] & 31 ) << 40; // vierten Buchstaben speichern
            break;
        case 4:
            a |= (__int64)( titel[ i ] & 31 ) << 35; // fünften Buchstaben speichern
            break;
        case 5:
            a |= (__int64)( titel[ i ] & 31 ) << 30; // sechsten Buchstaben speichern
            break;
        case 6:
            a |= (__int64)( titel[ i ] & 31 ) << 25; // siebten Buchstaben speichern
            break;
        case 7:
            a |= (__int64)( titel[ i ] & 31 ) << 20; // achten Buchstaben speichern
            break;
        case 8:
            a |= (__int64)( titel[ i ] & 31 ) << 15; // neunten Buchstaben speichern
            break;
        case 9:
            a |= (__int64)( titel[ i ] & 31 ) << 10; // zenten Buchstaben speichern
            break;
        case 10:
            a |= (__int64)( titel[ i ] & 31 ) << 5; // elften Buchstaben speichern
            break;
        case 11:
            a |= (__int64)( titel[ i ] & 31 ); // zwölften Buchstaben speichern
            break;
        case 12:
            b |= (__int32)( titel[ i ] & 31 ) << 27; // dreizenten Buchstaben speichern
            break;
        case 13:
            b |= (__int32)( titel[ i ] & 31 ) << 22; // vierzenten Buchstaben speichern
            break;
        case 14:
            b |= (__int32)( titel[ i ] & 31 ) << 17; // fünfzenten Buchstaben speichern
            break;
        }
    }
    __int16 grx = (short)( g.x & Bits( 12 ) );
    __int16 gry = (short)( g.y & Bits( 12 ) );
    int EndBit = BeginBit + 24;
    setBits( BeginBit, EndBit - 12, grx );
    setBits( BeginBit + 12, EndBit, gry );
    t->release();
    delete[]titel;
    return skipped;
}

void LTDBKopf::setBits( int BeginBit, int EndBit, __int16 bits )
{
    if( EndBit - BeginBit > 16 )
        EndBit = BeginBit + 16;
    if( BeginBit < 64 )
    {
        if( EndBit < 64 )
        {
            a |= ( (__int64)bits & Bits( EndBit - BeginBit ) ) << ( ( 64 - BeginBit ) - ( EndBit - BeginBit ) );
        }
        else
        {
            a |= ( ( (__int64)bits >> ( EndBit - 64 ) ) & Bits( 64 - BeginBit ) );
            b |= ( (__int32)bits & Bits( EndBit - 64 ) ) << ( 32 - ( EndBit - 64 ) );
        }
    }
    else
    {
        if( BeginBit < 96 )
        {
            if( EndBit < 96 )
            {
                b |= ( (__int32)bits & Bits( EndBit - BeginBit ) ) << ( ( 96 - BeginBit ) - ( EndBit - BeginBit ) );
            }
            else
            {
                b |= ( ( (__int32)bits >> ( EndBit - 96 ) ) & Bits( 96 - BeginBit ) );
                c = (char)( c | ( ( (__int8)bits & Bits( EndBit - 96 ) ) << ( 8 - ( EndBit - 96 ) ) ) );
            }
        }
        else
        {
            c = (char)( c | ( ( (__int8)bits & Bits( EndBit - BeginBit ) ) << ( 8 - ( EndBit - BeginBit ) ) ) );
        }
    }
}

// constant 
void LTDBKopf::speichern( std::ofstream *f ) const // Speichert die Daten in eine Datei
{
    if( f->is_open() )
    {
        int bits = 4/*Titellänge*/ + getTitelLength() * 5/*Titel*/ + 24/*Bildgröße*/;
        int bytes = bits / 8; // Bytelänge des Dateikopfes
        if( ( (float)bits / 8.0f ) != bytes )
            ++bytes;
        char c = 0;
        for( int i = 0; i < bytes; ++i )
        {
            c = (char)getBits( i * 8, i * 8 + 8 );
            f->write( &c, 1 );
        }
    }
}

int LTDBKopf::getTitelLength() const // gibt die länge des Bildnamens zurück
{
    return (int)( a >> 60 & Bits( 4 ) ); // Die Länge des Titels wird in den ersten 4 Bits der Tatei gespeichert
}

Text *LTDBKopf::getTitel() const // gibt den Namen des Bildes zurück
{
    Text *ret = new Text( "" );
    char c[ 2 ];
    c[ 1 ] = '\0';
    int l = getTitelLength();
    for( int i = 0; i < l; ++i )
    {
        c[ 0 ] = 0;
        switch( i )
        {
        case 0:
            c[ 0 ] = ( a >> 55 ) & 31; // ersten Buchstaben holen
            break;
        case 1:
            c[ 0 ] = ( a >> 50 ) & 31; // zweiten Buchstaben holen
            break;
        case 2:
            c[ 0 ] = ( a >> 45 ) & 31; // dritten Buchstaben holen
            break;
        case 3:
            c[ 0 ] = ( a >> 40 ) & 31; // vierten Buchstaben holen
            break;
        case 4:
            c[ 0 ] = ( a >> 35 ) & 31; // fünften Buchstaben holen
            break;
        case 5:
            c[ 0 ] = ( a >> 30 ) & 31; // sechsten Buchstaben holen
            break;
        case 6:
            c[ 0 ] = ( a >> 25 ) & 31; // siebten Buchstaben holen
            break;
        case 7:
            c[ 0 ] = ( a >> 20 ) & 31; // achten Buchstaben holen
            break;
        case 8:
            c[ 0 ] = ( a >> 15 ) & 31; // neunten Buchstaben holen
            break;
        case 9:
            c[ 0 ] = ( a >> 10 ) & 31; // zenten Buchstaben holen
            break;
        case 10:
            c[ 0 ] = ( a >> 5 ) & 31; // elften Buchstaben holen
            break;
        case 11:
            c[ 0 ] = a & 31; // zwölften Buchstaben holen
            break;
        case 12:
            c[ 0 ] = (char)( ( b >> 27 ) & 31 ); // dreizenten Buchstaben holen
            break;
        case 13:
            c[ 0 ] = (char)( ( b >> 22 ) & 31 ); // vierzenten Buchstaben holen
            break;
        case 14:
            c[ 0 ] = (char)( ( b >> 17 ) & 31 ); // fünfzenten Buchstaben holen
            break;
        }
        if( c[ 0 ] == 27 )
            c[ 0 ] = 'ü';
        else if( c[ 0 ] == 28 )
            c[ 0 ] = 'ö';
        else if( c[ 0 ] == 29 )
            c[ 0 ] = 'ä';
        else if( c[ 0 ] == 30 )
            c[ 0 ] = 'ß';
        else if( c[ 0 ] == 31 )
            c[ 0 ] = '.';
        else
            c[ 0 ] = (char)( c[ 0 ] + 96 );
        ret->append( c );
    }
    return ret;
}

Punkt LTDBKopf::getSize() const // gibt die Größe des Bildes zurück
{
    int BeginBit = 4/*Titellänge*/ + getTitelLength() * 5/*Titel*/;
    int EndBit = BeginBit + 24;
    __int16 grx = getBits( BeginBit, EndBit - 12 );
    __int16 gry = getBits( BeginBit + 12, EndBit );
    return Punkt( (int)( grx & Bits( 12 ) ), (int)( gry & Bits( 12 ) ) );
}

__int16 LTDBKopf::getBits( int begin, int ende )const // gibt die Bits von begin bis ende zurück( ohne ende );
{
    if( ende < begin )
        return 0;
    if( ende - begin > 16 )
        ende = begin + 16;
    __int16 ret = 0;
    if( begin < 64 )
    {
        if( ende < 64 )
        {
            ret = (__int16)( a >> ( ( 64 - begin ) - ( ende - begin ) ) & Bits( ende - begin ) );
        }
        else
        {
            ret = (__int16)( ( a & Bits( 64 - begin ) ) << ( ende - 64 ) );
            ret = (__int16)( ret | ( ( b >> ( 32 - ( ende - 64 ) ) ) & Bits( ende - 64 ) ) );
        }
    }
    else
    {
        if( begin < 96 )
        {
            if( ende < 96 )
            {
                ret = (__int16)( b >> ( ( 96 - begin ) - ( ende - begin ) ) & Bits( ende - begin ) );
            }
            else
            {
                ret = (__int16)( ( b & Bits( 96 - begin ) ) << ( ende - 96 ) );
                ret = (__int16)( ret | ( ( c >> ( 8 - ( ende - 96 ) ) ) & Bits( ende - 96 ) ) );
            }
        }
        else
        {
            ret = (__int16)( c >> ( ( 104 - begin ) - ( ende - begin ) ) & Bits( ende - begin ) );
        }
    }
    return ret;
}

// Reference Counting 
LTDBKopf *LTDBKopf::getThis()
{
    ++ref;
    return this;
}

LTDBKopf *LTDBKopf::release()
{
    --ref;
    if( ref < 1 )
        delete this;
    return 0;
}

// Inhalt der LTDBKörper Klasse aus Dateisystem.h
// Konstruktor 
LTDBBody::LTDBBody()
    : gr( 0, 0 ),
    b( new Bild() ),
    ref( 1 )
{}

LTDBBody::LTDBBody( LTDBKopf *k ) // ruft Init auf
    : gr( 0, 0 ),
    b( new Bild() ),
    ref( 1 )
{
    init( k );
}

// Destruktor 
LTDBBody::~LTDBBody()
{
    b->release();
}

// nicht constant
void LTDBBody::init( LTDBKopf k ) // Initialisiert, wird vor dem laden benötigt 
{
    gr = k.getSize();
    int l = k.getTitelLength();
    l = 4 + l * 5 + 24;
    dateiSize = ( l / 8.0 == l ) ? ( l / 8 ) : ( l / 8 + 1 );
}

void LTDBBody::init( LTDBKopf *k ) // Initialisiert, wird vor dem laden benötigt
{
    gr = k->getSize();
    int l = k->getTitelLength();
    l = 4 + l * 5 + 24;
    dateiSize = ( l / 8.0 == l ) ? ( l / 8 ) : ( l / 8 + 1 );
    k->release();
}

void LTDBBody::laden( FBalken *zF, std::ifstream *inF ) // läd das Bild
{
    b->neuBild( gr.x, gr.y, 0xFF000000 ); // neues Bild erstellen
    int *buff = b->getBuffer();
    int breite = b->getBreite();
    char byte = 0;
    int index = 0;
    LTDBPixel *davor = 0; // zuletzt geladener Pixel
    LTDBPixel *dieser = new LTDBPixel( 0 ); // momentan zu ladener Pixel
    int begin = 0; // Pixelbegin, endposition in der byte variable
#ifdef WIN32
    if( zF )
    {
        zF->reset();
        zF->setAktionAnzahl( gr.x * gr.y );
    }
#endif
    while( index < gr.x * gr.y ) // für jeden Pixel
    {
        if( !dieser ) // wenn es nicht der erste Pixel ist
            dieser = new LTDBPixel( davor->getThis() );
        int ende = -1;
        while( ende < 0 ) // Pixel laden
        {
            if( begin == 0 )
                inF->read( &byte, 1 );
            ende = dieser->addByte( byte, (char)begin ); // byte auswerten
            begin = 0;
        }
        begin = ende;
        if( begin == 8 )
            begin = 0;
        buff[ ( index % gr.x ) + ( index / gr.x ) * breite ] = dieser->zuFarbe();
        if( davor )
            davor = davor->release();
        davor = dieser->getThis();
        dieser = dieser->release();
        ++index;
#ifdef WIN32
        if( zF )
            zF->aktionPlus();
#endif
    }
    if( davor )
        davor = davor->release();
}

void LTDBBody::setBild( Bild *b ) // setzt das zu speichernde Bild
{
    this->b->release();
    this->b = b;
}

// constant 
void LTDBBody::speichern( FBalken *zF, std::ofstream *outF ) const // speichert Bild
{
    if( outF->is_open() )
    {
        LTDBPixel *letzter = 0; // Letzter gespeicherter Pixel
        LTDBPixel *dieser = new LTDBPixel( 0 ); // Der momentan zu speichernde Pixel
        int begin = 0, ende = 0; // Pixelbeginn, endposition in der byte variable
        char byte = 0; // Der nächste byte der Datei
        bool w = 0;
#ifdef WIN32
        if( zF )
        {
            zF->reset();
            zF->setAktionAnzahl( gr.x * gr.y );
        }
#endif
        int *pBuff = b->getBuffer();
        for( int i = 0; i < gr.x * gr.y; ++i ) // für jeden Pixel
        {
            if( !dieser ) // wenn es nicht der erste Pixel ist
                dieser = new LTDBPixel( letzter->getThis() );
            dieser->setFarbe( pBuff[ i ] ); // Farbe des Pixels setzen
            dieser->komprimieren(); // Pixel komprimieren
            ende = -1;
            while( ende < 0 ) // byte befüllen
            {
                ende = dieser->getNextByte( byte, begin );
                begin = 0;
                w = 0;
                if( ende == -1 || ende == 8 ) // byte speichern
                {
                    outF->write( &byte, 1 );
                    w = 1;
                    byte = 0;
                }
            } // Pixel fertig
            begin = ende;
            if( begin == 8 )
                begin = 0;
            if( letzter )
                letzter->release();
            letzter = dieser->getThis(); // dieser wird zu letzter
            dieser = dieser->release();
#ifdef WIN32
            if( zF )
                zF->aktionPlus();
#endif
        }
        if( letzter )
            letzter = letzter->release();
        if( !w )
        {
            outF->write( &byte, 1 ); // Das letzte byte speichern
        }
        outF->flush(); // dateistream speichern
    }
}

Bild *LTDBBody::getBild() const // gibt das geladene Bild zurück
{
    return b->getThis();
}

const Punkt &LTDBBody::getSize() const // gibt die größe des Bildes zurück
{
    return gr;
}

// Reference Counting
LTDBBody *LTDBBody::getThis()
{
    ++ref;
    return this;
}

LTDBBody *LTDBBody::release()
{
    --ref;
    if( ref < 1 )
        delete this;
    return 0;
}

// Inhalt det LTDBDatei Klasse aus Dateisystem.h
// Konstruktor 
LTDBDatei::LTDBDatei()
    : pfad( new Text() ),
    datKpf( 0 ),
    ref( 1 )
{}

// Destruktor 
LTDBDatei::~LTDBDatei()
{
    if( pfad )
        pfad->release();
    if( datKpf )
        datKpf->release();
}

// nicht constant 
void LTDBDatei::setDatei( Text *pfad ) // Setzt den Pfad zur Datei
{ // Werte der eventuellen vorherigen Datei löschen
    if( datKpf )
        datKpf = datKpf->release();
    // Pfad setzen
    this->pfad->setText( pfad->getThis() );
    pfad->release();
}

void LTDBDatei::erstellen() // Erstellt die Datei
{
    DateiPfadErstellen( pfad->getThis() );
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary );
    int i = 0;
    outF->write( (char *)&i, 2 );
    delete outF;
}

void LTDBDatei::leseDaten( FBalken *f ) // Die Klasse ließt alle Bilder kurz ein, und merkt sich, an welcher stelle in der Datei was ist
{ // Diese Funktion wird ein wenig Zeit in Anspruch nemen, dafüraber danach die anderen schneller machen
    if( DateiExistiert( pfad->getThis() ) )
    {
        if( datKpf )
            datKpf->release();
        datKpf = new LTDBDateiKopf();
        std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
        datKpf->laden( f, inF );
        delete inF;
    }
}

void LTDBDatei::remove() // Löscht die Datei
{
    if( DateiExistiert( pfad->getThis() ) )
    {
        DateiRemove( pfad->getThis() );
        if( datKpf )
            datKpf->release();
    }
}

void LTDBDatei::remove( FBalken *f, Text *name ) // Löscht ein Bild aus der Datei
{
    if( DateiExistiert( pfad->getThis() ) && name )
    {
        if( !datKpf )
            leseDaten( 0 ); // Daten einlesen
        // Prüfen, ob Datei nicht vorhanden
        if( !datKpf )
        {
            name->release();
            return;
        }
        int index = datKpf->getBildIndex( name->getThis() );
        if( index == -1 ) // das bild existiert nicht
        {
            name->release();
            return;
        }
        // Zwischenspeicherpfad ermitteln
        Text *pf_tmp = new Text( pfad->getText() );
        char c = '0';
        pf_tmp->append( "0" );
        for( int i = 0; DateiExistiert( pf_tmp->getThis() ); ++i )
        {
            c = (char)( '0' + ( i % 10 ) );
            if( ( i % 10 ) == 0 )
                pf_tmp->append( "$" );
            pf_tmp->ersetzen( pf_tmp->anzahlVon( (char)( '0' - ( ( i - 1 ) % 10 ) ) ) - 1, (char)( '0' - ( ( i - 1 ) % 10 ) ), c );
        }
        std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary ); // Alte Datei öffnen
        inF->seekg( 0, std::ios::end );
        __int64 datlen = inF->tellg();
        inF->seekg( 0, std::ios::beg );
        std::ofstream *outF = new std::ofstream( pf_tmp->getText(), std::ios::binary ); // Neue Datei öffnen
        if( inF->is_open() && outF->is_open() )
        {
            __int64 position = datKpf->getBildPosition( index );
            datKpf->removeBild( index );
            datKpf->speichern( outF );
            LTDBDateiKopf *kpf_tmp = new LTDBDateiKopf();
            kpf_tmp->laden( 0, inF );
            kpf_tmp->release();
            char byte = 0;
            __int64 pos_minus = inF->tellg() - outF->tellp();
            for( int i = 0; i < index; ++i )
                datKpf->setBildPos( i, datKpf->getBildPosition( i ) - pos_minus );
            // Bytes bis zur Datei kopieren
            for( __int64 i = inF->tellg(); i < position; ++i )
            {
                inF->read( &byte, 1 );
                outF->write( &byte, 1 );
            } // zu löschendes Bild überspringen
            LTDBKopf *delkpf = new LTDBKopf();
            delkpf->laden( inF );
            LTDBBody *delkpr = new LTDBBody( delkpf->getThis() );
            delkpr->laden( f, inF );
            delkpf = delkpf->release();
            delkpr = delkpr->release(); // restliche bytes kopieren
            pos_minus = inF->tellg() - outF->tellp();
            for( __int64 i = (__int64)inF->tellg(); i < datlen; ++i )
            {
                inF->read( &byte, 1 );
                outF->write( &byte, 1 );
            }
            for( int i = index; i < datKpf->getbAnzahl(); ++i )
                datKpf->setBildPos( i, datKpf->getBildPosition( i ) - pos_minus );
            outF->seekp( 0, std::ios::beg );
            datKpf->speichern( outF );
            inF->close();
            outF->close();
            DateiRemove( pfad->getThis() );
            DateiUmbenennen( pf_tmp->getThis(), pfad->getThis() );
        }
        delete inF;
        delete outF;
        pf_tmp = pf_tmp->release();
    }
    if( name )
        name = name->release();
}

Bild *LTDBDatei::laden( FBalken *f, Text *name ) // Läd ein Bild aus der Datei
{
    if( name )
    {
        if( !DateiExistiert( pfad->getThis() ) )
        {
            name->release();
            return 0;
        }
        if( !datKpf )
            leseDaten( 0 );
        LTDBKopf *k_tmp = new LTDBKopf();
        k_tmp->Init( name->getThis(), Punkt( 0, 0 ) );
        int index = datKpf->getBildIndex( k_tmp->getTitel() );
        k_tmp->release();
        if( index == -1 )
        { // Fehlermeldung 
            Text *fehler = new Text( "Das Bild " );
            fehler->append( name );
            fehler->append( " wurde nicht in der Datei\n" );
            fehler->append( pfad->getThis() );
            fehler->append( " gefunden!" );
            std::cout << fehler << "\n";
            return 0;
        }
        std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
        // Begin der zu ladenden Datei ermitteln
        __int64 position = datKpf->getBildPosition( index );
        inF->seekg( position, std::ios::beg );
        LTDBKopf *kpf = new LTDBKopf();
        kpf->laden( inF );
        Text *t = kpf->getTitel();
        if( !t->istGleich( name->getThis() ) )
        { // Fehlermeldung 
            t->release();
            kpf->release();
            inF->close();
            delete inF;
            Text *fehler = new Text( "Die Datei " );
            fehler->append( pfad );
            fehler->append( " ist ist keine gültige LTDB Datei!" );
#ifdef WIN32
            WMessageBox( 0, new Text( "Fehler" ), fehler, MB_ICONERROR );
#endif
            name->release();
            return 0;
        }
        t->release();
        LTDBBody *kpr = new LTDBBody( kpf->getThis() );
        kpr->laden( f, inF ); // Bild laden
        Bild *ret = kpr->getBild();
        kpr->release();
        kpf->release();
        inF->close();
        delete inF;
        name->release();
        return ret;
    }
    return 0;
}

int LTDBDatei::speichern( FBalken *f, Bild *bild, Text *name ) // Speichert ein Bild in die Datei
{
    int warn = -1;
    if( name && bild )
    {
        if( DateiExistiert( pfad->getThis() ) )
        {
            if( !datKpf )
                leseDaten( 0 );
            int index = datKpf->getBildIndex( name->getThis() );
            if( index == -1 )
            {
                warn = 0;
                LTDBKopf *kpf = new LTDBKopf();
                warn = kpf->Init( name->getThis(), bild->getSize() );
                if( datKpf->getBildIndex( kpf->getTitel() ) != -1 )
                {
                    std::cout << "Es existiert bereits ein Bild mit diesem Namen!\n";
                    bild->release();
                    name->release();
                    kpf->release();
                    return -1;
                }
                // zwischendateipfad suchen
                Text *pf_tmp = new Text( pfad->getText() );
                char c = '0';
                pf_tmp->append( "0" );
                for( int i = 0; DateiExistiert( pf_tmp->getThis() ); ++i )
                {
                    c = (char)( '0' + ( i % 10 ) );
                    if( ( i % 10 ) == 0 )
                        pf_tmp->append( "$" );
                    pf_tmp->ersetzen( pf_tmp->anzahlVon( (char)( '0' - ( ( i - 1 ) % 10 ) ) ) - 1, (char)( '0' - ( ( i - 1 ) % 10 ) ), c );
                }
                std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
                inF->seekg( 0, std::ios::end );
                __int64 datLen = inF->tellg();
                inF->seekg( 0, std::ios::beg );
                std::ofstream *outF = new std::ofstream( pf_tmp->getText(), std::ios::binary );
                datKpf->addBild( kpf->getTitel() );
                index = datKpf->getBildIndex( kpf->getTitel() );
                datKpf->speichern( outF );
                LTDBDateiKopf *kpf_tmp = new LTDBDateiKopf();
                kpf_tmp->laden( 0, inF );
                kpf_tmp->release();
                __int64 pos_plus = outF->tellp() - inF->tellg();
                for( int i = 0; i < index; ++i )
                    datKpf->setBildPos( i, datKpf->getBildPosition( i ) + pos_plus );
                datKpf->setBildPos( index, datLen + pos_plus );
                outF->seekp( 0, std::ios::beg );
                datKpf->speichern( outF );
                char byte = 0;
                for( __int64 i = inF->tellg(); i < datLen; ++i )
                {
                    inF->read( &byte, 1 );
                    outF->write( &byte, 1 );
                }
                kpf->speichern( outF ); // Bild Kopf speichern
                LTDBBody *kpr = new LTDBBody( kpf->getThis() );
                kpr->setBild( bild->getThis() );
                kpr->speichern( f, outF ); // Bild speichern
                kpf->release();
                kpr->release();
                inF->close();
                outF->close();
                delete inF;
                delete outF;
                DateiRemove( pfad->getThis() );
                DateiUmbenennen( pf_tmp, pfad->getThis() );
            }
        }
    }
    if( name )
        name->release();
    if( bild )
        bild->release();
    return warn;
}

RCArray< Text > *LTDBDatei::zBildListe() // Listet alle Bilder in der datei auf
{
    if( !datKpf )
        leseDaten( 0 );
    if( datKpf )
        return datKpf->zBildListe();
    return 0;
}

// constant 
Text *LTDBDatei::getPfad() const // Gibt den Pfad zur Datei zurück
{
    return pfad->getThis();
}

int LTDBDatei::getBildAnzahl() const
{
    if( !datKpf )
        return 0;
    return datKpf->getbAnzahl();
}

bool LTDBDatei::istOffen() const // Prüft, ob die Datei geöffnet ist
{
    if( !pfad )
        return 0;
    return DateiExistiert( pfad->getThis() );
}

// Reference Counting
LTDBDatei *LTDBDatei::getThis()
{
    ++ref;
    return this;
}

LTDBDatei *LTDBDatei::release()
{
    --ref;
    if( ref < 1 )
        delete this;
    return 0;
}
#ifdef WIN32
// LTDS Dateivormat 
// Inhalt der LTDSPixel Klasse aus DateiSystem.h
// Konstruktor 
LTDSPixel::LTDSPixel( LTDSPixel *davor )
    : ref( 1 ),
    index( 0 ),
    iA( 0 ),
    miA( 8 ),
    maxIndex( 1 ),
    änder( 0 ),
    änderA( 0 ),
    komp( 0 ),
    alpha( 0 ),
    davor( davor )
{}

// Destruktor 
LTDSPixel::~LTDSPixel()
{
    if( davor )
        davor->release();
}

// nicht constant 
// zum Laden gedacht 
bool LTDSPixel::addBitZuFarbe( unsigned char bit )
{
    if( änderA && iA != miA ) // Das Bit gehört zu Alpha
    {
        alpha |= ( ( bit & Bits( 1 ) ) ) << ( 7 - komp - iA );
        ++iA;
    }
    else // Das Bit gehört zum nächsten Pixel
        return false;
    return true;
}

char LTDSPixel::addByte( char byte, char begin ) // gibt ende des Pixels zurück, -1 wenn nicht zu ende
{
    if( begin >= 8 || begin < 0 )
        return -1;
    for( int i = begin; i < 8; ++i )
    {
        switch( index )
        {
        case 0:
            // Das erste Bit eines Pixels speichert, ob sich an der Komprimierung etwas änderte
            änder = ( ( byte >> ( 7 - i ) ) & Bits( 1 ) ) == 1;
            if( !änder ) // Ändert sich nichts an der Komprimierung, so werden die Werte vom vorherigen Pixel übernommen
            {
                if( !davor ) // Die Datei ist beschädigt ( Der erste Pixel kann nicht von dem davor Übernemen )
                {
                    MessageBox( NULL, "Fehler, die Bilddatei ist beschädigt", "Fehler", MB_ICONERROR );
                    exit( 0 );
                }
                änderA = davor->getÄnderA();
                komp = davor->getKomp();
                miA -= komp;
                if( !änderA )
                    alpha = davor->getA();
                maxIndex += änderA * ( 8 - komp ); // Bestimmung der Länge
                // des Pixels in Bits. Jede Farbe hat von grund auf 8 Bits, durch komprimierung kanns kleiner sein
            }
            else
                maxIndex += 4; // Da der Pixel nicht die Werte des vorherigen übernimmt, wird er um 4 Bits größer
            break;
        case 1: // Das zweite Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( änder ) // Das Bit gehört zu den 4 Komprimierungsbits
                änderA = ( ( byte >> ( 7 - i ) ) & Bits( 1 ) ) == 1;
            else
            {
                if( !addBitZuFarbe( byte >> ( 7 - i ) ) ) // Das Bit gehört zur Farbe
                    return i;
            }
            break;
        case 2: // Das sechste Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( änder ) // Das Bit gehört zu den 4 Komprimierungsbits
                komp |= (unsigned char)( ( byte >> ( 7 - i ) ) & Bits( 1 ) ) << 2;
            else
            {
                if( !addBitZuFarbe( byte >> ( 7 - i ) ) ) // Das Bit gehört zur Farbe
                    return i;
            }
            break;
        case 3: // Das siebte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( änder ) // Das Bit gehört zu den 4 Komprimierungsbits
                komp |= (unsigned char)( ( byte >> ( 7 - i ) ) & Bits( 1 ) ) << 1;
            else
            {
                if( !addBitZuFarbe( byte >> ( 7 - i ) ) ) // Das Bit gehört zur Farbe
                    return i;
            }
            break;
        case 4: // Das achte Bit eines Pixels speichert entweder die Komprimierungsart, oder schon die Farbe
            if( änder ) // Das Bit gehört zu den 4 Komprimierungsbits
            {
                komp |= (unsigned char)( ( byte >> ( 7 - i ) ) & Bits( 1 ) );
                // Das war das letzte Komprimierungsbit
                // Komprimierung auswerten 
                miA -= komp;
                if( !änderA )
                    alpha = davor->getA();
                maxIndex += änderA * ( 8 - komp ); // Bitlänge des Pixels
            }
            else
            {
                if( !addBitZuFarbe( byte >> ( 7 - i ) ) ) // Das Bit gehört zur Farbe
                    return i;
            }
            break;
        default: // Die restlichen Bits speichern alle die Farbwerte des Pixels
            if( !addBitZuFarbe( byte >> ( 7 - i ) ) ) // Das Bit gehört zur Farbe
                return i;
            break;
        }
        ++index;
        if( index >= maxIndex )
        {
            if( davor )
            {
                if( änderA )
                    alpha = davor->getA() + alpha;
                davor = davor->release();
            }
            return i + 1;
        }
    }
    return -1;
}

// zum speichern gedacht 
void LTDSPixel::setAlpha( unsigned char alpha ) // setzt die Farbe des Pixels
{
    this->alpha = alpha;
}

void LTDSPixel::Komp() // Komprimiert den Pixel
{
    maxIndex = 1;
    if( !davor )
    {// Das ist der erste Pixel
        änder = 1;
        änderA = 1;
        maxIndex += 4;
        miA = getBits( alpha );
    }
    else
    { // Es wird die differenz zum vorrigen Pixel gespeichert
        miA = getBits( alpha - davor->getA() );
        if( alpha != davor->getA() )
            änderA = 1;
        else
            änderA = 0;
    }// Prüfen ob sich etwas ändert
    if( !miA && änderA )
        ++miA;
    komp = 8 - miA;
    maxIndex += änderA * miA;
    if( davor )
    {
        if( änderA != davor->getÄnderA() ||
            komp != davor->getKomp() )
        { // Es ändert sich etwas
            änder = 1;
            maxIndex += 4;
        }
        else
        { // Es ändert sich nichts
            änder = 0;
        }
    }
}

bool LTDSPixel::getNextFarbeBit( char &byte, int i )
{
    unsigned char AA = alpha;
    if( davor )
    {
        AA -= davor->getA();
    }
    if( änderA && iA != miA ) // Das Bit gehört zu Alpha
    {
        byte |= ( ( AA >> ( 7 - komp - iA ) ) & Bits( 1 ) ) << ( 7 - i );
        ++iA;
    }
    else // Der Pixel ist bereits zu ende
        return false;
    return true;
}

char LTDSPixel::getNextByte( char &byte, int bbegin ) // Gibt die nächsten Bits Zurück, -1 wenn der Pixel nicht zu ende ist
{
    // bbegin gibt an wohin in die byte-variable geschrieben werden soll
    // die Funktion gibt das ende des Pixels in der byte-variable zurück
    // -1 heißt, dass der Pixel nicht zu ende ist
    for( int i = bbegin; i < 8; ++i )
    {
        switch( index )
        {
        case 0: // Das erste Bit des Pixels speichert, ob sich etwas an der Komprimierung ändert
            byte |= ( (int)änder & Bits( 1 ) ) << ( 7 - i );
            break;
        case 1: // Das zweite Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( änder ) // Komprimierung
                byte |= ( (int)änderA & Bits( 1 ) ) << ( 7 - i );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return i;
            }
            break;
        case 2: // Das zweite Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( änder ) // Komprimierung
                byte |= ( ( komp >> 2 ) & Bits( 1 ) ) << ( 7 - i );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return i;
            }
            break;
        case 3: // Das dritte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( änder ) // Komprimierung
                byte |= ( ( komp >> 1 ) & Bits( 1 ) ) << ( 7 - i );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return i;
            }
            break;
        case 4: // Das vierte Bit des Pixels speichert entweder schon die Farbe oder die Komprimierung
            if( änder ) // Komprimierung
                byte |= ( komp & Bits( 1 ) ) << ( 7 - i );
            else // Farbe
            {
                if( !getNextFarbeBit( byte, i ) )
                    return i;
            }
            break;
        default: // Die restlichen Bits speichern die Farbe des Pixels
            if( !getNextFarbeBit( byte, i ) )
                return i;
            break;
        }
        ++index;
        if( index >= maxIndex )
        {
            if( davor )
                davor = davor->release();
            return i + 1;
        }
    }
    return -1;
}

// constant 
unsigned char LTDSPixel::getKomp() const // hat sich die Komprimierung geändert
{
    return komp;
}

bool LTDSPixel::getÄnderA() const // gibt zurück, ob sich der alphawert ändert
{
    return änderA;
}

unsigned char LTDSPixel::getA() const // gibt Alpha zurück
{
    return alpha;
}

// Reference Counting 
LTDSPixel *LTDSPixel::getThis()
{
    ++ref;
    return this;
}

LTDSPixel *LTDSPixel::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDSDateiKopf Klasse aus Dateisystem.h
// Konstruktor 
LTDSDateiKopf::LTDSDateiKopf()
    : ref( 1 ),
    sganzahl( 0 ),
    gr( 0 ),
    pos( 0 )
{}

// Destruktor 
LTDSDateiKopf::~LTDSDateiKopf()
{
    delete[]gr;
    delete[]pos;
}

// nicht constant 
void LTDSDateiKopf::laden( std::ifstream *inF ) // Lät aus inF
{
    if( inF->good() && inF->is_open() )
    {
        inF->read( (char *)&sganzahl, 1 );
        delete[]gr;
        delete[]pos;
        gr = new unsigned char[ sganzahl + 1 ];
        pos = new int[ sganzahl + 1 ];
        gr[ sganzahl ] = 0;
        pos[ sganzahl ] = 0;
        for( int i = 0; i < sganzahl; ++i )
        {
            inF->read( (char*)&gr[ i ], 1 );
            inF->read( (char*)&pos[ i ], 4 );
        }
    }
}

void LTDSDateiKopf::addSG( char sg ) // Schriftgröße hinzufügen
{
    ++sganzahl;
    unsigned char *gr_tmp = gr;
    int *pos_tmp = pos;
    gr = new unsigned char[ sganzahl + 1 ];
    pos = new int[ sganzahl + 1 ];
    gr[ sganzahl ] = 0;
    pos[ sganzahl ] = 0;
    if( sganzahl - 1 > 0 )
    {
        memcpy( gr, gr_tmp, 1 * ( sganzahl - 1 ) );
        memcpy( pos, pos_tmp, 4 * ( sganzahl - 1 ) );
    }
    delete[]gr_tmp;
    delete[]pos_tmp;
    pos[ sganzahl - 1 ] = 0;
    gr[ sganzahl - 1 ] = sg;
}

void LTDSDateiKopf::removeSG( char sg ) // Schriftgröße entfernen
{
    bool hatsg = 0;
    int sgpos = 0;
    for( int i = 0; i < sganzahl; ++i )
    {
        hatsg = gr[ i ] == sg;
        sgpos = i;
        if( hatsg )
            break;
    }
    if( hatsg )
    {
        --sganzahl;
        unsigned char *gr_tmp = gr;
        int *pos_tmp = pos;
        gr = new unsigned char[ sganzahl + 1 ];
        pos = new int[ sganzahl + 1 ];
        gr[ sganzahl ] = 0;
        pos[ sganzahl ] = 0;
        for( int i = 0; i < sgpos; ++i )
        {
            gr[ i ] = gr_tmp[ i ];
            pos[ i ] = pos_tmp[ i ];
        }
        for( int i = sgpos + 1; i < sganzahl; ++i )
        {
            gr[ i - 1 ] = gr_tmp[ i ];
            pos[ i - 1 ] = pos_tmp[ i ];
        }
        delete[]gr_tmp;
        delete[]pos_tmp;
    }
}

// constant 
void LTDSDateiKopf::speichern( std::ofstream *outF ) const // Speichert nach outF
{
    if( outF->is_open() && outF->good() )
    {
        outF->write( (char*)&sganzahl, 1 );
        for( int i = 0; i < sganzahl; ++i )
        {
            outF->write( (char*)&gr[ i ], 1 );
            outF->write( (char*)&pos[ i ], 4 );
        }
    }
}

unsigned char *LTDSDateiKopf::getSchriftGrößeList() const // gibt eine Liste mit gespeicherten Schriftgrößen zurück
{
    return gr;
}

int *LTDSDateiKopf::getPositionList() const // gibt eine Positionsliste der gespeicherten Schriftgrößen zurück
{
    return pos;
}

int LTDSDateiKopf::getSchriftGrößeAnzahl() const // gibt die Anzahl der gespeicherten Schriftgrößen zurück
{
    return sganzahl;
}

// Reference Counting 
LTDSDateiKopf *LTDSDateiKopf::getThis()
{
    ++ref;
    return this;
}

LTDSDateiKopf *LTDSDateiKopf::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDSSchriftKopf Klasse aus Dateisystem.h
// Konstruktor 
LTDSSchriftKopf::LTDSSchriftKopf()
    : ref( 1 ),
    schriftSize( 0 ),
    zeichen( 0 ),
    pos( 0 ),
    zeichenAnzahl( 0 )
{}

// Destruktor 
LTDSSchriftKopf::~LTDSSchriftKopf()
{
    delete[]pos;
    delete[]zeichen;
}

// nicht constant 
void LTDSSchriftKopf::laden( std::ifstream *inF ) // läht von inF
{
    if( inF->good() && inF->is_open() )
    {
        inF->read( (char*)&schriftSize, 1 );
        inF->read( (char*)&zeichenAnzahl, 1 );
        delete[]pos;
        delete[]zeichen;
        zeichen = new unsigned char[ zeichenAnzahl ];
        pos = new int[ zeichenAnzahl + 1 ];
        for( int i = 0; i < zeichenAnzahl; ++i )
        {
            inF->read( (char*)&zeichen[ i ], 1 );
            inF->read( (char*)&pos[ i ], 4 );
        }
        pos[ zeichenAnzahl ] = 0;
    }
}

void LTDSSchriftKopf::setSchriftgröße( unsigned char gr ) // setze schriftgröße
{
    schriftSize = gr;
}

void LTDSSchriftKopf::setZeichenAlphabet( Alphabet *alphabet ) // setzt die Zeichen von alphabet
{
    int count = 0;
    for( int i = 0; i < 256; ++i )
    {
        Buchstabe *zeich = alphabet->zBuchstabe( i );
        if( zeich )
            ++count;
    }
    delete[]zeichen;
    delete[]pos;
    zeichen = new unsigned char[ count ];
    pos = new int[ count + 1 ];
    pos[ count ] = 0;
    zeichenAnzahl = count;
    count = 0;
    for( int i = 0; i < 256; ++i )
    {
        Buchstabe *zeich = alphabet->getBuchstabe( i );
        if( zeich )
        {
            zeichen[ count ] = i;
            pos[ count ] = 0;
            ++count;
            zeich->release();
        }
    }
    schriftSize = alphabet->getSchriftSize();
    alphabet->release();
}

void LTDSSchriftKopf::addZeichen( unsigned char zeichen ) // Zeichen hinzufügen
{
    ++zeichenAnzahl;
    unsigned char *zeichen_tmp = this->zeichen;
    int *pos_tmp = pos;
    this->zeichen = new unsigned char[ zeichenAnzahl ];
    pos = new int[ zeichenAnzahl + 1 ];
    if( zeichenAnzahl - 1 > 0 )
    {
        memcpy( this->zeichen, zeichen_tmp, 1 * ( zeichenAnzahl - 1 ) );
        memcpy( pos, pos_tmp, 4 * ( zeichenAnzahl - 1 ) );
    }
    delete[]zeichen_tmp;
    delete[]pos_tmp;
    this->zeichen[ zeichenAnzahl - 1 ] = zeichen;
    pos[ zeichenAnzahl - 1 ] = 0;
    pos[ zeichenAnzahl ] = 0;
}

void LTDSSchriftKopf::removeZeichen( unsigned char zeich ) // Zeichen entfernen
{
    bool hatZ = 0;
    int zPos = 0;
    for( int i = 0; i < zeichenAnzahl; ++i )
    {
        hatZ = zeichen[ i ] == zeich;
        zPos = i;
        if( hatZ )
            break;
    }
    if( hatZ )
    {
        --zeichenAnzahl;
        unsigned char *zeichen_tmp = zeichen;
        int *pos_tmp = pos;
        zeichen = new unsigned char[ zeichenAnzahl ];
        pos = new int[ zeichenAnzahl + 1 ];
        for( int i = 0; i < zPos; ++i )
        {
            zeichen[ i ] = zeichen_tmp[ i ];
            pos[ i ] = pos_tmp[ i ];
        }
        for( int i = zPos + 1; i <= zeichenAnzahl; ++i )
        {
            zeichen[ i - 1 ] = zeichen_tmp[ i ];
            pos[ i - 1 ] = pos_tmp[ i ];
        }
        pos[ zeichenAnzahl ] = 0;
        delete[]zeichen_tmp;
        delete[]pos_tmp;
    }
}

// constant 
void LTDSSchriftKopf::speichern( std::ofstream *outF ) const // speichert nach outF
{
    if( outF->good() && outF->is_open() )
    {
        outF->write( (char*)&schriftSize, 1 );
        outF->write( (char*)&zeichenAnzahl, 1 );
        for( int i = 0; i < zeichenAnzahl; ++i )
        {
            outF->write( (char*)&zeichen[ i ], 1 );
            outF->write( (char*)&pos[ i ], 4 );
        }
    }
}

unsigned char LTDSSchriftKopf::getSchriftGröße() const // gibt die Schriftgröße zurück
{
    return schriftSize;
}

unsigned char LTDSSchriftKopf::getZeichenAnzahl() const // gibt die Zeichenanzahl zurück
{
    return zeichenAnzahl;
}

int *LTDSSchriftKopf::getPositionen() const // gibt die Zeichenpositionen zurück
{
    return pos;
}

unsigned char *LTDSSchriftKopf::getZeichen() const // gibt die zeichen zurück
{
    return zeichen;
}

// Reference Counting 
LTDSSchriftKopf *LTDSSchriftKopf::getThis()
{
    ++ref;
    return this;
}

LTDSSchriftKopf *LTDSSchriftKopf::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDSBuchstabenKopf Klasse aus Dateisystem.h
// Konstruktor 
LTDSBuchstabenKopf::LTDSBuchstabenKopf()
    : ref( 1 ),
    zeichen( 0 ),
    size( 0, 0 )
{}

// nicht constant 
void LTDSBuchstabenKopf::laden( std::ifstream *inF ) // lät aus inF
{
    if( inF->good() && inF->is_open() )
    {
        inF->read( (char*)&zeichen, 1 );
        inF->read( (char*)&size.x, 1 );
        inF->read( (char*)&size.y, 1 );
    }
}

void LTDSBuchstabenKopf::init( unsigned char zeichen, const Punkt &größe ) // initialisierung( für speichern )
{
    this->zeichen = zeichen;
    this->size = größe;
}

void LTDSBuchstabenKopf::init( unsigned char zeichen, int br, int hö )
{
    this->zeichen = zeichen;
    size.x = br, size.y = hö;
}

// constant 
void LTDSBuchstabenKopf::speichern( std::ofstream *outF ) const // speichertn nach outF
{
    if( outF->good() && outF->is_open() )
    {
        outF->write( (char*)&zeichen, 1 );
        outF->write( (char*)&size.x, 1 );
        outF->write( (char*)&size.y, 1 );
    }
}

unsigned char LTDSBuchstabenKopf::getZeichen() const // gibt das Zeichen zurück
{
    return zeichen;
}

int LTDSBuchstabenKopf::getBreite() const // gibt die Breite zurück
{
    return size.x;
}

int LTDSBuchstabenKopf::getHöhe() const // gibt die höhe zurück
{
    return size.y;
}

const Punkt &LTDSBuchstabenKopf::getGröße() const // gibt die Größe zurück
{
    return size;
}

// Reference Counting 
LTDSBuchstabenKopf *LTDSBuchstabenKopf::getThis()
{
    ++ref;
    return this;
}

LTDSBuchstabenKopf *LTDSBuchstabenKopf::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDSBuchstabenKörper Klasse aus Dateisystem.h
// Konstruktor 
LTDSBuchstabenKörper::LTDSBuchstabenKörper( LTDSBuchstabenKopf *kopf )
    : ref( 1 ),
    size( kopf->getGröße() ),
    zeichen( kopf->getZeichen() ),
    buchstabe( new Buchstabe() )
{
    buchstabe->NeuBuchstabe( size );
    kopf->release();
}

// Destruktor 
LTDSBuchstabenKörper::~LTDSBuchstabenKörper()
{
    if( buchstabe )
        buchstabe->release();
}

// nicht constant 
void LTDSBuchstabenKörper::setBuchstabe( Buchstabe *zeichen ) // setzt den Buchstaben
{
    if( buchstabe )
        buchstabe->release();
    buchstabe = zeichen;
}

void LTDSBuchstabenKörper::laden( std::ifstream *inF ) // Läht aus inF
{
    if( inF->good() && inF->is_open() )
    {
        LTDSPixel *vorher = 0;
        LTDSPixel *jetzt = new LTDSPixel( vorher );
        char byte = 0;
        int beg = 0;
        int ende = -1;
        for( int i = 0; i < size.x * size.y; ++i )
        {
            if( !jetzt ) // wenn es nicht der erste Pixel ist
                jetzt = new LTDSPixel( vorher->getThis() );
            int ende = -1;
            while( ende < 0 ) // Pixel laden
            {
                if( beg == 0 )
                    inF->read( &byte, 1 );
                ende = jetzt->addByte( byte, beg ); // byte auswerten
                beg = 0;
            }
            beg = ende;
            if( beg == 8 )
                beg = 0;
            if( buchstabe )
                buchstabe->setPixel( i, jetzt->getA() );
            if( vorher )
                vorher = vorher->release();
            vorher = jetzt->getThis();
            jetzt = jetzt->release();
        }
        if( vorher )
            vorher->release();
        if( jetzt )
            jetzt->release();
    }
}

// constant 
void LTDSBuchstabenKörper::speichern( std::ofstream *outF ) const // speichert nach outF
{
    if( outF->good() && outF->is_open() )
    {
        LTDSPixel *vorher = 0; // Letzter gespeicherter Pixel
        LTDSPixel *jetzt = new LTDSPixel( 0 ); // Der momentan zu speichernde Pixel
        int begin = 0, ende = 0; // Pixelbeginn, endposition in der byte variable
        char byte = 0; // Der nächste byte der Datei
        bool w = 0;
        unsigned char *alphaBuff = buchstabe->getBuff();
        for( int i = 0; i < size.x * size.y; ++i ) // für jeden Pixel
        {
            if( !jetzt ) // wenn es nicht der erste Pixel ist
                jetzt = new LTDSPixel( vorher->getThis() );
            jetzt->setAlpha( alphaBuff[ i ] ); // Farbe des Pixels setzen
            jetzt->Komp(); // Pixel komprimieren
            ende = -1;
            while( ende < 0 ) // byte befüllen
            {
                ende = jetzt->getNextByte( byte, begin );
                begin = 0;
                w = 0;
                if( ende == -1 || ende == 8 ) // byte speichern
                {
                    outF->write( &byte, 1 );
                    w = 1;
                    byte = 0;
                }
            } // Pixel fertig
            begin = ende;
            if( begin == 8 )
                begin = 0;
            if( vorher )
                vorher->release();
            vorher = jetzt->getThis(); // dieser wird zu letzter
            jetzt = jetzt->release();
        }
        if( vorher )
            vorher = vorher->release();
        if( !w )
        {
            outF->write( &byte, 1 ); // Das letzte byte speichern
        }
        outF->flush(); // dateistream speichern
    }
}

Buchstabe *LTDSBuchstabenKörper::getBuchstabe() const // gibt den Buchstaben zurück
{
    return buchstabe->getThis();
}

unsigned char LTDSBuchstabenKörper::getZeichen() const // gibt das Zeichen zurück
{
    return zeichen;
}

// Reference Counting 
LTDSBuchstabenKörper *LTDSBuchstabenKörper::getThis()
{
    ++ref;
    return this;
}

LTDSBuchstabenKörper *LTDSBuchstabenKörper::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}

// Inhalt der LTDSDatei Klasse aus DAteisystem.h
// Konstruktor 
LTDSDatei::LTDSDatei()
    : ref( 1 ),
    pfad( new Text() ),
    dateiKopf( 0 )
{}

// Destruktor 
LTDSDatei::~LTDSDatei()
{
    if( dateiKopf )
        dateiKopf->release();
    pfad->release();
}

// nicht constant 
void LTDSDatei::setPfad( Text *txt ) // setzt den Pfad zur Datei
{
    if( dateiKopf )
        dateiKopf = dateiKopf->release();
    pfad->setText( txt->getText() );
    txt->release();
}

void LTDSDatei::leseDaten() // ließt den Dateikopf
{
    if( !DateiExistiert( pfad->getThis() ) )
        return;
    if( dateiKopf )
        dateiKopf->release();
    dateiKopf = new LTDSDateiKopf();
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
    dateiKopf->laden( inF );
    inF->close();
    delete inF;
}

void LTDSDatei::addSchriftgröße( Alphabet *alphabet ) // fügt eine Schriftgröße hinzu
{
    if( !DateiExistiert( pfad->getThis() ) ) // prüfen, ob die Datei existiert
        return;
    if( !dateiKopf ) // prüfen, ob der Dateikopf schon gelesen wurde
        leseDaten();
    int sgröße = alphabet->getSchriftSize(); // Schriftgröße die hinzugefügt werden soll
    unsigned char *sglist = dateiKopf->getSchriftGrößeList(); // Liste von bereits vorhandenen Schriftgrößen
    unsigned char sganzahl = dateiKopf->getSchriftGrößeAnzahl(); // Anzahl der bereits vorhandenen Schriftgrößen
    for( int i = 0; i < sganzahl; ++i ) // prüfen, ob die Schriftgröße bereits existiert
    {
        if( sglist[ i ] == sgröße )
        {
            alphabet->release();
            return;
        }
    }
    dateiKopf->addSG( sgröße ); // Schriftgröße dem Dateikopf hinzufügen
    int *sgPosList = dateiKopf->getPositionList(); // Liste mit positionen der Schriftgrößen
    sglist = dateiKopf->getSchriftGrößeList(); // Liste von bereits vorhandenen Schriftgrößen
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary ); // alte Datei
    pfad->append( "0" );
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary ); // neue Datei
    dateiKopf->speichern( outF ); // Dateikopf in neue datei speichern
    inF->seekg( 1 + 5 * sganzahl, std::ios::beg ); // Position der ersten Schriftgröße in der alten Datei
    for( int i = 0; i < sganzahl; ++i ) // Buchstabenpositionen aller Schriftgrößen aktualisieren
    {
        LTDSSchriftKopf *sgKpf_tmp = new LTDSSchriftKopf(); // Schriftgrößen Kopf
        sgKpf_tmp->laden( inF ); // aus alter Datei laden
        int *zeichP_tmp = sgKpf_tmp->getPositionen(); // Zeichenpositionen der Schriftgröße
        unsigned char zeichA_tmp = sgKpf_tmp->getZeichenAnzahl(); // Anzahl von Zeichen der Schriftgröße
        for( int i1 = 0; i1 < zeichA_tmp; ++i1 )
            zeichP_tmp[ i1 ] += 5; // Buchstabenpositionen aktualisieren
        sgKpf_tmp->speichern( outF ); // Schriftgröße Kopf in neue Datei speichern
        int beginByte = (int)inF->tellg(); // Die restlichen bytes bis zur nächsten Schriftgröße in neue Datei kopieren
        int endByte = sgPosList[ i + 1 ];
        if( !endByte )
        {
            inF->seekg( 0, std::ios::end );
            endByte = (int)inF->tellg();
            inF->seekg( beginByte, std::ios::beg );
        }
        char byte;
        for( int i1 = beginByte; i1 < endByte; ++i1 ) // Kopiervorgang
        {
            inF->read( &byte, 1 );
            outF->write( &byte, 1 );
        }
        sgKpf_tmp->release();
    }
    inF->close(); // Alte datei schließen
    sgPosList[ sganzahl ] = (int)outF->tellp();
    outF->seekp( 0, std::ios::beg );
    for( int i = 0; i < sganzahl; ++i ) // Positionen im Dateikopf aktualisieren
        sgPosList[ i ] += 5;
    dateiKopf->speichern( outF ); // aktualisierter Dateikopf speichern
    outF->seekp( sgPosList[ sganzahl ], std::ios::beg );
    LTDSSchriftKopf *sgkopf = new LTDSSchriftKopf(); // Kopf der neuen Schriftgröße
    sgkopf->setZeichenAlphabet( alphabet->getThis() ); // Kopf der Schriftgröße initialisieren
    sgkopf->speichern( outF ); // Kopf der Schriftgröße speichern
    int *BuchstabenPosList = sgkopf->getPositionen(); // positionen der verschiedenen Zeichen in der Datei( nuch 0 )
    int count = 0;
    for( int i = 0; i < 256; ++i )
    {
        Buchstabe *zeich = alphabet->getBuchstabe( i );
        if( zeich )
        {
            BuchstabenPosList[ count ] = (int)outF->tellp(); // position des Zeichens setzen
            LTDSBuchstabenKopf *zeichKpf = new LTDSBuchstabenKopf(); // Zeichenkopf
            zeichKpf->init( i, zeich->getBreite(), zeich->getHeight() );
            zeichKpf->speichern( outF ); // Zeichenkopf speichern
            LTDSBuchstabenKörper *zeichKörp = new LTDSBuchstabenKörper( zeichKpf->getThis() ); // Zeichenkörper
            zeichKpf->release();
            zeichKörp->setBuchstabe( zeich->getThis() );
            zeichKörp->speichern( outF ); // Zeichenkörper speichern
            zeich->release();
            ++count;
        }
    }
    outF->seekp( sgPosList[ sganzahl ], std::ios::beg );
    sgkopf->speichern( outF ); // aktualisierter Schriftgrößen Kopf speichern
    outF->close();
    Text *pfad2 = new Text();
    pfad2->setText( pfad->getText() );
    pfad->remove( pfad->getLength() - 1, pfad->getLength() );
    DateiRemove( pfad->getThis() ); // Alte datei Löschen
    DateiUmbenennen( pfad2->getThis(), pfad->getThis() ); // neue Datei nach alte umbenennen
    pfad2->release(); // Speicher freigeben
    sgkopf->release();
    delete inF;
    delete outF;
    alphabet->release();
}

void LTDSDatei::addBuchstabe( int gr, Buchstabe *zeich, unsigned char zeichen ) // Fügt einer Schriftgröße einen Buchstaben hinzu
{
    if( !DateiExistiert( pfad->getThis() ) ) // prüfen ob Datei existiert
    {
        zeich->release();
        return;
    }
    if( !dateiKopf ) // prüfen, ob der DAteikopf geladen wurde
        leseDaten();
    unsigned char *sgList = dateiKopf->getSchriftGrößeList(); // Liste mit Schriftgrößen aus der Datei
    int *sgPosList = dateiKopf->getPositionList(); // Liste mit Schriftgrößen positionen aus Datei
    unsigned char sgAnzahl = dateiKopf->getSchriftGrößeAnzahl(); // Anzahl von Schriftgrößen aus der Datei
    int sgNum = -1;
    for( int i = 0; i < sgAnzahl; ++i ) // Position der richtigen Schriftgröße ermitteln
    {
        if( sgList[ i ] == gr )
        {
            sgNum = i;
            break;
        }
    }
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary ); // Alte Datei
    pfad->append( "0" );
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary ); // Neue Datei
    inF->seekg( 1 + 5 * sgAnzahl, std::ios::beg );
    if( sgNum == -1 ) // Die Schriftgröße existiert noch nicht und wird erstellt
    {
        dateiKopf->addSG( gr ); // Schriftgröße dem Dateikopf hinzufügen
        sgPosList = dateiKopf->getPositionList();
        sgList = dateiKopf->getSchriftGrößeList();
        dateiKopf->speichern( outF ); // Dateikopf speichern
        inF->seekg( 1 + 5 * sgAnzahl, std::ios::beg ); // Position der ersten Schriftgröße in der alten Datei
        for( int i = 0; i < sgAnzahl; ++i ) // Buchstabenpositionen aller Schriftgrößen aktualisieren
        {
            LTDSSchriftKopf *sgKpf_tmp = new LTDSSchriftKopf(); // Schriftgrößen Kopf
            sgKpf_tmp->laden( inF ); // aus alter Datei laden
            int *zeichP_tmp = sgKpf_tmp->getPositionen(); // Zeichenpositionen der Schriftgröße
            unsigned char zeichA_tmp = sgKpf_tmp->getZeichenAnzahl(); // Anzahl von Zeichen der Schriftgröße
            for( int i1 = 0; i1 < zeichA_tmp; ++i1 )
                zeichP_tmp[ i1 ] += 5; // Buchstabenpositionen aktualisieren
            sgKpf_tmp->speichern( outF ); // Schriftgröße Kopf in neue Datei speichern
            int beginByte = (int)inF->tellg(); // Die restlichen bytes bis zur nächsten Schriftgröße in neue Datei kopieren
            int endByte = sgPosList[ i + 1 ];
            if( i + 1 >= sgAnzahl )
            {
                inF->seekg( 0, std::ios::end );
                endByte = (int)inF->tellg();
                inF->seekg( beginByte, std::ios::beg );
            }
            char byte;
            for( int i1 = beginByte; i1 < endByte; ++i1 ) // Kopiervorgang
            {
                inF->read( &byte, 1 );
                outF->write( &byte, 1 );
            }
            sgKpf_tmp->release();
        }
        sgPosList[ sgAnzahl ] = (int)outF->tellp();
        outF->seekp( 0, std::ios::beg );
        for( int i = 0; i < sgAnzahl; ++i ) // Positionen im Dateikopf aktualisieren
            sgPosList[ i ] += 5;
        dateiKopf->speichern( outF ); // aktualisierter Dateikopf speichern
        outF->seekp( sgPosList[ sgAnzahl ], std::ios::beg );
        LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf(); // Schriftgröße Kopf initialisiern
        sgKpf->setSchriftgröße( gr );
        sgKpf->addZeichen( zeichen );
        sgKpf->getPositionen()[ 0 ] = (int)outF->tellp() + 7;
        sgKpf->speichern( outF ); // Schriftgröße Kopf speichern
        sgKpf->release();
        LTDSBuchstabenKopf *zeichKpf = new LTDSBuchstabenKopf(); // Buchstabenkopf
        zeichKpf->init( zeichen, zeich->getSize() );
        zeichKpf->speichern( outF ); // Buchstabenkopf speichern
        LTDSBuchstabenKörper *zeichKörp = new LTDSBuchstabenKörper( zeichKpf->getThis() ); // Buchstabenkörper
        zeichKpf->release();
        zeichKörp->setBuchstabe( zeich->getThis() );
        zeichKörp->speichern( outF ); // Buchstabenkörper speichern
        zeichKörp->release();
    }
    else
    {
        dateiKopf->speichern( outF ); // Dateikopf speichern
        int beginByte = 1 + 5 * sgAnzahl; // Die restlichen bytes bis zur nächsten Schriftgröße in neue Datei kopieren
        int endByte = sgPosList[ sgNum ];
        char byte;
        for( int i1 = beginByte; i1 < endByte; ++i1 ) // Kopiervorgang bis zur angegebenen größe
        {
            inF->read( &byte, 1 );
            outF->write( &byte, 1 );
        }
        LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf(); // Schriftgröße Kopf
        sgKpf->laden( inF );
        for( int i = 0; i < sgKpf->getZeichenAnzahl(); ++i )
            sgKpf->getPositionen()[ i ] += 5;
        sgKpf->addZeichen( zeichen );
        int indexPlus = 5;
        int zeichenPos = sgPosList[ sgNum + 1 ]; // position des neuen Zeichens
        if( sgNum + 1 >= sgAnzahl )
        {
            int tmp = (int)inF->tellg();
            inF->seekg( 0, std::ios::end );
            zeichenPos = (int)inF->tellg();
            inF->seekg( tmp, std::ios::beg );
        }
        zeichenPos += indexPlus;
        sgKpf->getPositionen()[ sgKpf->getZeichenAnzahl() - 1 ] = zeichenPos;
        sgKpf->speichern( outF ); // Schriftgröße Kopf speichern
        sgKpf->release();
        for( int i = (int)inF->tellg() + indexPlus; i < zeichenPos; ++i ) // Kopiervorgang bis zum Zeichenbeginn
        {
            inF->read( &byte, 1 );
            outF->write( &byte, 1 );
        }
        LTDSBuchstabenKopf *zeichKpf = new LTDSBuchstabenKopf(); // Zeichenkopf
        zeichKpf->init( zeichen, zeich->getSize() );
        zeichKpf->speichern( outF ); // Zeichenkopf speichern
        LTDSBuchstabenKörper *zeichKörp = new LTDSBuchstabenKörper( zeichKpf->getThis() ); // Zeichenkörper
        zeichKpf->release();
        zeichKörp->setBuchstabe( zeich->getThis() );
        zeichKörp->speichern( outF ); // Zeichenkörper speichern
        zeichKörp->release();
        int nowPos = (int)outF->tellp();
        indexPlus += nowPos - zeichenPos;
        for( int i = sgNum + 1; i < sgAnzahl; ++i ) // Dateikopf aktualisieren
            sgPosList[ i ] += indexPlus;
        outF->seekp( 0, std::ios::beg );
        dateiKopf->speichern( outF ); // Dateikopf speichern
        outF->seekp( nowPos, std::ios::beg );
        for( int i = sgNum + 1; i < sgAnzahl; ++i ) // Restliche Schriftgrößen aktualisieren
        {
            LTDSSchriftKopf *sgKpf_tmp = new LTDSSchriftKopf(); // Schriftgrößen Kopf
            sgKpf_tmp->laden( inF ); // aus alter Datei laden
            int *zeichP_tmp = sgKpf_tmp->getPositionen(); // Zeichenpositionen der Schriftgröße
            unsigned char zeichA_tmp = sgKpf_tmp->getZeichenAnzahl(); // Anzahl von Zeichen der Schriftgröße
            for( int i1 = 0; i1 < zeichA_tmp; ++i1 )
                zeichP_tmp[ i1 ] += indexPlus; // Buchstabenpositionen aktualisieren
            sgKpf_tmp->speichern( outF ); // Schriftgröße Kopf in neue Datei speichern
            int beginByte = (int)inF->tellg(); // Die restlichen bytes bis zur nächsten Schriftgröße in neue Datei kopieren
            int endByte = sgPosList[ i + 1 ];
            if( i + 1 >= sgAnzahl )
            {
                inF->seekg( 0, std::ios::end );
                endByte = (int)inF->tellg();
                inF->seekg( beginByte, std::ios::beg );
            }
            char byte;
            for( int i1 = beginByte; i1 < endByte; ++i1 ) // Kopiervorgang
            {
                inF->read( &byte, 1 );
                outF->write( &byte, 1 );
            }
            sgKpf_tmp->release();
        }
    }
    inF->close();
    outF->close();
    Text *pfad2 = new Text( pfad->getText() );
    pfad->remove( pfad->getLength() - 1, pfad->getLength() );
    DateiRemove( pfad->getThis() ); // Alte Datei löschen
    DateiUmbenennen( pfad2->getThis(), pfad->getThis() ); // Neue Datei in alte umbenennen
    pfad2->release();// Speicher freigeben
    delete inF;
    delete outF;
    zeich->release();
}

void LTDSDatei::löscheSchrifrGröße( int gr ) // Löscht eine Schriftgröße aus der Datei
{
    if( !DateiExistiert( pfad->getThis() ) ) // prüfen, ob Datei existiert
        return;
    if( !dateiKopf ) // prüfen, ob der Dateikopf geladen wurde
        leseDaten();
    unsigned char *sgList = dateiKopf->getSchriftGrößeList(); // Liste mit Schriftgrößen
    unsigned char sgAnzahl = dateiKopf->getSchriftGrößeAnzahl(); // Anzahl der Schriftgrößen
    int sgNum = -1;
    for( int i = 0; i < sgAnzahl; ++i ) // zu löschende Schriftgröße suchen
    {
        if( sgList[ i ] == gr )
        {
            sgNum = i;
            break;
        }
    }
    if( sgNum == -1 ) // Die Schriftgröße ist nicht vorhanden
        return;
    int *sgPosList = dateiKopf->getPositionList();  // Liste von Positionen der Schriftgrößen
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary ); // alte Datei
    pfad->append( "0" );
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary ); // neue Datei
    outF->seekp( 1 + 5 * ( sgAnzahl - 1 ), std::ios::beg );
    inF->seekg( 1 + 5 * sgAnzahl, std::ios::beg );
    for( int i = 0; i < sgNum; ++i ) // Schriftgrößen vor der zu löschenden Schriftgröße aktualisieren
    {
        LTDSSchriftKopf *sgKpf_tmp = new LTDSSchriftKopf(); // Schriftgrößen Kopf
        sgKpf_tmp->laden( inF ); // Schriftgrößen Kopf laden
        int *zeichPosLTmp = sgKpf_tmp->getPositionen(); // Zeichenpositionen
        unsigned char zeichATmp = sgKpf_tmp->getZeichenAnzahl(); // Zeichenanzahl
        for( int i1 = 0; i1 < zeichATmp; ++i1 ) // Position der Zeichen um 5 bytes zurücksetzen
            zeichPosLTmp[ i1 ] -= 5;
        sgKpf_tmp->speichern( outF ); // speichern in Neue Datei
        char byte = 0;
        for( int i1 = (int)inF->tellg(); i1 < sgPosList[ i + 1 ]; ++i1 ) // Den Körper des Zeichens Kopieren
        {
            inF->read( &byte, 1 );
            outF->write( &byte, 1 );
        }
        sgKpf_tmp->release();
    }
    int indexMinus = 5 + sgPosList[ sgNum + 1 ] - (int)inF->tellg();
    inF->seekg( sgPosList[ sgNum + 1 ], std::ios::beg );
    for( int i = sgNum + 1; i < sgAnzahl; ++i ) // Die Schriftgröße nach der zu löschenden Schriftgröße
    {
        LTDSSchriftKopf *sgKpf_tmp = new LTDSSchriftKopf(); // Schriftgrößen Kopf
        sgKpf_tmp->laden( inF ); // Schriftgrößen Kopf laden
        int *zeichPosLTmp = sgKpf_tmp->getPositionen(); // Zeichenpositionen
        unsigned char zeichATmp = sgKpf_tmp->getZeichenAnzahl(); // Zeichenanzahl
        for( int i1 = 0; i1 < zeichATmp; ++i1 ) // Position der Zeichen aktualisieren
            zeichPosLTmp[ i1 ] -= indexMinus;
        sgKpf_tmp->speichern( outF ); // speichern in Neue Datei
        char byte = 0;
        int BeginByte = (int)inF->tellg();
        int EndByte = sgPosList[ i + 1 ];
        if( !EndByte )
        {
            inF->seekg( 0, std::ios::end );
            EndByte = (int)inF->tellg();
            inF->seekg( BeginByte, std::ios::beg );
        }
        for( int i1 = BeginByte; i1 < EndByte; ++i1 ) // Den Körper des Zeichens Kopieren
        {
            inF->read( &byte, 1 );
            outF->write( &byte, 1 );
        }
    }
    for( int i = 0; i < sgNum; ++i ) // Dateikopf aktualisieren
        sgPosList[ i ] -= 5;
    for( int i = sgNum + 1; i < sgAnzahl; ++i )
        sgPosList[ i ] -= indexMinus;
    dateiKopf->removeSG( gr );
    outF->seekp( 0, std::ios::beg );
    dateiKopf->speichern( outF ); // Dateikopf speichern
    inF->close();
    outF->close();
    Text *pfad2 = new Text( pfad->getText() );
    pfad->remove( pfad->getLength() - 1, pfad->getLength() );
    DateiRemove( pfad->getThis() ); // alte Datei löschen
    DateiUmbenennen( pfad2->getThis(), pfad->getThis() ); // neue Datei zu alter umbenennen
    pfad2->release();
    delete inF;
    delete outF;
}

void LTDSDatei::löscheBuchstabe( int gr, unsigned char zeichen ) // Löscht einen Buchstaben aus der Datei
{
    if( !DateiExistiert( pfad->getThis() ) ) // prüfen, ob die Datei existiert
        return;
    if( !dateiKopf ) // prüfen, ob der Dateikopf gelesen wurde
        leseDaten();
    unsigned char *sgList = dateiKopf->getSchriftGrößeList(); // Liste mit Schriftgrößen
    unsigned char sgAnzahl = dateiKopf->getSchriftGrößeAnzahl(); // Anzahl der Schriftgrößen
    int *sgPosList = dateiKopf->getPositionList(); // Liste mit Positionen der Schriftgrößen
    int sgNum = -1;
    for( int i = 0; i < sgAnzahl; ++i ) // Schriftgröße suchen
    {
        if( sgList[ i ] == gr )
        {
            sgNum = i;
            break;
        }
    }
    if( sgNum == -1 ) // Schriftgröße nicht gefunden
        return;
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary ); // alte Datei
    pfad->append( "0" );
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary ); // neue Datei
    int indexMinus = 0;
    inF->seekg( 1 + 5 * sgAnzahl, std::ios::beg );
    dateiKopf->speichern( outF ); // DateiKopf peichern
    for( int i = 0; i < sgAnzahl; ++i )
    {
        LTDSSchriftKopf *sgKopf = new LTDSSchriftKopf(); // Schriftkopf
        sgKopf->laden( inF ); // Schriftkopf laden
        unsigned char sgZeichAnzahl = sgKopf->getZeichenAnzahl(); // Zeichenanzahl
        unsigned char *sgZeichenList = sgKopf->getZeichen(); // Zeichen Liste
        int *sgZPosList = sgKopf->getPositionen(); // Positions Liste
        if( i == sgNum ) // Zeichen befindet sich in dieser Schriftgröße
        {
            int sgZNum = -1;
            for( int i1 = 0; i1 < sgZeichAnzahl; ++i1 ) // Zeichen suchen
            {
                if( sgZeichenList[ i1 ] == zeichen )
                {
                    sgZNum = i1;
                    break;
                }
            }
            if( sgZNum == -1 ) // Zeichen nicht gefunden
            {
                sgKopf->release();
                inF->close();
                outF->close();
                delete inF;
                delete outF;
                DateiRemove( pfad->getThis() );
                pfad->remove( pfad->getLength() - 1, pfad->getLength() );
                return; // abbruch
            }
            outF->seekp( 2 + 5 * ( sgZeichAnzahl - 1 ), std::ios::cur );
            indexMinus += 5;
            for( int i1 = 0; i1 < sgZNum; ++i1 ) // Zeichen vor dem zu löschenden Zeichen
            {
                char byte = 0;
                for( int i2 = sgZPosList[ i1 ]; i2 < sgZPosList[ i1 + 1 ]; ++i2 ) // Kopieren
                {
                    inF->read( &byte, 1 );
                    outF->write( &byte, 1 );
                }
                sgZPosList[ i1 ] -= indexMinus; // Schriftgröße Kopf aktualisieren
            }
            if( !sgZPosList[ sgZNum + 1 ] )
            {
                int endByte = sgPosList[ i + 1 ];
                if( !endByte )
                {
                    int beginByte = (int)inF->tellg();
                    inF->seekg( 0, std::ios::end );
                    endByte = (int)inF->tellg();
                    inF->seekg( beginByte, std::ios::beg );
                }
                indexMinus += endByte - sgZPosList[ sgZNum ];
            }
            else
                indexMinus += sgZPosList[ sgZNum + 1 ] - sgZPosList[ sgZNum ];
            if( sgZNum + 1 < sgZeichAnzahl )
                inF->seekg( sgZPosList[ sgZNum + 1 ], std::ios::beg );
            for( int i1 = sgZNum + 1; i1 < sgZeichAnzahl; ++i1 ) // Zeichen nach dem gelöschten Zeichen
            {
                int beginByte = (int)inF->tellg();
                int endByte = sgZPosList[ i1 + 1 ];
                if( !endByte )
                {
                    inF->seekg( 0, std::ios::end );
                    endByte = (int)inF->tellg();
                    inF->seekg( beginByte, std::ios::beg );
                }
                char byte = 0;
                for( int i2 = beginByte; i2 < endByte; ++i2 ) // Kopieren
                {
                    inF->read( &byte, 1 );
                    outF->write( &byte, 1 );
                }
                sgZPosList[ i1 ] -= indexMinus; // Schriftgröße Kopf aktualisieren
            }
            sgKopf->removeZeichen( zeichen );
        }
        else
        {
            for( int i1 = 0; i1 < sgZeichAnzahl; ++i1 ) // Schriftgröße Kopf aktualisieren
                sgZPosList[ i ] -= indexMinus;
            sgKopf->speichern( outF ); // Schriftgröße Kopf speichern
            int beginByte = (int)inF->tellg();
            int endByte = sgPosList[ i + 1 ];
            if( !endByte )
            {
                inF->seekg( 0, std::ios::end );
                endByte = (int)inF->tellg();
                inF->seekg( beginByte, std::ios::beg );
            }
            char byte;
            for( int i1 = beginByte; i1 < endByte; ++i1 ) // Kopieren
            {
                inF->read( &byte, 1 );
                outF->write( &byte, 1 );
            }
            sgPosList[ i ] -= indexMinus;
        }
        outF->seekp( sgPosList[ i ], std::ios::beg );
        sgKopf->speichern( outF ); // Schriftgröße Kopf speichern
        outF->seekp( sgPosList[ i + 1 ], std::ios::beg );
        sgKopf->release();
    }
    inF->close();
    outF->close();
    Text *pfad2 = new Text( pfad->getText() );
    pfad->remove( pfad->getLength() - 1, pfad->getLength() );
    DateiRemove( pfad->getThis() ); // alte Datei löschen
    DateiUmbenennen( pfad2->getThis(), pfad->getThis() ); // neue Datei nach alte umbenennen
    pfad2->release(); // Speicher freigeben
    delete inF;
    delete outF;
}

void LTDSDatei::löscheDatei() // Löscht die gesamte Datei
{
    if( !DateiExistiert( pfad->getThis() ) )
        return;
    if( !dateiKopf )
        leseDaten();
    DateiRemove( pfad->getThis() );
}

void LTDSDatei::erstelleDatei() // erstellt die Datei
{
    DateiPfadErstellen( pfad->getThis() );
    if( dateiKopf )
        dateiKopf->release();
    dateiKopf = new LTDSDateiKopf();
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary );
    dateiKopf->speichern( outF );
    outF->close();
    delete outF;
}

void LTDSDatei::speicherSchrift( Schrift *schrift ) // Speichert die übergebene Schrift
{
    if( !DateiExistiert( pfad->getThis() ) )
    {
        schrift->release();
        return;
    }
    löscheDatei();
    if( dateiKopf )
        dateiKopf->release();
    dateiKopf = new LTDSDateiKopf();
    for( int i = 0; i < schrift->getAlphabetAnzahl(); ++i )
    {
        Alphabet *alp = schrift->getAlphabetI( i );
        if( alp )
        {
            dateiKopf->addSG( alp->getSchriftSize() );
            alp->release();
        }
    }
    std::ofstream *outF = new std::ofstream( pfad->getText(), std::ios::binary );
    dateiKopf->speichern( outF );
    for( int i = 0; i < schrift->getAlphabetAnzahl(); ++i )
    {
        dateiKopf->getPositionList()[ i ] = (int)outF->tellp();
        LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf();
        sgKpf->setZeichenAlphabet( schrift->getAlphabetI( i ) );
        sgKpf->speichern( outF );
        Alphabet *alp = schrift->getAlphabetI( i );
        for( int i1 = 0; i1 < sgKpf->getZeichenAnzahl(); ++i1 )
        {
            sgKpf->getPositionen()[ i1 ] = (int)outF->tellp();
            LTDSBuchstabenKopf *zeichKpf = new LTDSBuchstabenKopf();
            Buchstabe *zeichen = alp->getBuchstabe( sgKpf->getZeichen()[ i1 ] );
            zeichKpf->init( sgKpf->getZeichen()[ i1 ], zeichen->getBreite(), zeichen->getHeight() );
            zeichKpf->speichern( outF );
            LTDSBuchstabenKörper *zeichKörp = new LTDSBuchstabenKörper( zeichKpf->getThis() );
            zeichKörp->setBuchstabe( zeichen->getThis() );
            zeichKörp->speichern( outF );
            zeichKörp->release();
            zeichen->release();
            zeichKpf->release();
        }
        alp->release();
        int p = (int)outF->tellp();
        outF->seekp( dateiKopf->getPositionList()[ i ], std::ios::beg );
        sgKpf->speichern( outF );
        outF->seekp( p, std::ios::beg );
        sgKpf->release();
    }
    outF->seekp( 0, std::ios::beg );
    dateiKopf->speichern( outF );
    outF->close();
    delete outF;
    schrift->release();
}

// constant 
Schrift *LTDSDatei::ladeSchrift() // gibt die geladene Schrift zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    Schrift *ret = new Schrift();
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
    inF->seekg( dateiKopf->getPositionList()[ 0 ], std::ios::beg );
    for( int i = 0; i < dateiKopf->getSchriftGrößeAnzahl(); ++i )
    {
        LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf();
        sgKpf->laden( inF );
        Alphabet *alphabet = new Alphabet();
        alphabet->setSchriftSize( sgKpf->getSchriftGröße() );
        alphabet->setDrawSchriftSize( sgKpf->getSchriftGröße() );
        for( int i1 = 0; i1 < sgKpf->getZeichenAnzahl(); ++i1 )
        {
            LTDSBuchstabenKopf *zeichKpf = new LTDSBuchstabenKopf();
            zeichKpf->laden( inF );
            LTDSBuchstabenKörper *zeichKörp = new LTDSBuchstabenKörper( zeichKpf->getThis() );
            zeichKörp->laden( inF );
            alphabet->setBuchstabe( zeichKpf->getZeichen(), zeichKörp->getBuchstabe() );
            zeichKörp->release();
            zeichKpf->release();
        }
        ret->addAlphabet( alphabet->getThis() );
        alphabet->release();
        sgKpf->release();
    }
    inF->close();
    delete inF;
    return ret;
}

Alphabet *LTDSDatei::ladeAlphabet( int schriftgröße ) // gibt eine geladene Schrift nur mit der angegebenen Schriftgröße zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    Alphabet *ret = 0;
    int sgNum = -1;
    for( int i = 0; i < dateiKopf->getSchriftGrößeAnzahl(); ++i )
    {
        if( dateiKopf->getSchriftGrößeList()[ i ] == schriftgröße )
        {
            sgNum = i;
            break;
        }
    }
    if( sgNum == -1 )
        return 0;
    ret = new Alphabet();
    ret->NeuAlphabet();
    ret->setSchriftSize( schriftgröße );
    ret->setDrawSchriftSize( schriftgröße );
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
    inF->seekg( dateiKopf->getPositionList()[ sgNum ], std::ios::beg );
    LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf();
    sgKpf->laden( inF );
    for( int i = 0; i < sgKpf->getZeichenAnzahl(); ++i )
    {
        LTDSBuchstabenKopf *sgZKpf = new LTDSBuchstabenKopf();
        sgZKpf->laden( inF );
        LTDSBuchstabenKörper *sgZKörp = new LTDSBuchstabenKörper( sgZKpf->getThis() );
        sgZKörp->laden( inF );
        ret->setBuchstabe( sgZKpf->getZeichen(), sgZKörp->getBuchstabe() );
        sgZKörp->release();
        sgZKpf->release();
    }
    sgKpf->release();
    inF->close();
    delete inF;
    return ret;
}

Buchstabe *LTDSDatei::ladeBuchstabe( int schriftgröße, unsigned char zeichen )// Läd einen bestimmten Buchstaben
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    Buchstabe *ret = 0;
    int sgNum = -1;
    for( int i = 0; i < dateiKopf->getSchriftGrößeAnzahl(); ++i )
    {
        if( dateiKopf->getSchriftGrößeList()[ i ] == schriftgröße )
        {
            sgNum = i;
            break;
        }
    }
    if( sgNum == -1 )
        return 0;
    std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
    inF->seekg( dateiKopf->getPositionList()[ sgNum ], std::ios::beg );
    LTDSSchriftKopf *sgKpf = new LTDSSchriftKopf();
    sgKpf->laden( inF );
    int sgZNum = -1;
    for( int i = 0; i < sgKpf->getZeichenAnzahl(); ++i )
    {
        if( sgKpf->getZeichen()[ i ] == zeichen )
        {
            sgZNum = i;
            break;
        }
    }
    if( sgZNum != -1 )
    {
        inF->seekg( sgKpf->getPositionen()[ sgZNum ], std::ios::beg );
        LTDSBuchstabenKopf *sgZKpf = new LTDSBuchstabenKopf();
        sgZKpf->laden( inF );
        LTDSBuchstabenKörper *sgZKörp = new LTDSBuchstabenKörper( sgZKpf->getThis() );
        sgZKörp->laden( inF );
        ret = sgZKörp->getBuchstabe();
        sgZKörp->release();
        sgZKpf->release();
    }
    sgKpf->release();
    inF->close();
    delete inF;
    if( ret )
    {
        ret->setSchriftSize( schriftgröße );
        ret->setDrawSchriftSize( schriftgröße );
    }
    return ret;
}

Text *LTDSDatei::getPfad() const // gibt den Dateipfad zurück
{
    return pfad->getThis();
}

int LTDSDatei::getAnzahlSchriftgrößen() const // gibt die Anzahl der Schriftgrößen aus der Datei zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    return dateiKopf->getSchriftGrößeAnzahl();
}

unsigned char *LTDSDatei::getSchriftGrößen() const // gibt einen Array von Schriftgrößen zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    return dateiKopf->getSchriftGrößeList();
}

unsigned char LTDSDatei::getAnzahlBuchstaben( int sg ) // gibt die anzahl gespeicherter Buchstaben einer Schriftgröße zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    int ret = 0;
    unsigned char *größen = dateiKopf->getSchriftGrößeList();
    unsigned char granzahl = dateiKopf->getSchriftGrößeAnzahl();
    int grpos = -1;
    for( int i = 0; i < granzahl; ++i )
    {
        if( größen[ i ] == sg )
        {
            grpos = i;
            break;
        }
    }
    if( grpos != -1 )
    {
        int *grposlist = dateiKopf->getPositionList();
        LTDSSchriftKopf *sgkpf = new LTDSSchriftKopf();
        std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
        inF->seekg( grposlist[ grpos ], std::ios::beg );
        sgkpf->laden( inF );
        ret = sgkpf->getZeichenAnzahl();
        sgkpf->release();
        inF->close();
        delete inF;
    }
    return ret;
}

unsigned char *LTDSDatei::getBuchstaben( int sg ) // gibt einen Array von Buchstaben einer Schriftgröße zurück
{
    if( !DateiExistiert( pfad->getThis() ) )
        return 0;
    if( !dateiKopf )
        return 0;
    unsigned char *ret = 0;
    unsigned char *größen = dateiKopf->getSchriftGrößeList();
    unsigned char granzahl = dateiKopf->getSchriftGrößeAnzahl();
    int grpos = -1;
    for( int i = 0; i < granzahl; ++i )
    {
        if( größen[ i ] == sg )
        {
            grpos = i;
            break;
        }
    }
    if( grpos != -1 )
    {
        int *grposlist = dateiKopf->getPositionList();
        LTDSSchriftKopf *sgkpf = new LTDSSchriftKopf();
        std::ifstream *inF = new std::ifstream( pfad->getText(), std::ios::binary );
        inF->seekg( grposlist[ grpos ], std::ios::beg );
        sgkpf->laden( inF );
        int anz = sgkpf->getZeichenAnzahl();
        ret = new unsigned char[ anz ];
        for( int i = 0; i < anz; ++i )
            ret[ i ] = sgkpf->getZeichen()[ i ];
        sgkpf->release();
        inF->close();
        delete inF;
    }
    return ret;
}

// Reference Counting 
LTDSDatei *LTDSDatei::getThis()
{
    ++ref;
    return this;
}

LTDSDatei *LTDSDatei::release()
{
    --ref;
    if( ref == 0 )
        delete this;
    return 0;
}
#endif
// Bit Funktionen
int Framework::Bits( int a ) // gibt 1-bits in gewinschter anzahl zurück
{
    int ret = 0;
    for( int i = 0; i < a; ++i )
    {
        ret <<= 1;
        ++ret;
    }
    return ret;
}

int Framework::getBits( char c ) // gibt zurück, wie viele Bits c benötigt
{
    int ret = 0;
    for( int i = 0; ( c & (char)Bits( i ) ) != c; ++i )
        ++ret;
    return ret;
}