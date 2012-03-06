
/** 
 * Klasa zwracaj¹ca na rz¹danie punkty po³o¿one na linii pomiêdzy P0 i P1 na obrazie.
 * Wykonywana jest aproxymacja i zwracana jest ¿¹dana iloœæ punktów, dla zadanego x
 * Wzorowana na complexapproxlin.m -> getProfile
 */ 
#ifndef _getLine_H_
#define _getLine_H_

#include <C_Matrix_Container.h>
#include <C_Image_Container.h>
#include <C_Error.h>
#include <math.h>

class C_getLine
{
public:
	C_getLine();
	~C_getLine();
};

#endif