/**
 * \file    C_LinearWeld.h
 * \brief	Implementuje dekecje spawów liniowych
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_LinearWeld_h__
#define C_LinearWeld_h__

#include "C_WeldlineDetect.h"

class C_LinearWeld : public C_WeldlineDetect
{
	friend class C_LinearWeld_Test1;
public:
	C_LinearWeld(const C_Matrix_Container *_rtg);
	~C_LinearWeld();
	void SetProcedureParameters(unsigned int _k,unsigned int _sp);
	void Start();
protected:
	bool evalNextParams();
	void fillBuffor();
	/// pozycja startowa - dla spawów linowych jedynie kolumna - te punkty zmieniaj¹ siê podczas dzia³ania algorytmu
	C_Point P0;
	C_Point P1;	/**< Pomiêdzy P0 i P1 rozci¹ga siê linia zdejmowania profilu */
};

#endif // C_LinearWeld_h__
