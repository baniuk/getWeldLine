#ifndef C_Vector_h__
#define C_Vector_h__

#include "C_Point.h"
/** 
 * Klasa implementuj�ca wektor w przestrzeni dwuwymiarowej o pocz�tku P0
 */ 
class C_Vector
{
	friend class C_VectorTest;
public:
	/// konstruktor domy�lny - tworzy wektor jednostkowy
	C_Vector();
	/// tworzy wektor o pocz�tku P0 i ko�cu P1
	C_Vector(const C_Point &_P0, const C_Point &_P1);
	/// zwraca poczatek i koniec wektora
	void getPoints(C_Point &_P0, C_Point &_P1) const;
	/// zwraca pocz�tek, koniec i wsp�rz�dne wektora
	void getVector(C_Point &_P0, C_Point &_P1, C_Point &_VEC) const;
	/// ustawia wektor przy pomocy jego pocz�tku i wsp�rz�dnych
	void recalculateVectorP0VEC(const C_Point &_P0, const C_Point _VEC);
	/// ustawia wektor przy pomocy jego pocz�tku ko�ca
	void recalculateVectorP0P1(const C_Point &_P0, const C_Point _P1);
	/// Ustala nowy punkt przy�o�enia
	void setNewP0(const C_Point &_P0);
	/// zwraca d�ugo�� wektora
	double getVectorLen() const;
	/// ustawia wektor wg d�ugo�ci, nie zmienia punktu pocz�tkowego
	void setVectorLen(double len);
	/// zwraca k�t pomi�dzy wektorami w stopniach
	double getAngleDeg(const C_Vector &_V1) const;
	/// operator odejmowania A-=B
	C_Vector& operator-=(const C_Vector &rhs);
	/// operator dodawania A+=B
	C_Vector& operator+=(const C_Vector &rhs);
	/// operator dodawania C=A+B
	const C_Vector operator+(const C_Vector &rhs) const;
	/// operaor odejmowania C = A-B
	const C_Vector operator-(const C_Vector &rhs) const;
	/// operator mnozenia skalarnego
	const double operator*(const C_Vector &rhs) const;
	~C_Vector();
private:
	/// oblicza wsp�rzedne wektora i zapisuje zmienna VEC
	C_Point &setVEC(const C_Point &_P0,const C_Point &_P1);
	C_Point &setVEC();
	C_Point VEC;	/**< PRzechowuje wektor jako d�ugo�ci x i y*/
	double len;	/**< d�ugo�� wektora */
	C_Point P0;	// pocz�tek wektora
	C_Point P1; // koniec wektora
};

#endif // C_Vector_h__