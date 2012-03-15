/**
 * \file    C_WeldlineDetect.h
 * \brief	Klasa abstrakcyjna obs�oguj�ca wykrywanie linii spawu
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
 * Klasa abstrakcyjna implementuj�ca metody u�ywane do wykrywania linii spawu. Z tej klasy dziedziczone b�d� klasy do spaw�w liniowych i zakrzywionych.
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
	/// przechowuje wska�nik do obrazka
	const C_Matrix_Container *rtg;
	/// generuje zestaw paramstr�w do detekcji kolejnej linii - punkty P0 i P1 
	virtual bool evalNextStartPoint()=0;
	/// generuje zestaw kolejnych parametr�w aproxymacji oraz wagi
	virtual bool evalNextParams()=0;
	/// wype�nia bufory approxymuj�c k poczatkowych linii
	virtual bool fillBuffor()=0;
	/// przechowuje k poprzednich wynik�w aproxymacji
	C_CircBuff<C_LineWeldApprox> approx_results;
	/// przechowuje k ostatnich wynik�w interpolacji linii - dane do aproxymacji i generowania wag
	C_CircBuff<C_LineInterp> interp_lines;
	/// przechowuje k ostatnich b��d�w dopasowania
	C_CircBuff<double> ERR;
	/// ilo�� poprzendich wynik�w brana pod uwag�
	unsigned int k;
	/// rozmiar obrazka wej�ciowego potrzebny do funkcji interpoluj�cej
	unsigned int rtgsize[3];
	double *_w; /**< Tablica wag. Jest inicjalizowana jednorazowo w funkcji evalNextParams*/
};
#endif // C_WeldlineDetect_h__