#ifndef POLE_H
#define POLE_H
#include "conio2.h"
#include <stdio.h>
#include "Point.h"
class Pole {
public:
	char znak;
	Point p;
	bool preset;
	Pole(int x, int y) : p(x, y) {
		znak = '.';
		preset = false;
	}
	Pole(int x, int y, char zn) : Pole(x, y) {
		znak = zn;
		preset = zn == '.' ? false : true;
	}
	void write() {
		gotoxy(p.x + 2, p.y + 2);
		putchar(znak);
	}
	void write(int x, int y) {
		gotoxy(p.x + x, p.y + y);
		putchar(znak);
	}

	void setchar(char c) {
		if (preset == false) {
			znak = c;
		}
	}
	void setchar_perm(char c) {
		if (preset == false) {
			znak = c;
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