#include <stdlib.h>
#include "mylist.h"

/*
 * Create a node that holds the given data pointer,
 * and add the node to the front of the list.
 *
 * Note that this function does not manage the lifetime of the object
 * pointed to by 'data'.
 * 
 * It returns the newly created node on success and NULL on failure.
 */
struct Node *addFront(struct List *list, void *data){
    struct Node *new_node = malloc(sizeof(struct Node));
    if (!new_node){
        return NULL; 
    }
    new_node->data = data; 
    new_node->next = list->head;
    list->head = new_node; 
    return new_node; 
}

/*
 * Traverse the list, calling f() with each data item.
 */
void traverseList(struct List *list, void (*f)(void *)){
    struct Node *current = list->head; 
    while(current){
        f(current->data); 
        current = current->next;  
    }
}

/*
 * Flip the sign of the double value pointed to by 'data' by
 * multiplying -1 to it and putting the result back into the memory
 * location.
 */
void flipSignDouble(void *data){
    double *value = (double *)data; 
    *value = -*value; 
}

/*
 * Compare two double values pointed to by the two pointers.
 * Return 0 if they are the same value, 1 otherwise.
 */
int compareDouble(const void *data1, const void *data2){
    double value1 = *(double *)data1; 
    double value2 = *(double *)data2; 
    return (!(value1 == value2)); 
}

/*
 * Traverse the list, comparing each data item with 'dataSought' using
 * 'compar' function.  ('compar' returns 0 if the data pointed to by
 * the two parameters are equal, non-zero value otherwise.)
 *
 * Returns the first node containing the matching data, 
 * NULL if not found.
 */
struct Node *findNode(struct List *list, const void *dataSought,
	int (*compar)(const void *, const void *)){
        struct Node *current = list->head; 
        while (current){
            if (compar(current->data, dataSought) == 0){
                return current; 
            }
            current = current->next; 
        }
        return NULL; 
    }

/*
 * Remove the first node from the list, deallocate the memory for the
 * ndoe, and return the 'data' pointer that was stored in the node.
 * Returns NULL is the list is empty.
 */
void *popFront(struct List *list){
    struct Node *first_node = list->head; 
    if (list->head == NULL){
        return NULL; 
    }
    void *first_data = first_node->data; 
    list->head = first_node->next; 
    free(first_node); 
    return(first_data); 
}

/*
 * Remove all nodes from the list, deallocating the memory for the
 * nodes.  You can implement this function using popFront().
 */
void removeAllNodes(struct List *list){
    while(list->head){
        popFront(list); 
    }
}

/*
 * Create a node that holds the given data pointer,
 * and add the node right after the node passed in as the 'prevNode'
 * parameter.  If 'prevNode' is NULL, this function is equivalent to
 * addFront().
 *
 * Note that prevNode, if not NULL, is assumed to be one of the nodes
 * in the given list.  The behavior of this function is undefined if
 * prevNode does not belong in the given list.
 *
 * Note that this function does not manage the lifetime of the object
 * pointed to by 'data'.
 * 
 * It returns the newly created node on success and NULL on failure.
 */
struct Node *addAfter(struct List *list, 
	struct Node *prevNode, void *data){
        if (prevNode == NULL){
            return addFront(list, data); 
        }
        struct Node *new_node = malloc(sizeof(struct Node));
        if (!new_node){
            return NULL; 
        }
        new_node->data = data; 
        new_node->next = prevNode->next;
        prevNode->next = new_node; 
        return new_node; 
    }

/* 
 * Reverse the list.
 *
 * Note that this function reverses the list purely by manipulating
 * pointers.  It does NOT call malloc directly or indirectly (which
 * means that it does not call addFront() or addAfter()).
 *
 * Implementation hint: keep track of 3 consecutive nodes (previous,
 * current, next) and move them along in a while loop.  Your function
 * should start like this:


      struct Node *prv = NULL;
      struct Node *cur = list->head;
      struct Node *nxt;

      while (cur) {
          ......


 * And at the end, prv will end up pointing to the first element of
 * the reversed list.  Don't forget to assign it to list->head.
 */
void reverseList(struct List *list){
    struct Node *prv = NULL;
    struct Node *cur = list->head;
    struct Node *nxt;

    while(cur){
        nxt = cur->next; 
        cur->next = prv; 
        prv = cur; 
        cur = nxt; 
    }
    list->head = prv; 
}

