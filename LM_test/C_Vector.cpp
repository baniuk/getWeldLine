#include "C_Vector.h"

C_Vector::C_Vector(C_Point &P0,C_Point &P1)
{
	// kopiowanie punkt�w pocz�tku i ko�ca do klasy
	P0.CopyToDest(P0);
	P1.CopyToDest(P1);
}

C_Vector::~C_Vector()
{

}