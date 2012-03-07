#include "C_Line.h"

/** 
 * Konstruktor domy�lny - tworzy prost�pod k�tem 45 stopni
 */
C_Line::C_Line() :
	par_a(1),
	par_b(0),
	jest_pion(NORMALNA)
{

}
/** Tworzy prost� n podstawie parametr�w. Je�li cz_pion jest ustawione na PIONOWA to
 * wtedy warto�� a jest ignorowana. Nale�y dba� aby warto�� tej zmiennej odzwiercied�a�a
 * czy prosta jest naprawd� pionwa czy pozioma.
 * \param[in] _a Wsp�czynnik kierunkowy
 * \param[in] _b Wyraz wolny
 * \param[in] czy_pion Informacja czy prost� traktujemy jako pionow� (tylko wsp�czynnik b jest brany pod uwag� czy normaln�
 */
C_Line::C_Line( const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion )
{
	setLine(_a,_b,_czy_pion);
}

C_Line::~C_Line()
{

}

/** 
 * Zwraca wszystkie parametry opisuj�ce prost�
 * \param[out] a Wsp�czynnik kierunkowy
 * \param[ouy] b wyraz wolny
 * \return jest_pion zwraca TRUE jesli prosta jest pionowa
 */
KIERUNEK_PROSTEJ C_Line::getLine( double &_a, double &_b ) const
{
	_a = getA();
	_b = getB();
	return getjest_pion();
}

/** Tworzy prost� n podstawie parametr�w. Je�li cz_pion jest ustawione na PIONOWA to
 * wtedy warto�� a jest ignorowana. Nale�y dba� aby warto�� tej zmiennej odzwiercied�a�a
 * czy prosta jest naprawd� pionwa czy pozioma.
 * \param[in] _a Wsp�czynnik kierunkowy
 * \param[in] _b Wyraz wolny
 * \param[in] czy_pion Informacja czy prost� traktujemy jako pionow� (tylko wsp�czynnik b jest brany pod uwag� czy normaln�
 */
void C_Line::setLine( const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion )
{
	par_a = _a;
	par_b = _b;
	jest_pion = _czy_pion;
}
/** 
 * Generuje N punkt�w roz�o�onych w wektorze <x0,x1>
 * \param[in] x0 pocz�tek zakresu
 * \param[in] x1 koniec zakresu
 * \param[out] x_out tablica wyj�ciowa, musi by� rozmiaru N
 * \param[in] N rozmiar wektora
 */
void C_Line::LinSpace( double x0, double x1, double *x_out, int N ) const
{
	unsigned int a;

	for(a=0;a<=N-2;a++)
		x_out[a] = x0 + a*(x1-x0)/(N-1);
	x_out[a] = x1;
}
/** 
 * Oblicza N punkt�w na prostej i zwraca je. Dla prostej pionowej zwraca wektor 
 * wsp�czynnik�w b, dla pozosta�ych przypadk�w normalni wed�ug r�wnani 
 * y = ax+b
 * \param[in] x tablica punkt�w
 * \param[in] N ilo�� punkt�w w tablicy
 * \param[out] out obliczone punkty
 */
void C_Line::evalLine( const double * const x,int N, double *const out)
{
	int a;
	if(PIONOWA==jest_pion)
		for (a=0;a<N;a++)
			out[a] = par_b;
	else
		for (a=0;a<N;a++)
			out[a] = par_a*x[a]+par_b;
}
/** 
 * Zwraca prost� przechodz�c� przez dwa punkty
 * \param[in] P0 pierwszy punkt
 * \param[in] P1 drugi punkt
 * \return Info czy powsta�a prosta jest pionowa czy nie
 */
KIERUNEK_PROSTEJ C_Line::getLine2Points( const C_Point &P0, const C_Point &P1 )
{
	if(P1.getX() - P0.getX()==0)
	{
		par_a = 0;
		par_b = P1.getX();
		jest_pion = PIONOWA;	// prosta pionowa
	} else
	{
		par_a = (P1.getY()-P0.getY())/(P1.getX()-P0.getX());
		par_b = P1.getY()-par_a*P1.getX();
		jest_pion = NORMALNA;
	}
	return jest_pion;
}
/** 
 * Zwraca dwa punkty b�d�ce przeci�ciem si� prostej o okre�gu o promieniu R
 * oraz �rodku P0, za�o�enie jest �e P0 le�y na prostej
 * \param[in] P0 srodek okregu
 * \param[in] R promien okregu
 * \param[out] out tablica z punktami przeci�cia
 * \return FALSE je�li P0 nie le�y na prostej, TRUE je�li tak, dla false nie ma zwr�conych punkt�w
 * \warning P0 musi le�e� na prostej
 */
bool C_Line::LineCutCircle( const C_Point &P0,double R,C_Point (&out)[2] )
{
	if(PIONOWA==jest_pion)
		if(P0.getX()==par_b)	{
			out[0] = C_Point(P0.getX(),P0.getY()+R);
			out[1] = C_Point(P0.getX(),P0.getY()-R);
		}
		else
			return false;
	else
		if(P0.getY()==par_a*P0.getX()+par_b)	{
			out[0] = C_Point(	P0.getX()+R*sqrt(1/(par_a*par_a+1)),
								P0.getY()+R*par_a*sqrt(1/(par_a*par_a+1)));
			out[1] = C_Point(	P0.getX()-R*sqrt(1/(par_a*par_a+1)),
								P0.getY()-R*par_a*sqrt(1/(par_a*par_a+1)));
		}
		else
			return false;

	return true;
}
