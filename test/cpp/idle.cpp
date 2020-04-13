#include "idle.h"
#include "PCB.h"
Thread* Idle::idle = 0;
Idle::Idle():Thread(defaultStackSize, 1)
{
	if(idle==0)
	{
		idle=this;
		PCB* idPCB = getIdlePCB();
		idPCB->status=IDLE;
	}

}


PCB* Idle::getIdlePCB()
{
	return idle->myPCB;
}
void Idle::idleDelete()
{
	if(idle!=0)
	{
		delete idle;
		idle=0;
	}
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
