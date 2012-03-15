#include "C_LineInterp.h"

/** 
 * Podstawowy konstruktor - nie powinien by� u�ywany
 */
C_LineInterp::C_LineInterp() : C_Line()
{
	typ_interpolacji = SPLINE;
	image = NULL;
	im_size[0] = im_size[1] = im_size[2] = 0;
	N = getNumOfElements();
	Np = 0;
	interpolated_data = NULL;
	x = NULL;
	y = NULL;
}
/** 
 * Tworzy obiekt Intepolacji, do��czony do okre�lonego obrazka (trzeba konwertowa� z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _P0 punkt poczatkowy
 * \param[in] _P1 punkt ko�cowy lini interpolacji
 * \param[in] _image wska�nik do obrazu na kt�rym b�dzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineInterp::C_LineInterp( APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1, const double *const _image, const unsigned int _size[]  )
{
	ManualConstructor( type,_P0,_P1,_image,_size);
	interpolated_data = NULL;
	x = NULL;
	y = NULL;
	Np = 0;
}
/** 
 * Tworzy oniekt Intepolacji, do��czony do okre�lonego obrazka (trzeba konwertowa� z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _a wsp�czynnik kierunkowy prostej interpolacyjnej
 * \param[in] _b wyraz wolny
 * \param[in] _czy_pion czy prosta jest pionowa czy nie
 * \param[in] _image wska�nik do obrazu na kt�rym b�dzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineInterp::C_LineInterp( APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion, const double *const _image, const unsigned int _size[]  ) :
							C_Line(_a,_b,_czy_pion),
							typ_interpolacji(type),
							Np(0)
{
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype�anianie ilosci ement�w danych wejsciowych (obrazu)
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=0;a<N;a++)
		image[a] = (float)_image[a];
	interpolated_data = NULL;
	x = NULL;
	y = NULL;
}

C_LineInterp::~C_LineInterp()
{
	SAFE_DELETE(image);
	SAFE_DELETE(interpolated_data);
	SAFE_DELETE(x);
	SAFE_DELETE(y);
}
/** 
 * \warning Nie zalecany syntax, nale�y u�y� poni�ej
 * Zwraca warto�ci interpolowane z obrazu pomi�dzy punktami le��cymi na linii. Wartoci interpolowane zapisane s� w interpolated_data.
 * wsp�rz�dne dla kt�rych by�a inteprolacja r�wnierz
 * \param[in] _P0 punkt pocz�tkowy
 * \param[in] _P1 punkt ko�cowy
 * \param[out] _outx wektor x o rozmiarze N
 * \param[out] _outy wektor y o rozmiarze n
 * \param[in] _Np ilo�� punkt�w pomi�dzy <P0;P1>
 * \return Je�li P0 i P1 nie le�� na linii to zwraca false i warto�ci w _out s� nieokre�lone
 * \warning Funkcja modyfikuje tablice image
 */

bool C_LineInterp::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, unsigned int _Np )
{
	_RPT0(_CRT_WARN,"Obsolete sytnax, use without _outx");
	bool ret;
	if(!isPointOnLine(_P0))
		return false;
	if(!isPointOnLine(_P1))
		return false;
	int Error = SamplesToCoefficients(image, im_size[1], im_size[0], 2);
	if(Error)
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients");
	// wypeninie lini P0 P1 punktami r�wno roz�o�onymi
	Np = _Np;
	SafeAllocateTab();
	ret = C_Line::getPointsOnLine(_P0,_P1,_outx,_outy,Np);	// tu musi by� wowo�ana funkcja z klasy podrz�dnej
	if(ret==false)
		return false;	// punkty nie na linii
	switch(typ_interpolacji)
	{
	case SPLINE:
		for(unsigned int a=0;a<Np;a++) { // po wszystkich punktach
			interpolated_data[a] = (double)InterpolatedValue(image,
															im_size[1],
															im_size[0],
															_outx[a],
															_outy[a],
															2);

		}
		
		break;
	default:
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients - wrong type");
	}
	// kopiowanie na wyj�cie
	DataCopy(_outx,x);
	DataCopy(_outy,y);
	return true;
}
/** 
 * Zwraca warto�ci interpolowane z obrazu pomi�dzy punktami le��cymi na linii. Wartoci interpolowane zapisane s� w interpolated_data.
 * wsp�rz�dne dla kt�rych by�a inteprolacja r�wnierz zapisywane s� w x i y
 * \param[in] _P0 punkt pocz�tkowy
 * \param[in] _P1 punkt ko�cowy
 * \param[in] _Np ilo�� punkt�w pomi�dzy <P0;P1>
 * \return Je�li P0 i P1 nie le�� na linii to zwraca false i warto�ci w _out s� nieokre�lone
 * \warning Funkcja modyfikuje tablice image
 */
