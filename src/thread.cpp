/*
 * thread.cpp
 *
 *  Created on: Mar 25, 2020
 *      Author: OS1
 */

#include <SCHEDULE.H>
#include "pcb.h"
#include "switch.h"
#include "collect.h"

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock;
	this->myPCB = new PCB(stackSize, timeSlice, this, CREATED, PCB::wrapper);
	unlock;
}

Thread::~Thread(){
	lock;
	this->waitToComplete();                  // cekaj na niti dok se ne zavrsi pa tek onda mozes da je dealociras
	if(this->myPCB != 0) delete this->myPCB;
	this->myPCB = 0;
	unlock;
}


void Thread::start(){
	if(this->myPCB != 0 && this->myPCB->state == CREATED) {
		this->myPCB->state = READY;
		lock;
		Scheduler::put(this->myPCB);
		unlock;
	}
}

void Thread::waitToComplete(){
	lock;
	if(this->myPCB != 0) this->myPCB->waitToComplete();
	unlock;
}

ID Thread::getId() const{
	if(this->myPCB != 0) return this->myPCB->getId();
	else return -1;
}

ID Thread::getRunningId(){
	if(MyCollection::runningPCB == 0) return -1;
	else return ((PCB*)(MyCollection::runningPCB))->getId();
}

Thread* Thread::getThreadById(ID id) {
	Thread* res = 0;
	lock;
	PcbList::Iterator it = MyCollection::allPCBs.getIterator();
	while(it.end() == 0){
		if(it.get()->getId() == id && it.get()->myThread != 0){
			res = it.get()->myThread;
			break;
		}
		it.next();
	}
	unlock;
	return res;
}


void Thread::run() {
	for(int i = 0; i < 50; i++)
		cout << i;
}


// SIGNALS

void Thread::signal(SignalId signal){
	lock;
	if(this->myPCB != 0)
		myPCB->signal(signal);
	unlock;
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	lock;
	if(this->myPCB != 0)
		myPCB->registerHandler(signal, handler);
	unlock;
}
void Thread::unregisterAllHandlers(SignalId id){
	lock;
	if(this->myPCB != 0)
		myPCB->unregisterAllHandlers(id);
	unlock;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	lock;
	if(this->myPCB != 0)
		myPCB->swap(id, hand1, hand2);
	unlock;
}

void Thread::blockSignal(SignalId signal){
	lock;
	if(this->myPCB != 0)
		myPCB->blockSignal(signal);
	unlock;
}

void Thread::blockSignalGlobally(SignalId signal){
	lock;
	PCB::blockSignalGlobally(signal);
	unlock;
}

void Thread::unblockSignal(SignalId signal){
	lock;
	if(this->myPCB != 0)
		myPCB->unblockSignal(signal);
	unlock;
}

void Thread::unblockSignalGlobally(SignalId signal){
	lock;
	PCB::unblockSignalGlobally(signal);
	unlock;
}

// SIGNALS
