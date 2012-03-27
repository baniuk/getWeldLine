/** 
* \test Przygotowanie testu parametrycznego - wypelnianie bufora. Parametrem s¹ró¿ne pozycje punktu startowego
 */
#include "stdafx.h"

#if GTEST_HAS_PARAM_TEST
using ::testing::TestWithParam;
using ::testing::Values;

/** 
 * Przygotowanie testu parametrycznego - wypelnianie bufora. 
 * Parametrem s¹ró¿ne pozycje punktu startowego
 */
class C_LinearWeld_FillBuffor : public TestWithParam<int> {
public:
	C_LinearWeld_FillBuffor()
	{
		rtg = new C_Matrix_Container();
		rtg->ReadBinary("testimag9.dat");
	}
	virtual ~C_LinearWeld_FillBuffor()
	{
		SAFE_DELETE(rtg);
	}
	virtual void SetUp()
	{
		// setup C_LinearWeld
		obj = new C_LinearWeld(rtg);
		x_start = GetParam();
	}
	virtual void TearDown() {
		SAFE_DELETE(obj);
	}
protected:
	C_Matrix_Container *rtg;
	C_LinearWeld *obj;// g³ówny obiekt testowy
	void access_fillBuffor(C_LinearWeld *obj)
	{
		obj->fillBuffor();
	}
	void access_interpaprox(C_LinearWeld *obj, C_CircBuff<C_LineWeldApprox> **approx,C_CircBuff<C_LineInterp> **interp,C_CircBuff<double> **rec)
	{
		*(approx) = &obj->approx_results;
		*(interp) = &obj->interp_lines;
		*(rec) = &obj->recalculated_approx_data;
	}
	void access_pwublb(C_LinearWeld *obj, double **p,double **ub,double **lb,double **w)
	{
		*(p) = obj->_p;
		*(ub) = obj->_ub;
		*(lb) = obj->_lb;
		*(w) = obj->_w;
	} 
	bool access_evalNextParams(C_LinearWeld *obj){
		return obj->evalNextParams();
	}
	void access_evalWeldPos(C_LinearWeld *obj,const C_LineWeldApprox *_approx, const C_LineInterp *_interp, const double *_pre, C_WeldPos &_weldPos )
	{
		obj->evalWeldPos(_approx, _interp, _pre, _weldPos );
	}
	C_CircBuff<C_LineWeldApprox> *approx;
	C_CircBuff<C_LineInterp> *interp;
	int x_start;
};

#endif //GTEST_HAS_PARAM_TEST

/// test fillBuffor - wype³aniam bufor i sprawdzam aproksymacje dla ró¿nych punktów startowych oraz evalNextParam
TEST_P(C_LinearWeld_FillBuffor, DISABLED_fillBuffor) {
	_RPT0(_CRT_WARN,"------ Entering Test C_LinearWeld_FillBuffor_case1 ------ ");
	_RPT1(_CRT_WARN,"PAR: %d\n",x_start);
	char buff[6];_itoa_s(x_start,buff,6,10);std::string liczba(buff);
	std::string nazwa("C_LinearWeld_FillBuffor");
	nazwa+=liczba;
	nazwa+=".out";
	std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa <<"\n\n";
	C_DumpAll dump(nazwa.c_str());

	C_CircBuff<double> *rec; // nie u¿ywane tu

	C_LineInterp *tmp;
	int ile_linii = 25;
	C_Point cp_x_start(x_start,0);
	obj->SetProcedureParameters(ile_linii,cp_x_start); // start of x=10, ile_linii lini w buforze
	// start
	access_fillBuffor(obj);
	// pobieral adresy struktur z danymi
	access_interpaprox(obj,&approx,&interp,&rec);
	// sprawdzam czy bufor pe³en - powinien byc ze wzgledu na sposób dzia³ania fillbuffor
	ASSERT_TRUE(approx->Czy_pelny());
	ASSERT_TRUE(interp->Czy_pelny());
	// czy ma tyle elementów co trzeba
	ASSERT_EQ(ile_linii,approx->getNumElem());
	ASSERT_EQ(ile_linii,interp->getNumElem());
	// czy pionowe
	tmp = interp->GetFirstInitialized();
	ASSERT_EQ(PIONOWA,tmp->getjest_pion());

	// test funkcji evalnextparams
	double *pp_med,*pud_med,*plb_med,*pw;	// wskaŸniki do  dostêpu przez eccess_pwubld
	C_Matrix_Container p_med(1,5); // mediana  z ca³ego bufora
	C_Matrix_Container ub_med(1,5); 
	C_Matrix_Container lb_med(1,5); 
	C_Matrix_Container w(1,interp->GetFirstInitialized()->getSizeofInterpData()); // srednia z aproxymacjiz ca³ego bufora
	access_evalNextParams(obj);
	access_pwublb(obj,&pp_med,&pud_med,&plb_med,&pw); // dostêp do wskaŸników paramerów prywatnych
	p_med.CopyfromTab(pp_med,5); // kopiowanie ze wskaŸników do obiektów C_Matrix_Container
	ub_med.CopyfromTab(pud_med,5);
	lb_med.CopyfromTab(plb_med,5);
	w.CopyfromTab(pw,interp->GetFirstInitialized()->getSizeofInterpData());

	// wyniki zwracane na zewn¹trz
	// 
	// zrzucanie wyników - w rzêdach kolejne wyniki
	// zrucam wyniki p pod postaci¹ wsp³ó³czynników
	// dane x i y interpolacji
	// wartoœci interpolownego profilu profil
	C_Matrix_Container x(interp->getNumElem(),interp->GetFirstInitialized()->getSizeofInterpData());
	C_Matrix_Container y(interp->getNumElem(),interp->GetFirstInitialized()->getSizeofInterpData());
	C_Matrix_Container profil(interp->getNumElem(),interp->GetFirstInitialized()->getSizeofInterpData());
	C_Matrix_Container p(approx->getNumElem(),5);
	C_Matrix_Container ub(approx->getNumElem(),5);
	C_Matrix_Container lb(approx->getNumElem(),5);
	// kopiowanie danych do matlaba
	for (int a=0,k=0;a<interp->getNumElem();a++)	// po ca³ym buforze
	{
		for (k=0;k<interp->GetObject(a)->getSizeofInterpData();k++)
		{
			x.SetPixel(a,k,interp->GetObject(a)->getInterpolated_X()[k]);
			y.SetPixel(a,k,interp->GetObject(a)->getInterpolated_Y()[k]);
			profil.SetPixel(a,k,interp->GetObject(a)->getInterpolated_data()[k]);
		}
		for(k=0;k<5;k++){
			p.SetPixel(a,k,approx->GetObject(a)->getApproxParams_p()[k]);
			ub.SetPixel(a,k,approx->GetObject(a)->getApproxParams_ub()[k]);
			lb.SetPixel(a,k,approx->GetObject(a)->getApproxParams_lb()[k]);
		}
	}
	dump.AddEntry(&x,"x");// wsp x dla których aproxymacja, w rzêdach kolejne linie (z bufora)
	dump.AddEntry(&y,"y");// wsp y dla których aproxymacja
	dump.AddEntry(&profil,"profil"); // aproxymowany profil
	dump.AddEntry(&p,"p"); // parametry krzywej aproxymuj¹cej
	dump.AddEntry(&ub,"ub"); // parametry krzywej aproxymuj¹cej
	dump.AddEntry(&lb,"lb"); // parametry krzywej aproxymuj¹cej

	dump.AddEntry(&p_med,"p_med"); // parmaetry oblicone z ca³ego bufora (mediana)
	dump.AddEntry(&ub_med,"ub_med");
	dump.AddEntry(&lb_med,"lb_med");
	dump.AddEntry(&w,"w"); // srednia wartoœæ z ca³ego bufora dla profilu normowana do 01
}

