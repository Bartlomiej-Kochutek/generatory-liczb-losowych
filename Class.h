#ifndef CLASS_H
#define CLASS_H

#include <fstream>

#include "Calculations.h"
#include "_Sfml.h"

//klasa zarz¹dzaj¹ca list¹ wylosowanych liczb
class Lista_wylosowanych {
	//wskaŸniki na pierwszy i ostatni element
	struct Wylosowana* pierwsza;
	struct Wylosowana* ostatnia;
	//okreœla ile elementów znajduje siê w liœcie
	int wylosowano;
public:
	Lista_wylosowanych() : pierwsza(nullptr), ostatnia(nullptr), wylosowano(0) {}
	//funkcja zwracaj¹ca wartoœæ wskaŸnika na pierwszy element listy
	struct Wylosowana* _pierwsza() {
		return pierwsza;
	}
	//funkcja informuje ile elementów znajduje siê w liœcie
	int _wylosowano() {
		return wylosowano;
	}
	//funkcja dodaj¹ca do listy jeden element (wylosowan¹ liczbê)
	void dodaj_liczbe(unsigned int wartosc);
	//funkcja wypisuj¹ca wylosowane liczby
	void wypisz();
	//funkcja usuwaj¹ca ca³¹ listê
	void usun_wszystkie();

	Lista_wylosowanych & operator= (const Lista_wylosowanych lista);

	~Lista_wylosowanych(){
		usun_wszystkie();
	}
};

void Lista_wylosowanych::dodaj_liczbe(unsigned int wartosc) {
	if (pierwsza == nullptr) {
		ostatnia = new struct Wylosowana;
		pierwsza = ostatnia;
		ostatnia->wartosc = wartosc;
		ostatnia->nast = nullptr;
	}
	else {
		ostatnia->nast = new struct Wylosowana;
		ostatnia = ostatnia->nast;
		ostatnia->nast = nullptr;
		ostatnia->wartosc = wartosc;
	}
	this->wylosowano++;
}

void Lista_wylosowanych::wypisz() {
	int x = 1;
	struct Wylosowana* temp = pierwsza;
	while (temp != nullptr) {
		std::cout << x << "\t" << temp->wartosc << "\n";
		temp = temp->nast;
		x++;
	}
}

void Lista_wylosowanych::usun_wszystkie() {
	while (pierwsza != nullptr) {
		struct Wylosowana* temp = pierwsza->nast;
		delete pierwsza;
		pierwsza = temp;
	}
	ostatnia = nullptr;
	this->wylosowano = 0;
}

Lista_wylosowanych & Lista_wylosowanych:: operator= (const Lista_wylosowanych lista) {
	if (this == &lista)
		return *this;
	this->wylosowano = lista.wylosowano;
	struct Wylosowana* temp = lista.pierwsza;
	if (temp == nullptr) {
		this->pierwsza = this->ostatnia = nullptr;
	}
	else{
		this->pierwsza = new struct Wylosowana;
		pierwsza->wartosc = temp->wartosc;
		this->ostatnia = pierwsza;
		temp = temp->nast;
		if (temp == nullptr) {
			this->ostatnia = pierwsza;
			pierwsza->nast = nullptr;
		}
		else{
			while (temp != nullptr) {
				ostatnia->nast = new struct Wylosowana;
				ostatnia = ostatnia->nast;
				ostatnia->nast = nullptr;
				ostatnia->wartosc = temp->wartosc;
				temp = temp->nast;
			}
		}
	}
	return *this;
}

//bazowa klasa generatorów liczb losowych
class Generator {
protected:
	//przedzia³ domkniêty, niepoprawne wartoœci podawane przez u¿ytkownika bêd¹ konwertowanego do nieujemnych liczb ca³kowitych
	unsigned int zakres_max, zakres_min, zakres;
	//przedzia³ okreœlaj¹cy ekstrmalne wartoœci min i max jakie zosta³ podane w ci¹gu ca³ego dzia³ania generatora (poniewa¿ mo¿na zmieniaæ zakres)
	//inicjalizowane skrajnymi wartoœciami aby zapewniæ wpisanie ich poprawnych wartoœci przy pierwszym podawaniu zakresu
	unsigned int calkowity_zakres_max = 0, calkowity_zakres_min = 4294967295, calkowity_zakres;
	unsigned int ilosc_do_wylosowania;
	Lista_wylosowanych* lista_wylosowanych;
	Rozklad rozklad;
	Wykres wykres;
	std::string nazwa;

