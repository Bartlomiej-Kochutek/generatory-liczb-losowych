#ifndef GENERATORS_H
#define GENERATORS_H

#include "Class.h"

//funkcja zwraca: 0 je¿eli u¿ytkownik chce wybraæ rodzaj generatora losuj¹cego, 1 je¿eli u¿ytkownik chce wyjœæ z programu
int menu_1() {
	std::cout << "\n1 - wybierz generator \n2 - wyjscie z programu\n";
	unsigned int x;
	x = pobierz_unsigned(1, 2);
	x--;
	return x;
}

//funkcja zwraca wartoœæ odpowiadaj¹c¹ wybranemu generatorowi
int menu_2() {
	std::cout << "\n1 - Park-Miller \n2 - Kwadratowy kongruencyjny\n3 - Mersenne Twister \n";
	unsigned int x;
	x = pobierz_unsigned(1, 3);
	return x;
}

//funkcja alokuj¹ca odpowiedni generator
void ustaw_wsk(Generator* & wsk_generator) {
	switch (menu_2()) {
	case 1:
		wsk_generator = dynamic_cast<Generator*>(new ParkMiller);
		break;
	case 2:
		wsk_generator = dynamic_cast<Generator*>(new Kwadratowy);
		break;
	case 3:
		wsk_generator = dynamic_cast<Generator*>(new MersenneTwister);
		break;
	}
}

//funkcja dealokuj¹ca dany generator
void usun_wsk(Generator* & wsk_generator) {
	delete wsk_generator;
	wsk_generator = nullptr;
}

//funkcja wywo³ywana po wybraniu generatora, zwraca wartoœæ odpowiadaj¹c¹ wyborowi u¿ytkownika
int menu_3() {
	std::cout << "\n1 - losuj \n2 - losuj jeden element \n3 - rysuj rozklad \n4 - rysuj wykres \n5 - wyswietl wylosowane \n6 - wyswietl jeden element";
	std::cout << "\n7 - wczytaj z pliku CSV\n8 - zapisz do pliku CSV\n9 - zmien parametry \n10 - zmien generator (usuwa wylosowane)\n11 - usun wylosowane\n12 - wyjscie z programu \n";
	unsigned int x;
	x = pobierz_unsigned(1, 12);
	return x;
}

//funkcja wykonuj¹ca dzia³ania zaproponowane u¿ytkownikowi przez "menu_3". Zwraca 1 je¿eli u¿ytkownik chce zamkn¹æ program; 0 jeœli chce zmieniæ generator
bool funkcje(Generator* & wsk_generator) {
	while(1){
		switch (menu_3()) {
		case 1: wsk_generator->losuj();
			break;
		case 2: (*wsk_generator)++;
			break;
		case 3: wsk_generator->rysuj_rozklad();
			break;
		case 4: wsk_generator->rysuj_wykres();
			break;
		case 5: std::cout << *wsk_generator;
			break;
		case 6: wsk_generator->wyswietl_jeden();
			break;
		case 7: wsk_generator->wczytaj_z_pliku();
			break;
		case 8: wsk_generator->zapisz_do_pliku(); 
			break;
		case 9: wsk_generator->pobierz_wszystkie_parametry(); 
			break;
		case 10: usun_wsk(wsk_generator); 
			return 0; 
		case 11: wsk_generator->usun_wylosowane();
			break;
		case 12: usun_wsk(wsk_generator);
			return 1;
		}
	}
}

#endif
