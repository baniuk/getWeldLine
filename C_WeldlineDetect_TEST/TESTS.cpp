#include "stdafx.h"
#include "GTEST_ADDONS.h"

#if GTEST_HAS_PARAM_TEST
using ::testing::TestWithParam;
using ::testing::Values;
/** 
 * Przygotowanie testu parametrycznego - wypelnianie bufora. 
 * Parametrem s¹ró¿ne pozycje punktu startowego
 */
class C_LinearWeld_FillBuffor : public TestWithParam<int> {
public:
	C_LinearWeld_FillBuffor()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("testimag1.dat");
	}
	virtual ~C_LinearWeld_FillBuffor()
	{
		SAFE_DELETE(rtg);
	}
	virtual void SetUp()
	{
		// setup C_LinearWeld
		obj = new C_LinearWeld(rtg);
		x_start = GetParam();
	}
	virtual void TearDown() {
		SAFE_DELETE(obj);
	}
protected:
	C_Matrix_Container *rtg;
	C_LinearWeld *obj;// g³ówny obiekt testowy
	void access_fillBuffor(C_LinearWeld *obj)
	{
		obj->fillBuffor();
	}
	void access_interpaprox(C_LinearWeld *obj, C_CircBuff<C_LineWeldApprox> **approx,C_CircBuff<C_LineInterp> **interp)
	{
		*(approx) = &obj->approx_results;
		*(interp) = &obj->interp_lines;
	}
	C_CircBuff<C_LineWeldApprox> *approx;
	C_CircBuff<C_LineInterp> *interp;
	int x_start;
};

