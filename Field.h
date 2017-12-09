#ifndef POLE_H
#define POLE_H
#include "conio2.h"
#include <stdio.h>
#include "Point.h"
class Field {
public:
	char symbol;
	Point p;
	bool preset;
	Field(int x, int y) : p(x, y) {
		symbol = '.';
		preset = false;
	}
	Field(int x, int y, char zn) : Field(x, y) {
		symbol = zn;
		preset = zn == '.' ? false : true;
	}
	void write(int x, int y) {
		gotoxy(p.x + x, p.y + y);
		putchar(symbol);
	}

	void setChar(char c) {
		if (preset == false) {
			symbol = c;
		}
	}
	void setPresetChar(char c) {
		if (preset == false) {
			symbol = c;
			preset = true;
		}
	}
	int x() {
		return p.x;
	}
	int y() {
		return p.y;
	}

};
#endif // !POLE_H