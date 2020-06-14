#include "kerSem.h"
#include "listKer.h"
#include <dos.h>
#include <iostream.h>

//zabranjuje prekide
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
#ifndef BCC_BLOCK_IGNORE
	lock
	semValue--;
	if(semValue<0)
	{
		if(maxTimeToWait<=0){
			PCB::running->status=BLOCKED;
			PCBblocked->putBlocked(PCB::running, -1); //cekam dok ne dodje signal, ne vraca me tajmer u scheduler
	unlock
			dispatch();
	semValue++;
			return 1;
		}else{
					PCB::running->status=BLOCKED;
					PCBblocked->putBlocked(PCB::running, maxTimeToWait); //cekam dok ne dodje signal, ili timer vracam se u scheduler
					unlock
					dispatch();
					semValue++;
					return 0;
		}
	}
	unlock
	return 1;
#endif
}


int KernelSem::getVal() const
{
	return semValue;
}


KernelSem:: ~KernelSem ()
{
	if(semValue<0)
		signal(semValue*-1);
	delete PCBblocked;
}

int KernelSem::signal(int n)
{
#ifndef BCC_BLOCK_IGNORE
	lock
	if(semValue>=0)
		semValue++;
	if(n==0)
	{
		PCB* myPCB=PCBblocked->getFirst();
		myPCB->status = READY;
		Scheduler::put(myPCB);
		PCBblocked->remove(myPCB);
		unlock
		return 0;
	}else if(n>0)
	{
		int cnt=0;
		if(!PCBblocked->isEmpty())
		{
			while(cnt!=n && !PCBblocked->isEmpty())
			{
						Node* old = PCBblocked->getHead();
						if(old!=0) old->data->status = READY;
						Scheduler::put(old->data);
						PCBblocked->remove(old->data);
						cnt++;
			}
		}
		unlock
		return cnt;
	}
	unlock
	return n;
#endif
}
