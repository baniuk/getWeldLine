/**
 * \file    C_Levmar_Warp.cpp
 * \brief	Warper do biblioteki levmar	
 * \details Zawiera definicje funkcji wykorzystuj¹cych biblioteke levmar
 * \author  PB
 * \date    2012/03/01
 */
#include "C_Levmar_Warp.h"

/** 
 * Evaluates approximation function defined by sum of Gauss and Linear function
 * \latexonly
 * [tex]y(x) = A*e^{-{\frac{x-B}/{C}}^2}+D*x+E[\tex]
 * \endlatexonly
 Expression is evaluated in domain x passed through struct data
 * @param[in] p		table of parameters
 * @param[out] y	table of evaluated data
 * @param[in] m		size of p
 * @param[in] n		size of y
 * @param[in] data	user data - values of domain of size n
*/
void GaussLin(double *p, double *y, int m, int n, void *data)
{
register int i; //i - counter
struct xtradata *dat;

  dat = (struct xtradata *)data;
  /* user-supplied data can now be accessed as dat->msg, etc */

  for(i=0; i<n; i++){
	  y[i] = p[0]*exp(-pow((dat->x[i]-p[1])/p[2],2) )+p[3]*dat->x[i]+p[4];
  }
}

/** 
 Evaluates jackobian of the approximation function defined by sum of Gauss and Linear function\n
 y = p[0]*exp(-((x-p[1])/p[2]).^2)+p[3]*x+p[4];\n
 Expression is evaluated in domain x passed throught struct data
 * @param[in] p		table of parameters
 * @param[out] jac	table of evaluated data
 * @param[in] m		
 * @param[in] n		size of jac
 * @param[in] data	user data - values of domain of size n
*/
void jGaussLin(double *p, double *jac, int m, int n, void *data)
{   
register int i,j; // j - index in jacobian matrix
struct xtradata *dat;

  dat=(struct xtradata *)data;
  /* user-supplied data can now be accessed as dat->msg, etc */

  /* fill Jacobian row by row */
  for(i=j=0; i<n; i++){
    jac[j++] = exp(-pow((dat->x[i]-p[1])/p[2],2));	// pochodna po p0
    jac[j++] = -p[0]*2*(p[1]-dat->x[i])/
			   (p[2]*p[2]*exp(pow((p[1]-dat->x[i])/p[2],2))); // pochodna po p1
    jac[j++]= (2*p[0]*(p[1]-dat->x[i])*(p[1]-dat->x[i]))/
			  (p[2]*p[2]*p[2]*exp(pow((p[1]-dat->x[i])/p[2],2)));	// pochodnia po p2
	jac[j++] = dat->x[i];// pochodna po p3
	jac[j++] = 1;//pochodna po p4
  }
}

/** 
 * Approximates one line of image. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size
 * @param[in,out] p		table of parameters
 * @param[in] y	table of data to be fit
 * @param[in] m		parameter vector dimension
 * @param[in] n		measurement vector dimension - size of x
 * @param[in] lb	lower bounds - size of m
 * @param[in] ub	upper bounds - size of m
 * @param[in] iter	maximum number of iterations
 * @param[in] opts  minim. options [\tau, \epsilon1, \epsilon2, \epsilon3]. Respectively the scale factor for initial \mu,
 *					stopping thresholds for ||J^T e||_inf, ||Dp||_2 and ||e||_2. Set to NULL for defaults to be used
 * @param[in,out] info	information regarding the minimization. Set to NULL if don't care
 *					info[0]= ||e||_2 at initial p.
 *					info[1-4]=[ ||e||_2, ||J^T e||_inf,  ||Dp||_2, \mu/max[J^T J]_ii ], all computed at estimated p.
 *					info[5]= # iterations,
 *					info[6]=reason for terminating: 
 *									1 - stopped by small gradient J^T e
 *									2 - stopped by small Dp
 *									3 - stopped by itmax
 *									4 - singular matrix. Restart from current p with increased \mu
 *									5 - no further error reduction is possible. Restart with increased mu
 *									6 - stopped by small ||e||_2
 *									7 - stopped by invalid (i.e. NaN or Inf) "func" values; a user error
 *					info[7]= # function evaluations
 *					info[8]= # Jacobian evaluations
 *					info[9]= # linear systems solved, i.e. # attempts for reducing error					
 * @param[in] x		user data - values of domain of y. Approximation function is evaluated for these data and fitted to y
 * \return Number of iterations or -1 if failed
*/
int getLineApproxGaussLin(	double *p,
							double *y,
							int m, int n,
							double *lb, double *ub,
							int iter,
							double opts[LM_OPTS_SZ], double info[LM_INFO_SZ],
							xtradata &x )
{
	int ret;
	ret = dlevmar_bc_der(GaussLin,
						 jGaussLin,
						 p,
						 y,
						 m, n,
						 lb,
						 ub,
						 NULL,
						 iter, 
						 opts,
						 info,
						 NULL,
						 NULL,
						 (void*)&x);
	return ret;
}
