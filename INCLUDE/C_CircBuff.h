/**
 * \file    C_CircBuff.h
 * \brief	Bufor ko³owy
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_CircBuff_h__
#define C_CircBuff_h__

/** 
 * Klasa obs³uguj¹ca bufor ko³owy. Umo¿liwia realziacjê bufora ko³owego ale przy indeksowaniu liniowym. Dane s¹ dopisuwane na kolejne 
 * miejsce zraz za oststnio dopisanym. T jest typem obiektu a S rozmiarem bufora. Buform mo¿na odczytaæ dopiero gdy jest pe³ny.
 */
template<class T> class C_CircBuff
{
public:
	C_CircBuff();
	~C_CircBuff();
	/// inicjalizuje dynamicznie bufor
	void BuffInit(unsigned int _S);
	/// dodaje obiekt typu T do bufora na kolejn¹ pozycjê
	T *AddObject();
	/// zwraca odres obiektu na pozycji n
	T *GetObject(unsigned int _n);  
	/// zwraca true jeœli bufor pe³ny
	bool Czy_pelny() const { return czy_pelny; }
	/// kasuje ostatnio dodany obiekt
	void DelObject();
private:
	/// tablica zawieraj¹ca obiekty w buforze ko³owym
	T **buff;
	/// ostatni dodany obiekt - index - liniowy od 0 do N moze byc
	unsigned int last;
	/// rozmiar bufora 
	unsigned int S;
	/// czy bufor pe³ny
	bool czy_pelny;
	/// ostatnio dodany index przy uwzglênieniu cyrkularnoœci
	unsigned int ostatni;
	
};

template<class T>
void C_CircBuff<T>::DelObject()
{
	SAFE_DELETE(buff[ostatni]);	
	last = ostatni;
}

/** 
 * Ta funkcja musi byæ wywo³ana jako pierwsza
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
 * Funkcja zwraca element na pozycji n. Jeœli n jest wiêksze ood rozmiaru bufora to
 * zwracany jest element na pozycji po przekrêceniu siê bufora
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
	ostatni = last;
	if(last==S-1)
		czy_pelny = true;
	return GetObject(last++);
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
	czy_pelny = false;
	S = 0;
}

#endif // C_CircBuff_h__