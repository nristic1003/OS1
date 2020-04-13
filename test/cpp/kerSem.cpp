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
				semValue++;
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
	if(semValue<0)
		signal(semValue*-1);
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
		PCBblocked->remove(myPCB);
		return 0;
		unlock

	}else if(n>0)
	{


		int cnt=0;
		if(!PCBblocked->isEmpty())
		{

		while(cnt!=n && !PCBblocked->isEmpty())
		{
					Node* old = PCBblocked->getHead();
					semValue++;
					//while(old->data->timeSlice==0 || old->data->timeSlice==-1) old=old->next;
					//Ne radi dobro jrt getujr uvrk prvi
					if(old!=0) old->data->status = READY;
					Scheduler::put(old->data);
					PCBblocked->remove(old->data);
					cnt++;
		}
		}


		return cnt;
	}
	return n;

}
