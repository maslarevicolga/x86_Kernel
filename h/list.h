/*
 * list.h
 *
 *  Created on: Mar 28, 2020
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_
#include <iostream.h>
template<class T>
class List {
public:

	struct Node {
		T data;
		Node* next;
		Node(T info, Node* nxt = 0):data(info),next(nxt){}
	};

	int id;
    static int ID;
	Node* head, *tail;
	unsigned size;

	List():head(0), tail(0), size(0), id(++ID){}
    List(const List<T>& l): head(0), tail(0), size(0){ copy(l); }
    ~List(){ cout << "del listd" << id << "\n"; deleteList(); }
	Node* begin() const { return head; }
	Node* end() const { return tail; }
	Node* begin() { return head; }
	Node* end() { return tail; }

	unsigned getSize() const{ return size; }
	unsigned isEmpty() {return (size == 0)? 1 : 0; }


	volatile void pushBack(T info){
		Node* newNode = new Node(info);
		if(head == 0) head = newNode;
		else tail->next = newNode;
		tail = newNode;
		size++;
	}
	void pushFront(const T& info){
		Node* newNode = new Node(info);
		newNode->next = head;
		if(head == 0) tail = newNode;
		head = newNode;
		size++;
	}
	void pushAfter(Node* node, T data){
		Node* newNode = new Node(data);
		if(node == 0 && head == 0) { head = tail = newNode; size++; return; }
		if(node == 0) return;
		newNode->next = node->next;
		node->next = newNode;
		if(newNode->next == 0) tail = newNode;
        size++;
	}

	T popFront(){
		if(head == 0){ return 0; }
		T ret = head->data;
		Node* del = head;
		head = head->next;
		del->next = 0;
		if(head == 0) tail = 0;
		size--;
		delete del;
		return ret;
	}

	void remove(T node){
		Node* curr = head, *prev = 0;
		while(curr != 0 && curr->data != node){
			prev = curr;
			curr = curr->next;
		}
		if(curr == 0) return;
		if(curr == head) this->popFront();
		else{
			prev->next = curr->next;
			curr->next = 0;
			delete curr;
		}
	}


	Node* find(const T& el) {
		for(Node* node = head; node != 0; node = node->next)
			if(node->data == el)
				return node;
		return 0;
	}
	void swap(Node* el1, Node* el2){
		if(el1 == 0 || el2 == 0) return;
		Node* tmp = el1;
		el1 = el2;
		el2 = tmp;
	}

	void clear() {
		deleteList();
	}

protected:

	void deleteList(){
		Node* old = 0, *curr = head;
		while(curr != 0){
			old = curr;
			curr = curr->next;
			old->next = 0;
			delete old;
		}
		head = tail = 0;
		size = 0;
	}

	void copy(const List<T>& l){
		for(Node* curr = l.head; curr; curr = curr->next){
			pushBack(curr->data);
		}
	}

};


#endif /* H_LIST_H_ */