	//funkcje ustawiaj¹ce/pobieraj¹ce wartoœci sk³adowych prywatnych 
	void ustaw_max(unsigned int x) {
		this->zakres_max = x;
	}
	unsigned int _zakres_max() {
		return zakres_max;
	}
	void ustaw_min(unsigned int x) {
		this->zakres_min = x;
	}
	unsigned int _zakres_min() {
		return zakres_min;
	}
	void ustaw_zakres() {
		zakres = zakres_max - zakres_min;
	}
	unsigned int _zakres() {
		return zakres;
	}
	void ustaw_ilosc_do_wylosowania(unsigned int x) {
		this->ilosc_do_wylosowania = x;
	}
	unsigned int _ilosc_do_wylosowania() {
		return ilosc_do_wylosowania;
	}
	//funkcja zwracaj¹ca losow¹ wartoœæ
	virtual unsigned int generuj() = 0;
public:
	//funkcja wypisuj¹ca wylosowane liczby
	void wypisz() {
		lista_wylosowanych->wypisz();
	}
	//funkcja usuwaj¹ca wylosowane liczby
	void usun_wylosowane() {
		lista_wylosowanych->usun_wszystkie();
	}
	//funkcja wyœwietlaj¹ca w nowym oknie rozk³ad wylosowanych liczb
	void rysuj_rozklad() {
		rozklad.ustaw_wartosci(calkowity_zakres, calkowity_zakres_min, calkowity_zakres_max, lista_wylosowanych->_pierwsza(), nazwa);
		rozklad.rysuj_rozklad();
	}
	//funkcja wyœwietlaj¹ca w nowym oknie wykres z wylosowanych liczb
	void rysuj_wykres() {
		wykres.ustaw_wartosci(calkowity_zakres, calkowity_zakres_min, calkowity_zakres_max, lista_wylosowanych->_pierwsza(), nazwa);
		wykres.rysuj_wykres();
	}
	//funkcja pobieraj¹ca od u¿ytkownika podstawowe parametry losowania
	void pobierz_parametry();
	//funkcja uruchamiaj¹ca wszystkie funkcje pobieraj¹ce od u¿ytkownika parametry losowania
	virtual void pobierz_wszystkie_parametry() = 0;
	int zapisz_do_pliku();
	int wczytaj_z_pliku();
	//funkcja generuj¹ca liczby losowe (tworzy listê liczb losowych lub dodaje liczyb do ju¿ istniej¹cej)
	virtual void losuj() = 0;

	//operator tablicowy zwraca odpowiedni element z listy wylosowanych liczb
	void operator[](const int index);
	//funkcja korzystaj¹ca z operatora tablicowego; wyœwietla jeden element listy wylosowanych liczb
	void wyswietl_jeden() {
		unsigned int index;
		std::cout << "\npodaj numer \n";
		index = pobierz_unsigned();
		index--;
		this->operator[](index);
	}

	//operator wywo³ania funkcji zmienia zakres losowania liczb
	unsigned int operator ()(unsigned int _zakres_min, unsigned int _zakres_max) {
		if (calkowity_zakres_max < _zakres_max)
			this->calkowity_zakres_max = _zakres_max;
		ustaw_max(_zakres_max);
		if (calkowity_zakres_min > _zakres_min)
			this->calkowity_zakres_min = _zakres_min;
		ustaw_min(_zakres_min);
		this->calkowity_zakres = calkowity_zakres_max - calkowity_zakres_min;
		this->zakres = zakres_max - zakres_min;
		return _zakres();
	}

	//operator, którego wywo³anie skutkuje wylosowaniem jednej liczby
	void operator++(int) {
		int ilosc = _ilosc_do_wylosowania();
		ustaw_ilosc_do_wylosowania(1);
		losuj();
		ustaw_ilosc_do_wylosowania(ilosc);
	}

	Generator() {
		lista_wylosowanych = new Lista_wylosowanych;
	}

	virtual ~Generator() {
		delete lista_wylosowanych;
	}
};

//definicja strumieniowego operatora wypisania dla klasy "Generator"
std::ostream & operator<< (std::ostream & ostream, Generator & generator) {
	generator.wypisz();
	return ostream << "\n";
}

