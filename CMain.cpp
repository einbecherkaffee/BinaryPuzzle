#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include "CMain.h"
#include "Matrix.h"
CMain::CMain() {
	startX = 5;
	startY = 5;
	znak = 0;
	consoleX = startX + 1;
	consoleY = startY + 1;
	textCol = 7;
	bgCol = 0;
	zero = 0;
	//m = new Matrix;
	if (true) { //DEBUG
		m = new Matrix(12); //plansza wczytana z pliku
	}
	srand(time(NULL));
}

CMain::~CMain() {
	delete m;
}

int CMain::run() {
	do {
		// ustawiamy czarny kolor t�a, z kt�rego b�dziemy
		// korzystali rysuj�c po ekranie
		// prosz� zerkn�� do conio2.h na list� dost�pnych kolor�w
		textbackground(BLACK);
		// czy�cimy ekran: wype�niamy spacjami z ustawionym kolrem t�a
		clrscr();
		textcolor(7);


		// wypisanie planszy od miejsca x,y (konsola zaczyna si� od (1,1)
		printBorder();
		if (m->noerrors == false) {
			printf("An error occured!");
			return -1;
		}
		m->printMatrix(startX + 1, startY + 1);

		printMenu(55, 1);



		// rysujemy na ekranie kolorow� gwiazdk�
		gotoxy(consoleX, consoleY);
		textcolor(textCol);
		textbackground(bgCol);
		//putch('*');


		zero = 0;
		znak = getch();
		if (znak == 0) {
			zero = 1;
			znak = getch();

			if (znak == 0x48 && consoleY > startY + 1) consoleY--;
			else if (znak == 0x50 && consoleY - m->getSize() < startY) consoleY++;
			else if (znak == 0x4b && consoleX > startX + 1) consoleX--;
			else if (znak == 0x4d && consoleX - m->getSize() < startX) consoleX++;
		}
		else if (znak == '1' || znak == '0' || znak == '2') {
			if (znak == '2')
				znak = '0';
			// wpisz znak do planszy w pozycji x, y
			// TODO: b��d gdy kursor jest poza plansz�
			m->setchar(consoleX - 1 - startX, consoleY - 1 - startY, znak);
		}
		else if (znak == 'n') {
			// TODO: nowa gra
			if (m->noerrors)
				delete m;
			m = new Matrix(12);
		}
		else if (znak == 'o') {
			m->fill_random_pola();
		}
		else if (znak == 'p') {
			// TODO: podpowiedz
		}
		else if (znak == 'r') {
			delete m;
			m = new Matrix();
			// TODO: wybieranie planszy z pliku
		}
		else if (znak == ' ') textCol = (textCol + 1) % 16;
		else if (znak == 0x0d) bgCol = (bgCol + 1) % 16;
	} while (znak != 'q' && znak != 0x1b);
	return 0;
}

void CMain::printMenu(int x, int y) {
	if (x > 0) {
		gotoxy(x, y);
		cputs("*=====+=====+=====+=====+=====*");
	}
	gotoxy(x, y + 1);
	// wy�wietlamy na ekranie, w miejscu kursora napis
	// kursor przesuwa si� w prawo o d�ugo�� napisu
	cputs("|| q = wyjscie");
	gotoxy(x, y + 2);
	cputs("|| strzalki = poruszanie");
	gotoxy(x, y + 3);
	cputs("|| spacja = zmiana koloru");
	gotoxy(x, y + 4);
	cputs("|| enter = zmiana koloru tla");

	// wypisujemy na ekranie kod ostatnio naci�ni�tego klawisza
	if (zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", znak);
	else sprintf(txt, "kod klawisza: 0x%02x", znak);
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


	gotoxy(x, y + 6);
	cputs("|| ");
	cputs(txt);
	gotoxy(x, y + 7);
	cputs("*=====+=====+=====+=====+=====*");
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

void Matrix::fill_random_pola() {
	int prob_nieudanych = 0;
	int udanych = 0;
	for (int i = 0; i < size; i++) {
		if (fill_random_pole()) {
			udanych++;
		}
		else {
			prob_nieudanych++;
		}
		if (prob_nieudanych >= ile_prob_uzupelnienia)
			return;
		if (udanych >= ile_randomowych_pol_uzupelnic)
			return;
	}
}