/** 
 * \test Testy klasy C_CircBuff
 */
#include "stdafx.h"


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