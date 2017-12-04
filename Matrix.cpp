#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h> 
#include <time.h>
#include <stdlib.h>

#include "Matrix.h"
#include "Error.h"

Matrix::Matrix() { }

Matrix::Matrix(int size) {
	highlightNotFillable = false;
	setSize(size);

	char filename[] = "plansza00x00.txt";
	char* row;
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
		board = new Field**[size];
		for (int i = 0; i < size; i++) {
			board[i] = new Field*[size];
		}


		row = new char[size + 1];
		for (int i = 0; i < size; i++) {
			fread(row, 1, size + 1, file);
			for (int j = 0; j < size; j++) {
				board[i][j] = new Field(j, i, row[j]);
			}
		}
		fclose(file);
	}
	else {
		board = new Field**[size];
		for (int i = 0; i < size; i++) {
			board[i] = new Field*[size];
		}


		row = new char[size + 1];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				board[i][j] = new Field(j, i, '.');
			}
		}
		for (int i = 0; i < size; i++) {
			fillRandomFields();
		}
	}
}

Matrix::Matrix(int size, bool a) {
	this->size = size;
	board = new Field**[size];
	for (int i = 0; i < size; i++) {
		board[i] = new Field*[size];
	}


	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = new Field(j, i);
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

bool Matrix::validate(int x, int y, char symbol) {
	ErrorList* list = new ErrorList;
	Field *a = board[y][x];
	if (a->preset)

		return false;
	// zasada 1)
	switch (0) {
	case 0:
		if (y > 0)
			if (board[y - 1][x]->symbol == symbol) {
				if (y > 1 && board[y - 2][x]->symbol == symbol) {
					list->add(new Error("Naruszenie pierwszej zasady"));
					return false;
				}
				if (y < size - 1)
					if (board[y + 1][x]->symbol == symbol) {
						list->add(new Error("Naruszenie pierwszej zasady"));
						return false;

					}
			}
	case 1:
		if (y < size - 1)
			if (board[y + 1][x]->symbol == symbol) {
				if (y < size - 2 && board[y + 2][x]->symbol == symbol) {
					list->add(new Error("Naruszenie pierwszej zasady"));

					return false;
				}
			}
	case 2:
		if (x > 0)
			if (board[y][x - 1]->symbol == symbol) {
				if (x > 1 && board[y][x - 2]->symbol == symbol) {
					list->add(new Error("Naruszenie pierwszej zasady"));
					return false;
				}
				if (x < size - 1)
					if (board[y][x + 1]->symbol == symbol) {
						list->add(new Error("Naruszenie pierwszej zasady"));
						return false;
					}
			}
	case 3:
		if (x < size - 1)
			if (board[y][x + 1]->symbol == symbol) {
				if (x < size - 2 && board[y][x + 2]->symbol == symbol) {
					list->add(new Error("Naruszenie pierwszej zasady"));
					return false;
				}
			}
	}
	// zasada 2)
	int sumx = 0,
		sumy = 0;
	for (int i = 0; i < size; i++) {

		if (board[y][i]->symbol == symbol) { // sprawdzanie rzêdu
			sumy++;
		}
		if (board[i][x]->symbol == symbol) { // sprawdzanie kolumny
			sumx++;
		}
		if (sumx >= size / 2 || sumy >= size / 2) {
			list->add(new Error("Naruszenie drugiej zasady"));
			return false;
		}
	}
	// zasada 3) // rzêdy nie mog¹ byæ takie same
	int sum = 0,
		sum_compare = 0;
	if (isCompleted(x, y, 'x')) { // sprawdzanie rzêdów
		sum = getSupposedValue(x, y, 'x', symbol);
		for (int i = 0; i < size; i++) {
			if (i != x)
				if (isCompleted(x, i, 'x')) {
					sum_compare = getValue(x, i, 'x');
					if (sum_compare == sum) {
						list->add(new Error("Naruszenie trzeciej zasady"));
						return false;
					}
				}
		}
	}
	sum_compare = sum = 0;
	if (isCompleted(x, y, 'y')) { // sprawdzenie kolumn
		sum = getSupposedValue(x, y, 'y', symbol);
		for (int i = 0; i < size; i++) {
			if (isCompleted(i, y, 'y')) {
				sum_compare = getValue(i, y, 'y');
				if (sum_compare == sum) {
					list->add(new Error("Naruszenie trzeciej zasady"));
					return false;
				}
			}
		}
	}
	delete list;
	//return list
	return true;
}

void Matrix::printMatrix(int x, int y) {
	Field* p;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p = board[i][j];
			if (p->preset) {
				/*textbackground(WHITE);*/
				textcolor(LIGHTGREEN);
			}
			else if (highlightNotFillable && !(validate(j, i, '0') || validate(j, i, '1'))) {
				textbackground(CYAN);
			}
			p->write(x, y);
			//textbackground(BLACK);
			textcolor(WHITE);
			textbackground(BLACK);
		}
	}

}

bool Matrix::setChar(int x, int y, char c) {
	if (validate(x, y, c)) {
		board[y][x]->setChar(c);
		return true;
	}
	return false;
}

bool Matrix::isCompleted(int x, int y, char c) {
	// TODO: naprawiæ funkcjê, ¿eby sensowenie dzia³a³a
	if (c == 'x')
		for (int i = 0; i < size; i++) {
			if (board[i][x]->symbol == '.') {
				return false;
			}
		}
	if (c == 'y')
		for (int i = 0; i < size; i++) {
			if (board[y][i]->symbol == '.') {
				return false;
			}
		}
	return true;
}

int Matrix::getValue(int x, int y, char c) {
	int sum = 0;
	int mnoznik = 1;
	if (c == 'x') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			sum += (board[y][i]->symbol - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			sum += (board[i][x]->symbol - '0')*mnoznik;
		}
	}
	return sum;
}

int Matrix::getSupposedValue(int x, int y, char c, char input) {
	int sum = 0;
	int mnoznik = 1;
	if (c == 'x') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			if (i == x)
				sum += input - '0';
			else
				sum += (board[y][i]->symbol - '0')*mnoznik;
		}
	}
	if (c == 'y') {
		for (int i = 0; i < size; i++, mnoznik *= 2) {
			if (i == y)
				sum += input - '0';
			else
				sum += (board[i][x]->symbol - '0')*mnoznik;
		}
	}
	return sum;
}

bool Matrix::fillRandomField() {
	int x = (int)rand() % size;
	int y = (int)rand() % size;
	int random_znak = rand() % 2 + '0';
	if (board[y][x]->symbol == '.') {
		if (validate(x, y, random_znak)) {
			board[y][x]->setPresetChar(random_znak);
			return true;
		}
		random_znak = random_znak == '0' ? '1' : '0';
		if (validate(x, y, random_znak)) {
			board[y][x]->setPresetChar(random_znak);
			return true;
		}
	}
	return false;
}
void Matrix::fillRandomFields() {
	int prob_nieudanych = 0;
	int udanych = 0;
	for (int i = 0; i < size; i++) {
		if (fillRandomField()) {
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
	board[y][x]->setChar('.');
}