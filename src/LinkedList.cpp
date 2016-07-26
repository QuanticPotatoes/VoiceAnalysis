#include "LinkedList.h"

LinkedList::LinkedList(){
	//push();
	std::cout << "liste créée" << std::endl;
}

void LinkedList::add(int height){
	Node *newNode = (Node*) malloc(sizeof(Node) );
	if(!newNode){
		exit(EXIT_FAILURE);
	}
	
	newNode->data = (RGBQUAD*) malloc(sizeof(RGBQUAD)*height);
	newNode->prev = ma_liste;
	
	newNode->test = 0;

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
}

void LinkedList::pop(){

	Node *tmp = first->next;
	delete(first);
	first = tmp;
	std::cout << first->test << std::endl;
}

LinkedList::llist LinkedList::front(){
	return ma_liste;
}

LinkedList::llist LinkedList::back(){
	return first;
}

/**
 * @brief Take the begin of the matrix and sends it to the end of itself.
 * @details [long description]
 */
 
void LinkedList::recycle(){

	ma_liste->next = first;
	first = first->next;

	ma_liste->next->next = NULL;
	ma_liste->next->prev = ma_liste;
	ma_liste = ma_liste->next;
}
