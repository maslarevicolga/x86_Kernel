/*
 * semaphore.h
 *
 *  Created on: Apr 2, 2020
 *      Author: OS1
 */

#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore {

public:
	Semaphore (int initial = 1);
	~Semaphore ();

	int wait (Time maxTimeToWait);
	int signal(int n = 0);

	int value() const;                          // Returns the current value of the semaphore

private:

	KernelSem* myImpl;
};



#endif /* H_SEMAPHOR_H_ */
