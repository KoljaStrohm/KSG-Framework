#ifndef Array_H
#define Array_H

#include "Betriebssystem.h"
#include <stdexcept>
#include "Text.h"

namespace Framework
{
    template< class TYP >
    // Ein Eintrag in einer Linked List
    struct ArrayEintrag
    {
        TYP var;
        bool set;
        ArrayEintrag< TYP > *next;

        // Setzt den Eintrag auf die Werte des anderen Eintrages
        ArrayEintrag &operator=( ArrayEintrag &r )
        {
            var = r.var;
            set = r.set;
            next = r.next;
            return *this;
        }
        // Gibt den aktuell gespeicherten Wert zurück
        operator TYP()
        {
            return var;
        }
        // inkrementiert durch die Linked List durch
        ArrayEintrag< TYP > &operator++() // prefix
        {
            if( !next )
            {
                ArrayEintrag<TYP> tmp = ArrayEintrag{ 0, 0, 0 };
                *this = tmp;
                return *this;
            }
            *this = *next;
            return *next;
        }
        // inkrementiert durch die Linked List durch
        ArrayEintrag< TYP > &operator++( int ) // postfix
        {
            if( !next )
            {
                ArrayEintrag<TYP> tmp = ArrayEintrag{ 0, 0, 0 };
                *this = tmp;
                return *this;
            }
            *this = *next;
            return *next;
        }
    };

    template< class TYP >
    // Eine Linked List von Klassen, die kein Reference Counting berteiben
    class Array
    {
    private:
        ArrayEintrag< TYP > *entries;
        int ref;

    public:
        // Erstellt eine neue Linked List
        Array()
        {
			entries = new ArrayEintrag< TYP >();
			entries->set = 0;
            entries->next = 0;
            ref = 1;
        }

        // Leert und löscht die Linked List 
        ~Array()
        {
            leeren();
            delete entries;
        }

        // Hängt ein Element ans Ende der Liste an
        //  t: Das neue Element
        void add( TYP t )
        {
            for( ArrayEintrag< TYP > *e = entries; 1; e = e->next )
            {
                if( !e->set && !e->next )
                {
                    e->var = t;
                    e->set = 1;
                    break;
                }
                if( !e->next )
                {
                    e->next = new ArrayEintrag< TYP >();
                    e->next->set = 0;
                    e->next->next = 0;
                }
            }
        }

        // Fügt ein Element bei einer bestimmten Position in die Liste ein
        //  t: das neue Element
        //  i: Die Position, wo das Element eingefügt wird (danach der Index des neuen Elementes)
        void add( TYP t, int i )
        {
            if( i < 0 )
                return;
            ArrayEintrag< TYP > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                {
                    ArrayEintrag< TYP > *ne = new ArrayEintrag< TYP >();
                    ne->set = 0;
                    ne->next = 0;
                    e->next = ne;
                }
                e = e->next;
            }
            ArrayEintrag< TYP > *ne = new ArrayEintrag< TYP >();
            ne->var = e->var;
            ne->set = e->set;
            ne->next = e->next;
            e->next = ne;
            e->var = t;
            e->set = 1;
        }

