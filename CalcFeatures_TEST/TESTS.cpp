/** 
 * \test Testy procedury liczenia odleg³oœci defektu od spawu wewn¹trz cf na obrazie
 */
#include "stdafx.h"
#include "../dISAR_CalcFeatures_Ext/cf.cpp"
/** 
 * Klasa inicjalizuj¹ca testy funkcji cf - w szczególnoœci liczenia odleg³oœci od spawu.
 * Przetestowane zostanie zachowanie procedury dla ró¿nych punktów startowych, niekoniecznie le¿¹cych na idelanych spawach.
 */
class C_cf_weld_dist_test : public ::testing::Test {
public:
	C_cf_weld_dist_test()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("testimag9.dat");
	}

	virtual ~C_cf_weld_dist_test() {
		SAFE_DELETE(rtg);
	}

	virtual void SetUp() {
		obj = new C_LinearWeld(rtg);
	}

	virtual void TearDown() {
		SAFE_DELETE(obj);
	}
protected:
	C_Matrix_Container *rtg;
	C_LinearWeld *obj;// g³ówny obiekt testowy

};

/** 
 * Klasa inicjalizuj¹ca testy funkcji cf - w szczególnoœci liczenia odleg³oœci od spawu.
 * Przetestowane zostanie zachowanie procedury dla ró¿nych punktów startowych, niekoniecznie le¿¹cych na idelanych spawach.
 * Inny obraz wejœciowy
 */
class C_cf_weld_dist_test_1 : public ::testing::Test {
public:
	C_cf_weld_dist_test_1()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("100_01s.dat");
	}

	virtual ~C_cf_weld_dist_test_1() {
		SAFE_DELETE(rtg);
	}

	virtual void SetUp() {
		obj = new C_LinearWeld(rtg);
	}

	virtual void TearDown() {
		SAFE_DELETE(obj);
	}
protected:
	C_Matrix_Container *rtg;
	C_LinearWeld *obj;// g³ówny obiekt testowy

};
/// Test dla punktu startowego. Porównanie wyników w logach. Chodzi o sprawdzenie czy siê nie wiesza dla dziwnych punktów
TEST_F(C_cf_weld_dist_test, point1) {
	_RPT0(_CRT_WARN,"------ Entering Test point1 ------\n");

	C_WeldPos weldpos;
	C_Matrix_Container weldpos_out;
	bool ret;
	double inout;
	double dist_g,dist_s,dist_d,szerokosc_spawu;
	C_DumpAll dump("point1.out");

	// testowy unkt startowy
	C_Point cp_x_start(100,100);	// punkt startowy
	_RPT2(_CRT_WARN,"Start Point: [%.1lf,%.1lf]\n",cp_x_start.getX(),cp_x_start.getY());

	obj->SetProcedureParameters(1,cp_x_start);
	ret = obj->getOneApproxLine(weldpos);

	dist_g = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.G.getX(),weldpos.G.getY());
	dist_s = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.S.getX(),weldpos.S.getY());
	dist_d = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.D.getX(),weldpos.D.getY());
	szerokosc_spawu = getDistance(weldpos.D.getX(),weldpos.D.getY(), weldpos.G.getX(),weldpos.G.getY());

	_RPT4(_CRT_WARN,"\t\tG S D RET: [%.1lf, %.1lf, %.1lf, %u]",dist_g,dist_s,dist_d,ret);

	// zgrywnaie dla matlaba
	double x,y;
	x = cp_x_start.getX();
	y = cp_x_start.getY();
	dump.AddEntry(&x,1,"x");
	dump.AddEntry(&y,1,"y");

	weldpos_out.AllocateData(3,2); 
	weldpos_out.SetPixel(0,0, weldpos.D.getX());
	weldpos_out.SetPixel(0,1, weldpos.D.getY());
	weldpos_out.SetPixel(1,0, weldpos.S.getX());
	weldpos_out.SetPixel(1,1, weldpos.S.getY());
	weldpos_out.SetPixel(2,0, weldpos.G.getX());
	weldpos_out.SetPixel(2,1, weldpos.G.getY());
	dump.AddEntry(&weldpos_out,"poz");

	if(dist_d>szerokosc_spawu || dist_g>szerokosc_spawu)
		inout = 2;	// na zewn¹trz
	else
		inout = 1;
	dump.AddEntry(&inout,1,"inout");
}

TEST_F(C_cf_weld_dist_test, point2) {
	_RPT0(_CRT_WARN,"------ Entering Test point2 ------\n");

	C_WeldPos weldpos;
	C_Matrix_Container weldpos_out;
	bool ret;
	double inout;
	double dist_g,dist_s,dist_d,szerokosc_spawu;
	C_DumpAll dump("point2.out");

	// testowy unkt startowy
	C_Point cp_x_start(2389,181);	// punkt startowy
	_RPT2(_CRT_WARN,"Start Point: [%.1lf,%.1lf]\n",cp_x_start.getX(),cp_x_start.getY());

	obj->SetProcedureParameters(1,cp_x_start);
	ret = obj->getOneApproxLine(weldpos);

	dist_g = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.G.getX(),weldpos.G.getY());
	dist_s = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.S.getX(),weldpos.S.getY());
	dist_d = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.D.getX(),weldpos.D.getY());
	szerokosc_spawu = getDistance(weldpos.D.getX(),weldpos.D.getY(), weldpos.G.getX(),weldpos.G.getY());

	_RPT4(_CRT_WARN,"\t\tG S D RET: [%.1lf, %.1lf, %.1lf, %u]",dist_g,dist_s,dist_d,ret);

	// zgrywnaie dla matlaba
	double x,y;
	x = cp_x_start.getX();
	y = cp_x_start.getY();
	dump.AddEntry(&x,1,"x");
	dump.AddEntry(&y,1,"y");

	weldpos_out.AllocateData(3,2); 
	weldpos_out.SetPixel(0,0, weldpos.D.getX());
	weldpos_out.SetPixel(0,1, weldpos.D.getY());
	weldpos_out.SetPixel(1,0, weldpos.S.getX());
	weldpos_out.SetPixel(1,1, weldpos.S.getY());
	weldpos_out.SetPixel(2,0, weldpos.G.getX());
	weldpos_out.SetPixel(2,1, weldpos.G.getY());
	dump.AddEntry(&weldpos_out,"poz");

	if(dist_d>szerokosc_spawu || dist_g>szerokosc_spawu)
		inout = 2;	// na zewn¹trz
	else
		inout = 1;
	dump.AddEntry(&inout,1,"inout");
}

