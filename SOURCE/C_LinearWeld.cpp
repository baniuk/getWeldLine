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
		recalculated_approx_data.BuffInit(k);
	}
	_RPT5(_CRT_WARN,"\t\tk=%.3lf, P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",k,P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::SetProcedureParameters\n");
}
/** 
 * Uruchamia w³aœciwy proces wykrywnia spawu.
 * \param[in] step Krok z jakimjest wykrywaie spawu. Bufor zawsze jest ype³niany z krokiem 1
 * \return OK jeœli uda³o siê zakoñczyæ, BLAD jeœli wystapi³ b³¹d. BLAD jest zwracany jesli nie uda sie stworzyc bufora lub krok by³o równy 0
 */
bool C_LinearWeld::Start(unsigned int step)
{
	// bierz¹ca pozycja spawu w przestrzeni obrazka
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app;
	const double *y;	// dane x do aproxymacji
	double *pre;	// wskaŸnik na dan¹ w buforze precalculated_approx_data
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret=OK;
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
			lineOK.push_back(BLAD);	// te wektory zawieraj¹ ju¿ info o liniach bufora
			currentPos.Clear();	// kasuje wszystko bo ten wpis nie wa¿ny
			weldPos.push_back(currentPos);
		} else {
			lineOK.push_back(OK);	// linia ok
			pre = recalculated_approx_data.AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype³niam currentPos w³aœciwymi danymo
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
 * Procedura wype³nia bufory ko³owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko³owym. Dodatkowo tak¿e zapisuje dane w weldPos oraz lineOK. Dziêki temu te zienne zawsze przechowuj¹ informacje o wszystkich obrobionych liniach, nawet tych niepoprawnych.\n
 * W tej funkcji wype³niany jest takze bufor recalculated_approx_data.
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
	const double *y;	// dane x do aproxymacji
	double *pre;	// wskaŸnik na dan¹ w buforze precalculated_approx_data
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
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
		else{
			_RPTF0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa");
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofInterpData()); // to nie 
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
			currentPos.Clear();
			weldPos.push_back(currentPos);	// wartoœci domyœlne
		} else {
			lineOK.push_back(OK);	// linia ok
			pre = recalculated_approx_data.AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype³niam currentPos w³aœciwymi danymo
			weldPos.push_back(currentPos);
		}
		// generuje nastêpne punkty, w buforze zawsze krok 1
		ret_evalnextparam = evalNextStartPoint(1);
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::fillBuffor\n");
	return ret_evalnextparam;
}
/** 
 * Generuje zestaw parametrów startowych do obliczenia kolejenj aproxymacji. Obraz jest
 * interpolowany na linii P0P1 a nastêpnie ten profil bêdzie podlega³ aproxymacji. Ka¿de wywo³anie funkcji 
 * powoduje wygenerowanie nastêpnego zestawu parametrów.
 * \param[in] step krok z jakim przesuwamy siê. Za³ozenie jest ¿e krok dotyczy tylko osi x
 * \return BLAD jeœli nie mo¿na wygenerowac kolejnych parametrów (na przyk³¹d koniec obrazka)
 * \warning Funkcja modyfikuje pola P0 i P1
 * 
 */
bool C_LinearWeld::evalNextStartPoint(unsigned int step)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextStartPoint");
	// generowanie nowych wspó³rzêdnych dla linii inteprolacyjnej - do paproxymacji profilu
