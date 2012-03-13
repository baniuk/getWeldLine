#include "stdafx.h"
#include "GTEST_ADDONS.h"

/** 
 * �rodowisko testowe 1 dla C_CircBuff
 */ 
class C_CircBuff_Test1 : public ::testing::Test {
protected:
	C_CircBuff<double> *test1;

	virtual ~C_CircBuff_Test1() {
		SAFE_DELETE(test1);
	}

	virtual void SetUp() {
		test1 = new C_CircBuff<double>;
	}

	virtual void TearDown() {
		SAFE_DELETE(test1);
	}
};
/// test czy bufora 1
TEST_F(C_CircBuff_Test1, CircBuff_case1) {
	test1->BuffInit(3);// 3 element�w
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(0,test1->getNumElem());
	test1->AddObject();
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(1,test1->getNumElem());
	test1->AddObject();
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(2,test1->getNumElem());
	test1->AddObject();
	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(3,test1->getNumElem());
}
/// test czy bufora 1
TEST_F(C_CircBuff_Test1, CircBuff_case2) {
	double *tmp;
	test1->BuffInit(3);	// 3 elementy
	tmp = test1->AddObject();
	*(tmp) = 1;
	tmp = test1->AddObject();
	*(tmp) = 2;
	tmp = test1->AddObject();
	*(tmp) = 3;
	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(1,*(test1->GetObject(0)));
	ASSERT_EQ(2,*(test1->GetObject(1)));
	ASSERT_EQ(3,*(test1->GetObject(2)));
	ASSERT_EQ(1,*(test1->GetObject(3)));

}
/// test czy bufora 3
TEST_F(C_CircBuff_Test1, CircBuff_case3) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	*(test1->AddObject()) = 3;
	*(test1->AddObject()) = 4;

	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(2,*(test1->GetObject(1)));
	ASSERT_EQ(3,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));
	// je�li pe�ny to zwraca pojemno��
	ASSERT_EQ(3,test1->getNumElem());

}
/// test czy bufora 4
TEST_F(C_CircBuff_Test1, CircBuff_case4) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	*(test1->AddObject()) = 3;
	*(test1->AddObject()) = 4;
	*(test1->AddObject()) = 5;
	*(test1->AddObject()) = 6;

	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(6,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));

}
/// test czy bufora 5
TEST_F(C_CircBuff_Test1, CircBuff_case5) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1, zamiana 4 na 30 nie powinna burzy� struktury
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	*(test1->AddObject()) = 3;
	*(test1->AddObject()) = 4;
	test1->DelObject();
	*(test1->AddObject()) = 30;
	*(test1->AddObject()) = 5;
	*(test1->AddObject()) = 6;

	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(30,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(6,*(test1->GetObject(2)));
	ASSERT_EQ(30,*(test1->GetObject(3)));

}

/// test czy bufora 6
TEST_F(C_CircBuff_Test1, CircBuff_case6) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1, zamiana 6 na 30 nie powinna burzy� struktury
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	*(test1->AddObject()) = 3;
	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	*(test1->AddObject()) = 4;
	*(test1->AddObject()) = 5;
	*(test1->AddObject()) = 6;
	test1->DelObject();
	ASSERT_TRUE(test1->Czy_pelny());	// pusty
	*(test1->AddObject()) = 30;

	ASSERT_TRUE(test1->Czy_pelny());	// pe�ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(30,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));

}
// w p�tli tworzenie
TEST(_C_CircularBuffer,CircBuff_case7)
{
	C_CircBuff<C_LineWeldApprox> approx_results;
	approx_results.BuffInit(3);
	int l=0;
	C_LineWeldApprox *ptr;
	while(!approx_results.Czy_pelny())
	{
		ptr = approx_results.AddObject();
		l++;
	}
	ASSERT_EQ(3,l);
}
/// tworzenie w p�tli i kasowanie jednego
TEST(_C_CircularBuffer,CircBuff_case8)
{
	C_CircBuff<C_LineWeldApprox> approx_results;
	approx_results.BuffInit(3);
	int l=0;
	C_LineWeldApprox *ptr;
	while(!approx_results.Czy_pelny())
	{
		ptr = approx_results.AddObject();
		if(l==1)
			approx_results.DelObject();
		l++;
	}
	ASSERT_EQ(4,l); // wykona sie o raz wi�cej bo jeden skasowany
}
/** 
 * �rodowisko testowe 1 dla C_LinearWeld. �rodowisko w��cza logowanie
 */ 
