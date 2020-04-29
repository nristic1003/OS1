#include "timer.h"
#include "PCB.h"
#include "kerSem.h"
#include "listKer.h"
#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "idle.h"

volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

volatile int cntr=1;

#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf


typedef void interrupt (*pInterrupt)(...);
 pInterrupt oldISR;

extern void tick();
int Timer::context_switch_on_demand=0;

 void inic()
 {
	#ifndef BCC_BLOCK_IGNORE
	 	lock
	 	oldISR = getvect(8);
	 	setvect(8,Timer::timer);
	 	unlock
	#endif
 }

 void restore()
 {
	#ifndef BCC_BLOCK_IGNORE
	 	 lock
	 	setvect(8,oldISR);
	 	unlock
	#endif
 }


void interrupt Timer::timer(...)
{
	if (!Timer::context_switch_on_demand)
		{
			KernelSem::kerList->checkSemaphores();
			tick();
			cntr--;
		}
	if(PCB::flag!=1)
	{

	if (cntr == 0 || Timer::context_switch_on_demand){
	#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
	#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;


		if(PCB::running->status!=FINISH && PCB::running->status!=BLOCKED && PCB::running!=Idle::getIdlePCB())
		{
			PCB::running->status=READY;
			Scheduler::put(PCB::running);
		}

		// scheduler
		PCB::running =Scheduler::get();
		if(PCB::running==0)
		{
			PCB::running = Idle::getIdlePCB();
		}else{
			PCB::running->status=RUNNING;
		}
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		cntr = PCB::running->timeSlice;
	#ifndef BCC_BLOCK_IGNORE
		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	#endif


		if(!PCB::running->queue->isEmpty())
		{
			PCB::running->flag=1;
			NodeQueue* curr=PCB::running->queue->getHead();

			while(curr!=0)
			{
				if(PCB::running->blokiraniGLobalno[curr->data]==0 && PCB::running->blokiraniSignali[curr->data]==0 )
				{


					if(!PCB::running->handleri[curr->data].isEmpty()){
						NodeFun* hend= (PCB::running->handleri[curr->data]).getHead();
						while(hend!=0)
						{

							hend->data();
							hend=hend->next;
						}
					}
					NodeQueue* old = curr;
					curr=curr->next;
					PCB::running->queue->remove(old->data);

				}else
				{

					curr=curr->next;
				}
			}
		}
		PCB::running->flag=0;


	}


	if(!Timer::context_switch_on_demand) oldISR();
	Timer::context_switch_on_demand = 0;
}
}



