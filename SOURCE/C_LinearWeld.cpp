#include "C_LinearWeld.h"



C_LinearWeld::C_LinearWeld( const C_Matrix_Container *_rtg ) : C_WeldlineDetect(_rtg)
{

}

C_LinearWeld::~C_LinearWeld()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::~C_LinearWeld\n");
}
/** 
 * Ustawia œrodowisko, tworzy potrzebne tablice. 
 * Funkcja musi byæ wywo³ana po konstruktorze aby ustawiæ potrzebne parametry.
 * \param[in] _k iloœæ linii branych do wygeneroania nastêpnej - wielkosæ bufora
 * \param[in] _sp punkt startowy - kolumna obrazka
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k,unsigned int _sp)
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::SetProcedureParameters");
	k = _k;
	P0.setPoint(_sp,0); // dó³ obrazka
	P1.setPoint(_sp,rtg->_rows-1);	// góra obrazka
	// tworzenie buforów na przechowanie danych
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
 * Procedura wype³nia bufory ko³owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko³owym.
 * \return OK jeœli uda³o siêwype³niæ, BLAD jeœli wystapi³ b³¹d
 */
bool C_LinearWeld::fillBuffor()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::fillBuffor");
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
		else
			app->ManualConstructor(typeGaussLin,obj->getInterpolated_data(),obj->getInterpolated_X(),obj->getSizeofApproxData()); // to nie powinno byc nigdy wykonene w tej wersji
		// aproxymacja - parametry domyœlne
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
			approx_results.DelObject();	// inny b³¹d
			interp_lines.DelObject();
		}else // zbieram przydatne parametry
			if(app->getInfo(err)>MAX_ERROR_LEVEL){
				_RPT0(_CRT_WARN,"\t\t Deleted: error");
				approx_results.DelObject();	// jeœli b³¹d jest za du¿y to kasuje tak¹ liniê
				interp_lines.DelObject();
			}
		// generuje nastêpne punkty
		ret_evalnextparam = evalNextParams();
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
bool C_LinearWeld::evalNextParams()
{
	_RPT0(_CRT_WARN,"\tEntering C_LinearWeld::evalNextParams");
	// generowanie nowych wspó³rzêdnych dla linii inteprolacyjnej - do paproxymacji profilu
	if(P0.getX()==rtg->_cols-1)	// jeœli jesteœmy na ostatniej kolumnie to nie mo¿na wygenerowac kolejnej
		return BLAD;
	C_Point delta(1,0);	// krok 1 po x
	_RPT4(_CRT_WARN,"\t\tInput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	P0+=delta;
	P1+=delta;
	_RPT4(_CRT_WARN,"\t\tOutput: P0[%.1lf;%.1lf] P1[%.1lf;%.1lf]",P0.getX(),P0.getY(),P1.getX(),P1.getY());
	_RPT0(_CRT_WARN,"\tLeaving C_LinearWeld::evalNextParams\n");
	return OK;
}
