#include "PCB.h"
#include <dos.h>
#include <iostream.h>
#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf



PCB* PCB::running = 0;
List* PCB::PCBlist = new List();
int PCB::id=0;
int PCB::blokiraniGLobalno[16] = {0};
int PCB::flag=0;

PCB::PCB(Thread* thre, Time t, StackSize s):thread(thre), timeSlice(t), size(s)
{
	if (s > 65535) size = 65535/sizeof(unsigned);
	else if (s < 12) size = defaultStackSize/sizeof(unsigned);
	else size = s/sizeof(unsigned);
	threadId=++id;
	st1=0;
	if(threadId!=1) createStartingContext(); // ID = 1 main nit
	PCB::PCBlist->put(this);
	waitForMe = new List();
	//SIGNALI
	if(threadId>2){parrent=PCB::running;}
	else {parrent=0;}
	for(int i=0;i<16;i++)
	{
		if(threadId>2){
			handleri[i]=PCB::running->handleri[i];
		}
		blokiraniSignali[i] = 0;

	}
	//handleri[0] = new ListFun();
	if(threadId==1) handleri[0].put(PCB::hendlerKill);
	//handleri[1].put(PCB::hendlerKill);
//handleri[1]=new ListFun();
	queue = new Queue();

};


Thread* PCB::getThread(){
	return thread;
}

void PCB::createStartingContext()
{
	 #ifndef BCC_BLOCK_IGNORE
		st1 = new unsigned[size];
		st1[size-1] =0x200;
		st1[size-2] = FP_SEG(PCB::wrapper);
		st1[size-3] = FP_OFF(PCB::wrapper); // adresa wrapper metode u memoriji. Unutar werapper-a se izvrsava run metoda, a za nju ne znamo gde je u memoriji.
		sp = FP_OFF(st1+size-12); //svi sacuvani registri st1=0 + size=4096 - 12= 4084
		bp = FP_OFF(st1+size-12);
		ss = FP_SEG(st1+size-12);
     #endif
}

void PCB::wrapper()
{
	PCB::running->thread->run();

	PCB::running->emptyWaitingList();
	PCB::running->status=FINISH;

	if(PCB::running->parrent!=0)PCB::running->parrent->getThread()->signal(1);
	PCB::running->flag=1;


	if(!PCB::running->handleri[2].isEmpty()){
	NodeFun* curr =PCB::running->handleri[2].getHead();
	while(curr!=0)
	{
		curr->data();
		curr=curr->next;
	}
	}
	for(int i=0;i<16;i++)
				{
				if(!PCB::running->handleri[i].isEmpty()){
					 PCB::running->handleri[i].removeAll();

					}
				}
	PCB::running->flag=0;

	//izvrsi za dvojku hendlere
	dispatch();
}

void PCB::emptyWaitingList()
{
	if(PCB::running->waitForMe->isEmpty()==0)
			{
				PCB::running->waitForMe->returntoScheduler();
				PCB::running->waitForMe->removeAll();
			}
}

ID PCB::getThreadId(){return threadId;}

PCB::~PCB()
{
	if(thread!=0)
	{
		delete waitForMe;
		delete[] st1;
			delete queue;


	}


}


void PCB::hendlerKill()
{
	PCB::running->emptyWaitingList();
	PCB::running->status = FINISH;
	if(PCB::running->parrent!=0)PCB::running->parrent->getThread()->signal(1);
	for(int i=0;i<16;i++)
	{
		if(!PCB::running->handleri[i].isEmpty()){
			PCB::running->handleri[i].removeAll();
		}
	}
		delete PCB::running->waitForMe;
		delete[] PCB::running->st1;
		PCB::running->waitForMe = 0;
		PCB::running->st1=0; //ok
		PCB::running->queue->removeAll();
		PCB::flag=0;
		dispatch();
}

