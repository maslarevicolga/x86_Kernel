/*
 * pcb.h
 *
 *  Created on: Mar 28, 2020
 *      Author: OS1
 */

#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"
#include "switch.h"
#include "signalQ.h"
#include "pcbList.h"

class HandlerList;
const StackSize minStackSize = 256;
const StackSize maxStackSize = 65536;

enum State { CREATED, READY, RUNNING, BLOCKED, FINISHED, BUSY_WAITING };

//	SIGNALS

enum Signal{
	SG0, SG1, SG2, SG3, SG4, SG5,
	SG6, SG7, SG8, SG9, SG10, SG11,
	SG12, SG13, SG14, SG15
};

#define numOfSignals 16
//	SIGNALS


class PCB{

friend void interrupt timer(...);
friend void ContextSwitch::inic();
friend class KernelEv;
friend class KernelSem;
friend class Thread;
friend class SemPQueue;

private:

    ID id;
    static ID _id;

	PcbList waitingToComplete;

	unsigned* stack;
	unsigned stackSize;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	Time timeSlice;

	unsigned isInfinite;        //ako je timeSlice 0
	unsigned unblockedByTime;  //ako je probudjena na semaforu kad joj je isteklo maxTime to wait tad je ovo 1
	Thread* myThread;
    unsigned myLock;

    static void releaseThread(PCB* pcb);
	void removeFromWaitingList();
	void removeFromAllPCBs();
	void delChildPtrs();
public:

	State state;

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread, State initialState, void(*runPtr)());

	~PCB();

	void waitToComplete();
	static void wrapper();

	ID getId() const { return this->id; }


    void kill();

//	SIGNALS


	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

private:

	void execSG2();
    void execSignals();


    HandlerList* (signal_handlers[numOfSignals]);
    SignalQueue arrived_signals;

    int blocked_signal[numOfSignals];
    static int blocked_signal_globally[numOfSignals];

    PCB* parent_pcb;

//	SIGNALS



};

#endif /* H_PCB_H_ */
