#include "Model2D.h"
#include "Bild.h"
#include "FrameworkMath.h"
#include "Mat3.h"
#include "MausEreignis.h"

using namespace Framework;

// Inhalt der Model2DData Klasse aus Model2D.h
// Konstruktor
Model2DData::Model2DData()
    : polygons( 0 ),
    vListen( 0 ),
    minP( 0, 0 ),
    maxP( 0, 0 )
{
    ref = 1;
}

// Destruktor
Model2DData::~Model2DData()
{
    if( polygons )
    {
        int anz = polygons->getEintragAnzahl();
        for( int i = 0; i < anz; i++ )
        {
            if( polygons->get( i ).tKordinaten )
                polygons->get( i ).tKordinaten->release();
            if( polygons->get( i ).vertex )
                polygons->get( i ).vertex->release();
        }
        polygons = polygons->release();
    }
    if( vListen )
        vListen->release();
}

// privat
bool Model2DData::istPunktInnen( Vertex p, int polygonId ) const
{
    if( p < minP || p > maxP || !polygons )
        return 0;
    int num = 0;
    for( auto *polygon = &polygons->getArray(); polygon && polygon->set; polygon = polygon->next, num++ )
    {
        if( polygonId >= 0 && num != polygonId )
            continue;
        int anz = polygon->var.vertex->getEintragAnzahl();
        int ola = outList.z( num )->getEintragAnzahl();
        bool c = 0;
        int j = anz - 1;
        for( int k = 0; k < ola; k++ )
        {
            Punkt out = outList.z( num )->get( k );
            if( out.x < out.y && j > out.x && j < out.y )
                j = out.x;
            if( out.x > out.y && ( j > out.x || j < out.y ) )
                j = out.x;
        }
        for( int i = 0; i < anz; i++ )
        {
            bool cont = 0;
            for( int k = 0; k < ola; k++ )
            {
                Punkt out = outList.z( num )->get( k );
                if( out.x < out.y && i > out.x && i < out.y )
                    cont = 1;
                if( out.x > out.y && ( i > out.x || i < out.y ) )
                    cont = 1;
            }
            if( cont )
                continue;
            Vertex a = polygon->var.vertex->get( i );
            Vertex b = polygon->var.vertex->get( j );
            if( ( ( a.y >= p.y ) != ( b.y >= p.y ) ) && ( p.x <= ( b.x - a.x ) * ( p.y - a.y ) / (float)( b.y - a.y ) + a.x ) )
                c = !c;
            j = i;
        }
        if( c )
            return 1;
    }
    return 0;
}

bool Model2DData::istLinieInnen( Vertex a, Vertex b, int polygonId ) const
{
    if( !polygons )
        return 0;
    int pAnz = polygons->getEintragAnzahl();
    for( int p = 0; p < pAnz; p++ )
    {
        if( polygonId >= 0 && p != polygonId )
            continue;
        int ola = outList.z( p )->getEintragAnzahl();
        int anz = polygons->get( p ).vertex->getEintragAnzahl();
        int j = anz - 1;
        for( int k = 0; k < ola; k++ )
        {
            Punkt out = outList.z( p )->get( k );
            if( out.x < out.y && j > out.x && j < out.y )
                j = out.x;
            if( out.x > out.y && ( j > out.x || j < out.y ) )
                j = out.x;
        }
        for( int i = 0; i < anz; i++ )
        {
            bool cont = 0;
            for( int k = 0; k < ola; k++ )
            {
                Punkt out = outList.z( p )->get( k );
                if( out.x < out.y && i > out.x && i < out.y )
                    cont = 1;
                if( out.x > out.y && ( i > out.x || i < out.y ) )
                    cont = 1;
            }
            if( cont )
                continue;
            Punkt va = polygons->get( p ).vertex->get( i );
            Punkt vb = polygons->get( p ).vertex->get( j );
            if( (Punkt)a == va && (Punkt)b == vb )
                return 1;
            if( (Punkt)a == vb && (Punkt)b == va )
                return 1;
            j = i;
        }
        Vertex len = b - a;
        Vertex speed( len.x > 0 ? 1 : -1.f, len.y > 0 ? 1 : -1.f );
        int mLen = 0;
        if( fabs( len.x ) > fabs( len.y ) )
        {
            mLen = (int)fabs( len.x );
            speed.y = len.y / (float)fabs( len.x );
        }
        else
        {
            mLen = (int)fabs( len.y );
            speed.x = len.x / (float)fabs( len.y );
        }
        int i = 1;
        bool inside = 1;
        for( Vertex vp = speed + a; (Punkt)vp != (Punkt)( b - speed ) && inside && i < mLen - 1; vp += speed, i++ )
            inside &= istPunktInnen( vp, p );
        if( inside )
            return 1;
    }
    return 0;
}

