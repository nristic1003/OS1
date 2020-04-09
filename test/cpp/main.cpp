#include <iostream.h>
#include <stdlib.h>
#include <malloc.h>
#include "PCB.h"
#include "thread.h"
#include "timer.h"
#include "semaphor.h"
#include "kerSem.h"
#include "idle.h"
#include <dos.h>

#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf
class A :public Thread{
public:
// Semaphore * sem;

	A():Thread(defaultStackSize,5){
		//sem = new Semaphore(0);
	};

	virtual void run();

	virtual ~A(){

	}


};

void A::run(){

#ifndef BCC_BLOCK_IGNORE
	//sem->wait(50);

	for (int i =0; i < 30; ++i) {
					lock
					cout<<"ID:"<<this->getId()<<" i = "<<i<<endl;
					unlock
					for (int k= 0; k<10000; ++k)
						for (int j = 0; j <30000; ++j);

				}

#endif
}


class B :public Thread{


public:

	//Semaphore* sem;
	 B():Thread(defaultStackSize,5){

		 //sem = new Semaphore(1);
	 };

	virtual void run();

	virtual ~B(){


	}
};

void B::run(){

#ifndef BCC_BLOCK_IGNORE
	for (int i =0; i < 30; ++i) {
	//	sem->wait(20);
		lock
		cout<<"ID:"<<this->getId()<<" i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);


	}

#endif
	delete this;
}


extern int userMain(int argc, char* argv[]);
int main(int argc, char* argv[])
{

#ifndef BCC_BLOCK_IGNORE
	lock
	inic();
	PCB* mainPCB = new PCB(0,1,0);
	PCB::PCBlist->put(mainPCB);

	cout<<("Kreiran main") <<endl;
	PCB::running = mainPCB;
	mainPCB->status = RUNNING;

	Idle* idl = new Idle(defaultStackSize, 1);
	unlock
#endif

	int ret =userMain(argc, argv);


	restore();

	cout<<"Happy End!"<<endl;

	return ret;




}
