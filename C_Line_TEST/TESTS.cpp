#include "stdafx.h"
#include "GTEST_ADDONS.h"

/** 
 * Œrodowisko testowe 1
 */ 
class C_LineTest : public ::testing::Test {
protected:
	C_Line line1;	// linia pod k¹tem 45 stopni
	C_Line line2;	// inna
	C_Line pionowa;	// linia pionowa

	C_LineTest() {

	}

	virtual ~C_LineTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		line1.setLine(1,0,NORMALNA);
		line2.setLine(-2,10,NORMALNA);
		pionowa.setLine(0,5,PIONOWA);

	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};
/** 
 * Klasa do testów parametrycznych - parametrem jest nazwa pliku do wczytania przez container
 * wygenerowanego w Matlabie prze newTESTS
 */
#if GTEST_HAS_PARAM_TEST
using ::testing::TestWithParam;
using ::testing::Values;

class C_LineTestParam : public TestWithParam<const char*> {
public:
	virtual ~C_LineTestParam()
	{
	}
	virtual void SetUp()
	{
		const char *nazwa = GetParam();
		line_LinSPace = new C_Line();
		// kolejnosc parametrów w kontajnerze x0,x1,N,expected
		dataLinSpace = new C_Matrix_Container_Factory(nazwa,4);
		dataLinSpace->LoadData();
	}
	virtual void TearDown() {
		SAFE_DELETE(dataLinSpace);
		SAFE_DELETE(line_LinSPace);
	}
protected:
	// testy LinSpace
	C_Matrix_Container_Factory *dataLinSpace;	// test funkcji LinSpace
	C_Line *line_LinSPace;
};

class C_LineTestParamEvalLine : public TestWithParam<const char*> {
public:
	virtual ~C_LineTestParamEvalLine()
	{
	}
	virtual void SetUp()
	{
		const char *nazwa = GetParam();
		line_LinSPace = new C_Line();
		// kolejnosc parametrów w kontajnerze a,b,pion,x_data,result
		dataLinSpace = new C_Matrix_Container_Factory(nazwa,5);
		dataLinSpace->LoadData();
	}
	virtual void TearDown() {
		SAFE_DELETE(dataLinSpace);
		SAFE_DELETE(line_LinSPace);
	}
protected:
	// testy LinSpace
	C_Matrix_Container_Factory *dataLinSpace;	// test funkcji LinSpace
	C_Line *line_LinSPace;
};

