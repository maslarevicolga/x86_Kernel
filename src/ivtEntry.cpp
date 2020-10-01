/*
 * ivtEntry.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: OS1
 */

#include "ivtEntry.h"
#include "DOS.H"


IVTEntry* IVTEntry::IVT[NUM_OF_ENTRY] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo_, ptrInterrupt newRoutine_) : newRoutine(newRoutine_), ivtNo(ivtNo_), event(0), oldRoutine(0) {
	IVTEntry::IVT[ivtNo] = this;
}

IVTEntry::~IVTEntry(){
	restoreIVTEntry();
}

void IVTEntry::signal(){
	if(this->event != 0) this->event->signal();
}


void IVTEntry::connectToEvent(KernelEv* kernelEv){
	this->event = kernelEv;

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, this->newRoutine);
	enableInterrupts;
#endif

}

void IVTEntry::restoreIVTEntry(){

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;
	setvect(ivtNo, this->oldRoutine);
	enableInterrupts;
#endif
	this->event = 0;
	IVTEntry::IVT[ivtNo] = 0;

}

