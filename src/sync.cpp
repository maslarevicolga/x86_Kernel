/*
 * sync.cpp
 *
 *  Created on: May 13, 2020
 *      Author: OS1
 */



#include <iostream.h>
#include <stdlib.h>
#include <STDIO.H>
#include <STDARG.H>
#include "switch.h"


int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}



