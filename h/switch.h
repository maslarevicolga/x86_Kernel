/*
 * switch.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef H_SWITCH_H_
#define H_SWITCH_H_

#include "thread.h"  //zbog Time ubaceno



//i timer i tastatura su maskirajuci pa kad klirujem onemogucava context_switch
#define enableInterrupts  asm popf;
#define disableInterrupts asm {pushf; cli; }

//neophodno da bih u pojedinim delovima zabranila da dodje do promene konteksta(da dodje do dispatch() gde se i vrsi njihova provera)
#define lock ContextSwitch::lck++

#define unlock \
ContextSwitch::lck--;\
if (ContextSwitch::lck == 0 && ContextSwitch::timeSliceEnd == 1) dispatch() 				//sa lock je dozvoljeno da se dodje do timer fje jer nisu maskirani
																							//prekidi ali nije moguce izvrsiti context _switch i zbog toga je ovde provera
                                                             	 	 	 	 	 	 	 	//da nije slucajno dosao timeSlice == 0 jer je tada conSwReq = 1
#define isNotLocked ContextSwitch::lck == 0


typedef void interrupt (*ptrInterrupt)(...);
void interrupt timer(...);
void dispatch();
extern ptrInterrupt oldTimerInterrupt;


class ContextSwitch {

public:

	static volatile int lck;
    static volatile Time timeLeft;
    static volatile unsigned timeSliceEnd;
    static volatile unsigned contextSwitchRequest;

    static void inic();
    static void restore();



};


#endif /* H_SWITCH_H_ */

