#include <iostream.h>
#include "PCB.h"
#include "thread.h"
#include "timer.h"
#include "semaphor.h"
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
 Semaphore * sem;

	A():Thread(defaultStackSize,5){
		sem = new Semaphore(0);
	};

	virtual void run();

	virtual ~A(){

	}


};

void A::run(){

#ifndef BCC_BLOCK_IGNORE
	sem->wait(50);
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

	Semaphore* sem;
	 B():Thread(defaultStackSize,5){

		 sem = new Semaphore(1);
	 };

	virtual void run();

	virtual ~B(){


	}
};

void B::run(){

#ifndef BCC_BLOCK_IGNORE
	for (int i =0; i < 30; ++i) {
		sem->wait(20);
		lock
		cout<<"ID:"<<this->getId()<<" i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);


	}
#endif
}
//extern int cntr;
int main()
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


	//cntr = mainPCB->timeSlice;
		B* t1 = new B();
		//B* t2 = new B();
		A* t3 = new A();
		A* t4 = new A();
		t1->start();
		//t2->start();
		t3->start();
		t4->start();
	unlock
#endif



Semaphore * sem2 = new Semaphore(0);
	sem2->wait(5);

#ifndef BCC_BLOCK_IGNORE
    for (int i = 0; i < 30; ++i) {
      lock
      cout<<"main "<<i<<endl;
      unlock
    for (int j = 0; j< 30000; ++j)
        for (int k = 0; k < 30000; ++k);

    }





#endif

	restore();





	return 0;




}
