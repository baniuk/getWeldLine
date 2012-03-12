#include "C_LinearWeld.h"

C_LinearWeld::C_LinearWeld( const C_Matrix_Container *_rtg ) : C_WeldlineDetect(_rtg)
{

}

C_LinearWeld::~C_LinearWeld()
{

}
/** 
 * Ustawia œrodowisko, tworzy potrzebne tablice. 
 * Funkcja musi byæ wywo³ana po konstruktorze aby ustawiæ potrzebne parametry.
 * \param[in] _k iloœæ linii branych do wygeneroania nastêpnej - wielkosæ bufora
 * \param[in] _sp punkt startowy - kolumna obrazka
 */
void C_LinearWeld::SetProcedureParameters(unsigned int _k,unsigned int _sp)
{
	k = _k;
	P0.setPoint(_sp,0); // dó³ obrazka
	P1.setPoint(_sp,rtg->_rows-1);	// góra obrazka
	// tworzenie buforów na przechowanie danych
	if(k>0)
	{
		approx_results.BuffInit(k);
		err.BuffInit(k);
		interp_lines.BuffInit(k);
	}
	

}

void C_LinearWeld::Start()
{

}
/** 
 * Procedura wype³nia bufory ko³owe. 
 * Dokonuje interpolacji k kolejnych linii obrazka oraz ich aproxymacji. Zapisuje obiekty w buforze ko³owym.
 */
void C_LinearWeld::fillBuffor()
{
	// interpolacja k kolejnych linii zacynaj¹c od startowej - poruszam siê po kolumnach
	C_LineInterp *obj;	// obiekt tymczasowy do ³atwiejszego adresowania
	C_LineWeldApprox *app;
	while(!interp_lines.Czy_pelny())	// dopuki bufor nie jest pe³ny
	{
		obj = interp_lines.AddObject();	// dodaje now¹ liniê
		// ustawiam parametry interpolacji - P0 lezy na dole obrazu, P1 na górze, linia pionowa
		obj->ManualConstructor(SPLINE,P0,P1,rtg->data,rtgsize);
		// wykonuje interpolacjê - biorê tyle punktów ile jest rzêdów w obrazie + punkty koñcowe
		obj->getPointsOnLine(P0,P1,rtg->_rows+1);
		// aproksymacja - dodaje obiekt aprox
		app = approx_results.AddObject();
		// konstruktor manualne
		app->ManualConstructor(SPLINE,obj->getInterpolated_X(),obj->getInterpolated_data(),rtg->_rows+1);
		// aproxymacja - parametry domyœlne
		app->setApproxParmas(NULL,NULL,NULL,NULL,NULL);
		// aproxymacja
		app->getLineApprox(100);
		// zbieram przydatne parametry
//		app->getInfo()
			//
	//		kasowanie jesli blad za duzy - nie zapamietujemy takiego
		// generuje nastêpne punkty
		evalNextParams();
	}
}
/** 
 * Generuje zestaw parametrów startowych do obliczenia kolejenj aproxymacji. Obraz jest
 * interpolowany na linii P0P1 a nastêpnie ten profil bêdzie podlega³ aproxymacji. Ka¿de wywo³anie funkcji 
 * powoduje wygenerowanie nastêpnego zestawu parametrów.
 * \return false jeœli nie mo¿na wygenerowac kolejnych parametrów (na przyk³¹d koniec obrazka)
 * \warning Funkcja modyfikuje pola P0 i P1
 * 
 */
bool C_LinearWeld::evalNextParams()
{
	// generowanie nowych wspó³rzêdnych dla linii inteprolacyjnej - do paproxymacji profilu
	if(P0.getX()==rtg->_cols-1)	// jeœli jesteœmy na ostatniej kolumnie to nie mo¿na wygenerowac kolejnej
		return false;
	C_Point delta(1,0);	// krok 1 po x
	P0+=delta;
	P1+=delta;
	return true;
}
