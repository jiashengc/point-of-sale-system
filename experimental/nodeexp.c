/*
	Node experimentation (DO NOT ADD)
*/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
    struct node * next;
};

void print_list(struct node * head) {
    struct node * current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

int pop(struct node * head) {
    int retval = -1;
    struct node * next_node = NULL;

    if (head == NULL) {
        return -1;
    }

    next_node = (head)->next;
    retval = (head)->val;
    free(head);
    head = next_node;

    return retval;
}

int remove_by_value(struct node * head, int val) {
    struct node *previous, *current;

    if (head == NULL) {
        return -1;
    }

    if (head->val == val) {
        return pop(head);
    }

    previous = current = head->next;
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

void delete_list(struct node *head) {
    struct node  *current = head, 
            *next = head;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

void push(struct node * head, int val) {
    struct node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = malloc(sizeof(struct node));
    current->next->val = val;
    current->next->next = NULL;
}

int main(void) {
    struct node * test_list = malloc(sizeof(struct node));

    test_list->val = 1;
    //test_list->next = malloc(sizeof(struct node));
    push(test_list, 2);
	push(test_list, 3);
	push(test_list, 4);
	/*
	test_list->next->val = 2;
    test_list->next->next = malloc(sizeof(struct node));
    test_list->next->next->val = 3;
    test_list->next->next->next = malloc(sizeof(struct node));
    test_list->next->next->next->val = 4;
    test_list->next->next->next->next = NULL;
	*/
	
    remove_by_value(test_list, 3);

    print_list(test_list);
    delete_list(test_list);

    return EXIT_SUCCESS;
}