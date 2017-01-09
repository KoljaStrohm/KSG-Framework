#include "Zeit.h"
#include "Text.h"
#include <time.h>
#include <ctime>
#ifdef WIN32
#include "Fenster.h"
#else
#include <string.h>
#include <sys/time.h>
#endif

#ifdef WIN32

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timeval
{
    long tv_sec;        // Sekunden seit dem 1.1.1970
    long tv_usec;       // und Mikrosekunden
};

struct timezone
{
    int tz_minuteswest; // minutes W of Greenwich
    int tz_dsttime;     // type of dst correction
};

int gettimeofday( struct timeval *tv, struct timezone *tz )
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if( NULL != tv )
    {
        GetSystemTimeAsFileTime( &ft );

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        //converting file time to unix epoch
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tmpres /= 10;  //convert into microseconds
        tv->tv_sec = (long)( tmpres / 1000000UL );
        tv->tv_usec = (long)( tmpres % 1000000UL );
    }

    if( NULL != tz )
    {
        if( !tzflag )
        {
            _tzset();
            ++tzflag;
        }
        _get_timezone( ( long* )&( tz->tz_minuteswest ) );
        tz->tz_minuteswest /= 60;
        _get_daylight( &( tz->tz_dsttime ) );
    }

    return 0;
}

#endif


using namespace Framework;

// Inhalt der Uhrzeit Klasse aus Zeit.h
// Privat 
int Uhrzeit::update() // berechnet die neue Zeit
{
    int ret = 0;
    while( sekunde >= 60 )
    {
        sekunde -= 60;
        ++minute;
    }
    while( minute >= 60 )
    {
        minute -= 60;
        ++stunde;
    }
    while( stunde >= 24 )
    {
        stunde -= 24;
        ++ret;
    }
    while( sekunde < 0 )
    {
        sekunde += 60;
        --minute;
    }
    while( minute < 0 )
    {
        minute += 60;
        --stunde;
    }
    while( stunde < 0 )
    {
        stunde += 24;
        --ret;
    }
    return ret;
}

Uhrzeit::Uhrzeit()
    : stunde( 0 ),
    minute( 0 ),
    sekunde( 0 ),
    ref( 1 )
{}

int Uhrzeit::setUhrzeit( Uhrzeit *zeit ) // setzt die Uhrzeit
{
    stunde = zeit->getStunde();
    minute = zeit->getMinute();
    sekunde = zeit->getSekunde();
    zeit->release();
    return update();
}

int Uhrzeit::setUhrzeit( int stunde, int minute, int sekunde )
{
    this->stunde = stunde;
    this->minute = minute;
    this->sekunde = sekunde;
    return update();
}

int Uhrzeit::setUhrzeit( const char *format, const char *zeit ) // format Beispiele: "H:i:s", "H-i-s" (H=stunde,i=minute,s=sekunde)
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            stunde = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            minute = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sekunde = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return update();
}

int Uhrzeit::setUhrzeit( const char *format, Text *zeit )
{
    int ret = setUhrzeit( format, zeit->getText() );
    zeit->release();
    return ret;
}

int Uhrzeit::setStunde( int stunde ) // setzt die Stunde
{
    this->stunde = stunde;
    return update();
}

int Uhrzeit::setMinute( int minute ) // setzt die Minute
{
    this->minute = minute;
    return update();
}

int Uhrzeit::setSekunde( int sekunde ) // setzt die Sekunde
{
    this->sekunde = sekunde;
    return update();
}

int Uhrzeit::plusUhrzeit( Uhrzeit *zeit ) // addiert die zeiten
{
    stunde += zeit->getStunde();
    minute += zeit->getMinute();
    sekunde += zeit->getSekunde();
    return update();
}

int Uhrzeit::plusUhrzeit( int stunde, int minute, int sekunde )
{
    this->stunde += stunde;
    this->minute += minute;
    this->sekunde += sekunde;
    return update();
}

int Uhrzeit::plusUhrzeit( const char *format, const char *zeit )
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            stunde += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            minute += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sekunde += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return update();
}

int Uhrzeit::plusUhrzeit( const char *format, Text *zeit )
{
    int ret = plusUhrzeit( format, zeit->getText() );
    zeit->release();
    return ret;
}

