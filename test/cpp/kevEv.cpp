#include "kerEv.h"
#include "PCB.h"
#include "thread.h"
#include "SCHEDULE.h"

KernelEv::KernelEv()
{
	value = 1;
	blocked = 0;
}

void KernelEv::wait()
{
	if(blocked==0){
			value--;
			PCB::running->status = BLOCKED;
			blocked = PCB::running;
			dispatch();

	}
}

void KernelEv::signal()
{
	if(blocked!=0){
		value++;
		blocked->status = READY;
		Scheduler::put(blocked);
		blocked=0;
		dispatch();
	}

}
