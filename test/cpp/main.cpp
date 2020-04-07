#include <iostream.h>
#include "PCB.h"
#include "thread.h"
#include "timer.h"
#include <dos.h>

#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf
class A :public Thread{
public:


	A():Thread(defaultStackSize,5){ };

	virtual void run();

	virtual ~A(){

	}


};

void A::run(){

#ifndef BCC_BLOCK_IGNORE
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

	 B():Thread(defaultStackSize,5){};

	virtual void run();

	virtual ~B(){


	}
};

void B::run(){

#ifndef BCC_BLOCK_IGNORE
	for (int i =0; i < 30; ++i) {
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

	//cntr = mainPCB->timeSlice;
		B* t1 = new B();
		B* t2 = new B();
		A* t3 = new A();
		A* t4 = new A();
		t1->start();
		t2->start();
		t3->start();
		t4->start();
	unlock
#endif




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