bool C_LineInterp::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, unsigned int _Np )
{
	bool ret;
	if(!isPointOnLine(_P0))
		return false;
	if(!isPointOnLine(_P1))
		return false;
	int Error = SamplesToCoefficients(image, im_size[1], im_size[0], 2);
	if(Error)
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients");
	// wypeninie lini P0 P1 punktami r�wno roz�o�onymi
	Np = _Np;
	SafeAllocateTab();
	ret = C_Line::getPointsOnLine(_P0,_P1,x,y,Np);	// tu musi by� wowo�ana funkcja z klasy podrz�dnej
	if(ret==false)
		return false;	// punkty nie na linii
	
	switch(typ_interpolacji)
	{
	case SPLINE:
		for(unsigned int a=0;a<Np;a++) { // po wszystkich punktach
			interpolated_data[a] = (double)InterpolatedValue(image,
				im_size[1],
				im_size[0],
				x[a],
				y[a],
				2);

		}

		break;
	default:
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients - wrong type");
	}
	return true;
}
/** 
 * Oblicza rozmiar obrazu wej�ciowego. Rozmiar ten jest dany za pomoc� tablicy [rows cols z]. Funkcja zwraca ilo�� element�w w tej tablicy.
 * Takie podej�cie jest potzrebne poniewa� funkcja inteproluj�ca wymaga rozmiar�w x i y obrazu (przyjmuje dane w forma,cie linowym zgodnym z C_Matrix_Container::data
 * \return Ilo�� element�w w obrazu wej�ciowego
 */
unsigned int C_LineInterp::getNumOfElements() const
{
	unsigned int n=1;
	for(int a=0;a<3;a++)
		if(0!=im_size[a])
			n*=im_size[a];
	return n;
}
/** 
 * Funkcja sprawdza czy nie ma poprzedniej tablicy w przypadku gdy kilka razy wywo�a si� funkcj� GetPointsOLine
 */
void C_LineInterp::SafeAllocateTab()
{
	if(interpolated_data!=NULL)
		SAFE_DELETE(interpolated_data);
	interpolated_data = new double[Np];
	if(NULL!=x)
		SAFE_DELETE(x);
	x = new double[Np];
	if(NULL!=y)
		SAFE_DELETE(y);
	y = new double[Np];
}

void C_LineInterp::DataCopy( double *src,double *dest )
{
	memcpy_s(dest,Np*sizeof(double),src,Np*sizeof(double));
}
/** 
 * Ustawia linieinterpolacji przy pomocy dw�ch punkt�w. Funkcja ta inicjaklizuje tak�e ca�y obiekt. Powinna by� u�ywana w przypadku u�ycia domy�lnego konstruktora.
 * \param[in] type Typ interpolacji
 * \param[in] P0 punkt poczatkowy
 * \param[in] P1 punkt ko�cowy lini interpolacji
 * \param[in] _image wska�nik do obrazu na kt�rym b�dzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */ 
void C_LineInterp::ManualConstructor( APPROX_TYPE type,const C_Point &P0, const C_Point &P1, const double *const _image, const unsigned int _size[] )
{
	typ_interpolacji = type;
	getLine2Points(P0,P1);	// inicjalizacja obieku Line
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype�anianie ilosci ement�w obrzy wej�ciowego
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=0;a<N;a++)
		image[a] = (float)_image[a];

}

