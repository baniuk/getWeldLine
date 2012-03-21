/** \mainpage TrackWeldLine 
* 
* \section sec1 Introduction
* Procedura wyznaczajaca granice spaw�w prostych i zakrzywionych.

* \section sec2 Struktura
* Ka�da klasa jest w osobnym projekcie, parametry ustawione jak zawsze tylko �e komilacja do lib pez stdafx. 
* Wszystkie kody z tej solucji znajduja si� w katalogach source i include w katalogu solucji.Do ka�dego
* Takiego projektu jest projekt z testami kt�ry linkuje biblioteki googla, sie�ki ustawione w ustawieniach generalnych 
* oraz dodane odpowiednie liby w linkerze. W Common properties s� dodane projekty na kt�rych zale�y test oraz dodatkowo scie�ki
* do ich includ�w. W projektach test�w ustawione s� tak�e postbuilds.
* Linkowane s� tylko funkcji na poziomie danego includa, tzn ze na przyk��d jesli cmatrix uzywa czego� z wx lib to w bibliotece wynikowej cmatrix nie b�dzie tych dunkcji i w projekcie finalnym trzeba je do��czy�
* \section sec3 Katalogi w VS
* \li Include $(GTEST)\include;$(WX_ROOT)\include;$(WX_ROOT)\include\msvc;$(LEVMAR_ROOT);$(LAPACK_ROOT)\include;$(WX_MATHPLOT_ROOT);$(SolutionDir)SDK;$(SolutionDir)INCLUDE;
* \li LIB $(GTEST)\msvc\gtest-md\release;$(WX_ROOT)\lib\vc_lib\msw;$(WX_ROOT)\lib\vc_lib;$(LEVMAR_ROOT);$(LAPACK_ROOT)\lib;$(SolutionDir)SDK;
* \section sec4 Testy
* Testy s� wykonane za pomoc� biblioteki GLib. Nie ma jej dodanej w ISARInstaller, wiec nale�y j� zainstalowac samodzielnie. Kompilacja na podstawie projektu VS do��czonego do �rodowiska. Po kompilacji nalezy doda� zmienna GTEST do katalogu c:\gtest-1.6.0. Je�li u�ywamy opcji kompilatora md to trzeba kompilowa� odpowiednie biblioteki z ko�c�wk� md\n
* Je�li kompilujemy do test�w release to trzeba w projekcie SDK ustawi� _TEST bo inaczej nie zostanie skompilownay konstruktor. Tych test�w nie mo�na kompilowa� w czystym release poniewa� dla tego przypadku nie jest inicjalizowany konstruktor i inne metody (.AddEntry) nie dzia�aj�. Trzeba by ka�d� zamyka� w #ifdef...


 */