#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

using namespace std;

/*#include <iostream>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
*/

template<class T>
struct node{
	node* next;
	node* prev;
	T data;
	bool empty;
};

template<class T>
class LinkedList {
public:
	LinkedList();
	~LinkedList();
	LinkedList create();
	void addFront(T data);
	void addBack(T data);
	void removeFront();
	void removeBack();
	void reset();
	void remove(node<T>* item);
	node<T>* get(int t);
	node<T>* getTail();
	void print();
	bool empty();
	bool Tempty();
	bool Hempty();
	int getLength();

private:
	node<T> *head;
	node<T> *tail;
	node<T> * NULL0 = 0;
};

template<class T>
LinkedList<T>::LinkedList(){
	head = new node<T>;
	tail = new node<T>;
	head->prev = tail;
	head->next = NULL0;

	tail->prev = NULL0;
	tail->next = head;

	tail->empty = true;
	head->empty = true;
}

template<class T>
void LinkedList<T>::addFront(T data){
	if (head->empty == true){
		head->data = data;
		head->empty = false;
		//cout << "added " << data << " to head" << endl;
	}
	else if (tail->empty == true){
		T tmp = data;
		tail->data = head->data;
		tail->empty = false;
		head->data = tmp;
		//cout << "added " << data << " to tail" << endl;
	}
	else{
		node<T> *temp = new node<T>;
		temp->data = head->data; 
		head->prev->next = temp;

		temp->prev = head->prev;
		temp->next = head;

		head->data = data;
		head->prev = temp;

		temp->empty = false;

		//cout << "added " << data << " to head of length >= 2\t" << head->data << " " << head->prev->data << " " << tail->next->data << " " << tail->data << endl;
	}
}

template<class T>
void LinkedList<T>::removeFront(){
	if (head->empty == true && tail->empty == true){
		//cout << "already empty" << endl;
	}
	else if(head->prev != tail){
		//cout << "removing front" << endl;
		//cout << head->prev->data << endl;
		node<T> *temp = head;
		head = head->prev;
		delete temp;
	}
	else if (head->prev == tail){
		if (head->empty != true){
			head->empty = true;
		}
		else if (head->empty == true){
			tail->empty = true;
		}
		
	}
	else{
		//cout << "huh?" << endl;
	}
}

template<class T>
void LinkedList<T>::addBack(T data){
	if (tail->empty == true){
		tail->data = data;
		tail->empty = false;
		//cout << "added " << data << " to tail" << endl;
	}
	else if (head->empty == true){
		T tmp = data;
		head->data = tail->data;
		head->empty = false;
		tail->data = tmp;
		//cout << "added " << data << " to head" << endl;
	}
	else{
		node<T> *temp = new node<T>;
		tail->next->prev = temp;
		temp->data = tail->data;
		temp->next = tail->next;
		temp->prev = tail;

		tail->data = data;
		tail->next = temp;
		temp->empty = false;

		//cout << "added " << data << " to tail of length >= 2" << endl;
	}
}

template<class T>
void LinkedList<T>::removeBack(){
	//add check for head!!
	if (head->empty == true && tail->empty == true){
		//cout << "already empty" << endl;
	}
	else if (tail->next != head){
		//cout << "removing back" << endl;
		//cout << tail->next->data << endl;
		node<T> *temp = tail;
		tail = tail->next;
		temp->empty = true;
		delete temp;
	}
	else if (tail->next == head){
		if (tail->empty != true){
			tail->empty = true;
		}
		else if (tail->empty == true){
			head->empty = true;
		}

	}
	else{
		//cout << "huh?" << endl;
	}
}

template<class T>
void LinkedList<T>::remove(node<T>* item){
	if (item == head){
		removeFront();
	}
	else if (item == tail){
		removeBack();
	}
	else{
		if (item->next != NULL0){
			item->next->prev = item->prev;
		}
		if (item->prev != NULL0){
			item->prev->next = item->next;
		}
		//cout << "Removed item: \t" << item->data << "\t" << item << endl;
		delete(item);
	}
}

template<class T>
void LinkedList<T>::reset(){
	while (get(1) != tail){
		removeFront();
	}
	if(head->empty){
		removeFront();
	}
	if(tail->empty){
		removeBack();
	}

	if(head->empty){
		removeFront();
	}
	if(tail->empty){
		removeBack();
	}
}

template<class T>
int LinkedList<T>::getLength(){
	int i = 0;
	node<T>* tmp = head;
	if(head->empty){
		if(tail->empty){
			return 0;
		} else{
			return 1;
		}
	}
	while(tmp != tail){
		tmp = tmp->prev;
		i++;
	}
	return i;
}

template<class T>
node<T>* LinkedList<T>::get(int t){
	int j = t;
	node<T>* tmp = head;
	while (t > 0) {
		tmp = tmp->prev;
		t--;
	}
	//cout << "got " << tmp->data << " from position " << j << endl;
	return tmp;
}

template<class T>
node<T>* LinkedList<T>::getTail(){
	node<T>* tmp = tail;
	return tmp;
}

template<class T>
bool LinkedList<T>::empty(){
	if(head->empty == true && tail->empty == true){
		return true;
	} else{
		return false;
	}
}

template<class T>
bool LinkedList<T>::Hempty(){
	return head->empty;
}
template<class T>
bool LinkedList<T>::Tempty(){
	return tail->empty;
}

/*
template<class T>
void LinkedList<T>::print(){
	//cout << "Printing..." << head->data << endl;
	////cout << tail->data << "\t" << tail->next << "\t" << tail->prev << endl;
	////cout << "..." << endl;
	if (head->empty != true){
		node<T> *temp = head;
		//cout << head->data << "\t" << head << "\tHead" << endl;
		////cout << temp->data << "\t" << temp << endl;
		while (temp->prev != NULL0){
			temp = temp->prev;
			if (temp == tail){
				//cout << temp->data << "\t" << temp << "\tTail" << endl;
				break;
			}
			else{
				//cout << temp->data << "\t" << temp << endl;
			}
		}
		////cout << tail->data << "\t" << tail << "\tTail" << endl;
	}
	else if(tail->empty != true){
		//cout << tail->data << " " << head->data << endl;
	}
	else{
		//cout << "Empty List" << endl;
	}
}
*/

template<class T>
LinkedList<T>::~LinkedList(){
	reset();
	delete head;
	delete tail;
}

#endif // __LINKED_LIST_H__
