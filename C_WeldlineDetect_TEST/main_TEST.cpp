/** 
* \test G³ówny test testuj¹cy ca³¹ procedurê.
 */

#include "stdafx.h"

#if GTEST_HAS_PARAM_TEST
using ::testing::TestWithParam;
using ::testing::Values;

/** 
 * Przygotowanie testu parametrycznego - Parametrem s¹ nazwy plików z obrazami do obrobienia
 */
class C_LinearWeld_Start : public TestWithParam<const char *> {
public:
	C_LinearWeld_Start()
	{
		rtg = new C_Matrix_Container();

	}
	virtual ~C_LinearWeld_Start()
	{
		SAFE_DELETE(rtg);
	}
	virtual void SetUp()
	{
		nazwa = GetParam();
		rtg->ReadBinary(nazwa);
		obj = new C_LinearWeld(rtg);		
	}
	virtual void TearDown() {
		SAFE_DELETE(obj);
	}
protected:
	C_Matrix_Container *rtg;
	C_LinearWeld *obj;// g³ówny obiekt testowy
// 	void access_fillBuffor(C_LinearWeld *obj)
// 	{
// 		obj->fillBuffor();
// 	}
// 	void access_interpaprox(C_LinearWeld *obj, C_CircBuff<C_LineWeldApprox> **approx,C_CircBuff<C_LineInterp> **interp)
// 	{
// 		*(approx) = &obj->approx_results;
// 		*(interp) = &obj->interp_lines;
// 	}
// 	void access_pwublb(C_LinearWeld *obj, double **p,double **ub,double **lb,double **w)
// 	{
// 		*(p) = obj->_p;
// 		*(ub) = obj->_ub;
// 		*(lb) = obj->_lb;
// 		*(w) = obj->_w;
// 	} 
// 	bool access_evalNextParams(C_LinearWeld *obj){
// 		return obj->evalNextParams();
// 	}
// 	void access_evalWeldPos(C_LinearWeld *obj,const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )
// 	{
// 		obj->evalWeldPos(_approx, _interp, _weldPos );
// 	}
// 	C_CircBuff<C_LineWeldApprox> *approx;
// 	C_CircBuff<C_LineInterp> *interp;
 	const char *nazwa;
};
#endif

/// test start
TEST_P(C_LinearWeld_Start, DISABLED_start) {
	_RPT0(_CRT_WARN,"------ Entering Test start ------\n");
	_RPT1(_CRT_WARN,"PAR: %s\n",nazwa);
	std::string casename(nazwa);
	std::string nazwa_out("C_LinearWeld_Start_start_");
	nazwa_out+=casename;
	nazwa_out+=".out";
	std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa_out <<"\n\n";
	C_DumpAll dump(nazwa_out.c_str());
	C_Matrix_Container lineok;
	C_Matrix_Container weldpos;

	bool data;
	C_WeldPos wp;

	const vector<bool> *_lineOK;
	const vector<C_WeldPos> *_weldPos;

	bool ret;

	C_Point cp_x_start(10,0);	// punkt startowy
	obj->SetProcedureParameters(50,cp_x_start); // inicjalizacja srodowiska, wielkosc bufora 100
	ret = obj->Start(1,0);	// krok
	ASSERT_TRUE(ret);	

	_lineOK = obj->getLineOK();
	_weldPos = obj->getweldPos();

	// kopiowanie wybników do C_Marix_Container
	lineok.AllocateData(1,(unsigned int)_lineOK->size());
	weldpos.AllocateData(6,(unsigned int)_weldPos->size()); // wyniki dla jednej linii wszystkie 6 punktów w jednej kolumnie w kolejnoœci
	// [Dx
	//	Dy
	//	Sx
	//	Sy
	//	Gx
	//	Gy]


	for(unsigned int l=0;l<_lineOK->size();++l)
	{
		data = _lineOK->at(l);
		lineok.SetPixel(0,l,(double)data);

		wp = _weldPos->at(l);
		weldpos.SetPixel(0,l, wp.D.getX());
		weldpos.SetPixel(1,l, wp.D.getY());
		weldpos.SetPixel(2,l, wp.S.getX());
		weldpos.SetPixel(3,l, wp.S.getY());
		weldpos.SetPixel(4,l, wp.G.getX());
		weldpos.SetPixel(5,l, wp.G.getY());
	}

	dump.AddEntry(&lineok,"lineok");
	dump.AddEntry(&weldpos,"weldpos");
}