// nicht constant
bool Model2DData::erstelleModell( Array< Polygon2D > *polygons )
{
    removeModell();
    if( !polygons || !polygons->getEintragAnzahl() )
    {
        this->polygons = polygons;
        vListen = new RCArray< RCArray< DreieckListe< Vertex > > >();
        return 1;
    }
    this->polygons = polygons;
    int pAnz = polygons->getEintragAnzahl();
    vListen = new RCArray< RCArray< DreieckListe< Vertex > > >();
    for( int p = 0; p < pAnz; p++ )
    {
        Polygon2D pg = polygons->get( p );
        if( !pg.vertex || pg.vertex->getEintragAnzahl() < 3 )
            continue;
        vListen->add( new RCArray< DreieckListe< Vertex > >() );
        outList.set( new Array< Punkt >, p );
        int vAnz = pg.vertex->getEintragAnzahl();
        bool textur = pg.tKordinaten != 0;
        for( int i = 0; i < vAnz && textur; i++ )
            textur &= pg.tKordinaten->hat( i );
        for( int i = 0; i < vAnz; i++ )
        {
            if( maxP.x < fabs( pg.vertex->get( i ).x ) )
                maxP.x = abs( (int)pg.vertex->get( i ).x ) + 1;
            if( maxP.y < fabs( pg.vertex->get( i ).y ) )
                maxP.y = abs( (int)pg.vertex->get( i ).y ) + 1;
        }
        minP = -maxP;
        if( !textur )
        {
            if( pg.tKordinaten )
                pg.tKordinaten->leeren();
        }
        RCArray< RCArray< DreieckListe< Vertex > > > lists;
        int lauf = 0;
        while( 1 )
        {
            lists.set( new RCArray< DreieckListe< Vertex > >(), lauf );
            outList.z( p )->set( Punkt( 0, 0 ), lauf );
            bool fertig = 0;
            Vertex a;
            Vertex b;
            Array< Punkt > tmpOutList;
            for( int i = 0; i < vAnz; i++ )
            {
                bool cont = 0;
                int vorher = i - 1;
                int nachher = i + 1;
                if( nachher >= vAnz )
                    nachher = 0;
                if( vorher < 0 )
                    vorher = vAnz - 1;
                int ola = outList.z( p )->getEintragAnzahl();
                for( int j = 0; j < ola; j++ )
                {
                    Punkt out = outList.z( p )->get( j );
                    if( out.x < out.y )
                    {
                        if( nachher > out.x && nachher < out.y )
                            nachher = out.y;
                        if( vorher > out.x && vorher < out.y )
                            vorher = out.x;
                    }
                    if( out.x > out.y )
                    {
                        if( nachher > out.x || nachher < out.y )
                            nachher = out.y;
                        if( vorher > out.x || vorher < out.y )
                            vorher = out.x;
                    }
                    if( out.x < out.y && i > out.x && i < out.y )
                        cont = 1;
                    if( out.x > out.y && ( i > out.x || i < out.y ) )
                        cont = 1;
                }
                if( cont )
                    continue;
                if( vorher < 0 )
                    a = pg.vertex->get( vAnz + vorher );
                else
                    a = pg.vertex->get( vorher );
                if( nachher > vAnz - 1 )
                    b = pg.vertex->get( nachher - vAnz + 1 );
                else
                    b = pg.vertex->get( nachher );
                if( istLinieInnen( a, b, p ) )
                {
                    DreieckListe< Vertex > *lowL = new DreieckListe< Vertex >();
                    DreieckListe< Vertex > *heightL = new DreieckListe< Vertex >();
                    lowL->addPunkt( new Vertex( pg.vertex->get( i ) ), textur ? new Punkt( pg.tKordinaten->get( i ) ) : 0 );
                    heightL->addPunkt( new Vertex( pg.vertex->get( i ) ), textur ? new Punkt( pg.tKordinaten->get( i ) ) : 0 );
                    int height = i + 1;
                    int low = i - 1;
                    Punkt outL( 0, 0 );
                    Punkt outH( 0, 0 );
                    for( int k = 0; k < 2; k++ )
                    {
                        bool lowp = !k;
                        while( 1 )
                        {
                            if( height >= vAnz )
                                height = 0;
                            if( low < 0 )
                                low = vAnz - 1;
                            for( int j = 0; j <= lauf; j++ )
                            {
                                Punkt out = outList.z( p )->get( j );
                                if( out.x < out.y )
                                {
                                    if( height > out.x && height < out.y )
                                        height = out.y;
                                    if( low > out.x && low < out.y )
                                        low = out.x;
                                }
                                if( out.x > out.y )
                                {
                                    if( height > out.x || height < out.y )
                                        height = out.y;
                                    if( low > out.x || low < out.y )
                                        low = out.x;
                                }
                            }
                            Vertex a = pg.vertex->get( height );
                            Vertex b = pg.vertex->get( low );
                            if( low == height )
                            {
                                fertig = 1;
                                outList.z( p )->set( Punkt( 0, 0 ), lauf );
                                if( !k )
                                    lowL->addPunkt( new Vertex( b ), textur ? new Punkt( pg.tKordinaten->get( low ) ) : 0 );
                                else
                                    heightL->addPunkt( new Vertex( b ), textur ? new Punkt( pg.tKordinaten->get( low ) ) : 0 );
                                break;
                            }
                            bool inside = istLinieInnen( a, b, p );
                            if( inside )
                            {
                                if( !k )
                                    outL = Punkt( low, height );
                                else
                                    outH = Punkt( low, height );
                                outList.z( p )->set( Punkt( low, height ), lauf );
                            }
                            if( lowp )
                            {
                                if( !k )
                                    lowL->addPunkt( new Vertex( b ), textur ? new Punkt( pg.tKordinaten->get( low ) ) : 0 );
                                else
                                    heightL->addPunkt( new Vertex( b ), textur ? new Punkt( pg.tKordinaten->get( low ) ) : 0 );
                                low--;
                            }
                            else
                            {
                                if( !k )
                                    lowL->addPunkt( new Vertex( a ), textur ? new Punkt( pg.tKordinaten->get( height ) ) : 0 );
                                else
                                    heightL->addPunkt( new Vertex( a ), textur ? new Punkt( pg.tKordinaten->get( height ) ) : 0 );
                                height++;
                            }
                            lowp = !lowp;
                            if( !inside )
                            {
                                height = i + 1;
                                low = i - 1;
                                outList.z( p )->set( Punkt( 0, 0 ), lauf );
                                break;
                            }
                        }
                        if( fertig )
                            break;
                    }
                    if( lowL->getDreieckAnzahl() > heightL->getDreieckAnzahl() )
                    {
                        lists.z( lauf )->set( lowL, i );
                        tmpOutList.set( outL, i );
                        heightL->release();
                    }
                    else
                    {
                        lists.z( lauf )->set( heightL, i );
                        tmpOutList.set( outH, i );
                        lowL->release();
                    }
                }
                else
                    lists.z( lauf )->set( new DreieckListe< Vertex >(), i );
                if( fertig )
                    break;
            }
            int maxP = -1;
            int max = 0;
            for( int i = 0; i < vAnz; i++ )
            {
                if( lists.z( lauf )->z( i ) && lists.z( lauf )->z( i )->getDreieckAnzahl() > max )
                {
                    max = lists.z( lauf )->z( i )->getDreieckAnzahl();
                    maxP = i;
                }
            }
            if( !max || maxP < 0 )
                break;
            vListen->z( p )->add( lists.z( lauf )->get( maxP ) );
            outList.z( p )->set( tmpOutList.get( maxP ), lauf );
            if( fertig )
                break;
            lauf++;
        }
        outList.z( p )->leeren();
    }
    return 1;
}

