#include "C_Vector.h"

/** 
 * Tworzy wektor na podstawie punktu zaczepienia i koñca. Wspó³rzêdne wektora s¹ obliczane
 * \param[in] _P0 punkt zaczepienia
 * \param[in] _P1 koniec wektora
 */
C_Vector::C_Vector(const C_Point &_P0, const C_Point &_P1)
{
	// kopiowanie punktów pocz¹tku i koñca do klasy
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

void C_Vector::getPoints( C_Point &_P0, C_Point &_P1 ) const
{
	_P0 = P0;
	_P1 = P1;
}

/** 
 * Wspó³rzêdne wektora - P1 - P0 pocz¹tek odj¹æ koniec
 */
C_Point &C_Vector::setVEC(const C_Point &_P0,const C_Point &_P1 )
{
	VEC = _P1 - _P0;
	return VEC;
}
/** 
 * Wspó³rzêdne wektora - na podstawie danych klasy
 */
C_Point &C_Vector::setVEC()
{
	VEC = P1 - P0;
	return VEC;
}
/** 
 * Zwraca wszystkie dane wektora.
 * \params[out] _P0 pocz¹tek wektora
 * \params[out] _P1 koniec wektora
 * \params[out] _VEC wspó³rzêdne wektora
 */
void C_Vector::getVector( C_Point &_P0, C_Point &_P1, C_Point &_VEC ) const
{
	_P0 = P0;
	_P1 = P1;
	_VEC = VEC;
}
/** 
 * Tworzy wektor na podstawie punktu zaczepienia i jego wspó³rzêdnych. Koniec wektora jest obliczany
 * \param[in] _P0 punkt zaczepienia
 * \param[in] _VEC wspó³rzêdne wektora
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

double C_Vector::getVectorLen() const
{
	double x,y;
	VEC.getPoint(x,y);
	return sqrt(x*x+y*y);
}
/** 
 * Tworzy wersor z wektora i przemna¿a go przez len. NAstêpnie przelicza koniec wektora
 */
void C_Vector::setVectorLen( double len )
{
	double l;
	C_Point tmpVEC;

	tmpVEC = VEC;
	l = getVectorLen();
	tmpVEC/=l;	// wersor
	tmpVEC*=len;	// okreœlona d³ugoœæ
	recalculateVectorP0VEC(P0,tmpVEC);

}

/** 
 * Wektor wynikowy jest zaczpiony w punkcie P0
 */
C_Vector& C_Vector::operator+=( const C_Vector &rhs )
{
	C_Point rP0,rP1,rVEC;
	rhs.getVector(rP0,rP1,rVEC);
	VEC+=rVEC;
	recalculateVectorP0VEC(P0,VEC);
	return *this;
}
/** 
 * Wektor wynikowy jest zaczpiony w punkcie P0
 */
C_Vector& C_Vector::operator-=( const C_Vector &rhs )
{
	C_Point rP0,rP1,rVEC;
	rhs.getVector(rP0,rP1,rVEC);
	VEC-=rVEC;
	recalculateVectorP0VEC(P0,VEC);
	return *this;
}
/** 
 * Wektor wynikowy jest zaczpiony w punkcie P0
 */
const C_Vector C_Vector::operator+( const C_Vector &rhs ) const
{
	C_Vector result = *this;
	result+=rhs;
	return result;
}
/** 
 * Wektor wynikowy jest zaczpiony w punkcie P0
 */
const C_Vector C_Vector::operator-( const C_Vector &rhs ) const
{
	C_Vector result = *this;
	result-=rhs;
	return result;
}

const double C_Vector::operator*( const C_Vector &rhs ) const
{
	C_Point dummy1,dummy2,VEC1,VEC2;
	double x_VEC1,y_VEC1,
		   x_VEC2,y_VEC2;
	this->getVector(dummy1,dummy2,VEC1);
	rhs.getVector(dummy1,dummy2,VEC2);
	VEC1.getPoint(x_VEC1,y_VEC1);
	VEC2.getPoint(x_VEC2,y_VEC2);

	return x_VEC1*x_VEC2 + y_VEC1*y_VEC2;
}

double C_Vector::getAngleDeg( const C_Vector &_V1 ) const
{
	double alfa;
	alfa = (*this*_V1)/(this->getVectorLen()*_V1.getVectorLen());
	return acos(alfa)*180/M_PI;
//	a = (obj*V1)/(obj.GetVectorLen*V1.GetVectorLen);
}


