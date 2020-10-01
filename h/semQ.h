/*
 * semQ.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#ifndef H_SEMQ_H_
#define H_SEMQ_H_

#include "pcb.h"

class SemQueue{
	struct Elem{
		PCB *pcb;
		Elem *next;
	};
	Elem *head, *tail;
	int size;
public:
	SemQueue();
	~SemQueue();
	void push(PCB* thread);
	PCB* getPop();
	int getSize() const { return size; }
};



#endif /* H_SEMQ_H_ */
