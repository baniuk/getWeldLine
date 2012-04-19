/** 
 * \test Testy klasy C_WeldlineDetect
 */
#include "stdafx.h"

/** 
 * Œrodowisko testowe 1 dla C_LinearWeld. Œrodowisko w³¹cza logowanie
 */ 
class C_LinearWeld_Test1 : public ::testing::Test {
protected:
	C_Matrix_Container *rtg;
	// na dane wyjœciowe interpolowany profil
	C_Matrix_Container *interpdata;
	// na dane wyjœciowe x
	C_Matrix_Container *interpdata_x;
	// na dane wyjœciowe parametry aproxymacji
	C_Matrix_Container *approxdata;
	/// dostêp do metod prywatnych
	const C_Matrix_Container* getRTG(C_LinearWeld *obj)
	{
		return obj->rtg;
	}
	/// dostêp do metod prywatnych
	bool evalNextStartPoint(C_LinearWeld *obj,unsigned int step)
	{
		return obj->evalNextStartPoint(step);
	}
	/// dostêp do metod prywatnych
	void access_fillBuffor(C_LinearWeld *obj)
	{
		obj->fillBuffor();
	}
	/// dostêp do metod prywatnych
	void access_P0P1(C_LinearWeld *obj, C_Point &_P0, C_Point &_P1)
	{
		_P0 = obj->P0;
		_P1 = obj->P1;
	}
	/// dostêp do danych prywatnych
	void access_interpaprox(C_LinearWeld *obj, C_CircBuff<C_LineWeldApprox> **approx,C_CircBuff<C_LineInterp> **interp)
	{
		*(approx) = &obj->approx_results;
		*(interp) = &obj->interp_lines;
	}
	/// dostêp do funkcji prywatnej
	bool access_evalNextParams(C_LinearWeld *obj)
	{
		return obj->evalNextParams();
	}
	void access_evalWeldPos(C_LinearWeld *obj,const C_LineWeldApprox *_approx, const C_LineInterp *_interp, const double *_pre, C_WeldPos &_weldPos )
	{
		obj->evalWeldPos(_approx, _interp, _pre, _weldPos );
	}
	C_LinearWeld_Test1()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("testimag9.dat");
	}

	virtual ~C_LinearWeld_Test1() {
		SAFE_DELETE(rtg);

	}

	virtual void SetUp() {
		
	}

	virtual void TearDown() {
		
	}
};
/// test konstruktora
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case1) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case1 ------\n");
	C_LinearWeld obj(rtg);
	const C_Matrix_Container *tmp = getRTG(&obj);
	std::cout << "\n\t" << "size: [" << tmp->_rows << "," << tmp->_cols <<"]"<<  "\n\n";
	ASSERT_EQ(1825,tmp->_rows);
	ASSERT_EQ(7842,tmp->_cols);
}
/// test evalNextStartPoint - przesówanie od poczatku o kilka punktów 
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case2) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case2 ------\n");
	C_LinearWeld obj(rtg);
	C_Point x_poz(10,0); C_Point delta(1,0);
	obj.SetProcedureParameters(10,x_poz); // start of x=10
	C_Point P0test;
	C_Point P1test;
	bool ok;
	const C_Matrix_Container *tmp = getRTG(&obj);
	for(int a=0;a<10;a++)	// 10 razy
	{
		access_P0P1(&obj,P0test,P1test);
		ASSERT_EQ(x_poz.getX(),P0test.getX());
		ASSERT_EQ(0,P0test.getY());		
		ASSERT_EQ(x_poz.getX(),P1test.getX());
		ASSERT_EQ(tmp->_rows-1,P1test.getY());
		ok = evalNextStartPoint(&obj,1);	//przesówam punkt
		ASSERT_TRUE(ok);
		x_poz+=delta;		
	}
}
/// test evalNextStartPoint - koniec obrazka 
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case3) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case3 ------\n");
	C_LinearWeld obj(rtg);

	C_Point x_poz(7831,0); C_Point delta(1,0);
	obj.SetProcedureParameters(10,x_poz); // start of x=10
	C_Point P0test;
	C_Point P1test;
	bool ok;
	const C_Matrix_Container *tmp = getRTG(&obj);
	for(int a=0;a<10;a++)	// 10 razy
	{
		access_P0P1(&obj,P0test,P1test);
		ASSERT_EQ(x_poz.getX(),P0test.getX());
		ASSERT_EQ(0,P0test.getY());		
		ASSERT_EQ(x_poz.getX(),P1test.getX());
		ASSERT_EQ(tmp->_rows-1,P1test.getY());
		ok = evalNextStartPoint(&obj,1);	//przesówam punkt
		ASSERT_TRUE(ok);
		x_poz+=delta;		
	}
	ok = evalNextStartPoint(&obj,1);	//przesówam punkt
	ASSERT_FALSE(ok);
}

/// test evalNextStartPoint - krok 0 
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case4) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case4 ------\n");
	C_LinearWeld obj(rtg);

	C_Point x_poz(7831,0); C_Point delta(1,0);
	obj.SetProcedureParameters(10,x_poz); // start of x=10
	C_Point P0test;
	C_Point P1test;
	bool ok;
	const C_Matrix_Container *tmp = getRTG(&obj);
	ok = evalNextStartPoint(&obj,0);	//przesówam punkt
	ASSERT_FALSE(ok);
}
