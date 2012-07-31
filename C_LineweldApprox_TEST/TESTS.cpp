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
	C_MATRIX_LOAD(p,"getLineApproxGaussLin_test_1_p.dat"); // parametry aproxymowanej krzywej
	C_MATRIX_LOAD(lb,"getLineApproxGaussLin_test_1_lb.dat");
	C_MATRIX_LOAD(ub,"getLineApproxGaussLin_test_1_ub.dat");
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
	C_MATRIX_LOAD(x,"getLineApproxGaussLin_test_1_x.dat"); // dane wejsciowe
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
 	std::cout << "\n\t" << "Wyniki do porownania w Matlabie getLineApproxGaussLin_test_1_expected.out" << "\n\n";
 	EXPECT_NEAR(761.1147,p.data[0],1e-4);
 	EXPECT_NEAR(1.079115936954478e+003,p.data[1],1e-4);
 	EXPECT_NEAR(1.152311635531913e+002,p.data[2],1e-3);
 	EXPECT_NEAR(-1.063228816386850e-002,p.data[3],1e-2);
 	EXPECT_NEAR(8.148828860093591e+002,p.data[4],1e-3);

}
/// test konstruktora
TEST(C_LineweldApprox_Test1,construct)
{
	const double *p;
	const double *ub;
	const double *lb;
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
	C_MATRIX_LOAD(x,"getLineApproxGaussLin_test_1_x.dat");
	C_LineWeldApprox obj;
	obj.ManualConstructor(typeGaussLin,profil.data,x.data,x._cols);
	p = obj.getApproxParams_p();
	ub = obj.getApproxParams_ub();
	lb = obj.getApproxParams_lb();
	ASSERT_EQ(1060,p[A]);
	ASSERT_EQ(x._cols/2,p[B]);
	ASSERT_EQ(160,p[C]);
	ASSERT_EQ(0,p[D]);
	ASSERT_EQ(0,p[E]);
}

/// test setParam
TEST(C_LineweldApprox_Test1,setParam)
{
	const double *p;
	const double *ub;
	const double *lb;
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
	C_MATRIX_LOAD(x,"getLineApproxGaussLin_test_1_x.dat");
	double pset[5] = {1,2,3,4,5};
	double ubset[5] = {6,7,8,9,10};
	double lbset[5] = {9,8,7,6,5};
	C_Matrix_Container w(1,x._cols);
	for(int a=0;a<x._cols;a++)
		w.SetPixel(0,a,a);

	C_LineWeldApprox obj;
	obj.ManualConstructor(typeGaussLin,profil.data,x.data,x._cols);
	obj.setApproxParmas(pset,w.data,ubset,lbset,NULL);

	p = obj.getApproxParams_p();
	ub = obj.getApproxParams_ub();
	lb = obj.getApproxParams_lb();

	ASSERT_EQ(1,p[A]);
	ASSERT_EQ(2,p[B]);
	ASSERT_EQ(3,p[C]);
	ASSERT_EQ(4,p[D]);
	ASSERT_EQ(5,p[E]);

	ASSERT_EQ(6,ub[A]);
	ASSERT_EQ(7,ub[B]);
	ASSERT_EQ(8,ub[C]);
	ASSERT_EQ(9,ub[D]);
	ASSERT_EQ(10,ub[E]);

	ASSERT_EQ(9,lb[A]);
	ASSERT_EQ(8,lb[B]);
	ASSERT_EQ(7,lb[C]);
	ASSERT_EQ(6,lb[D]);
	ASSERT_EQ(5,lb[E]);
}

/// test wysokopoziomowej funkcji aproksymuj¹cej
TEST(C_LineweldApprox_Test1,getApprox)
{
	const double *p;
	const double *ub;
	const double *lb;
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
	C_MATRIX_LOAD(x,"getLineApproxGaussLin_test_1_x.dat"); // dane wejsciowe
	C_LineWeldApprox obj;
	obj.ManualConstructor(typeGaussLin,profil.data,x.data,x._cols);
	obj.setApproxParmas(NULL,NULL,NULL,NULL,NULL);
	obj.getLineApprox(100);
	// p[arametry
	p = obj.getApproxParams_p();
	ub = obj.getApproxParams_ub();
	lb = obj.getApproxParams_lb();

	std::cout << "err: "<< sqrt(obj.getInfo(err))<<"\n";

	EXPECT_NEAR(761.1147,p[A],1e-4);
	EXPECT_NEAR(1.079115936954478e+003,p[B],1e-4);
	EXPECT_NEAR(1.152311635531913e+002,p[C],1e-3);
	EXPECT_NEAR(-1.063228816386850e-002,p[D],1e-2);
	EXPECT_NEAR(8.148828860093591e+002,p[E],1e-3);
	

}

/// test funkcji evalApproxFcn
TEST(C_LineweldApprox_Test1, evalApproxFcn_Case_1) {
	C_MATRIX_LOAD(data,"GaussLin_test_1.dat"); // parametry funkcji
	C_MATRIX_LOAD(x,"GaussLin_test_1_x.dat"); // dane wejsciowe
	C_MATRIX_LOAD(expected,"GaussLin_test_1_expected.dat"); // wyniki
	C_MATRIX_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji (nie uzywany tu)
	C_Matrix_Container y; //wyjœcie - wartoœci funkcji
	y.AllocateData(1,x.GetNumofElements());

	C_LineWeldApprox obj;
	obj.ManualConstructor(typeGaussLin,profil.data,x.data,x._cols);
	obj.setApproxParmas(data.data,NULL,NULL,NULL,NULL);
// 	for(int a=0;a<x.GetNumofElements();++a)
// 		y.data[a] = obj.evalApproxFcn(x.data[a]);
	obj.evalApproxFcnVec(x.data,y.data,x.GetNumofElements());
	for(unsigned int a=0;a<x._cols;a++)
		EXPECT_FLOAT_EQ(expected.data[a],y.data[a]);
}

/// test funkcji Ranged rand - bez inicjalizacji generatora - ka¿de uruchomienie takie same
TEST(C_LineweldApprox_Test1, RangedRand_Case_1) {
	C_LineWeldApprox obj;
	int n = 10;
	char out[10];

	obj.RangedRand(65,91,10,out);

	std::cout << "RandomStr: ";
	for(int a=0;a<n;a++)
		std::cout << out[a];
	std::cout << "\n";

}

/// test funkcji Ranged rand - z inicjalizacj¹ generatora
TEST(C_LineweldApprox_Test1, RangedRand_Case_2) {
	C_LineWeldApprox obj;
	int n = 10;
	char out[10];

	srand( (unsigned)time( NULL ) );
	obj.RangedRand(65,91,10,out);

	std::cout << "RandomStr: ";
	for(int a=0;a<n;a++)
		std::cout << out[a];
	std::cout << "\n";

}