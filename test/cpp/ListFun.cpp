#include "funList.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include <iostream.h>
#include <dos.h>

typedef void (*SignalHandler)();
#define lock asm{\
 		pushf;\
 		cli;\
}

// dozvoljava prekide
#define unlock asm popf




void ListFun::put(SignalHandler t)
{
	NodeFun *node= new NodeFun(t);
	if(head==0)
	{
		head=last=node;
	}else{
		last->next = node;
		last=node;
	}
}


SignalHandler ListFun::getFirst()
{
	if(head!=0)
		return head->data;
	return 0;
}

int ListFun::isEmpty()
{
	if(head==0) return 1;
	return 0;
}
void ListFun::swap(SignalHandler p1, SignalHandler p2)
{
	if(head!=0 && p1!=0 && p2!=0 && p1!=p2)
	{
		NodeFun* curr=head;
		NodeFun* prev =head;

		while(curr->data!=p1 && prev->data!=p2)
			{
				curr=curr->next;
				prev=prev->next;
			}
		if(curr->data==p1)
		{
			while(prev!=0 && prev->data!=p2) prev=prev->next;
		}else
		{
			while(curr!=0 && curr->data!=p1) curr=curr->next;
		}
		if(curr==0 || prev==0) return;
		if(curr->data==p1 &&prev->data ==p2 )
		{
			curr->data=p2;
			prev->data=p1;
		}
	}
}

 void ListFun::removeAll()
{
	while(head!=0)
	{

		NodeFun *p=head;
		head=head->next;
		p->data=0;
		delete p;
	}
	head=0; last=0;
}

void ListFun::operator =(const ListFun& list) {
	NodeFun* curr=list.head;
	while(curr!=0)
	{
		put(curr->data);
		curr=curr->next;
	}
}

ListFun::~ListFun()
{
	removeAll();
	/*lock
	cout<<"delete"<<endl;
	unlock*/

}
