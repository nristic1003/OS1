#include "kerSem.h"
#include "listKer.h"
#include <dos.h>
#include <iostream.h>
#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf
ListKernel* KernelSem::kerList = new ListKernel();

KernelSem::KernelSem(int init1)
{
	semValue= init1;
	PCBblocked = new List();
	kerList->put(this);


}

int KernelSem::wait (Time maxTimeToWait)
{

	semValue--;
	if(semValue<0)
	{
		if(maxTimeToWait<=0){
			PCB::running->status=BLOCKED;
			PCBblocked->putBlocked(PCB::running, -1); //cekam dok ne dodje signal, ne vraca me tajmer u scheduler

			dispatch();
			return 1;
		}else {

				PCB::running->status=BLOCKED;
				PCBblocked->putBlocked(PCB::running, maxTimeToWait); //cekam dok ne dodje signal, ili timer vracam se u scheduler

				dispatch();
				return 0;
		}

	}
	return 1;
}


int KernelSem::getVal() const
{
	return semValue;
}


KernelSem:: ~KernelSem ()
{
	delete PCBblocked;
}

int KernelSem::signal(int n)
{
	if(n==0)
	{
		lock
		semValue++;
		PCB* myPCB=PCBblocked->getFirst();
		myPCB->status = READY;
		Scheduler::put(myPCB);
		unlock
		return 0;

	}else if(n>0)
	{
		lock
		int cnt=0;
		while(cnt!=n && !PCBblocked->isEmpty())
		{
					semValue++;
					PCB* myPCB=PCBblocked->getFirst();
					myPCB->status = READY;
					Scheduler::put(myPCB);
					cnt++;
		}
		unlock
		return cnt;
	}
	return n;

}
