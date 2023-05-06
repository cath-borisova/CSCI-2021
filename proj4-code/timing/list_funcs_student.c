#include <stdlib.h>
#include "linked_list.h"

int list_find_student(list_t *list, int value) {
    // TODO: Write an algorithmically more efficient version of list_find
  if(list == NULL){		//check if the list is empty, and return -1 if so
  	return -1;
  }
   int index = 0;		//index of linked list
    node_t *node = list -> head;	//create a temporary node object that is set to the head node
    while(node != NULL){		//keep searching for the node with the value until there are no more nodes
    	if (node -> value == value){	//if found the node
    		return index;		// return the index of the node in the linked list
    	} else {
    		index ++;		//otherwise look at the next node
    		node = node -> next;
    	}
    	}
    	free(node);			//if value not found, return -1
    	return -1;
    	
}
