
#ifndef kerSem_h
#define kerSem_h
#include "PCB.h"

typedef unsigned int Time;
class ListKernel;
class List;

class KernelSem {
public:
	static ListKernel* kerList;
	List* PCBblocked;


 KernelSem (int init=1);
 virtual ~KernelSem ();

  int wait (Time maxTimeToWait);
  int signal(int n=0);
 int getVal() const; // Returns the current value of the semaphore
private:
  int semValue;

};

#endif
