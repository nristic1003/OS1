#include "IVTEntry.h"
#include <dos.h>
#include <iostream.h>
#include "kerEv.h"

IVTEntry* IVTEntry::arrayEntry[256]={0};
IVTEntry::IVTEntry(IVTNo entry,  pInterrupt newInter)
{
	ker=0;
	en = entry;
	#ifndef BCC_BLOCK_IGNORE
	oldISR = getvect(entry);
	setvect(entry,newInter);
	#endif
	arrayEntry[entry] = this;
}

void IVTEntry::signal()
{
	ker->signal();
}

void IVTEntry::callOldmethod()
{
	oldISR();
}
IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(en, oldISR);
#endif
	arrayEntry[en] = 0;
}
