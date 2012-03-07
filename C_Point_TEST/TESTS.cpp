#include "stdafx.h"

/// sprawdzanie konstruktora
TEST(_C_Point, noInitialization) {
	C_Point notinitialized;
	double x,y;
	notinitialized.getPoint(x,y);
	EXPECT_EQ(0, x);
	EXPECT_EQ(0, y);
}

/// inicjalizacja oraz metody setPoint i getPoint
TEST(_C_Point, Initialization) {
	C_Point initialized;
	initialized.setPoint(-1,20);
	double x,y;
	initialized.getPoint(x,y);
	EXPECT_EQ(-1, x);
	EXPECT_EQ(20, y);
	EXPECT_EQ(-1, initialized.getX());
	EXPECT_EQ(20, initialized.getY());
}

/// test operatora przypisania
TEST(_C_Point, przypisanie) {
	C_Point initialized;
	C_Point noinitialized;
	double x,y;

	initialized.setPoint(-1,20);
	
	noinitialized = initialized;
	noinitialized.getPoint(x,y);
	EXPECT_EQ(-1, x);
	EXPECT_EQ(20, y);
}

/** 
 * test operatora przypisania - kasowanie orygina³u. Sprawdzam czy po skasowaniu orygina³u w kopii bêd¹ prawid³owe dane
 */ 
TEST(_C_Point, przypisanie_del) {
	C_Point *initialized = new C_Point(-1,20);
	C_Point noinitialized;
	double x,y;

	noinitialized = *initialized;
	delete initialized;
	noinitialized.getPoint(x,y);
	EXPECT_EQ(-1, x);
	EXPECT_EQ(20, y);
}
/** 
 * Test operatora A+=B;
 */
TEST(_C_Point, operAprB) {
	double xa,ya,xb,yb;
	C_Point A(10,1);
	C_Point B(2,4);
	A+=B;
	A.getPoint(xa,ya);
	B.getPoint(xb,yb);
	EXPECT_EQ(12, xa);
	EXPECT_EQ(5, ya);
	// b bez zmian
	EXPECT_EQ(2, xb);
	EXPECT_EQ(4, yb);
}

/** 
 * Test operatora A-=B;
 */
TEST(_C_Point, operAmrB) {
	double xa,ya,xb,yb;
	C_Point A(10,1);
	C_Point B(2,4);
	A-=B;
	A.getPoint(xa,ya);
	B.getPoint(xb,yb);
	EXPECT_EQ(8, xa);
	EXPECT_EQ(-3, ya);
	// b bez zmian
	EXPECT_EQ(2, xb);
	EXPECT_EQ(4, yb);
}

/** 
 * Test operatora A/=B;
 */
TEST(_C_Point, operAdzielrB) {
	double xa,ya;
	C_Point A(10,1);
	double d = 2;
	A/=d;
	A.getPoint(xa,ya);
	EXPECT_EQ(5,xa);
	EXPECT_EQ(0.5, ya);
}

/** 
 * Test operatora A*=B;
 */
TEST(_C_Point, operAmnozrB) {
	double xa,ya;
	C_Point A(10,1);
	double d = 2;
	A*=d;
	A.getPoint(xa,ya);
	EXPECT_EQ(20,xa);
	EXPECT_EQ(2, ya);
}

/** 
 * Test operatora C = A + B;
 */
TEST(_C_Point, operApB) {
	double xa,ya,xb,yb,xc,yc;
	C_Point A(10,1);
	C_Point B(2,4);
	C_Point C;
	C = A+B;
	A.getPoint(xa,ya);
	B.getPoint(xb,yb);
	C.getPoint(xc,yc);
	EXPECT_EQ(12, xc);
	EXPECT_EQ(5, yc);
	// b bez zmian
	EXPECT_EQ(2, xb);
	EXPECT_EQ(4, yb);
	// a bez zmian
	EXPECT_EQ(10, xa);
	EXPECT_EQ(1, ya);
}

/** 
 * Test operatora C = A - B;
 */
TEST(_C_Point, operAmB) {
	double xa,ya,xb,yb,xc,yc;
	C_Point A(10,1);
	C_Point B(2,4);
	C_Point C;
	C = A-B;
	A.getPoint(xa,ya);
	B.getPoint(xb,yb);
	C.getPoint(xc,yc);
	EXPECT_EQ(8, xc);
	EXPECT_EQ(-3, yc);
	// b bez zmian
	EXPECT_EQ(2, xb);
	EXPECT_EQ(4, yb);
	// a bez zmian
	EXPECT_EQ(10, xa);
	EXPECT_EQ(1, ya);
}