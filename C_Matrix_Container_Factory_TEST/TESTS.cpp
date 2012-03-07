#include "stdafx.h"


class C_Matrix_Container_FactoryTest : public ::testing::Test {
protected:
	
	void C_get_private_CreateName(C_Matrix_Container_Factory *obj, int n, char* out ) {
		obj->CreateName(n,out);
	}
	void C_get_private_LoadData(C_Matrix_Container_Factory *obj) {
		obj->LoadData();
	}

	C_Matrix_Container_FactoryTest() {

	}

	virtual ~C_Matrix_Container_FactoryTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};

/// testowanie funccji CreateName
TEST_F(C_Matrix_Container_FactoryTest, CreateName) {
	C_Matrix_Container_Factory factory("Plik.dat",5);
	char res[MAX_FILE_NAME];
	C_get_private_CreateName(&factory,4,res);
	char expected[] = "Plik_4.dat";
	ASSERT_STREQ(expected, res) << "ciagi nie sa takie same";
}
TEST_F(C_Matrix_Container_FactoryTest, CreateName_bez) {
	C_Matrix_Container_Factory factory("Plik",5);
	char res[MAX_FILE_NAME];
	C_get_private_CreateName(&factory,14,res);
	char expected[] = "Plik_14";
	ASSERT_STREQ(expected, res) << "ciagi nie sa takie same";
	// czy koniec tablicy prawid³owo
	C_get_private_CreateName(&factory,4,res);
	char expected1[] = "Plik_4";
	ASSERT_STREQ(expected1, res) << "ciagi nie sa takie same";
}
/// test funkcji loaddata oraz poprawnosci wczytanych danych
TEST_F(C_Matrix_Container_FactoryTest,loaddata)
{
	char currentdir[1024];
	_getcwd(currentdir,1024);
	C_Matrix_Container_Factory factory("Plik.dat",3);
	std::cout << "\t" << currentdir << "\n";
	C_get_private_LoadData(&factory); 
}

TEST_F(C_Matrix_Container_FactoryTest,getcontainer)
{
	C_Matrix_Container_Factory factory("Plik.dat",3);
	factory.LoadData();
//	cm1 = factory.getContainer(0);
//	cm2 = factory.getContainer(1);
	//cm3 = factory.getContainer(2);
	EXPECT_EQ(1,factory.getContainer(0).data[0]);
	EXPECT_EQ(3,factory.getContainer(1).data[0]);
	EXPECT_EQ(4,factory.getContainer(1).data[1]);
	EXPECT_EQ(5,factory.getContainer(2).data[0]);

}
