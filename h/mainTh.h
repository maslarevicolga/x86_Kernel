/*
 * mainTh.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef H_MAINTH_H_
#define H_MAINTH_H_


#include "thread.h"


class MainThread : public Thread {
public:

	MainThread() : Thread(defaultStackSize, 1){}
	~MainThread() { waitToComplete(); }

protected:

	void run() {}

};


#endif /* H_MAINTH_H_ */
