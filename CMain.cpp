#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include "CMain.h"
#include "Matrix.h"
CMain::CMain() {
	startX = 40;
	startY = 5;
	symbol = 0;
	consoleX = startX + 1;
	consoleY = startY + 1;
	textCol = 7;
	bgCol = 0;
	zero = 0;
	m = new Matrix(12); //plansza wczytana z pliku
	srand((unsigned int)time(NULL));
}

CMain::~CMain() {
	delete m;
}

int CMain::run() {
	int x, y;
	do {
		textbackground(BLACK);
		clrscr();
		textcolor(7);
		x = consoleX - 1 - startX;
		y = consoleY - 1 - startY;
		posx = x;
		posy = y;
		printBorder(); // wypisanie planszy od miejsca x,y (konsola zaczyna siê od (1,1)
		//if (m->noerrors == false) { // TODO: sprawdziæ czy potrzebne
		//	printf("An error occured!");
		//	return -1;
		//}
		m->printMatrix(startX + 1, startY + 1);

		printMenu(1, 1);



		// rysujemy na ekranie kolorow¹ gwiazdkê
		gotoxy(consoleX, consoleY);
		textcolor(textCol);
		textbackground(bgCol);
		//putch('*');


		zero = 0;
		symbol = getch();
		if (symbol == 0) {
			zero = 1;
			symbol = getch();

			if (symbol == 0x48 && consoleY > startY + 1) consoleY--;
			else if (symbol == 0x50 && consoleY - m->getSize() < startY) consoleY++;
			else if (symbol == 0x4b && consoleX > startX + 1) consoleX--;
			else if (symbol == 0x4d && consoleX - m->getSize() < startX) consoleX++;
		}
		else if (symbol == '1' || symbol == '0' || symbol == '2') {
			if (symbol == '2')
				symbol = '0';
			// wpisz znak do planszy w pozycji x, y
			// TODO: b³¹d gdy kursor jest poza plansz¹
			m->setChar(x, y, symbol);
		}
		else if (symbol == 'j') {

		}
		else if (symbol == 'l') {
			load();
		}
		else if (symbol == 'n') {
			delete m;
			m = new Matrix(12);
		}
		else if (symbol == 'o') {
			m->fillRandomFields();
		}
		else if (symbol == 'p') {
			// TODO: podpowiedz
		}
		else if (symbol == 'r') {
			startNewGame();
			// TODO: wybieranie planszy z pliku
		}
		else if (symbol == 's') {
			save();
		}
		else if (symbol == '.') {
			m->clear(x, y);
		}
		else if (symbol == ' ') textCol = (textCol + 1) % 16;
		else if (symbol == 0x0d) bgCol = (bgCol + 1) % 16;
	} while (symbol != 'q' && symbol != 0x1b);
	return 0;
}

