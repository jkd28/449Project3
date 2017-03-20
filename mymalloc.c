/*
John Dott
COE 0449: Project 3
3/19/17
*/

#include <stdlib.h>
#include <stdio.h>

//basic linked list node for malloc function
struct Node {
	int size;
	int occupied; // will contain 0 if free space
	struct Node *next;
	struct Node *previous;
};

//global variable declarations
static struct Node *head;	// keep the first node of the linked list
static struct Node *tail;	// keep the last node in the linked list

//allocates memory in the heap based on the best_fit algorithm
void *my_bestfit_malloc(int size){
	void  *new_brk;
	void  *brk;

	// check empty status of Linked list
	if(head == NULL){
		//if empty, set new memory location to the head/tail node
		//allocate space on the heap for node
		brk = sbrk(sizeof(struct Node));
		struct Node *new_node = (struct Node *)brk;	//create a new node in allocated space
		new_node->size = size+sizeof(struct Node);
		new_node->occupied = 1;
		new_node->next = NULL;
		new_node->previous = NULL;
		head = new_node;
		tail = head;
		brk = sbrk(size); //allocate space for return
		return brk; // return the value of the new memory space to be used by calling function
	}else {
		// search for best fit node
		struct Node *best = NULL;
		struct Node *current_node = head; //start traversal at the head
		//size = size + sizeof(struct Node);
		while(current_node != NULL){	// while we have no best choice
			if (size == current_node->size){
				best = current_node;  //immediate best fit
				break;
			}
			if (((best == NULL )||(current_node->size < best->size)) && (current_node->occupied == 0) && (current_node->size > size)){
				best = current_node;
			}
			current_node = current_node->next;
		}

		//best does not exist, create new node and allocate more space
		if(best == NULL){
			brk = sbrk(sizeof(struct Node)); //allocate space on heap for node
			struct Node *new_node = (struct Node *)brk;
			//create new node with space allocated
			new_node->size = size+sizeof(struct Node);
			new_node->occupied = 1;
			new_node->next = NULL;
			new_node->previous = tail;
			tail = new_node;
			brk = sbrk(size);	//allocate space for return
			return brk;
		} else {
			//we have a best fit in empty space
			best->occupied=1;
			return (void *)(best + sizeof(struct Node));
		}
	}
}

void my_free(void *ptr){
	struct Node *current = head;	//get head node
	struct Node *node = ptr; // get the pointer to the node in memory section
	node = (char *)node-sizeof(struct Node);
	void *brk = sbrk(0); //check current value of brk
	//find node in LL
	while(current != node){
		current = current->next;
		if(current == NULL){return;}	//node is not found, free FAILED
	}

	//make node free space
	current->occupied = 0;

	//check previous and next for free space and coalesce if necessary
	while((current->previous)->occupied == 0){
		//if previous is free
		(current->previous)->next = current->next;
		(current->next)->previous = current->previous;
		(current->previous)->size = (current->previous)->size + current->size; //set precious node's size to total new size
		current = current->previous;
		if(current->previous == NULL){break;}
	}
	while((current->next)->occupied == 0){
		//if next is free after the elimination of previous
		current->size= current->size + (current->next)->size;	//combine sizes
		current->next=((current->next)->next);
		current = current->next;
		if(current->next == NULL){break;}
	}
	//free space if tail is unoccupied
	while((tail->occupied == 0) && (tail+tail->size == brk)){
		current = tail;
		tail = tail->previous;
		brk = sbrk(-1 * current->size);
	}
	//check status of head
	while(current->previous != NULL){
		current = current->previous;
	}
	head = current;
	//check status of tail
	while(current->next != NULL){
		current = current->next;
	}
	tail = current;
	return;
}
