/*
 * time_pr_queue.h
 *
 *  Created on: Apr 3, 2020
 *      Author: OS1
 */

#ifndef H_QUEUE_H_
#define H_QUEUE_H_

#include "pcb.h"

template<class T>
class Queue : public List<T> {
public:

	void push(const T& data){
		this->pushBack(data);
	}

	T pop(){
		return this->popFront();
	}

};

#endif /* H_QUEUE_H_ */
