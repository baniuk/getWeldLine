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
		recalculated_approx_data.BuffInit(k);
	}
	_RPT5(_CRT_WARN,"\t\tk=%.3lf, P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",k,P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::SetProcedureParameters\n");
}
/** 
 * Uruchamia w�a�ciwy proces wykrywnia spawu.
 * \param[in] step Krok z jakimjest wykrywaie spawu. Bufor zawsze jest ype�niany z krokiem 1
 * \return OK je�li uda�o si� zako�czy�, BLAD je�li wystapi� b��d. BLAD jest zwracany jesli nie uda sie stworzyc bufora lub krok by�o r�wny 0
 */
bool C_LinearWeld::Start(unsigned int step)
{
	// bierz�ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do �atwiejszego adresowania
	C_LineWeldApprox *app;
	const double *y;	// dane x do aproxymacji
	double *pre;	// wska�nik na dan� w buforze precalculated_approx_data
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret=OK;
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
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
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
			approx_results.DelObject();
			interp_lines.DelObject();
			lineOK.push_back(BLAD);	// te wektory zawieraj� ju� info o liniach bufora
			currentPos.Clear();	// kasuje wszystko bo ten wpis nie wa�ny
			weldPos.push_back(currentPos);
		} else {
			lineOK.push_back(OK);	// linia ok
			pre = recalculated_approx_data.AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generowanie nast punktu start (zwrocone z  blad oznacza koniec spawu i wtedy tu przerywamy)
		ret = evalNextStartPoint(step);
		if(!((int)P0.getX()%10))
			std::cout<<P0.getX()<<"\n";
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::Start\n");
	return OK; // zwracamy ok normalnie, BLAD jest zwracany jesli nie uda sie stworzyc bufora
}
/** 
 * Procedura wype�nia bufory ko�owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko�owym. Dodatkowo tak�e zapisuje dane w weldPos oraz lineOK. Dzi�ki temu te zienne zawsze przechowuj� informacje o wszystkich obrobionych liniach, nawet tych niepoprawnych.\n
 * W tej funkcji wype�niany jest takze bufor recalculated_approx_data.
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
	const double *y;	// dane x do aproxymacji
	double *pre;	// wska�nik na dan� w buforze precalculated_approx_data
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
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
		else{
			_RPTF0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa");
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofInterpData()); // to nie 
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
			currentPos.Clear();
			weldPos.push_back(currentPos);	// warto�ci domy�lne
		} else {
			lineOK.push_back(OK);	// linia ok
			pre = recalculated_approx_data.AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generuje nast�pne punkty, w buforze zawsze krok 1
		ret_evalnextparam = evalNextStartPoint(1);
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::fillBuffor\n");
	return ret_evalnextparam;
}
/** 
 * Generuje zestaw parametr�w startowych do obliczenia kolejenj aproxymacji. Obraz jest
 * interpolowany na linii P0P1 a nast�pnie ten profil b�dzie podlega� aproxymacji. Ka�de wywo�anie funkcji 
 * powoduje wygenerowanie nast�pnego zestawu parametr�w.
 * \param[in] step krok z jakim przesuwamy si�. Za�ozenie jest �e krok dotyczy tylko osi x
 * \return BLAD je�li nie mo�na wygenerowac kolejnych parametr�w (na przyk��d koniec obrazka)
 * \warning Funkcja modyfikuje pola P0 i P1
 * 
 */
bool C_LinearWeld::evalNextStartPoint(unsigned int step)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextStartPoint");
	// generowanie nowych wsp�rz�dnych dla linii inteprolacyjnej - do paproxymacji profilu
//	_ASSERT(step!=0);
	if(P0.getX()==rtg->_cols-1 || 0==step)	// je�li jeste�my na ostatniej kolumnie to nie mo�na wygenerowac kolejnej
		return BLAD;
	C_Point delta(step,0);	// krok 1 po x
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
#ifdef _DEBUG
	if(approx_results.getNumElem()<=0)
