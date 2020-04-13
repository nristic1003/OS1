#include <iostream.h>
#include <stdlib.h>
#include <malloc.h>
#include "PCB.h"
#include "thread.h"
#include "timer.h"
#include "semaphor.h"
#include "kerSem.h"
#include "listKer.h"
#include "idle.h"
#include <dos.h>
#include "SCHEDULE.H"


#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf

int counter = 0;

void* operator new(unsigned size) {



  void* ret = malloc(size);



  if(ret==0)
    exit(1);//Out of mem
  counter++;



  return ret;
}


void operator delete(void* loc) {
  if(loc != 0){
    counter--;
    free(loc);
  }
}
extern int userMain(int argc, char* argv[]);
int main(int argc, char* argv[])
{
	lock
	cout<<counter<<endl;
	unlock
#ifndef BCC_BLOCK_IGNORE
	lock
	inic();
	PCB* mainPCB = new PCB(0,1,0);
	PCB::PCBlist->put(mainPCB);

	cout<<("Kreiran main") <<endl;
	PCB::running = mainPCB;
	mainPCB->status = RUNNING;

	Idle* idl = new Idle();
	unlock
#endif

	int ret =userMain(argc, argv);

	lock
	cout<<"Pre idle "<<counter<<endl;
	unlock
	Idle::idleDelete();
	lock
		cout<<"Pre PCB "<<counter<<endl;
		unlock
	delete PCB::PCBlist;
		lock
			cout<<"Pre Semafora "<<counter<<endl;
			unlock

	delete KernelSem::kerList;

			lock
				cout<<"Pre konacno "<<counter<<endl;
				unlock
	restore();

		if(ret==16) cout<<"OK"<<endl;

	return ret;




}
