#include <FreeImage.h>
#include <unistd.h>
#include <iostream>

class List
{
public:
	List();

	bool isLastNode ();
	bool isFirstNode();

	void front();
	void back();

	void push(int height);
	void pop();

private:
	typedef struct Node
	{
		
		struct Node* prev;
		struct Node* next;

		RGBQUAD *data;

	} Node ;

	typedef Node* llist;

	llist ma_liste = NULL;

	Node *first;
};