void Model2DData::removeModell() // setzt die Vertex daten zurück
{
    if( polygons )
    {
        int anz = polygons->getEintragAnzahl();
        for( int i = 0; i < anz; i++ )
        {
            if( polygons->get( i ).tKordinaten )
                polygons->get( i ).tKordinaten->release();
            if( polygons->get( i ).vertex )
                polygons->get( i ).vertex->release();
        }
        polygons = polygons->release();
    }
    if( vListen )
        vListen = vListen->release();
    outList.leeren();
    minP = Punkt( 0, 0 );
    maxP = Punkt( 0, 0 );
}

// Reference Counting
Model2DData *Model2DData::getThis()
{
    ref++;
    return this;
}

Model2DData *Model2DData::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}


// Inhalt der Model2D Klasse aus Model2D.h
// Konstruktor
Model2D::Model2D()
    : Zeichnung(),
    textur( 0 )
{
    farbe = 0;
    style = 0;
    rData = 0;
    drehung = 0;
    size = 1;
    ref = 1;
}

// Destruktor
Model2D::~Model2D()
{
    if( rData )
        rData->release();
	if( textur )
		textur->release();
}

// nicht constant
void Model2D::setModel( Model2DData *mdl )
{
    if( rData )
        rData->release();
    rData = mdl;
}

