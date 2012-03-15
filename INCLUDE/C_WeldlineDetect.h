/**
 * \file    C_WeldlineDetect.h
 * \brief	Klasa abstrakcyjna obs³oguj¹ca wykrywanie linii spawu
 * \author  PB
 * \date    2012/03/01
 */

#ifndef C_WeldlineDetect_h__
#define C_WeldlineDetect_h__

#include "C_Matrix_Container.h"
#include "C_CircBuff.h"
#include "C_LineWeldApprox.h"
#include "C_LineInterp.h"

/** 
 * Klasa abstrakcyjna implementuj¹ca metody u¿ywane do wykrywania linii spawu. Z tej klasy dziedziczone bêd¹ klasy do spawów liniowych i zakrzywionych.
 */
class C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
	friend class C_LinearWeld_FillBuffor;
public:
	/// pobiera obrazek rtg
	C_WeldlineDetect(const C_Matrix_Container *_rtg);
	virtual ~C_WeldlineDetect();
	/// ustawia parametry procedury i tworzy potrzebne struktury
	virtual void SetProcedureParameters(unsigned int _k,unsigned int _sp)=0;
	/// Znajduje profil spawu
	virtual void Start()=0;
protected:
	/// przechowuje wskaŸnik do obrazka
	const C_Matrix_Container *rtg;
	/// generuje zestaw paramstrów do detekcji kolejnej linii - punkty P0 i P1 
	virtual bool evalNextStartPoint()=0;
	/// generuje zestaw kolejnych parametrów aproxymacji oraz wagi
	virtual bool evalNextParams()=0;
	/// wype³nia bufory approxymuj¹c k poczatkowych linii
	virtual bool fillBuffor()=0;
	/// przechowuje k poprzednich wyników aproxymacji
	C_CircBuff<C_LineWeldApprox> approx_results;
	/// przechowuje k ostatnich wyników interpolacji linii - dane do aproxymacji i generowania wag
	C_CircBuff<C_LineInterp> interp_lines;
	/// przechowuje k ostatnich b³êdów dopasowania
	C_CircBuff<double> ERR;
	/// iloœæ poprzendich wyników brana pod uwagê
	unsigned int k;
	/// rozmiar obrazka wejœciowego potrzebny do funkcji interpoluj¹cej
	unsigned int rtgsize[3];
	double *_w; /**< Tablica wag. Jest inicjalizowana jednorazowo w funkcji evalNextParams*/
};
#endif // C_WeldlineDetect_h__