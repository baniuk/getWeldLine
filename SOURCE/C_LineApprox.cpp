#include "C_LineApprox.h"


C_LineApprox::C_LineApprox()
{
	typ_interpolacji = SPLINE;
	image = NULL;
	im_size[0] = im_size[1] = im_size[2] = 0;
	N = getNumOfElements();
	interpolated_data = NULL;
}
/** 
 * Tworzy oniekt Intepolacji, do��czony do okre�lonego obrazka (trzeba konwertowa� z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _P0 punkt poczatkowy
 * \param[in] _P1 punkt ko�cowy lini interpolacji
 * \param[in] image wska�nik do obrazu na kt�rym b�dzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineApprox::C_LineApprox( APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1, const double *const _image, const unsigned int _size[]  ) :
							C_Line(_P0,_P1),
							typ_interpolacji(type)
{
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype�anianie ilosci ement�w
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=1;a<N;a++)
		image[a] = (float)_image[a];
	interpolated_data = NULL;
}
/** 
 * Tworzy oniekt Intepolacji, do��czony do okre�lonego obrazka (trzeba konwertowa� z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _a wsp�czynnik kierunkowy prostej interpolacyjnej
 * \param[in] _b wyraz wolny
 * \param[in] czy_pion czy prosta jest pionowa czy nie
 * \param[in] image wska�nik do obrazu na kt�rym b�dzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineApprox::C_LineApprox( APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion, const double *const _image, const unsigned int _size[]  ) :
							C_Line(_a,_b,_czy_pion),
							typ_interpolacji(type)
{
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype�anianie ilosci ement�w
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=1;a<N;a++)
		image[a] = (float)_image[a];
	interpolated_data = NULL;
}

C_LineApprox::~C_LineApprox()
{
	SAFE_DELETE(image);
	SAFE_DELETE(interpolated_data);
}
/** 
 * Zwraca warto�ci interpolowane z obrazu pomi�dzy punktami le��cymi na linii. Wartoci interpolowane zapisane s� w interpolated_data
 * \param[in] _P0 punkt pocz�tkowy
 * \param[in] _P2 punkt ko�cowy
 * \param[out] _outx wektor x o rozmiarze N
 * \param[out] _outy wektor y o rozmiarze n
 * \param[in] N ilo�� punkt�w pomi�dzy <P0;P1>
 * \return Je�li P0 i P1 nie le�� na linii to zwraca false i warto�ci w _out s� nieokre�lone
 * \warning Funkcja modyfikuje tablice image
 */
bool C_LineApprox::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int N )
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
	ret = C_Line::getPointsOnLine(_P0,_P1,_outx,_outy,N);	// tu musi by� wowo�ana funkcja z klasy podrz�dnej
	if(ret==false)
		return false;	// punkty nie na linii
	interpolated_data = new double[N];
	switch(typ_interpolacji)
	{
	case SPLINE:
		for(int a=0;a<N;a++) { // po wszystkich punktach
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

	return true;
}

unsigned int C_LineApprox::getNumOfElements() const
{
	unsigned int n=1;
	for(int a=0;a<2;a++)
		if(0!=im_size[a])
			n*=im_size[a];
	return n;
}
