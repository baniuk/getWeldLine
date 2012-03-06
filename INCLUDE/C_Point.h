#ifndef C_Point_h__
#define C_Point_h__

/** 
 * Klasa implementuj�ca punkt w przestreni dwuwymiarowej
 */ 
#include <math.h>

class C_Point
{
public:
	C_Point();
	C_Point(double _x, double _y);
	~C_Point();
	/// kopjuje punkt do innego obiektu dest
	void CopyToDest(C_Point &dest) const;
	/// ustawia warto�� punktu
	void setPoint(double _x, double _y);
	/// zwraca warto�ci punkt�w
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
private:
	double x0, y0;	// wsp�rz�dne punktu
};
#endif // C_Point_h__
