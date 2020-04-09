// File: event.h
#ifndef kerev_h_
#define kerev_h_


typedef unsigned char IVTNo;

class PCB;
class KernelEv {
public:
friend class IVTEntry;
 PCB* blocked;
 KernelEv ();
 ~KernelEv ();
 void wait ();
 void signal();

int value;

};
#endif
