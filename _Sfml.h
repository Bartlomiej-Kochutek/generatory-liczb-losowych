#ifndef SFML_H
#define SFML_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <boost/lexical_cast.hpp>

//wymiary okna prezentuj¹cego rozk³ad
#define SZEROKOSC 1000.0
#define WYSOKOSC 600.0

//struktura stanowi¹ca element listy wylosowanych liczb
struct Wylosowana {
	unsigned int wartosc;
	struct Wylosowana* nast;
};

//klasa odpowiedzialna za wyœwietlanie rozk³adu wylosowanych liczb w postaci punktów uk³adu wspó³rzêdnych
class Rozklad {
	//wskaŸnik do pierwszego elementu listy
	struct Wylosowana* pierwszy_X = nullptr;
	//wspó³czynniki proporcjonalnoœci 
	double prop_szerokosc, prop_wysokosc;
	//nazwa okna, w którym wyœwietlany jest rozk³ad
	std::string nazwa_okna;

	//funkcja jest czêœci¹ funkcji "rysuj_rozklad", modyfikuje obraz (zmienn¹ "image") w taki sposób, ¿e 
	// dla ka¿dego punktu rozk³adu zmienia kolor jednego piksela o wspó³rzêdnych danego punktu
	void rysuj_obraz(sf::Image & image);
public:
	//funkcja ustawiaj¹ca wartoœci zmiennych prywatnych klasy
	void ustaw_wartosci(const unsigned int & zakres, const unsigned int & zakres_min, const unsigned int & zakres_max, struct Wylosowana *pierwszy, const std::string & napis);

	//funkcja tworzy okno, w którym wyœwietlany jest rozk³ad
	void rysuj_rozklad();

	Rozklad & operator = (const Rozklad & rozklad);
};

void Rozklad::ustaw_wartosci(const unsigned int & zakres, const unsigned int & zakres_min, const unsigned int & zakres_max, struct Wylosowana *pierwszy, const std::string & napis) {
	//ustawienie wartoœci wspó³czynników
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
	//w sfml wspó³rzêdne wyznaczane s¹ od lewego górnego rogu dlatego nale¿y obróciæ modyfikowany obraz tak by pocz¹tek 
	// uk³adu wspó³rzêdnych by³ w lewym dolnym rogu
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

//klasa odpowiedzialna za wyœwietlanie rozk³adu wylosowanych liczb w postaci wykresu
class Wykres {
	//wskaŸnik do pierwszego elementu listy
	struct Wylosowana* pierwszy_X = nullptr;
	//wspó³czynnik proporcjonalnoœci szerokoœci wykresu do zakresu losowanych liczb
	double prop_szerokosc;
	//wysokoœæ okna, w którym bêdzie wyœwietlany wykres
	int wysokosc = 0;
	//nazwa okna, w którym bêdzie wyœwietlany wykres
	std::string nazwa_okna;
	//tablica zliczaj¹ca ile razy zosta³a wylosowana zmienna z danego przedzia³u
	int tab_ilosc[static_cast<int>(SZEROKOSC)];

	//funkcja zape³niaj¹ca tablicê "tab_ilosc" odpowiednimi wartoœciami
	void ustaw_tablice();
	//funkcja jest czêœci¹ funkcji "rysuj_wykres"
	void rysuj_obraz(sf::Image & image);
public:
	//funkcja ustawiaj¹ca wartoœci zmiennych prywatnych klasy
	void ustaw_wartosci(unsigned int zakres, unsigned int zakres_min, unsigned int zakres_max, Wylosowana* pierwszy, std::string napis);
	//funkcja tworzy okno, w którym wyœwietlany jest rozk³ad
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
	//ustawienie wartoœci wspó³czynników
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