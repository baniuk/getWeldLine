/**
 * \file    C_WeldPos.h
 * \brief	Definicja klasy przechowuj�cej pozycj� spawu dla jednej linii		
 * \author  PB
 * \date    2012/03/17
 */
#ifndef C_WeldPos_h__
#define C_WeldPos_h__

#include "C_Point.h"
/** 
 * Kalsa s�u�y doprzechowywania pozycji spawu dla jednej linii. Przechowywane s� trzy punkty:
 * \li G�rny
 * \li �rodkowy
 * \li Dolny\n
 * Klasa musi by� u�ywana jako typ tablicowy.
 */ 
class C_WeldPos
{
public:
	C_Point G; /**< Punkt g�rny */
	C_Point S;/**< Punkt srodkowy */
	C_Point D;/**< Punkt dolny */
	/// Kasuje wszyskie pola
	void Clear();
protected:
private:
};



#endif // C_WeldPos_h__
