/*
 * switch.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

/*
 * timer.cpp
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#include "switch.h"
#include "collect.h"
#include "pcb.h"
#include "kerSem.h"
#include "DOS.H"
#include "SCHEDULE.H"

volatile unsigned ContextSwitch::contextSwitchRequest = 0;
volatile int ContextSwitch::lck = 0;

volatile Time ContextSwitch::timeLeft = defaultTimeSlice;

volatile unsigned ContextSwitch::timeSliceEnd = 0;

ptrInterrupt oldTimerInterrupt = 0;

void ContextSwitch::inic() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;
	lock;

	oldTimerInterrupt = getvect(8);//ovako uzimam adresu iz ulaza 8 IV tabele
	setvect(8, timer);

	MyCollection::mainPCB = new PCB(4096, 1, &MyCollection::mainThread, RUNNING, 0);
	MyCollection::mainPCB->ss = _SS;
	MyCollection::mainPCB->sp = _SP;
	MyCollection::mainPCB->bp = _BP;
	PCB::releaseThread(MyCollection::mainPCB);

    MyCollection::idlePCB = new PCB(4096, 1, &MyCollection::idleThread, BUSY_WAITING, PCB::wrapper);
    PCB::releaseThread(MyCollection::idlePCB);

	MyCollection::runningPCB = MyCollection::mainPCB;

//	SIGNALS
    for(int i = 0; i < numOfSignals; i++)
    	PCB::unblockSignalGlobally(i);
//	SIGNALS

    unlock;
	enableInterrupts;

#endif

}

void ContextSwitch::restore() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;

	setvect(8, oldTimerInterrupt);

	enableInterrupts;
#endif

}


extern void tick();

void interrupt timer(...) {
	if(ContextSwitch::contextSwitchRequest == 0){
		oldTimerInterrupt();
	    tick();
	    SemList::Iterator it = MyCollection::allKernelSems.getIterator();
	    while(it.end() == 0) {
	    	it.get()->numTicks++;
	    	it.next();
	    }
	}

	if(ContextSwitch::contextSwitchRequest == 0 && ContextSwitch::timeLeft > 0)
		ContextSwitch::timeLeft--;

	if(ContextSwitch::contextSwitchRequest == 1 || (MyCollection::runningPCB->isInfinite == 0 && ContextSwitch::timeLeft == 0 && isNotLocked)){
		ContextSwitch::contextSwitchRequest = 0;
		ContextSwitch::timeSliceEnd = 0;

        #ifndef BCC_BLOCK_IGNORE
		MyCollection::runningPCB->sp = _SP;
		MyCollection::runningPCB->ss = _SS;
		MyCollection::runningPCB->bp = _BP;
        #endif

		MyCollection::runningPCB->myLock = ContextSwitch::lck;

		if(MyCollection::runningPCB->state == RUNNING){
			MyCollection::runningPCB->state = READY;
			Scheduler::put((PCB*)MyCollection::runningPCB);
		}

		lock;
		//wake threads on semaphores
		SemList::Iterator it = MyCollection::allKernelSems.getIterator();
	    while(it.end() == 0) {
	    	it.get()->wakeOnSemsByTime();
	        it.next();
	    }
#ifndef BCC_BLOCK_IGNORE
			asm cli
#endif
	    unlock;


		while(1) {

			MyCollection::runningPCB = Scheduler::get();
			if(MyCollection::runningPCB == 0)
				MyCollection::runningPCB = MyCollection::idlePCB;
			else{
				if(MyCollection::runningPCB->state != READY) continue;
				else
					MyCollection::runningPCB->state = RUNNING;
			}

			ContextSwitch::timeLeft = MyCollection::runningPCB->timeSlice;
            ContextSwitch::lck = MyCollection::runningPCB->myLock;

            #ifndef BCC_BLOCK_IGNORE
		  	_SP = MyCollection::runningPCB->sp;
			_SS = MyCollection::runningPCB->ss;
			_BP = MyCollection::runningPCB->bp;
        	#endif

			if(MyCollection::runningPCB == MyCollection::idlePCB)
				break;
// SIGNALS
			lock;
#ifndef BCC_BLOCK_IGNORE
		    asm sti
#endif
			((PCB*)MyCollection::runningPCB)->execSignals();
			if(MyCollection::runningPCB->state == FINISHED)
				MyCollection::runningPCB = 0;
#ifndef BCC_BLOCK_IGNORE
			asm cli
#endif
			unlock;
//  SIGNALS
			if(MyCollection::runningPCB != 0) break;
		}

	}
	else
		if(isNotLocked == 0 && ContextSwitch::timeLeft == 0)
			ContextSwitch::timeSliceEnd = 1;
}



void dispatch() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;

	ContextSwitch::contextSwitchRequest = 1;
	timer();

	enableInterrupts;

#endif
}





