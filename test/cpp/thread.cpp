#include "thread.h"
#include "PCB.h"
#include "timer.h"
#include <dos.h>
#include <iostream.h>

#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf



Thread::Thread (StackSize stackSize, Time timeSlice)
{
	myPCB = new PCB(this, timeSlice, stackSize);
	myPCB->status=CREATED;
	PCB::PCBlist ->put(myPCB);
}
Thread::~Thread()
{
	waitToComplete();
	delete myPCB;
}
void Thread::start()
{
	if(myPCB->status==CREATED)
	{
		myPCB->status= READY;
		Scheduler::put(myPCB);
	}

}
void Thread::waitToComplete()
{

	if(PCB::running!=this->myPCB && PCB::running!=PCB::PCBlist->getIdle() && myPCB->status!=FINISH )
	{
		PCB::running->status = BLOCKED;
		myPCB->waitForMe->put(PCB::running);
		dispatch();
	}
}

PCB* Thread::getPCB()
{
	return myPCB;
}

ID Thread::getId() {return myPCB->getThreadId();}
ID Thread::getRunningId(){return PCB::running->getThreadId();}


void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
		lock
		Timer::context_switch_on_demand = 1;
		Timer::timer();
		unlock
#endif
}

Thread* Thread::getThreadById(ID id)
{
	List* p = PCB::PCBlist;
	return p->getByID(id)->thread;

}
