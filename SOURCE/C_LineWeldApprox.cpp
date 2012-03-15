/**
 * \file    C_LineWeldApprox.cpp
 * \brief	Zbi�r podstawowch funkcji do detekcji linii spawu	
 * \details Zbi�r podstawowch funkcji do detekcji linii spawu	
 * \author  PB
 * \date    2012/03/01
 */
#include "C_LineWeldApprox.h"

/** 
 * Standardowy konstruktor obiektu dokonuj�cego aproxymacji linii spawu i przechowujacego wynik. Klasa przechowuje wyniki minimalizacji oraz uzyskane parametry.
 * Pozosta�e dane jak w, y, x nie s� przechowywane celem zaoszcz�dzenia miejsca.
 * \param[in] _typeApprox Rodzaj krzywej do aproxymacji - zgodne z eApproxFcn
 * \param[in] _y table of data to be fit
 * \param[in] _x values of domain of y. Approximation function is evaluated for these data and fitted to y
 * \param[in] _len danych x i y
 */
C_LineWeldApprox::C_LineWeldApprox(eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len)
{
	ManualConstructor( _typeApprox,_y, _x, _len );
}

C_LineWeldApprox::C_LineWeldApprox()
{
	typeApprox = none;
	len = 0;
	p = copy_y = ub = lb = NULL;

}
/** 
 * Standardowy konstruktor obiektu dokonuj�cego aproxymacji linii spawu i przechowujacego wynik. Klasa przechowuje wyniki minimalizacji oraz uzyskane parametry.
 * Pozosta�e dane jak w, y, x nie s� przechowywane celem zaoszcz�dzenia miejsca.
 * \param[in] _typeApprox Rodzaj krzywej do aproxymacji - zgodne z eApproxFcn
 * \param[in] _y table of data to be fit
 * \param[in] _x values of domain of y. Approximation function is evaluated for these data and fitted to y
 * \param[in] _len danych x i y
 */
void C_LineWeldApprox::ManualConstructor( eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len )
{
	typeApprox = _typeApprox;
	x = _x;
	y = _y;
	len = _len;
	copy_y = new double[len];
	memcpy_s(copy_y,len*sizeof(double),_y,len*sizeof(double));
	switch(typeApprox)
	{
	case typeGaussLin:
		p = new double[5];
		ub = new double[5];
		lb = new double[5];
		setDefaultParams(); 
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}

}

	// doda� konstruktor pobieraj�cy obiekt C_LineInterp

C_LineWeldApprox::~C_LineWeldApprox()
{
	SAFE_DELETE(copy_y);
	SAFE_DELETE(lb);
	SAFE_DELETE(ub);
	SAFE_DELETE(p);
}

/** 
 * Approximates one line of image. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size. Require to call method C_LineWeldApprox::setApproxPAram before
 * @param[in] _iter	maximum number of iterations			
 * \return Number of iterations or -1 if failed
*/
int C_LineWeldApprox::getLineApprox(int _iter)
{
	int ret;
	switch(typeApprox)
	{
	case typeGaussLin:
		ret = getLineApproxGaussLinWeighted(_iter);
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}
	return ret;
}
/** 
 * Internal aproximation by Sum of gauss and linear function. Approximation functions are evaluated for vector xtradata::x->x and then its output values are compared to 
 * vector y. Vector y and xtradata::x->x are the same size
 * @param[in] iter	maximum number of iterations			
 * \return Number of iterations or -1 if failed
 * \warning Zawsze uzywane s� parametry domy�lne
*/
int C_LineWeldApprox::getLineApproxGaussLinWeighted(int iter)
{
	int ret;
	xtradata X;
	X.x = x;
	ret = dlevmar_bc_der(GaussLin,
						jGaussLin,
						p,
						copy_y,
						5, len,
						lb,
						ub,
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
 * Ustawia parametry aproxymacji. Zaden z parametr�w nie jest modyfikowany. _p, _ub _lb _opts s� kopiowane do klasy. Je�li
 * parametry s� NULL to u�yte s� parametry domy�lne ustawiane w konstruktorze
 * @param[in] _w		weights to weight y data (NULL if no wieghts)
 * @param[in] _p	table of parameters
 * @param[in] _lb lower bounds - size of m
 * @param[in] _ub upper bounds - size of m
 * @param[in] _opts minim. options \latexonly [\tau, \epsilon_1, \epsilon_2, \epsilon_3]. Respectively the scale factor for initial \mu,stopping thresholds for ||J^T e||_inf, ||Dp||_2 and ||e||_2. \endlatexonly
 * Set to NULL for defaults to be used
*/
void C_LineWeldApprox::setApproxParmas( double *_p, double *_w, double *_ub, double *_lb, double *_opts )
{
	int a;
	switch(typeApprox)
	{
	case typeGaussLin:
		if(NULL!=_w)
			for(unsigned a=0;a<len;a++) copy_y[a]*=_w[a];
		if(NULL!=_p)
			memcpy_s(p,5*sizeof(double),_p,5*sizeof(double));
		if(NULL!=_ub)
			memcpy_s(ub,5*sizeof(double),_ub,5*sizeof(double));
		if(NULL!=_lb)
			memcpy_s(lb,5*sizeof(double),_lb,5*sizeof(double));
		if(NULL!=_opts)
			for (a=0;a<LM_OPTS_SZ;a++)
				opts[a] = _opts[a];
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}

}
/** 
 * Ustawia domy�lne parametry wzale�no�ci od typu krzywej u�ytej do aproksymacji.
 * \warning Parmetr C_LineWeldApprox::opts nie jest u�ywane w tej wersji
 */
void C_LineWeldApprox::setDefaultParams()
{
	switch(typeApprox)
	{
	case typeGaussLin:
		p[A] = 1060;
		p[B] = len/2;
		p[C] = 160;
		p[D] = p[E] = 0;

		ub[A] = 65535;
		ub[B] = p[B] + len/4;
		ub[C] = 600;
		ub[D] = 1;
		ub[E] = 20000;

		lb[A] = 0;
		lb[B] = p[B] - len/4;
		lb[C] = 50;
		lb[D] = -1;
		lb[E] = -20000;
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}
}
/** 
 * Zwraca wybrane parmaetry. Je�li optymalizacja nie by�a wykonana lub funkcja setApproxParams nie by�� u�yta to zwraca domy�lne.
 * \return Wska�nik do tablicy z parametrami p trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_p()
{
	return p;
}
/** 
 * Zwraca wybrane parmaetry. Je�li optymalizacja nie by�a wykonana lub funkcja setApproxParams nie by�� u�yta to zwraca domy�lne.
 * \return Wska�nik do tablicy z parametrami ub trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_ub()
{
	return ub;
}
/** 
 * Zwraca wybrane parmaetry. Je�li optymalizacja nie by�a wykonana lub funkcja setApproxParams nie by�� u�yta to zwraca domy�lne.
 * \return Wska�nik do tablicy z parametrami lb trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_lb()
{
	return lb;
}
/** 
 * Zwraca warto�� wybranej pozycji z tablicy info zawieraj�cej wyniki optymalizacji
 * \param[in] _res	nazwa parametru zgodna z wektor z eOptimInfo
 * \return wartosc parametru o nazwie _res
*/
double C_LineWeldApprox::getInfo( eOptimInfo _res )
{
	if(err==_res)
		return sqrt(info[_res]);
	else
		return info[_res];
}


