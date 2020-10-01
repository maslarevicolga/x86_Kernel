/*
 * idleTh.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef H_IDLETH_H_
#define H_IDLETH_H_

#include "thread.h"


class IdleThread : public Thread {
public:

	IdleThread() : Thread(defaultStackSize, 1){ doing = 1;}
	~IdleThread() { doing = 0; waitToComplete(); }

protected:

	void run() {
		while(doing == 1){}
	}

private:
	unsigned doing;
};



#endif /* H_IDLETH_H_ */
