#ifndef DreieckListe_H
#define DreieckListe_H

#include "Punkt.h"
#include "Array.h"

namespace Framework
{
	template< typename T >
    // Eine Ecke eines Dreiecks
	struct DreieckPunkt
	{
		T *punkt;
		Punkt *textur;
        // Konstruktor
        //  punkt: Die Koordinate der Ecke
        //  textur: Die Koordinate in der Textur
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
    // Eine Liste von Dreiecken, wobei immer die Beiden letzten Punkte des vorheriegen Dreiecks mit dem nächsten Punkt ein neues Dreieck bilden
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

		// Fügt der Liste einen Punkt hinzu
        //  p: Die Koordinaten des Punktes
        //  textur: Die Koordinaten in der Textur
		void addPunkt( T *p, Punkt *textur )
		{
			punkte->add( new DreieckPunkt< T >( p, textur ) );
		}

        // Löscht den letzten Punkt
		void löscheLetztenPunkt()
		{
			int i = punkte->getEintragAnzahl() - 1;
			if( !punkte->hat( i ) )
				return;
			delete punkte->get( i );
			punkte->lösche( i );
		}

        // Löscht alle Ecken
		void lehren()
		{
			int anz = punkte->getEintragAnzahl();
			for( int i = 0; i < anz; i++ )
				delete punkte->get( i );
			punkte->leeren();
		}

		// gibt die Anzahl der Dreiecke zurück
		int getDreieckAnzahl() const
		{
			return punkte->getEintragAnzahl() - 2;
		}

        // Gibt zurück, ob eine Textur verwendet wird
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

        // Gibt die Liste mit Punkten zurück
		Array< DreieckPunkt< T >* > *zListe() const
		{
			return punkte;
		}

        // Erhöht den Reference Counting Zähler.
        //  return: this.
		DreieckListe *getThis()
		{
			ref++;
			return this;
		}

        // Verringert den Reference Counting Zähler. Wenn der Zähler 0 erreicht, wird das Objekt automatisch gelöscht.
        //  return: 0.
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