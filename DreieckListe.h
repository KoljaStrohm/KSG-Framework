#ifndef DreieckListe_H
#define DreieckListe_H

#include "Punkt.h"
#include "Array.h"

namespace Framework
{
	template< typename T >
	struct DreieckPunkt
	{
		T *punkt;
		Punkt *textur;
        // Konstruktor
        DreieckPunkt( T *punkt, Punkt *textur )
        {
            this->punkt = punkt;
            this->textur = textur;
        }
		// Destruktor
		~DreieckPunkt()
		{
			delete punkt;
			delete textur;
		}
	};

	template< typename T >
	class DreieckListe
	{
	private:
		Array< DreieckPunkt< T >* > *punkte;
		int ref;

	public:
		// Konstruktor
		DreieckListe()
		{
			ref = 1;
			punkte = new Array< DreieckPunkt< T >* >();
		}

		// Destruktor
		~DreieckListe()
		{
			int anz = punkte->getEintragAnzahl();
			for( int i = 0; i < anz; i++ )
				delete punkte->get( i );
			punkte->release();
		}

		// nicht constant
		void addPunkt( T *p, Punkt *textur )
		{
			punkte->add( new DreieckPunkt< T >( p, textur ) );
		}

		void löscheLetztenPunkt()
		{
			int i = punkte->getEintragAnzahl() - 1;
			if( !punkte->hat( i ) )
				return;
			delete punkte->get( i );
			punkte->lösche( i );
		}

		void lehren()
		{
			int anz = punkte->getEintragAnzahl();
			for( int i = 0; i < anz; i++ )
				delete punkte->get( i );
			punkte->leeren();
		}

		// constant
		int getDreieckAnzahl() const
		{
			return punkte->getEintragAnzahl() - 2;
		}

		bool hatTextur() const
		{
			int anz = punkte->getEintragAnzahl();
			bool ret = 1;
			for( int i = 0; i < anz; i++ )
			{
				if( punkte->hat( i ) )
				    ret &= punkte->get( i )->textur;
			}
			return ret;
		}

		Array< DreieckPunkt< T >* > *zListe() const
		{
			return punkte;
		}

		// Reference Counting
		DreieckListe *getThis()
		{
			ref++;
			return this;
		}

		DreieckListe *release()
		{
			ref--;
			if( !ref )
				delete this;
			return 0;
		}
	};
}

#endif