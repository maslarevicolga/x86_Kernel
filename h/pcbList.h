/*
 * pcbList.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */


#ifndef PCB_LIST_H_
#define PCB_LIST_H_

#include <iostream.h>
class PCB;

class PcbList{
	struct Elem{
		PCB* pcb;
		Elem *next;
		Elem() {
			next = 0;
			pcb = 0;
		}
	};

public:
	Elem *head, *tail;
	PcbList();
	~PcbList();
    void push(PCB* ksem);

    struct Iterator {
        private:
    	    PcbList *l;
            Elem *currElem;
            Elem *prevElem;
        public:
            Iterator(PcbList *);
            void pop();
            PCB* get();
            void next();
            unsigned end();
    };

    Iterator getIterator();
};

#endif /* H_LIST_H_ */




