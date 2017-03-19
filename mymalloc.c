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
		printf("Adding Head\n");
		//if empty, set new memory location to the head/tail node
		struct Node *new_node = {size, 1, NULL, NULL};	//create a new node for the pointer of allocated space
		head = new_node;
		tail = head;
		//allocate space on the heap
		brk = sbrk(size+sizeof(struct Node));
		return (char *)brk + sizeof(struct Node); // return the value of the new memory space to be used by calling function
	}else {
		printf("Adding other\n");
		// search for best fit node
		struct Node *best = NULL;
		struct Node *current_node = &head; //start traversal at the head

		while(&current_node != NULL){	// while we have no best choice
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
			brk = sbrk(size+sizeof(struct Node)); //allocate all space on heap
			
		}
	}
}

void my_free(void *ptr){
	struct Node *node = ptr - sizeof(struct Node); // get the node that points to this memory section
	void * brk = sbrk(0); //check current value of brk
	int size = node->size;
	//find node in LL
	//delete node
		//check previous and next for free space and coalesce if necessary
	//free space of that size if that node touched brk
	if(node+size == brk){
		size = -1 * size;
		brk = sbrk(size);
		return;
	}
}
