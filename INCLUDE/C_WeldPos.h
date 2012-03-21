/**
 * \file    C_WeldPos.h
 * \brief	Definicja klasy przechowuj¹cej pozycjê spawu dla jednej linii		
 * \author  PB
 * \date    2012/03/17
 */
#ifndef C_WeldPos_h__
#define C_WeldPos_h__

#include "C_Point.h"
/** 
 * Kalsa s³u¿y doprzechowywania pozycji spawu dla jednej linii. Przechowywane s¹ trzy punkty:
 * \li Górny
 * \li Œrodkowy
 * \li Dolny\n
 * Klasa musi byæ u¿ywana jako typ tablicowy.
 */ 
class C_WeldPos
{
public:
	C_Point G; /**< Punkt górny */
	C_Point S;/**< Punkt srodkowy */
	C_Point D;/**< Punkt dolny */
	/// Kasuje wszyskie pola
	void Clear();
protected:
private:
};



#endif // C_WeldPos_h__
