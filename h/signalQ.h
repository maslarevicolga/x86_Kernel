/*
 * signalQ.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#ifndef H_SIGNALQ_H_
#define H_SIGNALQ_H_

#include "thread.h"

class SignalQueue{
	struct Elem{
		SignalId signal;
		Elem *next;
	};


public:
	Elem *head, *tail;
	SignalQueue();
	~SignalQueue();

	void push(SignalId signal);
	SignalId getPop();
    SignalId getFirstSignal();
    unsigned isEmpty();

    struct Iterator{
        private:
            SignalQueue *q;
            Elem *currElem;
            Elem *prevElem;
        public:
            Iterator(SignalQueue *);
            void pop();
            SignalId get();
            void next();
            unsigned end();
    };

    Iterator getIterator();
};


#endif /* H_SIGNALQ_H_ */
