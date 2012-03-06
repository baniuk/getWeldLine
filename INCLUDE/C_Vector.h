#ifndef C_Vector_h__
#define C_Vector_h__

#include "C_Point.h"
/** 
 * Klasa implementuj¹ca wektor w przestrzeni dwuwymiarowej o pocz¹tku P0
 */ 
class C_Vector
{
	friend class C_VectorTest;
public:
	/// konstruktor domyœlny - tworzy wektor jednostkowy
	C_Vector();
	/// tworzy wektor o pocz¹tku P0 i koñcu P1
	C_Vector(const C_Point &_P0, const C_Point &_P1);
	/// zwraca poczatek i koniec wektora
	void getPoints(C_Point &_P0, C_Point &_P1);
	/// zwraca pocz¹tek, koniec i wspó³rzêdne wektora
	void getVector(C_Point &_P0, C_Point &_P1, C_Point &_VEC) const;
	/// ustawia wektor przy pomocy jego pocz¹tku i wspó³rzêdnych
	void recalculateVectorP0VEC(const C_Point &_P0, const C_Point _VEC);
	/// ustawia wektor przy pomocy jego pocz¹tku koñca
	void recalculateVectorP0P1(const C_Point &_P0, const C_Point _P1);
	/// Ustala nowy punkt przy³o¿enia
	void setNewP0(const C_Point &_P0);
	/// zwraca d³ugoœæ wektora
	double getVectorLen();
	// ustawia wektor wg d³ugoœci, nie zmienia punktu pocz¹tkowego
	void setVectorLen(double len);
	~C_Vector();
private:
	/// oblicza wspó³rzedne wektora i zapisuje zmienna VEC
	C_Point &setVEC(const C_Point &_P0,const C_Point &_P1);
	C_Point &setVEC();
	C_Point VEC;	/**< PRzechowuje wektor jako d³ugoœci x i y*/
	double len;	/**< d³ugoœæ wektora */
	C_Point P0;	// pocz¹tek wektora
	C_Point P1; // koniec wektora
};

#endif // C_Vector_h__