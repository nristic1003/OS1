#include "semaphor.h"
#include "kerSem.h"
Semaphore::Semaphore(int init1)
{
	myImpl= new KernelSem(init1);
}

 int Semaphore::signal(int n)
 {
	 return myImpl->signal(n);
 }
 int Semaphore::wait (Time maxTimeToWait)
 {
	 return myImpl->wait(maxTimeToWait);
 }
 int Semaphore::val () const
 {
	 return myImpl->getVal();
 }
Semaphore:: ~Semaphore ()
{
	delete myImpl;
}
