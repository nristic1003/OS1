#ifndef ivtEntry_h
#define ivtEntry_h
#include "kerEv.h"

typedef unsigned char IVTNo;


typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry
{

public:
	 pInterrupt oldISR;
	 KernelEv* ker;
	 IVTNo en;
	 static IVTEntry* arrayEntry[256];
	IVTEntry(IVTNo entry,  pInterrupt newInter);
	 ~IVTEntry();
	void signal();
	void callOldmethod();



};
#define PREPAREENTRY(entryNum, callOld)\
	void interrupt interrupt##entryNum(...);\
	IVTEntry IVTEntry##entryNum(entryNum, interrupt##entryNum);\
	void interrupt interrupt##entryNum(...) {\
		IVTEntry##entryNum.signal();\
		if (callOld == 1) IVTEntry##entryNum.callOldmethod();\
	}\



#endif
