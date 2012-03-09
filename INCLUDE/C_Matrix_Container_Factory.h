/**
 * \file    C_Matrix_Container_Factory.h
 * \brief	Obs³uguje automatyczne ³adowania ci¹gów danych z matlaba
 * \author  PB
 * \date    2012/03/01
 */
#ifndef C_Matrix_Container_Factory_h__
#define C_Matrix_Container_Factory_h__

#include "C_Error.h"
#include "C_Matrix_Container.h"

/// d³ugoœæ nazwy pliku
#define MAX_FILE_NAME 1024
/// maksymalna d³ugoœæ rozszerzenia
#define MAX_EXT_NAME 10
/** Obs³uguje automatyczne ³adowania ci¹gów danych z matlaba. Klasa tworzy zbiór obiektów klasy C_Matrix_Cntainer i ³aduje do nich pliki o dowolnej nazwie z koñcówk¹ _xx, gdzie xx jest numerem. Przydatne gdy 
 * z matlaba mamy wiele plików z danymi dotycz¹cymi tego samego przypadku
 * i chcemy je ³atwo za³adowaæ.
 */ 
class C_Matrix_Container_Factory
{
	friend class C_Matrix_Container_FactoryTest;
public:
	C_Matrix_Container_Factory(const char* const _baza, int _ile);
	~C_Matrix_Container_Factory();
	/// zwraca adres do wybranego pojemnika z danymi
	const C_Matrix_Container& getContainer(int num); 
	/// wczytuje za³o¿ony zbiór
	void LoadData();
protected:
	
private:
	/// tablica zawieraj¹ca wskaŸniki do obiektów C_Matrix_Container
	C_Matrix_Container **tab;
	/// nazwa bazowa zbioru danych. Kolejne pliki maj¹ format baza_numer
	char baza[MAX_FILE_NAME];
	/// ile tablic mamy
	int ile;
	/// odczytuje przypadki i ³aduje je
	bool ReadFiles();
	/// Przygotowuje kolejne nazwy plików
	void CreateName(int n, char* out);
	/// czy by³o loaddata?
	bool czy_initialized;

};

#endif // C_Matrix_Container_Factory_h__