TEST_F(C_cf_weld_dist_test, point3) {
	_RPT0(_CRT_WARN,"------ Entering Test point3 ------\n");

	C_WeldPos weldpos;
	C_Matrix_Container weldpos_out;
	bool ret;
	double inout;
	double dist_g,dist_s,dist_d,szerokosc_spawu;
	C_DumpAll dump("point3.out");

	// testowy unkt startowy
	C_Point cp_x_start(4166,1029);	// punkt startowy
	_RPT2(_CRT_WARN,"Start Point: [%.1lf,%.1lf]\n",cp_x_start.getX(),cp_x_start.getY());

	obj->SetProcedureParameters(1,cp_x_start);
	ret = obj->getOneApproxLine(weldpos);

	dist_g = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.G.getX(),weldpos.G.getY());
	dist_s = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.S.getX(),weldpos.S.getY());
	dist_d = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.D.getX(),weldpos.D.getY());
	szerokosc_spawu = getDistance(weldpos.D.getX(),weldpos.D.getY(), weldpos.G.getX(),weldpos.G.getY());

	_RPT4(_CRT_WARN,"\t\tG S D RET: [%.1lf, %.1lf, %.1lf, %u]",dist_g,dist_s,dist_d,ret);

	// zgrywnaie dla matlaba
	double x,y;
	x = cp_x_start.getX();
	y = cp_x_start.getY();
	dump.AddEntry(&x,1,"x");
	dump.AddEntry(&y,1,"y");

	weldpos_out.AllocateData(3,2); 
	weldpos_out.SetPixel(0,0, weldpos.D.getX());
	weldpos_out.SetPixel(0,1, weldpos.D.getY());
	weldpos_out.SetPixel(1,0, weldpos.S.getX());
	weldpos_out.SetPixel(1,1, weldpos.S.getY());
	weldpos_out.SetPixel(2,0, weldpos.G.getX());
	weldpos_out.SetPixel(2,1, weldpos.G.getY());
	dump.AddEntry(&weldpos_out,"poz");

	if(dist_d>szerokosc_spawu || dist_g>szerokosc_spawu)
		inout = 2;	// na zewn¹trz
	else
		inout = 1;
	dump.AddEntry(&inout,1,"inout");
}

TEST_F(C_cf_weld_dist_test_1, point1) {
	_RPT0(_CRT_WARN,"------ Entering Test point1 ------\n");

	C_WeldPos weldpos;
	C_Matrix_Container weldpos_out;
	bool ret;
	double inout;
	double dist_g,dist_s,dist_d,szerokosc_spawu;
	C_DumpAll dump("point1_1.out");

	// testowy unkt startowy
	C_Point cp_x_start(767,100);	// punkt startowy
	_RPT2(_CRT_WARN,"Start Point: [%.1lf,%.1lf]\n",cp_x_start.getX(),cp_x_start.getY());

	obj->SetProcedureParameters(1,cp_x_start);
	ret = obj->getOneApproxLine(weldpos);

	dist_g = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.G.getX(),weldpos.G.getY());
	dist_s = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.S.getX(),weldpos.S.getY());
	dist_d = getDistance(cp_x_start.getX(),cp_x_start.getY(),weldpos.D.getX(),weldpos.D.getY());
	szerokosc_spawu = getDistance(weldpos.D.getX(),weldpos.D.getY(), weldpos.G.getX(),weldpos.G.getY());

	_RPT4(_CRT_WARN,"\t\tG S D RET: [%.1lf, %.1lf, %.1lf, %u]",dist_g,dist_s,dist_d,ret);

	// zgrywnaie dla matlaba
	double x,y;
	x = cp_x_start.getX();
	y = cp_x_start.getY();
	dump.AddEntry(&x,1,"x");
	dump.AddEntry(&y,1,"y");

	weldpos_out.AllocateData(3,2); 
	weldpos_out.SetPixel(0,0, weldpos.D.getX());
	weldpos_out.SetPixel(0,1, weldpos.D.getY());
	weldpos_out.SetPixel(1,0, weldpos.S.getX());
	weldpos_out.SetPixel(1,1, weldpos.S.getY());
	weldpos_out.SetPixel(2,0, weldpos.G.getX());
	weldpos_out.SetPixel(2,1, weldpos.G.getY());
	dump.AddEntry(&weldpos_out,"poz");

	if(dist_d>szerokosc_spawu || dist_g>szerokosc_spawu)
		inout = 2;	// na zewn¹trz
	else
		inout = 1;
	dump.AddEntry(&inout,1,"inout");
}