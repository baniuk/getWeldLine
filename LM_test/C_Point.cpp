#include "C_Point.h"

C_Point::C_Point( double _x, double _y ) :
	x0(_x),
	y0(_y)
{

}

C_Point::C_Point() :
	x0(0),
	y0(0)
{

}

C_Point::~C_Point()
{

}

void C_Point::CopyToDest( C_Point &dest )
{
	dest.x0 = x0;
	dest.y0 = y0;
}

void C_Point::setPoint( double _x, double _y )
{
	x0 = _x;
	y0 = _y;
}

void C_Point::getPoint( double &_x, double &_y )
{
	_x = x0;
	_y = y0;
}
