#include "Generators.h"

int main() {
	std::cout << "program generujacy (z powtorzeniami) liczby calkowite z zakresu (maksymalny) <0; 4 294 967 295>\n";
	Generator* wsk_generator = nullptr;
	while (true) {
		if (menu_1())
			break;

		ustaw_wsk(wsk_generator);

		if (funkcje(wsk_generator))
			break;
	}
	
	return 0;
};
