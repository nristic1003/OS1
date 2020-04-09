#include "timer.h"
#include "PCB.h"
#include "kerSem.h"
#include "listKer.h"


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
	if(PCB::running->status==RUNNING){
	if (!Timer::context_switch_on_demand) {
		NodeKer* p = KernelSem::kerList->getHead();
		while(p!=0)
		{
			KernelSem* ker = p->data;
			List* blokirani = ker->PCBblocked;
			blokirani->decreaseTime();
			p=p->next;
		}
		tick();
		cntr--;
	}
	if (cntr == 0 || Timer::context_switch_on_demand) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if(PCB::running->status!=IDLE){
			PCB::running->status=READY;
			Scheduler::put(PCB::running);
		}
		// scheduler
		PCB::running =Scheduler::get();
		if(PCB::running==0)
		{
			PCB::running = PCB::PCBlist->getIdle();
		}
		PCB::running->status=RUNNING;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		cntr = PCB::running->timeSlice;

		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	// poziv stare prekidne rutine
	// koja se nalazila na 08h, a sad je na 60h;
	// poziva se samo kada nije zahtevana promena konteksta
	// tako da se stara rutina poziva
	// samo kada je stvarno doslo do prekida
	if(!Timer::context_switch_on_demand) oldISR();
	Timer::context_switch_on_demand = 0;
} else if(PCB::running->status==FINISH || PCB::running->status==BLOCKED)
{

			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;


			PCB::running =Scheduler::get();
			if(PCB::running==0)
			{
				PCB::running = PCB::PCBlist->getIdle();
			}
			PCB::running->status=RUNNING;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			cntr = PCB::running->timeSlice;

		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
		Timer::context_switch_on_demand = 0;
	}
}



