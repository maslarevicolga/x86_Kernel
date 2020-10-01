/*
 * collect.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */


#include "collect.h"
#include "pcb.h"
#include "kerSem.h"

PcbList MyCollection::allPCBs;
volatile SemList MyCollection::allKernelSems;

volatile PCB* MyCollection::runningPCB = 0;

PCB* MyCollection::mainPCB = 0;
PCB* MyCollection::idlePCB = 0;


IdleThread MyCollection::idleThread;
MainThread MyCollection::mainThread;

