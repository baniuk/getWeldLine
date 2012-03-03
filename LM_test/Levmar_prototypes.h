/**
 * \file    Levmar_prototypes.h
 * \brief	Zawiera definicje funkcji wykorzystuj¹cych biblioteke levmar	
 * \details Zawiera definicje funkcji wykorzystuj¹cych biblioteke levmar
 * \author  PB
 * \date    2012/03/01
 */
#ifndef _LEVMAR_PROT_H_
#define _LEVMAR_PROT_H_

#include <levmar.h>
#include <C_Matrix_Container.h>
#include <C_Image_Container.h>
#include <C_Error.h>
#include <math.h>

#ifndef LM_DBL_PREC
#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!
#endif

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
/// High level approximation function - weight data before aproximation
int getLineApproxGaussLinWeighted(C_Matrix_Container *w, C_Matrix_Container *p, C_Matrix_Container *y, C_Matrix_Container *x, C_Matrix_Container *lb, C_Matrix_Container *ub, int iter, double opts[LM_OPTS_SZ], double info[LM_INFO_SZ]);
#endif /* _LEVMAR_PROT_H_ */