int Uhrzeit::plusStunde( int stunde ) // addiert stunde Stunden
{
    this->stunde += stunde;
    return update();
}

int Uhrzeit::plusMinute( int minute ) // addiert minute Minuten
{
    this->minute += minute;
    return update();
}

int Uhrzeit::plusSekunde( int sekunde ) // addiert sekunde Sekunden
{
    this->sekunde += sekunde;
    return update();
}

int Uhrzeit::minusUhrzeit( Uhrzeit *zeit ) // subtrahiert die zeiten
{
    stunde -= zeit->getStunde();
    minute -= zeit->getMinute();
    sekunde -= zeit->getSekunde();
    zeit->release();
    return update();
}

int Uhrzeit::minusUhrzeit( int stunde, int minute, int sekunde )
{
    this->stunde -= stunde;
    this->minute -= minute;
    this->sekunde -= sekunde;
    return update();
}

int Uhrzeit::minusUhrzeit( const char *format, const char *zeit )
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            stunde -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            minute -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sekunde -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return update();
}

int Uhrzeit::minusUhrzeit( const char *format, Text *zeit )
{
    int ret = plusUhrzeit( format, zeit->getText() );
    zeit->release();
    return ret;
}

int Uhrzeit::minusStunde( int stunde ) // subtrahiert stunde Stunden
{
    this->stunde -= stunde;
    return update();
}

int Uhrzeit::minusMinute( int minute ) // subtrahiert minute Minuten
{
    this->minute -= minute;
    return update();
}

int Uhrzeit::minusSekunde( int sekunde ) // subtrahiert sekunde Sekunden
{
    this->sekunde -= sekunde;
    return update();
}

// constant 
int Uhrzeit::getStunde() const // gibt die Stunde zurück
{
    return stunde;
}

int Uhrzeit::getMinute() const // gibt die Minute zurück
{
    return minute;
}

int Uhrzeit::getSekunde() const // gibt die Sekunde zurück
{
    return sekunde;
}

Text *Uhrzeit::getUhrzeit( const char *format ) const // gibt die Uhrzeit als Text formatiert zurück
{
    Text *ret = new Text( "" );
    int flen = textLength( format );
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            ret->append( stunde );
            break;
        case 'i':
            ret->append( minute );
            break;
        case 's':
            ret->append( sekunde );
            break;
        default:
            ret->append( f, 1 );
            break;
        }
    }
    return ret;
}

bool Uhrzeit::istGleich( Uhrzeit *zeit ) const // prüft, ob die Uhrzeit gleich zeit ist
{
    bool ret = stunde == zeit->getStunde() &&
        minute == zeit->getMinute() &&
        sekunde == zeit->getSekunde();
    zeit->release();
    return ret;
}

bool Uhrzeit::istGleich( const char *format, const char *zeit ) const
{
    int st = stunde, min = minute, sek = sekunde;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            st = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            min = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sek = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    bool ret = stunde == st &&
        minute == min &&
        sekunde == sek;
    return ret;
}

bool Uhrzeit::istGleich( const char *format, Text *zeit ) const
{
    bool ret = istGleich( format, zeit->getText() );
    zeit->release();
    return ret;
}

bool Uhrzeit::istGleich( int stunde, int minute, int sekunde ) const
{
    return this->stunde == stunde &&
        this->minute == minute &&
        this->sekunde == sekunde;
}

bool Uhrzeit::stundeGleich( int stunde ) const // prüft, ob die Stunde gleich stunde ist
{
    return this->stunde == stunde;
}

bool Uhrzeit::minuteGleich( int minute ) const // prüft, ob die Minute gleich minute ist
{
    return this->minute == minute;
}

bool Uhrzeit::sekundeGleich( int sekunde ) const // prüft, ob die Sekunde gleich sekunde ist
{
    return this->sekunde == sekunde;
}

bool Uhrzeit::istKleiner( Uhrzeit *zeit ) const // prüft, ob die Zeit kleiner als zeit ist
{
    bool ret = istKleiner( zeit->getStunde(), zeit->getMinute(), zeit->getSekunde() );
    zeit->release();
    return ret;
}

