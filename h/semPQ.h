/*
 * semPQ.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#ifndef H_SEMPQ_H_
#define H_SEMPQ_H_

#include "pcb.h"

class SemPQueue{
  struct Elem {
    Time time;
    PCB* pcb;
    Elem* next;
  };

  Elem *head;
  int size;
public:
    SemPQueue();
    ~SemPQueue();
    void push(PCB* pcb, Time t);
    PCB* getPop();
    int getSize() const { return size; }
    unsigned wake(Time currTime);
    unsigned wakeAll();

};



#endif /* H_SEMPQ_H_ */
