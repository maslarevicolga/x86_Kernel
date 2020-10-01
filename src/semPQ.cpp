/*
 * semPQ.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */
#include "semPQ.h"
#include "SCHEDULE.h"
#include "kerSem.h"
#include "pcb.h"

SemPQueue::SemPQueue(){
    head = 0;
    size = 0;
}

SemPQueue::~SemPQueue(){
	Elem *t = head;
	while(head != 0){
		t = head;
		head = head->next;
		delete t;
	}
	size = 0;
	head = 0;
}


void SemPQueue::push(PCB* pcb, Time t){
    Elem* new_el = new Elem();
    new_el->next = 0;
    new_el->pcb = pcb;
    new_el->time = t;
    Elem* curr = head;
    Elem* prev = 0;
    if(head == 0)
        head = new_el;
    else{
        while(curr != 0){
            if(new_el->time >= curr->time){
                prev = curr;
                curr = curr->next;
            }else{
                new_el->next = curr;
                if(prev == 0)
                    head = new_el;
                else
                    prev->next = new_el;
                break;
            }
        }
        if(curr == 0){
            prev->next = new_el;
            new_el->next = 0;
        }
    }
    size++;
}

PCB* SemPQueue::getPop(){
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


unsigned SemPQueue::wake(Time currTime){
    if(head == 0)
        return 0;
    unsigned cnt = 0;
    while(head->time <= currTime){
        size--;
    	Elem* temp = head;
        head = head->next;
        temp->pcb->state = READY;
        temp->pcb->unblockedByTime = 1;
        Scheduler::put(temp->pcb);
        cnt++;
        temp->next = 0;
        delete temp;
        //temp = 0;
        if(head == 0)
            break;
    }
    return cnt;
}

unsigned SemPQueue::wakeAll(){
    Elem *temp = head;
    unsigned cnt = 0;
	while(head != 0){
		temp = head;
		head = head->next;
        temp->pcb->state = READY;
        temp->pcb->unblockedByTime = 0;
        Scheduler::put(temp->pcb);
        cnt++;
        delete temp;
        temp = 0;
	}
	return cnt;
}



