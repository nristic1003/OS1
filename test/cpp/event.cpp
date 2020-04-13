#include "event.h"
#include "IVTEntry.h"
#include "kerEv.h"

Event::Event(IVTNo ivtNo)
{
	myImpl = new KernelEv();
	IVTEntry::arrayEntry[ivtNo]->ker = myImpl;
}

void Event::wait()
{
	myImpl->wait();
}

void Event::signal()
{
	myImpl->signal();
}
Event::~Event()
{

	delete myImpl;

}