#endif //GTEST_HAS_PARAM_TEST
/** 
 * Œrodowisko testowe 1 dla C_CircBuff
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
	test1->BuffInit(3);// 3 elementów
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(0,test1->getNumElem());
	test1->AddObject();
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(1,test1->getNumElem());
	test1->AddObject();
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	ASSERT_EQ(2,test1->getNumElem());
	test1->AddObject();
	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
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
	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
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

	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(2,*(test1->GetObject(1)));
	ASSERT_EQ(3,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));
	// jeœli pe³ny to zwraca pojemnoœæ
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

	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(6,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));

}
/// test czy bufora 5
TEST_F(C_CircBuff_Test1, CircBuff_case5) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1, zamiana 4 na 30 nie powinna burzyæ struktury
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	*(test1->AddObject()) = 3;
	*(test1->AddObject()) = 4;
	test1->DelObject();
	*(test1->AddObject()) = 30;
	*(test1->AddObject()) = 5;
	*(test1->AddObject()) = 6;

	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
	ASSERT_EQ(30,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(6,*(test1->GetObject(2)));
	ASSERT_EQ(30,*(test1->GetObject(3)));

}

/// test czy bufora 6
TEST_F(C_CircBuff_Test1, CircBuff_case6) {
	test1->BuffInit(3);	// 3 elementy [1 2 3 4]->4 idzie za 1, zamiana 6 na 30 nie powinna burzyæ struktury
	*(test1->AddObject()) = 1;
	*(test1->AddObject()) = 2;
	ASSERT_FALSE(test1->Czy_pelny());	// pusty
	*(test1->AddObject()) = 3;
	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
	*(test1->AddObject()) = 4;
	*(test1->AddObject()) = 5;
	*(test1->AddObject()) = 6;
	test1->DelObject();
	ASSERT_TRUE(test1->Czy_pelny());	// pusty
	*(test1->AddObject()) = 30;

	ASSERT_TRUE(test1->Czy_pelny());	// pe³ny
	ASSERT_EQ(4,*(test1->GetObject(0)));
	ASSERT_EQ(5,*(test1->GetObject(1)));
	ASSERT_EQ(30,*(test1->GetObject(2)));
	ASSERT_EQ(4,*(test1->GetObject(3)));

}
// w pêtli tworzenie
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
/// tworzenie w pêtli i kasowanie jednego
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
	ASSERT_EQ(4,l); // wykona sie o raz wiêcej bo jeden skasowany
}
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
	bool evalNextStartPoint(C_LinearWeld *obj)
	{
		return obj->evalNextStartPoint();
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
/// test evalNextStartPoint - przesówanie od poczatku o kilka punktów 
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
		ok = evalNextStartPoint(&obj);	//przesówam punkt
		ASSERT_TRUE(ok);
		x_poz++;		
	}
}
/// test evalNextStartPoint - koniec obrazka 
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
		ok = evalNextStartPoint(&obj);	//przesówam punkt
		ASSERT_TRUE(ok);
		x_poz++;		
	}
	ok = evalNextStartPoint(&obj);	//przesówam punkt
	ASSERT_FALSE(ok);
}


/// test fillBuffor - wype³aniam bufor i sprawdzam aproksymacje dla ró¿nych punktów startowych
TEST_P(C_LinearWeld_FillBuffor, case1) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_FillBuffor_case1 ------ ");
	_RPT1(_CRT_WARN,"PAR: %d\n",x_start);
	char buff[6];_itoa(x_start,buff,10);std::string liczba(buff);
	std::string nazwa("C_LinearWeld_FillBuffor");
	nazwa+=liczba;
	nazwa+=".out";
		std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa <<"\n\n";
	C_DumpAll dump(nazwa.c_str());

	C_LineInterp *tmp;
	int ile_linii = 25;
	obj->SetProcedureParameters(ile_linii,x_start); // start of x=10, ile_linii lini w buforze
	// start
	access_fillBuffor(obj);
	// pobieral adresy struktur z danymi
	access_interpaprox(obj,&approx,&interp);
	// sprawdzam czy bufor pe³en - powinien byc ze wzgledu na sposób dzia³ania fillbuffor
	ASSERT_TRUE(approx->Czy_pelny());
	ASSERT_TRUE(interp->Czy_pelny());
	// czy ma tyle elementów co trzeba
	ASSERT_EQ(ile_linii,approx->getNumElem());
	ASSERT_EQ(ile_linii,interp->getNumElem());
	// czy pionowe
	tmp = interp->GetObject(0);
	ASSERT_EQ(PIONOWA,tmp->getjest_pion());
	// zrzucanie wyników - w rzêdach kolejne wyniki
	// zrucam wyniki p pod postaci¹ wsp³ó³czynników
	// dane x i y interpolacji
	// wartoœci interpolownego profilu profil
	C_Matrix_Container x(interp->getNumElem(),interp->GetObject(0)->getSizeofApproxData());
	C_Matrix_Container y(interp->getNumElem(),interp->GetObject(0)->getSizeofApproxData());
	C_Matrix_Container profil(interp->getNumElem(),interp->GetObject(0)->getSizeofApproxData());
	C_Matrix_Container p(approx->getNumElem(),5);
	// kopiowanie danych do matlaba
	for (int a=0,k=0;a<interp->getNumElem();a++)	// po ca³ym buforze
	{
		for (k=0;k<interp->GetObject(a)->getSizeofApproxData();k++)
		{
			x.SetPixel(a,k,interp->GetObject(a)->getInterpolated_X()[k]);
			y.SetPixel(a,k,interp->GetObject(a)->getInterpolated_Y()[k]);
			profil.SetPixel(a,k,interp->GetObject(a)->getInterpolated_data()[k]);
		}
		for(k=0;k<5;k++)
			p.SetPixel(a,k,approx->GetObject(a)->getApproxParams_p()[k]);
	}
	dump.AddEntry(&x,"x");
	dump.AddEntry(&y,"y");
	dump.AddEntry(&profil,"profil");
	dump.AddEntry(&p,"p");
}

INSTANTIATE_TEST_CASE_P(
	FillBuffor_Test,
	C_LinearWeld_FillBuffor,
	::testing::Values(10,1612,3253,4294)); // te same wartosci sa w matlabie
