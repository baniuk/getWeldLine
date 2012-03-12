/**
 * \file    C_Levmar_Warp.h
 * \brief	Warper do biblioteki levmar		
 * \details Zawiera definicje funkcji wykorzystuj¹cych biblioteke levmar
 * \author  PB
 * \date    2012/03/01
 */
#ifndef _C_Levmar_Warp_H_
#define _C_Levmar_Warp_H_

#include <levmar.h>
#include <C_Matrix_Container.h>
#include <C_Image_Container.h>
#include <C_Error.h>
#include <math.h>

#ifndef LM_DBL_PREC
#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!
#endif

/// Typy aproxymacji
enum eApproxFcn {
	typeGaussLin = 1,
	none
}; 
/// Nazwy parametrów dla funkcji Gauss+Lin
enum eParNamesGaussLin {
	A = 0,
	B,
	C,
	D,
	E
};
/// Nazwy wyników optymalizacji
enum eOptimInfo {
	par0 = 0,
	err,
	par2,
	par3,
	par4,
	iterations,
	stopreason,
	funceval,
	jacevals,
	linsolves
};
/** 
  \brief struct to pass function domain to it
 * Variable X is defined as const to prevent modifications throught the structure. It is read-only */
struct xtradata{
    const double *x;	/** pointer to table with data, always size of n*/
};

/// model to be fitted - Gauss + Linear
void GaussLin(double *p, double *y, int m, int n, void *data);
/// Jackobian of GaussLin
void jGaussLin(double *p, double *jac, int m, int n, void *data);
/// Low level approximation function - approximates one line of image
int getLineApproxGaussLin( double *p, double *y, int m, int n, double *lb, double *ub, int iter, double opts[LM_OPTS_SZ], double info[LM_INFO_SZ], xtradata &x);
#endif /* _LEVMAR_PROT_H_ */