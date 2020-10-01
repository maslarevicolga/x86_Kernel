/*
 * priority.h
 *
 *  Created on: May 8, 2020
 *      Author: OS1
 */

#ifndef H_PRIORITY_H_
#define H_PRIORITY_H_


#include "list.h"

template<class T>
class PriorityQueue : public List<T> {
public:

	void push(const T& data){
		if(this->size == 0){
			this->pushBack(data);
			return;
		}
		Node* curr = head, *prev = 0;
		while(curr != 0 && curr->data <= data) {
			prev = curr;
			curr = curr->next;
		}
		if(prev != 0)
			this->pushAfter(prev, data);
		else
			this->pushFront(data);
	}

	T pop(){
		return this->popFront();
	}


};



#endif /* H_PRIORITY_H_ */
