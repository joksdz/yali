#include <stdio.h>
#include <stdlib.h>


typedef struct Node{

int data; 
struct Node  *next;
struct Node *prev;
}Node;

void init(Node *head){
	head->next=head->prev=NULL;
}

void fill(Node *head){
int c= 0;
Node *current= head;
do{
printf("enter your data:\n");
int v ;
Node *node= malloc(sizeof(Node));
scanf("%d",&v);
node->data =v;

node->prev= current;
node->next= NULL;

current->next = node;
current= node;
printf("are you done (1=yes/0=no)");
scanf("%d",&c);
}while(c!=1);


}

void disp(Node *head){
Node *current = head;
if (head->next !=NULL){

current = head->next;
}

while(current!=NULL){

printf("%d->",current->data);
current = current->next;
}
printf("NULL\n");

}


int main(){
Node head;

init(&head);
fill(&head);
disp(&head);



	return 0 ;
}
