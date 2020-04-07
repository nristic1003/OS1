#include "List.h"

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
	if(head!=0) return 1;
	return 0;
}
void List::removeFirst()
{
	if(head!=0)
	{
		Node *p =head;
		head=head->next;
		delete p;
	}
}
void List::removeLast()
{
	if(head!=0)
	{
		Node *p = head;
		while(p->next!=last) p=p->next;
		last = p;
		p=last->next;
		delete p;
	}
}

void List::iterator()
{
	Node *p = head;
	while(p!=0)
	{
		p=p->next;
	}
}
List::~List()
{
	while(head!=0)
	{
		Node *next=head;
		head=head->next;
		delete next;
	}
	last=0;
}