/// test start dla kroku 10
TEST_P(C_LinearWeld_Start, DISABLED_start10) {
	_RPT0(_CRT_WARN,"------ Entering Test start10 ------\n");
	_RPT1(_CRT_WARN,"PAR: %s\n",nazwa);
	std::string casename(nazwa);
	std::string nazwa_out("C_LinearWeld_Start_start10_");
	nazwa_out+=casename;
	nazwa_out+=".out";
	std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa_out <<"\n\n";
	C_DumpAll dump(nazwa_out.c_str());
	C_Matrix_Container lineok;
	C_Matrix_Container weldpos;

	bool data;
	C_WeldPos wp;

	const vector<bool> *_lineOK;
	const vector<C_WeldPos> *_weldPos;

	bool ret;

	C_Point cp_x_start(10,0);	// punkt startowy
	obj->SetProcedureParameters(50,cp_x_start); // inicjalizacja srodowiska, wielkosc bufora 100
	ret = obj->Start(10,0);	// krok
	ASSERT_TRUE(ret);	

	_lineOK = obj->getLineOK();
	_weldPos = obj->getweldPos();

	// kopiowanie wybników do C_Marix_Container
	lineok.AllocateData(1,(unsigned int)_lineOK->size());
	weldpos.AllocateData(6,(unsigned int)_weldPos->size()); // wyniki dla jednej linii wszystkie 6 punktów w jednej kolumnie w kolejnoœci
	// [Dx
	//	Dy
	//	Sx
	//	Sy
	//	Gx
	//	Gy]


	for(unsigned int l=0;l<_lineOK->size();++l)
	{
		data = _lineOK->at(l);
		lineok.SetPixel(0,l,(double)data);

		wp = _weldPos->at(l);
		weldpos.SetPixel(0,l, wp.D.getX());
		weldpos.SetPixel(1,l, wp.D.getY());
		weldpos.SetPixel(2,l, wp.S.getX());
		weldpos.SetPixel(3,l, wp.S.getY());
		weldpos.SetPixel(4,l, wp.G.getX());
		weldpos.SetPixel(5,l, wp.G.getY());
	}

	dump.AddEntry(&lineok,"lineok");
	dump.AddEntry(&weldpos,"weldpos");
}
/// test start dla kroku 100
TEST_P(C_LinearWeld_Start, DISABLED_start100) {
	_RPT0(_CRT_WARN,"------ Entering Test start100 ------\n");
	_RPT1(_CRT_WARN,"PAR: %s\n",nazwa);
	std::string casename(nazwa);
	std::string nazwa_out("C_LinearWeld_Start_start100_");
	nazwa_out+=casename;
	nazwa_out+=".out";
	std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa_out <<"\n\n";
	C_DumpAll dump(nazwa_out.c_str());
	C_Matrix_Container lineok;
	C_Matrix_Container weldpos;

	bool data;
	C_WeldPos wp;

	const vector<bool> *_lineOK;
	const vector<C_WeldPos> *_weldPos;

	bool ret;

	C_Point cp_x_start(10,0);	// punkt startowy
	obj->SetProcedureParameters(50,cp_x_start); // inicjalizacja srodowiska, wielkosc bufora 100
	ret = obj->Start(100,0);	// krok
	ASSERT_TRUE(ret);	

	_lineOK = obj->getLineOK();
	_weldPos = obj->getweldPos();

	// kopiowanie wybników do C_Marix_Container
	lineok.AllocateData(1,(unsigned int)_lineOK->size());
	weldpos.AllocateData(6,(unsigned int)_weldPos->size()); // wyniki dla jednej linii wszystkie 6 punktów w jednej kolumnie w kolejnoœci
	// [Dx
	//	Dy
	//	Sx
	//	Sy
	//	Gx
	//	Gy]


	for(unsigned int l=0;l<_lineOK->size();++l)
	{
		data = _lineOK->at(l);
		lineok.SetPixel(0,l,(double)data);

		wp = _weldPos->at(l);
		weldpos.SetPixel(0,l, wp.D.getX());
		weldpos.SetPixel(1,l, wp.D.getY());
		weldpos.SetPixel(2,l, wp.S.getX());
		weldpos.SetPixel(3,l, wp.S.getY());
		weldpos.SetPixel(4,l, wp.G.getX());
		weldpos.SetPixel(5,l, wp.G.getY());
	}

	dump.AddEntry(&lineok,"lineok");
	dump.AddEntry(&weldpos,"weldpos");
}
/// test start dla delta 0
TEST_P(C_LinearWeld_Start, DISABLED_start0) {
	_RPT0(_CRT_WARN,"------ Entering Test start0 ------\n");
	_RPT1(_CRT_WARN,"PAR: %s\n",nazwa);
	bool ret;

	C_Point cp_x_start(10,0);	// punkt startowy
	obj->SetProcedureParameters(100,cp_x_start); // inicjalizacja srodowiska, wielkosc bufora 100
	ret = obj->Start(0,0);	// krok
	ASSERT_FALSE(ret);	
}

INSTANTIATE_TEST_CASE_P(
	start,
	C_LinearWeld_Start,
	::testing::Values("testimag1.dat","testimag2.dat","testimag3.dat","testimag4.dat","testimag5.dat","testimag6.dat","testimag7.dat","testimag8.dat","testimag9.dat")); // te same wartosci sa w matlabie