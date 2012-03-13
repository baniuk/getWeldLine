#include "C_LinearWeld.h"



C_LinearWeld::C_LinearWeld( const C_Matrix_Container *_rtg ) : C_WeldlineDetect(_rtg)
{

}

C_LinearWeld::~C_LinearWeld()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::~C_LinearWeld\n");
}
/** 
 * Ustawia �rodowisko, tworzy potrzebne tablice. 
 * Funkcja musi by� wywo�ana po konstruktorze aby ustawi� potrzebne parametry.
 * \param[in] _k ilo�� linii branych do wygeneroania nast�pnej - wielkos� bufora
 * \param[in] _sp punkt startowy - kolumna obrazka
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k,unsigned int _sp)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::SetProcedureParameters");
	k = _k;
	P0.setPoint(_sp,0); // d� obrazka
	P1.setPoint(_sp,rtg->_rows-1);	// g�ra obrazka
	// tworzenie bufor�w na przechowanie danych
	if(k>0)
	{
		_RPT1(_CRT_WARN,"\t\tk = %d",k);
		approx_results.BuffInit(k);
		ERR.BuffInit(k);
		interp_lines.BuffInit(k);
	}
	_RPT5(_CRT_WARN,"\t\tk=%.3lf, P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",k,P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::SetProcedureParameters\n");
}

void C_LinearWeld::Start()
{

}
/** 
 * Procedura wype�nia bufory ko�owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko�owym.
 * \return OK je�li uda�o si�wype�ni�, BLAD je�li wystapi� b��d
 */
bool C_LinearWeld::fillBuffor()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::fillBuffor");
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
		else
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofApproxData()); // to nie powinno byc nigdy wykonene w tej wersji
		// aproxymacja - parametry domy�lne
		app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
		// aproxymacja
		app->getLineApprox(100);
		// sprawdzam powodzenie interpolacji
#ifdef _DEBUG
		const double *pdeb;pdeb = app->getApproxParams_p();
#endif
		_RPT5(_CRT_WARN,"\t\tRES: A=%.2lf B=%.2lf C=%.2lf D=%.2lf E=%.2lf",pdeb[A],pdeb[B],pdeb[C],pdeb[D],pdeb[E]);
		_RPT1(_CRT_WARN," err=%.3e",app->getInfo(err));
		if(7==app->getInfo(stopreason)){
			_RPT0(_CRT_WARN,"\t\t Deleted: reason");
			approx_results.DelObject();	// inny b��d
			interp_lines.DelObject();
		}else // zbieram przydatne parametry
			if(app->getInfo(err)>MAX_ERROR_LEVEL){
				_RPT0(_CRT_WARN,"\t\t Deleted: error");
				approx_results.DelObject();	// je�li b��d jest za du�y to kasuje tak� lini�
				interp_lines.DelObject();
			}
		// generuje nast�pne punkty
		ret_evalnextparam = evalNextParams();
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
bool C_LinearWeld::evalNextParams()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextParams");
	// generowanie nowych wsp�rz�dnych dla linii inteprolacyjnej - do paproxymacji profilu
	if(P0.getX()==rtg->_cols-1)	// je�li jeste�my na ostatniej kolumnie to nie mo�na wygenerowac kolejnej
		return BLAD;
	C_Point delta(1,0);	// krok 1 po x
	_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	P0+=delta;
	P1+=delta;
	_RPT4(_CRT_WARN,"\t\tOutput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalNextParams\n");
	return OK;
}
