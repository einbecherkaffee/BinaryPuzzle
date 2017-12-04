#define _CRT_SECURE_NO_WARNINGS
#ifndef ERROR_H
#define ERROR_H
#include <stdio.h>
#include <string.h>
class Error {
	char* komunikat;
	int length;
public:
	Error(char* c) {
		length = strlen(c);
		komunikat = new char[length + 1];
		int i;
		for (i = 0; i < length; i++) {
			komunikat[i] = c[i];
		}
		komunikat[i] = '\0';
	}
	char* peek() {
		return komunikat;
	}
	~Error() {
		delete komunikat;
	}
};

class ErrorList {
	class ErrorContainer {
	public:
		Error* e;
		ErrorContainer* next;
		ErrorContainer(Error* er) {
			e = er;
			next = NULL;
		}
		~ErrorContainer() {
			delete e;
		}
	};

	bool _empty;
	ErrorContainer* head;
	ErrorContainer* tail;
public:
	ErrorList() {
		head = tail = NULL;
		_empty = true;
	}
	~ErrorList() {
		ErrorContainer* tmp;
		if (head != NULL) {
			if (head == tail) {
				delete head;
			}
			else {
				do {
					tmp = head;
					head = head->next;
					delete tmp;
				} while (head != tail);
				delete head;
				head = tail = NULL;

			}
		}
	}

	void add(Error* e) {
		_empty = false;
		if (head == NULL) {
			head = tail = new ErrorContainer(e);
		}
		else {
			tail->next = new ErrorContainer(e);
			tail = tail->next;
		}

	}
	char* peek() {
		char* tmp = head->e->peek();
		return tmp;
	}
	void pop() {
		if (head != NULL) {
			ErrorContainer* tmp = head;
			head = head->next;
			delete tmp;
		}
		else {
			_empty = true;
		}
	}
	bool empty() {
		return _empty;
	}
};
#endif // !ERROR_H
