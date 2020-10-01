/*
 * kernelEv.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: OS1
 */

#include "kernelEv.h"
#include "pcb.h"
#include "switch.h"
#include "collect.h"
#include "SCHEDULE.H"
#include "ivtEntry.h"

//IVTEntry za dati dogadjaj moram napraviti pre KernelEv
//tek kad se desi dogadjaj kad dodje prekid broj ivtNo pcb se deblokira i obradjuje taj prekid
//samo nit koja je stvorila dogadjaj moze i da se blokira na njemu
//na pocetku ni jedna nit nije blokirana
 //kad se desi prekid na ovom ulazu IVT onda se deblokira nit

KernelEv::KernelEv(IVTNo ivtNo_) : ivtNo(ivtNo_), binSem(0), blockedPCB(0) {
	this->myPCB = (PCB*)(MyCollection::runningPCB);
	IVTEntry::IVT[ivtNo]->connectToEvent((KernelEv*)this);
}

KernelEv::~KernelEv() {
	//this->myPCB->state = FINISHED;
	//this->myPCB->removeFromWaitingList();
	this->myPCB = 0;
	IVTEntry::IVT[this->ivtNo]->restoreIVTEntry();
}

void KernelEv::wait() {
	lock;
	if(this->myPCB == MyCollection::runningPCB && this->binSem.value() == 0){
		this->blockedPCB = (PCB*)MyCollection::runningPCB;
		this->blockedPCB->state = BLOCKED;
		unlock;
		dispatch();
	}
	else{
		if(this->myPCB == MyCollection::runningPCB && this->binSem.value() == 1)
			this->binSem.wait(0);  								//u ovom slucaju je potrebno samo val = 0 staviti
		unlock;
    }
}

void KernelEv::signal() {
	lock;
	if(this->blockedPCB == 0) this->binSem.signal();	 //u ovom slucaju je samo potrebno naznaciti da je se desio prekid da se moze uci u kriticnu
	else { 													//u suprotnom deblokiraj nit
		this->blockedPCB->state = READY;
		Scheduler::put((PCB*)this->blockedPCB);
		this->blockedPCB = 0;
	}
	unlock;
}

