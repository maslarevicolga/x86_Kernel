///*
// * extend.h
// *
// *  Created on: May 23, 2020
// *      Author: OS1
// */
//
//#ifndef H_EXTEND_H_
//#define H_EXTEND_H_
//
//#include <iostream.h>
//
//#include "thread.h"
//#include "switch.h"
//
//Semaphore sem1(0);
//
//void four() {
//
//	sem1.signal(1200);
//
//}
//
//class Thread1 : public Thread {
//
//public:
//	Thread1() : Thread(4096, 1) {}
//	~Thread1() { waitToComplete(); }
//
//protected:
//
//	void run() {
//
//		/*Thread* th1;
//		Thread* th2;
//		if(MyCollection::runningPCB->getId() == 3) {
//			th1 = new Thread1();
//			th1->registerHandler(4, four);
//			th2 = new Thread1();
//			th2->registerHandler(4, four);
//			th1->start();
//			th2->start();
//		}
//		for(long i = 0; i < 1000; i++) {
//			sem1.wait(5);
//			lock;
//			cout << i << " " << this->getId() << "\n";
//			unlock;
//			if(MyCollection::runningPCB->getId() == 3 && i == 50)
//				th1->signal(4);
//			if(MyCollection::runningPCB->getId() == 3 && i == 50)
//				th2->signal(4);
//		}
//		if(MyCollection::runningPCB->getId() == 3) {
//			delete th1;
//			delete th2;
//		}*/
//	}
//
//};
//
//Semaphore sem(0);
//
//void fun() {
//
//	for(int i = 0; i < 1000; i++) {
//		//lock;
//		cout << "sig3 " << MyCollection::runningPCB->getId() << "\n";
//
//		//unlock;
//	}
//
//
//}
//void fun1() {
//
//	for(int i = 0; i < 1000; i++){
//		//lock;
//		cout << "sig4 " << MyCollection::runningPCB->getId() << "\n";
//		//unlock;
//	}
//}
//
//
//
//void myself() {
//	lock;
//	cout << "myself thread\n";
//	unlock;
//}
//
//void parent();
//
//class Thread2 : public Thread {
//
//public:
//	Thread2() : Thread(4096, 1) {}
//	~Thread2() { waitToComplete(); }
//
//protected:
//
//	void run() {
//		this->registerHandler(2, myself);
//		this->registerHandler(1, parent);
//
//		Thread* th1 = new Thread1();
//		th1->registerHandler(3, fun);
//		th1->registerHandler(4, fun1);
//
//		th1->signal(3); th1->signal(4);
//		Thread* th2 = new Thread1();
//		th2->signal(0);
//		th1->start();
//		th2->start();
//
//	for(long i = 0; i < 1000; i++) {
//			lock;
//
//		cout << i << " " << this->getId() << " " << sem.value() << "\n";
//			unlock;
//
//			sem.wait(5);
//
//			if(i == 200) {
//				cout << "------------------------+++++++++++++++++++++++++++++++\n";
//				sem.signal(1000);
//			}
//		}
//		delete th1;
//		delete th2;
//	}
//
//};
//
//void tick() {}
//void parent() {
//	lock;
//
//	cout << "parent thread\n";
//
//	unlock;
//
//}
//
//int userMain(int argc, char *argv[]) {
//	Thread* th1 = new Thread2();
//	th1->start();
//	delete th1;
//}
//
//
//
//
//
//#endif /* H_EXTEND_H_ */
