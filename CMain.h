#ifndef CMAIN_H
#define CMAIN_H
class Matrix;
class CMain {
	int posx, posy;			// pozycja kursora na planszy
	int symbol;				// odbierany znak
	int consoleX, consoleY;	// pozycja kursora w konsoli
	int startX, startY;		// pozycja startowa planszy (gdzie j� rysowa�)
	int textCol, bgCol;		//kolor tekstu, kolor t�a
	int zero;				// czy pierwszy pobrany znak by� zerem
	char txt[32];
	Matrix* m;
	
public:
	CMain();
	~CMain();
	int run();
	void printMenu(int x, int y);
	void printBorder(int x1, int y1, int x2, int y2);
	void printKey(int x, int y);
	int getUserInput();
	void startNewGame();
	void save();
	void load();
};

#endif // !CMAIN_H
