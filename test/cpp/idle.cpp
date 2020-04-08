#include "idle.h"
#include "PCB.h"
Idle::Idle(StackSize stackSize, Time timeSlice ): Thread(stackSize, timeSlice)
{
	//myPCB->status=IDLE;
	PCB* myPCB = getPCB(); // // SUmnjivo
myPCB->status = IDLE; // //

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
