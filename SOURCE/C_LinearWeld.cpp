/**
 * \file    C_LinearWeld.cpp
 * \brief	Implementuje dekecje spaw�w liniowych
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
 * Ustawia �rodowisko, tworzy potrzebne tablice. 
 * Funkcja musi by� wywo�ana po konstruktorze aby ustawi� potrzebne parametry.
 * \param[in] _k ilo�� linii branych do wygeneroania nast�pnej - wielkos� bufora
 * \param[in] _StartPoint punkt startowy - kolumna obrazka w tym przypadku (sk��dowa x)
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k, C_Point _StartPoint)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::SetProcedureParameters");
	k = _k;
	_ASSERT(!(_StartPoint.getX()<1) ||
			!(_StartPoint.getX()>rtg->_cols-1)); // punkt poza obrazem
	P0.setPoint(_StartPoint.getX(),0); // d� obrazka
	P1.setPoint(_StartPoint.getX(),rtg->_rows-1);	// g�ra obrazka
	// tworzenie bufor�w na przechowanie danych
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
 * Uruchamia w�a�ciwy proces wykrywnia spawu.
 * \param[in] step Krok z jakimjest wykrywaie spawu. Bufor zawsze jest ype�niany z krokiem 1
 * \param[in] ile Ile linii ma by� aproxymowanych, jesli 0 to do ko�ca spawu
 * \return OK je�li uda�o si� zako�czy�, BLAD je�li wystapi� b��d. BLAD jest zwracany jesli nie uda sie stworzyc bufora lub krok by�o r�wny 0
 */
