/*
 * kernelEv.h
 *
 *  Created on: Apr 4, 2020
 *      Author: OS1
 */

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_

#include "event.h"
#include "semaphor.h"

class PCB;



class KernelEv {

	Semaphore binSem;
	PCB* myPCB;                  //pcb niti koja je kreirala ovaj dogadjaj
	IVTNo ivtNo;                 //br prekida koji je vezan za ovaj dogadjaj
    PCB* blockedPCB;             //pcb niti koja je blokirana na ovom semaforu
public:

	KernelEv(IVTNo ivtNo);

	~KernelEv();

	void wait();
	void signal();

};


#endif /* H_KERNELEV_H_ */
