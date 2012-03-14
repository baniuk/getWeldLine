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

#ifndef BLAD
#define BLAD false
#endif
#ifndef OK
#define OK true
#endif


class C_LinearWeld : public C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
	friend class C_LinearWeld_FillBuffor;
public:
	C_LinearWeld(const C_Matrix_Container *_rtg);
	~C_LinearWeld();
	void SetProcedureParameters(unsigned int _k,unsigned int _sp);
	void Start();
protected:
	/// generuje nast�pny punkt startowy
	bool evalNextStartPoint();
	/// wype�nia bufor pocz�tkowymi aproksymacjami
	bool fillBuffor();
	/// generuje zestaw parametr�w do aproksymacji
	bool evalNextParams();
	/// pozycja startowa - dla spaw�w linowych jedynie kolumna - te punkty zmieniaj� si� podczas dzia�ania algorytmu
	C_Point P0;
	C_Point P1;	/**< Pomi�dzy P0 i P1 rozci�ga si� linia zdejmowania profilu */
};

#endif // C_LinearWeld_h__
