#include "PCB.h"
#include <dos.h>



PCB* PCB::running = 0;
List* PCB::PCBlist = new List();
int PCB::id=0;


PCB::PCB(Thread* thre, Time t, StackSize s):thread(thre), timeSlice(t), size(s)
{
	threadId=++id;
	st1 =0;
	if(threadId!=1) createStartingContext(); // ID = 1 main nit

};

void PCB::createStartingContext()
{
	    #ifndef BCC_BLOCK_IGNORE
		st1 = new unsigned[size];

		st1[size-1] =0x200;//setovan I fleg u
							  // pocetnom PSW-u za nit
		st1[size-2] = FP_SEG(PCB::wrapper);
		st1[size-3] = FP_OFF(PCB::wrapper); // adresa wrapper metode u memoriji. Unutar werapper-a se izvrsava run metoda, a za nju ne znamo gde je u memoriji.

		sp = FP_OFF(st1+size-12); //svi sacuvani registri st1=0 + size=4096 - 12= 4084
		bp = FP_OFF(st1+size-12);
		ss = FP_SEG(st1+size-12);
		 //pri ulasku u interrupt

		#endif
}

void PCB::wrapper()
{
	PCB::running->thread->run();
	PCB::running->status=FINISH;
	dispatch();
}

ID PCB::getThreadId(){return threadId;}


PCB::~PCB()
{
	delete[] st1;
}
