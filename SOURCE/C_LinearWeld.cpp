/**
 * \file    C_LinearWeld.cpp
 * \brief	Implementuje dekecje spawów liniowych
 * \author  PB
 * \date    2012/03/01
 * \version 1.0
 */

#include "C_LinearWeld.h"



C_LinearWeld::C_LinearWeld( const C_Matrix_Container *_rtg ) : C_WeldlineDetect(_rtg)
{
	interp_lines = new C_CircBuff<C_LineInterp>;
	approx_results = new C_CircBuff<C_LineWeldApprox>;
	recalculated_approx_data = new C_CircBuff<double>;
	lineOK = new vector<bool>;
	weldPos = new vector<C_WeldPos>;
}

C_LinearWeld::~C_LinearWeld()
{
	SAFE_DELETE(_w);
	SAFE_DELETE(interp_lines);
	SAFE_DELETE(approx_results);
	SAFE_DELETE(recalculated_approx_data);
	SAFE_DELETE(lineOK);
	SAFE_DELETE(weldPos);
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
	_ASSERT(!(_StartPoint.getX()<1) ||
			!(_StartPoint.getX()>rtg->_cols-1)); // punkt poza obrazem
	P0.setPoint(_StartPoint.getX(),0); // dó³ obrazka
	P1.setPoint(_StartPoint.getX(),rtg->_rows-1);	// góra obrazka
	// tworzenie buforów na przechowanie danych
	if(k>0)
	{
		_RPT1(_CRT_WARN,"\t\tk = %d",k);
		approx_results->BuffInit(k);
		interp_lines->BuffInit(k);
		recalculated_approx_data->BuffInit(k);
	}
	_RPT5(_CRT_WARN,"\t\tk=%.3lf, P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",k,P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::SetProcedureParameters\n");
}
/** 
 * Uruchamia w³aœciwy proces wykrywnia spawu.
 * \param[in] step Krok z jakimjest wykrywaie spawu. Bufor zawsze jest ype³niany z krokiem 1
 * \param[in] ile Ile linii ma byæ aproxymowanych, jesli 0 to do koñca spawu
 * \return OK jeœli uda³o siê zakoñczyæ, BLAD jeœli wystapi³ b³¹d. BLAD jest zwracany jesli nie uda sie stworzyc bufora lub krok by³o równy 0
 */
bool C_LinearWeld::Start(unsigned int step,unsigned int ile)
{
	// bierz¹ca pozycja spawu w przestrzeni obrazka
	bool liniaok = OK;	// czy aproxymacja bierzacej lini zakoñczy³a siê sukcesem. Jeœli tak to zosta³a ona w³¹czona do bufora i treba go przeliczyæ. Jesli nie to nie ma takiej potrzeby
	unsigned int ile_done=0;	// ile linii zosta³o ju¿ zaproxymowanych, licz¹ siêwszystkie nawet te z³e
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app;
	const double *y;	// dane x do aproxymacji
	double *pre;	// wskaŸnik na dan¹ w buforze precalculated_approx_data
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret=OK;
	int iter;
	// wype³nianie bufora w zale¿noœci od punktu startowego podanego w SetProcedureParameters()
	ret = fillBuffor();
	if(BLAD==ret)
		return ret;
	// w P0P1 jest ju¿ nastêpny punkt bo zosta³ wygenerowany w fillBuffor()
	// jesli ile jest 0 to nie uwzglêdniamy tego warunku stopu. ile jest ustawiane na wartoœæ max to zawsze bedzie warunek spe³niony
	if(0==ile) ile = UINT_MAX;
	while(OK==ret && ile_done<ile)
	{
		// generowanie nast paramerów na podstawie bufora wype³niane s¹ zmienne _p, _ub, _lb, _w
		if(liniaok)	// przeliczamy nowe parametry jesli by³a zmiana w buforze
			evalNextParams();
		// generowanie linii itp
		obj = interp_lines->AddObject();	// dodaje now¹ liniê
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na górze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe. Wyniki s¹ zapamiêtywane w klasie i dostêpne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results->AddObject();
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a jeœli nie to x
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
		else
			_RPT0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa"); // to nie powinno byc nigdy wykonene w tej wersji
		// aproxymacja - parametry obliczone przez evalNextParams()
		app->setApproxParmas(_p,_w,_ub,_lb,NULL);
		// aproxymacja
		/// \todo dodaæ tu sprawdzanie czy zwraca poprawn¹ iloœæ iteracji
		iter = app->getLineApprox(100);
		_RPT1(_CRT_WARN,"\t\tITER: %d",iter);
#ifdef _DEBUG
		const double *pdeb;pdeb = app->getApproxParams_p();
		_RPT5(_CRT_WARN,"\t\tRES: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
		pdeb = app->getApproxParams_ub();
		_RPT5(_CRT_WARN,"\t\tUB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
		pdeb = app->getApproxParams_lb();
		_RPT5(_CRT_WARN,"\t\tLB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam powodzenie interpolacji danej linii
		if(BLAD==czyAccept(app,obj))
		{
			liniaok = BLAD;	// nie dodaliœmy nowej linii
			approx_results->DelObject();
			interp_lines->DelObject();
			lineOK->push_back(liniaok);	// te wektory zawieraj¹ ju¿ info o liniach bufora
			currentPos.Clear();	// kasuje wszystko bo ten wpis nie wa¿ny
			weldPos->push_back(currentPos);
		} else {
			liniaok = OK;	// dodlaiœmy nowa liniê
			lineOK->push_back(liniaok);	// linia ok
			pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych -dodawan etylko dla dobrych linii, wiec nie ma potrzeby kasownaia dla z³ych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype³niam currentPos w³aœciwymi danymo
			weldPos->push_back(currentPos);
		}
		// generowanie nast punktu start (zwrocone z  blad oznacza koniec spawu i wtedy tu przerywamy)
		ile_done++;
		ret = evalNextStartPoint(step);
		if(!((int)P0.getX()%10))
			std::cout<<P0.getX()<<"\n";
	}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::Start\n");
	return OK; // zwracamy ok normalnie, BLAD jest zwracany jesli nie uda sie stworzyc bufora
}
/** 
 * Oblicza jedn¹ liniê dla spawu i zwraca pozycje brzegów i œrodka. Sprawdza poprwnoœæ aproksymacji
 * oraz nie bierze pod uwagê buforów. Linia obliczana dla bierz¹cych punktów (tych zdefinoowanych w klasie).
 * Mo¿na je zmieniaæ poprzez evalNextStartPoint. Ta funkcja wymaga zainicjalizowanej klasy jak w normalnym przypadku.
 * \param[out] weldpos Struktura z po³o¿eniem spawu dla jednego punktu
 * \return OK jeœli aproxymacja dobra, BLAD jeœli nie
 */
bool C_LinearWeld::getOneApproxLine(C_WeldPos &weldpos)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::getOneApproxLine");
	C_LineInterp *obj = NULL;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app = NULL;
	double *pre;	// wskaŸnik na dan¹ w buforze precalculated_approx_data
	const double *y;	// dane x do aproxymacji

	bool ret;
	int iter;
	C_WeldPos wp;

	obj = new C_LineInterp;
	obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
	// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe. Wyniki s¹ zapamiêtywane w klasie i dostêpne poprzez getInterpolated
	obj->getPointsOnLine(P0,P1,rtg->_rows+1);

	app = new C_LineWeldApprox;
	app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
	// aproxymacja - parametry domyœlne
	app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
	// aproxymacja
	iter = app->getLineApprox(200);
	_RPT1(_CRT_WARN,"\t\tITER: %d",iter);
	double sr = app->getInfo(stopreason);
	_RPT1(_CRT_WARN,"\t\tSTOPREASON: %lf",sr);

	pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych
	y = obj->getInterpolated_Y();
	app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
	evalWeldPos(app,obj,pre,weldpos);	// wype³niam currentPos w³aœciwymi danymo

	ret = czyAccept(app,obj);

	SAFE_DELETE(app);
	SAFE_DELETE(obj);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::getOneApproxLine\n");
	return(ret);
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
	while(!interp_lines->Czy_pelny() && interp_lines->getNumElem()>-1 && ret_evalnextparam==OK)	// dopuki bufor nie jest pe³ny i jednoczeœnie jest zainicjowany
	{
		obj = interp_lines->AddObject();	// dodaje now¹ liniê
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na górze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe. Wyniki s¹ zapamiêtywane w klasie i dostêpne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results->AddObject();
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
		pdeb = app->getApproxParams_ub();
		_RPT5(_CRT_WARN,"\t\tUB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
		pdeb = app->getApproxParams_lb();
		_RPT5(_CRT_WARN,"\t\tLB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam poprawnoœæ danych
		if(BLAD==czyAccept(app,obj))
		{
			approx_results->DelObject();
			interp_lines->DelObject();
			lineOK->push_back(BLAD);
			currentPos.Clear();
			weldPos->push_back(currentPos);	// wartoœci domyœlne
		} else {
			lineOK->push_back(OK);	// linia ok
			pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype³niam currentPos w³aœciwymi danymo
			weldPos->push_back(currentPos);
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
	if(P0.getX()>=rtg->_cols-1 || 0==step)	// jeœli jesteœmy na ostatniej kolumnie to nie mo¿na wygenerowac kolejnej
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
	if(approx_results->getNumElem()<=0)
		_RPT0(_CRT_ASSERT,"C_LinearWeld::evalNextParams->pusty bufor");
#endif
	/// \warning Jeœli bufor nie pe³ny to mo¿e zajœæ rozbierznoœæ pomiêdzy buforami, tzn w ró¿nych buforach ko³owych mo¿e byæ ró¿na iloœæ danych. Rozmiar danch tez ma znaczneie przy innych spawach niz liniowe. Trzeba pilnowac zeby zawsze tyle smo punktów by³o
	int num_el = approx_results->getNumElem(); // wielkoœæ bufora
	int num_points = interp_lines->GetFirstInitialized()->getSizeofInterpData();
#ifdef _DEBUG
	int num_elrec = recalculated_approx_data->getNumElem(); // wielkoœæ bufora
	_ASSERT(num_el==num_elrec); // powinny byæ tego samemgo rozmiaru te bufory
#endif
	C_Matrix_Container a(1,num_el);
	C_Matrix_Container b(1,num_el);
	C_Matrix_Container c(1,num_el);
	C_Matrix_Container d(1,num_el);
	C_Matrix_Container e(1,num_el);
//	C_Image_Container waga;	// pomocniczy do skalowania wag
	const double *p_par;	// parametry z bufora lub wektor wag
	// meidana z p - poszczególne elementy tego wektora z ca³ego bufora s¹ zbierane do jednego wektora
	_RPT1(_CRT_WARN,"\t\tSizebuff: %d",num_el);
	_RPT1(_CRT_WARN,"\t\tNumInterpElem: %d",num_points);
	// inicjalizacja wektora wag (globalny) inicjalizowany tylko raz
	_RPT1(_CRT_WARN,"\t\t_w = %p:",_w);
	if(NULL==_w)
	{
		_RPT0(_CRT_WARN,"\t\t_w initialized");
		_w = new double[num_points];
	}
	unsigned int l;
	for (int la=l=0;la<num_el;la++)
	{
		p_par = approx_results->GetObject(la)->getApproxParams_p(); if(NULL==p_par) continue;
		a.data[l] = p_par[A];
		b.data[l] = p_par[B];
		c.data[l] = p_par[C];
		d.data[l] = p_par[D];
		e.data[l] = p_par[E];
		l++;
	}
	_ASSERT(l==num_el);	// jeden element by³ NULL i mediana moze byæ fa³szywa bo nie wszystkie elementy wype³nione
	_p[A] = a.quick_select();
	_p[B] = b.quick_select();
	_p[C] = c.quick_select();
	_p[D] = d.quick_select();
	_p[E] = e.quick_select();

	// sprawdzanie czy parametry ub i lb nie s¹ takie same (dla 0 moga byæ)

	/// \bug Dla parametru ujemnego mo¿e byæ ¿e ub<lb. Poprawione poprzez dodanie fabs do parametrów.

 	_ub[A] = _p[A] + fabs(_p[A])*0.3; if(0==_ub[A]) _ub[A] = 0.2;
	_ub[B] = _p[B] + fabs(_p[B])*0.3; if(0==_ub[B]) _ub[B] = 0.2;
	_ub[C] = _p[C] + fabs(_p[C])*0.2; if(0==_ub[C]) _ub[C] = 0.1;
	_ub[D] = _p[D] + fabs(_p[D])*0.3; if(0==_ub[D]) _ub[D] = 0.2;
	_ub[E] = _p[E] + fabs(_p[E])*0.3; if(0==_ub[E]) _ub[E] = 0.2;

	_lb[A] = _p[A] - fabs(_p[A])*0.3; if(0==_lb[A]) _lb[A] = -0.2;
	_lb[B] = _p[B] - fabs(_p[B])*0.3; if(0==_lb[B]) _lb[B] = -0.2;
	_lb[C] = _p[C] - fabs(_p[C])*0.2; if(0==_lb[C]) _lb[C] = -0.1;
	_lb[D] = _p[D] - fabs(_p[D])*0.3; if(0==_lb[D]) _lb[D] = -0.2;
	_lb[E] = _p[E] - fabs(_p[E])*0.3; if(0==_lb[E]) _lb[E] = -0.2;

	
	
	// wagi
	// wype³nienie zerami

	for(int ld=0;ld<num_points;++ld)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld] = 0.0;
	unsigned int real_num_el = 0;	// rzeczywista ilosc linii w buforze - gdy niektóre nie bêd¹ zainicjalizowane
	for(int la=0;la<num_el;la++)	// po liniach w buforze
	{
		p_par = recalculated_approx_data->GetObject(la); if(NULL==p_par) continue; // biore dane z la linii w buforze - to s a wartoœci z aproxymacji wype³nione w fillbufor i start na bierz¹co z innymi buforami razem
		real_num_el++;
		for(int ld=0;ld<num_points;ld++)
			_w[ld]+=p_par[ld];	// dodaje do bufora wag
	}
	for(int ld=0;ld<num_points;ld++)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld]/=real_num_el;

	// skalowanie wag do zakresu 01
	/// \todo Tu mo¿na przyspieszyæ znacznie wykonuj¹c te operacje w miejscu - funkcja albo metoda statyczna lub stl
	/// \verbatim
//	waga.AllocateData(1,num_points);
//	waga.CopyfromTab(_w,num_points);
//	waga.Normalize(0,1);
	/// \endverbatim
	// kopiowanie spowrotem
//	memcpy_s(_w,num_points*sizeof(double),waga.data,waga.GetNumofElements()*sizeof(double));
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
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::czyAccept");
	_RPT1(_CRT_WARN,"\t\terr=%.3e",_approx->getInfo(err));
	if(7==_approx->getInfo(stopreason)){
		_RPT0(_CRT_WARN,"\t\t Deleted: reason");
		return BLAD;
	}else // zbieram przydatne parametry
		if(_approx->getInfo(err)>MAX_ERROR_LEVEL){
			_RPT0(_CRT_WARN,"\t\t Deleted: error");
			return BLAD;
		}
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::czyAccept\n");
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
	_RPT2(_CRT_WARN,"\t\t_weldPos.S.setPoint(%.1lf,%.1lf])",x[(int)pos],y[(int)pos]);

	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // wartoœæ trendu dla pozycji max 
	_RPT1(_CRT_WARN,"\t\tmax_lin = %lf.1",max_lin);

	// górna granica od œrodka do koñca
	licznik = 0;
	for(l=(unsigned int)pos;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysokoœci piku + przesuniêcie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	/// \warning Mo¿e siê zda¿yæ ze nie bedzie w ifie i wtedy wartoœæ indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak siê zda¿a gdy aproxymowane s¹ z³e dane i funkcja aproxymuj¹ca jest zbyt szeoka, tzn obeajmuje ca³y obraz i nie daje rady zmaleæ do ¿¹danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê góry na wyjœciu
	_RPT2(_CRT_WARN,"\t\t_weldPos.G.setPoint(%.1lf,%.1lf])",x[indexy[0]],y[indexy[0]]);

	// dolna granica do œrodka
	licznik = 0;
	for(l=(unsigned int)pos+1;l>0;--l) // +1aby unikn¹æ b³êdu z przekrêceniem siê zakresu!!
		if(_pre[l-1]<max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l-1;	// pierwszy element to pozycja górnej granicy
			break;	// nie ma potrzeby analizowania pozosta³ych el
		}
	/** 
	 * \warning - w tym kodzie wystêpuje bardzo ciekawy b³¹d:
	 * \code
	 licznik = 0;
	 for(l=(unsigned int)pos;l>=0;--l)
	 if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{
	 indexy[licznik++] = l;	// pierwszy element to pozycja górnej granicy
	 break;	// nie ma potrzeby analizowania pozosta³ych el
	 }
	 * \endcode
	 * l jest zdefinoowane jako uint i bior¹c pod uwagê kolejnosc dzia³ania for to dla l=1 zostanie
	 * wykonane pêtla, nastêpnie l bêdzie zmniejszone o 1, sprawdzony warunek (bêdzie na tak) i dalej 
	 * wykona siê petla, zmiejszy siê l o 1 i tu nastêpuje przekrêcenie zakresu bo l jest uint!! a warunek ciêgle bêdzie
	 * spe³niony bo l>0
	 */

	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycjê œrodka na wyjœciu
	_RPT2(_CRT_WARN,"\t\t_weldPos.D.setPoint(%.1lf,%.1lf])",x[indexy[0]],y[indexy[0]]);

	_RPT1(_CRT_WARN,"\t\tmaxlin: %.3lf",max_lin);
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
