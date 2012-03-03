// LM_test.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <tchar.h>
#include "Levmar_prototypes.h"
#include <C_Matrix_Container.h>
#include <C_Image_Container.h>
#include <C_Error.h>
#include <C_DumpAll.h>

int _tmain(int argc, _TCHAR* argv[])
{
	/// GaussLin Test 1
	/** Test funkcji obliczaj¹cej Gaussa*/
	{
		xtradata X;
		C_DumpAll GaussLin_test_1("GaussLin_test_1.out");
		C_DUMP_LOAD(data,"GaussLin_test_1.dat"); // parametry funkcji
		C_DUMP_LOAD(x,"GaussLin_test_1_x.dat"); // dane wejsciowe
		C_Matrix_Container y; //wyjœcie - wartoœci funkcji
		y.AllocateData(1,x.GetNumofElements());
		X.x = x.data;
		GaussLin(data.data, y.data, 5,x.GetNumofElements(), (void*)&X);
		GaussLin_test_1.AddEntry(&y,"data");
	}
	/// getApproxLine Test 1
	/** Test procedury aproxksymuj¹cej pojedyñczy profil*/
	{
		C_DumpAll getApproxLine_test_1("getApproxLine_test_1.out");
		C_Matrix_Container data;
		data.ReadBinary("getApproxLine_test_1.dat");
		getApproxLine_test_1.AddEntry(&data,"data");
	}
	/// jGaussLin TestCase_3
	/** Test funkcji obliczaj¹cej jakobian*/
	{
		xtradata X;
		C_DumpAll jGaussLin_test_1("jGaussLin_test_1.out");
		C_DUMP_LOAD(data,"jGaussLin_test_1.dat"); // parametry funkcji
		C_DUMP_LOAD(x,"jGaussLin_test_1_x.dat"); // dane wejsciowe
		C_Matrix_Container y; //wyjœcie - wartoœci funkcji
		y.AllocateData(x.GetNumofElements(),5);
		X.x = x.data; // przypisanie danych dziedziny do struktury
		jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
		jGaussLin_test_1.AddEntry(&y,"data");
	}
	/// jGaussLin TestCase_32
	/** Test funkcji obliczaj¹cej jakobian*/
	{
		xtradata X;
		C_DumpAll jGaussLin_test_2("jGaussLin_test_2.out");
		C_DUMP_LOAD(data,"jGaussLin_test_2.dat"); // parametry funkcji
		C_DUMP_LOAD(x,"jGaussLin_test_2_x.dat"); // dane wejsciowe
		C_Matrix_Container y; //wyjœcie - wartoœci funkcji
		y.AllocateData(x.GetNumofElements(),5);
		X.x = x.data; // przypisanie danych dziedziny do struktury
		jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
		jGaussLin_test_2.AddEntry(&y,"data");
	}
	/// jGaussLin TestCase_33
	/** Test funkcji obliczaj¹cej jakobian*/
	{
		xtradata X;
		C_DumpAll jGaussLin_test_3("jGaussLin_test_3.out");
		C_DUMP_LOAD(data,"jGaussLin_test_3.dat"); // parametry funkcji
		C_DUMP_LOAD(x,"jGaussLin_test_3_x.dat"); // dane wejsciowe
		C_Matrix_Container y; //wyjœcie - wartoœci funkcji
		y.AllocateData(x.GetNumofElements(),5);
		X.x = x.data; // przypisanie danych dziedziny do struktury
		jGaussLin(data.data, y.data, 5, x.GetNumofElements(), (void*)&X);
		jGaussLin_test_3.AddEntry(&y,"data");
	}
	/// getLineApproxGaussLin TestCase4_1
	/** Test funkcji aproxymuj¹cej profil */
	{
		xtradata X;
		double info[LM_INFO_SZ];
		int ret;
		C_DumpAll getLineApproxGaussLin_test_1("getLineApproxGaussLin_test_1.out");
		C_DUMP_LOAD(lb,"getLineApproxGaussLin_test_1_lb.dat");
		C_DUMP_LOAD(ub,"getLineApproxGaussLin_test_1_ub.dat");
		C_DUMP_LOAD(profil,"getLineApproxGaussLin_test_1.dat"); // profil do aproxymacji
		C_DUMP_LOAD(x,"getLineApproxGaussLin_test_1_x.dat"); // dane wejsciowe
		C_DUMP_LOAD(p,"getLineApproxGaussLin_test_1_p.dat"); // parametry aproxymowanej krzywej
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
		getLineApproxGaussLin_test_1.AddEntry(&p,"p");
		getLineApproxGaussLin_test_1.AddEntry(info,LM_INFO_SZ,"info");
		getLineApproxGaussLin_test_1.AddEntry(&profil,"profil");
	}
		/// getLineApproxGaussLinWeighted TestCase5_1
		/** Test funkcji aproxymuj¹cej profil High level */
	{
		double info[LM_INFO_SZ];
		int ret;
		C_DumpAll getLineApproxGaussLinWeighted_test_1("getLineApproxGaussLinWeighted_test_1.out");
		C_DUMP_LOAD(lb,"getLineApproxGaussLinWeighted_test_1_lb.dat");
		C_DUMP_LOAD(ub,"getLineApproxGaussLinWeighted_test_1_ub.dat");
		C_DUMP_LOAD(profil,"getLineApproxGaussLinWeighted_test_1.dat"); // profil do aproxymacji
		C_DUMP_LOAD(x,"getLineApproxGaussLinWeighted_test_1_x.dat"); // dane wejsciowe
		C_DUMP_LOAD(p,"getLineApproxGaussLinWeighted_test_1_p.dat"); // parametry aproxymowanej krzywej
		ret = getLineApproxGaussLinWeighted(NULL,
											&p,
											&profil,
											&x,
											&lb,
											&ub,
											100,
											NULL,
											info);
		getLineApproxGaussLinWeighted_test_1.AddEntry(&p,"p");
		getLineApproxGaussLinWeighted_test_1.AddEntry(info,LM_INFO_SZ,"info");
		getLineApproxGaussLinWeighted_test_1.AddEntry(&profil,"profil");
	}


	return 0;
}

