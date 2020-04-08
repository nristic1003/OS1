#ifndef list_h
#define list_h

class PCB;
struct Node
{
  PCB *data;
  int timeWait;
  Node *next;
  Node (PCB *p, Node *n=0, int t=0):data(p), next(n), timeWait(t){}
 ~Node(){}
};


class List
{
private:
	 Node *head;
	Node *last;

public:
	List()
	{
		head=last=0;
	}

	void put(PCB *t);
	void putBlocked(PCB *p, int time);
	PCB* getFirst();
	PCB* getIdle();
	void remove(PCB* p);
	void decreaseTime();
	int isEmpty();
	void iterator();
	//PCB* getByID(ID id);

	~List();

};

#endif
