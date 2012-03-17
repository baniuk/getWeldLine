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
 * Ustawia œrodowisko, tworzy potrzebne tablice. 
 * Funkcja musi byæ wywo³ana po konstruktorze aby ustawiæ potrzebne parametry.
 * \param[in] _k iloœæ linii branych do wygeneroania nastêpnej - wielkosæ bufora
 * \param[in] _StartPoint punkt startowy - kolumna obrazka w tym przypadku (sk³¹dowa x)
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k, C_Point _StartPoint)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::SetProcedureParameters");
	k = _k;
	P0.setPoint(_StartPoint.getX(),0); // dó³ obrazka
	P1.setPoint(_StartPoint.getX(),rtg->_rows-1);	// góra obrazka
	// tworzenie buforów na przechowanie danych
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
 * Uruchamia w³aœciwy proces wykrywnia spawu.
 * \return OK jeœli uda³o siê zakoñczyæ, BLAD jeœli wystapi³ b³¹d
 */
bool C_LinearWeld::Start()
{
	// bierz¹ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app;
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret;
	// wype³nianie bufora w zale¿noœci od punktu startowego podanego w SetProcedureParameters()
	ret = fillBuffor();
	if(BLAD==ret)
		return ret;
	// w P0P1 jest ju¿ nastêpny punkt bo zosta³ wygenerowany w fillBuffor()
	while(OK==ret)
	{
		// generowanie nast paramerów na podstawie bufora wype³niane s¹ zmienne _p, _ub, _lb, _w
		evalNextParams();
		// generowanie linii itp
		obj = interp_lines.AddObject();	// dodaje now¹ liniê
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na górze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe. Wyniki s¹ zapamiêtywane w klasie i dostêpne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results.AddObject();
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a jeœli nie to x
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
			lineOK.push_back(BLAD);	// te wektory zawieraj¹ ju¿ info o liniach bufora
			weldPos.push_back(currentPos);
		} else {
			lineOK.push_back(OK);	// linia ok
			evalWeldPos(app,obj,currentPos);	// wype³niam currentPos w³aœciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generowanie nast punktu start (zwrocone z  blad oznacza koniec spawu i wtedy tu przerywamy)
		ret = evalNextStartPoint();
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::Start\n");
	return OK;
}
/** 
 * Procedura wype³nia bufory ko³owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko³owym. Dodatkowo tak¿e zapisuje dane w weldPos oraz lineOK. Dziêki temu te zienne zawsze przechowuj¹ informacje o wszystkich obrobionych liniach, nawet tych niepoprawnych.
 * \return OK jeœli uda³o siêwype³niæ, BLAD jeœli wystapi³ b³¹d
 */
bool C_LinearWeld::fillBuffor()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::fillBuffor");
	// bierz¹ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	// interpolacja k kolejnych linii zacynaj¹c od startowej - poruszam siê po kolumnach
	C_LineInterp *obj;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app;
	bool ret_evalnextparam = OK; // jeœli false to koniec obrazka
	while(!interp_lines.Czy_pelny() && interp_lines.getNumElem()>-1 && ret_evalnextparam==OK)	// dopuki bufor nie jest pe³ny i jednoczeœnie jest zainicjowany
	{
		obj = interp_lines.AddObject();	// dodaje now¹ liniê
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na górze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe. Wyniki s¹ zapamiêtywane w klasie i dostêpne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results.AddObject();
		// konstruktor manualne
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a jeœli nie to x
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofApproxData());
		else{
			_RPTF0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa");
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofApproxData()); // to nie 
			// powinno byc nigdy wykonene w tej wersji
		}
		// aproxymacja - parametry domyœlne
		app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
		// aproxymacja
		app->getLineApprox(100);
		// sprawdzam powodzenie interpolacji
