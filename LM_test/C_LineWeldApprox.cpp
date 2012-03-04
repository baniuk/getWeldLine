/**
 * \file    C_LineWeldApprox.cpp
 * \brief	Zbiór podstawowch funkcji do detekcji linii spawu	
 * \details Zbiór podstawowch funkcji do detekcji linii spawu	
 * \author  PB
 * \date    2012/03/01
 */
#include "C_LineWeldApprox.h"

C_LineWeldApprox::C_LineWeldApprox()
{

}
C_LineWeldApprox::~C_LineWeldApprox()
{

}
/** 
 * Approximates one line of image. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size
 * @param[in] w		weights to weight y data (NULL if no wieghts)
 * @param[inout] p	table of parameters
 * @param[in] y		table of data to be fit
 * @param[in] x		values of domain of y. Approximation function is evaluated for these data and fitted to y
 * @param[in] lb	lower bounds - size of m
 * @param[in] ub	upper bounds - size of m
 * @param[in] iter	maximum number of iterations
 * @param[in] opts  minim. options [\tau, \epsilon1, \epsilon2, \epsilon3]. Respectively the scale factor for initial \mu,
 *					stopping thresholds for ||J^T e||_inf, ||Dp||_2 and ||e||_2. Set to NULL for defaults to be used
 * @param[inout] info	information regarding the minimization. Set to NULL if don't care
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
 * @param[in] x		user data - values of domain of x. Approximation function is evaluated for these data and fitted to x
 * \return Number of iterations or -1 if failed
*/
int C_LineWeldApprox::getLineApproxGaussLinWeighted( C_Matrix_Container *w, C_Matrix_Container *p, C_Matrix_Container *y, C_Matrix_Container *x, C_Matrix_Container *lb, C_Matrix_Container *ub, int iter, double opts[LM_OPTS_SZ], double info[LM_INFO_SZ] )
{
	C_Matrix_Container *tmp_w;
	int ret;
	xtradata X;
	if(NULL==w) {
		tmp_w = new C_Matrix_Container(1,y->_cols);
		tmp_w->Ones();
	}
	else
		tmp_w = w;
	y->DotMulti(tmp_w);
	X.x = x->data;
	ret = dlevmar_bc_der(GaussLin,
						jGaussLin,
						p->data,
						y->data,
						p->_cols, y->_cols,
						lb->data,
						ub->data,
						NULL,
						iter, 
						opts,
						info,
						NULL,
						NULL,
						(void*)&X);

	return ret;


}