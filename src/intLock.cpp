#include "intLock.h"

int mutex_glb = 1;

void mutex_glb_wait(){

#ifndef BCC_BLOCK_IGNORE
 sloop:asm{
	mov ax, 0
	xchg ax, mutex_glb
}
 if(_AX ==0){
	dispatch();
	asm jmp sloop;
 }
#endif

}