        // Setzt den Wert des i-ten Eintrags
        //  t: der Neue Wert
        //  i: Der Index des Eintrages der gesetzt werden soll
        void set( TYP t, int i )
        {
            if( i < 0 )
                return;
            ArrayEintrag< TYP > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                {
                    ArrayEintrag< TYP > *ne = new ArrayEintrag< TYP >();
                    ne->set = 0;
                    ne->next = 0;
                    e->next = ne;
                }
                e = e->next;
            }
            e->var = t;
            e->set = 1;
        }

        // Verändert die Position des i-ten Elementes in der Liste
        //  i: Der Index des Elementes, welches verschoben werden soll
        //  p: Die Zielposition des Elementes (danach der neue Index des Elementes)
        void setPosition( int i, int p )
        {
            if( i < 0 || p < 0 || i == p )
                return;
            ArrayEintrag< TYP > *e = entries;
            ArrayEintrag< TYP > *ve = 0;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                    return;
                ve = e;
                e = e->next;
            }
            ArrayEintrag< TYP > *e2 = entries == e ? e->next : entries;
            ArrayEintrag< TYP > *ve2 = 0;
            for( int a = 0; a < p; ++a )
            {
                if( !e2 )
                    return;
                ve2 = e2;
                if( e2->next == e )
                    e2 = e->next;
                else
                    e2 = e2->next;
            }
            if( !e )
                return;
            if( !ve2 )
				entries = e;
            else
                ve2->next = e;
            if( ve )
                ve->next = e->next;
            else
				entries = e->next;
            e->next = e2;
        }

        // Löscht ein Bestimmtes Element
        //  i: Der Index des Elementes das gelöscht werden soll
        void remove( int i )
        {
            if( i < 0 )
                return;
            ArrayEintrag< TYP > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                    return;
                e = e->next;
            }
            if( !e )
                return;
            if( e->next )
            {
                e->var = e->next->var;
                e->set = e->next->set;
            }
            else
                e->set = 0;
            ArrayEintrag< TYP > *del = e->next;
            if( e->next )
                e->next = e->next->next;
            else
                e->next = 0;
            if( del )
            {
                del->set = 0;
                del->next = 0;
                delete del;
            }
        }

        // Vertauscht zwei Elemente in der Liste
        //  vi: Der Index des ersten Elementes
        //  ni: Der Index des zweiten Elementes
        void tausch( int vi, int ni )
        {
            if( vi < 0 || ni < 0 )
                return;
            TYP tmp = get( ni );
            set( get( vi ), ni );
            set( tmp, vi );
        }

        // Löscht alle Elemente der Liste
        void leeren()
        {
            ArrayEintrag< TYP > *e2 = 0;
            for( ArrayEintrag< TYP > *e = entries; e; e = e->next )
            {
                delete e2;
                e2 = e;
            }
            delete e2;
			entries = new ArrayEintrag< TYP >();
			entries->set = 0;
			entries->next = 0;
        }

        // Gibt das Erste Element der Liste zurück.
        // Mit ++ kann durch die Liste iteriert werden
        ArrayEintrag< TYP > &getArray()
        {
            return *entries;
        }

        // Gibt zurück, wie viele Elemente in der Liste sind
        int getEintragAnzahl() const
        {
            int i = 0;
            for( ArrayEintrag< TYP > *e = entries; e && ( e->set || e->next ); e = e->next )
                ++i;
            return i;
        }

        // Gibt den Wert des i-ten Elementes zurück
        //  i: Der index des gesuchten Elementes
        // throws:
        //  std::out_of_range wenn i < 0 oder i >= getEintragAnzahl()
        TYP get( int i ) const
        {
            if( i < 0 )
            {
                Text err = "Index out of Range Exception File: ";
                err += __FILE__;
                err += " Line: ";
                err += __LINE__;
                err += " Index: ";
                err += i;
                throw std::out_of_range( (char*)err );
            }
            ArrayEintrag< TYP > *e = entries;
            for( int a = 0; a < i && e; ++a )
                e = e->next;
            if( e && e->set )
                return e->var;
            Text err = "Index out of Range Exception File: ";
            err += __FILE__;
            err += " Line: ";
            err += __LINE__;
            err += " Index: ";
            err += i;
            throw std::out_of_range( (char*)err );
        }

        // Überprüft, ob ein Element in der Liste enthalten ist
        //  i: Der Index des gesuchten Elementes
        //  return: (true), wenn der Index vorhanden ist. (false) sonnst
        bool hat( int i ) const
        {
            if( i < 0 )
                return 0;
            ArrayEintrag< TYP > *e = entries;
            for( int a = 0; a < i && e; ++a )
                e = e->next;
            if( e && e->set )
                return 1;
            return 0;
        }

        // Gibt den Index eines Wertes zurück
        //  t: Der Wert, nach dem gesucht werden soll
        int getWertIndex( TYP t ) const
        {
            int ret = 0;
            for( ArrayEintrag< TYP > *e = entries; e; e = e->next )
            {
                if( e->set && e->var == t )
                    return ret;
                ++ret;
            }
            return -1;
        }

        // Erhöht den Reference Counting Zähler.
        //  return: this.
        Array< TYP > *getThis()
        {
            ++ref;
            return this;
        }

        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        Array< TYP > *release()
        {
            --ref;
            if( !ref )
                delete this;
            return 0;
        }
    };

    template< class TYP >
    // Eine Linked List von Zeigern auf Zeichnunge, die Reference Counting berteiben
    class RCArray
    {
    private:
        ArrayEintrag< TYP* > *entries;
        int ref;

    public:
        // Erstellt eine neue Linked List
        RCArray()
        {
            entries = new ArrayEintrag< TYP* >();
            entries->set = 0;
            entries->next = 0;
            ref = 1;
        }

        // Leert und löscht die Linked List 
        ~RCArray()
        {
            leeren();
            delete entries;
        }

        // Hängt ein Element ans Ende der Liste an
        //  t: Das neue Element
        void add( TYP* t )
        {
            for( ArrayEintrag< TYP* > *e = entries; 1; e = e->next )
            {
                if( !e->set && !e->next )
                {
                    e->var = t;
                    e->set = 1;
                    break;
                }
                if( !e->next )
                {
                    e->next = new ArrayEintrag< TYP* >();
                    if( e->next->set && e->next->var )
                        e->next->var->release();
                    e->next->set = 0;
                    e->next->next = 0;
                }
            }
        }

        // Fügt ein Element bei einer bestimmten Position in die Liste ein
        //  t: das neue Element
        //  i: Die Position, wo das Element eingefügt wird (danach der Index des neuen Elementes)
        void add( TYP* t, int i )
        {
            if( i < 0 )
            {
                if( t )
                    t->release();
                return;
            }
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                {
                    ArrayEintrag< TYP* > *ne = new ArrayEintrag< TYP* >();
                    ne->set = 0;
                    ne->next = 0;
                    e->next = ne;
                }
                e = e->next;
            }
            ArrayEintrag< TYP* > *ne = new ArrayEintrag< TYP* >();
            ne->var = e->var;
            ne->set = e->set;
            ne->next = e->next;
            e->next = ne;
            e->var = t;
            e->set = 1;
        }

        // Setzt den Wert des i-ten Eintrags
        //  t: der Neue Wert
        //  i: Der Index des Eintrages der gesetzt werden soll
        void set( TYP* t, int i )
        {
            if( i < 0 )
            {
                if( t )
                    t->release();
                return;
            }
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                {
                    ArrayEintrag< TYP* > *ne = new ArrayEintrag< TYP* >();
                    ne->set = 0;
                    ne->next = 0;
                    e->next = ne;
                }
                e = e->next;
            }
            if( e->set && e->var )
                e->var->release();
            e->var = t;
            e->set = 1;
        }

        // Verändert die Position des i-ten Elementes in der Liste
        //  i: Der Index des Elementes, welches verschoben werden soll
        //  p: Die Zielposition des Elementes (danach der neue Index des Elementes)
        void setPosition( int i, int p )
        {
            if( i < 0 || p < 0 || i == p )
                return;
            ArrayEintrag< TYP* > *ve = 0;
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                    return;
                ve = e;
                e = e->next;
            }
            ArrayEintrag< TYP* > *e2 = entries == e ? e->next : entries;
            ArrayEintrag< TYP* > *ve2 = 0;
            for( int a = 0; a < p; ++a )
            {
                if( !e2 )
                    return;
                ve2 = e2;
                if( e2->next == e )
                    e2 = e->next;
                else
                    e2 = e2->next;
            }
            if( !e )
                return;
            if( !ve2 )
                entries = e;
            else
                ve2->next = e;
            if( ve )
                ve->next = e->next;
            else
                entries = e->next;
            e->next = e2;
        }

        // Löscht ein Bestimmtes Element
        //  i: Der Index des Elementes das gelöscht werden soll
        void remove( int i )
        {
            if( i < 0 )
                return;
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i; ++a )
            {
                if( !e->next )
                    return;
                e = e->next;
            }
            if( !e )
                return;
            if( e->next )
            {
                if( e->set && e->var )
                    e->var->release();
                e->var = e->next->var;
                e->set = e->next->set;
            }
            else
            {
                if( e->set && e->var )
                    e->var->release();
                e->set = 0;
            }
            ArrayEintrag< TYP* > *del = e->next;
            if( e->next )
                e->next = e->next->next;
            else
                e->next = 0;
            if( del )
            {
                del->set = 0;
                del->next = 0;
                delete del;
            }
        }

        // Vertauscht zwei Elemente in der Liste
        //  vi: Der Index des ersten Elementes
        //  ni: Der Index des zweiten Elementes
        void tausch( int vi, int ni )
        {
            if( vi < 0 || ni < 0 )
                return;
            TYP* tmp = get( ni );
            set( get( vi ), ni );
            set( tmp, vi );
        }

        // Löscht alle Elemente der Liste
        void leeren()
        {
            ArrayEintrag< TYP* > *e2 = 0;
            for( ArrayEintrag< TYP* > *e = entries; e; e = e->next )
            {
                if( e2 && e2->var && e2->set )
                    e2->var->release();
                delete e2;
                e2 = e;
            }
            if( e2 && e2->var && e2->set )
                e2->var->release();
            delete e2;
            entries = new ArrayEintrag< TYP* >();
            entries->set = 0;
            entries->next = 0;
        }

        // Gibt das Erste Element der Liste zurück.
        // Mit ++ kann durch die Liste iteriert werden
        ArrayEintrag< TYP* > &getArray()
        {
            return *entries;
        }

        // Gibt zurück, wie viele Elemente in der Liste sind
        int getEintragAnzahl() const
        {
            int i = 0;
            for( ArrayEintrag< TYP* > *e = entries; e && ( e->set || e->next ); e = e->next )
                ++i;
            return i;
        }

        // Gibt den Wert des i-ten Elementes zurück mit erhöhtem Reference Counter
        //  i: Der index des gesuchten Elementes, (0) wenn der Index nicht existiert
        TYP *get( int i ) const
        {
            if( i < 0 )
                return (TYP*)0;
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i && e; ++a )
                e = e->next;
            if( e && e->set && e->var )
                return (TYP*)e->var->getThis();
            return (TYP*)0;
        }

        // Gibt den Wert des i-ten Elementes zurück ohne erhöhten Reference Counter
        //  i: Der index des gesuchten Elementes, (0) wenn der Index nicht existiert
        TYP *z( int i ) const // gibt den index - ten T zurück
        {
            if( i < 0 )
                return (TYP*)0;
            ArrayEintrag< TYP* > *e = entries;
            for( int a = 0; a < i && e; ++a )
                e = e->next;
            if( e && e->set && e->var )
                return (TYP*)e->var;
            return (TYP*)0;
        }

        // Erhöht den Reference Counting Zähler.
        //  return: this.
        RCArray< TYP > *getThis()
        {
            ++ref;
            return this;
        }

        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Zeichnung automatisch gelöscht.
        //  return: 0.
        RCArray< TYP > *release()
        {
            --ref;
            if( !ref )
                delete this;
            return 0;
        }
    };
}

#endif