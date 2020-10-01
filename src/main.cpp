/*
 * main.cpp
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include "thread.h"
#include "pcb.h"
#include "switch.h"
#include "semaphor.h"
#include "collect.h"
#include "kerSem.h"

#include "extend.h"


int userMain(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	ContextSwitch::inic();

    int ret = userMain(argc, argv);

    ContextSwitch::restore();

	return 0;
}



