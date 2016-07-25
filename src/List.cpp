#include "List.h"

List::List(){
	//push();
	std::cout << "liste créée" << std::endl;
}

void List::push(int height){
	Node *newNode = (Node*) malloc(sizeof(Node));
	if(!newNode){
		exit(EXIT_FAILURE);
	}
	newNode->prev = ma_liste;
	
	//ma_liste = newNode;	
	newNode->data = (RGBQUAD*) malloc(sizeof(RGBQUAD*)*height);

	std::cout << sizeof newNode->data << std::endl;
	ma_liste = newNode;

	if(first == NULL){
		first = newNode;
	}
}
