#ifndef __X_H_INCLUDED__
#define __X_H_INCLUDED__   

#include <unistd.h>
#include <iostream>

extern int size; 

template<typename T>
class LinkedList
{
public:

 	typedef struct Node
	{
		
		struct Node* prev;
		struct Node* next;

		T* data;

	} Node ;

	typedef Node* llist;

	LinkedList(){
	}

	bool isLastNode ();
	bool isFirstNode();

	llist front(){
		return ma_liste;
	}
	llist back(){
		return first;
	}

	llist push(int height){
		
		Node *newNode = (Node*) malloc(sizeof(Node) );
	if(!newNode){
		exit(EXIT_FAILURE);
	}
	
	newNode->data = (T*) malloc(sizeof(T)*height);
	newNode->prev = ma_liste;
	
	if(ma_liste != NULL){ 
		
		if(ma_liste->prev == NULL){
			first = newNode;
			newNode->next = NULL;

		}
		else {
			ma_liste->next = newNode;
		}
	
	}
	else {
		first = newNode;
		newNode->next = NULL;
	}

	ma_liste = newNode;

	

	return ma_liste;
	}

	void pop(){
		Node *tmp = first->next;
	delete(first);
	first = tmp;

	}
	void recycle(){
			ma_liste->next = first;
	first = first->next;

	ma_liste->next->next = NULL;
	ma_liste->next->prev = ma_liste;
	ma_liste = ma_liste->next;
	}

	Node *first;
private:

	llist ma_liste = NULL;

	
};

#endif