/*
 * kernelSemaphore.h
 *
 *  Created on: Apr 2, 2020
 *      Author: OS1
 */

#ifndef H_KERSEM_H_
#define H_KERSEM_H_

#include "pcb.h"
#include "switch.h"
#include "semPQ.h"
#include "semQ.h"
#include "semaphor.h"

class KernelSem {

friend void interrupt timer(...);

public:

	KernelSem(int initial, Semaphore* mySemaphore);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	int value() const;  // Returns the current value of the semaphore

	void wakeOnSemsByTime();
	volatile unsigned long long numTicks;

private:
	int val;
	Semaphore* mySemaphore;



	SemPQueue time_blocked;
	SemQueue blocked;


	void block(int myMaxTimeToWait);

	void finishBlocked();
	void finishTimeBlocked();
};



#endif /* H_KERSEM_H_ */
