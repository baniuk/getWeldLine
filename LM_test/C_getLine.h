
/** 
 * Klasa zwracaj�ca na rz�danie punkty po�o�one na linii pomi�dzy P0 i P1 na obrazie.
 * Wykonywana jest aproxymacja i zwracana jest ��dana ilo�� punkt�w, dla zadanego x
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