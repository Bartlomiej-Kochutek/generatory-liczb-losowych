#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <math.h>
#include <limits>

#include <boost\multiprecision\cpp_int.hpp>

//zbyt ma³y wyk³adnik mo¿e spowodowaæ ¿e jakaœ liczba nie zostanie znaleziona
//zbyt du¿y wyk³adnik mo¿e spowodowaæ ¿e zostanie znaleziona b³êdna liczba
#define MAKSYMALNY_WYKLADNIK 180

//funkcja zwraca wartoœæ pobran¹ od u¿ytkownika; je¿eli wprowadzono b³êdne dane zapytanie jest powtarzane
//funkcja w 3 wariantach: pobranie liczby z przedzia³u domkniêtego min-max,  z ograniczeniem dolnym,  bez ograniczeñ 
unsigned int pobierz_unsigned() {
	unsigned long long liczba = 0;
	//zmienna potrzebna do sprawdzenia czy podana przez u¿ytkownika liczba nie jest ujemna
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
//pobranie liczby z ograniczeniem górnym
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

//funkcja wyznaczaj¹ca najwiêkszy wspólny dzielnik
unsigned int NWD(unsigned int n, int i) {
	while (i) {
		unsigned int temp = n % i;
		n = i;
		i = temp;
	}
	return n;
}

//funkcja wype³niaj¹ca tablice resztami modulo n, które s¹ wzglêdnie pierwsze z n
void WzgledniePierwsze(unsigned int rWzgledniePierwsze[], int & iloscWzglednie, const unsigned int & n) {
	for (unsigned int i = 2; i < n; i++) {
		if (NWD(n, i) == 1) {
			rWzgledniePierwsze[iloscWzglednie] = i;
			iloscWzglednie++;
		}
	}
}

//czêœæ funkcji "PierwiastekPierwotny"
void zerowanie(unsigned int & n, int & iloscWzglednie, unsigned int & x, unsigned int* & rWzgledniePierwsze, unsigned int* & rWzgledniePierwszeKopia) {
	if (rWzgledniePierwszeKopia != nullptr)
		delete[] rWzgledniePierwszeKopia;
	rWzgledniePierwszeKopia = new unsigned int[iloscWzglednie];
	for (int i = 0; i < iloscWzglednie; i++) {
		rWzgledniePierwszeKopia[i] = rWzgledniePierwsze[i];
	}
	//pêtla sprawdzaj¹ca dla danego x'sa czy pierwiastki wszystkich liczby wzglêdnie pierwszych z n s¹ mu równe
	for (int i = 0; i < iloscWzglednie; i++) {
		boost::multiprecision::int1024_t wynikPotegi;
		int wykladnik = 1;
		do {
		//	w tym miejscu nie mo¿na u¿yæ funkcji pow(), poniewa¿ zwraca liczby ze zbioru o zbyt ma³ym zakresie (kolejne potêgi przekraczaj¹ zakres)
		//	wynikPotegi = static_cast<boost::multiprecision::int1024_t>(pow(x, wykladnik));
			wynikPotegi = x;
			for (int j = 1; j < wykladnik; j++)
				wynikPotegi *= x;
			unsigned int resztaModulo = static_cast<unsigned int>(wynikPotegi % n);
			if (resztaModulo == rWzgledniePierwsze[i]) {
				//je¿eli dane reszty modulo n s¹ sobie równe to zostaje to zapamiêtane jako wyzerowanie odpowiedniego elementu kopii tablicy 
				rWzgledniePierwszeKopia[i] = 0;
				break;
			}
			wykladnik++;
		} while (wykladnik < MAKSYMALNY_WYKLADNIK);
	}
}

//czêœæ funkcji "PierwiastekPierwotny"
bool czy_wyzerowana(int & iloscWzglednie, unsigned int* & rWzgledniePierwsze, unsigned int* & rWzgledniePierwszeKopia) {
	//sprawdzenie czy ca³a tablica zosta³a wype³niona zerami
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

//funkcja zwracaj¹ca najmniejszy pierwiastek pierwotny n. Je¿eli taki nie istnieje to zwraca 0
unsigned int PierwiastekPierwotny(unsigned int n) {
	//tablica, która zostanie wype³niona liczbami wzglêdnie pierwszymi z n
	unsigned int* rWzgledniePierwsze = new unsigned int[n];
	//tablica, która jest kopi¹ czeœci tablicy "rWzgledniePierwsze"; jest odtwarzana przed sprawdzeniem ka¿dego nastêpnego x'sa (patrz ni¿ej)
	unsigned int* rWzgledniePierwszeKopia = nullptr;
	//iloœæ liczb wzglêdnie pierwszych z n
	int iloscWzglednie = 0;
	WzgledniePierwsze(rWzgledniePierwsze, iloscWzglednie, n);
	//wartoœæ x to aktualnie sprawdzana liczba pod wzglêdem bycia pierwiastkiem pierwotnym 
	unsigned int x = 2;
	//pêtla sprawdzaj¹ca wszystkie x'sy
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