void Model2D::setDrehung( float drehung )
{
    this->drehung = drehung;
    while( this->drehung > PI * 2 )
        this->drehung -= (float)PI * 2;
    while( this->drehung < 0 )
        this->drehung += (float)PI * 2;
    rend = 1;
}

void Model2D::addDrehung( float drehung )
{
    this->drehung += drehung;
    while( this->drehung > PI * 2 )
        this->drehung -= (float)PI * 2;
    while( this->drehung < 0 )
        this->drehung += (float)PI * 2;
    rend = 1;
}

void Model2D::setSize( float size )
{
    this->size = size;
    rend = 1;
}

void Model2D::addSize( float size )
{
    this->size += size;
    rend = 1;
}

void Model2D::setTextur( Bild *t )
{
    if( textur )
        textur->release();
    textur = t;
}

void Model2D::setFarbe( int f )
{
    farbe = f;
    rend = 1;
}

void Model2D::doMausEreignis( MausEreignis &me )
{
    if( !mak || me.verarbeitet || !istPunktInnen( Punkt( me.mx, me.my ) ) )
        return;
    me.mx -= pos.x;
    me.my -= pos.y;
    mak( makParam, this, me );
    me.mx += pos.x;
    me.my += pos.y;
    me.verarbeitet = 1;
}

bool Model2D::tick( double tickVal )
{
    bool ret = rend;
    rend = 0;
    return ret;
}

