/*
 * semList.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */
#include "semList.h"
#include "kerSem.h"
#include <iostream.h>

// da imam promenljivu u kojoj cuvam numTicks i onda to postavim semaforima u trenutnku kad ih budim

SemList::SemList() {
	head = tail = 0;
}

SemList::~SemList() {
	Elem *t = head;
	while(head != 0){
		t = head;
		head = head->next;
		delete t;
	}
	tail = head = 0;
}

void SemList::push(KernelSem* ksem) {

#ifndef BCC_BLOCK_IGNORE
	Elem* el = new Elem();
	disableInterrupts

	if(head == 0)
		tail = head = el;
	else
		tail = tail->next = el;

    tail->ksem = ksem;
	tail->next = 0;
	enableInterrupts
#endif
}

SemList::Iterator SemList::getIterator(){
    return SemList::Iterator(this);
}

SemList::Iterator::Iterator(SemList* ksl){
    l = ksl;
    prevElem = 0;
    currElem = ksl->head;
}

void SemList::Iterator::pop(){

	Elem *t = currElem;
    if (t == 0)
        return;
#ifndef BCC_BLOCK_IGNORE
	disableInterrupts
    if (prevElem != 0)
        prevElem->next = currElem->next;
    else
    	l->head = currElem->next;

    currElem = currElem->next;
    if (currElem == 0)
    	l->tail = prevElem;
    t->next = 0;
    enableInterrupts
#endif
    delete t;
}

KernelSem* SemList::Iterator::get(){
    if(currElem == 0)
        return 0;
    else
        return currElem->ksem;
}

void SemList::Iterator::next(){
    if (currElem != 0){
        prevElem = currElem;
        currElem = currElem->next;
    }
}

unsigned SemList::Iterator::end(){
    return currElem == 0;
}



