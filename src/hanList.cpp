/*
 * hanList.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */


#include "hanList.h"

HandlerList::HandlerList(){
	tail = head = 0;
}

HandlerList::~HandlerList(){
    pop();
}

void HandlerList::push(SignalHandler handler){
	if(head == 0)
		tail = head = new Elem();
	else
		tail = tail->next = new Elem();
    tail->handler = handler;
	tail->next = 0;
}

void HandlerList::pop() {
	Elem *t = head;
	while(t != 0) {
		t = head;
		head = head->next;
		delete t;
	}
	head = tail = 0;
}


void HandlerList::call(){
    Elem* t = head;
    while(t != 0) {
        (t->handler)();
        t = t->next;
    }
}

void HandlerList::swap(SignalHandler hand1, SignalHandler hand2){
    Elem *el1 = 0, *el2 = 0;
    Elem* t = head;
    if(hand1 == hand2)
        return;
    while(t != 0) {
        if(t->handler == hand1 || t->handler == hand2) {
            el1 = t;
            t = t->next;
            break;
        }
        t = t->next;
    }
    if(el1 == 0)
        return;
    while(t != 0) {
        if((t->handler == hand1 || t->handler == hand2) && t->handler != el1->handler){
            el2 = t;
            break;
        }
        t = t->next;
    }
    if(el2 == 0)
        return;
    SignalHandler temp = el1->handler;
    el1->handler = el2->handler;
    el2->handler = temp;
}

HandlerList* HandlerList::copy() const {
    HandlerList* rez = new HandlerList();
    Elem* t = head;
    while(t != 0){
        rez->push(t->handler);
        t = t->next;
    }
    return rez;
}

