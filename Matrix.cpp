//#include <stdio.h> 
#include <time.h>
#include <stdlib.h>


#define _CRT_SECURE_NO_WARNINGS
#include "Matrix.h"

Matrix::Matrix() {
	noerrors = true;
	clrscr();
	gotoxy(0, 0);
	cputs("Podaj rozmiar planszy: ");
	int startX = wherex();
	int x, y = wherey(); // pozycja kursora
	char znak;
	bool podanoParzysta = true;


	do
	{
		do {
			znak = getch();
			if (znak == '\r') break;	//enter
			else if (znak == 0x08) {	//backspace
				x = wherex();
				if (startX < x) {
					putchar(znak);
					putchar(' ');
					putchar(znak);
					size /= 10;
				}

			}
			else if (znak >= '0' && znak <= '9') {
				putchar(znak);
				size *= 10;
				size += znak - 0x30;

			}
		} while (true);
		if (size < 2 || size % 2 == 1) {
			gotoxy(startX, y + 1);
			printf("Podano nieparzysta liczbe lub za maly rozmiar!");
			podanoParzysta = false;
			size = 0;
			int charsToDelete = wherex() - startX;
			gotoxy(startX, y);
			for (int i = 0; i < charsToDelete; i++)
				putch(' ');
			gotoxy(startX, y);
		}
		else {
			podanoParzysta = true;
		}
	} while (!podanoParzysta);

	elements_size = size*size;
	matrix = new Pole**[size];

	for (int i = 0; i < size; i++) {
		matrix[i] = new Pole*[size];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix[i][j] = new Pole(j, i);
		}
	}
}
Matrix::Matrix(int size) {
	noerrors = true;
	//TODO: sprawdziæ poprawnoœæ tej funkcji
	setSize(size);

	char filename[17] = "plansza00x00.txt";
	char* rzad;
	if (size < 10) {
		filename[8] = size + '0';
		filename[11] = size + '0';
	}
	else {
		filename[7] = filename[10] = size / 10 + '0';
		filename[8] = filename[11] = size % 10 + '0';
	}

	if (size == 12) {
		//FILE* file = fopen(filename, "r");
		FILE* file = fopen("C:\\Users\\chom\\Dropbox\\PP\\BinaryPuzzle\\Debug\\plansza12x12.txt", "r");
		if (file != NULL) {
			matrix = new Pole**[size];
			for (int i = 0; i < size; i++) {
				matrix[i] = new Pole*[size];
			}


			rzad = new char[size + 1];
			for (int i = 0; i < size; i++) {
				fread(rzad, 1, size + 1, file);
				for (int j = 0; j < size; j++) {
					matrix[i][j] = new Pole(j, i, rzad[j]);
				}
			}
			fclose(file);
		}
		else {
			noerrors = false;
		}
	}
}

Matrix::~Matrix() {
	for (int i = 0; i < size; i++) {
		delete matrix[i];
	}
	delete matrix;
}

int Matrix::getSize() {
	return size;
}

void Matrix::setSize(int s) {
	size = s;
}

bool Matrix::validate(int x, int y, char znak) {
	Pole *a = matrix[y][x];
	if (a->preset)
		return false;
	// zasada 1)
	switch (0) {
	case 0:
		if (y > 0)
			if (matrix[y - 1][x]->znak == znak) {
				if (y > 1 && matrix[y - 2][x]->znak == znak)
					return false;
				if (y < size - 1)
					if (matrix[y + 1][x]->znak == znak)
						return false;
			}
	case 1:
		if (y < size - 1)
			if (matrix[y + 1][x]->znak == znak) {
				if (y < size - 2 && matrix[y + 2][x]->znak == znak)
					return false;
			}
	case 2:
		if (x > 0)
			if (matrix[y][x - 1]->znak == znak) {
				if (x > 1 && matrix[y][x - 2]->znak == znak)
					return false;
				if (x < size - 1)
					if (matrix[y][x + 1]->znak == znak)
						return false;

			}
	case 3:
		if (x < size - 1)
			if (matrix[y][x + 1]->znak == znak) {
				if (x < size - 2 && matrix[y][x]->znak == znak)
					return false;

			}
	}
	// zasada 2)
	int sumx = 0,
		sumy = 0;
	for (int i = 0; i < size; i++) {

		if (matrix[y][i]->znak == znak) { // sprawdzanie rzêdu
			sumy++;
		}
		if (matrix[i][x]->znak == znak) { // sprawdzanie kolumny
			sumx++;
		}
		if (sumx >= size / 2 || sumy >= size / 2)
			return false;
	}
	// zasada 3) // rzêdy nie mog¹ byæ takie same
	int sum = 0,
		sum_compare = 0;
	if (completed(x, y, 'x')) { // sprawdzanie rzêdów
		sum = get_supposed_value(x, y, 'x', znak);
		for (int i = 0; i < size; i++) {
			if (completed(x, i, 'x')) {
				sum_compare = get_value(x, i, 'x');
				if (sum_compare == sum)
					return false;
			}
		}
	}
	sum_compare = sum = 0;
	if (completed(x, y, 'y')) { // sprawdzenie kolumn
		sum = get_supposed_value(x, y, 'y', znak);
		for (int i = 0; i < size; i++) {
			if (completed(i, y, 'y')) {
				sum_compare = get_value(i, y, 'y');
				if (sum_compare == sum)
					return false;
			}
		}
	}
	return true;
}

void Matrix::printMatrix(int x, int y) {
	Pole* p;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p = matrix[i][j];
			if (p->preset) {
				/*textbackground(WHITE);*/
				textcolor(LIGHTGREEN);
			}
			p->write(x, y);
			//textbackground(BLACK);
			textcolor(WHITE);
		}
	}

}

bool Matrix::setchar(int x, int y, char c) {
	if (validate(x, y, c)) {
		matrix[y][x]->setchar(c);
		return true;
	}
	return false;
}

bool Matrix::completed(int x, int y, char c) {
	// TODO: naprawiæ funkcjê, ¿eby sensowenie dzia³a³a
	int value = c == 'x' ? x : y;
	for (int i = 0; i < size; i++) {
		if (matrix[y][x]->znak == '.') {
			return false;
		}
	}
	return true;
}

int Matrix::get_value(int x, int y, char c) {
	int sum = 0;
	int mnoznik = 1;
	if (c == 'x') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			sum += (matrix[y][i]->znak - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			sum += (matrix[i][x]->znak - '0')*mnoznik;
		}
	}
	return sum;
}

int Matrix::get_supposed_value(int x, int y, char c, char input) {
	int sum = 0;
	int mnoznik = 1;
	if (c == 'x') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			if (i == x)
				sum += input - '0';
			else
				sum += (matrix[y][i]->znak - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			if (i == y)
				sum += input - '0';
			else
				sum += (matrix[i][x]->znak - '0')*mnoznik;
		}
	}
	return sum;
}

bool Matrix::fill_random_pole() {
	int x = (int)rand() % size;
	int y = (int)rand() % size;
	if (validate(x, y, '0')) {
		matrix[y][x]->setchar('0');
		return true;
	}
	else if (validate(x, y, '1')) {
		matrix[y][x]->setchar('0');
		return true;
	}
	else {
		return false;
	}
}