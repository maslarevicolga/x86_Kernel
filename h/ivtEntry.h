/*
 * ivtEntry.h
 *
 *  Created on: Apr 4, 2020
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_


#include "kernelEv.h"
#include "switch.h" 	// zbog ptrInterrupt

#define NUM_OF_ENTRY 256

#define PREPAREENTRY(ivtNo, shouldCallOldInterrupt)\
void interrupt new_interrupt_routine##ivtNo(...);\
IVTEntry new_ivt_entry##ivtNo(ivtNo, new_interrupt_routine##ivtNo);\
void interrupt new_interrupt_routine##ivtNo(...) {\
    new_ivt_entry##ivtNo.signal();\
    if (shouldCallOldInterrupt) new_ivt_entry##ivtNo.oldRoutine();\
}\



class IVTEntry {

	IVTNo ivtNo;
	KernelEv* event;
	ptrInterrupt newRoutine;

public:

	static IVTEntry* IVT[NUM_OF_ENTRY];
	ptrInterrupt oldRoutine;

	IVTEntry(IVTNo ivtNo, ptrInterrupt newRoutine);
	~IVTEntry();

	void signal();
	void connectToEvent(KernelEv* kernelEv);
	void restoreIVTEntry();

};



#endif /* H_IVTENTRY_H_ */
