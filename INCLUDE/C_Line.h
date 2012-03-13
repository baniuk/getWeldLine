/**
 * \file    C_Line.h
 * \brief	Definiuje linie w przestrzeni 2D	
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_Line_h__
#define C_Line_h__

#include "C_Point.h"
#include <crtdbg.h>
/// definiuje typ danych okre�laj�cy kierunek prostej
enum KIERUNEK_PROSTEJ {
	PIONOWA,
	NORMALNA
};

#ifndef NULL
#define NULL 0
#endif

/** 
 * Klasa implementuj�ca lini� w przestrzeni 2D. Je�li prosta jest pionowa to wtedy warto�� a jest nieokre�lona a warto��
 * b przyjmuje warto�� wsp�rz�dnej x
 */ 
class C_Line
{
public:
	C_Line();
	/// buduje prost� na podstawie paramstr�w
	C_Line(const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion);
	/// buduje linie na podstawie dw�ch punkt�w na niej
	C_Line(const C_Point &_P0,const C_Point &_P1);
	virtual ~C_Line();
	/// zwraca wszystkie parametry prostej
	KIERUNEK_PROSTEJ getLine(double &_a, double &_b) const;
	/// ustawia parametry prostej
	void setLine(const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion);
	/// Generates a row vector of linearly equally spaced points between x0 and x1
	void LinSpace(double x0, double x1, double *x_out, int N) const;
	/// Oblicza warto�ci na linii w zale�no�ci od zadanego x
	void evalLine(const double * const x,int N, double * const out) const;
	/// zwraca prost� przechodz�c� przez 2 punkty
	KIERUNEK_PROSTEJ getLine2Points(const C_Point &P0, const C_Point &P1);
	/// zwraca punkt przeci�cia okr�gu i prostej
	bool LineCutCircle(const C_Point &P0,double R,C_Point (&out)[2]);
	/// sprawdza czy punkt jest na linii
	bool isPointOnLine(const C_Point &_P0) const;
	/// zwraca punkt r�wnoroz�o�eone na linii pomi�dzy P0 i P1
	virtual bool getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N  ) const;
public:
	/// get a
	double getA() const { return par_a; }
	/// get b
	double getB() const { return par_b; }
	/// get bion
	KIERUNEK_PROSTEJ getjest_pion() const { return jest_pion; }
protected:
	
private:
	double par_a; /**< parametr kierunkowy prostej*/
	double par_b; /**< wyraz wolny*/
	KIERUNEK_PROSTEJ jest_pion; /** < przyjmuje PIONOWA gdy prosta jest pionowa*/
	
};


#endif // C_Line_h__
