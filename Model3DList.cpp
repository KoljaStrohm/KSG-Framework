#include "Model3DList.h"
#include "Model3D.h"
#include "Text.h"

using namespace Framework;

int Model3DList::id = 0;
Critical Model3DList::cs;

const char *Standart3DTypes::cube = "f_würfel";

// Inhalt der Model3DList Klasse
// Konstruktor
Model3DList::Model3DList()
{
    models = new RCArray< Model3DData >();
    names = new RCArray< Text >();
    ref = 1;
}

// Destruktor
Model3DList::~Model3DList()
{
    models->release();
    names->release();
}

// Fügt der Liste ein Model Hinzu
//  mdl: Das Model
//  name: Der name, unter dem das Model in der Liste gespeichert wird
bool Model3DList::addModel( Model3DData *mdl, const char *name )
{
    cs.lock();
    for( auto i = names->getArray(); i.set; i++ )
    {
        if( i.var->istGleich( name ) )
        {
            mdl->release();
            cs.unlock();
            return 0;
        }
    }
    mdl->id = id++;
    models->add( mdl );
    names->add( new Text( name ) );
    cs.unlock();
    return 1;
}

// Entfernt ein Model aus der Liste
//  name: Der Name des Models
void Model3DList::removeModel( const char *name )
{
    cs.lock();
    int index = 0;
    for( auto i = names->getArray(); i.set; i++ )
    {
        if( i.var->istGleich( name ) )
        {
            names->remove( index );
            models->remove( index );
            cs.unlock();
            return;
        }
        index++;
    }
    cs.unlock();
}

// Überprüft, ob unter einem bestimmten Namen ein Model abgespeichert wurde
//  name: Der Name
//  return: true, wenn ein Model mit dem Namen existiert
bool Model3DList::hatModel( const char *name ) const
{
    cs.lock();
    for( auto i = names->getArray(); i.set; i++ )
    {
        if( i.var->istGleich( name ) )
        {
            cs.unlock();
            return 1;
        }
    }
    cs.unlock();
    return 0;
}

// Gibt ein bestimmtes Model zurück
//  name: Der Name des Models
Model3DData *Model3DList::getModel( const char *name ) const
{
    cs.lock();
    int index = 0;
    for( auto i = names->getArray(); i.set; i++ )
    {
        if( i.var->istGleich( name ) )
        {
            cs.unlock();
            return models->get( index );
        }
        index++;
    }
    cs.unlock();
    return 0;
}

// Gibt ein bestimmtes Model ohne erhöhten Reference Counter zurück
//  name: Der Name des Models
Model3DData *Model3DList::zModel( const char *name ) const
{
    cs.lock();
    int index = 0;
    for( auto i = names->getArray(); i.set; i++ )
    {
        if( i.var->istGleich( name ) )
        {
            cs.unlock();
            return models->z( index );
        }
        index++;
    }
    cs.unlock();
    return 0;
}

// Erhöht den Reference Counting Zähler.
//  return: this.
Model3DList *Model3DList::getThis()
{
    ref++;
    return this;
}

// Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
//  return: 0.
Model3DList *Model3DList::release()
{
    ref--;
    if( !ref )
        delete this;
    return 0;
}

// statische Funktionen

// Initialisiert statische private member. Wird vom Framework automatisch aufgerufen.
void Model3DList::init()
{
    id = 0;
}

// Löscht statische private member. Wird vom Framework automatisch aufgerufen.
void Model3DList::destroy()
{
}