bool C_LinearWeld::Start(unsigned int step,unsigned int ile)
{
	// bierz�ca pozycja spawu w przestrzeni obrazka
	bool liniaok = OK;	// czy aproxymacja bierzacej lini zako�czy�a si� sukcesem. Je�li tak to zosta�a ona w��czona do bufora i treba go przeliczy�. Jesli nie to nie ma takiej potrzeby
	unsigned int ile_done=0;	// ile linii zosta�o ju� zaproxymowanych, licz� si�wszystkie nawet te z�e
	C_WeldPos currentPos;
	C_LineInterp *obj;	// obiekt tymczasowy do �atwiejszego adresowania
	C_LineWeldApprox *app;
	const double *y;	// dane x do aproxymacji
	double *pre;	// wska�nik na dan� w buforze precalculated_approx_data
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::Start");
	bool ret=OK;
	int iter;
	// wype�nianie bufora w zale�no�ci od punktu startowego podanego w SetProcedureParameters()
	ret = fillBuffor();
	if(BLAD==ret)
		return ret;
	// w P0P1 jest ju� nast�pny punkt bo zosta� wygenerowany w fillBuffor()
	// jesli ile jest 0 to nie uwzgl�dniamy tego warunku stopu. ile jest ustawiane na warto�� max to zawsze bedzie warunek spe�niony
	if(0==ile) ile = UINT_MAX;
	while(OK==ret && ile_done<ile)
	{
		// generowanie nast paramer�w na podstawie bufora wype�niane s� zmienne _p, _ub, _lb, _w
		if(liniaok)	// przeliczamy nowe parametry jesli by�a zmiana w buforze
			evalNextParams();
		// generowanie linii itp
		obj = interp_lines->AddObject();	// dodaje now� lini�
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na g�rze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacj� - bior� tyle punkt�w ile jest rz�d�w w obrazie + punkty ko�cowe. Wyniki s� zapami�tywane w klasie i dost�pne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results->AddObject();
		if(obj->getjest_pion()==PIONOWA) // sprawdzam bo jak jest pionowa to aproxymacja jest w funkcji y a je�li nie to x
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
		else
			_RPT0(_CRT_ASSERT,"C_LinearWeld::fillBuffor()->linia nie jest pionowa"); // to nie powinno byc nigdy wykonene w tej wersji
		// aproxymacja - parametry obliczone przez evalNextParams()
		app->setApproxParmas(_p,_w,_ub,_lb,NULL);
		// aproxymacja
		/// \todo doda� tu sprawdzanie czy zwraca poprawn� ilo�� iteracji
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
			liniaok = BLAD;	// nie dodali�my nowej linii
			approx_results->DelObject();
			interp_lines->DelObject();
			lineOK->push_back(liniaok);	// te wektory zawieraj� ju� info o liniach bufora
			currentPos.Clear();	// kasuje wszystko bo ten wpis nie wa�ny
			weldPos->push_back(currentPos);
		} else {
			liniaok = OK;	// dodlai�my nowa lini�
			lineOK->push_back(liniaok);	// linia ok
			pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych -dodawan etylko dla dobrych linii, wiec nie ma potrzeby kasownaia dla z�ych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
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
 * Oblicza jedn� lini� dla spawu i zwraca pozycje brzeg�w i �rodka. Sprawdza poprwno�� aproksymacji
 * oraz nie bierze pod uwag� bufor�w. Linia obliczana dla bierz�cych punkt�w (tych zdefinoowanych w klasie).
 * Mo�na je zmienia� poprzez evalNextStartPoint. Ta funkcja wymaga zainicjalizowanej klasy jak w normalnym przypadku.
 * \param[out] weldpos Struktura z po�o�eniem spawu dla jednego punktu
 * \return OK je�li aproxymacja dobra, BLAD je�li nie
 */
bool C_LinearWeld::getOneApproxLine(C_WeldPos &weldpos)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::getOneApproxLine");
	C_LineInterp *obj = NULL;	// obiekt tymczasowy do �atwiejszego adresowania
	C_LineWeldApprox *app = NULL;
	double *pre;	// wska�nik na dan� w buforze precalculated_approx_data
	const double *y;	// dane x do aproxymacji

	bool ret;
	int iter;
	C_WeldPos wp;

	obj = new C_LineInterp;
	obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
	// wykonuje interpolacj� - bior� tyle punkt�w ile jest rz�d�w w obrazie + punkty ko�cowe. Wyniki s� zapami�tywane w klasie i dost�pne poprzez getInterpolated
	obj->getPointsOnLine(P0,P1,rtg->_rows+1);

	app = new C_LineWeldApprox;
	app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_Y(),obj->getSizeofInterpData());
	// aproxymacja - parametry domy�lne
	app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
	// aproxymacja
	iter = app->getLineApprox(200);
	_RPT1(_CRT_WARN,"\t\tITER: %d",iter);
	double sr = app->getInfo(stopreason);
	_RPT1(_CRT_WARN,"\t\tSTOPREASON: %lf",sr);

	pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych
	y = obj->getInterpolated_Y();
	app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
	evalWeldPos(app,obj,pre,weldpos);	// wype�niam currentPos w�a�ciwymi danymo

	ret = czyAccept(app,obj);

	SAFE_DELETE(app);
	SAFE_DELETE(obj);
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::getOneApproxLine\n");
	return(ret);
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
	while(!interp_lines->Czy_pelny() && interp_lines->getNumElem()>-1 && ret_evalnextparam==OK)	// dopuki bufor nie jest pe�ny i jednocze�nie jest zainicjowany
	{
		obj = interp_lines->AddObject();	// dodaje now� lini�
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na g�rze, linia pionowa
		_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacj� - bior� tyle punkt�w ile jest rz�d�w w obrazie + punkty ko�cowe. Wyniki s� zapami�tywane w klasie i dost�pne poprzez getInterpolated
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results->AddObject();
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
		pdeb = app->getApproxParams_ub();
		_RPT5(_CRT_WARN,"\t\tUB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
		pdeb = app->getApproxParams_lb();
		_RPT5(_CRT_WARN,"\t\tLB: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
#endif
		// sprawdzam poprawno�� danych
		if(BLAD==czyAccept(app,obj))
		{
			approx_results->DelObject();
			interp_lines->DelObject();
			lineOK->push_back(BLAD);
			currentPos.Clear();
			weldPos->push_back(currentPos);	// warto�ci domy�lne
		} else {
			lineOK->push_back(OK);	// linia ok
			pre = recalculated_approx_data->AddObject(obj->getSizeofInterpData());	// przeliczanie danych
			y = obj->getInterpolated_Y();
			app->evalApproxFcnVec(y,pre,obj->getSizeofInterpData());
			evalWeldPos(app,obj,pre,currentPos);	// wype�niam currentPos w�a�ciwymi danymo
			weldPos->push_back(currentPos);
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
	if(P0.getX()>=rtg->_cols-1 || 0==step)	// je�li jeste�my na ostatniej kolumnie to nie mo�na wygenerowac kolejnej
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
	if(approx_results->getNumElem()<=0)
		_RPT0(_CRT_ASSERT,"C_LinearWeld::evalNextParams->pusty bufor");
#endif
	/// \warning Je�li bufor nie pe�ny to mo�e zaj�� rozbierzno�� pomi�dzy buforami, tzn w r�nych buforach ko�owych mo�e by� r�na ilo�� danych. Rozmiar danch tez ma znaczneie przy innych spawach niz liniowe. Trzeba pilnowac zeby zawsze tyle smo punkt�w by�o
	int num_el = approx_results->getNumElem(); // wielko�� bufora
	int num_points = interp_lines->GetFirstInitialized()->getSizeofInterpData();
#ifdef _DEBUG
	int num_elrec = recalculated_approx_data->getNumElem(); // wielko�� bufora
	_ASSERT(num_el==num_elrec); // powinny by� tego samemgo rozmiaru te bufory
#endif
	C_Matrix_Container a(1,num_el);
	C_Matrix_Container b(1,num_el);
	C_Matrix_Container c(1,num_el);
	C_Matrix_Container d(1,num_el);
	C_Matrix_Container e(1,num_el);
//	C_Image_Container waga;	// pomocniczy do skalowania wag
	const double *p_par;	// parametry z bufora lub wektor wag
	// meidana z p - poszczeg�lne elementy tego wektora z ca�ego bufora s� zbierane do jednego wektora
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
	_ASSERT(l==num_el);	// jeden element by� NULL i mediana moze by� fa�szywa bo nie wszystkie elementy wype�nione
	_p[A] = a.quick_select();
	_p[B] = b.quick_select();
	_p[C] = c.quick_select();
	_p[D] = d.quick_select();
	_p[E] = e.quick_select();

	// sprawdzanie czy parametry ub i lb nie s� takie same (dla 0 moga by�)

	/// \bug Dla parametru ujemnego mo�e by� �e ub<lb. Poprawione poprzez dodanie fabs do parametr�w.

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
	// wype�nienie zerami

	for(int ld=0;ld<num_points;++ld)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld] = 0.0;
	unsigned int real_num_el = 0;	// rzeczywista ilosc linii w buforze - gdy niekt�re nie b�d� zainicjalizowane
	for(int la=0;la<num_el;la++)	// po liniach w buforze
	{
		p_par = recalculated_approx_data->GetObject(la); if(NULL==p_par) continue; // biore dane z la linii w buforze - to s a warto�ci z aproxymacji wype�nione w fillbufor i start na bierz�co z innymi buforami razem
		real_num_el++;
		for(int ld=0;ld<num_points;ld++)
			_w[ld]+=p_par[ld];	// dodaje do bufora wag
	}
	for(int ld=0;ld<num_points;ld++)	// dzielenie oprzez ilosc kazdej warotsci
		_w[ld]/=real_num_el;

	// skalowanie wag do zakresu 01
	/// \todo Tu mo�na przyspieszy� znacznie wykonuj�c te operacje w miejscu - funkcja albo metoda statyczna lub stl
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
 * Funkcja spawdza czy uzyskana interpolacja mo�e by� zaakceptowana.
 * \param[in] _approx Wska�nik do obiektu z aproksymacj� linii
 * \param[in] _interp Wska�nik do obietu z interpolacj� danych
 * \return OK je�li dane s� akceptowalne, BLAD je�li nie
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
	_RPT2(_CRT_WARN,"\t\t_weldPos.S.setPoint(%.1lf,%.1lf])",x[(int)pos],y[(int)pos]);

	p = _approx->getApproxParams_p();
	max_lin = y[(int)pos]*p[D]+p[E]; // warto�� trendu dla pozycji max 
	_RPT1(_CRT_WARN,"\t\tmax_lin = %lf.1",max_lin);

	// g�rna granica od �rodka do ko�ca
	licznik = 0;
	for(l=(unsigned int)pos;l<_interp->getSizeofInterpData();++l)
		if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{	// procent wysoko�ci piku + przesuni�cie liniowe
			indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	/// \warning Mo�e si� zda�y� ze nie bedzie w ifie i wtedy warto�� indexy niezdefiniowana, wtedy przyjmuje ostatni i pierwszy element. Tak si� zda�a gdy aproxymowane s� z�e dane i funkcja aproxymuj�ca jest zbyt szeoka, tzn obeajmuje ca�y obraz i nie daje rady zmale� do ��danego zakresu.
	if(0==licznik)
		indexy[0] = _interp->getSizeofInterpData()-1;// ostatni element
	_weldPos.G.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� g�ry na wyj�ciu
	_RPT2(_CRT_WARN,"\t\t_weldPos.G.setPoint(%.1lf,%.1lf])",x[indexy[0]],y[indexy[0]]);

	// dolna granica do �rodka
	licznik = 0;
	for(l=(unsigned int)pos+1;l>0;--l) // +1aby unikn�� b��du z przekr�ceniem si� zakresu!!
		if(_pre[l-1]<max_lin+(max_el-max_lin)*WELD_EDGE)	{
			indexy[licznik++] = l-1;	// pierwszy element to pozycja g�rnej granicy
			break;	// nie ma potrzeby analizowania pozosta�ych el
		}
	/** 
	 * \warning - w tym kodzie wyst�puje bardzo ciekawy b��d:
	 * \code
	 licznik = 0;
	 for(l=(unsigned int)pos;l>=0;--l)
	 if(_pre[l]<max_lin+(max_el-max_lin)*WELD_EDGE)	{
	 indexy[licznik++] = l;	// pierwszy element to pozycja g�rnej granicy
	 break;	// nie ma potrzeby analizowania pozosta�ych el
	 }
	 * \endcode
	 * l jest zdefinoowane jako uint i bior�c pod uwag� kolejnosc dzia�ania for to dla l=1 zostanie
	 * wykonane p�tla, nast�pnie l b�dzie zmniejszone o 1, sprawdzony warunek (b�dzie na tak) i dalej 
	 * wykona si� petla, zmiejszy si� l o 1 i tu nast�puje przekr�cenie zakresu bo l jest uint!! a warunek ci�gle b�dzie
	 * spe�niony bo l>0
	 */

	if(0==licznik)
		indexy[0] = 0;// pierwszy element
	_weldPos.D.setPoint(x[indexy[0]],y[indexy[0]]); // ustawiam pozycj� �rodka na wyj�ciu
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
