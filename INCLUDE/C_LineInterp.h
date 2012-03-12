/**
 * \file    C_LineInterp.h
 * \brief	Interpoluje profil liniowy obrazy zdjêty z linii prostej.	
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_LineInterp_h__
#define C_LineInterp_h__

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
 * Klasa realizujaca interpolacjê danych na obrazie na podstawie linii pomiêdzy punktami. Przechowuje kopiê danych
 * ze wzglêdu na wymóg float stawiany przez procedury interpolacji. Przy inicjalizacji trzeba podaæ odnoœcnik do obrazu oraz macierzy 3
 * elementowej okreœlaj¹cej iloœæ elementów w ka¿dym wymiarze.
 * Interpolowane dane oraz ich wspó³rzêdne s¹ przechowywane wewnêtrz klasy i dostêpne za pomoc¹ C_LineApprox::getInterpolated_data.
 * Schmemat dzia³ania:\n
 * \li Inicjalizacja obiektu = dane interpolowane interpolated_data ustawione na null oraz image te¿ - jeœli u¿yty jest domyœlny konstruktor to trzeba inicjalizowaæ funkcj¹ setLine2Points
 * \li Interpolacja getPointsOnLine wype³nione s¹ tablice oraz zwracane wspó³rzêdne x,y punktów dla których wykonana by³a interpolacja
 * \li Pobranie wyników za pomoc¹ funkcji read-only getInterpolated_data oraz getInterpolatedXY
 */
class C_LineInterp : public C_Line
{
public:
	/// domyœlny konstruktor
	C_LineInterp();
	/// konstruktor wykorzystuj¹cy wspó³czynniki prostej
	C_LineInterp(APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion, const double *const _image, const unsigned int _size[] );
	/// konstruktor wykorzystuj¹cy punkty przez które prosta przechodzi
	C_LineInterp(APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1, const double *const _image, const unsigned int _size[] );
	/// definiuje linie interpolacji na podstawie 2 punktów
	virtual void ManualConstructor(APPROX_TYPE type,const C_Point &P0, const C_Point &P1, const double *const _image, const unsigned int _size[] );
	/// Wykonuje interpolacjê: Zwraca punkty na linii aproxymowanej pomiêdzy P0 i P1 oraz zapisuje je w obiekcie
	virtual bool getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N  );
	/// Wykonuje interpolacjê Zapisuje obliczone punkty w obiekcie
	virtual bool getPointsOnLine( const C_Point &_P0, const C_Point &_P1, int N  );
	/// zwaraca iloœæ elementów na podstawie tablicy im_size
	unsigned int getNumOfElements() const; 
	/// zwraca tablicê z interpolowanymi danymi
	const double * getInterpolated_data() const { return interpolated_data; }
	/// zwraca tablice ze wspó³rzêdnymi x
	const double * getInterpolated_X() const { return x; }
	/// zwraca tablice ze wspó³rzêdnymi y
	const double * getInterpolated_Y() const { return y; }
	virtual ~C_LineInterp();
protected:
//	void Interpolate
private:
	/// Tworzy tablice w klasie
	void SafeAllocateTab();
	/// kopiuje dane pomiêdzy buforami
	void DataCopy(double *src,double *dest);
	APPROX_TYPE typ_interpolacji;
	/// wskaŸnik przechowuj¹cy obraz, jest modyfikowany przez getPointsOnLine
	float *image;
	/// tablica przechowuj¹ca wynik interpolacji
	double *interpolated_data;
	/// wymiary danych w image [rows cols z]
	int im_size[3];
	/// ilosc elementów obrazu
	unsigned int N;
	/// tablica z wsp x
	double *x;
	/// tablica z wsp y
	double *y;
};

#endif // C_LineApprox_h__
