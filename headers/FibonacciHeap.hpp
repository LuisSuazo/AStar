#ifndef FibonacciHeap_HPP
#define FibonacciHeap_HPP
#include "Estructura.hpp"

using namespace std; 
  
//https://github.com/robinmessage/fibonacci

class node {

private:
	node* prev;
	node* next;
	node* child;
	node* parent;
	Expandido* value;
	int degree;
	bool marked;

public:
	friend class FibonacciHeap;
	node* getPrev() {return prev;}
	node* getNext() {return next;}
	node* getChild() {return child;}
	node* getParent() {return parent;}
	Expandido* getValue() {return value;}
	bool isMarked() {return marked;}

	bool hasChildren() {return child;}
	bool hasParent() {return parent;}
};

class FibonacciHeap {
protected:
	node* heap;
public:

	FibonacciHeap() {
		heap=_empty();
	}
	~FibonacciHeap() {
		if(heap) {
			_deleteAll(heap);
		}
	}
	node* insert(Expandido* value) {
		node* ret=_singleton(value);
		heap=_merge(heap,ret);
		return ret;
	}

	void merge(FibonacciHeap& other) {
		heap=_merge(heap,other.heap);
		other.heap=_empty();
	}

	bool empty() {
		return heap==NULL;
	}

	Expandido* top() {
		return heap->value;
	}

	Expandido* pop() {
		node* old=heap;
		heap=_pop(heap);
		Expandido* ret=old->value;
		delete old;
		return ret;
	}

	void decreaseKey(node* n,Expandido* value) {
		heap=_decreaseKey(heap,n,value);
	}

	node* find(Expandido* value) {
		return _find(heap,value);
	}
private:
	node* _empty() {
		return NULL;
	}

	node* _singleton(Expandido* value) {
		node* n=new node;
		n->value=value;
		n->prev=n->next=n;
		n->degree=0;
		n->marked=false;
		n->child=NULL;
		n->parent=NULL;
		return n;
	}

	node* _merge(node* a,node* b) {
		if(a==NULL)return b;
		if(b==NULL)return a;
		if(a->value->key>b->value->key) {
			node* temp=a;
			a=b;
			b=temp;
		}
		node* an=a->next;
		node* bp=b->prev;
		a->next=b;
		b->prev=a;
		an->prev=bp;
		bp->next=an;
		return a;
	}

	void _deleteAll(node* n) {
		if(n!=NULL) {
			node* c=n;
			do {
				node* d=c;
				c=c->next;
				_deleteAll(d->child);
				delete d;
			} while(c!=n);
		}
	}
	
	void _addChild(node* parent,node* child) {
		child->prev=child->next=child;
		child->parent=parent;
		parent->degree++;
		parent->child=_merge(parent->child,child);
	}

	void _unMarkAndUnParentAll(node* n) {
		if(n==NULL)return;
		node* c=n;
		do {
			c->marked=false;
			c->parent=NULL;
			c=c->next;
		}while(c!=n);
	}

	node* _pop(node* n) {
		_unMarkAndUnParentAll(n->child);
		if(n->next==n) {
			n=n->child;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n=_merge(n->next,n->child);
		}
		if(n==NULL)return n;
		node* trees[64]={NULL};
		
		while(true) {
			if(trees[n->degree]!=NULL) {
				node* t=trees[n->degree];
				if(t==n)break;
				trees[n->degree]=NULL;
				if(n->value->key<t->value->key) {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					_addChild(n,t);
				} else {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					if(n->next==n) {
						t->next=t->prev=t;
						_addChild(t,n);
						n=t;
					} else {
						n->prev->next=t;
						n->next->prev=t;
						t->next=n->next;
						t->prev=n->prev;
						_addChild(t,n);
						n=t;
					}
				}
				continue;
			} else {
				trees[n->degree]=n;
			}
			n=n->next;
		}
		node* min=n;
		node* start=n;
		do {
			if(n->value->key<min->value->key)min=n;
			n=n->next;
		} while(n!=start);
		return min;
	}

	node* _cut(node* heap,node* n) {
		if(n->next==n) {
			n->parent->child=NULL;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->marked=false;
		return _merge(heap,n);
	}

	node* _decreaseKey(node* heap,node* n,Expandido* value) {
		if(n->value->key<value->key)return heap;
		n->value=value;
		if(n->parent) {
			if(n->value->key<n->parent->value->key) {
				heap=_cut(heap,n);
				node* parent=n->parent;
				n->parent=NULL;
				while(parent!=NULL && parent->marked) {
					heap=_cut(heap,parent);
					n=parent;
					parent=n->parent;
					n->parent=NULL;
				}
				if(parent!=NULL && parent->parent!=NULL)parent->marked=true;
			}
		} else {
			if(n->value->key < heap->value->key) {
				heap = n;
			}
		}
		return heap;
	}

	node* _find(node* heap,Expandido* value) {
		node* n=heap;
		if(n==NULL)return NULL;
		do {
			if(n->value->key==value->key)return n;
			node* ret=_find(n->child,value);
			if(ret)return ret;
			n=n->next;
		}while(n!=heap);
		return NULL;
	}
};

#endif