#endif
		_RPT0(_CRT_ASSERT,"C_LinearWeld::evalNextParams->pusty bufor");
	int num_el = approx_results.getNumElem();
	int num_points = interp_lines.GetObject(0)->getSizeofInterpData();
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
		a.data[la] = p_par[A];
		b.data[la] = p_par[B];
		c.data[la] = p_par[C];
		d.data[la] = p_par[D];
		e.data[la] = p_par[E];
// 		a.SetPixel(0,la,p_par[A]);
// 		b.SetPixel(0,la,p_par[B]);
// 		c.SetPixel(0,la,p_par[C]);
// 		d.SetPixel(0,la,p_par[D]);
// 		e.SetPixel(0,la,p_par[E]);
	}
	_p[A] = a.quick_select();
	_p[B] = b.quick_select();
	_p[C] = c.quick_select();
	_p[D] = d.quick_select();
	_p[E] = e.quick_select();

	for (int la=0;la<num_el;la++)
	{
		p_par = approx_results.GetObject(la)->getApproxParams_ub();
		a.data[la] = p_par[A];
		b.data[la] = p_par[B];
		c.data[la] = p_par[C];
		d.data[la] = p_par[D];
		e.data[la] = p_par[E];
// 		a.SetPixel(0,la,p_par[A]);
// 		b.SetPixel(0,la,p_par[B]);
// 		c.SetPixel(0,la,p_par[C]);
// 		d.SetPixel(0,la,p_par[D]);
// 		e.SetPixel(0,la,p_par[E]);
	}
	_ub[A] = a.quick_select();
	_ub[B] = b.quick_select();
	_ub[C] = c.quick_select();
	_ub[D] = d.quick_select();
	_ub[E] = e.quick_select();

	for (int la=0;la<num_el;la++)
	{
		p_par = approx_results.GetObject(la)->getApproxParams_lb();
		a.data[la] = p_par[A];
		b.data[la] = p_par[B];
		c.data[la] = p_par[C];
		d.data[la] = p_par[D];
		e.data[la] = p_par[E];
// 		a.SetPixel(0,la,p_par[A]);
// 		b.SetPixel(0,la,p_par[B]);
// 		c.SetPixel(0,la,p_par[C]);
// 		d.SetPixel(0,la,p_par[D]);
// 		e.SetPixel(0,la,p_par[E]);
	}
	_lb[A] = a.quick_select();
	_lb[B] = b.quick_select();
	_lb[C] = c.quick_select();
	_lb[D] = d.quick_select();
	_lb[E] = e.quick_select();
	// wagi
	// wype�nienie zerami

	for(int ld=0;ld<num_points;++ld)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld] = 0.0;
	for(int la=0;la<num_el;la++)	// po liniach w buforze
	{
		p_par = recalculated_approx_data.GetObject(la); // biore dane z la linii w buforze - to s a warto�ci z aproxymacji wype�nione w fillbufor i start na bierz�co z innymi buforami razem
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
 * Oblicza pozycj� spawu. Wersja u�ywaj�ca przeliczonych warto�ci funkcji aproxymacyjnej
 * \param[in] _approx Wska�nik do obiektu z aproksymacj� linii
 * \param[in] _interp Wska�nik do obietu z interpolacj� danych
 * \param[in] _pre przeliczone dane z aproxymacji - celem przyspieszenia
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale�y sprwdza� czy prosta intepolacyjna by�a pionowa i odpowiednio bra� dane x albo y do evaluacji warto�ci w klasie C_LineWeldApprox. Funkcja zak��da ze dane _approx oraz _interp s� sp�jne, tzn dotycz� tej samej linii obrazu
 */ 
void C_LinearWeld::evalWeldPos( const C_LineWeldApprox *_approx, const C_LineInterp *_interp,const double *_pre, C_WeldPos &_weldPos )
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalWeldPos");
//	double *evaldata = new double[_interp->getSizeofInterpData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	unsigned int *indexy = new unsigned int[_interp->getSizeofInterpData()]; // tablica na indexy
	unsigned int l,licznik;
	const double *p;	// parametry aproxymacji
	double pos;
	const double *x,*y;	// wskazniki na wektory x i y dla kt�rych zosta�a wykonan interpolacja i aproxymacja (wsp�rz�dne obrazu)
	double max_el,max_lin;	// wartosc maksymalna funkcji aproxymuj�cej


 	x = _interp->getInterpolated_X();
 	y = _interp->getInterpolated_Y();
// 	_approx->evalApproxFcnVec(y,evaldata,_interp->getSizeofInterpData());
	for(l=0,max_el = _pre[0];l<_interp->getSizeofInterpData();++l)
		if(_pre[l]>max_el)
			max_el = _pre[l];	// w max_el warto�� maxymalna
	// warto�� maxymalna nie jest od zera ale ma jaki� liniowy trend dodany - trzeba to uwzgl�dni�. Dlatego liczona jest warto�� funkcji liniowej skojarzonej z gaussem i dopiero r�znica jest brana jako wysoko�� piku. Wszystko liczone jest wzgl�dem punktu maximum, wi�c dla du�ego tendu mo�e by� b��dne
	_RPT1(_CRT_WARN,"\t\tmax_el: %.1lf",max_el);
	// znajdowanie indeks�w z warto�ci� max
	for (l=licznik=0;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]==max_el)
			indexy[licznik++] = l;	// w indexy pozycje element�w maksymalnych (je�li wi�cej ni� 1 to liczymy �redni�)
	pos = 0.0;
	for(l=0;l<licznik;++l)	// po wszyskich pozycjach el. maksymalnych
		pos+=(double)indexy[l];
	pos/=licznik;	// w pos pozycja �rodka, u�redniona je�li by�o wi�cej maxim�w
	pos = floor(pos+0.5);	// zaokraglona do ca�kowitej
	_RPT1(_CRT_WARN,"\t\tmax_el_pos: %d",(int)pos);
	_weldPos.S.setPoint(x[(int)pos],y[(int)pos]); // ustawiam pozycj� �rodka na wyj�ciu
	
	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // warto�� trendu dla pozycji max 

	// g�rna granica od �rodka do ko�ca
	licznik = 0;
	for(l=(int)pos;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysoko�ci piku + przesuni�cie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	/// \warning Mo�e si� zda�y� ze nie bedzie w ifie i wtedy warto�� indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak si� zda�a gdy aproxymowane s� z�e dane i funkcja aproxymuj�ca jest zbyt szeoka, tzn obeajmuje ca�y obraz i nie daje rady zmale� do ��danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� �rodka na wyj�ciu

	// dolna granica do �rodka
	licznik = 0;
	for(l=0;l<(unsigned int)pos;++l)
		if(_pre[l]>max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� �rodka na wyj�ciu

	_RPT2(_CRT_WARN,"\t\tG: [%.1lf,%.1lf]",_weldPos.G.getX(),_weldPos.G.getY());
	_RPT2(_CRT_WARN,"\t\tS: [%.1lf,%.1lf]",_weldPos.S.getX(),_weldPos.S.getY());
	_RPT2(_CRT_WARN,"\t\tD: [%.1lf,%.1lf]",_weldPos.D.getX(),_weldPos.D.getY());

//	SAFE_DELETE(evaldata);
	SAFE_DELETE(indexy);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalWeldPos\n");
}

/** 
 * Oblicza pozycj� spawu. 
 * \param[in] _approx Wska�nik do obiektu z aproksymacj� linii
 * \param[in] _interp Wska�nik do obietu z interpolacj� danych
 * \param[in] _pre przeliczone dane z aproxymacji - celem przyspieszenia
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale�y sprwdza� czy prosta intepolacyjna by�a pionowa i odpowiednio bra� dane x albo y do evaluacji warto�ci w klasie C_LineWeldApprox. Funkcja zak��da ze dane _approx oraz _interp s� sp�jne, tzn dotycz� tej samej linii obrazu
 * \obsolete Wersja nie u�ywaj�ca przeliczonych warto�ci funkcji aproxymacyjnej - wolniejsza
 */ 
void C_LinearWeld::evalWeldPos( const C_LineWeldApprox *_approx, const C_LineInterp *_interp, C_WeldPos &_weldPos )
{
	_RPT0(_CRT_ASSERT,"Obsolete");
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalWeldPos");
	double *evaldata = new double[_interp->getSizeofInterpData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	unsigned int *indexy = new unsigned int[_interp->getSizeofInterpData()]; // tablica na indexy
	unsigned int l,licznik;
	const double *p;	// parametry aproxymacji
	double pos;
	const double *x,*y;	// wskazniki na wektory x i y dla kt�rych zosta�a wykonan interpolacja i aproxymacja (wsp�rz�dne obrazu)
	double max_el,max_lin;	// wartosc maksymalna funkcji aproxymuj�cej


 	x = _interp->getInterpolated_X();
 	y = _interp->getInterpolated_Y();
 	_approx->evalApproxFcnVec(y,evaldata,_interp->getSizeofInterpData());
	for(l=0,max_el = evaldata[0];l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]>max_el)
			max_el = evaldata[l];	// w max_el warto�� maxymalna
	// warto�� maxymalna nie jest od zera ale ma jaki� liniowy trend dodany - trzeba to uwzgl�dni�. Dlatego liczona jest warto�� funkcji liniowej skojarzonej z gaussem i dopiero r�znica jest brana jako wysoko�� piku. Wszystko liczone jest wzgl�dem punktu maximum, wi�c dla du�ego tendu mo�e by� b��dne
	_RPT1(_CRT_WARN,"\t\tmax_el: %.1lf",max_el);
	// znajdowanie indeks�w z warto�ci� max
	for (l=licznik=0;l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]==max_el)
			indexy[licznik++] = l;	// w indexy pozycje element�w maksymalnych (je�li wi�cej ni� 1 to liczymy �redni�)
	pos = 0.0;
	for(l=0;l<licznik;++l)	// po wszyskich pozycjach el. maksymalnych
		pos+=(double)indexy[l];
	pos/=licznik;	// w pos pozycja �rodka, u�redniona je�li by�o wi�cej maxim�w
	pos = floor(pos+0.5);	// zaokraglona do ca�kowitej
	_RPT1(_CRT_WARN,"\t\tmax_el_pos: %d",(int)pos);
	_weldPos.S.setPoint(x[(int)pos],y[(int)pos]); // ustawiam pozycj� �rodka na wyj�ciu
	
	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // warto�� trendu dla pozycji max

	// g�rna granica od �rodka do ko�ca
	licznik = 0;
	for(l=(int)pos;l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysoko�ci piku + przesuni�cie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	/// \warning Mo�e si� zda�y� ze nie bedzie w ifie i wtedy warto�� indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak si� zda�a gdy aproxymowane s� z�e dane i funkcja aproxymuj�ca jest zbyt szeoka, tzn obeajmuje ca�y obraz i nie daje rady zmale� do ��danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� �rodka na wyj�ciu

	// dolna granica do �rodka
	licznik = 0;
	for(l=0;l<(unsigned int)pos;++l)
		if(evaldata[l]>max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� �rodka na wyj�ciu

	_RPT2(_CRT_WARN,"\t\tG: [%.1lf,%.1lf]",_weldPos.G.getX(),_weldPos.G.getY());
	_RPT2(_CRT_WARN,"\t\tS: [%.1lf,%.1lf]",_weldPos.S.getX(),_weldPos.S.getY());
	_RPT2(_CRT_WARN,"\t\tD: [%.1lf,%.1lf]",_weldPos.D.getX(),_weldPos.D.getY());

//	SAFE_DELETE(evaldata);
	SAFE_DELETE(indexy);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalWeldPos\n");
}
