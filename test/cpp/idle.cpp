#include "idle.h"
#include "PCB.h"
Idle::Idle(StackSize stackSize, Time timeSlice ): Thread(stackSize, timeSlice)
{
	PCB* myPCB = getPCB();
	myPCB->status = IDLE;

}




Idle::~Idle()
{

}

void Idle::run()
{
	while(1)
	{

	}
}
