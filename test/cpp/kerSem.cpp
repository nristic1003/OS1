#include "kerSem.h"
#include "listKer.h"

ListKernel* KernelSem::kerList = new ListKernel();

KernelSem::KernelSem(int init1)
{
	semValue= init1;
	PCBblocked = new List();


}

int KernelSem::wait (Time maxTimeToWait)
{
	semValue--;
	if(semValue<0)
	{
		PCB::running->status=BLOCKED;
		PCBblocked->putBlocked(PCB::running, maxTimeToWait);
		dispatch();
		if(maxTimeToWait==0) return 0;
		return 1;
	}
	return 1;
}




int KernelSem::getVal() const
{
	return semValue;
}


KernelSem:: ~KernelSem ()
{

}

int KernelSem::signal(int n)
{
	if(n==0)
	{
		semValue++;
		PCB* myPCB=PCBblocked->getFirst();
		myPCB->status = READY;
		Scheduler::put(myPCB);
		return 0;

	}else if(n>0)
	{
		int cnt=0;
		while(cnt!=n && !PCBblocked->isEmpty())
		{
					semValue++;
					PCB* myPCB=PCBblocked->getFirst();
					myPCB->status = READY;
					Scheduler::put(myPCB);
					cnt++;
		}
		return cnt;
	}
	return n;

}
