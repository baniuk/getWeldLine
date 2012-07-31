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
 * Standardowy konstruktor obiektu dokonuj¹cego aproxymacji linii spawu i przechowujacego wynik. Klasa przechowuje wyniki minimalizacji oraz uzyskane parametry.
 * Pozosta³e dane jak w, y, x nie s¹ przechowywane celem zaoszczêdzenia miejsca.
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

	// dodaæ konstruktor pobieraj¹cy obiekt C_LineInterp

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
 * \warning Zawsze uzywane s¹ parametry domyœlne
 * \remarks Funkcja zrzuca dane wejœciowe do dlevmar_bc_der do pliku o losowej nazwie
*/
int C_LineWeldApprox::getLineApproxGaussLinWeighted(int iter)
{
	int ret;
	xtradata X;
	X.x = x;

#if _DEBUG
	char randstring[11];
	srand( (unsigned)time( NULL ) );
	RangedRand(65,91,10,randstring); randstring[10] = 0;
	std::string nazwa_pliku("getLineApproxGaussLinWeighted_");
	std::string Randstring(randstring);
	nazwa_pliku+=Randstring;
	nazwa_pliku+=".out";
	C_DumpAll *dump = new C_DumpAll(nazwa_pliku.c_str());
	// konwersja i zrucanie
	dump->AddEntry(p,5,"p");
	dump->AddEntry(copy_y,len,"copy_y");
	dump->AddEntry(lb,5,"lb");
	dump->AddEntry(ub,5,"ub");
	dump->AddEntry(X.x,len,"x");
	SAFE_DELETE(dump);
#endif

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
 * Ustawia parametry aproxymacji. Zaden z parametrów nie jest modyfikowany. _p, _ub _lb _opts s¹ kopiowane do klasy. Wagi _w s¹ u¿ywane natychmiast do przemno¿enia danych przechowywanych w klasiei u¿ywanych póŸniej do aproxymacji. Jeœli
 * parametry s¹ NULL to u¿yte s¹ parametry domyœlne ustawiane w konstruktorze.\n
 * Zawiera tak¿e procedurê skalowania
 * @param[in] _w weights to weight y data (NULL if no wieghts)
 * @param[in] _p table of parameters
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
		if(NULL!=_p)
			memcpy_s(p,5*sizeof(double),_p,5*sizeof(double));
		if(NULL!=_ub)
			memcpy_s(ub,5*sizeof(double),_ub,5*sizeof(double));
		if(NULL!=_lb)
			memcpy_s(lb,5*sizeof(double),_lb,5*sizeof(double));
		if(NULL!=_opts)
			for (a=0;a<LM_OPTS_SZ;a++)
				opts[a] = _opts[a];
		if(NULL!=_w)
			WeightProfile(_w);
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}
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
 * Zwraca wybrane parmaetry. Jeœli optymalizacja nie by³a wykonana lub funkcja setApproxParams nie by³¹ u¿yta to zwraca domyœlne.
 * \return WskaŸnik do tablicy z parametrami p trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_p() const
{
	return p;
}
/** 
 * Zwraca wybrane parmaetry. Jeœli optymalizacja nie by³a wykonana lub funkcja setApproxParams nie by³¹ u¿yta to zwraca domyœlne.
 * \return WskaŸnik do tablicy z parametrami ub trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_ub() const
{
	return ub;
}
/** 
 * Zwraca wybrane parmaetry. Jeœli optymalizacja nie by³a wykonana lub funkcja setApproxParams nie by³¹ u¿yta to zwraca domyœlne.
 * \return WskaŸnik do tablicy z parametrami lb trzymanymi w obiekcie
*/
const double* C_LineWeldApprox::getApproxParams_lb() const
{
	return lb;
}
/** 
 * Zwraca wartoœæ wybranej pozycji z tablicy info zawieraj¹cej wyniki optymalizacji
 * \param[in] _res	nazwa parametru zgodna z wektor z eOptimInfo
 * \return wartosc parametru o nazwie _res
*/
double C_LineWeldApprox::getInfo( eOptimInfo _res ) const
{
	if(err==_res)
		return sqrt(info[_res]);
	else
		return info[_res];
}
/** 
 * Oblicza wartoœæ funkcji dla parametru x. U¿ywane s¹ dane z wektora _p. Funkcja nie sprawdza poprawnoœci tych danych.
 * \param[in] _x wartoœæ dla której zostanie obliczona funkcja
 * \return Obliczona wartoœæ
 */
// double C_LineWeldApprox::evalApproxFcn( double _x ) const
// {
// 	double y;
// 	double x = _x;
// 	xtradata X;
// 
// 	X.x = &x;
// 	switch(typeApprox)
// 	{
// 	case typeGaussLin:
// 		GaussLin(p, &y, 5, 1, (void*)&X);
// 		break;
// 	default:
// 		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
// 	}
// 	return y;
// }

/** 
 * Oblicza wartoœæ funkcji dla parametru x. U¿ywane s¹ dane z wektora _p. Funkcja nie sprawdza poprawnoœci tych danych.
 * \param[in] _x tablica dla której zostanie obliczona funkcja
 * \param[in] siz rozmiar tablic
 * \param[out] _y tablica wyjœciowa
 */
void C_LineWeldApprox::evalApproxFcnVec(const double *_x,double *_y,unsigned int siz ) const
{
	xtradata X;

	X.x = _x;
	switch(typeApprox)
	{
	case typeGaussLin:
		for(unsigned int l=0;l<siz;l++)
			GaussLin(p, _y, 5, siz, (void*)&X);
		break;
	default:
		_RPTF0(_CRT_ASSERT, "C_LineWeldApprox::Wrong type of approximation\n");
	}
}
/** 
* Procedura wa¿y sygna³ copy_y za pomoc¹ wektora wag _w o tym samym rozmiarze. Funkcja modyfikuje tablicê copy_y. Wagi wchodz¹ jako œrednia nienormowana. Procedura z matlaba z testów
 * \param[in] _w tablica wag
 */
void C_LineWeldApprox::WeightProfile( const double *_w )
{
	double *w_tab = NULL;
	unsigned int l;
	double minel, maxel, delta,wtab;
	w_tab = new double[len];	// w len jest d³ugoœæ danych aproxymowanych
	for(l=0;l<len;++l)
		w_tab[l] = sqrt(_w[l]);
	// skalowanie w_tab do zakresu 0-1
	for(l=0,minel=maxel=w_tab[0];l<len;l++)	{
		if(w_tab[l]<minel)	minel = w_tab[l];
		if(w_tab[l]>maxel) maxel = w_tab[l];
	}
	delta = maxel - minel;
	_ASSERT(delta!=0);
	if(delta!=0)
		for(l=0;l<len;++l)	{
			wtab = (w_tab[l]-minel)/delta; // wartosc wagi przeskalowana do 0-1
			copy_y[l] = copy_y[l]*wtab + _w[l]*(1-wtab); // srednia wa¿ona
		}
	
	SAFE_DELETE(w_tab);

}
/** 
 * Generate random numbers in the half-closed interval [range_min, range_max). In other words,
 * range_min <= random number < range_max 
 * \param[in] range_min min bound
 * \param[in] range_max max bound
 * \param[in] n number of chars
 * \param[out] *tab target table of size n
 */
void C_LineWeldApprox::RangedRand( int range_min, int range_max, int n, char *tab )
{
	int i;
	char u;
	for ( i = 0; i < n; i++ )
	{
		u = (char)((double)rand() / (RAND_MAX + 1) * (range_max - range_min)
			+ range_min);
		tab[i] = u;
	}
}