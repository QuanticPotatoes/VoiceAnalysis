/*#include "LinkedList.h"

template<class T>
LinkedList<T>::LinkedList(){
	//push();
	std::cout << "liste créée" << std::endl;
}

template<class T>
void LinkedList<T>::add(int height){
	Node *newNode = (Node*) malloc(sizeof(Node) );
	if(!newNode){
		exit(EXIT_FAILURE);
	}
	
	newNode->data = (RGBQUAD*) malloc(sizeof(RGBQUAD)*height);
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
}

template<class T>
void LinkedList<T>::pop(){

	Node *tmp = first->next;
	delete(first);
	first = tmp;

}

template<class T>
typename LinkedList<T>::llist LinkedList<T>::front(){
	return ma_liste;
}

template<class T>
typename LinkedList<T>::llist LinkedList<T>::back(){
	return first;
}

/**
 * @brief Take the begin of the matrix and sends it to the end of itself.
 * @details [long description]
 
 
template<class T>
void LinkedList<T>::recycle(){

	ma_liste->next = first;
	first = first->next;

	ma_liste->next->next = NULL;
	ma_liste->next->prev = ma_liste;
	ma_liste = ma_liste->next;
}
 */