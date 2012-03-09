#include "stdafx.h"

/// test funkcji GaussLin
TEST(C_LineweldApprox_Test1, GaussLin_Case_1) {
	xtradata X;
	C_MATRIX_LOAD(data,"GaussLin_test_1.dat"); // parametry funkcji
	C_MATRIX_LOAD(x,"GaussLin_test_1_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(expected,"GaussLin_test_1_expected.dat"); // wyniki
	C_Matrix_Container y; //wyjœcie - wartoœci funkcji
	y.AllocateData(1,x.GetNumofElements());
	X.x = x.data;

	GaussLin(data.data, y.data, 5,x.GetNumofElements(), (void*)&X);
	for(unsigned int a=0;a<x._cols;a++)
		EXPECT_FLOAT_EQ(expected.data[a],y.data[a]);
}

/// test funkcji jGaussLin
TEST(C_LineweldApprox_Test1, jGaussLin_Case_1) {
	xtradata X;
	C_MATRIX_LOAD(data,"jGaussLin_test_1.dat"); // parametry funkcji
	C_MATRIX_LOAD(x,"jGaussLin_test_1_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(expected,"jGaussLin_test_1_expected.dat"); // wyniki
	C_Matrix_Container y; //wyjœcie - wartoœci funkcji
	y.AllocateData(x.GetNumofElements(),5);
	X.x = x.data; // przypisanie danych dziedziny do struktury

	jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
	for(unsigned int a=0;a<x._cols;a++)
		EXPECT_FLOAT_EQ(expected.data[a],y.data[a]);	
}

/// test funkcji jGaussLin
TEST(C_LineweldApprox_Test1, jGaussLin_Case_2) {
	xtradata X;
	C_MATRIX_LOAD(data,"jGaussLin_test_2.dat"); // parametry funkcji
	C_MATRIX_LOAD(x,"jGaussLin_test_2_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(expected,"jGaussLin_test_2_expected.dat"); // wyniki
	C_Matrix_Container y; //wyjœcie - wartoœci funkcji
	y.AllocateData(x.GetNumofElements(),5);
	X.x = x.data; // przypisanie danych dziedziny do struktury

	jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
	for(unsigned int a=0;a<x._cols;a++)
		EXPECT_FLOAT_EQ(expected.data[a],y.data[a]);	
}
/// test funkcji jGaussLin
TEST(C_LineweldApprox_Test1, jGaussLin_Case_3) {
	xtradata X;
	C_MATRIX_LOAD(data,"jGaussLin_test_3.dat"); // parametry funkcji
	C_MATRIX_LOAD(x,"jGaussLin_test_3_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(expected,"jGaussLin_test_3_expected.dat"); // wyniki
	C_Matrix_Container y; //wyjœcie - wartoœci funkcji
	y.AllocateData(x.GetNumofElements(),5);
	X.x = x.data; // przypisanie danych dziedziny do struktury

	jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
	for(unsigned int a=0;a<x._cols;a++)
		EXPECT_FLOAT_EQ(expected.data[a],y.data[a]);	
}
/// test niskopoziomowej funkcji aproksymuj¹cej
TEST(C_LineweldApprox_Test1,getLineApproxGaussLin)
{
	xtradata X;
	double info[LM_INFO_SZ];
	int ret;
	C_DumpAll getLineApproxGaussLin_test_1("getLineApproxGaussLin_test_1_expected.out");
	C_MATRIX_LOAD(lb,"getLineApproxGaussLin_test_1_lb.dat");
	C_MATRIX_LOAD(ub,"getLineApproxGaussLin_test_1_ub.dat");
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
	C_MATRIX_LOAD(x,"getLineApproxGaussLin_test_1_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(p,"getLineApproxGaussLin_test_1_p.dat"); // parametry aproxymowanej krzywej
	X.x = x.data;	// przypisanie danych dziedziny do struktury
	ret = getLineApproxGaussLin(p.data,
		profil.data,
		p._cols,
		profil._cols,
		lb.data,
		ub.data,
		100,
		NULL,
		info,
		X);
	getLineApproxGaussLin_test_1.AddEntry(&p,"expected");
	std::cout << "\n\t" << "Wyniki do porownania w Matlabie getInterpolated_data.out" << "\n\n";
	EXPECT_NEAR(761.1147,p.data[0],1e-4);
	EXPECT_NEAR(1.079115936954478e+003,p.data[1],1e-4);
	EXPECT_NEAR(1.152311635531913e+002,p.data[2],1e-3);
	EXPECT_NEAR(-1.063228816386850e-002,p.data[3],1e-2);
	EXPECT_NEAR(8.148828860093591e+002,p.data[4],1e-3);

}