void CMain::printMenu(int x, int y) {
	if (x > 0) {
		gotoxy(x, y);
		cputs("*=====+=====+=====+=====+=====*");
	}
	gotoxy(x, y + 1);
	// wyœwietlamy na ekranie, w miejscu kursora napis
	// kursor przesuwa siê w prawo o d³ugoœæ napisu
	cputs("|| q = wyjscie");
	gotoxy(x, y + 2);
	cputs("|| strzalki = poruszanie");
	gotoxy(x, y + 3);
	cputs("|| spacja = zmiana koloru");
	gotoxy(x, y + 4);
	cputs("|| enter = zmiana koloru tla");

	// wypisujemy na ekranie kod ostatnio naciœniêtego klawisza
	if (zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", symbol);
	else sprintf(txt, "kod klawisza: 0x%02x", symbol);
	gotoxy(x, y + 5);
	//wyswietlanie pozycji kursora
	cputs("|| kursor: ");
	if (consoleX >= 10)
		putchar(consoleX / 10 + 0x30);
	putchar(consoleX % 10 + 0x30);

	putchar(' ');
	if (consoleY >= 10)
		putchar(consoleY / 10 + 0x30);
	putchar(consoleY % 10 + 0x30);

	cputs("//");
	if (posx >= 10)
		putchar(posx / 10 + 0x30);
	putchar(posx % 10 + 0x30);

	putchar(' ');
	if (posy >= 10)
		putchar(posy / 10 + 0x30);
	putchar(posy % 10 + 0x30);



	gotoxy(x, y + 6);
	cputs("|| ");
	cputs(txt);
	gotoxy(x, y + 7);
	cputs("|| Mozliwe wartosci:");
	if (m->validate(posx, posy, '0'))
		putchar('0');
	if (m->validate(posx, posy, '1'))
		putchar('1');

	gotoxy(x, y + 8);
	cputs("*=====+=====+=====+=====+=====*");

	//drukowane boków
	for (int i = 1; i < 7; i++) {
		gotoxy(x + 29, y + i);
		cputs("||");
	}
}

void CMain::printBorder() {
	int rozmiar = m->getSize();
	gotoxy(startX, startY);
	for (int i = 1; i < rozmiar + 3; i++) {
		cputs("=");
	}
	for (int i = 1; i < rozmiar + 1; i++) {
		gotoxy(startX, startY + i);
		cputs("|");
		gotoxy(startX + rozmiar + 1, startY + i);
		cputs("|");
	}
	gotoxy(startX, startY + rozmiar + 1);
	for (int i = 1; i < rozmiar + 3; i++) {
		cputs("=");
	}
}

int CMain::getUserInput() {

	int startX_pisania = wherex();
	char symbol;
	bool podanoParzysta = true;
	int x = wherex();
	int y = wherey();
	int size = 0;
	do
	{
		do {
			symbol = getch();
			if (symbol == '\r') break;	//enter
			else if (symbol == 0x08) {	//backspace
				x = wherex();
				if (startX_pisania < x) {
					putchar(symbol);
					putchar(' ');
					putchar(symbol);
					size /= 10;
				}

			}
			else if (symbol >= '0' && symbol <= '9') {
				putchar(symbol);
				size *= 10;
				size += symbol - 0x30;

			}
		} while (true);
		if (size < 2 || size % 2 == 1) {
			gotoxy(startX_pisania, y + 1);
			printf("Podano nieparzysta liczbe lub za maly rozmiar!");
			podanoParzysta = false;
			size = 0;
			int charsToDelete = wherex() - startX_pisania;
			gotoxy(startX_pisania, y);
			for (int i = 0; i < charsToDelete; i++)
				putch(' ');
			gotoxy(startX_pisania, y);
		}
		else {
			podanoParzysta = true;
		}
	} while (!podanoParzysta);
	return size;
}

void CMain::startNewGame() {
	clrscr();
	gotoxy(0, 0);
	cputs("Podaj rozmiar planszy: ");
	delete m;
	int size = getUserInput();
	m = new Matrix(size);
}

void CMain::save() {
	char nazwa_pliku[255];
	int startX_pisania = wherex();
	char symbol;
	bool podanoParzysta = true;
	int x = wherex();
	int y = wherey();
	int index = 0;
	do
	{
		clrscr();
		printf("Podaj nazwe pliku: ");
		do {
			symbol = getch();
			if (symbol == '\r') break;	//enter
			else if (symbol == 0x08) {	//backspace
				x = wherex();
				if (startX_pisania < x) {
					putchar(symbol);
					putchar(' ');
					putchar(symbol);
					index--;
				}

			}
			else if ((symbol >= '0' && symbol <= '9') || (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z')) {
				putchar(symbol);
				nazwa_pliku[index] = symbol;
				index++;

			}
		} while (true);
	} while (index < 1);
	nazwa_pliku[index] = '.';
	nazwa_pliku[index + 1] = 't';
	nazwa_pliku[index + 2] = 'x';
	nazwa_pliku[index + 3] = 't';
	nazwa_pliku[index + 4] = '\0';
	FILE* plik = fopen(nazwa_pliku, "w");
	if (plik != NULL) {
		fprintf(plik, "%d\n", m->getSize());
		char* linia = new char[2 * m->getSize() + 2];
		linia[m->getSize() * 2] = '\n';
		linia[m->getSize() * 2 + 1] = '\0';
		for (int j = 0; j < m->getSize(); j++) {
			for (int i = 0; i < m->getSize(); i++) {
				linia[2 * i] = m->board[j][i]->symbol;
				linia[2 * i + 1] = m->board[j][i]->preset ? 't' : 'f';
			}
			fputs(linia, plik);
		}
		delete linia;
		fclose(plik);
	}
	else {
		cputs("Blad przy otwieraniu pliku!");
		system("pause");
	}
}

void CMain::load() {
	char nazwa_pliku[255];
	int startX_pisania = wherex();
	char symbol;
	bool podanoParzysta = true;
	int x = wherex();
	int y = wherey();
	int index = 0;
	do
	{
		clrscr();
		printf("Podaj nazwe pliku: ");
		do {
			symbol = getch();
			if (symbol == '\r') break;	//enter
			else if (symbol == 0x08) {	//backspace
				x = wherex();
				if (startX_pisania < x) {
					putchar(symbol);
					putchar(' ');
					putchar(symbol);
					index--;
				}

			}
			else if ((symbol >= '0' && symbol <= '9') || (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z')) {
				putchar(symbol);
				nazwa_pliku[index] = symbol;
				index++;

			}
		} while (true);
	} while (index < 1);
	nazwa_pliku[index] = '.';
	nazwa_pliku[index + 1] = 't';
	nazwa_pliku[index + 2] = 'x';
	nazwa_pliku[index + 3] = 't';
	nazwa_pliku[index + 4] = '\0';
	FILE* plik = fopen(nazwa_pliku, "r");
	if (plik != NULL) {
		int size = 0;
		fscanf(plik, "%d\n", &size);
		char* linia = new char[2 * size + 2];

		delete m;
		m = new Matrix(size, true);
		linia[size * 2] = '\n';
		linia[size * 2 + 1] = '\0';
		for (int j = 0; j < size; j++) {
			fgets(linia, size * 2 + 2, plik);
			for (int i = 0; i < size; i++) {
				if (linia[2 * i + 1] == 't')
					m->board[j][i]->setPresetChar(linia[2 * i]);
				else
					m->board[j][i]->setChar(linia[2 * i]);
			}

		}
		delete[] linia;
		fclose(plik);
	}
	else {
		cputs("Blad przy otwieraniu pliku!");
		system("pause");
	}
}