#ifndef idle_h_
#define idle_h_
#include "thread.h"

class Idle: public Thread
{
public:

	friend class PCB;
	friend class Thread;
	Idle();
	 virtual void run();
	 virtual ~Idle();

	 static Thread*  idle;
	 static PCB* getIdlePCB();
	 static void idleDelete();
};
#endif
