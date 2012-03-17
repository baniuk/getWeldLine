#include "C_LinearWeld.h"



C_LinearWeld::C_LinearWeld( const C_Matrix_Container *_rtg ) : C_WeldlineDetect(_rtg)
{
	
}

C_LinearWeld::~C_LinearWeld()
{
	SAFE_DELETE(_w);
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::~C_LinearWeld\n");
}
/** 
 * Ustawia �rodowisko, tworzy potrzebne tablice. 
 * Funkcja musi by� wywo�ana po konstruktorze aby ustawi� potrzebne parametry.
 * \param[in] _k ilo�� linii branych do wygeneroania nast�pnej - wielkos� bufora
 * \param[in] _StartPoint punkt startowy - kolumna obrazka w tym przypadku (sk��dowa x)
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k, C_Point _StartPoint)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::SetProcedureParameters");
	k = _k;
	P0.setPoint(_StartPoint.getX(),0); // d� obrazka
	P1.setPoint(_StartPoint.getX(),rtg->_rows-1);	// g�ra obrazka
	// tworzenie bufor�w na przechowanie danych
	if(k>0)
	{
		_RPT1(_CRT_WARN,"\t\tk = %d",k);
		approx_results.BuffInit(k);
		interp_lines.BuffInit(k);
	}
	_RPT5(_CRT_WARN,"\t\tk=%.3lf, P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",k,P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::SetProcedureParameters\n");
}
/** 
 * Uruchamia w�a�ciwy proces wykrywnia spawu.
 * \return OK je�li uda�o si� zako�czy�, BLAD je�li wystapi� b��d
 */
