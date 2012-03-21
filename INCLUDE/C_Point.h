/**
 * \file    C_Point.h
 * \brief	Definiuje punkt w przestrzeni
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_Point_h__
#define C_Point_h__


#define _USE_MATH_DEFINES
#include <cmath>

/** 
 * Klasa implementuj¹ca punkt w przestreni dwuwymiarowej
 */ 
class C_Point
{
public:
	C_Point();
	C_Point(double _x, double _y);
	~C_Point();
	/// kopjuje punkt do innego obiektu dest
	void CopyToDest(C_Point &dest) const;
	/// ustawia wartoœæ punktu
	void setPoint(double _x, double _y);
	/// zwraca wartoœci punktów
	void getPoint(double &_x, double &_y) const;
	/// operator kopiownia
	C_Point& operator=(const C_Point &rhs);
	/// operator odejmowania A-=B
	C_Point& operator-=(const C_Point &rhs);
	/// operator dzielenia z podstawieniem A/=B
	C_Point& operator/=(double rhs);
	/// operator mnozenia z podstawieniem A*=B
	C_Point& operator*=(double rhs);
	/// operator dodawania A+=B
	C_Point& operator+=(const C_Point &rhs);
	/// operator dodawania C=A+B
	const C_Point operator+(const C_Point &rhs) const;
	/// operaor odejmowania C = A-B
	const C_Point operator-(const C_Point &rhs) const;
	// get x
	double getX() const { return x0; }
	// get y
	double getY() const { return y0; }
private:
	double x0, y0;	// wspó³rzêdne punktu

};
#endif // C_Point_h__
