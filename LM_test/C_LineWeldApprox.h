/**
 * \file    C_LineWeldApprox.h
 * \brief	Aproxymacja spawów liniowych	
 * \details Aproksymuje spawy liniowe
 * \author  PB
 * \date    2012/03/01
 */
#ifndef _LineWeldApprox_H_
#define _LineWeldApprox_H_

#include "C_Levmar_Warp.h"

class C_LineWeldApprox
{
	/// Standard Constructor
	C_LineWeldApprox();
	/// Destructor
	~C_LineWeldApprox();
	/// High level approximation function - weight data before aproximation
	int getLineApproxGaussLinWeighted(C_Matrix_Container *w, C_Matrix_Container *p, C_Matrix_Container *y, C_Matrix_Container *x, C_Matrix_Container *lb, C_Matrix_Container *ub, int iter, double opts[LM_OPTS_SZ], double info[LM_INFO_SZ]);
};


#endif