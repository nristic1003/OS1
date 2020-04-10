#include <iostream.h>
#include <stdlib.h>
#include <malloc.h>
#include "PCB.h"
#include "thread.h"
#include "timer.h"
#include "semaphor.h"
#include "kerSem.h"
#include "idle.h"
#include <dos.h>

#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf

extern int userMain(int argc, char* argv[]);
int main(int argc, char* argv[])
{

#ifndef BCC_BLOCK_IGNORE
	lock
	inic();
	PCB* mainPCB = new PCB(0,1,0);
	PCB::PCBlist->put(mainPCB);

	cout<<("Kreiran main") <<endl;
	PCB::running = mainPCB;
	mainPCB->status = RUNNING;

	Idle* idl = new Idle(defaultStackSize, 1);
	unlock
#endif

	int ret =userMain(argc, argv);


	restore();


	return ret;




}
