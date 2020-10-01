/*
 * kernelSemaphore.cpp
 *
 *  Created on: Apr 3, 2020
 *      Author: OS1
 */

#include "kerSem.h"
#include "switch.h"
#include "collect.h"

#include "SCHEDULE.H"

KernelSem::KernelSem(int initial_, Semaphore* mySemaphore_) : val(initial_), mySemaphore(mySemaphore_), numTicks(0){
	if(initial_ < 0) val = 0;
	MyCollection::allKernelSems.push(this);

}
//------------------------------------------------------------------------------------------------------------------------------

int KernelSem::value() const {
	return this->val;
}
//-------------------------------------------------------------------------------------------------------------------------------

void KernelSem::block(int myMaxTimeToWait){
	MyCollection::runningPCB->state = BLOCKED;
	if(myMaxTimeToWait == 0)
		this->blocked.push((PCB*)MyCollection::runningPCB);
	else
		this->time_blocked.push((PCB*)MyCollection::runningPCB, numTicks + myMaxTimeToWait);
}
//---------------------------------------------------------------------------------------------------------------------------------------

int KernelSem::wait(Time maxTimeToWait){

	lock;

	if(this->val-- > 0) { //moze da udje u kriticnu
		unlock;
		return 1;
	}

	block(maxTimeToWait);        // block running thread and call dispatch

    unlock;

    dispatch();

	if(MyCollection::runningPCB->unblockedByTime == 1) {
		MyCollection::runningPCB->unblockedByTime = 0;
		return 0;
	}
	return 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------


int KernelSem::signal(int n) {
    if (n < 0) return n;
    if (n == 0)  n = 1;

    lock;
    val += n;
    int ret = 0;
    int numToUnblock = n;
    if(n > this->blocked.getSize()) numToUnblock = this->blocked.getSize();
    for (int i = 0; i < numToUnblock; i++) {
        PCB *pcb = this->blocked.getPop();
        pcb->state = READY;
        Scheduler::put(pcb);
    }
    ret += numToUnblock;
    numToUnblock = n - numToUnblock;

    while (numToUnblock > 0 && this->time_blocked.getSize() > 0) {
        numToUnblock--;
        PCB *pcb = this->time_blocked.getPop();
        pcb->state = READY;
        Scheduler::put(pcb);
        ret++;
    }

    if(n == 0) ret = 0;

    unlock;
    return ret;
}
//---------------------------------------------------------------------------------------------------------------------------------------


void KernelSem::wakeOnSemsByTime() {                    // call from timer so PSWI = 0 => NO interrupts
   	if(this->time_blocked.getSize() == 0)
		this->numTicks = 0;

   	int ret = this->time_blocked.wake(this->numTicks);
	val += ret;
}
//---------------------------------------------------------------------------------------------------------------------------------------

/*void KernelSem::finishBlocked(){
	List<PCB*>::Node* it = this->blocked.begin();
	whil

	e(it != 0){
		it->data->state = FINISHED;
		it->data->removeFromWaitingList();
		this->blocked.popFront();
		it = this->blocked.begin();
	}
}*/
//---------------------------------------------------------------------------------------------------------------------------------------

/*void KernelSem::finishTimeBlocked(){
	List<TimeBlocked*>::Node* it = this->time_blocked.begin();
	while(it != 0){
		it->data->pcb->state = FINISHED;
        it->data->pcb->removeFromWaitingList();
#ifdef SIGNAL
    if(it->data->pcb->parent_pcb != 0)
    	it->data->pcb->parent_pcb->signal(SG1);                           //salje roditelju da je finished
    ((PCB*)MyCollection::runningPCB)->signal(SG2);                        // salje sama sebi da je finished

#endif
        this->time_blocked.popFront();
        it = this->time_blocked.begin();
	}
}*/
//---------------------------------------------------------------------------------------------------------------------------------------

KernelSem::~KernelSem() {
	lock;

	if(this->val < 0) signal(-val);
	this->mySemaphore = 0;

	SemList::Iterator it = MyCollection::allKernelSems.getIterator();
	while(it.end() == 0){
		if(it.get() == this){
			it.pop();
			break;
		}
		it.next();
	}
	unlock;

}







