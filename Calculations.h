#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <math.h>
#include <limits>

#include <boost\multiprecision\cpp_int.hpp>

//zbyt ma�y wyk�adnik mo�e spowodowa� �e jaka� liczba nie zostanie znaleziona
//zbyt du�y wyk�adnik mo�e spowodowa� �e zostanie znaleziona b��dna liczba
#define MAKSYMALNY_WYKLADNIK 180

//funkcja zwraca warto�� pobran� od u�ytkownika; je�eli wprowadzono b��dne dane zapytanie jest powtarzane
//funkcja w 3 wariantach: pobranie liczby z przedzia�u domkni�tego min-max,  z ograniczeniem dolnym,  bez ogranicze� 
unsigned int pobierz_unsigned() {
	unsigned long long liczba = 0;
	//zmienna potrzebna do sprawdzenia czy podana przez u�ytkownika liczba nie jest ujemna
	unsigned int najwiekszy_int = static_cast<unsigned int>(-1);
	do {
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "niepoprawna wartosc \n";
		}
		std::cin >> liczba;
	} while (std::cin.fail() || liczba > najwiekszy_int);
	return static_cast<unsigned int>(liczba);
}

unsigned int pobierz_unsigned(unsigned int min) {
	unsigned long long liczba = 0;
	unsigned int najwiekszy_int = static_cast<unsigned int>(-1);
	do {
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "niepoprawna wartosc \n";
		}
		std::cin >> static_cast<unsigned long long>(liczba);
	} while (liczba < min || std::cin.fail() || liczba > najwiekszy_int);
	return static_cast<unsigned int>(liczba);;
}

unsigned int pobierz_unsigned(unsigned int min, unsigned int max) {
	unsigned long long liczba = 0;
	do {
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "niepoprawna wartosc \n";
		}
		std::cin >> static_cast<unsigned long long>(liczba);
	} while (liczba < min || liczba > max || std::cin.fail());
	return static_cast<unsigned int>(liczba);
}
//pobranie liczby z ograniczeniem g�rnym
unsigned int pobierz_unsigned_max(unsigned int max) {
	unsigned long long liczba = 0;
	do {
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "niepoprawna wartosc \n";
		}
		std::cin >> static_cast<unsigned long long>(liczba);
	} while (liczba > max || std::cin.fail());
	return static_cast<unsigned int>(liczba);
}

//funkcja wyznaczaj�ca najwi�kszy wsp�lny dzielnik
unsigned int NWD(unsigned int n, int i) {
	while (i) {
		unsigned int temp = n % i;
		n = i;
		i = temp;
	}
	return n;
}

//funkcja wype�niaj�ca tablice resztami modulo n, kt�re s� wzgl�dnie pierwsze z n
void WzgledniePierwsze(unsigned int rWzgledniePierwsze[], int & iloscWzglednie, const unsigned int & n) {
	for (unsigned int i = 2; i < n; i++) {
		if (NWD(n, i) == 1) {
			rWzgledniePierwsze[iloscWzglednie] = i;
			iloscWzglednie++;
		}
	}
}

//cz�� funkcji "PierwiastekPierwotny"
void zerowanie(unsigned int & n, int & iloscWzglednie, unsigned int & x, unsigned int* & rWzgledniePierwsze, unsigned int* & rWzgledniePierwszeKopia) {
	if (rWzgledniePierwszeKopia != nullptr)
		delete[] rWzgledniePierwszeKopia;
	rWzgledniePierwszeKopia = new unsigned int[iloscWzglednie];
	for (int i = 0; i < iloscWzglednie; i++) {
		rWzgledniePierwszeKopia[i] = rWzgledniePierwsze[i];
	}
	//p�tla sprawdzaj�ca dla danego x'sa czy pierwiastki wszystkich liczby wzgl�dnie pierwszych z n s� mu r�wne
	for (int i = 0; i < iloscWzglednie; i++) {
		boost::multiprecision::int1024_t wynikPotegi;
		int wykladnik = 1;
		do {
		//	w tym miejscu nie mo�na u�y� funkcji pow(), poniewa� zwraca liczby ze zbioru o zbyt ma�ym zakresie (kolejne pot�gi przekraczaj� zakres)
		//	wynikPotegi = static_cast<boost::multiprecision::int1024_t>(pow(x, wykladnik));
			wynikPotegi = x;
			for (int j = 1; j < wykladnik; j++)
				wynikPotegi *= x;
			unsigned int resztaModulo = static_cast<unsigned int>(wynikPotegi % n);
			if (resztaModulo == rWzgledniePierwsze[i]) {
				//je�eli dane reszty modulo n s� sobie r�wne to zostaje to zapami�tane jako wyzerowanie odpowiedniego elementu kopii tablicy 
				rWzgledniePierwszeKopia[i] = 0;
				break;
			}
			wykladnik++;
		} while (wykladnik < MAKSYMALNY_WYKLADNIK);
	}
}

//cz�� funkcji "PierwiastekPierwotny"
bool czy_wyzerowana(int & iloscWzglednie, unsigned int* & rWzgledniePierwsze, unsigned int* & rWzgledniePierwszeKopia) {
	//sprawdzenie czy ca�a tablica zosta�a wype�niona zerami
	bool wyzerowana = true;
	for (int i = 0; i < iloscWzglednie; i++) {
		if (rWzgledniePierwszeKopia[i] != 0) {
			wyzerowana = false;
			break;
		}
	}
	if (wyzerowana) {
		delete[] rWzgledniePierwszeKopia;
		rWzgledniePierwszeKopia = nullptr;
		delete[] rWzgledniePierwsze;
		rWzgledniePierwsze = nullptr;
		return 1;
	}
	else
		return 0;
}

//funkcja zwracaj�ca najmniejszy pierwiastek pierwotny n. Je�eli taki nie istnieje to zwraca 0
unsigned int PierwiastekPierwotny(unsigned int n) {
	//tablica, kt�ra zostanie wype�niona liczbami wzgl�dnie pierwszymi z n
	unsigned int* rWzgledniePierwsze = new unsigned int[n];
	//tablica, kt�ra jest kopi� cze�ci tablicy "rWzgledniePierwsze"; jest odtwarzana przed sprawdzeniem ka�dego nast�pnego x'sa (patrz ni�ej)
	unsigned int* rWzgledniePierwszeKopia = nullptr;
	//ilo�� liczb wzgl�dnie pierwszych z n
	int iloscWzglednie = 0;
	WzgledniePierwsze(rWzgledniePierwsze, iloscWzglednie, n);
	//warto�� x to aktualnie sprawdzana liczba pod wzgl�dem bycia pierwiastkiem pierwotnym 
	unsigned int x = 2;
	//p�tla sprawdzaj�ca wszystkie x'sy
	while (x < n){
		zerowanie(n, iloscWzglednie, x, rWzgledniePierwsze, rWzgledniePierwszeKopia);
		if (czy_wyzerowana(iloscWzglednie, rWzgledniePierwsze, rWzgledniePierwszeKopia))
			return x;
		x++;
	}
	if (rWzgledniePierwszeKopia != nullptr)
		delete[] rWzgledniePierwszeKopia;
	rWzgledniePierwszeKopia = nullptr;
	delete[] rWzgledniePierwsze;
	rWzgledniePierwsze = nullptr;
	return 0;
}

#endif 
