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
		printf("Adding Head\n");
		//allocate space on the heap
		brk = sbrk(size+sizeof(struct Node));
		struct Node *new_node = (struct Node *)brk+1;	//create a new node in allocated space
		new_node->size = size+sizeof(struct Node);
		new_node->occupied = 1;
		new_node->next = NULL;
		new_node->previous = NULL;
		head = new_node;
		tail = head;
		return brk + sizeof(struct Node); // return the value of the new memory space to be used by calling function
	}else {
		int count = 0;
		printf("Adding other\n");
		// search for best fit node
		struct Node *best = NULL;
		struct Node *current_node = head; //start traversal at the head
		//size = size + sizeof(struct Node);
		while(current_node != NULL){	// while we have no best choice
			printf("Stuck %d\n", ++count);
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
			struct Node *new_node = (struct Node *)brk +1;
			//create new node with space allocated
			new_node->size = size+sizeof(struct Node);
			new_node->occupied = 1;
			new_node->next = NULL;
			new_node->previous = tail;
			tail = new_node;
			return (new_node + sizeof(struct Node));
		} else {
			//we have a best fit in empty space
			best->occupied=1;
			return (best + sizeof(struct Node));
		}
	}
}

void my_free(void *ptr){
	struct Node *current = head;	//get head node
	struct Node *node = ptr - sizeof(struct Node); // get the pointer to the node in memory section
	void *brk = sbrk(0); //check current value of brk
	int size = node->size;
	//find node in LL
	while(current != node){
		current = current->next;
	}
	//delete node
	(current->next)->previous = current->previous;
	(current->previous)->next = current->next;
	//check previous and next for free space and coalesce if necessary

	//free space of that size if that node touched brk
	if(node+size == brk){
		size = -1 * size;
		brk = sbrk(size);
		return;
	}
}
