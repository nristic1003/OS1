#include "thread.h"
#include "PCB.h"
#include "timer.h"
#include <dos.h>

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

}
void Thread::start()
{
	if(myPCB->status==CREATED)
	{
		myPCB->status= READY;
		Scheduler::put(myPCB);
	}

}
PCB* Thread::getPCB()
{
	return myPCB;
}

ID Thread::getId() {return myPCB->getThreadId();}
ID Thread::getRunningId(){return PCB::running->getThreadId();}

/*Thread* Thread::getThreadById(ID id)
{
	Node* p = PCBlist;
	while(p->data->threadId!=id) p=p->next;
	if(p==0) return -1;
	return p->data->thread;

}
*/
void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
		lock
		Timer::context_switch_on_demand = 1;
		Timer::timer();
		unlock
#endif
}
