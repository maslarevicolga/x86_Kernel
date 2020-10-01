/*
 * collect.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef H_COLLECT_H_
#define H_COLLECT_H_

#include "switch.h"
#include "pcbList.h"
#include "semList.h"
#include "idleTh.h"
#include "mainTh.h"

class PCB;
class KernelSem;

class MyCollection {

public:

	volatile static SemList allKernelSems;
	static PcbList allPCBs;

	static volatile PCB* runningPCB;
	static PCB* mainPCB;
	static PCB* idlePCB;


	static IdleThread idleThread;
	static MainThread mainThread;


};




#endif /* H_COLLECT_H_ */
