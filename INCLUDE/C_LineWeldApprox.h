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
	/// Standard Constructor
	C_LineWeldApprox(eApproxFcn _typeApprox,C_Matrix_Container *_y, C_Matrix_Container *_x);
	/// Destructor
	~C_LineWeldApprox();
	/// High level approximation function - weight data before aproximation
private:
	/// wska�nik do danych y \warning Poprzez ten wska�nik nie mo�na modyfikowa� nic
	C_Matrix_Container const *y;
	/// wska�nik do x
	C_Matrix_Container const *x;
	/// kopia y �eby wag� nie modyfikowa�
	C_Matrix_Container copy_y;
	/// przechowuje wsp�czynniki dla funkcji GaussLin
	C_Matrix_Container p;
	/// Przechowuje typ aproxymacji skojarzony z obiektem, zgodnie z ApproxFcn
	eApproxFcn typeApprox;
	/// Przechowuje lb
	C_Matrix_Container lb;
	/// Przechowje ub
	C_Matrix_Container ub;
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
public:
	/// Aproxymuje jedn� lini� wybran� funkcj� zgodnie z eApproxFcn
	int getLineApprox(int _iter);
	/// ustawia parametry optymalizacji i dane
	void setApproxParmas(C_Matrix_Container *_p, C_Matrix_Container *_w, C_Matrix_Container *_ub, C_Matrix_Container *_lb, double *_opts=NULL);
	/// Zwraca wybrane parametry, bez sprawdzania czy optymalizacja by�a wykoana
	void getApproxParams(C_Matrix_Container *_p, C_Matrix_Container *_ub, C_Matrix_Container *_lb);
	/// Zwraca wybran� informacj� o wyniku optymalizacji
	double getInfo(eOptimInfo _res);
};


#endif