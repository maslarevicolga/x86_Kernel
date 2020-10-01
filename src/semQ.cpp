/*
 * semQ.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */
#include "semQ.h"


SemQueue::SemQueue(){
	head = tail = 0;
	size = 0;
}

SemQueue::~SemQueue(){
	Elem *t = head;
	while(head != 0){
		t = head;
		head = head->next;
		delete t;
	}
	size = 0;
	head = tail = 0;
}

void SemQueue::push(PCB* pcb){
	Elem* new_el = new Elem();
	new_el->pcb = pcb;
	new_el->next = 0;
	if(head == 0){
		head = tail = new_el;
	}
	else{
		tail = tail->next = new_el;
	}
	size++;
}

PCB* SemQueue::getPop(){
	if(head == 0)
		return 0;
	Elem *p = head;
	head = p->next;
	PCB* ret = p->pcb;
	p->next = 0;
	delete p;
	size--;
	return ret;
}



