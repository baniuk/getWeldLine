#include "C_WeldlineDetect.h"


C_WeldlineDetect::C_WeldlineDetect( const C_Matrix_Container *_rtg ) :
					rtg(_rtg),
					k(0)
{
	rtgsize[0] = rtg->_rows;
	rtgsize[1] = rtg->_cols;
	rtgsize[2] = 0;
}

C_WeldlineDetect::~C_WeldlineDetect()
{

}
