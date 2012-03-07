#ifndef C_LineApprox_h__
#define C_LineApprox_h__

#include "C_Line.h"

/// Rodzaje approxymacji
enum APPROX_TYPE {
	SPLINE
};
/** 
 * Klasa realizujaca aproxymacjê danych na obrazie na podstawie linii pomiêdzy punktami
 */
class C_LineApprox : public C_Line
{
public:
	/// domyœlny konstruktor
	C_LineApprox();
	/// konstruktor wykorzystuj¹cy wspó³czynniki prostej
	C_LineApprox(APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion );
	/// konstruktor wykorzystuj¹cy punkty przez które prosta przechodzi
	C_LineApprox(APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1);
	/// 
	virtual bool getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N  ) const;
	virtual ~C_LineApprox();
protected:
	
private:
};

#endif // C_LineApprox_h__
