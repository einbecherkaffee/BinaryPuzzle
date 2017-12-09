#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include "CMain.h"
#include "Matrix.h"
CMain::CMain() {
	startX = 30;
	startY = 1;
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
	const Point menu_pos(2, 2);
	int x, y;
	bool b_printKey = false;
	printMenu(menu_pos.x, menu_pos.y);
	printBorder(startX, startY, startX + m->getSize() + 1, startY + m->getSize() + 1);
	m->printMatrix(startX + 1, startY + 1);
	do {
		m->updateSizeArrays();
		textcolor(7);
		x = consoleX - 1 - startX;
		y = consoleY - 1 - startY;
		posx = x;
		posy = y;

		printMenu(menu_pos.x, menu_pos.y);

		if (b_printKey) {
			clrscr();
			printKey(1, 1);
			b_printKey = false;

			if (getch() == 0) {
				getch();
			}
			clrscr();
			printMenu(menu_pos.x, menu_pos.y);
			printBorder(startX, startY, startX + m->getSize() + 1, startY + m->getSize() + 1);
			m->printMatrix(startX + 1, startY + 1);
		}
		m->printMatrix(startX + 1, startY + 1);
		gotoxy(consoleX, consoleY);


		zero = 0;
		symbol = getch();
		if (m->highlightNotFillable) {
			m->highlightNotFillable = false;
			m->printMatrix(startX + 1, startY + 1);
		}
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
			
			m->setChar(x, y, symbol);
			m->board[y][x]->write(startX + 1, startY + 1);
			m->updateSizeArrays();
			
		}
		else if (symbol == 'j') {
			m->highlightUnambiguous = true;
			m->printMatrix(startX + 1, startY + 1);
			m->highlightUnambiguous = false;
			if (getch() == 'w') {
				m->fillUnambiguous();
			}
			//getch();
			m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == 'k') {
			m->highlightNotFillable = true;
			m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == 'l') {
			load();
			
		}
		else if (symbol == 'n') {
			clrscr();
			delete m;
			m = new Matrix(12);
			printBorder(startX, startY, startX + m->getSize() + 1, startY + m->getSize() + 1);
			m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == 'o') {
			m->fillRandomFields();
			//m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == 'p') {
			// TODO: podpowiedz
		}
		else if (symbol == 'r') {
			startNewGame();
			printMenu(menu_pos.x, menu_pos.y);
			printBorder(startX, startY, startX + m->getSize() + 1, startY + m->getSize() + 1);
			m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == 's') {
			save();
			printMenu(menu_pos.x, menu_pos.y);
			printBorder(startX, startY, startX + m->getSize() + 1, startY + m->getSize() + 1);
			m->printMatrix(startX + 1, startY + 1);
		}
		else if (symbol == '.') {
			m->clear(x, y);
		}
		else if (symbol == ' ') {
			b_printKey = true;
		}
	} while (symbol != 'q' && symbol != 0x1b);
	return 0;
}

void CMain::printMenu(int x, int y) {
	// esc, q = wyjœcie
	gotoxy(x, y);
	cputs("spacja = legenda");
	// kod klawisza
	if (zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", symbol);
	else sprintf(txt, "kod klawisza: 0x%02x", symbol);
	gotoxy(x, y + 1);
	cputs("wymagania: a-f");
	// kursor
	{
		gotoxy(x, y + 2);
		cputs("kursor: ");
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
	}
	// kod klawisza
	gotoxy(x, y + 3);
	cputs(txt);
	// mo¿liwe wartoœci dla pola
	gotoxy(x, y + 4);
	cputs("Mozliwe wartosci:  ");
	gotoxy(x + 17, y + 4);
	if (m->validate(posx, posy, '0'))
		putchar('0');
	if (m->validate(posx, posy, '1'))
		putchar('1');
	printBorder(x - 1, y - 1, x + 24, y + 5);
}

void CMain::printBorder(int x1, int y1, int x2, int y2) {
	gotoxy(x1, y1);
	int width = x2 - x1;
	int height = y2 - y1;
	for (int i = 1; i < width + 2; i++) {
		cputs("=");
	}
	for (int i = 1; i < height + 1; i++) {
		gotoxy(x1, y1 + i);
		cputs("|");
		gotoxy(x2, y1 + i);
		cputs("|");
	}
	gotoxy(x1, y2);
	for (int i = 1; i < width + 2; i++) {
		cputs("=");
	}
}

void CMain::printKey(int x, int y) {
	gotoxy(x, y);
	cputs("esc, q = wyjscie");
	gotoxy(x, y + 1);
	cputs("n= nowa gra");
	gotoxy(x, y + 2);
	cputs("01 = wpisanie cyfry");
	gotoxy(x, y + 3);
	cputs("o = losowe wypelnienie planszy");
	gotoxy(x, y + 4);
	cputs("//p = prosta podpowiedz");
	gotoxy(x, y + 5);
	cputs("r = zmiana rozmiaru planszy");
	gotoxy(x, y + 6);
	cputs("k = proste sprawdzenie mozliwosci ukonczenia gry");
	gotoxy(x, y + 7);
	cputs("d = sprawdzenie reguly (2)");
	gotoxy(x, y + 8);
	cputs("a = automatyczna detekcja konca gry");
	gotoxy(x, y + 9);
	cputs("j = podswietlenie jednoznacznych pol");
	gotoxy(x, y + 10);
	cputs("s = zapisanie stanu gry");
	gotoxy(x, y + 11);
	cputs("l = wczytanie stanu gry");
	gotoxy(x, y + 12);
	cputs("b = pelne sprawdzenie i pokazanie"\
		"przykladowego wypelnienia pol");
}

int CMain::getUserInput() {
	const int max_size = 40;
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
		if (size < 2 || size % 2 == 1 || size>max_size) {
			gotoxy(1, y + 1);
			printf("Podano nieparzysta liczbe lub za maly/duzy rozmiar!");
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
	delete m;
	gotoxy(1, 1);
	cputs("Podaj rozmiar planszy: ");
	int size = getUserInput();
	m = new Matrix(size);
}

void CMain::save() {
	gotoxy(1, 1);
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
	gotoxy(1, 1);
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