/*
 * pcbList.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#include "pcbList.h"

PcbList::PcbList(){
	head = tail = 0;
}

PcbList::~PcbList(){
	Elem *t = head;
	while(head != 0) {
		t = head;
		head = head->next;
		delete t;
	}
	tail = head = 0;
}

void PcbList::push(PCB* pcb){
	if(head == 0)
		tail = head = new Elem();
	else
		tail = tail->next = new Elem();
    tail->pcb = pcb;
	tail->next = 0;
}

PcbList::Iterator PcbList::getIterator(){
    return PcbList::Iterator(this);
}

PcbList::Iterator::Iterator(PcbList* pl) {
    l = pl;
    prevElem = 0;
    currElem = pl->head;
}

void PcbList::Iterator::pop() {
    Elem *t = currElem;
    if (t == 0)
        return;
    if (prevElem != 0)
        prevElem->next = currElem->next;
    else
    	l->head = currElem->next;

    currElem = currElem->next;
    if (currElem == 0)
    	l->tail = prevElem;
    delete t;
}

PCB* PcbList::Iterator::get(){
    if(currElem == 0)
        return 0;
    else
        return currElem->pcb;
}

void PcbList::Iterator::next(){
    if (currElem != 0){
        prevElem = currElem;
        currElem = currElem->next;
    }
}

unsigned PcbList::Iterator::end(){
    return currElem == 0;
}