bool Uhrzeit::istKleiner( int stunde, int minute, int sekunde ) const
{
    if( this->stunde < stunde )
        return 1;
    else if( this->stunde == stunde )
    {
        if( this->minute < minute )
            return 1;
        else if( this->minute == minute )
        {
            if( this->sekunde < sekunde )
                return 1;
            else if( this->sekunde == sekunde )
                return 0;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

bool Uhrzeit::istKleiner( const char *format, const char *zeit ) const
{
    int st = stunde, min = minute, sek = sekunde;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            st = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            min = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sek = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return istKleiner( st, min, sek );
}

bool Uhrzeit::istKleiner( const char *format, Text *zeit ) const
{
    bool ret = istKleiner( format, zeit->getText() );
    zeit->release();
    return ret;
}

bool Uhrzeit::istLater( Uhrzeit *zeit ) const // prüft, ob die Zeit größer als zeit ist
{
    bool ret = istLater( zeit->getStunde(), zeit->getMinute(), zeit->getSekunde() );
    zeit->release();
    return ret;
}

bool Uhrzeit::istLater( int stunde, int minute, int sekunde ) const
{
    if( this->stunde > stunde )
        return 1;
    else if( this->stunde == stunde )
    {
        if( this->minute > minute )
            return 1;
        else if( this->minute == minute )
        {
            if( this->sekunde > sekunde )
                return 1;
            else if( this->sekunde == sekunde )
                return 0;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

bool Uhrzeit::istLater( const char *format, const char *zeit ) const
{
    int st = stunde, min = minute, sek = sekunde;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'h':
            st = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            min = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            sek = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return istLater( st, min, sek );
}

bool Uhrzeit::istLater( const char *format, Text *zeit ) const
{
    bool ret = istLater( format, zeit->getText() );
    zeit->release();
    return ret;
}

// Reference Counting 
Uhrzeit *Uhrzeit::getThis()
{
    ++ref;
    return this;
}

Uhrzeit *Uhrzeit::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der Datum Klasse aus Zeit.h
// Privat 
void Datum::update() // berechnet die neue Zeit
{
    while( monat > 12 )
    {
        monat -= 12;
        ++jahr;
    }
    while( monat <= 0 )
    {
        monat += 12;
        --jahr;
    }
    if( istSchaltjahr( jahr ) )
        maxTage[ 1 ] = 29;
    else
        maxTage[ 1 ] = 28;
    while( tag > maxTage[ monat - 1 ] )
    {
        tag -= maxTage[ monat - 1 ];
        ++monat;
        if( monat > 12 )
        {
            monat -= 12;
            ++jahr;
            if( istSchaltjahr( jahr ) )
                maxTage[ 1 ] = 29;
            else
                maxTage[ 1 ] = 28;
        }
    }
    while( tag <= 0 )
    {
        tag += maxTage[ ( monat - 2 > 0 ? monat - 2 : 11 ) ];
        --monat;
        if( monat <= 0 )
        {
            monat += 12;
            --jahr;
            if( istSchaltjahr( jahr ) )
                maxTage[ 1 ] = 29;
            else
                maxTage[ 1 ] = 28;
        }
    }
}

// Konstruktor 
Datum::Datum()
    : jahr( 0 ),
    monat( 0 ),
    tag( 0 ),
    ref( 1 )
{
    int maxT[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    maxTage = new int[ 12 ];
    memcpy( maxTage, maxT, 12 * sizeof( int ) );
}

// Destruktor 
Datum::~Datum()
{
    delete[] maxTage;
}

// nicht constant 
void Datum::setDatum( Datum *datum ) // setzt das Datum
{
    jahr = datum->getJahr();
    monat = datum->getMonat();
    tag = datum->getTag();
    datum->release();
    update();
}

void Datum::setDatum( int jahr, int monat, int tag )
{
    this->jahr = jahr;
    this->monat = monat;
    this->tag = tag;
    update();
}

void Datum::setDatum( const char *format, const char *datum ) // format Beispiele: "Y:m:d", "Y-m-d" (Y=Jahr,m=Monat,d=tag)
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            jahr = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            monat = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            tag = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    update();
}

void Datum::setDatum( const char *format, Text *datum )
{
    setDatum( format, datum->getText() );
    datum->release();
}

void Datum::setJahr( int jahr ) // setzt das Jahr
{
    this->jahr = jahr;
    update();
}

void Datum::setMonat( int monat ) // setzt den Monat
{
    this->monat = monat;
    update();
}

void Datum::setTag( int tag ) // setzt den Tag
{
    this->tag = tag;
    update();
}

void Datum::plusDatum( Datum *datum ) // addiert das datum
{
    this->jahr += datum->getJahr();
    this->monat += datum->getMonat();
    this->tag += datum->getTag();
    datum->release();
    update();
}

void Datum::plusDatum( int jahr, int monat, int tag )
{
    this->jahr += jahr;
    this->monat += monat;
    this->tag += tag;
    update();
}

void Datum::plusDatum( const char *format, const char *datum )
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            jahr += TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            monat += TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            tag += TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    update();
}

void Datum::plusDatum( const char *format, Text *datum )
{
    plusDatum( format, datum->getText() );
    datum->release();
}

void Datum::plusJahr( int jahr ) // addiert jahr Jahre
{
    this->jahr += jahr;
    update();
}

void Datum::plusMonat( int monat ) // addiert monat Monate
{
    this->monat = monat;
    update();
}

void Datum::plusTag( int tag ) // addiert tag Tage
{
    this->tag += tag;
    update();
}

void Datum::minusDatum( Datum *datum ) // subtrahiert das datum
{
    jahr -= datum->getJahr();
    monat -= datum->getMonat();
    tag -= datum->getTag();
    datum->release();
    update();
}

void Datum::minusDatum( int jahr, int monat, int tag )
{
    this->jahr -= jahr;
    this->monat -= monat;
    this->tag -= tag;
    update();
}

void Datum::minusDatum( const char *format, const char *datum )
{
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            jahr -= TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            monat -= TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            tag -= TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    update();
}

void Datum::minusDatum( const char *format, Text *datum )
{
    minusDatum( format, datum->getText() );
    datum->release();
}

void Datum::minusJahr( int jahr ) // subtrahiert jahr Jahre
{
    this->jahr -= jahr;
    update();
}

void Datum::minusMonat( int monat ) // subtrahiert monat Monate
{
    this->monat -= monat;
    update();
}

void Datum::minusTag( int tag ) // subtrahiert tag Tage
{
    this->tag -= tag;
    update();
}

// constant 
int Datum::getJahr() const // gibt das Jahr zurück
{
    return jahr;
}

int Datum::getMonat() const // gibt der Monat zurück
{
    return monat;
}

int Datum::getTag() const // gibt der Tag zurück
{
    return tag;
}

Text *Datum::getDatum( const char *format ) const // gibt das Datum als Text formatiert zurück
{
    Text *ret = new Text( "" );
    int flen = textLength( format );
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            ret->append( jahr );
            break;
        case 'm':
            ret->append( monat );
            break;
        case 'd':
            ret->append( tag );
            break;
        default:
            ret->append( f, 1 );
            break;
        }
    }
    return ret;
}

bool Datum::istGleich( Datum *datum ) const // prüft, ob das Datum gleich datum ist
{
    bool ret = jahr == datum->getJahr() &&
        monat == datum->getMonat() &&
        tag == datum->getTag();
    datum->release();
    return ret;
}

bool Datum::istGleich( const char *format, const char *datum ) const
{
    int j = jahr, m = monat, t = tag;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            j = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            t = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    bool ret = jahr == j &&
        monat == m &&
        tag == t;
    return ret;
}

bool Datum::istGleich( const char *format, Text *datum ) const
{
    bool ret = istGleich( format, datum->getText() );
    datum->release();
    return ret;
}

bool Datum::istGleich( int jahr, int monat, int tag ) const
{
    return this->jahr == jahr &&
        this->monat == monat &&
        this->tag == tag;
}

bool Datum::jahrGleich( int jahr ) const // prüft, ob das Jahr gleich jahr ist
{
    return this->jahr == jahr;
}

bool Datum::monatGleich( int monat ) const // prüft, ob der Monat gleich monat ist
{
    return this->monat == monat;
}

bool Datum::tagGleich( int tag ) const // prüft, ob der Tag gleich tag ist
{
    return this->tag == tag;
}

bool Datum::istKleiner( Datum *datum ) const // prüft, ob die Datum kleiner als datum ist
{
    bool ret = istKleiner( datum->getJahr(), datum->getMonat(), datum->getTag() );
    datum->release();
    return ret;
}

bool Datum::istKleiner( int jahr, int monat, int tag ) const
{
    if( this->jahr < jahr )
        return 1;
    else if( this->jahr == jahr )
    {
        if( this->monat < monat )
            return 1;
        else if( this->monat == monat )
        {
            if( this->tag < tag )
                return 1;
            else if( this->tag == tag )
                return 0;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

bool Datum::istKleiner( const char *format, const char *datum ) const
{
    int j = jahr, m = monat, t = tag;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            j = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            t = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    return istKleiner( j, m, t );
}

bool Datum::istKleiner( const char *format, Text *datum ) const
{
    bool ret = istKleiner( format, datum->getText() );
    datum->release();
    return ret;
}

bool Datum::istLater( Datum *datum ) const // prüft, ob die Datum größer als datum ist
{
    bool ret = istLater( datum->getJahr(), datum->getMonat(), datum->getTag() );
    datum->release();
    return ret;
}

bool Datum::istLater( int jahr, int monat, int tag ) const
{
    if( this->jahr > jahr )
        return 1;
    else if( this->jahr == jahr )
    {
        if( this->monat > monat )
            return 1;
        else if( this->monat == monat )
        {
            if( this->tag > tag )
                return 1;
            else if( this->tag == tag )
                return 0;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}

bool Datum::istLater( const char *format, const char *datum ) const
{
    int j = jahr, m = monat, t = tag;
    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            j = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        case 'd':
            t = TextZuInt( (char*)datum, &ende, 10 );
            datum = ende;
            ende = 0;
            break;
        default:
            ++datum;
            break;
        }
    }
    return istLater( j, m, t );
}

bool Datum::istLater( const char *format, Text *datum ) const
{
    bool ret = istLater( format, datum->getText() );
    datum->release();
    return ret;
}

// Reference Counting 
Datum *Datum::getThis()
{
    ++ref;
    return this;
}

Datum *Datum::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der Zeit Klasse aus Zeit.h
// Konstruktor 
Zeit::Zeit()
    : datum( new Datum() ),
    uhrzeit( new Uhrzeit() ),
    ref( 1 )
{}

// Destruktor 
Zeit::~Zeit()
{
    datum->release();
    uhrzeit->release();
}

// nicht constant 
void Zeit::setZeit( Zeit *zeit ) // setzt die Zeit
{
    datum->setDatum( zeit->getDatum() );
    datum->plusTag( uhrzeit->setUhrzeit( zeit->getUhrzeit() ) );
    zeit->release();
}

void Zeit::setZeit( int jahr, int monat, int tag, int stunde, int minute, int sekunde )
{
    datum->setDatum( jahr, monat, tag );
    datum->plusTag( uhrzeit->setUhrzeit( stunde, minute, sekunde ) );
}

void Zeit::setZeit( const char *format, const char *zeit ) // format Beispiele: "Y:m:d H-i-s", "Y-m-d H:i:s" (Y=Jahr,m=Monat,d=tag,H=stunde,i=minute,s=sekunde)
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    datum->setDatum( y, m, d );
    datum->plusTag( uhrzeit->setUhrzeit( h, i, s ) );
}

void Zeit::setZeit( const char *format, Text *zeit )
{
    setZeit( format, zeit->getText() );
    zeit->release();
}

void Zeit::setJahr( int jahr ) // setzt das Jahr
{
    datum->setJahr( jahr );
}

void Zeit::setMonat( int monat ) // setzt den Monat
{
    datum->setMonat( monat );
}

void Zeit::setTag( int tag ) // setzt den Tag
{
    datum->setTag( tag );
}

void Zeit::setStunde( int stunde ) // setzt die Stunde
{
    datum->plusTag( uhrzeit->setStunde( stunde ) );
}

void Zeit::setMinute( int minute ) // setzt die Minute
{
    datum->plusTag( uhrzeit->setMinute( minute ) );
}

void Zeit::setSekunde( int sekunde ) // setzt die Sekunde
{
    datum->plusTag( uhrzeit->setSekunde( sekunde ) );
}

void Zeit::plusZeit( Zeit *zeit ) // addiert die zeit
{
    datum->plusDatum( zeit->getDatum() );
    datum->plusTag( uhrzeit->plusUhrzeit( zeit->getUhrzeit() ) );
    zeit->release();
}

void Zeit::plusZeit( int jahr, int monat, int tag, int stunde, int minute, int sekunde )
{
    datum->plusDatum( jahr, monat, tag );
    datum->plusTag( uhrzeit->plusUhrzeit( stunde, minute, sekunde ) );
}

void Zeit::plusZeit( const char *format, const char *zeit )
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    datum->setDatum( y, m, d );
    datum->plusTag( uhrzeit->setUhrzeit( h, i, s ) );
}

void Zeit::plusZeit( const char *format, Text *zeit )
{
    plusZeit( format, zeit->getText() );
    zeit->release();
}

void Zeit::plusJahr( int jahr ) // addiert jahr Jahre
{
    datum->plusJahr( jahr );
}

void Zeit::plusMonat( int monat ) // addiert monat Monate
{
    datum->plusMonat( monat );
}

void Zeit::plusTag( int tag ) // addiert tag Tage
{
    datum->plusTag( tag );
}

void Zeit::plusStunde( int stunde ) // addiert stunde Stunden
{
    datum->plusTag( uhrzeit->plusStunde( stunde ) );
}

void Zeit::plusMinute( int minute ) // addiert minute Minuten
{
    datum->plusTag( uhrzeit->plusMinute( minute ) );
}

void Zeit::plusSekunde( int sekunde ) // addiert sekunde Sekunden
{
    datum->plusTag( uhrzeit->plusSekunde( sekunde ) );
}

void Zeit::minusZeit( Zeit *zeit ) // subtrahiert die zeit
{
    datum->minusDatum( zeit->getDatum() );
    datum->plusTag( uhrzeit->minusUhrzeit( zeit->getUhrzeit() ) );
    zeit->release();
}

void Zeit::minusZeit( int jahr, int monat, int tag, int stunde, int minute, int sekunde )
{
    datum->minusDatum( jahr, monat, tag );
    datum->plusTag( uhrzeit->plusUhrzeit( stunde, minute, sekunde ) );
}

void Zeit::minusZeit( const char *format, const char *zeit )
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h += TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s -= TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    datum->setDatum( y, m, d );
    datum->plusTag( uhrzeit->setUhrzeit( h, i, s ) );
}

void Zeit::minusZeit( const char *format, Text *zeit )
{
    minusZeit( format, zeit->getText() );
    zeit->release();
}

void Zeit::minusJahr( int jahr ) // subtrahiert jahr Jahre
{
    datum->minusJahr( jahr );
}

void Zeit::minusMonat( int monat ) // subtrahiert monat Monate
{
    datum->minusMonat( monat );
}

void Zeit::minusTag( int tag ) // subtrahiert tag Tage
{
    datum->minusTag( tag );
}

void Zeit::minusStunde( int stunde ) // subtrahiert stunde Stunden
{
    datum->plusTag( uhrzeit->minusStunde( stunde ) );
}

void Zeit::minusMinute( int minute ) // subtrahiert minute Minuten
{
    datum->plusTag( uhrzeit->minusMinute( minute ) );
}

void Zeit::minusSekunde( int sekunde ) // subtrahiert sekunde Sekunden
{
    datum->plusTag( uhrzeit->minusSekunde( sekunde ) );
}

// constant 
Text *Zeit::getZeit( const char *format ) const // gibt die Zeit als Text formatiert zurück
{
    Text *ret = new Text( "" );
    int flen = textLength( format );
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            ret->append( datum->getJahr() );
            break;
        case 'm':
            ret->append( datum->getMonat() );
            break;
        case 'd':
            ret->append( datum->getTag() );
            break;
        case 'h':
            ret->append( uhrzeit->getStunde() );
            break;
        case 'i':
            ret->append( uhrzeit->getMinute() );
            break;
        case 's':
            ret->append( uhrzeit->getSekunde() );
            break;
        default:
            ret->append( f, 1 );
        }
    }
    return ret;
}

bool Zeit::istGleich( Zeit *zeit ) const // prüft, ob die Uhrzeit gleich zeit ist
{
    bool ret = datum->istGleich( zeit->getDatum() ) && uhrzeit->istGleich( zeit->getUhrzeit() );
    zeit->release();
    return ret;
}

bool Zeit::istGleich( const char *format, const char *zeit ) const
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    return datum->istGleich( y, m, d ) && uhrzeit->istGleich( h, i, s );
}

