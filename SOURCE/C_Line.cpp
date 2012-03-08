#include "C_Line.h"

/** 
 * Konstruktor domyœlny - tworzy prost¹pod k¹tem 45 stopni
 */
C_Line::C_Line() :
	par_a(1),
	par_b(0),
	jest_pion(NORMALNA)
{

}
/** Tworzy prost¹ na podstawie parametrów. Jeœli cz_pion jest ustawione na PIONOWA to
 * wtedy wartoœæ a jest ignorowana. Nale¿y dbaæ aby wartoœæ tej zmiennej odzwiercied³a³a
 * czy prosta jest naprawdê pionwa czy pozioma.
 * \param[in] _a Wspó³czynnik kierunkowy
 * \param[in] _b Wyraz wolny
 * \param[in] czy_pion Informacja czy prost¹ traktujemy jako pionow¹ (tylko wspó³czynnik b jest brany pod uwagê czy normaln¹
 */
C_Line::C_Line( const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion )
{
	setLine(_a,_b,_czy_pion);
}
/** Tworzy prost¹ na podstawie punktów na niej
 * czy prosta jest naprawdê pionwa czy pozioma.
 * \param[in] _P0 pierwszy punkt na linii
 * \param[in] _P1 drugi punkt na linii
 */
C_Line::C_Line(const C_Point &_P0,const C_Point &_P1)
{
	getLine2Points(_P0,_P1);
}

C_Line::~C_Line()
{

}

/** 
 * Zwraca wszystkie parametry opisuj¹ce prost¹
 * \param[out] a Wspó³czynnik kierunkowy
 * \param[ouy] b wyraz wolny
 * \return jest_pion zwraca TRUE jesli prosta jest pionowa
 */
KIERUNEK_PROSTEJ C_Line::getLine( double &_a, double &_b ) const
{
	_a = getA();
	_b = getB();
	return getjest_pion();
}

/** Tworzy prost¹ n podstawie parametrów. Jeœli cz_pion jest ustawione na PIONOWA to
 * wtedy wartoœæ a jest ignorowana. Nale¿y dbaæ aby wartoœæ tej zmiennej odzwiercied³a³a
 * czy prosta jest naprawdê pionwa czy pozioma.
 * \param[in] _a Wspó³czynnik kierunkowy
 * \param[in] _b Wyraz wolny
 * \param[in] czy_pion Informacja czy prost¹ traktujemy jako pionow¹ (tylko wspó³czynnik b jest brany pod uwagê czy normaln¹
 */
void C_Line::setLine( const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion )
{
	par_a = _a;
	par_b = _b;
	jest_pion = _czy_pion;
}
/** 
 * Generuje N punktów roz³o¿onych w wektorze <x0,x1>
 * \param[in] x0 pocz¹tek zakresu
 * \param[in] x1 koniec zakresu
 * \param[out] x_out tablica wyjœciowa, musi byæ rozmiaru N
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
 * Oblicza N punktów na prostej i zwraca je. Dla prostej pionowej zwraca wektor 
 * wspó³czynników b, dla pozosta³ych przypadków normalni wed³ug równani 
 * y = ax+b
 * \param[in] x tablica punktów
 * \param[in] N iloœæ punktów w tablicy
 * \param[out] out obliczone punkty
 */
void C_Line::evalLine( const double * const x,int N, double *const out) const
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
 * Zwraca prost¹ przechodz¹c¹ przez dwa punkty
 * \param[in] P0 pierwszy punkt
 * \param[in] P1 drugi punkt
 * \return Info czy powsta³a prosta jest pionowa czy nie
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
 * Zwraca dwa punkty bêd¹ce przeciêciem siê prostej o okreêgu o promieniu R
 * oraz œrodku P0, za³o¿enie jest ¿e P0 le¿y na prostej
 * \param[in] P0 srodek okregu
 * \param[in] R promien okregu
 * \param[out] out tablica z punktami przeciêcia
 * \return FALSE jeœli P0 nie le¿y na prostej, TRUE jeœli tak, dla false nie ma zwróconych punktów
 * \warning P0 musi le¿eæ na prostej
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
/** 
 * Zwraca wartoœci y=ax+b pomiêdzy punktami le¿¹cymi na linii
 * \param[in] _P0 punkt pocz¹tkowy
 * \param[in] _P2 punkt koñcowy
 * \param[out] _outx wektor x o rozmiarze N
 * \param[out] _outy wektor y o rozmiarze n
 * \param[in] N iloœæ punktów pomiêdzy <P0;P1>
 * \return Jeœli P0 i P1 nie le¿¹ na linii to zwraca false i wartoœci w _out s¹ nieokreœlone
 */
bool C_Line::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N ) const
{
	if(!isPointOnLine(_P0))
		return false;
	if(!isPointOnLine(_P1))
		return false;
	if(PIONOWA==jest_pion){
		// jeœli pionowa to dzielimy po y
		LinSpace(_P0.getY(),_P1.getY(),_outy,N);
		for(int a=0;a<N;a++)
			_outx[a] = _P0.getX();
	}
	else {
		LinSpace(_P0.getX(),_P1.getX(),_outx,N);
		evalLine(_outx,N,_outy);
	}
	return true;
}
/** 
 * Sprawdza czy punkt jest na linii
 * \return false jeœli nie jest na linii
 */
bool C_Line::isPointOnLine( const C_Point &_P0 ) const
{
	if(PIONOWA==jest_pion) {
		if(_P0.getX()==par_b)
			return true;
	}
	else {
		if(_P0.getY() == par_a*_P0.getX()+par_b)
			return true;
	}
	return false;
}
