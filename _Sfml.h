#ifndef SFML_H
#define SFML_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <boost/lexical_cast.hpp>

//wymiary okna prezentuj�cego rozk�ad
#define SZEROKOSC 1000.0
#define WYSOKOSC 600.0

//struktura stanowi�ca element listy wylosowanych liczb
struct Wylosowana {
	unsigned int wartosc;
	struct Wylosowana* nast;
};

//klasa odpowiedzialna za wy�wietlanie rozk�adu wylosowanych liczb w postaci punkt�w uk�adu wsp�rz�dnych
class Rozklad {
	//wska�nik do pierwszego elementu listy
	struct Wylosowana* pierwszy_X = nullptr;
	//wsp�czynniki proporcjonalno�ci 
	double prop_szerokosc, prop_wysokosc;
	//nazwa okna, w kt�rym wy�wietlany jest rozk�ad
	std::string nazwa_okna;

	//funkcja jest cz�ci� funkcji "rysuj_rozklad", modyfikuje obraz (zmienn� "image") w taki spos�b, �e 
	// dla ka�dego punktu rozk�adu zmienia kolor jednego piksela o wsp�rz�dnych danego punktu
	void rysuj_obraz(sf::Image & image);
public:
	//funkcja ustawiaj�ca warto�ci zmiennych prywatnych klasy
	void ustaw_wartosci(const unsigned int & zakres, const unsigned int & zakres_min, const unsigned int & zakres_max, struct Wylosowana *pierwszy, const std::string & napis);

	//funkcja tworzy okno, w kt�rym wy�wietlany jest rozk�ad
	void rysuj_rozklad();

	Rozklad & operator = (const Rozklad & rozklad);
};

void Rozklad::ustaw_wartosci(const unsigned int & zakres, const unsigned int & zakres_min, const unsigned int & zakres_max, struct Wylosowana *pierwszy, const std::string & napis) {
	//ustawienie warto�ci wsp�czynnik�w
	this->prop_szerokosc = static_cast<double>((SZEROKOSC - 1) / zakres);
	this->prop_wysokosc = static_cast<double>((WYSOKOSC - 1) / zakres);
	
	this->pierwszy_X = pierwszy;
	//modyfikacja nazwy okna
	nazwa_okna = "Rozklad wylosowanych liczb generatora " + napis + " z zakresu od ";
	nazwa_okna += boost::lexical_cast<std::string>(zakres_min) + " do " + boost::lexical_cast<std::string>(zakres_max);
}

void Rozklad::rysuj_obraz(sf::Image & image) {
	struct Wylosowana* temp = pierwszy_X;
	struct Wylosowana* nast = pierwszy_X->nast;
	while (temp != nullptr) {
		if (nast == nullptr)
			nast = pierwszy_X;
		int x = static_cast<int>(temp->wartosc * prop_szerokosc);
		int y = static_cast<int>(nast->wartosc * prop_wysokosc);
		image.setPixel(x, y, sf::Color::Green);
		temp = temp->nast;
		nast = nast->nast;
	}
	//w sfml wsp�rz�dne wyznaczane s� od lewego g�rnego rogu dlatego nale�y obr�ci� modyfikowany obraz tak by pocz�tek 
	// uk�adu wsp�rz�dnych by� w lewym dolnym rogu
	image.flipVertically();
}

void Rozklad::rysuj_rozklad() {
	if (pierwszy_X == nullptr) {
		std::cout << "brak wylosowanych liczb\n";
	}
	else {
		sf::RenderWindow window(sf::VideoMode(static_cast<int>(SZEROKOSC), static_cast<int>(WYSOKOSC)), nazwa_okna);

		sf::Image image;
		image.create(static_cast<int>(SZEROKOSC), static_cast<int>(WYSOKOSC), sf::Color::Black);
		rysuj_obraz(image);

		sf::Texture texture;
		texture.create(static_cast<int>(SZEROKOSC), static_cast<int>(WYSOKOSC));
		texture.update(image, 0, 0);

		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);

		window.clear();
		window.draw(sprite);
		std::cout << "\n\nrozklad wylosowanych liczb w postaci punktow (Xi , X(i+1))\n\n";
		window.display();
		system("Pause");
	}
}

Rozklad & Rozklad::operator = (const Rozklad & rozklad) {
	if (this == &rozklad)
		return *this;
	this->nazwa_okna = rozklad.nazwa_okna;
	this->pierwszy_X = rozklad.pierwszy_X;
	this->prop_szerokosc = rozklad.prop_szerokosc;
	this->prop_wysokosc = rozklad.prop_wysokosc;
	return *this;
}

