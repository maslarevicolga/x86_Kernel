/*
 * SCHEDULE.H
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#ifndef LIB_SCHEDULE_H_
#define LIB_SCHEDULE_H_

// Obavezno odbrati HUGE mem. model kao sto je prikazano u uputstvu za podesavanje BC31 okruzenja. Takodje, podesiti putanje ka .lib i .h fajlovima kao i ostale parametre prema uputstvu!
// Dodati APLICAT.LIB u projekat: (Project->Add Item...), promeniti Name filter u *.* kako bi se videli svi fajlovi i odabrati APLICAT.LIB. Zatim pozvati Compile->Build All
class PCB;

class Scheduler{
public:
	static void put (PCB *);
	static PCB* get ();
};



#endif /* LIB_SCHEDULE_H_ */
