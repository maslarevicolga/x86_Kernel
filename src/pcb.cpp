/*
 * pcb.cpp
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include "signalQ.h"
#include "pcbList.h"
#include "hanList.h"
#include "collect.h"
#include <SCHEDULE.H>
#include "DOS.H"
#include <iostream.h>


ID PCB::_id = -2;

/*
 * psw
 * pc_seg
 * pc_offs
 * ax
 * bx
 * cx
 * dx
 * es
 * ds
 * si
 * di
 * bp
 */

PCB::PCB(StackSize stackSize_, Time timeSlice_, Thread* myThread_,State initialState_, void (*runPtr)()) : stackSize(stackSize_),
		timeSlice(timeSlice_),
		myThread(myThread_),
		state(initialState_),
		unblockedByTime(0),
		myLock(0),
		id(++PCB::_id)
{
	if(stackSize > maxStackSize) stackSize = maxStackSize;
	if(stackSize < minStackSize) stackSize = minStackSize;
    unsigned numLocation = stackSize / sizeof(unsigned);

    lock;
	this->stack = new unsigned[numLocation];
	unlock;

	if(timeSlice == 0) this->isInfinite = 1;
	else this->isInfinite = 0;

#ifndef BCC_BLOCK_IGNORE
	this->stack[numLocation - 1] = 0x200; //na ovoj lok na steku je PSW a na 200 h je bit I koji se postavlja sada na 1
	this->stack[numLocation - 2] = FP_SEG(runPtr);
	this->stack[numLocation - 3] = FP_OFF(runPtr);

	this->ss = FP_SEG(stack + numLocation - 12);
	this->sp = FP_OFF(stack + numLocation - 12);
	this->bp = this->sp;
#endif

	MyCollection::allPCBs.push((PCB*)this);

// SIGNALS
	this->parent_pcb = (PCB*)MyCollection::runningPCB;
	for(int i = 0; i < numOfSignals; i++){
		if(this->parent_pcb == 0) this->blocked_signal[i] = 0;
		else this->blocked_signal[i] = this->parent_pcb->blocked_signal[i];

		if(this->parent_pcb == 0)
			this->signal_handlers[i] = new HandlerList();
		else
			this->signal_handlers[i] = this->parent_pcb->signal_handlers[i]->copy();
	}
// SIGNALS

}
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

PCB::~PCB() {                                                            //kad dodje ovde sigurno je finished i waitingList je empty za tu nit
	delete [] this->stack;                                               // ne treba lock jer se zove iz ~thread koji je lock & unlock
	this->stack = 0;
	if(this->myThread != 0) this->myThread = 0;

// SIGNALS
	for(int i = 0; i < numOfSignals; i++)
		delete signal_handlers[i];

	delChildPtrs();
// SIGNALS

	//delete from allPCbs
	PcbList::Iterator it = MyCollection::allPCBs.getIterator();
	while(it.end() == 0){
		if(it.get() == this){
			it.pop();
			break;
		}
		it.next();
	}

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PCB::delChildPtrs() {
#ifdef SIGNAL
	PcbList::Iterator it = MyCollection::allPCBs.getIterator();
	while(it.end() == 0){
		if(it.get()->parent_pcb == this)
			it.get()->parent_pcb = 0;
		it.next();
	}
#endif
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void PCB::releaseThread(PCB* to) { // call from inic which is lock and unlock
	delete to->myThread->myPCB;
	to->myThread->myPCB = to;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PCB::removeFromWaitingList() {
	PcbList::Iterator it = ((PCB*)MyCollection::runningPCB)->waitingToComplete.getIterator();
	while(it.end() == 0) {
		it.get()->state = READY;
		Scheduler::put(it.get());
		it.pop();
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PCB::wrapper() {
	MyCollection::runningPCB->myThread->run();
	lock;

	//thread is finished there                                 prirodna smrt
	MyCollection::runningPCB->state = FINISHED;
	((PCB*)MyCollection::runningPCB)->removeFromWaitingList();

// SIGNALS
    if(MyCollection::runningPCB->parent_pcb != 0)
    	MyCollection::runningPCB->parent_pcb->signal(SG1);                //salje roditelju da je finished
    ((PCB*)MyCollection::runningPCB)->signal(SG2);                        // salje sama sebi da je finished

    ((PCB*)MyCollection::runningPCB)->execSG2();                      // handle left signals
// SIGNALS

	unlock;
	dispatch();                                                          //zavrsio je ne sme on da nastavi mora obavezno dispatch
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PCB::waitToComplete() {         // running ceka na nekoj niti
	if(this != MyCollection::runningPCB && this->state != CREATED && this->state != FINISHED && this != MyCollection::mainPCB && this->state != BUSY_WAITING){ //ne moze da ceka sama na sebi, na zavrsenoj, na mainPCB, i na idle
		MyCollection::runningPCB->state = BLOCKED;
		this->waitingToComplete.push((PCB*)MyCollection::runningPCB);
		dispatch();
	}
}

//---------------------------------------------------------SIGNALS------------------------------------------------------------------------------------------------
//	SIGNALS


int PCB::blocked_signal_globally[numOfSignals];

void PCB::signal(SignalId signal) {
	if(signal >= numOfSignals) return;
	this->arrived_signals.push(signal);
}

void PCB::registerHandler(SignalId signal, SignalHandler handler){
	if(signal >= numOfSignals) return;
	this->signal_handlers[signal]->push(handler);
}
void PCB::unregisterAllHandlers(SignalId id){
	if(id >= numOfSignals) return;
	this->signal_handlers[id]->pop();
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(id >= numOfSignals) return;
	this->signal_handlers[id]->swap(hand1, hand2);
}

void PCB::blockSignal(SignalId signal){
	if(signal >= numOfSignals) return;
	this->blocked_signal[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal){
	if(signal >= numOfSignals) return;
	PCB::blocked_signal_globally[signal] = 1;
}

void PCB::unblockSignal(SignalId signal){
	if(signal >= numOfSignals) return;
	this->blocked_signal[signal] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal){
	if(signal >= numOfSignals) return;
	PCB::blocked_signal_globally[signal] = 0;
}

void PCB::execSG2() {
	execSignals();
}


void PCB::execSignals(){                                                                      // zovem iz prekidne rutine nema potrebe za lock & unlock
	SignalQueue::Iterator it = this->arrived_signals.getIterator();
	while(it.end() == 0) {
		SignalId signal = it.get();

		if(this->blocked_signal[signal] == 1 || this->blocked_signal_globally[signal] == 1)
			it.next();
		else {
			this->signal_handlers[signal]->call();
			it.pop();
			if(signal == SG0 && this->state != FINISHED) {
				this->kill();
				return;
			}
		}
	}
}

//neprirodna smrt nece doci kod wrapper dela
void PCB::kill() {                                         // zovem iz prekidne rutine nema potrebe za lock & unlock
	this->state = FINISHED;
	this->removeFromWaitingList();

	if(this->parent_pcb != 0)
    	this->parent_pcb->signal(SG1);                //salje roditelju da je finished
    this->signal(SG2);
    this->execSG2();                                                    // salje sama sebi da je finished
}

//	SIGNALS