//klasa odpowiedzialna za wy�wietlanie rozk�adu wylosowanych liczb w postaci wykresu
class Wykres {
	//wska�nik do pierwszego elementu listy
	struct Wylosowana* pierwszy_X = nullptr;
	//wsp�czynnik proporcjonalno�ci szeroko�ci wykresu do zakresu losowanych liczb
	double prop_szerokosc;
	//wysoko�� okna, w kt�rym b�dzie wy�wietlany wykres
	int wysokosc = 0;
	//nazwa okna, w kt�rym b�dzie wy�wietlany wykres
	std::string nazwa_okna;
	//tablica zliczaj�ca ile razy zosta�a wylosowana zmienna z danego przedzia�u
	int tab_ilosc[static_cast<int>(SZEROKOSC)];

	//funkcja zape�niaj�ca tablic� "tab_ilosc" odpowiednimi warto�ciami
	void ustaw_tablice();
	//funkcja jest cz�ci� funkcji "rysuj_wykres"
	void rysuj_obraz(sf::Image & image);
public:
	//funkcja ustawiaj�ca warto�ci zmiennych prywatnych klasy
	void ustaw_wartosci(unsigned int zakres, unsigned int zakres_min, unsigned int zakres_max, Wylosowana* pierwszy, std::string napis);
	//funkcja tworzy okno, w kt�rym wy�wietlany jest rozk�ad
	void rysuj_wykres();

	Wykres & operator = (const Wykres & wykres);
};

void Wykres::ustaw_tablice() {
	int szerokosc = static_cast<int>(SZEROKOSC);
	memset(tab_ilosc, 0, static_cast<std::size_t>(szerokosc * sizeof(int)));
	struct Wylosowana* temp = pierwszy_X;
	int x;
	while (temp != nullptr) {
		x = static_cast<int>(temp->wartosc * prop_szerokosc);
		temp = temp->nast;
		tab_ilosc[x]++;
		if (tab_ilosc[x] > wysokosc)
			(this->wysokosc)++;
	}
	if (wysokosc > WYSOKOSC) 
		this->wysokosc = static_cast<int>(WYSOKOSC);
	x = 0;
	for (int i = 0; i < szerokosc; i++) {
		if (tab_ilosc[i] > wysokosc) {
			tab_ilosc[i] = wysokosc;
			x++;
		}
	}
	if (x > 0)
		std::cout << "ilosc wylosowanych liczb przekracza wysokosc okna (w pikselach) w " << x << " miejscach \n";
}

void Wykres::ustaw_wartosci(unsigned int zakres, unsigned int zakres_min, unsigned int zakres_max, struct Wylosowana* pierwszy, std::string napis) {
	//ustawienie warto�ci wsp�czynnik�w
	this->wysokosc = 0;
	this->prop_szerokosc = static_cast<double>((SZEROKOSC) / zakres);
	
	this->pierwszy_X = pierwszy;
	ustaw_tablice();
	//modyfikacja nazwy okna
	nazwa_okna = "Wykres wylosowanych liczb generatora " + napis + " z zakresu od ";
	nazwa_okna += boost::lexical_cast<std::string>(zakres_min) + " do " + boost::lexical_cast<std::string>(zakres_max);
}

void Wykres::rysuj_obraz(sf::Image & image) {
	for (int i = 0; i < SZEROKOSC; i++) {
		image.setPixel(i, tab_ilosc[i], sf::Color::Green);
		if (tab_ilosc[i] > 0)
			image.setPixel(i, tab_ilosc[i] - 1, sf::Color::Green);
		if (tab_ilosc[i] > 1)
			image.setPixel(i, tab_ilosc[i] - 2, sf::Color::Green);
	}
	image.flipVertically();
}

void Wykres::rysuj_wykres() {
	if (pierwszy_X == nullptr) {
		std::cout << "brak wylosowanych liczb\n";
	}
	else {
		int _szerokosc = static_cast<int>(SZEROKOSC);
		int _wysokosc = wysokosc + 1;
		sf::RenderWindow window(sf::VideoMode(_szerokosc, _wysokosc), nazwa_okna);

		sf::Image image;
		image.create(_szerokosc, _wysokosc, sf::Color::Black);
		rysuj_obraz(image);

		sf::Texture texture;
		texture.create(_szerokosc, _wysokosc);
		texture.update(image, 0, 0);

		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);

		window.clear();
		window.draw(sprite);
		std::cout << "\n\nwykres przedstawiajacy rozklad wylosowanych liczb  \n\n";
		window.display();
		system("Pause");
	}
}

Wykres & Wykres::operator = (const Wykres & wykres) {
	if (this == &wykres)
		return *this;
	this->nazwa_okna = wykres.nazwa_okna;
	this->pierwszy_X = wykres.pierwszy_X;
	this->prop_szerokosc = wykres.prop_szerokosc;
	for(int i = 0; i < static_cast<int>(SZEROKOSC); i++)
		this->tab_ilosc[i] = wykres.tab_ilosc[i];
	return *this;
}

#endif SFML_H