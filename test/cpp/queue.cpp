#include "Queue.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include <iostream.h>

void Queue::put(int t)
{
	NodeQueue *node= new NodeQueue(t);
	if(head==0)
	{
		head=last=node;
	}else{
		last->next = node;
		last=node;
	}
}


int Queue::getFirst()
{
	if(head!=0)
		return head->data;
	return 0;
}

int Queue::isEmpty()
{
	if(head==0) return 1;
	return 0;
}
void Queue::remove(int x)
{
	if(head!=0)
	{
		NodeQueue* prev=0;
		NodeQueue* curr=head;
		while(curr->data!=x)
		{
			prev=curr;
			curr=curr->next;
		}
		if(curr==head)
		{
			head=head->next;
			delete curr;
			return;
		}else if(curr==last)
		{
			last=prev;
			last->next=0;
			delete curr;
			return;
		}else{
			prev->next=curr->next;
			delete curr;
			return;
		}

	}
}
 void Queue::removeAll()
{
	while(head!=0)
	{
		NodeQueue *p=head;
		head=head->next;
		p->data=0;
		delete p;
	}
	head=0; last=0;
}
Queue::~Queue()
{
	removeAll();

}
