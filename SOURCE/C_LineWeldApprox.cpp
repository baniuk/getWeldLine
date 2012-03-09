/**
 * \file    C_LineWeldApprox.cpp
 * \brief	Zbiór podstawowch funkcji do detekcji linii spawu	
 * \details Zbiór podstawowch funkcji do detekcji linii spawu	
 * \author  PB
 * \date    2012/03/01
 */
#include "C_LineWeldApprox.h"

/** 
 * Standardowy konstruktor obiektu dokonuj¹cego aproxymacji linii spawu i przechowujacego wynik. Klasa przechowuje wyniki minimalizacji oraz uzyskane parametry.
 * Pozosta³e dane jak w, y, x nie s¹ przechowywane celem zaoszczêdzenia miejsca.
 * \param[in] ApproxFcn		Rodzaj krzywej do aproxymacji - zgodne z eApproxFcn
 * \param[in] _y		table of data to be fit
 * \param[in] _x		values of domain of y. Approximation function is evaluated for these data and fitted to y
 */
C_LineWeldApprox::C_LineWeldApprox(eApproxFcn _typeApprox,C_Matrix_Container *_y, C_Matrix_Container *_x)
	:
	typeApprox(_typeApprox),
	x(_x),
	y(_y)
{
	y->CloneObject(&copy_y);
	switch(typeApprox)
	{
		case typeGaussLin:
			p.AllocateData(1,5);
			ub.AllocateData(1,5);
			lb.AllocateData(1,5);
			setDefaultParams(); 
			break;
		default:
			_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}

}
	// dodaæ konstruktor pobieraj¹cy obiekt C_LineInterp

C_LineWeldApprox::~C_LineWeldApprox()
{

}

/** 
 * Approximates one line of image. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size. Require to call method C_LineWeldApprox::setApproxPAram before
 * @param[in] iter	maximum number of iterations			
 * \return Number of iterations or -1 if failed
*/
int C_LineWeldApprox::getLineApprox(int _iter)
{
	int ret;
	switch(typeApprox)
	{
	case typeGaussLin:
		ret = getLineApproxGaussLinWeighted(_iter);
	}
	return ret;
}
/** 
 * Internal aproximation by Sum of gauss and linear function. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size
 * @param[in] iter	maximum number of iterations			
 * \return Number of iterations or -1 if failed
 * \warning Zawsze uzywane s¹ parametry domyœlne
*/
int C_LineWeldApprox::getLineApproxGaussLinWeighted(int iter)
{
	int ret;
	xtradata X;
	X.x = x->data;
	ret = dlevmar_bc_der(GaussLin,
						jGaussLin,
						p.data,
						y->data,
						p._cols, y->_cols,
						lb.data,
						ub.data,
						NULL,
						iter, 
						NULL, //opts,
						info,
						NULL,
						NULL,
						(void*)&X);

	return ret;


}
/** 
 * Ustawia parametry aproxymacji. Zaden z parametrów nie jest modyfikowany. _p, _ub _lb _opts s¹ kopiowane do klasy. Jeœli
 * parametry s¹ NULL to u¿yte s¹ parametry domyœlne ustawiane w konstruktorze
 * @param[in] w		weights to weight y data (NULL if no wieghts)
 * @param[inout] p	table of parameters
 * @param[in] lb	lower bounds - size of m
 * @param[in] ub	upper bounds - size of m
 * @param[in] opts  minim. options [\tau, \epsilon1, \epsilon2, \epsilon3]. Respectively the scale factor for initial \mu,
 *					stopping thresholds for ||J^T e||_inf, ||Dp||_2 and ||e||_2. Set to NULL for defaults to be used
*/

void C_LineWeldApprox::setApproxParmas( C_Matrix_Container *_p, C_Matrix_Container *_w, C_Matrix_Container *_ub, C_Matrix_Container *_lb, double *_opts )
{
	int a;
	if(NULL!=_w)
		copy_y.DotMulti(_w);
	if(NULL!=_p)
		_p->CloneObject(&p);
	if(NULL!=_ub)
		_ub->CloneObject(&ub);
	if(NULL!=_lb)
		_lb->CloneObject(&lb);
	if(NULL!=_opts)
		for (a=0;a<LM_OPTS_SZ;a++)
			opts[a] = _opts[a];

}
/** 
 * Ustawia domyœlne parametry wzale¿noœci od typu krzywej u¿ytej do aproksymacji.
 * \warning Parmetr C_LineWeldApprox::opts nie jest u¿ywane w tej wersji
 */
void C_LineWeldApprox::setDefaultParams()
{
	switch(typeApprox)
	{
	case typeGaussLin:
		p.data[A] = 1060;
		p.data[B] = x->_cols/2;
		p.data[C] = 160;
		p.data[D] = p.data[E] = 0;

		ub.data[A] = 65535;
		ub.data[B] = p.data[B] + x->_cols/4;
		ub.data[C] = 600;
		ub.data[D] = 1;
		ub.data[E] = 20000;

		ub.data[A] = 0;
		ub.data[B] = p.data[B] - x->_cols/4;
		ub.data[C] = 50;
		ub.data[D] = -1;
		ub.data[E] = -20000;
	}
}
/** 
 * Zwraca wybrane parmaetry. Jeœli optymalizacja nie by³a wykonana lub funkcja setApproxParams nie by³¹ u¿yta to zwraca domyœlne.
 * Funkcja kasuje i inicjalizuje wartoœci parametrów wejœciowych 
 * \param[out] _p	wektor z parametrami, jeœli jest NULL to nie zwraca
 * \param[out] _ub	wektor z górna granic¹ parametrów, jeœli jest NULL to nie zwraca
 * \param[out] _lb	wektor z dolna granic¹ parametrów, jeœli jest NULL to nie zwraca
*/
void C_LineWeldApprox::getApproxParams( C_Matrix_Container *_p, C_Matrix_Container *_ub, C_Matrix_Container *_lb )
{
	switch(typeApprox)
	{
	case typeGaussLin:
		if(NULL!=_p)
		{
			_p->AllocateData(1,5);
			p.CloneObject(_p);
		}
		if(NULL!=_ub)
		{
			_ub->AllocateData(1,5);
			ub.CloneObject(_ub);
		}
		if(NULL!=_lb)
		{
			_lb->AllocateData(1,5);
			lb.CloneObject(_lb);
		}
	}
}
/** 
 * Zwraca wartoœæ wybranej pozycji z tablicy info zawieraj¹cej wyniki optymalizacji
 * \param[in] res	nazwa parametru zgodna z wektor z eOptimInfo
 * \return wartosc parametru o nazwie _res
*/
double C_LineWeldApprox::getInfo( eOptimInfo _res )
{
	return info[_res];
}
