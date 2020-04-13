
#include "listKer.h"

void ListKernel::put(KernelSem* t)
{
	NodeKer *node= new NodeKer(t);
	if(head==0)
	{
		head=last=node;
	}else{
		last->next = node;
		last=node;
	}
}

NodeKer* ListKernel::getHead() {return head;}

int ListKernel::isEmpty()
{
	if(head!=0) return 1;
	return 0;
}


void ListKernel::removeAll()
{
	while(head!=0)
	{
		NodeKer *p=head;
		head=head->next;
		p->data =0;
		delete p;
	}
	head=0;
	last=0;
}

ListKernel::~ListKernel()
{
	removeAll();

}