void Generator::pobierz_parametry() {
	unsigned int x;
	//zmienna okreœla czy podany przez u¿ytkownika zakres jest poprawny
	bool nie_poprawny_zakres;
	do {
		std::cout << "\npodaj zakres liczb do wylosowania\nnajmniejsza wartosc: min = ";
		x = pobierz_unsigned();
		if (calkowity_zakres_min > x)
			this->calkowity_zakres_min = x;

		ustaw_min(x);
		std::cout << "\nnajwieksza wartosc: max = ";
		x = pobierz_unsigned(x);

		if (calkowity_zakres_max < x) 
			this->calkowity_zakres_max = x;

		ustaw_max(x);
		if ((_zakres_max() - _zakres_min()) < 0)
			nie_poprawny_zakres = true;
		else
			nie_poprawny_zakres = false;
	} while (nie_poprawny_zakres);
	ustaw_zakres();
	this->calkowity_zakres = calkowity_zakres_max - calkowity_zakres_min;
	std::cout << "\npodaj ilosc liczb do wygenerowania w trakcie pojedynczego losowania (mozna losowac kilka razy)\nilosc = ";
	x = pobierz_unsigned(1);
	ustaw_ilosc_do_wylosowania(x);
}

int Generator::zapisz_do_pliku() {
	std::ofstream plik_o;
	std::cout << "\npodaj pelna nazwe pliku (bez rozszerzenia) \nnazwa pliku: ";
	std::string _nazwa;
	std::cin >> _nazwa;
	_nazwa.append(".CSV");
	plik_o.open(_nazwa);
	if (!plik_o.is_open())
	{
		std::cout << "niepoprawna nazwa pliku\n";
		return 1;
	}
	struct Wylosowana* temp = lista_wylosowanych->_pierwsza();
	//zapisanie wszystkich wylosowanych liczb
	while (temp != nullptr) {
		plik_o << temp->wartosc;
		plik_o << ",";
		temp = temp->nast;
	}
	plik_o.close();
	return 0;
}

int Generator::wczytaj_z_pliku() {
	std::ifstream plik_i;
	std::cout << "\npodaj pelna nazwe pliku (bez rozszerzenia) \nnazwa pliku: ";
	std::string _nazwa;
	std::cin >> _nazwa;
	_nazwa.append(".CSV");
	plik_i.open(_nazwa);
	if (!plik_i.is_open()) {
		std::cout << "podany plik nie istnieje\n";
		return 1;
	}
	//wczytanie wszystkich zapisanych liczb
	unsigned int x;
	char c;
	while (1) {
		plik_i >> x;
		if (plik_i.eof())
			break;
		lista_wylosowanych->dodaj_liczbe(x);
		plik_i >> c;
	}
	plik_i.close();
	return 0;
}

void Generator::operator[](const int index) {
	if (index < 0 || index > lista_wylosowanych->_wylosowano()) {
		std::cout << "\nnie wylosowano liczby o podanym numerze(indeksie) \n";
	}
	else {
		struct Wylosowana* temp = lista_wylosowanych->_pierwsza();
		for (int i = 0; i < index; i++) {
			temp = temp->nast;
		}
		if(temp != nullptr)
			std::cout << index+1 << "\t" << temp->wartosc << "\n";
	}
}

//klasa zawieraj¹ca czêœæ wspóln¹ dwóch generatorów kongruencyjnych: kwadratowego i Park-Miller
class Kongruencyjny : public Generator {
protected:
	//wartoœæ ostatnio wylosowanej liczby lub ziarna
	unsigned int X;

	//funkcja ustawiaj¹ca wartoœæ X
	void ustaw_X(unsigned int x) {
		this->X = x;
	}
	//funkcja zwracaj¹ca wartoœæ X
	unsigned int _X() {
		return X;
	}
	//funkcja zwraca wartoœæ X+1 
	virtual unsigned int generuj() = 0;
public:
	//funkcja pobieraj¹ca od u¿ytkownika parametry losowania charakterystyczne dla danego generatora
	virtual void pobierz_szczegolowe_parametry() = 0;
	//funkcja uruchamiaj¹ca wszystkie funkcje pobieraj¹ce od u¿ytkownika parametry losowania
	virtual void pobierz_wszystkie_parametry() {
		pobierz_parametry();
		pobierz_szczegolowe_parametry();
	}
	//funkcja zawieraj¹ca zestaw parametrów 
	virtual void proponowane() = 0;
	//funkcja ustawiaj¹ca parametry podane przez u¿ytkownika
	virtual void wlasne() = 0;
	//funkcja generuj¹ca liczby losowe (tworzy listê liczb losowych)
	virtual void losuj() = 0;
	virtual ~Kongruencyjny() {};
};

