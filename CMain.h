#ifndef CMAIN_H
#define CMAIN_H
class Matrix;
class CMain {
	int posx, posy;			// pozycja kursora na planszy
	int znak;				// odbierany znak
	int consoleX, consoleY;	// pozycja kursora w konsoli
	int startX, startY;		// pozycja startowa planszy (gdzie j¹ rysowaæ)
	int textCol, bgCol;		//kolor tekstu, kolor t³a
	int zero;				// czy pierwszy pobrany znak by³ zerem
	char txt[32];
	Matrix* m;
	
public:
	CMain();
	~CMain();
	int run();
	void printMenu(int x, int y);
	void printBorder();
	int getUserInput();
	void startNewGame();
	void save();
	void load();
};

#endif // !CMAIN_H
