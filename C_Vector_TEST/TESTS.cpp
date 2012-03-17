#include "stdafx.h"

#include "GTEST_ADDONS.h"

/** \brief Funkcja obs³uguj¹ca porównaina wektorów - obiektów C_Vector
 * Funkcja porównuje pocz¹tki, koñce oraz wspó³rzêdne zapisane w klasie
 */ 
void C_Vector_CMP(const C_Vector V0, const C_Vector V1)
{
	C_Point v0P0,v0P1,v0VEC,v1P0,v1P1,v1VEC;
	V0.getVector(v0P0,v0P1,v0VEC);
	V1.getVector(v1P0,v1P1,v1VEC);
	C_Point_CMP(v0P0,v1P0);
	C_Point_CMP(v0P1,v1P1);
	C_Point_CMP(v0VEC,v1VEC);

}
/** 
 * Œrodowisko testowe 1 dla C_Vector.
 */ 
class C_VectorTest : public ::testing::Test {
protected:
	C_Point _P0;	// punkt pocz¹tkowy
	C_Point _P1; // punkt koñcowy
	C_Point _VEC;	// wsp wektora
	C_Vector vector;
	double len;

	C_Point _P01;	// punkt pocz¹tkowy
	C_Point _P11; // punkt koñcowy
	C_Point _VEC1;	// wsp wektora
	C_Vector vector1;
	double len1;

	C_Point C_get_private_setVEC(C_Vector* obj,C_Point &_P0, C_Point &_P1) {
		return obj->setVEC(_P0,_P1);
	}

	C_VectorTest() {

	}

	virtual ~C_VectorTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		_P0.setPoint(1,2);
		_P1.setPoint(11,21);
		_VEC.setPoint(10,19);
		vector.recalculateVectorP0P1(_P0,_P1); // ten wektor jest zgodny z _VEC
		len = sqrt(10.0*10.0+19.0*19.0);

		_P01.setPoint(-4,1);
		_P11.setPoint(5,4);
		_VEC1.setPoint(9,3);
		vector1.recalculateVectorP0P1(_P01,_P11); // ten wektor jest zgodny z _VEC
		len1 = sqrt(9.0*9.0+3.0*3.0);

	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};

/// sprawdzanie konstruktora domyœlnego oraz getPoints
TEST_F(C_VectorTest, DefaultConstructor) {
	C_Point P0,P1;
	C_Vector initialized(_P0,_P1);
	initialized.getPoints(P0,P1);
	SCOPED_TRACE("DefaultConstructor");
	C_Point_CMP(_P0,P0);
	C_Point_CMP(_P1,P1);
}

/// testowanie konstruktora dowolnego i getVector
TEST_F(C_VectorTest, Constructor) {
	C_Point P0,P1,VEC;
	C_Vector initialized(_P0,_P1);
	initialized.getVector(P0,P1,VEC);
	SCOPED_TRACE("Constructor");
	C_Point_CMP(_P0,P0);
	C_Point_CMP(_P1,P1);
	C_Point_CMP(VEC,P1-P0); // wspó³rzêdne wektora po inicjalziacji
}
/// testowanie wspó³rzêdnych wektora
TEST_F(C_VectorTest, setVEC) {
	C_Point P0(5,3);
	C_Point P1(9,10);
	C_Point correct(9-5,10-3);
	C_Point VEC;
	C_Vector vector;
	VEC = C_get_private_setVEC(&vector,P0,P1);
	SCOPED_TRACE("setVEC");
	C_Point_CMP(correct,VEC);
}

/// testowanie recalculateVectorP0VEC
TEST_F(C_VectorTest, recalculateVectorP0VEC) {
	C_Point VEC(5,3);	// wsp wektora
	C_Point retP0,retP1,retVEC;	// parametry zwrócone przez fcn
	C_Vector vector;
	vector.recalculateVectorP0VEC(_P0,VEC);
	vector.getVector(retP0,retP1,retVEC);
	SCOPED_TRACE("recalculateVectorP0VEC");
	C_Point correctP0 = _P0;
	C_Point correctP1(6,5); //_P0+VEC
	C_Point correctVEC = VEC;
	C_Point_CMP(retP0,correctP0);
	C_Point_CMP(retP1,correctP1);
	C_Point_CMP(retVEC,correctVEC);
}

/// testowanie recalculateVectorP0P1
TEST_F(C_VectorTest, recalculateVectorP0P1) {

	C_Point retP0,retP1,retVEC;	// parametry zwrócone przez fcn
	C_Vector vector;
	vector.recalculateVectorP0P1(_P0,_P1);
	vector.getVector(retP0,retP1,retVEC);
	SCOPED_TRACE("recalculateVectorP0P1");
	C_Point correctP0 = _P0;
	C_Point correctP1 = _P1;
	C_Point correctVEC = _VEC;
	C_Point_CMP(retP0,correctP0);
	C_Point_CMP(retP1,correctP1);
	C_Point_CMP(retVEC,correctVEC);
}

/// testowanie setNewP0
TEST_F(C_VectorTest, setNewP0) {

	C_Point newP0(0,0);
	vector.setNewP0(newP0);
	SCOPED_TRACE("setNewP0");

	C_Vector expected(newP0,_VEC); // pocz¹tek jak newP0 a wspó³rzêdne bez zmian, koniec powinien siê zgodziæ
	C_Vector_CMP(vector,expected);
}

/// testowanie getVectorLen
TEST_F(C_VectorTest, getVectorLen) {

	double l;
	l = vector.getVectorLen();
	
	EXPECT_EQ(l,len);
}

/// testowanie setVectorLen
TEST_F(C_VectorTest, setVectorLen) {

	C_Point P0(0,0);
	C_Point P1(0,10);
	C_Point P3(0,2);	// spodziewany koniec wektora
	C_Vector v(P0,P1);

	v.setVectorLen(2.0);

	C_Vector expected(P0,P3);
	C_Vector_CMP(expected,v);
}

/// testowanie operatora A+=B
TEST_F(C_VectorTest, operAprB)	{
	C_Vector expected;
	expected.recalculateVectorP0VEC(_P0,_VEC+_VEC1);
	vector+=vector1;
	C_Vector_CMP(expected,vector);
}

/// testowanie operatora A-=B
TEST_F(C_VectorTest, operAmrB)	{
	C_Vector expected;
	expected.recalculateVectorP0VEC(_P0,_VEC-_VEC1);
	vector-=vector1;
	C_Vector_CMP(expected,vector);
}

/// testowanie operatora C=A+B
TEST_F(C_VectorTest, operApB)	{
	C_Vector expected;
	C_Vector res;
	expected.recalculateVectorP0VEC(_P0,_VEC+_VEC1);
	res = vector+vector1;
	C_Vector_CMP(expected,res);
}

/// testowanie operatora C=A-B
TEST_F(C_VectorTest, operAmB)	{
	C_Vector expected;
	C_Vector res;
	expected.recalculateVectorP0VEC(_P0,_VEC-_VEC1);
	res = vector-vector1;
	C_Vector_CMP(expected,res);
}

/// testowanie operatora *
TEST_F(C_VectorTest, operskalarny)	{
	double sk;
	sk = vector*vector1;
	EXPECT_EQ(147,sk);
}