//klasa odpowiedzialna z czêœæ programu zwi¹zan¹ z generatorem Park-Miller
class ParkMiller : public Kongruencyjny {
	//wspó³czynniki ze wzoru na nastêpn¹ liczbê losow¹
	unsigned int g, n;

	//funkcje ustawiaj¹ce/pobieraj¹ce wartoœci sk³adowych prywatnych 
	void ustaw_g(unsigned int x) {
		this->g = x;
	}
	unsigned int _g() {
		return g;
	}
	void ustaw_n(unsigned int x) {
		this->n = x;
	}
	unsigned int _n() {
		return n;
	}
	//funkcja zwraca wartoœæ X+1 
	virtual unsigned int generuj();
public:
	//funkcja pobieraj¹ca od u¿ytkownika parametry losowania charakterystyczne dla generatora Park-Miller
	virtual void pobierz_szczegolowe_parametry();
	//funkcja zawieraj¹ca zestaw parametrów 
	virtual void proponowane(); 
	//funkcja ustawiaj¹ca parametry podane przez u¿ytkownika
	virtual void wlasne();
	//funkcja generuj¹ca liczby losowe (tworzy listê liczb losowych)
	virtual void losuj();

	ParkMiller & operator = (const ParkMiller & generator);

	ParkMiller() {
		std::cout << "\nPark-Miller \nXi+1 = (Xi * g) mod n \n";
		pobierz_wszystkie_parametry();
		this->nazwa = "Park-Miller";
	}
};

void ParkMiller::proponowane() {
	unsigned int x;
	std::cout << "1 - n = 65 537  g = 75\n2 - n = 2 147 483 647  g = 16 807\n3 - n = 4 294 967 291  g = 279 470 273\n";
	std::cout << "podaj nr wybranego wspolczynnika\n";
	x = pobierz_unsigned(1, 3);
	switch (x){
	case 1:
		ustaw_n(65537);
		ustaw_g(75);
		break;
	case 2:
		ustaw_n(2147483647);
		ustaw_g(16807);
		break;
	case 3:
		ustaw_n(4294967291);
		ustaw_g(279470273);
		break;
	}
}

void ParkMiller::wlasne() {
	unsigned int x;
	std::cout << "\npodaj liczbe pierwsza\nn = ";
	x = pobierz_unsigned(5);
	ustaw_n(x);
	x = PierwiastekPierwotny(n);
	if (x == 0) {
		std::cout << "nie znaleziono pierwiastka pierwotnego, podaj liczbe\n";
		std::cout << "g = ";
		x = pobierz_unsigned(2, n - 1);
		ustaw_g(x);
	}
	else
		ustaw_g(x);
}

void ParkMiller::pobierz_szczegolowe_parametry() {
	unsigned int x;
	std::cout << "podaj ziarno \nX0 = ";
	x = pobierz_unsigned(1);
	ustaw_X(x);
	std::cout << "\nczy zaproponowac liczbe pierwsza i jej pierwiastek pierwotny ? (1-tak, 0-nie)\n";
	x = pobierz_unsigned(0, 1);
	if (x)
		proponowane();
	else
		wlasne();
};

unsigned int ParkMiller::generuj() {
	this->X = X * g % n;
	return X;
}
void ParkMiller::losuj() {
	for (unsigned int i = 0; i < ilosc_do_wylosowania; i++) {
		unsigned int wygenerowana;
		if (zakres == 4294967295)
			wygenerowana = generuj();
		else
			wygenerowana = (generuj() % (zakres + 1)) + zakres_min;
		lista_wylosowanych->dodaj_liczbe(wygenerowana);
	}
}

ParkMiller & ParkMiller::operator = (const ParkMiller & generator) {
	if (this == &generator)
		return *this;

	this->ilosc_do_wylosowania = generator.ilosc_do_wylosowania;
	this->lista_wylosowanych = generator.lista_wylosowanych;
	this->nazwa = generator.nazwa;
	this->rozklad = generator.rozklad;
	this->wykres = generator.wykres;
	this->zakres = generator.zakres;
	this->zakres_max = generator.zakres_max;
	this->zakres_min = generator.zakres_min;
	this->calkowity_zakres = generator.calkowity_zakres;
	this->calkowity_zakres_max = generator.calkowity_zakres_max;
	this->calkowity_zakres_min = generator.calkowity_zakres_min;

	this->g = generator.g;
	this->n = generator.n;

	return *this;
}

