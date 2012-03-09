// LM_test.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <tchar.h>
#include "C_Levmar_Warp.h"
#include "C_LineWeldApprox.h"
#include <C_Matrix_Container.h>
#include <C_Image_Container.h>
#include <C_Error.h>
#include <C_DumpAll.h>

int _tmain(int argc, _TCHAR* argv[])
{

	
	
	/// getLineApproxGaussLin TestCase4_1
	/** Test funkcji aproxymuj¹cej profil */
	{
		xtradata X;
		double info[LM_INFO_SZ];
		int ret;
		C_DumpAll getLineApproxGaussLin_test_1("getLineApproxGaussLin_test_1.out");
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
		getLineApproxGaussLin_test_1.AddEntry(&p,"p");
		getLineApproxGaussLin_test_1.AddEntry(info,LM_INFO_SZ,"info");
		getLineApproxGaussLin_test_1.AddEntry(&profil,"profil");
	}
		/// getLineApproxGaussLinWeighted TestCase5_1
		/** Test funkcji aproxymuj¹cej profil High level */
/*	{
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
*/

	return 0;
}

