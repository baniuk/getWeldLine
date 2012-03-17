/**
 * \file    C_WeldlineDetect.h
 * \brief	Klasa abstrakcyjna obs³oguj¹ca wykrywanie linii spawu
 * \author  PB
 * \date    2012/03/01
 */

#ifndef C_WeldlineDetect_h__
#define C_WeldlineDetect_h__

#include <vector>
#include "C_Matrix_Container.h"
#include "C_CircBuff.h"
#include "C_LineWeldApprox.h"
#include "C_LineInterp.h"
#include "C_WeldPos.h"

using namespace std;
/** 
 * Klasa abstrakcyjna implementuj¹ca metody u¿ywane do wykrywania linii spawu. Z tej klasy dziedziczone bêd¹ klasy do spawów liniowych i zakrzywionych. Wektory lineOK oraz WeldPos s¹ zsynchronizowane, dana pozycja dotyczy okreslonej linii. Dane w WeldPos s¹ wa¿ne tylko jesli w lineOK jest status OK na tej pozycji. Te dwa wektory obejmuj¹ wszystkie linie spawu - ³¹cznie z tymi niepoprawnymi.
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
	virtual void SetProcedureParameters(unsigned int _k, C_Point _StartPoint)=0;
	/// Znajduje profil spawu
	virtual bool Start()=0;
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
	/** Przechowuje inforacjê czy linia jest poprawna czy nie - poprawnoœæ okreœlona na podstwie czyAccept. Jesli linia jest poprawna to jest OK a jeœli nie to BLAD*/
	vector<bool> lineOK;
	/// iloœæ poprzendich wyników brana pod uwagê
	unsigned int k;
	/// rozmiar obrazka wejœciowego potrzebny do funkcji interpoluj¹cej
	unsigned int rtgsize[3];
	double *_w; /**< Tablica wag. Jest inicjalizowana jednorazowo w funkcji evalNextParams*/
	/// tablica przechowuj¹cej obiekty C_WeldPos
	vector<C_WeldPos> weldPos;
	/// funkcja sprawdza czy obliczona aproksymacja mo¿e byæzaakcepyowana czy nie
	virtual bool czyAccept(const C_LineWeldApprox *_approx, const C_LineInterp *_interp) = 0;
	/// funkcja oblicza pozycjespawu dla danej linii
	virtual void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )=0;
};
#endif // C_WeldlineDetect_h__