/** \mainpage TrackWeldLine 
* 
* \section sec1 Introduction
* Procedura wyznaczajaca granice spawów prostych i zakrzywionych.

* \section sec2 Struktura
* Ka¿da klasa jest w osobnym projekcie, parametry ustawione jak zawsze tylko ¿e komilacja do lib pez stdafx. 
* Wszystkie kody z tej solucji znajduja siê w katalogach source i include w katalogu solucji.Do ka¿dego
* Takiego projektu jest projekt z testami który linkuje biblioteki googla, sie¿ki ustawione w ustawieniach generalnych 
* oraz dodane odpowiednie liby w linkerze. W Common properties s¹ dodane projekty na których zale¿y test oraz dodatkowo scie¿ki
* do ich includów. W projektach testów ustawione s¹ tak¿e postbuilds.
* Linkowane s¹ tylko funkcji na poziomie danego includa, tzn ze na przyk³¹d jesli cmatrix uzywa czegoœ z wx lib to w bibliotece wynikowej cmatrix nie bêdzie tych dunkcji i w projekcie finalnym trzeba je do³¹czyæ
* \section sec3 Katalogi w VS
* \li Include $(GTEST)\include;$(WX_ROOT)\include;$(WX_ROOT)\include\msvc;$(LEVMAR_ROOT);$(LAPACK_ROOT)\include;$(WX_MATHPLOT_ROOT);$(SolutionDir)SDK;$(SolutionDir)INCLUDE;
* \li LIB $(GTEST)\msvc\gtest-md\release;$(WX_ROOT)\lib\vc_lib\msw;$(WX_ROOT)\lib\vc_lib;$(LEVMAR_ROOT);$(LAPACK_ROOT)\lib;$(SolutionDir)SDK;
* \section sec4 Testy
* Testy s¹ wykonane za pomoc¹ biblioteki GLib. Nie ma jej dodanej w ISARInstaller, wiec nale¿y j¹ zainstalowac samodzielnie. Kompilacja na podstawie projektu VS do³¹czonego do œrodowiska. Po kompilacji nalezy dodaæ zmienna GTEST do katalogu c:\gtest-1.6.0. Jeœli u¿ywamy opcji kompilatora md to trzeba kompilowaæ odpowiednie biblioteki z koñcówk¹ md\n
* Jeœli kompilujemy do testów release to trzeba w projekcie SDK ustawiæ _TEST bo inaczej nie zostanie skompilownay konstruktor. Tych testów nie mo¿na kompilowaæ w czystym release poniewa¿ dla tego przypadku nie jest inicjalizowany konstruktor i inne metody (.AddEntry) nie dzia³aj¹. Trzeba by ka¿d¹ zamykaæ w #ifdef...


 */