class C_LinearWeld_Test1 : public ::testing::Test {
protected:
	C_Matrix_Container *rtg;
	// na dane wyj�ciowe interpolowany profil
	C_Matrix_Container *interpdata;
	// na dane wyj�ciowe x
	C_Matrix_Container *interpdata_x;
	// na dane wyj�ciowe parametry aproxymacji
	C_Matrix_Container *approxdata;
	/// dost�p do metod prywatnych
	const C_Matrix_Container* getRTG(C_LinearWeld *obj)
	{
		return obj->rtg;
	}
	/// dost�p do metod prywatnych
	bool access_evalNextParams(C_LinearWeld *obj)
	{
		return obj->evalNextParams();
	}
	/// dost�p do metod prywatnych
	void access_fillBuffor(C_LinearWeld *obj)
	{
		obj->fillBuffor();
	}
	/// dost�p do metod prywatnych
	void access_P0P1(C_LinearWeld *obj, C_Point &_P0, C_Point &_P1)
	{
		_P0 = obj->P0;
		_P1 = obj->P1;
	}
	/// dost�p do danych prywatnych
	void access_interpaprox(C_LinearWeld *obj, C_CircBuff<C_LineWeldApprox> **approx,C_CircBuff<C_LineInterp> **interp)
	{
		*(approx) = &obj->approx_results;
		*(interp) = &obj->interp_lines;
	}
	C_LinearWeld_Test1()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("testimag1.dat");
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
/// test evalNextParams - przes�wanie od poczatku o kilka punkt�w 
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case2) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case2 ------\n");
	C_LinearWeld obj(rtg);
	int x_poz = 10;
	obj.SetProcedureParameters(10,x_poz); // start of x=10
	C_Point P0test;
	C_Point P1test;
	bool ok;
	const C_Matrix_Container *tmp = getRTG(&obj);
	for(int a=0;a<10;a++)	// 10 razy
	{
		access_P0P1(&obj,P0test,P1test);
		ASSERT_EQ(x_poz,P0test.getX());
		ASSERT_EQ(0,P0test.getY());		
		ASSERT_EQ(x_poz,P1test.getX());
		ASSERT_EQ(tmp->_rows-1,P1test.getY());
		ok = access_evalNextParams(&obj);	//przes�wam punkt
		ASSERT_TRUE(ok);
		x_poz++;		
	}
}
/// test evalNextParams - koniec obrazka 
TEST_F(C_LinearWeld_Test1, C_LinearWeld_case3) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_case3 ------\n");
	C_LinearWeld obj(rtg);

	int x_poz = 7831;
	obj.SetProcedureParameters(10,x_poz); // start of x=10
	C_Point P0test;
	C_Point P1test;
	bool ok;
	const C_Matrix_Container *tmp = getRTG(&obj);
	for(int a=0;a<10;a++)	// 10 razy
	{
		access_P0P1(&obj,P0test,P1test);
		ASSERT_EQ(x_poz,P0test.getX());
		ASSERT_EQ(0,P0test.getY());		
		ASSERT_EQ(x_poz,P1test.getX());
		ASSERT_EQ(tmp->_rows-1,P1test.getY());
		ok = access_evalNextParams(&obj);	//przes�wam punkt
		ASSERT_TRUE(ok);
		x_poz++;		
	}
	ok = access_evalNextParams(&obj);	//przes�wam punkt
	ASSERT_FALSE(ok);
}
/// test fillBuffor
TEST_F(C_LinearWeld_Test1, C_LinearWeld_FillBuffor) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_FillBuffor ------\n");
	C_LinearWeld obj(rtg);
	C_CircBuff<C_LineWeldApprox> *approx;
	C_CircBuff<C_LineInterp> *interp;
	C_LineInterp *tmp;
	int ile_linii = 10;
	int x_poz = 10;	// 10 linia
	obj.SetProcedureParameters(ile_linii,x_poz); // start of x=10, ile_linii lini w buforze
	access_fillBuffor(&obj);
	access_interpaprox(&obj,&approx,&interp);
	// sprawdzam czy bufor pe�en - powinien byc ze wzgledu na spos�b dzia�ania fillbuffor
	ASSERT_TRUE(approx->Czy_pelny());
	ASSERT_TRUE(interp->Czy_pelny());
	// czy ma tyle element�w co trzeba
	ASSERT_EQ(ile_linii,approx->getNumElem());
	ASSERT_EQ(ile_linii,interp->getNumElem());
	// czy pionowe
	tmp = interp->GetObject(0);
	ASSERT_EQ(PIONOWA,tmp->getjest_pion());
	// zrzucanie wynik�w

}
