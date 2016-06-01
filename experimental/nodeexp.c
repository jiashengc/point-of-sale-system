/*
	Node experimentation (DO NOT ADD)
*/

#include <stdio.h>

void print_list(node_t * head) {
    node_t * current = head;
	if(current == NULL){
		printf("There's nothing in this list.\n");
	}
	else{
		do{
			printf("%d\n", current->val);
			current = current->next;
		}while(current != NULL);
	}
}

int pop(node_t * head) {
    int retval = -1;
    node_t * next_node = NULL;

    if (head == NULL) {
        return -1;
    }

    next_node = (head)->next;
    retval = (head)->val;
    free(head);
    head = next_node;

    return retval;
}

int remove_by_value(node_t * head, int val) {
    node_t *previous, *current;

    if (head == NULL) {
        return -1;
    }

    if ((head)->val == val) {
        return pop(head);
    }

    previous = current = (head)->next;
    while (current) {
        if (current->val == val) {
            previous->next = current->next;
            free(current);
            return val;
        }

        previous = current;
        current  = current->next;
    }
    return -1;
}

void delete_list(node_t *head) {
    node_t  *current = head, 
            *next = head;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

void push(node_t *head, int input){
	if(head == NULL){
		head = malloc(sizeof(node_t));
		head->val = input;
	}
	else{
		node_t *current = head;
		while(current->next != NULL)
			current = current->next;
		current->next = malloc(sizeof(node_t));
		current->next->val = input;
	}		
}

//This function is used to add another item to a linked list

int main(){
	//Test for roundToNearest5
	/*
	int i;
	for(i = 0; i < 1000; i++){
		double value = (double)i / 100;
		printf("%.2f --> %.2f\n", value, roundToNearest5(value));
	}
	*/
	
	node_t * test_list;// = malloc(sizeof(node_t));

    //test_list->val = 1;
	int i;
	for(i = 0; i < 10; i++)
		push(test_list, i);
    remove_by_value(test_list, 3);

    print_list(test_list);
    delete_list(test_list);
	return 0;
}