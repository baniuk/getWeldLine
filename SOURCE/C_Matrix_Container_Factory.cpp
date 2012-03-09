#include "C_Matrix_Container_Factory.h"

/** 
 * G³ówny konstruktor - przyjmuje iloœæ plików do wczytania oraz ich nazwê bazow¹. Po konstrukcji nale¿y wywo³aæ loaddata
 * \param[in] _baza nazwa bazowa
 * \param[in] _ile liczba plików
 * \warning Pliki s¹ numerowane od 0
 */
C_Matrix_Container_Factory::C_Matrix_Container_Factory(const char* const _baza, int _ile)
{
	tab = new C_Matrix_Container *[_ile];
	for(int a=0;a<_ile;a++)
		tab[a] = new C_Matrix_Container;
	ile = _ile;
	strcpy_s(baza,MAX_FILE_NAME,_baza);
	czy_initialized = false;
}

C_Matrix_Container_Factory::~C_Matrix_Container_Factory()
{
	for(int a=0;a<ile;a++)
		SAFE_DELETE(tab[a]);
	SAFE_DELETE(tab);
}

/** 
 * Dodaje do nazwy bazowej numer kolejny.
 * \param[in] n numer do dodania
 * \param[out] out tu ³aduje gotowy ci¹g
 */
void C_Matrix_Container_Factory::CreateName( int n, char* out )
{
	char copy_baza[MAX_FILE_NAME];
	char bez_roz[MAX_FILE_NAME];
	char rozszerzenie[MAX_EXT_NAME];
	char liczba[5];
	char *pch;
	int pozkropki;
	bool czyroz;
	strcpy_s(copy_baza,MAX_FILE_NAME,baza);
	pch = strrchr(copy_baza,'.');
	if(NULL==pch) {
		strcpy_s(bez_roz,MAX_FILE_NAME,copy_baza);
		czyroz = false;// nie ma roz
	}
	else
	{
		pozkropki = pch-copy_baza+1;
		strncpy_s(bez_roz,MAX_FILE_NAME,copy_baza,pozkropki-1);
		bez_roz[pozkropki-1]='\0';
		strcpy_s(rozszerzenie,MAX_EXT_NAME,copy_baza+pozkropki-1);
		czyroz = true;
	}
	strcpy_s(out,MAX_FILE_NAME,bez_roz);
	strcat_s(out,MAX_FILE_NAME,"_");
	_itoa_s(n,liczba,5,10);
	strcat_s(out,MAX_FILE_NAME,liczba);
	if(czyroz)
		strcat_s(out,MAX_FILE_NAME,rozszerzenie);
}

void C_Matrix_Container_Factory::LoadData()
{
	int a;
	char tmp_name[MAX_FILE_NAME];
	for(a=0;a<ile;a++)
	{
		CreateName(a,tmp_name);
		tab[a]->ReadBinary(tmp_name);
	}
	czy_initialized = true;
}

const C_Matrix_Container& C_Matrix_Container_Factory::getContainer( int num )
{
	_ASSERTE(num<ile);
	_ASSERTE(czy_initialized);
	return *tab[num];

}
