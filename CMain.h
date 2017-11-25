#ifndef CMAIN_H
#define CMAIN_H
class Matrix;
class CMain {
	
	int znak;	// odbierany znak
	int consoleX, consoleY;	// pozycja kursora
	int startX, startY;	// pozycja startowa planszy (gdzie j� rysowa�)
	int textCol, bgCol;	//kolor tekstu, kolor t�a
	int zero;	// czy pierwszy pobrany znak by� zerem
	char txt[32];
	Matrix* m;
	
public:
	CMain();
	~CMain();
	int run();
	void printMenu(int x, int y);
	void printBorder();
};

#endif // !CMAIN_H