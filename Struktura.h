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

 */