bool Zeit::istGleich( const char *format, Text *zeit ) const
{
    bool ret = istGleich( format, zeit->getText() );
    zeit->release();
    return ret;
}

bool Zeit::istGleich( int jahr, int monat, int tag, int stunde, int minute, int sekunde ) const
{
    return datum->istGleich( jahr, monat, tag ) && uhrzeit->istGleich( stunde, minute, sekunde );
}

Datum *Zeit::getDatum() const // gibt das Datum zurück
{
    return datum->getThis();
}

Datum *Zeit::zDatum() const
{
    return datum;
}

Uhrzeit *Zeit::getUhrzeit() const // gibt die Uhrzeit zurück
{
    return uhrzeit->getThis();
}

Uhrzeit *Zeit::zUhrzeit() const
{
    return uhrzeit;
}

bool Zeit::istKleiner( Zeit *zeit ) const // prüft, ob die Zeit kleiner als zeit ist
{
    if( datum->istKleiner( zeit->getDatum() ) )
    {
        zeit->release();
        return 1;
    }
    else if( datum->istGleich( zeit->getDatum() ) )
    {
        if( uhrzeit->istKleiner( zeit->getUhrzeit() ) )
        {
            zeit->release();
            return 1;
        }
        else
        {
            zeit->release();
            return 0;
        }
    }
    else
    {
        zeit->release();
        return 0;
    }
}