//	_ASSERT(step!=0);
	if(P0.getX()==rtg->_cols-1 || 0==step)	// jeœli jesteœmy na ostatniej kolumnie to nie mo¿na wygenerowac kolejnej
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
 * Generuje parametry aproksymacji: wspó³czynniki startowe p, ub, lb oraz wagi. Na podstawie struktur buforowych approx_results i interp_lines.Obliczone parametry zwraca na zewn¹trz. W kolejnej moga one zostaæ przekazane do klasy C_LineWeldApprox, która robi sobie ich lokaln¹ kopiê.Funkcja inicalizuje tablcê wag, ale jednorazowo. Za³o¿enie jest ze do obiektu nie mo¿na pod³¹czyæ innego obrazka wiêc iloœæ elementów interpolowanych nie zmieni siê.
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
	// meidana z p - poszczególne elementy tego wektora z ca³ego bufora s¹ zbierane do jednego wektora
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
	// wype³nienie zerami

	for(int ld=0;ld<num_points;++ld)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld] = 0.0;
	for(int la=0;la<num_el;la++)	// po liniach w buforze
	{
		p_par = recalculated_approx_data.GetObject(la); // biore dane z la linii w buforze - to s a wartoœci z aproxymacji wype³nione w fillbufor i start na bierz¹co z innymi buforami razem
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
 * Oblicza pozycjê spawu. Wersja u¿ywaj¹ca przeliczonych wartoœci funkcji aproxymacyjnej
 * \param[in] _approx WskaŸnik do obiektu z aproksymacj¹ linii
 * \param[in] _interp WskaŸnik do obietu z interpolacj¹ danych
 * \param[in] _pre przeliczone dane z aproxymacji - celem przyspieszenia
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale¿y sprwdzaæ czy prosta intepolacyjna by³a pionowa i odpowiednio braæ dane x albo y do evaluacji wartoœci w klasie C_LineWeldApprox. Funkcja zak³¹da ze dane _approx oraz _interp s¹ spójne, tzn dotycz¹ tej samej linii obrazu
 */ 
void C_LinearWeld::evalWeldPos( const C_LineWeldApprox *_approx, const C_LineInterp *_interp,const double *_pre, C_WeldPos &_weldPos )
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalWeldPos");
//	double *evaldata = new double[_interp->getSizeofInterpData()]; // tablica na dane obliczone dla krzywej aproxymacyjnej
	unsigned int *indexy = new unsigned int[_interp->getSizeofInterpData()]; // tablica na indexy
	unsigned int l,licznik;
	const double *p;	// parametry aproxymacji
	double pos;
	const double *x,*y;	// wskazniki na wektory x i y dla których zosta³a wykonan interpolacja i aproxymacja (wspó³rzêdne obrazu)
	double max_el,max_lin;	// wartosc maksymalna funkcji aproxymuj¹cej


 	x = _interp->getInterpolated_X();
 	y = _interp->getInterpolated_Y();
// 	_approx->evalApproxFcnVec(y,evaldata,_interp->getSizeofInterpData());
	for(l=0,max_el = _pre[0];l<_interp->getSizeofInterpData();++l)
		if(_pre[l]>max_el)
			max_el = _pre[l];	// w max_el wartoœæ maxymalna
	// wartoœæ maxymalna nie jest od zera ale ma jakiœ liniowy trend dodany - trzeba to uwzglêdniæ. Dlatego liczona jest wartoœæ funkcji liniowej skojarzonej z gaussem i dopiero róznica jest brana jako wysokoœæ piku. Wszystko liczone jest wzglêdem punktu maximum, wiêc dla du¿ego tendu mo¿e byæ b³êdne
	_RPT1(_CRT_WARN,"\t\tmax_el: %.1lf",max_el);
	// znajdowanie indeksów z wartoœci¹ max
	for (l=licznik=0;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]==max_el)
			indexy[licznik++] = l;	// w indexy pozycje elementów maksymalnych (jeœli wiêcej ni¿ 1 to liczymy œredni¹)
	pos = 0.0;
	for(l=0;l<licznik;++l)	// po wszyskich pozycjach el. maksymalnych
		pos+=(double)indexy[l];
	pos/=licznik;	// w pos pozycja œrodka, uœredniona jeœli by³o wiêcej maximów
	pos = floor(pos+0.5);	// zaokraglona do ca³kowitej
	_RPT1(_CRT_WARN,"\t\tmax_el_pos: %d",(int)pos);
	_weldPos.S.setPoint(x[(int)pos],y[(int)pos]); // ustawiam pozycjê œrodka na wyjœciu
	
	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // wartoœæ trendu dla pozycji max 

	// górna granica od œrodka do koñca
	licznik = 0;
	for(l=(int)pos;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysokoœci piku + przesuniêcie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	/// \warning Mo¿e siê zda¿yæ ze nie bedzie w ifie i wtedy wartoœæ indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak siê zda¿a gdy aproxymowane s¹ z³e dane i funkcja aproxymuj¹ca jest zbyt szeoka, tzn obeajmuje ca³y obraz i nie daje rady zmaleæ do ¿¹danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê œrodka na wyjœciu

	// dolna granica do œrodka
	licznik = 0;
	for(l=0;l<(unsigned int)pos;++l)
		if(_pre[l]>max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê œrodka na wyjœciu

	_RPT2(_CRT_WARN,"\t\tG: [%.1lf,%.1lf]",_weldPos.G.getX(),_weldPos.G.getY());
	_RPT2(_CRT_WARN,"\t\tS: [%.1lf,%.1lf]",_weldPos.S.getX(),_weldPos.S.getY());
	_RPT2(_CRT_WARN,"\t\tD: [%.1lf,%.1lf]",_weldPos.D.getX(),_weldPos.D.getY());

//	SAFE_DELETE(evaldata);
	SAFE_DELETE(indexy);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalWeldPos\n");
}

/** 
 * Oblicza pozycjê spawu. 
 * \param[in] _approx WskaŸnik do obiektu z aproksymacj¹ linii
 * \param[in] _interp WskaŸnik do obietu z interpolacj¹ danych
 * \param[in] _pre przeliczone dane z aproxymacji - celem przyspieszenia
 * \param[out] _weldPos Dane o pozycji spawu
 * \warning Dla inne,go przypdku aproxymacji nale¿y sprwdzaæ czy prosta intepolacyjna by³a pionowa i odpowiednio braæ dane x albo y do evaluacji wartoœci w klasie C_LineWeldApprox. Funkcja zak³¹da ze dane _approx oraz _interp s¹ spójne, tzn dotycz¹ tej samej linii obrazu
 * \obsolete Wersja nie u¿ywaj¹ca przeliczonych wartoœci funkcji aproxymacyjnej - wolniejsza
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
	const double *x,*y;	// wskazniki na wektory x i y dla których zosta³a wykonan interpolacja i aproxymacja (wspó³rzêdne obrazu)
	double max_el,max_lin;	// wartosc maksymalna funkcji aproxymuj¹cej


 	x = _interp->getInterpolated_X();
 	y = _interp->getInterpolated_Y();
 	_approx->evalApproxFcnVec(y,evaldata,_interp->getSizeofInterpData());
	for(l=0,max_el = evaldata[0];l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]>max_el)
			max_el = evaldata[l];	// w max_el wartoœæ maxymalna
	// wartoœæ maxymalna nie jest od zera ale ma jakiœ liniowy trend dodany - trzeba to uwzglêdniæ. Dlatego liczona jest wartoœæ funkcji liniowej skojarzonej z gaussem i dopiero róznica jest brana jako wysokoœæ piku. Wszystko liczone jest wzglêdem punktu maximum, wiêc dla du¿ego tendu mo¿e byæ b³êdne
	_RPT1(_CRT_WARN,"\t\tmax_el: %.1lf",max_el);
	// znajdowanie indeksów z wartoœci¹ max
	for (l=licznik=0;l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]==max_el)
			indexy[licznik++] = l;	// w indexy pozycje elementów maksymalnych (jeœli wiêcej ni¿ 1 to liczymy œredni¹)
	pos = 0.0;
	for(l=0;l<licznik;++l)	// po wszyskich pozycjach el. maksymalnych
		pos+=(double)indexy[l];
	pos/=licznik;	// w pos pozycja œrodka, uœredniona jeœli by³o wiêcej maximów
	pos = floor(pos+0.5);	// zaokraglona do ca³kowitej
	_RPT1(_CRT_WARN,"\t\tmax_el_pos: %d",(int)pos);
	_weldPos.S.setPoint(x[(int)pos],y[(int)pos]); // ustawiam pozycjê œrodka na wyjœciu
	
	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // wartoœæ trendu dla pozycji max

	// górna granica od œrodka do koñca
	licznik = 0;
	for(l=(int)pos;l<_interp->getSizeofInterpData();++l)
		if(evaldata[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysokoœci piku + przesuniêcie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	/// \warning Mo¿e siê zda¿yæ ze nie bedzie w ifie i wtedy wartoœæ indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak siê zda¿a gdy aproxymowane s¹ z³e dane i funkcja aproxymuj¹ca jest zbyt szeoka, tzn obeajmuje ca³y obraz i nie daje rady zmaleæ do ¿¹danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê œrodka na wyjœciu

	// dolna granica do œrodka
	licznik = 0;
	for(l=0;l<(unsigned int)pos;++l)
		if(evaldata[l]>max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê œrodka na wyjœciu

	_RPT2(_CRT_WARN,"\t\tG: [%.1lf,%.1lf]",_weldPos.G.getX(),_weldPos.G.getY());
	_RPT2(_CRT_WARN,"\t\tS: [%.1lf,%.1lf]",_weldPos.S.getX(),_weldPos.S.getY());
	_RPT2(_CRT_WARN,"\t\tD: [%.1lf,%.1lf]",_weldPos.D.getX(),_weldPos.D.getY());

//	SAFE_DELETE(evaldata);
	SAFE_DELETE(indexy);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalWeldPos\n");
}
