/**
 * \file    C_WeldlineDetect.cpp
 * \brief	Implementuje dekecje spawów liniowych
 * \author  PB
 * \date    2012/03/01
 * \version 1.0
 */
#include "C_WeldlineDetect.h"


C_WeldlineDetect::C_WeldlineDetect( const C_Matrix_Container *_rtg ) :
					rtg(_rtg),
					k(0),
					_w(NULL)
{
	rtgsize[0] = rtg->_rows;
	rtgsize[1] = rtg->_cols;
	rtgsize[2] = 0;
}

C_WeldlineDetect::~C_WeldlineDetect()
{

}