void Model2D::render( Bild &zRObj )
{
    if( !rData || hatStyleNicht( Model2D::Style::Sichtbar ) || !rData->polygons )
        return;
	Zeichnung::render( zRObj );
    int num = 0;
    for( auto *p = &rData->vListen->getArray(); p && p->set; p = p->next, num++ )
    {
        Mat3< float > mat = Mat3< float >::translation( pos ) * Mat3< float >::rotation( drehung ) * Mat3< float >::scaling( size );
        if( hatStyle( Model2D::Style::Textur ) )
        {
            if( !textur || !rData->polygons->get( num ).tKordinaten )
            {
                for( auto *i = &p->var->getArray(); i && i->set; i = i->next )
                {
                    for( auto *j = &i->var->zListe()->getArray(); j->next->next && j->next->next->set; j = j->next )
                    {
                        Vertex a = mat * *j->var->punkt;
                        Vertex b = mat * *j->next->var->punkt;
                        Vertex c = mat * *j->next->next->var->punkt;
                        if( hatStyle( Model2D::Style::Alpha ) )
                            zRObj.drawDreieckAlpha( a, b, c, farbe );
                        else
                            zRObj.drawDreieck( a, b, c, farbe );
                    }
                }
            }
            else
            {
                for( auto *i = &p->var->getArray(); i && i->set; i = i->next )
                {
                    for( auto *j = &i->var->zListe()->getArray(); j->next->next && j->next->next->set; j = j->next )
                    {
                        Vertex a = mat * *j->var->punkt;
                        Vertex b = mat * *j->next->var->punkt;
                        Vertex c = mat * *j->next->next->var->punkt;
                        if( hatStyle( Model2D::Style::Alpha ) )
                            zRObj.drawDreieckTexturAlpha( a, b, c, *j->var->textur, *j->next->var->textur, *j->next->next->var->textur, *textur );
                        else
                            zRObj.drawDreieckTextur( a, b, c, *j->var->textur, *j->next->var->textur, *j->next->next->var->textur, *textur );
                    }
                }
            }
        }
        if( hatStyle( Model2D::Style::Mesh ) )
        {
            for( auto *i = &p->var->getArray(); i && i->set; i = i->next )
            {
                for( auto *j = &i->var->zListe()->getArray(); j->next->next && j->next->next->set; j = j->next )
                {
                    Vertex a = mat * *j->var->punkt;
                    Vertex b = mat * *j->next->var->punkt;
                    Vertex c = mat * *j->next->next->var->punkt;
                    if( hatStyle( Model2D::Style::Alpha ) )
                    {
                        zRObj.drawLinieAlpha( a, b, farbe );
                        zRObj.drawLinieAlpha( b, c, farbe );
                        zRObj.drawLinieAlpha( c, a, farbe );
                    }
                    else
                    {
                        zRObj.drawLinie( a, b, farbe );
                        zRObj.drawLinie( b, c, farbe );
                        zRObj.drawLinie( c, a, farbe );
                    }
                }
            }
        }
        if( hatStyle( Model2D::Style::Rahmen ) )
        {
            ArrayEintrag< Vertex > &beg = rData->polygons->get( num ).vertex->getArray();
            if( beg.set )
            {
                ArrayEintrag< Vertex > *letzter = 0;
                for( ArrayEintrag< Vertex > *e = &beg; e && e->next && e->set && e->next->set; e = e->next )
                {
                    if( hatStyle( Model2D::Style::Alpha ) )
                        zRObj.drawLinieAlpha( mat * e->var, mat * e->next->var, farbe );
                    else
                        zRObj.drawLinie( mat * e->var, mat * e->next->var, farbe );
                    letzter = e->next;
                }
                if( letzter && letzter->set )
                {
                    if( hatStyle( Model2D::Style::Alpha ) )
                        zRObj.drawLinieAlpha( mat * letzter->var, mat * beg.var, farbe );
                    else
                        zRObj.drawLinie( mat * letzter->var, mat * beg.var, farbe );
                }
            }
        }
    }
}

// constant
float Model2D::getDrehung() const
{
    return drehung;
}

float Model2D::getSize() const
{
    return size;
}

