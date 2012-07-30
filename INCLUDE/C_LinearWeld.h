/**
 * \file    C_LinearWeld.h
 * \brief	Implementuje dekecje spaw�w liniowych
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_LinearWeld_h__
#define C_LinearWeld_h__

#include "C_WeldlineDetect.h"

/// maksymalny b��d aproxymacji kt�ry jest akceptowany. Powy�ej tego b��du dane s� odrzucane
#define MAX_ERROR_LEVEL 10000
/// ile rpocent wysoko�ci spawu to brzeg
#define WELD_EDGE 0.2

#ifndef BLAD
#define BLAD false
#endif
#ifndef OK
#define OK true
#endif

/** 
 * Klasa implementujaca funkcje do wykrywania spaw�w liniowych. W ramach obiektu tworzonego z tej klasy realizowana jest pe�na procedura wykrywania. Spos�b u�ycia w testach parametrycznych C_LinearWeld_FillBuffor case1 i inne. Podstaw� jest funkcja start(), wyniki s� w struktyrach weldPos oraz lineOK. W tych strukturach s� wszystkie wyniki dla ka�dej znalezionej linii. Bufory weldPos oraz lineOK s� zbierzne, to jest na odpowiednich pozycjach s� odpowiadaj�ce sobie dane. 
 */
class C_LinearWeld : public C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
	friend class C_LinearWeld_FillBuffor_test2;
	friend class C_LinearWeld_FillBuffor;
public:
	C_LinearWeld(const C_Matrix_Container *_rtg);
	~C_LinearWeld();
	/// Funkcja inicjalizacyjna �rodowiska.
	void SetProcedureParameters(unsigned int _k, C_Point _StartPoint);
	/// g��wna funkcja startowa
	bool Start(unsigned int step,unsigned int ile);
	/// oblicza aprxymacje jednej linii
	bool getOneApproxLine(C_WeldPos &weldpos);
protected:
	/// generuje nast�pny punkt startowy
	bool evalNextStartPoint(unsigned int step);
	/// wype�nia bufor pocz�tkowymi aproksymacjami
	bool fillBuffor();
	/// generuje zestaw parametr�w do aproksymacji izwraca je
	bool evalNextParams();
	/// sprawdza poprawno�� uzyskanej aproxymacji/interpolacji
	bool czyAccept(const C_LineWeldApprox *_approx, const C_LineInterp *_interp);
	/// oblicza pozycj� spawu dla linii pionowej i aproxymacji gaussLin
	void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp,const double *_pre, C_WeldPos &_weldPos );
	/// oblicza pozycj� spawu dla linii pionowej i aproxymacji gaussLin
	void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos );
private:
	/// pozycja startowa - dla spaw�w linowych jedynie kolumna - te punkty zmieniaj� si� podczas dzia�ania algorytmu
	C_Point P0;
	C_Point P1;	/**< Pomi�dzy P0 i P1 rozci�ga si� linia zdejmowania profilu */
	double _p[5]; /**< Parametry bierz�ce aproxymacji - tak�e zmieniaj� si� podczas dzia�ania algorytmu */
	double _ub[5];/**< Granice g�rne bierz�ce aproxymacji - tak�e zmieniaj� si� podczas dzia�ania algorytmu */
	double _lb[5];/**< Granice dolne bierz�ce aproxymacji - tak�e zmieniaj� si� podczas dzia�ania algorytmu */
	
};

#endif // C_LinearWeld_h__