bool C_LinearWeld::Start()
{
	// bierz�ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do �atwiejszego adresowania
	C_LineWeldApprox *app;
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret;
	// wype�nianie bufora w zale�no�ci od punktu startowego podanego w SetProcedureParameters()
	ret = fillBuffor();
	if(BLAD==ret)
		return ret;
	// w P0P1 jest ju� nast�pny punkt bo zosta� wygenerowany w fillBuffor()
	while(OK==ret)
	{
		// generowanie nast paramer�w na podstawie bufora wype�niane s� zmienne _p, _ub, _lb, _w
		evalNextParams();
		// generowanie linii itp
		obj = interp_lines.AddObject();	// dodaje now� lini�
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na g�rze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacj� - bior� tyle punkt�w ile jest rz�d�w w obrazie + punkty ko�cowe. Wyniki s� zapami�tywane w klasie i dost�pne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results.AddObject();
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a je�li nie to x
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofApproxData());
		else
			_RPTF0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa"); // to nie powinno byc nigdy wykonene w tej wersji
		// aproxymacja - parametry obliczone przez evalNextParams()
		app->setApproxParmas(_p,_w,_ub,_lb,NULL);
		// aproxymacja
		app->getLineApprox(100);
#ifdef _DEBUG
		const double *pdeb;pdeb = app->getApproxParams_p();
		_RPT5(_CRT_WARN,"\t\tRES: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam powodzenie interpolacji danej linii
		if(BLAD==czyAccept(app,obj))
		{
			lineOK.push_back(BLAD);	// te wektory zawieraj� ju� info o liniach bufora
			weldPos.push_back(currentPos);
		} else {
			lineOK.push_back(OK);	// linia ok
			evalWeldPos(app,obj,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generowanie nast punktu start (zwrocone z  blad oznacza koniec spawu i wtedy tu przerywamy)
		ret = evalNextStartPoint();
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::Start\n");
	return OK;
}
/** 
 * Procedura wype�nia bufory ko�owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko�owym. Dodatkowo tak�e zapisuje dane w weldPos oraz lineOK. Dzi�ki temu te zienne zawsze przechowuj� informacje o wszystkich obrobionych liniach, nawet tych niepoprawnych.
 * \return OK je�li uda�o si�wype�ni�, BLAD je�li wystapi� b��d
 */
bool C_LinearWeld::fillBuffor()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::fillBuffor");
	// bierz�ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	// interpolacja k kolejnych linii zacynaj�c od startowej - poruszam si� po kolumnach
	C_LineInterp *obj;	// obiekt tymczasowy do �atwiejszego adresowania
	C_LineWeldApprox *app;
	bool ret_evalnextparam = OK; // je�li false to koniec obrazka
	while(!interp_lines.Czy_pelny() && interp_lines.getNumElem()>-1 && ret_evalnextparam==OK)	// dopuki bufor nie jest pe�ny i jednocze�nie jest zainicjowany
	{
		obj = interp_lines.AddObject();	// dodaje now� lini�
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na g�rze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacj� - bior� tyle punkt�w ile jest rz�d�w w obrazie + punkty ko�cowe. Wyniki s� zapami�tywane w klasie i dost�pne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results.AddObject();
		// konstruktor manualne
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a je�li nie to x
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofApproxData());
		else{
			_RPTF0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa");
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofApproxData()); // to nie 
			// powinno byc nigdy wykonene w tej wersji
		}
		// aproxymacja - parametry domy�lne
		app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
		// aproxymacja
		app->getLineApprox(100);
		// sprawdzam powodzenie interpolacji
#ifdef _DEBUG
		const double *pdeb;pdeb = app->getApproxParams_p();
		_RPT5(_CRT_WARN,"\t\tRES: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam poprawno�� danych
		if(BLAD==czyAccept(app,obj))
		{
			approx_results.DelObject();
			interp_lines.DelObject();
			lineOK.push_back(BLAD);
			weldPos.push_back(currentPos);	// warto�ci domy�lne
		} else {
			lineOK.push_back(OK);	// linia ok
			evalWeldPos(app,obj,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generuje nast�pne punkty
		ret_evalnextparam = evalNextStartPoint();
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::fillBuffor\n");
	return ret_evalnextparam;
}
/** 
 * Generuje zestaw parametr�w startowych do obliczenia kolejenj aproxymacji. Obraz jest
 * interpolowany na linii P0P1 a nast�pnie ten profil b�dzie podlega� aproxymacji. Ka�de wywo�anie funkcji 
 * powoduje wygenerowanie nast�pnego zestawu parametr�w.
 * \return BLAD je�li nie mo�na wygenerowac kolejnych parametr�w (na przyk��d koniec obrazka)
 * \warning Funkcja modyfikuje pola P0 i P1
 * 
 */
bool C_LinearWeld::evalNextStartPoint()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextStartPoint");
	// generowanie nowych wsp�rz�dnych dla linii inteprolacyjnej - do paproxymacji profilu
	if(P0.getX()==rtg->_cols-1)	// je�li jeste�my na ostatniej kolumnie to nie mo�na wygenerowac kolejnej
		return BLAD;
	C_Point delta(1,0);	// krok 1 po x
	_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	P0+=delta;
	P1+=delta;
	_RPT4(_CRT_WARN,"\t\tOutput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalNextStartPoint\n");
	return OK;
}
/** 
 * Generuje parametry aproksymacji: wsp�czynniki startowe p, ub, lb oraz wagi. Na podstawie struktur buforowych approx_results i interp_lines.Obliczone parametry zwraca na zewn�trz. W kolejnej moga one zosta� przekazane do klasy C_LineWeldApprox, kt�ra robi sobie ich lokaln� kopi�.Funkcja inicalizuje tablc� wag, ale jednorazowo. Za�o�enie jest ze do obiektu nie mo�na pod��czy� innego obrazka wi�c ilo�� element�w interpolowanych nie zmieni si�.
 * \return OK jesli w porzadku
 */
bool C_LinearWeld::evalNextParams()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextParams");
	if(approx_results.getNumElem()<=0)
		_RPT0(_CRT_ASSERT,"C_LinearWeld::evalNextParams->pusty bufor");
	int num_el = approx_results.getNumElem();
	int num_points = interp_lines.GetObject(0)->getSizeofApproxData();
	C_Matrix_Container a(1,num_el);
	C_Matrix_Container b(1,num_el);
	C_Matrix_Container c(1,num_el);
	C_Matrix_Container d(1,num_el);
	C_Matrix_Container e(1,num_el);
	C_Image_Container waga;	// pomocniczy do skalowania wag
	const double *p_par;	// parametry z bufora lub wektor wag
	// meidana z p - poszczeg�lne elementy tego wektora z ca�ego bufora s� zbierane do jednego wektora
	_RPT1(_CRT_WARN,"\t\tSizebuff: %d",num_el);
	_RPT1(_CRT_WARN,"\t\tNumInterpElem: %d",num_points);
	// inicjalizacja wektora wag (globalny) inicjalizowany tylko raz
	if(NULL==_w)
		_w = new double[num_points];
	for (int la=0;la<num_el;la++)
	{
		p_par = approx_results.GetObject(la)->getApproxParams_p();
		a.SetPixel(0,la,p_par[A]);
		b.SetPixel(0,la,p_par[B]);
		c.SetPixel(0,la,p_par[C]);
		d.SetPixel(0,la,p_par[D]);
		e.SetPixel(0,la,p_par[E]);
	}
	_p[A] = a.Median();
	_p[B] = b.Median();
	_p[C] = c.Median();
	_p[D] = d.Median();
	_p[E] = e.Median();

	for (int la=0;la<num_el;la++)
	{
		p_par = approx_results.GetObject(la)->getApproxParams_ub();
		a.SetPixel(0,la,p_par[A]);
		b.SetPixel(0,la,p_par[B]);
		c.SetPixel(0,la,p_par[C]);
		d.SetPixel(0,la,p_par[D]);
		e.SetPixel(0,la,p_par[E]);
	}
	_ub[A] = a.Median();
	_ub[B] = b.Median();
	_ub[C] = c.Median();
	_ub[D] = d.Median();
	_ub[E] = e.Median();

	for (int la=0;la<num_el;la++)
	{
		p_par = approx_results.GetObject(la)->getApproxParams_lb();
		a.SetPixel(0,la,p_par[A]);
		b.SetPixel(0,la,p_par[B]);
		c.SetPixel(0,la,p_par[C]);
		d.SetPixel(0,la,p_par[D]);
		e.SetPixel(0,la,p_par[E]);
	}
	_lb[A] = a.Median();
	_lb[B] = b.Median();
	_lb[C] = c.Median();
	_lb[D] = d.Median();
	_lb[E] = e.Median();
	// wagi
	// wype�nienie zerami
	for(int ld=0;ld<num_points;++ld)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld] = 0.0;
	for(int la=0;la<num_el;la++)	// po liniach w buforze
	{
		p_par = interp_lines.GetObject(la)->getInterpolated_data(); // biore obiekt z la linii w buforze
		for(int ld=0;ld<num_points;ld++)
			_w[ld]+=p_par[ld];	// dodaje do bufora wag
	}
	for(int ld=0;ld<num_points;ld++)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld]/=num_el;

	// skalowanie wag do zakresu 01
	/// \todo Tu mo�na przyspieszy� znacznie wykonuj�c te operacje w miejscu - funkcja albo metoda statyczna lub stl
	/// \verbatim
	waga.AllocateData(1,num_points);
	waga.CopyfromTab(_w,num_points);
	waga.Normalize(0,1);
	/// \endverbatim
	// kopiowanie spowrotem
	memcpy_s(_w,num_points*sizeof(double),waga.data,waga.GetNumofElements()*sizeof(double));
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalNextParams\n");
	return OK;
}
/** 
 * Funkcja spawdza czy uzyskana interpolacja mo�e by� zaakceptowana.
 * \param[in] _approx Wska�nik do obiektu z aproksymacj� linii
 * \param[in] _interp Wska�nik do obietu z interpolacj� danych
 * \return OK je�li dane s� akceptowalne, BLAD je�li nie
 */
