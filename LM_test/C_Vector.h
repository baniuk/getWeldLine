#ifndef C_Vector_h__
#define C_Vector_h__

#include "C_Point.h"
/** 
 * Klasa implementuj�ca wektor w przestrzeni dwuwymiarowej o pocz�tku P0
 */ 
class C_Vector
{
public:
	/// tworzy wektor o pocz�tku P0 i ko�cu P1
	C_Vector(C_Point &P0,C_Point &P1);
	~C_Vector();
private:
	C_Point P0;	// pocz�tek wektora
	C_Point P1; // koniec wektora
};

#endif // C_Vector_h__