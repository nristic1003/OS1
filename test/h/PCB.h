#ifndef _PCB_h
#define _PCB_h

#include "thread.h"
#include "List.h"
#include  "queue.h"
#include "funList.h"


enum st{READY,IDLE, BLOCKED, RUNNING, CREATED, FINISH};

class PCB
{
public:
	unsigned sp;
	unsigned bp;
	unsigned ss;
	static  List* PCBlist;
	static	PCB* running;
	List* waitForMe;
	Time timeSlice;
	friend class Thread;
	enum st status;

	static int flag;
	ListFun* handleri[16];
	int blokiraniSignali[16];
	static int blokiraniGLobalno[16];
	Queue* queue;
	static void hendlerKill();

	PCB(Thread* thre, Time t, StackSize s);

	ID getThreadId();
	static void wrapper();
	void createStartingContext();
	~PCB();
	Thread* getThread();
	void emptyWaitingList();

private:
	Thread* thread;
	StackSize size;
	unsigned* st1;
	static int id;
	ID threadId;

	PCB* parrent;

};


#endif
