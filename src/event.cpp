/*
 * event.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: OS1
 */

#include "event.h"
#include "switch.h"
#include "kernelEv.h"

Event::Event(IVTNo ivtNo){
	lock;
	this->myImpl = new KernelEv(ivtNo);
	unlock;
}

Event::~Event(){
	if(this->myImpl != 0){
		lock;
		delete this->myImpl;
		unlock;
	}
}

void Event::wait(){
	if(this->myImpl != 0) this->myImpl->wait();
}

void Event::signal(){
	if(this->myImpl != 0) this->myImpl->signal();
}