bool C_LinearWeld::czyAccept( const C_LineWeldApprox *_approx, const C_LineInterp *_interp )
{
	_RPT1(_CRT_WARN," err=%.3e",_approx->getInfo(err));
	if(7==_approx->getInfo(stopreason)){
		_RPT0(_CRT_WARN,"\t\t Deleted: reason");
		return BLAD;
	}else // zbieram przydatne parametry
		if(_approx->getInfo(err)>MAX_ERROR_LEVEL){
			_RPT0(_CRT_WARN,"\t\t Deleted: error");
			return BLAD;
		}
	return OK;
}
/** 
 * Oblicza pozycj� spawu.
 * \param[in] _approx Wska�nik do obiektu z aproksymacj� linii
 * \param[in] _interp Wska�nik do obietu z interpolacj� danych
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale�y sprwdza� czy prosta intepolacyjna by�a pionowa i odpowiednio bra� dane x albo y do evaluacji warto�ci w klasie C_LineWeldApprox. Funkcja zak��da ze dane _approx oraz _interp s� sp�jne, tzn dotycz� tej samej linii obrazu
 */ 
void C_LinearWeld::evalWeldPos( const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )
{
	double *evaldata = new double[_interp->getSizeofApproxData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	const double *x,*y;	// wskazniki na wektory x i y dla kt�rych zosta�a wykonan interpolacja i aproxymacja (wsp�rz�dne obrazu)
	double wartosc;
	x = _interp->getInterpolated_X();
	y = _interp->getInterpolated_Y();
	for (unsigned int a=0;a<_interp->getSizeofApproxData();a++)
	{
		wartosc = y[a];
		evaldata[a] = _approx->evalApproxFcn(wartosc);
	}
	///\todo znale�� min max itp

	SAFE_DELETE(evaldata);
}