/// test evalWeldPos - ta funkcja jest sprawdzana tylko dla poorawnych aproxymacji - te nie poprawne sa odrzucane
TEST_P(C_LinearWeld_FillBuffor, DISABLED_evalWeldPos) {
	_RPT0(_CRT_WARN,"------ Entering Test evalWeldPos ------\n");
	_RPT1(_CRT_WARN,"PAR: %d\n",x_start);
	char buff[6];_itoa_s(x_start,buff,6,10);std::string liczba(buff);
	std::string nazwa("C_LinearWeld_evalWeldPos");
	nazwa+=liczba;
	nazwa+=".out";
	std::cout<< "\n\t Wyniki do testu w matlabie " << nazwa <<"\n\n";
	C_DumpAll dump(nazwa.c_str());

	C_CircBuff<C_LineWeldApprox> *approx;
	C_CircBuff<double> *rec;
	C_WeldPos pos;	// pozycja spawu
	C_Matrix_Container p(1,5);
	C_Matrix_Container points(3,2); // punkty wyjsciowe
	const double *_p;
	double *pre_data;

	C_Point cp_x_start(x_start,0);
	obj->SetProcedureParameters(1,cp_x_start); // start
	// start
	access_fillBuffor(obj);
	// pobieral adresy struktur z danymi
	access_interpaprox(obj,&approx,&interp,&rec);

	pre_data = new double[interp->GetFirstInitialized()->getSizeofInterpData()];
	const double *y = interp->GetFirstInitialized()->getInterpolated_Y();
	approx->GetFirstInitialized()->evalApproxFcnVec(y,pre_data,interp->GetFirstInitialized()->getSizeofInterpData());

	access_evalWeldPos(obj,approx->GetFirstInitialized(),interp->GetFirstInitialized(),pre_data,pos);
	_p = approx->GetFirstInitialized()->getApproxParams_p();	// parametry approxymacji
	p.CopyfromTab(_p,5);	// kopiuje do matrixa
	points.SetPixel(0,0,pos.D.getX());points.SetPixel(0,1,pos.D.getY());
	points.SetPixel(1,0,pos.S.getX());points.SetPixel(1,1,pos.S.getY());
	points.SetPixel(2,0,pos.G.getX());points.SetPixel(2,1,pos.G.getY());
	dump.AddEntry(&p,"p");
	dump.AddEntry(&points,"points");
}
INSTANTIATE_TEST_CASE_P(
	FillBuffor_Test,
	C_LinearWeld_FillBuffor,
	::testing::Values(10,15,100,1612,3253,4294,6000)); // te same wartosci sa w matlabie