//klasa odpowiedzialna z czêœæ programu zwi¹zan¹ z generatorem kwadratowym
class Kwadratowy : public Kongruencyjny {
	//wspó³czynniki ze wzoru na nastêpn¹ liczbê losow¹
	unsigned int M, a, b, c;

	//funkcja zwraca wartoœæ X+1 
	virtual unsigned int generuj();
public:
	//funkcja pobieraj¹ca od u¿ytkownika parametry losowania charakterystyczne dla generatora kwadratowego
	virtual void pobierz_szczegolowe_parametry();
	//funkcja zawieraj¹ca zestaw parametrów M 
	virtual void proponowane();
	//funkcja ustawiaj¹ca parametr M na podstawie wartoœci podanej przez u¿ytkownika
	virtual void wlasne();
	//funkcja generuj¹ca liczby losowe (tworzy listê liczb losowych)
	virtual void losuj();

	Kwadratowy & operator = (const Kwadratowy & generator);

	Kwadratowy() {
		std::cout << "\nKwadratowy generator kongruencyjny \nX(i+1) = (a*(Xi)^2 + b*Xi + c) mod M \n";
		pobierz_wszystkie_parametry();
		this->nazwa = "kwadratowego kongruencyjnego";
	}
};

void Kwadratowy::pobierz_szczegolowe_parametry() {
	unsigned int x;
	std::cout << "podaj ziarno \nX0 = ";
	x = pobierz_unsigned();
	ustaw_X(x);
	std::cout << "czy zaproponowac wartosc wspolczynnika M ? (1-tak, 0-nie)\n";
	x = pobierz_unsigned(0, 1);
	if (x)
		proponowane();
	else
		wlasne();
	std::cout << "\npodaj pozostale wspolczynniki \na = ";
	this->a = pobierz_unsigned_max(M);
	std::cout << "b = ";
	this->b = pobierz_unsigned_max(M);
	std::cout << "c = ";
	this->c = pobierz_unsigned_max(M);
}

void Kwadratowy::proponowane() {
	unsigned int x;
	std::cout << "1 - 65 537 \n2 - 1 185 961 087\n3 - 2 147 483 647\n4 - 4 294 967 291\npodaj liczbe\n";
	x = pobierz_unsigned(1, 4);
	switch (x) {
	case 1:
		this->M = 65537;
		break;
	case 2:
		this->M = 1185961087;
		break;
	case 3:
		this->M = 2147483647;
		break;
	case 4:
		this->M = 4294967291;
		break;
	}
}

void Kwadratowy::wlasne() {
	unsigned int x;
	bool czy_pierwsza;
	do {
		std::cout << "\npodaj liczbe pierwsza wieksza od 5 \nM = ";
		x = pobierz_unsigned(5);
		this->M = x;
		//sprawdzenie czy podana przez u¿ytkownika liczba jest liczb¹ pierwsz¹
		czy_pierwsza = true;
		for (unsigned int i = 2; i < M; i++) {
			x = M % i;
			if (x == 0) {
				czy_pierwsza = false;
				break;
			}
		}
	} while (!czy_pierwsza);
}

unsigned int Kwadratowy::generuj() {
	this->X = (a*X*X + b*X + c) % M;
	return X;
}

void Kwadratowy::losuj() {
	for (unsigned int i = 0; i < ilosc_do_wylosowania; i++) {
		unsigned int wygenerowana;
		if (zakres == 4294967295)
			wygenerowana = generuj();
		else
			wygenerowana = (generuj() % (zakres + 1)) + zakres_min;
		lista_wylosowanych->dodaj_liczbe(wygenerowana);
	}
}

Kwadratowy & Kwadratowy::operator = (const Kwadratowy & generator) {
	if (this == &generator)
		return *this;

	this->ilosc_do_wylosowania = generator.ilosc_do_wylosowania;
	this->lista_wylosowanych = generator.lista_wylosowanych;
	this->nazwa = generator.nazwa;
	this->rozklad = generator.rozklad;
	this->wykres = generator.wykres;
	this->zakres = generator.zakres;
	this->zakres_max = generator.zakres_max;
	this->zakres_min = generator.zakres_min;
	this->calkowity_zakres = generator.calkowity_zakres;
	this->calkowity_zakres_max = generator.calkowity_zakres_max;
	this->calkowity_zakres_min = generator.calkowity_zakres_min;


	this->a = generator.a;
	this->b = generator.b;
	this->c = generator.c;
	this->M = generator.M;

	return *this;
}