bool Zeit::istKleiner( int jahr, int monat, int tag, int stunde, int minute, int sekunde ) const
{
    if( datum->istKleiner( jahr, monat, tag ) )
        return 1;
    else if( datum->istGleich( jahr, monat, tag ) )
    {
        if( uhrzeit->istKleiner( stunde, minute, sekunde ) )
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

bool Zeit::istKleiner( const char *format, const char *zeit ) const
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    if( datum->istKleiner( y, m, d ) )
        return 1;
    else if( datum->istGleich( y, m, d ) )
    {
        if( uhrzeit->istKleiner( h, i, s ) )
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

bool Zeit::istKleiner( const char *format, Text *zeit ) const
{
    bool ret = istKleiner( format, zeit->getText() );
    zeit->release();
    return ret;
}

bool Zeit::istLater( Zeit *zeit ) const // prüft, ob die Zeit größer als zeit ist
{
    if( datum->istLater( zeit->getDatum() ) )
    {
        zeit->release();
        return 1;
    }
    else if( datum->istGleich( zeit->getDatum() ) )
    {
        if( uhrzeit->istLater( zeit->getUhrzeit() ) )
        {
            zeit->release();
            return 1;
        }
        else
        {
            zeit->release();
            return 0;
        }
    }
    else
    {
        zeit->release();
        return 0;
    }
}

bool Zeit::istLater( int jahr, int monat, int tag, int stunde, int minute, int sekunde ) const
{
    if( datum->istLater( jahr, monat, tag ) )
        return 1;
    else if( datum->istGleich( jahr, monat, tag ) )
    {
        if( uhrzeit->istLater( stunde, minute, sekunde ) )
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

bool Zeit::istLater( const char *format, const char *zeit ) const
{
    int y = datum->getJahr();
    int m = datum->getMonat();
    int d = datum->getTag();
    int h = uhrzeit->getStunde();
    int i = uhrzeit->getMinute();
    int s = uhrzeit->getSekunde();

    int flen = textLength( format );
    char *ende = 0;
    for( const char *f = format; f < format + flen; ++f )
    {
        switch( *f )
        {
        case 'y':
            y = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'm':
            m = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'd':
            d = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'h':
            h = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 'i':
            i = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        case 's':
            s = TextZuInt( (char*)zeit, &ende, 10 );
            zeit = ende;
            ende = 0;
            break;
        default:
            ++zeit;
            break;
        }
    }
    if( datum->istLater( y, m, d ) )
        return 1;
    else if( datum->istGleich( y, m, d ) )
    {
        if( uhrzeit->istLater( h, i, s ) )
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

bool Zeit::istLater( const char *format, Text *zeit ) const
{
    bool ret = istLater( format, zeit->getText() );
    zeit->release();
    return ret;
}

// Reference Counting 
Zeit *Zeit::getThis()
{
    ++ref;
    return this;
}

Zeit *Zeit::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Inhalt der ZeitMesser Klasse aus Zeit.h
// Konstruktor 
ZeitMesser::ZeitMesser()
    : start( 0 ),
    ende( 0 ),
    messung( 0 ),
    ref( 1 )
{}

// Destruktor 
ZeitMesser::~ZeitMesser()
{

}

// nicht constant 
void ZeitMesser::messungStart() // legt des Startpunkt der Zeitmessung fest
{
    timeval tv;
    gettimeofday( &tv, 0 );
    start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

void ZeitMesser::messungEnde() // legt des Endpunkt der Zeitmessung fest
{
    timeval tv;
    gettimeofday( &tv, 0 );
    ende = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    messung = ende - start;
}

// constant 
double ZeitMesser::getSekunden() const // gibt den sekundenabstand zwischen start und ende der Messung zurück
{
    return messung;
}

double ZeitMesser::getMinuten() const // gibt den minutenabstand zwischen start und ende der Messung zurück
{
    return messung / 60;
}

double ZeitMesser::getStunden() const // gibt den stundenabstand zwischen start und ende der Messung zurück
{
    return messung / 3600;
}

// Reference Counting 
ZeitMesser *ZeitMesser::getThis()
{
    ++ref;
    return this;
}

ZeitMesser *ZeitMesser::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}

// Globale Funktionen 
Uhrzeit *Framework::getUhrzeit() // gibt die aktuelle Uhrzeit zurück
{
#ifdef WIN32
    Uhrzeit *ret = new Uhrzeit();
    time_t Zeitstempel = time( 0 );
    tm now;
    localtime_s( &now, &Zeitstempel );
    ret->setUhrzeit( now.tm_hour, now.tm_min, now.tm_sec );
    return ret;
#else
    Uhrzeit *ret = new Uhrzeit();
    time_t Zeitstempel = time( 0 );
    tm *now = localtime( &Zeitstempel );
    ret->setUhrzeit( now->tm_hour, now->tm_min, now->tm_sec );
    return ret;
#endif
}

Datum *Framework::getDatum() // gibt das aktuelle Datum zurück
{
#ifdef WIN32
    Datum *ret = new Datum();
    time_t Zeitstempel = time( 0 );
    tm now;
    localtime_s( &now, &Zeitstempel );
    ret->setDatum( now.tm_year + 1900, now.tm_mon + 1, now.tm_mday );
    return ret;
#else
    Datum *ret = new Datum();
    time_t Zeitstempel = time( 0 );
    tm *now = localtime( &Zeitstempel );
    ret->setDatum( now->tm_year + 1900, now->tm_mon + 1, now->tm_mday );
    return ret;
#endif
}

Zeit *Framework::getZeit() // gibt die aktuelle Zeit( Datum und Uhrzeit ) zurück
{
#ifdef WIN32
    Zeit *ret = new Zeit();
    time_t Zeitstempel = time( 0 );
    tm now;
    localtime_s( &now, &Zeitstempel );
    ret->setZeit( now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec );
    return ret;
#else
    Zeit *ret = new Zeit();
    time_t Zeitstempel = time( 0 );
    tm *now = localtime( &Zeitstempel );
    ret->setZeit( now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec );
    return ret;
#endif
}

bool Framework::istSchaltjahr( int jahr ) // prüft, ob jahr ein Schaltjahr ist
{
    if( jahr % 4 == 0 )
    {
        if( jahr % 100 == 0 )
        {
            if( jahr % 400 == 0 )
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}