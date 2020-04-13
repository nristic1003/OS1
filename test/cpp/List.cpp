#include "List.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include <iostream.h>
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
void List::remove(PCB* pcb)
{
	 if(head==0)
	        return;
	    if(head->data == pcb){
	        if(head->next == 0){
	            delete(head);
	            head = last = 0;
	            return;
	        }
	        Node* old = head;
	        head = head->next;
	        delete old;
	    }
	    else{
	        Node* cur = head;
	        Node* prev = head;
	        while((cur->data!=pcb) && (cur->next!=0)){
	            prev = cur;
	            cur = cur->next;
	        }

	        if((cur->data!=pcb )&& (cur->next==0))
	            return;
	        else if((cur->data==pcb) && (cur->next==0)){
	            last=prev;
	            prev->next=0;
	            delete(cur);
	        }
	        else{
	            prev->next = cur->next;
	            delete(cur);
	        }
	    }
}

void List::decreaseTime()
{
	Node* p=head;
	while(p!=0)
	{

		p->timeWait=p->timeWait-1;

		if(p->timeWait==0)
		{
			Node* old = p;
			p=p->next;
			old->data->status=READY;
			Scheduler::put(old->data);
			remove(old->data);
		}else
			p=p->next;
	}
}

PCB* List::getFirst()
{
	if(head!=0)
		return head->data;
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
					remove(blockedPCB->data);
					blockedPCB = blockedPCB->next;

			}

	}
}

PCB* List::getByID(ID id)
{
	Node*p = head;
	while(p!=0)
	{
		if(p->data->getThreadId()==id) return p->data;
		p=p->next;
	}
	return 0;


}

 void List::removeAll()
{
		while(head!=0)
		{
			Node *p=head;
			head=head->next;
			p->data=0;
			delete p;
		}
		head=0; last=0;
}
List::~List()
{
	removeAll();

}
