/**
 * \file    C_WeldlineDetect.h
 * \brief	Klasa abstrakcyjna obs�oguj�ca wykrywanie linii spawu
 * \author  PB
 * \date    2012/03/01
 */

#ifndef C_WeldlineDetect_h__
#define C_WeldlineDetect_h__

#include <vector>
#include <algorithm>
#include <iostream>
#include "C_Matrix_Container.h"
#include "C_CircBuff.h"
#include "C_LineWeldApprox.h"
#include "C_LineInterp.h"
#include "C_WeldPos.h"

using namespace std;
/** 
 * Klasa abstrakcyjna implementuj�ca metody u�ywane do wykrywania linii spawu. Z tej klasy dziedziczone b�d� klasy do spaw�w liniowych i zakrzywionych. Wektory lineOK oraz WeldPos s� zsynchronizowane, dana pozycja dotyczy okreslonej linii. Dane w WeldPos s� wa�ne tylko jesli w lineOK jest status OK na tej pozycji. Te dwa wektory obejmuj� wszystkie linie spawu - ��cznie z tymi niepoprawnymi. Dost�p do nich poprzez funkcje getLineOK() oraz getweldPos().\n
 * W celu przyspieszenie procedury podczas wype�niania bufora od razu przeliczane s� wyniki aproxymacji i wstawiane do bufora recalculated_approx_data. Ten bufor idzie w parze z dobrymi liniami. Jest u�ywany do wyznaczenia wag w procedurze evalNextParams(). Podczas oblicze� dane s� przechowywane w 3 buforach - s� to obiekty interpolacji->aproxymacji->oraz przeliczone dane na podstawie obiektu aproxymacji. W buforach s� tylko dobre linie, tzn takie kt�re maj� ma�y b��d aproxyamcji. Zrezygnowano z przechowywania przeliczonych dnych w obiekcie aproxymacji aby zaoszcz�dzi� pami��. W ka�dej chwili mo�na te dane jednak obliczy�:
 * \code
  	double *evaldata = new double[_interp->getSizeofInterpData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	const double *x,*y;	// wskazniki na wektory x i y dla kt�rych zosta�a wykonan interpolacja i aproxymacja (wsp�rz�dne obrazu)
	x = _interp->getInterpolated_X();
	y = _interp->getInterpolated_Y();
	_approx->evalApproxFcnVec(y,evaldata,_interp->getSizeofInterpData());
 *	\endcode
 *	\remarks
 *	Generalnie jedn� para zbierznych bufor�w jest lineOK oraz weldPos - te bufory obejmuj� wszystkie linie obrazka dla kt�rych jest liczona aproxymacja. Drugim zestawe s� wymienione wcze�niej bufory ko�owe, kt�re tak�e sa zgodne.\n
 *	Rozmiar danch tez ma znaczneie przy innych spawach niz liniowe. Trzeba pilnowac zeby zawsze tyle smo punkt�w by�o\n
 */
class C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
	friend class C_LinearWeld_Test2;
	friend class C_LinearWeld_FillBuffor;
public:
	/// pobiera obrazek rtg
	C_WeldlineDetect(const C_Matrix_Container *_rtg);
	virtual ~C_WeldlineDetect();
	/// ustawia parametry procedury i tworzy potrzebne struktury
	virtual void SetProcedureParameters(unsigned int _k, C_Point _StartPoint)=0;
	/// Znajduje profil spawu
	virtual bool Start(unsigned int step,unsigned int ile)=0;
	/// zwraca wyniki detekcji spawu
	const vector<bool> *getLineOK() const { return lineOK; }
	/// zwraca wyniki detekcji spawu
	const vector<C_WeldPos> *getweldPos() const { return weldPos; }
	/// oblicza aprxymacje jednej linii
	virtual bool getOneApproxLine(C_WeldPos &weldpos)=0;
protected:
	/// przechowuje wska�nik do obrazka
	const C_Matrix_Container *rtg;
	/// generuje zestaw paramstr�w do detekcji kolejnej linii - punkty P0 i P1 
	virtual bool evalNextStartPoint(unsigned int step)=0;
	/// generuje zestaw kolejnych parametr�w aproxymacji oraz wagi
	virtual bool evalNextParams()=0;
	/// wype�nia bufory approxymuj�c k poczatkowych linii
	virtual bool fillBuffor()=0;
	/// przechowuje k poprzednich wynik�w aproxymacji
	C_CircBuff<C_LineWeldApprox> *approx_results;
	/// przechowuje k ostatnich wynik�w interpolacji linii - dane do aproxymacji i generowania wag
	C_CircBuff<C_LineInterp> *interp_lines;
	/// Bufor pomocniczy przechowuj�cy przeliczone warto�ci funkcji aproxymuj�cej - ma na celu przyspieszenie
	C_CircBuff<double> *recalculated_approx_data;
	/** Przechowuje inforacj� czy linia jest poprawna czy nie - poprawno�� okre�lona na podstwie czyAccept. Jesli linia jest poprawna to jest OK a je�li nie to BLAD*/
	vector<bool> *lineOK;
	/// ilo�� poprzendich wynik�w brana pod uwag�
	unsigned int k;
	/// rozmiar obrazka wej�ciowego potrzebny do funkcji interpoluj�cej
	unsigned int rtgsize[3];
	double *_w; /**< Tablica wag. Jest inicjalizowana jednorazowo w funkcji evalNextParams*/
	/// tablica przechowuj�cej obiekty C_WeldPos
	vector<C_WeldPos> *weldPos;
	/// funkcja sprawdza czy obliczona aproksymacja mo�e by�zaakcepyowana czy nie
	virtual bool czyAccept(const C_LineWeldApprox *_approx, const C_LineInterp *_interp) = 0;
	/// funkcja oblicza pozycjespawu dla danej linii
	virtual void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, const double *_pre, C_WeldPos &_weldPos )=0;
	/// funkcja oblicza pozycjespawu dla danej linii
	virtual void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )=0;
};
#endif // C_WeldlineDetect_h__