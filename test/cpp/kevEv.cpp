#include "kerEv.h"
#include "PCB.h"
#include "thread.h"
#include "SCHEDULE.h"

KernelEv::KernelEv()
{
	blocked = 0;
}
KernelEv::~KernelEv()
{

}

void KernelEv::wait()
{
	if(blocked==0){
		PCB::running->status = BLOCKED;
		blocked = PCB::running;
		dispatch();
	}
}

void KernelEv::signal()
{
	if(blocked!=0){
		blocked->status = READY;
		Scheduler::put(blocked);
		blocked=0;
	}
}
