/**
 * \file    C_CircBuff.h
 * \brief	Bufor ko�owy
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_CircBuff_h__
#define C_CircBuff_h__

/** 
 * Klasa obs�uguj�ca bufor ko�owy. Umo�liwia realziacj� bufora ko�owego ale przy indeksowaniu liniowym. Dane s� dopisuwane na kolejne 
 * miejsce zraz za oststnio dopisanym. T jest typem obiektu a S rozmiarem bufora. Bepo�rednio po zapisaniu elementu do bufora mo�na go skasowa�. Funkcja getNumElements() zwraca ilo�� element�w w buforze kompatybiln� z funkcj� GetObject, dzi�ki czemu mo�na j� adresowa� za pomoc� p�tli. Przyk�ady u�ycia w pliku TESTS.cpp w C_WeldlineDetet_TESTS.\n
 * Kolejno�� u�ywania:\n
 * \li Stworzy� obiekt u�ywaj�c konstruktora bezparametrowego
 * \li Doda� obiekty u�ywaj�c AddObject, kt�ra zwrca adres nowododanego obiektu
 * \li Po ka�dej tej instrukcji mo�na odwo�a� si� do tego obiektu lub go skasowa�
 */
template<class T> class C_CircBuff
{
public:
	C_CircBuff();
	~C_CircBuff();
	/// inicjalizuje dynamicznie bufor
	void BuffInit(unsigned int _S);
	/// dodaje obiekt typu T do bufora na kolejn� pozycj�
	T *AddObject();
	/// zwraca odres obiektu na pozycji n
	T *GetObject(unsigned int _n);  
	/// zwraca true je�li bufor pe�ny
	bool Czy_pelny() const;
	/// kasuje ostatnio dodany obiekt
	void DelObject();
	/// zwraca ilo�� element�w w buforze
	int getNumElem();
private:
	/// tablica zawieraj�ca obiekty w buforze ko�owym
	T **buff;
	/// ostatni dodany obiekt - index - liniowy od 0 do N moze byc
	unsigned int last;
	/// rozmiar bufora 
	unsigned int S;
	/// ostatnio dodany index przy uwzgl�nieniu cyrkularno�ci
	int ostatni;
	/// ilosc zapis�w
	unsigned int ile;
	
};
/** 
 * Zwraca ilo�� zapisanych element�w w buforze.
 * \return Ilo�� element�w. Jesli bufor pe�ny to zwraca jego pojemno��, je�li nie pe�ny to ilo�� zapisanych element�w.
 */
template<class T>
int C_CircBuff<T>::getNumElem()
{
	if(NULL==buff )
		return -1;
	if( ostatni<0)
		return 0;
	if(Czy_pelny())
		return S;
	else
		return (unsigned int)(ostatni+1);	// rozmiar zwraca a nie index
}

template<class T>
void C_CircBuff<T>::DelObject()
{
	_ASSERT(buff!=NULL);	// bez inicjalizacji
	_ASSERT(ostatni>=0);	// nie ma nic dodane ale jest zainicjalizowane
	SAFE_DELETE(buff[ostatni]);	
	last = ostatni;
	ile--;
}

/** 
 * Ta funkcja musi by� wywo�ana jako pierwsza
 * \param S Rozmiar bufora
 */
template<class T>
void C_CircBuff<T>::BuffInit( unsigned int _S )
{
	if(0==_S)
		_RPT0(_CRT_ERROR,"void C_CircBuff<T>::BuffInit->Wrong size");
	S = _S;
	buff = new T *[S];
	for(unsigned int a=0;a<S;a++)
		buff[a] = NULL;
}

/** 
 * Funkcja zwraca element na pozycji n. Je�li n jest wi�ksze ood rozmiaru bufora to
 * zwracany jest element na pozycji po przekr�ceniu si� bufora
 * \warning Istnieje mozliwosc pobrania pozycji niezainicjalizowanej
 */
template<class T>
T * C_CircBuff<T>::GetObject( unsigned int _n )
{
	_ASSERT(buff!=NULL);
	return buff[_n%S];
}

/** 
 * \return Funkcja zwraca adres nowododanego obiektu
 */
template<class T>
T * C_CircBuff<T>::AddObject()
{
	_ASSERT(buff!=NULL);
	if(last>S-1) {last = 0;}
	SAFE_DELETE(buff[last]);	// kasownaie tego co tam jest na nowej pozycji
	buff[last] = new T;
	ostatni = (int)last;
	ile++;
	return GetObject(last++);
}
template<class T>
bool C_CircBuff<T>::Czy_pelny() const
{ 
	if(ile>=S)
		return true;	// wi�cej zapis�w niz miejsca
	else
		return false; 
}

template<class T>
C_CircBuff<T>::~C_CircBuff()
{
	for(unsigned int a=0;a<S;a++)
		SAFE_DELETE(buff[a]);
	SAFE_DELETE(buff);
}

template<class T>
C_CircBuff<T>::C_CircBuff()
{
	buff = NULL;
	last = 0;	// do ustawienia
	S = 0;
	ile = 0;
	ostatni = -1;
}

#endif // C_CircBuff_h__