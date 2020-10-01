/*
 * semaphore.cpp
 *
 *  Created on: Apr 3, 2020
 *      Author: OS1
 */

#include "semaphor.h"
#include "kerSem.h"
#include <iostream.h>

Semaphore::Semaphore(int initial){
	lock;
	this->myImpl = new KernelSem(initial, this);
	unlock;
}

int Semaphore::wait(Time maxTimeToWait){
	return this->myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return this->myImpl->signal(n);
}

int Semaphore::value() const {
	lock;
	return this->myImpl->value();
	unlock;
}

Semaphore::~Semaphore(){
	if(this->myImpl != 0){
		lock;
		delete this->myImpl;
		this->myImpl = 0;
		unlock;

	}
}
