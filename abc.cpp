#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
	long id;
	char mail[31];
	char passwd[21];
} Account;

typedef struct _list{
	void* data;
	struct _list* next;
}Node,*List;

int InitList(List &l){
	l = new Node();
	if(!l)	return 0;
	l->data = NULL;
	l->next = NULL;
	return 1;
}

int ClearList(List &l){
	List p = l;
	void *q = l->data;

}

int main(void){
	return 0;
}