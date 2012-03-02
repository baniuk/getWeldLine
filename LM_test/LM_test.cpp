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
		GaussLin(data.data, y.data, 0,x.GetNumofElements(), (void*)&X);
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
		jGaussLin(data.data, y.data, 0, x.GetNumofElements(), (void*)&X);
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
		jGaussLin(data.data, y.data, 0, x.GetNumofElements(), (void*)&X);
		jGaussLin_test_2.AddEntry(&y,"data");
		
	}


	return 0;
}

