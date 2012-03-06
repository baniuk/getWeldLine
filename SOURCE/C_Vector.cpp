#include "C_Vector.h"

/** 
 * Tworzy wektor na podstawie punktu zaczepienia i ko�ca. Wsp�rz�dne wektora s� obliczane
 * \param[in] _P0 punkt zaczepienia
 * \param[in] _P1 koniec wektora
 */
C_Vector::C_Vector(const C_Point &_P0, const C_Point &_P1)
{
	// kopiowanie punkt�w pocz�tku i ko�ca do klasy
	_P0.CopyToDest(P0);
	_P1.CopyToDest(P1);
	setVEC(P0,P1);
}

C_Vector::C_Vector()
{
	P0.setPoint(0,0);
	P1.setPoint(1,0);
	setVEC(P0,P1);
}

C_Vector::~C_Vector()
{

}

void C_Vector::getPoints( C_Point &_P0, C_Point &_P1 )
{
	_P0 = P0;
	_P1 = P1;
}

/** 
 * Wsp�rz�dne wektora - P1 - P0 pocz�tek odj�� koniec
 */
C_Point &C_Vector::setVEC(const C_Point &_P0,const C_Point &_P1 )
{
	VEC = _P1 - _P0;
	return VEC;
}
/** 
 * Wsp�rz�dne wektora - na podstawie danych klasy
 */
C_Point &C_Vector::setVEC()
{
	VEC = P1 - P0;
	return VEC;
}
/** 
 * Zwraca wszystkie dane wektora.
 * \params[out] _P0 pocz�tek wektora
 * \params[out] _P1 koniec wektora
 * \params[out] _VEC wsp�rz�dne wektora
 */
void C_Vector::getVector( C_Point &_P0, C_Point &_P1, C_Point &_VEC ) const
{
	_P0 = P0;
	_P1 = P1;
	_VEC = VEC;
}
/** 
 * Tworzy wektor na podstawie punktu zaczepienia i jego wsp�rz�dnych. Koniec wektora jest obliczany
 * \param[in] _P0 punkt zaczepienia
 * \param[in] _VEC wsp�rz�dne wektora
 */
void C_Vector::recalculateVectorP0VEC(const C_Point &_P0,const C_Point _VEC )
{
	P0 = _P0;
	P1 = P0 + _VEC;
	setVEC();
}

void C_Vector::recalculateVectorP0P1(const C_Point &_P0,const C_Point _P1 )
{
	P0 = _P0;
	P1 = _P1;
	setVEC();
}

void C_Vector::setNewP0( const C_Point &_P0 )
{
	P0 = _P0;
	P1 = P0 + VEC;
}

double C_Vector::getVectorLen()
{
	double x,y;
	VEC.getPoint(x,y);
	return sqrt(x*x+y*y);
}
/** 
 * Tworzy wersor z wektora i przemna�a go przez len. NAst�pnie przelicza koniec wektora
 */
void C_Vector::setVectorLen( double len )
{
	double l;
	C_Point tmpVEC;

	tmpVEC = VEC;
	l = getVectorLen();
	tmpVEC/=l;	// wersor
	tmpVEC*=len;	// okre�lona d�ugo��
	recalculateVectorP0VEC(P0,tmpVEC);

}
