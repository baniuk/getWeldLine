/**
 * \file    C_LineWeldApprox.h
 * \brief	Aproxymuje profil liniowy obrazu	
 * \author  PB
 * \date    2012/03/01
 */
#ifndef _LineWeldApprox_H_
#define _LineWeldApprox_H_

#include "C_Levmar_Warp.h"


/** 
 * Wykonuje aproxymacjê jednej linii spawu i przechowuje wyniki takiej aproxymacji 
 * Inicjalizacja nastêpuje konstruktorem, przy przekazaniu typu krzywej aproksymujacej oraz wskaŸników do wektórw
 * _x oraz _y. Te wektory nie s¹ modyfikowane wewn¹trz klasy, wektor _y jest kopiowany do zmiennej pomocniczej copy_y
 * co jest potrzebne przy wagach.\n
 * Po inicjalizacji mo¿na wywo³aæ funkcjê C_LineWeldApprox::setApproxParams aby przekazaæ do klasy parmaetry aproksymacji.
 * Jeœli nie wywo³a siê tej funkcji lub poda siê NULL w argumencie to u¿ywane s¹ parametry domyœlne.\n
 * Nastêpnie wywo³uje siê wysokopoziomow¹ procedurê C_LineWeldApprox::getLineApprox, która dokonuje aproksymacji i zachowuje
 * wyniki w klasie. Modyfikowana jest zmienna p, która zawiera obliczone wspó³czynniki krzywej\n
 * Wyniki mo¿na uzyskaæ za pomoc¹ funkcji C_LineWeldApprox::getApproxParams, która kopiuje szereg parametrów na zewnatrz klasy. 
 * Dodatkowe informace za pomoc¹ funkcji C_LineWeldApprox::getInfo.
  */
class C_LineWeldApprox
{
	/// Standard Constructor
	C_LineWeldApprox(eApproxFcn _typeApprox,C_Matrix_Container *_y, C_Matrix_Container *_x);
	/// Destructor
	~C_LineWeldApprox();
	/// High level approximation function - weight data before aproximation
private:
	/// wskaŸnik do danych y \warning Poprzez ten wskaŸnik nie mo¿na modyfikowaæ nic
	C_Matrix_Container const *y;
	/// wskaŸnik do x
	C_Matrix_Container const *x;
	/// kopia y ¿eby wag¹ nie modyfikowaæ
	C_Matrix_Container copy_y;
	/// przechowuje wspó³czynniki dla funkcji GaussLin
	C_Matrix_Container p;
	/// Przechowuje typ aproxymacji skojarzony z obiektem, zgodnie z ApproxFcn
	eApproxFcn typeApprox;
	/// Przechowuje lb
	C_Matrix_Container lb;
	/// Przechowje ub
	C_Matrix_Container ub;
	/// Rodzaj krzywej aproxymuj¹cej
	eApproxFcn currentApproxFcn;
	/// parametry optymalizacji
	double opts[LM_OPTS_SZ];
	/// wyniki optymalizacji
	double info[LM_INFO_SZ];
	/// Aproxymacja sum¹ gaussa i funkcji liniowej
	int getLineApproxGaussLinWeighted(int iter);
	/// ustawia parametry domyœlne dla wybranego typu aproxymacji
	void setDefaultParams();
public:
	/// Aproxymuje jedn¹ liniê wybran¹ funkcj¹ zgodnie z eApproxFcn
	int getLineApprox(int _iter);
	/// ustawia parametry optymalizacji i dane
	void setApproxParmas(C_Matrix_Container *_p, C_Matrix_Container *_w, C_Matrix_Container *_ub, C_Matrix_Container *_lb, double *_opts=NULL);
	/// Zwraca wybrane parametry, bez sprawdzania czy optymalizacja by³a wykoana
	void getApproxParams(C_Matrix_Container *_p, C_Matrix_Container *_ub, C_Matrix_Container *_lb);
	/// Zwraca wybran¹ informacjê o wyniku optymalizacji
	double getInfo(eOptimInfo _res);
};


#endif