//klasa odpowiedzialna z czêœæ programu zwi¹zan¹ z generatorem Mersenne Twister
class MersenneTwister : public Generator {
	int Rejestr_przesuwny[624];
	//indeks rejestru przesuwnego
	int indeks = 0;
	//sta³a dla MT19937
	const int F = 1812433253;
	//pozosta³e sta³e
	const int R = 31;
	const unsigned int dolna_maska = (1ull << R) - 1; //1ull to 1 traktowane jako unsigned long long
	const unsigned long long gorna_maska = 1ull << R;
	const unsigned int A = 0x9908B0DF;
	const int U = 11;
	const int S = 7;
	const unsigned B = 0x9D2C5680;
	const int T = 15;
	const unsigned C = 0xEFC60000;
	const int L = 18;

	//funkcja inicjalizuj¹ca rejestr przesuwny 
	void inicjalizuj(unsigned int ziarno) {
		Rejestr_przesuwny[0] = ziarno;
		for (int i = 1; i < 624; i++) {
			Rejestr_przesuwny[i] = F * (Rejestr_przesuwny[i - 1] ^ (Rejestr_przesuwny[i - 1] >> 30)) + i;
		}
		this->indeks = 624;
	}
	//funkcja sk³adowa funkcji "generuj" u¿ywana gdy indeks rejestru przesuwnego osi¹gnie wartoœæ 624 
	void twist();
	//funkcja zwracaj¹ca losow¹ wartoœæ
	virtual unsigned int generuj();
public:
	//funkcja generuj¹ca liczby losowe (tworzy listê liczb losowych)
	virtual void losuj() {
		inicjalizuj(static_cast<unsigned int>(time(NULL)));
		for (unsigned int i = 0; i < ilosc_do_wylosowania; i++) { 
			unsigned int wygenerowana;
			if (zakres == 4294967295)
				wygenerowana = generuj();
			else
				wygenerowana = (generuj() % (zakres + 1)) + zakres_min;
			lista_wylosowanych->dodaj_liczbe(wygenerowana);
		}
	}
	//funkcja uruchamiaj¹ca wszystkie funkcje pobieraj¹ce od u¿ytkownika parametry losowania
	virtual void pobierz_wszystkie_parametry() {
		pobierz_parametry();
	}

	MersenneTwister & operator = (const MersenneTwister & generator);

	MersenneTwister() {
		std::cout << "\nMersenne Twister\n";
		pobierz_wszystkie_parametry();
		this->nazwa = "Mersenne Twister";
	}
};

void MersenneTwister::twist() {
	unsigned int x, xA;
	for (int i = 0; i < 624; i++) {
		x = (Rejestr_przesuwny[i] & gorna_maska) + (Rejestr_przesuwny[(i + 1) % 624] & dolna_maska);
		xA = x >> 1;
		if (x & 0x1)
			xA ^= A;
		Rejestr_przesuwny[i] = Rejestr_przesuwny[(i + 1) % 624] ^ xA;
	}
	this->indeks = 0;
}

unsigned int MersenneTwister::generuj() {
	unsigned int y;
	int i = indeks;
	if (indeks >= 624) {
		twist();
		i = indeks;
	}
	y = Rejestr_przesuwny[i];
	this->indeks = i + 1;
	y ^= (Rejestr_przesuwny[i] >> U);
	y ^= (y << S) & B;
	y ^= (y << T) & C;
	y ^= (y >> L);
	return y;
}

MersenneTwister & MersenneTwister::operator = (const MersenneTwister & generator) {
	if (this == &generator)
		return *this;

	this->ilosc_do_wylosowania = generator.ilosc_do_wylosowania;
	this->lista_wylosowanych = generator.lista_wylosowanych;
	this->nazwa = generator.nazwa;
	this->rozklad = generator.rozklad;
	this->wykres = generator.wykres;
	this->zakres = generator.zakres;
	this->zakres_max = generator.zakres_max;
	this->zakres_min = generator.zakres_min;
	this->calkowity_zakres = generator.calkowity_zakres;
	this->calkowity_zakres_max = generator.calkowity_zakres_max;
	this->calkowity_zakres_min = generator.calkowity_zakres_min;

	this->indeks = generator.indeks;
	for(int i = 0; i < 624; i++)
		this->Rejestr_przesuwny[i] = generator.Rejestr_przesuwny[i];

	return *this;
}

#endif

