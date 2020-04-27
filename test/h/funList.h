#ifndef funList_h_
#define funList_h_

#include "thread.h"


struct NodeFun
{
  SignalHandler data;
  NodeFun *next;
  NodeFun (SignalHandler p, NodeFun *n=0):data(p), next(n){}
 ~NodeFun(){}
};


class ListFun
{
private:
	NodeFun *head;
	NodeFun *last;

public:
	ListFun()
	{
		head=last=0;
	}

	void put(SignalHandler t);
	SignalHandler getFirst();
	void removeAll();
	int isEmpty();
	void iterator();
	void swap(SignalHandler p1, SignalHandler p2);
	NodeFun* getHead(){return head;}

	~ListFun();

};

#endif
