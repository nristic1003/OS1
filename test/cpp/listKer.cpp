#include "listKer.h"
#include "kerSem.h"

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

void ListKernel::checkSemaphores()
{
	NodeKer* p = KernelSem::kerList->getHead();
		while(p!=0)
		{
			KernelSem* ker = p->data;
			List* blokirani = ker->PCBblocked;
			blokirani->decreaseTime();
			p=p->next;
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
	head=last=0;
}

ListKernel::~ListKernel()
{
	removeAll();
}
