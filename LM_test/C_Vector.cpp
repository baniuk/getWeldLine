#include "C_Vector.h"

C_Vector::C_Vector(C_Point &P0,C_Point &P1)
{
	// kopiowanie punktów pocz¹tku i koñca do klasy
	P0.CopyToDest(P0);
	P1.CopyToDest(P1);
}

C_Vector::~C_Vector()
{

}