#endif //GTEST_HAS_PARAM_TEST
/// sprawdzanie konstruktora domyœlnego oraz getPoints
TEST_F(C_LineTest, DefaultConstructor) {
	double a,b;
	KIERUNEK_PROSTEJ kier;
	kier = line1.getLine(a,b);
	EXPECT_EQ(1,a);
	EXPECT_EQ(0,b);
	EXPECT_EQ(NORMALNA,kier);
}
/// testowanie funccji setLine
TEST(_C_Line,setLineTest) {
	C_Line line;
	double reta,retb;
	KIERUNEK_PROSTEJ kier;
	line.setLine(5,3,NORMALNA);
	kier = line.getLine(reta,retb);
	EXPECT_EQ(5,reta);
	EXPECT_EQ(3,retb);
	EXPECT_EQ(NORMALNA,kier);
}
/// test czy zewnêtrzne liby dzia³aj¹ - trzeba linkowac takze wxy osobno bo w LIBIE z SDK nie ma ich
TEST(_C_Line, matrixcontainer) {
	C_Matrix_Container t;
	double expected[3] = {1,2,3};
	t.AllocateData(1,3);
	t.SetPixel(0,0,1);
	t.SetPixel(0,1,2);
	t.SetPixel(0,2,3);
	for (int i = 0; i < 3; ++i) {
		EXPECT_EQ(t.data[i], expected[i]) << "Vectors x and y differ at index " << i;
	}
}
/// test parametryczny LinSpace
TEST_P(C_LineTestParam, LinSpace) {
	// w datalinspce kolejno w ramach ka¿dego testu bed¹ pojawiaæ siê poszczegóne casey
	
	double x0,x1,N;
	int i;
	x0 = dataLinSpace->getContainer(0).data[0];
	x1 = dataLinSpace->getContainer(1).data[0];
	N = dataLinSpace->getContainer(2).data[0];

	double *calculated = new double[N];
	line_LinSPace->LinSpace(x0,x1,calculated,N);
	for(i=0;i<N;i++)
		ASSERT_DOUBLE_EQ(dataLinSpace->getContainer(3).GetPixel(0,i), calculated[i]) << "Vectors x and y differ at index " << i;

	SAFE_DELETE(calculated);
}
/// test parametryczny evalLine
TEST_P(C_LineTestParamEvalLine, evalLine) {
	// w datalinspce kolejno w ramach ka¿dego testu bed¹ pojawiaæ siê poszczegóne casey

	double a,b,pion,N;
	KIERUNEK_PROSTEJ kier=NORMALNA;
	int i;
	// dane do wczytania
	a = dataLinSpace->getContainer(0).data[0];
	b = dataLinSpace->getContainer(1).data[0];
	pion = dataLinSpace->getContainer(2).data[0];
	const C_Matrix_Container &x_data = dataLinSpace->getContainer(3);
	N = x_data.GetPixel(0,2);
	// transformowanie informacji z matlaba na kierunek (1 to pionowa)
	if(1==pion)
		kier = PIONOWA;
	// przygotowanie tablicy na wejœcie
	double *x = new double[N];
	// przygotowanie tablicy na wyjœcie
	double *y = new double[N];
	// inicjalizacja linii
	line_LinSPace->setLine(a,b,kier);
	// generowanie wartosci x
	line_LinSPace->LinSpace(x_data.GetPixel(0,0),x_data.GetPixel(0,1),x,N);
	// obloczneie wartoœci na linii
	line_LinSPace->evalLine(x,N,y);
	for(i=0;i<N;i++)
		ASSERT_DOUBLE_EQ(dataLinSpace->getContainer(4).GetPixel(0,i), y[i]) << "Vectors x and y differ at index " << i;

	SAFE_DELETE(x);
	SAFE_DELETE(y);
}
/// test getLine2Points
TEST(_C_Line,getLine2Points_case1)
{
	C_Line line;
	C_Point P0(0,0);
	C_Point P1(10,10);
	double a,b;
	KIERUNEK_PROSTEJ kier;
	line.getLine2Points(P0,P1);
	kier = line.getLine(a,b);
	ASSERT_DOUBLE_EQ(1,a);
	ASSERT_DOUBLE_EQ(0,b);
	EXPECT_EQ(NORMALNA,kier);
}
TEST(_C_Line,getLine2Points_case2)
{
	C_Line line;
	C_Point P0(10,0);
	C_Point P1(10,10);
	double a,b;
	KIERUNEK_PROSTEJ kier;
	line.getLine2Points(P0,P1);
	kier = line.getLine(a,b);
//	ASSERT_DOUBLE_EQ(1,a);
	ASSERT_DOUBLE_EQ(10,b);
	EXPECT_EQ(PIONOWA,kier);
}
/// test LineCutCircle
TEST_F(C_LineTest,LineCutCircle_case1)
{
	C_Point P0(0,0);
	C_Point out[2];
	C_Point exp1(1,1);
	C_Point exp2(-1,-1);
	bool ret;
	ret = line1.LineCutCircle(P0,sqrt(2.0),out);
	SCOPED_TRACE("LineCutCircle_case1");
	ASSERT_TRUE(ret);
	C_Point_CMP_DOUBLE(exp1,out[0]);
	C_Point_CMP_DOUBLE(exp2,out[1]);
}
TEST_F(C_LineTest,LineCutCircle_case2)
{
	C_Point P0(5,7);
	C_Point out[2];
	C_Point exp1(5,8);
	C_Point exp2(5,6);
	bool ret;
	ret = pionowa.LineCutCircle(P0,1,out);
	SCOPED_TRACE("LineCutCircle_case1");
	ASSERT_TRUE(ret);
	C_Point_CMP_DOUBLE(exp1,out[0]);
	C_Point_CMP_DOUBLE(exp2,out[1]);
}
TEST_F(C_LineTest,LineCutCircle_case3)
{
	C_Point P0(1,0);
	C_Point out[2];
	bool ret;
	ret = line1.LineCutCircle(P0,sqrt(2.0),out);
	SCOPED_TRACE("LineCutCircle_case3");
	ASSERT_FALSE(ret);
}

INSTANTIATE_TEST_CASE_P(
	LinSpace_test,
	C_LineTestParam,
	::testing::Values("case1.dat","case2.dat","case3.dat"));

INSTANTIATE_TEST_CASE_P(
	evalLin_test,
	C_LineTestParamEvalLine,
	::testing::Values("line1.dat","line2.dat","line3.dat"));