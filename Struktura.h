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

 */