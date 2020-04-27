#ifndef queue_h_
#define queue_h_

#include "thread.h"


struct NodeQueue
{
  int data;
  NodeQueue *next;
  NodeQueue (int p, NodeQueue *n=0):data(p), next(n){}
 ~NodeQueue(){}
};


class Queue
{
private:
	NodeQueue *head;
	NodeQueue *last;

public:
	Queue()
	{
		head=last=0;
	}

	void put(int t);
	int getFirst();
	void removeAll();
	void remove(int x);
	int isEmpty();
	NodeQueue* getHead(){return head;}

	~Queue();

};

#endif