#ifdef _DEBUG
		const double *pdeb;pdeb = app->getApproxParams_p();
		_RPT5(_CRT_WARN,"\t\tRES: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam poprawnoœæ danych
		if(BLAD==czyAccept(app,obj))
		{
			approx_results.DelObject();
			interp_lines.DelObject();
			lineOK.push_back(BLAD);
			weldPos.push_back(currentPos);	// wartoœci domyœlne
		} else {
			lineOK.push_back(OK);	// linia ok
			evalWeldPos(app,obj,currentPos);	// wype³niam currentPos w³aœciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generuje nastêpne punkty
		ret_evalnextparam = evalNextStartPoint();
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::fillBuffor\n");
	return ret_evalnextparam;
}
/** 
 * Generuje zestaw parametrów startowych do obliczenia kolejenj aproxymacji. Obraz jest
 * interpolowany na linii P0P1 a nastêpnie ten profil bêdzie podlega³ aproxymacji. Ka¿de wywo³anie funkcji 
 * powoduje wygenerowanie nastêpnego zestawu parametrów.
 * \return BLAD jeœli nie mo¿na wygenerowac kolejnych parametrów (na przyk³¹d koniec obrazka)
 * \warning Funkcja modyfikuje pola P0 i P1
 * 
 */
bool C_LinearWeld::evalNextStartPoint()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextStartPoint");
	// generowanie nowych wspó³rzêdnych dla linii inteprolacyjnej - do paproxymacji profilu
	if(P0.getX()==rtg->_cols-1)	// jeœli jesteœmy na ostatniej kolumnie to nie mo¿na wygenerowac kolejnej
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
 * Generuje parametry aproksymacji: wspó³czynniki startowe p, ub, lb oraz wagi. Na podstawie struktur buforowych approx_results i interp_lines.Obliczone parametry zwraca na zewn¹trz. W kolejnej moga one zostaæ przekazane do klasy C_LineWeldApprox, która robi sobie ich lokaln¹ kopiê.Funkcja inicalizuje tablcê wag, ale jednorazowo. Za³o¿enie jest ze do obiektu nie mo¿na pod³¹czyæ innego obrazka wiêc iloœæ elementów interpolowanych nie zmieni siê.
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
	// meidana z p - poszczególne elementy tego wektora z ca³ego bufora s¹ zbierane do jednego wektora
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
	// wype³nienie zerami
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
	/// \todo Tu mo¿na przyspieszyæ znacznie wykonuj¹c te operacje w miejscu - funkcja albo metoda statyczna lub stl
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
 * Funkcja spawdza czy uzyskana interpolacja mo¿e byæ zaakceptowana.
 * \param[in] _approx WskaŸnik do obiektu z aproksymacj¹ linii
 * \param[in] _interp WskaŸnik do obietu z interpolacj¹ danych
 * \return OK jeœli dane s¹ akceptowalne, BLAD jeœli nie
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
 * Oblicza pozycjê spawu.
 * \param[in] _approx WskaŸnik do obiektu z aproksymacj¹ linii
 * \param[in] _interp WskaŸnik do obietu z interpolacj¹ danych
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale¿y sprwdzaæ czy prosta intepolacyjna by³a pionowa i odpowiednio braæ dane x albo y do evaluacji wartoœci w klasie C_LineWeldApprox. Funkcja zak³¹da ze dane _approx oraz _interp s¹ spójne, tzn dotycz¹ tej samej linii obrazu
 */ 
void C_LinearWeld::evalWeldPos( const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )
{
	double *evaldata = new double[_interp->getSizeofApproxData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	const double *x,*y;	// wskazniki na wektory x i y dla których zosta³a wykonan interpolacja i aproxymacja (wspó³rzêdne obrazu)
	double wartosc;
	x = _interp->getInterpolated_X();
	y = _interp->getInterpolated_Y();
	for (unsigned int a=0;a<_interp->getSizeofApproxData();a++)
	{
		wartosc = y[a];
		evaldata[a] = _approx->evalApproxFcn(wartosc);
	}
	///\todo znaleœæ min max itp

	SAFE_DELETE(evaldata);
}
