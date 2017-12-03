#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "conio2.h"
#include "Pole.h"

class Matrix {
private:
	int size = 0;
	const int randomFieldsToFill = 5;
	const int maxFillingTries = 50;
public:
	Pole*** board;	// TODO: doda� getter

	int getSize();
	void setSize(int size);

public:
	Matrix();
	Matrix(int size);
	Matrix(int size, bool a);
	~Matrix();

	bool validate(int x, int y, char znak);
	void printMatrix(int x, int y);
	bool setchar(int x, int y, char c);
	bool completed(int x, int y, char c);
	int get_value(int x, int y, char c);
	int get_supposed_value(int x, int y, char c, char input);
	bool fill_random_pole();
	void fill_random_pola();
	void clear(int x, int y);
	
};
#endif // !MATRIX_H