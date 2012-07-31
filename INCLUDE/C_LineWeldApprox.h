/**
 * \file    C_LineWeldApprox.h
 * \brief	Aproxymuje profil liniowy obrazu	
 * \author  PB
 * \date    2012/03/01
 */
#ifndef _LineWeldApprox_H_
#define _LineWeldApprox_H_

#include "C_Levmar_Warp.h"
#include <string>
#include "C_DumpAll.h"
#include <time.h>

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
public:
	/// Standard Constructor - po jego u¿yciu obowi¹zkowo ManualConstructor
	C_LineWeldApprox();
	/// konstruktor podstawowy
	C_LineWeldApprox(eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len);
	/// Destructor
	~C_LineWeldApprox();
	/// konstruktor manualny - musi byæ u¿yty po konstruktorze bezparametrowym
	void ManualConstructor(eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len);
	/// oblicza wartoœæ funkcji apryxymuj¹cej dla parametru x
//	double evalApproxFcn(double _x) const;
	/// oblicza wartoœæ funkcji apryxymuj¹cej dla wektora x
	void evalApproxFcnVec(const double *_x,double *_y,unsigned int siz ) const;
private:
	/// wskaŸnik do danych y \warning Poprzez ten wskaŸnik nie mo¿na modyfikowaæ nic
	double const *y;
	/// wskaŸnik do x
	double const *x;
	/// kopia y ¿eby wag¹ nie modyfikowaæ
	double *copy_y;
	/// przechowuje wspó³czynniki dla funkcji GaussLin
	double *p;
	/// Przechowuje typ aproxymacji skojarzony z obiektem, zgodnie z ApproxFcn
	eApproxFcn typeApprox;
	/// Przechowuje lb
	double *lb;
	/// Przechowje ub
	double *ub;
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
	/// rozmiar wektorów x i y
	unsigned int len;
	/// wazy sygna³ profilu za pomoc¹ wag
	void WeightProfile(const double *_w);
public:
	/// Aproxymuje jedn¹ liniê wybran¹ funkcj¹ zgodnie z eApproxFcn
	int getLineApprox(int _iter);
	/// ustawia parametry optymalizacji i dane
	void setApproxParmas(double *_p, double *_w, double *_ub, double *_lb, double *_opts=NULL);
	/// Zwraca wybrane parametry, bez sprawdzania czy optymalizacja by³a wykoana
	const double* getApproxParams_p() const;
	const double* getApproxParams_ub() const;
	const double* getApproxParams_lb() const;
	/// Zwraca wybran¹ informacjê o wyniku optymalizacji
	double getInfo(eOptimInfo _res) const;
	/// Generuje pseudolosowe znaki
	void RangedRand( int range_min, int range_max, int n, char *tab );
};


#endif