bool Model2D::istPunktInnen( Vertex p ) const
{
    if( !rData )
        return 0;
    p -= pos;
    if( p < Mat3< float >::scaling( size ) * rData->minP || p > Mat3< float >::scaling( size ) * rData->maxP || !rData->polygons )
        return 0;
    int num = 0;
    for( auto *polygon = &rData->polygons->getArray(); polygon && polygon->set; polygon = polygon->next, num++ )
    {
        Mat3< float > mat = Mat3< float >::rotation( drehung ) * Mat3< float >::scaling( size );
        int anz = polygon->var.vertex->getEintragAnzahl();
        bool c = 0;
        int j = anz - 1;
        for( int i = 0; i < anz; i++ )
        {
            Vertex a = mat * polygon->var.vertex->get( i );
            Vertex b = mat * polygon->var.vertex->get( j );
            if( ( ( a.y >= p.y ) != ( b.y >= p.y ) ) && ( p.x <= ( b.x - a.x ) * ( p.y - a.y ) / (float)( b.y - a.y ) + a.x ) )
                c = !c;
            j = i;
        }
        if( c )
            return 1;
    }
    return 0;
}

bool Model2D::istLinieInnen( Vertex a, Vertex b ) const
{
    if( !rData || !rData->polygons )
        return 0;
    int pAnz = rData->polygons->getEintragAnzahl();
    for( int p = 0; p < pAnz; p++ )
    {
        Mat3< float > mat = Mat3< float >::rotation( drehung ) * Mat3< float >::scaling( size );
        int anz = rData->polygons->get( p ).vertex->getEintragAnzahl();
        int j = anz - 1;
        for( int i = 0; i < anz; i++ )
        {
            Punkt va = mat * rData->polygons->get( p ).vertex->get( i );
            Punkt vb = mat * rData->polygons->get( p ).vertex->get( j );
            if( (Punkt)a == pos + va && (Punkt)b == pos + vb )
                return 1;
            if( (Punkt)a == pos + vb && (Punkt)b == pos + va )
                return 1;
            j = i;
        }
        Vertex len = b - a;
        Vertex speed( len.x > 0 ? 1 : -1.f, len.y > 0 ? 1 : -1.f );
        int mLen = 0;
        if( fabs( len.x ) > fabs( len.y ) )
        {
            mLen = (int)fabs( len.x );
            speed.y = len.y / (float)fabs( len.x );
        }
        else
        {
            mLen = (int)fabs( len.y );
            speed.x = len.x / (float)fabs( len.y );
        }
        int i = 1;
        bool inside = 1;
        for( Vertex vp = speed + a; (Punkt)vp != (Punkt)( b - speed ) && inside && i < mLen - 1; vp += speed, i++ )
            inside &= istPunktInnen( vp );
        if( inside )
            return 1;
    }
    return 0;
}

bool Model2D::istModelInnen( const Model2D *zMdl, bool end ) const
{
    if( !end )
    {
        Vertex min = (Vertex)rData->minP * size + pos;
        Vertex max = (Vertex)rData->maxP * size + pos;
        Vertex min2 = (Vertex)zMdl->zModel()->minP * zMdl->getSize() + zMdl->getPosition();
        Vertex max2 = (Vertex)zMdl->zModel()->maxP * zMdl->getSize() + zMdl->getPosition();
        if( max.x < min2.x || min.x > max2.x || max.y < min2.y || min.y > max2.y )
            return 0;
    }
    Mat3< float > mat = Mat3< float >::translation( pos ) * Mat3< float >::rotation( drehung ) * Mat3< float >::scaling( size );
    for( auto *polygon = &rData->polygons->getArray(); polygon && polygon->set; polygon = polygon->next )
    {
        int anz = polygon->var.vertex->getEintragAnzahl();
        for( int i = 0; i < anz; i++ )
        {
            if( zMdl->istPunktInnen( mat * polygon->var.vertex->get( i ) ) )
                return 1;
        }
    }
    if( end )
        return 0;
    return zMdl->istModelInnen( this, 1 );
}

Model2DData *Model2D::getModel() const
{
    return rData ? rData->getThis() : 0;
}

Model2DData *Model2D::zModel() const
{
    return rData;
}

// Reference Counting
Model2D *Model2D::getThis()
{
    ++ref;
    return this;
}

Model2D *Model2D::release()
{
    --ref;
    if( !ref )
        delete this;
    return 0;
}