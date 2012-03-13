#include "stdafx.h"
#include "GTEST_ADDONS.h"

/** 
 * Œrodowisko testowe 1
 * Tworzone trzy rózne linie,
 */ 
class C_LineApprox_Test1 : public ::testing::Test {
protected:
	C_LineInterp *line1;	// linia pod k¹tem 45 stopni
	C_LineInterp *line2;	// inna
	C_LineInterp *pionowa;	// linia pionowa
	double obrazek1[3];
	unsigned int siz1[3];
	double obrazek2[6];
	unsigned int siz2[3];

	virtual ~C_LineApprox_Test1() {
		SAFE_DELETE(line1);
		SAFE_DELETE(line2);
		SAFE_DELETE(pionowa);
	}

	virtual void SetUp() {
		obrazek1[0] = 0;obrazek1[1] = 10;obrazek1[2] = 30;
		siz1[0] = 3;siz1[1]=0;siz1[2]=0;
		siz2[0] = 3;siz2[1]=2;siz2[2]=0;
		line1 = new C_LineInterp(SPLINE,1,0,NORMALNA,obrazek1,siz1);
		line2 = new C_LineInterp(SPLINE,-2,10,NORMALNA,obrazek2,siz2);
		pionowa = new C_LineInterp(SPLINE,0,5,PIONOWA,obrazek1,siz1);
	}

	virtual void TearDown() {
		SAFE_DELETE(line1);
		SAFE_DELETE(line2);
		SAFE_DELETE(pionowa);
	}
};
/// sprawdzanie konstruktora domyœlnego
TEST(_C_LineApprox, DefaultConstructor) {
	C_LineInterp la;
	double a,b;
	KIERUNEK_PROSTEJ kier;
	kier = la.getLine(a,b);
	EXPECT_EQ(0,b);
	EXPECT_EQ(1,a);
	EXPECT_EQ(NORMALNA,kier);
}
/// sprawdzanie dziedziczenia: funkcja getPointonLine
TEST_F(C_LineApprox_Test1, getPointonLine) {
	// sprawdzam poprawnosc odwo³ania do metody klasy bazowej
	C_Matrix_Container expected;
	double *x,*y;
	bool ret;
	x = new double[100];
	y = new double[100];
	C_Point P0(0,10);
	C_Point P1(1,8);
	expected.ReadBinary("getPointonLine.dat");
	// rzutuje na obiekt bazowy - jesli tak to wywo³uje wci¹¿ metodê pochdn¹
			// C_Line &li = *line2;
	// a tak dzia³a
	C_Line &li = (C_Line)*line2;
	// wywo³uje metode bazowa
	ret = li.getPointsOnLine(P0,P1,x,y,100);
	ASSERT_TRUE(ret);
	for(int i=0;i<100;i++)
		ASSERT_DOUBLE_EQ(expected.GetPixel(0,i), y[i]) << "Vectors y and differ at index " << i;
}

/// sprawdzenie przekazywania przez int tab[3] oraz funkcji getNumOfElements
TEST_F(C_LineApprox_Test1, getNumOfElements) {
	unsigned int n;
	n = line1->getNumOfElements();
	ASSERT_EQ(3,n);
	n = line2->getNumOfElements();
	ASSERT_EQ(6,n);
}

/** 
 * Œrodowisko testowe 2
 * Tworzone trzy rózne linie, obrazek testowy
 */ 
class C_LineApprox_Test2 : public ::testing::Test {
protected:
	C_LineInterp *line1;	// linia pozioma
	C_LineInterp *line2;	// inna
	C_LineInterp *pionowa;	// linia pionowa
	C_LineInterp *line3;
	C_Matrix_Container obraz;
	unsigned int siz[3];
	C_Point P01;
	C_Point P11;

	C_LineApprox_Test2::C_LineApprox_Test2()
	{
		// wczytanie obrazka
		obraz.ReadBinary("getInterpolated_data.dat");
		siz[0] = obraz._rows;siz[1]=obraz._cols;siz[2]=0;
	}

	virtual ~C_LineApprox_Test2() {
		SAFE_DELETE(line1);
		SAFE_DELETE(line2);
		SAFE_DELETE(pionowa);
	}

