/*
 * semList.h
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */

#ifndef H_SEMLIST_H_
#define H_SEMLIST_H_


class KernelSem;

class SemList{

	struct Elem{
		KernelSem* ksem;
		Elem *next;
		Elem(){
			next = 0;
			ksem = 0;
		}
	};

public:
	Elem *head, *tail;
	SemList();
	~SemList();
    void push(KernelSem* ksem);

    struct Iterator {
        private:
            SemList *l;
            Elem *currElem;
            Elem *prevElem;
        public:
            Iterator(SemList *);
            void pop();
            KernelSem* get();
            void next();
            unsigned end();
    };

    Iterator getIterator();
};


#endif /* H_SEMLIST_H_ */
