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
 * Wykonuje aproxymacj� jednej linii spawu i przechowuje wyniki takiej aproxymacji 
 * Inicjalizacja nast�puje konstruktorem, przy przekazaniu typu krzywej aproksymujacej oraz wska�nik�w do wekt�rw
 * _x oraz _y. Te wektory nie s� modyfikowane wewn�trz klasy, wektor _y jest kopiowany do zmiennej pomocniczej copy_y
 * co jest potrzebne przy wagach.\n
 * Po inicjalizacji mo�na wywo�a� funkcj� C_LineWeldApprox::setApproxParams aby przekaza� do klasy parmaetry aproksymacji.
 * Je�li nie wywo�a si� tej funkcji lub poda si� NULL w argumencie to u�ywane s� parametry domy�lne.\n
 * Nast�pnie wywo�uje si� wysokopoziomow� procedur� C_LineWeldApprox::getLineApprox, kt�ra dokonuje aproksymacji i zachowuje
 * wyniki w klasie. Modyfikowana jest zmienna p, kt�ra zawiera obliczone wsp�czynniki krzywej\n
 * Wyniki mo�na uzyska� za pomoc� funkcji C_LineWeldApprox::getApproxParams, kt�ra kopiuje szereg parametr�w na zewnatrz klasy. 
 * Dodatkowe informace za pomoc� funkcji C_LineWeldApprox::getInfo.
  */
class C_LineWeldApprox
{
public:
	/// Standard Constructor - po jego u�yciu obowi�zkowo ManualConstructor
	C_LineWeldApprox();
	/// konstruktor podstawowy
	C_LineWeldApprox(eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len);
	/// Destructor
	~C_LineWeldApprox();
	/// konstruktor manualny - musi by� u�yty po konstruktorze bezparametrowym
	void ManualConstructor(eApproxFcn _typeApprox,const double *_y, const double *_x,unsigned int _len);
	/// oblicza warto�� funkcji apryxymuj�cej dla parametru x
//	double evalApproxFcn(double _x) const;
	/// oblicza warto�� funkcji apryxymuj�cej dla wektora x
	void evalApproxFcnVec(const double *_x,double *_y,unsigned int siz ) const;
private:
	/// wska�nik do danych y \warning Poprzez ten wska�nik nie mo�na modyfikowa� nic
	double const *y;
	/// wska�nik do x
	double const *x;
	/// kopia y �eby wag� nie modyfikowa�
	double *copy_y;
	/// przechowuje wsp�czynniki dla funkcji GaussLin
	double *p;
	/// Przechowuje typ aproxymacji skojarzony z obiektem, zgodnie z ApproxFcn
	eApproxFcn typeApprox;
	/// Przechowuje lb
	double *lb;
	/// Przechowje ub
	double *ub;
	/// Rodzaj krzywej aproxymuj�cej
	eApproxFcn currentApproxFcn;
	/// parametry optymalizacji
	double opts[LM_OPTS_SZ];
	/// wyniki optymalizacji
	double info[LM_INFO_SZ];
	/// Aproxymacja sum� gaussa i funkcji liniowej
	int getLineApproxGaussLinWeighted(int iter);
	/// ustawia parametry domy�lne dla wybranego typu aproxymacji
	void setDefaultParams();
	/// rozmiar wektor�w x i y
	unsigned int len;
	/// wazy sygna� profilu za pomoc� wag
	void WeightProfile(const double *_w);
public:
	/// Aproxymuje jedn� lini� wybran� funkcj� zgodnie z eApproxFcn
	int getLineApprox(int _iter);
	/// ustawia parametry optymalizacji i dane
	void setApproxParmas(double *_p, double *_w, double *_ub, double *_lb, double *_opts=NULL);
	/// Zwraca wybrane parametry, bez sprawdzania czy optymalizacja by�a wykoana
	const double* getApproxParams_p() const;
	const double* getApproxParams_ub() const;
	const double* getApproxParams_lb() const;
	/// Zwraca wybran� informacj� o wyniku optymalizacji
	double getInfo(eOptimInfo _res) const;
	/// Generuje pseudolosowe znaki
	void RangedRand( int range_min, int range_max, int n, char *tab );
};


#endif