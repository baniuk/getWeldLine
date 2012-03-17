/**
 * \file    C_WeldlineDetect.h
 * \brief	Klasa abstrakcyjna obs�oguj�ca wykrywanie linii spawu
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
 * Klasa abstrakcyjna implementuj�ca metody u�ywane do wykrywania linii spawu. Z tej klasy dziedziczone b�d� klasy do spaw�w liniowych i zakrzywionych. Wektory lineOK oraz WeldPos s� zsynchronizowane, dana pozycja dotyczy okreslonej linii. Dane w WeldPos s� wa�ne tylko jesli w lineOK jest status OK na tej pozycji. Te dwa wektory obejmuj� wszystkie linie spawu - ��cznie z tymi niepoprawnymi.
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
	/** Przechowuje inforacj� czy linia jest poprawna czy nie - poprawno�� okre�lona na podstwie czyAccept. Jesli linia jest poprawna to jest OK a je�li nie to BLAD*/
	vector<bool> lineOK;
	/// ilo�� poprzendich wynik�w brana pod uwag�
	unsigned int k;
	/// rozmiar obrazka wej�ciowego potrzebny do funkcji interpoluj�cej
	unsigned int rtgsize[3];
	double *_w; /**< Tablica wag. Jest inicjalizowana jednorazowo w funkcji evalNextParams*/
	/// tablica przechowuj�cej obiekty C_WeldPos
	vector<C_WeldPos> weldPos;
	/// funkcja sprawdza czy obliczona aproksymacja mo�e by�zaakcepyowana czy nie
	virtual bool czyAccept(const C_LineWeldApprox *_approx, const C_LineInterp *_interp) = 0;
	/// funkcja oblicza pozycjespawu dla danej linii
	virtual void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )=0;
};
#endif // C_WeldlineDetect_h__