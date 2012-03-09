#include "C_LineInterp.h"

/** 
 * Podstawowy konstruktor - nie powinien byæ u¿ywany
 */
C_LineInterp::C_LineInterp()
{
	typ_interpolacji = SPLINE;
	image = NULL;
	im_size[0] = im_size[1] = im_size[2] = 0;
	N = getNumOfElements();
	interpolated_data = NULL;
	x = NULL;
	y = NULL;
}
/** 
 * Tworzy oniekt Intepolacji, do³¹czony do okreœlonego obrazka (trzeba konwertowaæ z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _P0 punkt poczatkowy
 * \param[in] _P1 punkt koñcowy lini interpolacji
 * \param[in] image wskaŸnik do obrazu na którym bêdzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineInterp::C_LineInterp( APPROX_TYPE type,const C_Point &_P0, const C_Point &_P1, const double *const _image, const unsigned int _size[]  ) :
							C_Line(_P0,_P1),
							typ_interpolacji(type)
{
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype³anianie ilosci ementów
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=1;a<N;a++)
		image[a] = (float)_image[a];
	interpolated_data = NULL;
	x = NULL;
	y = NULL;
}
/** 
 * Tworzy oniekt Intepolacji, do³¹czony do okreœlonego obrazka (trzeba konwertowaæ z C_Matrix_Container
 * \param[in] type Typ interpolacji
 * \param[in] _a wspó³czynnik kierunkowy prostej interpolacyjnej
 * \param[in] _b wyraz wolny
 * \param[in] czy_pion czy prosta jest pionowa czy nie
 * \param[in] image wskaŸnik do obrazu na którym bêdzie intepolaowana linia
 * \param[in] _size tablica z wymiarami obrazu [rows,cols,z]
 */
C_LineInterp::C_LineInterp( APPROX_TYPE type,const double &_a, const double &_b, const KIERUNEK_PROSTEJ &_czy_pion, const double *const _image, const unsigned int _size[]  ) :
							C_Line(_a,_b,_czy_pion),
							typ_interpolacji(type)
{
	// kopiowanie tablicy z rozmiarami danych
	memcpy_s(im_size,sizeof(im_size),_size,3*sizeof(unsigned int));
	// wype³anianie ilosci ementów
	N = getNumOfElements();
	// tworzenie bufora z danymi
	image = new float[N];
	// kopiowanie danych z zewnatrz do bufora
	for (unsigned int a=1;a<N;a++)
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
 * \warning Nie zalecany syntax, nale¿y u¿yæ poni¿ej
 * Zwraca wartoœci interpolowane z obrazu pomiêdzy punktami le¿¹cymi na linii. Wartoci interpolowane zapisane s¹ w interpolated_data.
 * wspó³rzêdne dla których by³a inteprolacja równierz
 * \param[in] _P0 punkt pocz¹tkowy
 * \param[in] _P2 punkt koñcowy
 * \param[out] _outx wektor x o rozmiarze N
 * \param[out] _outy wektor y o rozmiarze n
 * \param[in] Np iloœæ punktów pomiêdzy <P0;P1>
 * \return Jeœli P0 i P1 nie le¿¹ na linii to zwraca false i wartoœci w _out s¹ nieokreœlone
 * \warning Funkcja modyfikuje tablice image
 */

bool C_LineInterp::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, double *const _outx, double *const _outy, int Np )
{
	_RPT0(CRT_WARNING_POP,"Obsolete sytnax, use without _outx");
	bool ret;
	if(!isPointOnLine(_P0))
		return false;
	if(!isPointOnLine(_P1))
		return false;
	int Error = SamplesToCoefficients(image, im_size[1], im_size[0], 2);
	if(Error)
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients");
	// wypeninie lini P0 P1 punktami równo roz³o¿onymi
	ret = C_Line::getPointsOnLine(_P0,_P1,_outx,_outy,Np);	// tu musi byæ wowo³ana funkcja z klasy podrzêdnej
	if(ret==false)
		return false;	// punkty nie na linii
	SafeAllocateTab();
	switch(typ_interpolacji)
	{
	case SPLINE:
		for(int a=0;a<Np;a++) { // po wszystkich punktach
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
	DataCopy(_outx,x);
	DataCopy(_outy,y);
	return true;
}
/** 
 * Zwraca wartoœci interpolowane z obrazu pomiêdzy punktami le¿¹cymi na linii. Wartoci interpolowane zapisane s¹ w interpolated_data.
 * wspó³rzêdne dla których by³a inteprolacja równierz zapisywane s¹ w x i y
 * \param[in] _P0 punkt pocz¹tkowy
 * \param[in] _P2 punkt koñcowy
 * \param[in] N iloœæ punktów pomiêdzy <P0;P1>
 * \return Jeœli P0 i P1 nie le¿¹ na linii to zwraca false i wartoœci w _out s¹ nieokreœlone
 * \warning Funkcja modyfikuje tablice image
 */
bool C_LineInterp::getPointsOnLine( const C_Point &_P0, const C_Point &_P1, int Np )
{
	bool ret;
	if(!isPointOnLine(_P0))
		return false;
	if(!isPointOnLine(_P1))
		return false;
	int Error = SamplesToCoefficients(image, im_size[1], im_size[0], 2);
	if(Error)
		_RPT0(_CRT_ERROR,"Error in C_LineApprox::getPointsOnLine->SamplesToCoefficients");
	// wypeninie lini P0 P1 punktami równo roz³o¿onymi
	ret = C_Line::getPointsOnLine(_P0,_P1,x,y,Np);	// tu musi byæ wowo³ana funkcja z klasy podrzêdnej
	if(ret==false)
		return false;	// punkty nie na linii
	SafeAllocateTab();
	switch(typ_interpolacji)
	{
	case SPLINE:
		for(int a=0;a<Np;a++) { // po wszystkich punktach
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

unsigned int C_LineInterp::getNumOfElements() const
{
	unsigned int n=1;
	for(int a=0;a<2;a++)
		if(0!=im_size[a])
			n*=im_size[a];
	return n;
}
/** 
 * Funkcja sprawdza czy nie ma poprzedniej tablicy w przypadku gdy kilka razy wywo³a siê funkcjê GetPointsOLine
 */
void C_LineInterp::SafeAllocateTab()
{
	if(interpolated_data!=NULL)
		SAFE_DELETE(interpolated_data);
	interpolated_data = new double[N];
	if(NULL!=x)
		SAFE_DELETE(x);
	x = new double[N];
	if(NULL!=y)
		SAFE_DELETE(y);
	y = new double[N];
}

void C_LineInterp::DataCopy( double *src,double *dest )
{
	memcpy_s(dest,N*sizeof(double),src,N*sizeof(double));
}

