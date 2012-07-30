/**
 * \file    C_LinearWeld.h
 * \brief	Implementuje dekecje spawów liniowych
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_LinearWeld_h__
#define C_LinearWeld_h__

#include "C_WeldlineDetect.h"

/// maksymalny b³¹d aproxymacji który jest akceptowany. Powy¿ej tego b³êdu dane s¹ odrzucane
#define MAX_ERROR_LEVEL 10000
/// ile rpocent wysokoœci spawu to brzeg
#define WELD_EDGE 0.2

#ifndef BLAD
#define BLAD false
#endif
#ifndef OK
#define OK true
#endif

/** 
 * Klasa implementujaca funkcje do wykrywania spawów liniowych. W ramach obiektu tworzonego z tej klasy realizowana jest pe³na procedura wykrywania. Sposób u¿ycia w testach parametrycznych C_LinearWeld_FillBuffor case1 i inne. Podstaw¹ jest funkcja start(), wyniki s¹ w struktyrach weldPos oraz lineOK. W tych strukturach s¹ wszystkie wyniki dla ka¿dej znalezionej linii. Bufory weldPos oraz lineOK s¹ zbierzne, to jest na odpowiednich pozycjach s¹ odpowiadaj¹ce sobie dane. 
 */
class C_LinearWeld : public C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
	friend class C_LinearWeld_FillBuffor_test2;
	friend class C_LinearWeld_FillBuffor;
public:
	C_LinearWeld(const C_Matrix_Container *_rtg);
	~C_LinearWeld();
	/// Funkcja inicjalizacyjna œrodowiska.
	void SetProcedureParameters(unsigned int _k, C_Point _StartPoint);
	/// g³ówna funkcja startowa
	bool Start(unsigned int step,unsigned int ile);
	/// oblicza aprxymacje jednej linii
	bool getOneApproxLine(C_WeldPos &weldpos);
protected:
	/// generuje nastêpny punkt startowy
	bool evalNextStartPoint(unsigned int step);
	/// wype³nia bufor pocz¹tkowymi aproksymacjami
	bool fillBuffor();
	/// generuje zestaw parametrów do aproksymacji izwraca je
	bool evalNextParams();
	/// sprawdza poprawnoœæ uzyskanej aproxymacji/interpolacji
	bool czyAccept(const C_LineWeldApprox *_approx, const C_LineInterp *_interp);
	/// oblicza pozycjê spawu dla linii pionowej i aproxymacji gaussLin
	void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp,const double *_pre, C_WeldPos &_weldPos );
	/// oblicza pozycjê spawu dla linii pionowej i aproxymacji gaussLin
	void evalWeldPos(const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos );
private:
	/// pozycja startowa - dla spawów linowych jedynie kolumna - te punkty zmieniaj¹ siê podczas dzia³ania algorytmu
	C_Point P0;
	C_Point P1;	/**< Pomiêdzy P0 i P1 rozci¹ga siê linia zdejmowania profilu */
	double _p[5]; /**< Parametry bierz¹ce aproxymacji - tak¿e zmieniaj¹ siê podczas dzia³ania algorytmu */
	double _ub[5];/**< Granice górne bierz¹ce aproxymacji - tak¿e zmieniaj¹ siê podczas dzia³ania algorytmu */
	double _lb[5];/**< Granice dolne bierz¹ce aproxymacji - tak¿e zmieniaj¹ siê podczas dzia³ania algorytmu */
	
};

#endif // C_LinearWeld_h__
