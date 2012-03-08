#ifndef C_LineApprox_h__
#define C_LineApprox_h__

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#include <memory.h>
#include <crtdbg.h>
#include "C_Line.h"
#include "interpol.h"
#include "coeff.h"

/// Rodzaje approxymacji
enum APPROX_TYPE {
	SPLINE
};
/** 
 * Klasa realizujaca interpolacj� danych na obrazie na podstawie linii pomi�dzy punktami. Przechowuje kopi� danych
 * ze wzgl�du na wym�g float stawiany przez procedury interpolacji. Przy inicjalizacji trzeba poda� odno�cnik do obrazu oraz macierzy 3
 * elementowej okre�laj�cej ilo�� element�w w ka�dym wymiarze.
 */
class C_LineApprox : public C_Line
{
public:
	/// domy�lny konstruktor
	C_LineApprox();
	/// konstruktor wykorzystuj�cy wsp�czynniki prostej
	C_LineApprox(APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion, const double *const _image, const unsigned int _size[] );
	/// konstruktor wykorzystuj�cy punkty przez kt�re prosta przechodzi
	C_LineApprox(APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1, const double *const _image, const unsigned int _size[] );
	/// Zwraca punkty na linii aproxymowanej pomi�dzy P0 i P1
	virtual bool getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N  );
	/// zwaraca ilo�� element�w na podstawie tablicy im_size
	unsigned int getNumOfElements() const; 
	/// zwraca tablic� z interpolowanymi danymi
	const double * getInterpolated_data() const { return interpolated_data; }
	virtual ~C_LineApprox();
protected:
//	void Interpolate
private:
	APPROX_TYPE typ_interpolacji;
	/// wska�nik przechowuj�cy obraz, jest modyfikowany przez getPointsOnLine
	float *image;
	/// tablica przechowuj�ca wynik interpolacji
	double *interpolated_data;
	/// wymiary danych w image [rows cols z]
	int im_size[3];
	/// ilosc element�w
	unsigned int N;
};

#endif // C_LineApprox_h__
