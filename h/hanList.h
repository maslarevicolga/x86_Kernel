/*
 * hanList.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#ifndef H_HANLIST_H_
#define H_HANLIST_H_

#include "pcb.h"

class HandlerList {

    struct Elem {
        SignalHandler handler;
        Elem *next;
    };
	Elem *head, *tail;

public:

	HandlerList();
	~HandlerList();
	void push(SignalHandler handler);
	void pop();
    void call();
    void swap(SignalHandler hand1, SignalHandler hand2);
    HandlerList* copy() const;
};


#endif /* H_HANLIST_H_ */
