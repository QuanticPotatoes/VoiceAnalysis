#include <FreeImage.h>
#include <unistd.h>
#include <iostream>

class LinkedList
{
public:

 	typedef struct Node
	{
		
		struct Node* prev;
		struct Node* next;

		RGBQUAD *data;
		int test;

	} Node ;

	typedef Node* llist;

	LinkedList();

	bool isLastNode ();
	bool isFirstNode();

	llist front();
	llist back();

	void add(int height);
	void pop();
	void recycle();

	Node *first;
private:

	llist ma_liste = NULL;

	
};