/*
 * signalQ.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#include "signalQ.h"
#include "pcb.h"


SignalQueue::SignalQueue(){
	tail = head= 0;
}

SignalQueue::~SignalQueue(){
	Elem *t = head;
	while(head != 0){
		t = head;
		head = head->next;
		delete t;
	}
	tail = head = 0;
}

void SignalQueue::push(SignalId signal){
	if(head == 0){
		tail = head = new Elem();
	}
	else{
		tail = tail->next = new Elem();
	}
	tail->signal = signal;
	tail->next = 0;
}

SignalId SignalQueue::getPop() {
	if(head == 0)
		return numOfSignals;
	SignalId sig = head->signal;
	Elem *t = head;
	head = head->next;
    if(head == 0){
        tail = 0;
    }
	delete t;
	return sig;
}

SignalId SignalQueue::getFirstSignal(){
	if(head == 0)
		return numOfSignals;
	SignalId sig = head->signal;
	return sig;
}

unsigned SignalQueue::isEmpty(){
    return head == 0;
}

SignalQueue::Iterator SignalQueue::getIterator(){
    return SignalQueue::Iterator(this);
}

SignalQueue::Iterator::Iterator(SignalQueue*sq){
    q = sq;
    prevElem = 0;
    currElem = sq->head;
}

void SignalQueue::Iterator::pop(){
    Elem *t = currElem;
    if (t == 0)
        return;
    if (prevElem != 0) {
        prevElem->next = currElem->next;
    }else{
    	q->head = currElem->next;
    }
    currElem = currElem->next;
    if (currElem == 0)
    	q->tail = prevElem;
    delete t;
}

SignalId SignalQueue::Iterator::get(){
    if(currElem == 0)
        return numOfSignals;
    else
        return currElem->signal;
}

void SignalQueue::Iterator::next(){
    if (currElem != 0){
        prevElem = currElem;
        currElem = currElem->next;
    }
}

unsigned SignalQueue::Iterator::end(){
    return currElem == 0;
}


