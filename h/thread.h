/*
 * thread.h
 *
 *  Created on: Mar 28, 2020
 *      Author: OS1
 */

#ifndef H_THREAD_H_
#define H_THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;                 // time, x 55ms
const Time defaultTimeSlice = 2;          // default = 2*55ms

typedef int ID;

typedef void (*SignalHandler)();
typedef unsigned SignalId;

void dispatch();

class PCB;                                   // Kernel's implementation of a user's thread

class Thread {
public:

	void start();
	void waitToComplete();

	ID getId() const;
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual ~Thread();

//	SIGNALS
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
//	SIGNALS


protected:

	friend class PCB;

	virtual void run();

private:

	PCB* myPCB;

};


#endif /* H_THREAD_H_ */
