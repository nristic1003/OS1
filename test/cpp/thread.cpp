#include "thread.h"
#include "PCB.h"
#include "timer.h"
#include <dos.h>
#include <iostream.h>
#include "idle.h"

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
	if(myPCB!=Idle::getIdlePCB() && myPCB!=0)
		waitToComplete();
	if(myPCB!=0)
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
#ifndef BCC_BLOCK_IGNORE
	lock
	if(PCB::running!=myPCB && myPCB!=Idle::getIdlePCB() && myPCB->status!=FINISH && myPCB!=0)
	{
		PCB::running->status = BLOCKED;
		myPCB->waitForMe->put(PCB::running);
		dispatch();
	}
	unlock
#endif
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

	PCB* pthread = PCB::PCBlist->getByID(id);
	return pthread->getThread();

}


//SIGNALI
Thread::Thread(int x)
{
	 myPCB = new PCB(this,1,0);
	PCB::PCBlist->put(myPCB);
	PCB::running = myPCB;
	myPCB->status = RUNNING;
}

void Thread::signal(SignalId signal)
{
	if(signal>=0 && signal!=2 && signal<16 && myPCB!=PCB::running && myPCB->status!=FINISH)
	{
		if(!myPCB->handleri[signal].isEmpty() || signal==0)
				myPCB->queue->put(signal);
		return;
	}


}

void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
	if(signal>0 && signal<16 && myPCB->status!=FINISH)
			myPCB->handleri[signal].put(handler);


}

void Thread::unregisterAllHandlers(SignalId id)
{
	if(id>0 && id<16 && myPCB->status!=FINISH)
	{
		if(!myPCB->handleri[id].isEmpty())
		{
			 myPCB->handleri[id].removeAll();
			//myPCB->handleri[id]=0;
		}
	}
}

void Thread::blockSignal(SignalId signal)
{
	if(signal>=0 && signal<16 && myPCB->status!=FINISH)
		myPCB->blokiraniSignali[signal] = 1;
}
void Thread::unblockSignal(SignalId signal)
{
	if(signal>=0 && signal<16 && myPCB->status!=FINISH)
		myPCB->blokiraniSignali[signal] = 0;
}
 void Thread::blockSignalGlobally(SignalId signal)
{
	 if(signal>=0 && signal<16)
		 PCB::blokiraniGLobalno[signal] =1;
}

 void Thread::unblockSignalGlobally(SignalId signal)
{
	if(signal>=0 && signal<16)
		PCB::blokiraniGLobalno[signal] =0; // Otvoreno pitanje? da li se odblokiraju sve ili samo ove
}

 void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2)
 {

	 if(id>2 && id<16 && !myPCB->handleri[id].isEmpty() && myPCB->status!=FINISH)
	 {
		 myPCB->handleri[id].swap(hand1, hand2);
	 }
 }