	virtual void SetUp() {
		
		P01.setPoint(5,35);
		P11.setPoint(75,35);
		line1 = new C_LineInterp(SPLINE,P01,P11,obraz.data,siz);
		line2 = new C_LineInterp(SPLINE,-2,10,NORMALNA,obraz.data,siz);
		pionowa = new C_LineInterp(SPLINE,0,5,PIONOWA,obraz.data,siz);
		line3 = new C_LineInterp;	// konstruktor domyœlny a póŸniej setline
	}

	virtual void TearDown() {
		SAFE_DELETE(line1);
		SAFE_DELETE(line2);
		SAFE_DELETE(pionowa);
		SAFE_DELETE(line3);
	}
};

/// test interpolacji getInterpolated_data
TEST_F(C_LineApprox_Test2, getInterpolated_data) {
	const double *result;
	C_DumpAll dump("getInterpolated_data.out");
	std::cout << "\n\t" << "Wyniki do porownania w Matlabie getInterpolated_data_result.out" << "\n\n";
	C_Matrix_Container outx(1,71);
	C_Matrix_Container outy(1,71);
	line1->getPointsOnLine(P01,P11,outx.data,outy.data,71);
	result = line1->getInterpolated_data();
	
	dump.AddEntry(&outx,"getInterpolated_data_outx");
	dump.AddEntry(&outy,"getInterpolated_data_outy");	
	dump.AddEntry(result,71,"getInterpolated_data_result");
}

/// test interpolacji getInterpolated_data - inna inicjalizacja
TEST_F(C_LineApprox_Test2, getInterpolated_data_case1) {
	const double *result;
	C_DumpAll dump("getInterpolated_data_case1.out");
	std::cout << "\n\t" << "Wyniki do porownania w Matlabie getInterpolated_data_result_case1.out" << "\n\n";
	C_Matrix_Container outx(1,71);
	C_Matrix_Container outy(1,71);
	line3->ManualConstructor(SPLINE,P01,P11,obraz.data,siz);
	line3->getPointsOnLine(P01,P11,outx.data,outy.data,71);
	result = line3->getInterpolated_data();

	dump.AddEntry(&outx,"getInterpolated_data_outx_case1");
	dump.AddEntry(&outy,"getInterpolated_data_outy_case1");	
	dump.AddEntry(result,71,"getInterpolated_data_result_case1");
}

/// test interpolacji getInterpolated_data - pionowa
TEST_F(C_LineApprox_Test2, getInterpolated_data_case3) {
	const double *result;
	C_DumpAll dump("getInterpolated_data_case3.out");
	std::cout << "\n\t" << "Wyniki do porownania w Matlabie getInterpolated_data_result_case3.out" << "\n\n";
	C_Matrix_Container outx(1,41);
	C_Matrix_Container outy(1,41);
	C_Point P0(5,10);
	C_Point P1(5,50);
	line3->ManualConstructor(SPLINE,P0,P1,obraz.data,siz);
	bool ret = line3->getPointsOnLine(P0,P1,outx.data,outy.data,41);
	result = line3->getInterpolated_data();
	ASSERT_TRUE(ret);
	dump.AddEntry(&outx,"getInterpolated_data_outx_case3");
	dump.AddEntry(&outy,"getInterpolated_data_outy_case3");	
	dump.AddEntry(result,41,"getInterpolated_data_result_case3");
}

/// test interpolacji getInterpolated_data - u¿ycie getInterpolated_data bez interpolacji
TEST_F(C_LineApprox_Test2, getInterpolated_data_case2) {
	const double *result;
	const double *x;
	const double *y;

	result = line1->getInterpolated_data();
	x = line1->getInterpolated_X();
	y = line1->getInterpolated_Y();
	ASSERT_EQ(NULL,result);
	ASSERT_EQ(NULL,x);
	ASSERT_EQ(NULL,y);
}

/// test interpolacji getInterpolated_x oraz getInterpolated_y
TEST_F(C_LineApprox_Test2, getInterpolated_xy) {
	const double *x;
	const double *y;

	C_Matrix_Container outx(1,71);
	C_Matrix_Container outy(1,71);
	line1->getPointsOnLine(P01,P11,outx.data,outy.data,71);
	x = line1->getInterpolated_X();
	y = line1->getInterpolated_Y();
	// x i y powinny byæ kopi¹ outx i out y
	for(int a=0;a<71;a++)
	{
		ASSERT_DOUBLE_EQ(outx.GetPixel(0,a), x[a]) << "Vectors x and differ at index " << a;
		ASSERT_DOUBLE_EQ(outy.GetPixel(0,a), y[a]) << "Vectors y and differ at index " << a;
	}
}

