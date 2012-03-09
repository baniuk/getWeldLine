/**
 * \file    C_Matrix_Container_Factory.h
 * \brief	Obs�uguje automatyczne �adowania ci�g�w danych z matlaba
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_Matrix_Container_Factory_h__
#define C_Matrix_Container_Factory_h__

#include "C_Error.h"
#include "C_Matrix_Container.h"

/// d�ugo�� nazwy pliku
#define MAX_FILE_NAME 1024
/// maksymalna d�ugo�� rozszerzenia
#define MAX_EXT_NAME 10
/** Obs�uguje automatyczne �adowania ci�g�w danych z matlaba. Klasa tworzy zbi�r obiekt�w klasy C_Matrix_Cntainer i �aduje do nich pliki o dowolnej nazwie z ko�c�wk� _xx, gdzie xx jest numerem. Przydatne gdy 
 * z matlaba mamy wiele plik�w z danymi dotycz�cymi tego samego przypadku
 * i chcemy je �atwo za�adowa�.
 */ 
class C_Matrix_Container_Factory
{
	friend class C_Matrix_Container_FactoryTest;
public:
	C_Matrix_Container_Factory(const char* const _baza, int _ile);
	~C_Matrix_Container_Factory();
	/// zwraca adres do wybranego pojemnika z danymi
	const C_Matrix_Container& getContainer(int num); 
	/// wczytuje za�o�ony zbi�r
	void LoadData();
protected:
	
private:
	/// tablica zawieraj�ca wska�niki do obiekt�w C_Matrix_Container
	C_Matrix_Container **tab;
	/// nazwa bazowa zbioru danych. Kolejne pliki maj� format baza_numer
	char baza[MAX_FILE_NAME];
	/// ile tablic mamy
	int ile;
	/// odczytuje przypadki i �aduje je
	bool ReadFiles();
	/// Przygotowuje kolejne nazwy plik�w
	void CreateName(int n, char* out);
	/// czy by�o loaddata?
	bool czy_initialized;

};

#endif // C_Matrix_Container_Factory_h__