#ifndef idle_h_
#define idle_h_
#include "thread.h"

class Idle: public Thread
{
public:

	friend class PCB;
	friend class Thread;
	Idle(StackSize stackSize = defaultStackSize, Time timeSlice =defaultTimeSlice);
	 virtual void run();
	 virtual ~Idle();

};
#endif
