#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "conio2.h"
#include "Field.h"

class Matrix {
private:
	int size = 0;
	const int randomFieldsToFill = 5;
	const int maxFillingTries = 50;
	int* sum0_x;
	int* sum1_x;
	int* sum0_y;
	int* sum1_y;
public:
	Field*** board;

	int getSize();
	void setSize(int size);
	bool highlightNotFillable;
	bool highlightUnambiguous;

public:
	Matrix();
	Matrix(int size);
	Matrix(int size, bool a);
	~Matrix();

	bool validate(int x, int y, char symbol);
	void printMatrix(int x, int y);
	bool setChar(int x, int y, char c);
	bool isCompleted(int x, int y, char c);
	int getValue(int x, int y, char c);
	int getSupposedValue(int x, int y, char c, char input);
	bool fillRandomField();
	void fillRandomFields();
	void clear(int x, int y);
	void fillUnambiguous();
	void updateSizeArrays();
	
};
#endif // !MATRIX_H