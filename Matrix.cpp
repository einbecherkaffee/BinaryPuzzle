#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h> 
#include <time.h>
#include <stdlib.h>

#include "Matrix.h"

Matrix::Matrix() { }

Matrix::Matrix(int size) {
	setSize(size);

	char filename[] = "plansza00x00.txt";
	char* rzad;
	if (size < 10) {
		filename[8] = size + '0';
		filename[11] = size + '0';
	}
	else {
		filename[7] = filename[10] = size / 10 + '0';
		filename[8] = filename[11] = size % 10 + '0';
	}


	FILE* file = fopen(filename, "r");
	if (file != NULL) {
		board = new Pole**[size];
		for (int i = 0; i < size; i++) {
			board[i] = new Pole*[size];
		}


		rzad = new char[size + 1];
		for (int i = 0; i < size; i++) {
			fread(rzad, 1, size + 1, file);
			for (int j = 0; j < size; j++) {
				board[i][j] = new Pole(j, i, rzad[j]);
			}
		}
		fclose(file);
	}
	else {
		board = new Pole**[size];
		for (int i = 0; i < size; i++) {
			board[i] = new Pole*[size];
		}


		rzad = new char[size + 1];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				board[i][j] = new Pole(j, i, '.');
			}
		}
		for (int i = 0; i < size; i++) {
			fill_random_pola();
		}
	}
}

Matrix::Matrix(int size, bool a) {
	this->size = size;
	board = new Pole**[size];
	for (int i = 0; i < size; i++) {
		board[i] = new Pole*[size];
	}


	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = new Pole(j, i);
		}
	}
}


Matrix::~Matrix() {
	if (!board)
		for (int i = 0; i < size; i++) {
			delete board[i];
		}
	delete board;
}

int Matrix::getSize() {
	return size;
}

void Matrix::setSize(int s) {
	size = s;
}

bool Matrix::validate(int x, int y, char znak) {
	Pole *a = board[y][x];
	if (a->preset)
		return false;
	// zasada 1)
	switch (0) {
	case 0:
		if (y > 0)
			if (board[y - 1][x]->znak == znak) {
				if (y > 1 && board[y - 2][x]->znak == znak)
					return false;
				if (y < size - 1)
					if (board[y + 1][x]->znak == znak)
						return false;
			}
	case 1:
		if (y < size - 1)
			if (board[y + 1][x]->znak == znak) {
				if (y < size - 2 && board[y + 2][x]->znak == znak)
					return false;
			}
	case 2:
		if (x > 0)
			if (board[y][x - 1]->znak == znak) {
				if (x > 1 && board[y][x - 2]->znak == znak)
					return false;
				if (x < size - 1)
					if (board[y][x + 1]->znak == znak)
						return false;
			}
	case 3:
		if (x < size - 1)
			if (board[y][x + 1]->znak == znak) {
				if (x < size - 2 && board[y][x + 2]->znak == znak)
					return false;
			}
	}
	// zasada 2)
	int sumx = 0,
		sumy = 0;
	for (int i = 0; i < size; i++) {

		if (board[y][i]->znak == znak) { // sprawdzanie rzêdu
			sumy++;
		}
		if (board[i][x]->znak == znak) { // sprawdzanie kolumny
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
			if (i != x)
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
			p = board[i][j];
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
		board[y][x]->setchar(c);
		return true;
	}
	return false;
}

bool Matrix::completed(int x, int y, char c) {
	// TODO: naprawiæ funkcjê, ¿eby sensowenie dzia³a³a
	int value = c == 'x' ? x : y;
	for (int i = 0; i < size; i++) {
		if (board[y][x]->znak == '.') {
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
			sum += (board[y][i]->znak - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			sum += (board[i][x]->znak - '0')*mnoznik;
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
				sum += (board[y][i]->znak - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			if (i == y)
				sum += input - '0';
			else
				sum += (board[i][x]->znak - '0')*mnoznik;
		}
	}
	return sum;
}

bool Matrix::fill_random_pole() {
	int x = (int)rand() % size;
	int y = (int)rand() % size;
	int random_znak = rand() % 2 + '0';
	if (board[y][x]->znak == '.') {
		if (validate(x, y, random_znak)) {
			board[y][x]->setchar_perm(random_znak);
			return true;
		}
		random_znak = random_znak == '0' ? '1' : '0';
		if (validate(x, y, random_znak)) {
			board[y][x]->setchar_perm(random_znak);
			return true;
		}
	}
	return false;
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
		if (prob_nieudanych >= maxFillingTries)
			return;
		if (udanych >= randomFieldsToFill)
			return;
	}
}

void Matrix::clear(int x, int y) {
	board[y][x]->setchar('.');
}