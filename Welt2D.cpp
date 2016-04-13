#include "Welt2D.h"
#include "Model2D.h"

using namespace Framework;

// Inhalt der Welt2D Klasse aus Welt3D.h
// Konstruktor
Welt2D::Welt2D()
{
    obj = new RCArray< Model2D >();
    ref = 1;
}

// Destruktor
Welt2D::~Welt2D()
{
    obj->release();
}

// nicht constant
void Welt2D::addModel( Model2D *obj )
{
    this->obj->add( obj, 0 );
}

void Welt2D::removeModel( Model2D *zObj )
{
    auto *e = &obj->getArray();
    for( int z = 0; e && e->set; z++, e = e->next )
    {
        if( e->var == zObj )
        {
            obj->lösche( z );
            return;
        }
    }
}

void Welt2D::setModelInVordergrund( Model2D *zObj )
{
    auto *e = &obj->getArray();
    for( int z = 0; e && e->set; z++, e = e->next )
    {
        if( e->var == zObj )
        {
            obj->setPosition( z, 0 );
            return;
        }
    }
}

void Welt2D::setModelInHintergrund( Model2D *zObj )
{
    int anz = obj->getEintragAnzahl();
    auto *e = &obj->getArray();
    for( int z = 0; e && e->set; z++, e = e->next )
    {
        if( e->var == zObj && z != anz - 1 )
        {
            obj->setPosition( z, anz - 1 );
            return;
        }
    }
}

void Welt2D::removeAll()
{
    obj->leeren();
}

void Welt2D::doMausEreignis( MausEreignis &me )
{

}

void Welt2D::doTastaturEreignis( TastaturEreignis &me )
{

}

bool Welt2D::tick( double t )
{
    return 0;
}

void Welt2D::render( Bild &zRObj, Punkt &wPos, Punkt &wGr, Punkt &kamGr )
{

}

// constant
int Welt2D::getModelAnzahl() const
{
    return obj->getEintragAnzahl();
}

Model2D *Welt2D::getModel( int z ) const
{
    return obj->get( z );
}

Model2D *Welt2D::zModel( int z ) const
{
    return obj->z( z );
}

// Reference Counting
Welt2D *Welt2D::getThis()
{
    ref++;
    return this;
}

Welt2D *Welt2D::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}