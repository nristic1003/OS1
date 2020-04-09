#include "List.h"
#include "SCHEDULE.H"
#include "PCB.h"
void List::put(PCB* t)
{
	Node *node= new Node(t);
	if(head==0)
	{
		head=last=node;
	}else{
		last->next = node;
		last=node;
	}
}
void List::putBlocked(PCB* t, int time)
{
	Node *node= new Node(t, 0,time);
	if(head==0)
	{
		head=last=node;
	}else{
		last->next = node;
		last=node;
	}
}


void List::decreaseTime()
{
	Node* p=head;
	while(p!=0)
	{
		p->timeWait -=1;

		if(p->timeWait==0)
		{
			p->data->status=READY;
			Scheduler::put(p->data);
		}
		p=p->next;
	}
}

PCB* List::getFirst()
{
	if(head!=0)
	{
		Node *p = head;
		head=head->next;
		return p->data;
	}
	return 0;
}

int List::isEmpty()
{
	if(head==0) return 1;
	return 0;
}




void List::returntoScheduler()
{
		if(head!=0) {

		Node* blockedPCB=head;
		while(blockedPCB!=0)
			{
					blockedPCB->data->status= READY;
					Scheduler::put(blockedPCB->data);
					blockedPCB = blockedPCB->next;
			}

	}
}

PCB* List::getIdle()
{
	if(head!=0)
	{
		Node*p = head;
		while(p->data->status!=IDLE) p=p->next;
		return p->data;
	}
	return 0;

}
PCB* List::getByID(ID id)
{
	Node*p = head;
	while(p->data->getThreadId()!=id) p=p->next;
	return p->data;

}

void List::removeAll()
{
		while(head!=0)
		{
			Node *p=head;
			head=head->next;
			delete p;
		}

}
List::~List()
{
	removeAll();
	last=0;
}
