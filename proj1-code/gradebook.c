#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gradebook.h"

gradebook_t *create_gradebook(const char *class_name) {
    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    if (new_book == NULL) {
        return NULL;
    }

    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;
    return new_book;
}

const char *get_gradebook_name(const gradebook_t *book) {
    // TODO: Not yet implemented
    return book->class_name;
}
void addRecursion(node_t* node, const char *name, int score){
	if(strcmp(node->name, name) == 0){
		node->score = score;
	} else {
		if(node->left != NULL){
			if(strcmp(node->name, name)>0){
				addRecursion(node->left, name, score);
			}
		}
		if(node->right != NULL){
			if (strcmp(name, node->name) > 0){
				addRecursion(node->right, name, score);
			}
		}
		if(node->right == NULL){ //only make a new node when node->rigt is null
			if(strcmp(name, node->name) >0){
				node_t *new_node = malloc(sizeof(node_t));
				node->right = new_node;
				strcpy(new_node->name, name);
				new_node->score = score;
				new_node->left = NULL;
				new_node->right = NULL;
//			break;
			}
		}
		if(node->left == NULL){
			if(strcmp(node->name, name) > 0){
				node_t *new_node = malloc(sizeof(node_t));
				node->left = new_node;
				strcpy(new_node->name, name);
				new_node->score = score;
				new_node -> right = NULL;
				new_node ->left = NULL;
			}
		}
	}
}
int add_score(gradebook_t *book, const char *name, int score) {
    // TODO: Not yet implemented //when would i be returning -1?
	if(score<0){
//		printf("Error: You must enter a score in the valid range (0 <= score)\n");
		return -1;
	} 
	if(book->root ==  NULL){
	node_t* new_node = malloc(sizeof(node_t));
	new_node -> score = score;
	strcpy(new_node->name, name);
	new_node -> left = NULL;
	new_node -> right = NULL;
	book->root = new_node;
	} else {
		addRecursion(book->root, name, score);
	}
//free(new_node);
    return 0;
}

int find_score(const gradebook_t *book, const char *name) {
    // TODO: Not yet implemented
		node_t *node = book->root;
		while(node != NULL) {
			if(strcmp(node->name, name)== 0){
				return node->score;
			} else {
				if(strcmp(node->name, name) > 0){
					node = node-> left;
				} else {
					node = node->right;
				}
			}
}
//		free(node);
		return -1;
	}

void print_recursion(node_t *node){
	if(node->left != NULL){
		//printf("%s: %d", node->name, node->score);
		print_recursion(node->left);
	}
	printf("%s: %d\n", node->name, node->score);
	if(node -> right != NULL){
	//	printf("%s: %d", node->name, node->score);
		print_recursion(node->right);
	}
//	printf("%s: %d\n", node->name, node->score);
}

void print_gradebook(const gradebook_t *book) {
	if(book-> root != NULL){
//	node_t *node = malloc(sizeof(node_t));
	print_recursion(book->root);
//	free(node);
	}
    // TODO: Not yet implemented - NOT DONE
}
void free_recursion(node_t *node){
	if(node -> right != NULL){
		free_recursion(node->right);
	}
	if(node->left != NULL){
		free_recursion(node->left);
	}
	free(node);
}
void free_gradebook(gradebook_t *book) {
//	node_t *node = malloc(sizeof(node_t));
//	node = book->root;
	if(book != NULL && book->root != NULL){
	free_recursion(book->root);
}
//	free(node);
	free(book);
    // TODO: Not yet implemented
}


// Helper function to allow recursion for writing out tree
int write_gradebook_to_text_aux(const node_t *current, FILE *f) {
    if (current == NULL) {
        return 0;
    }

    // Each line is formatted as "<name> <score>"
    fprintf(f, "%s %d\n", current->name, current->score);

    if (current->left != NULL) {
        if (write_gradebook_to_text_aux(current->left, f) != 0) {
            return -1;
        }
    }

    if (current->right != NULL) {
        if (write_gradebook_to_text_aux(current->right, f) != 0) {
            return -1;
        }
    }

    return 0;
}

int write_gradebook_to_text(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".txt");

    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        return -1;
    }

    int result = write_gradebook_to_text_aux(book->root, f);
    fclose(f);
    return result;
}
//int read_gradebook_from_text_aux

gradebook_t *read_gradebook_from_text(const char *file_name) {
    // TODO: Not yet implemented
	FILE *f = fopen(file_name, "r");
	if(f==NULL){
		return NULL;
	} else {
	char name [strlen(file_name)];
	int len = strlen(file_name);
	strcpy(name, file_name); 
	name[len-4] = '\0';
	gradebook_t* book= create_gradebook(name); //name should be pointer??
	char namen[MAX_NAME_LEN];
	int score;
	while(fscanf(f, "%s %d", namen, &score) != EOF){

		add_score(book, namen, score);
	}
fclose(f);
    return book;
}
}
int write_gradebook_to_binary_aux(const node_t *current, FILE *f){
	if(current == NULL){
		return 0;
	}
	int len = strlen(current->name);
	fwrite(&len, sizeof(int), 1, f);
	fwrite(current->name, sizeof(char), len, f);
	fwrite(&(current -> score), sizeof(int), 1, f);
	if(current ->left != NULL){
		if(write_gradebook_to_binary_aux(current->left, f) != 0){
			return -1;
		}
	}
	if(current ->right != NULL){
		if(write_gradebook_to_binary_aux(current->right, f) != 0){
			return -1;
		}
	}
return 0;
}
int write_gradebook_to_binary(const gradebook_t *book) {
    // TODO: Not yet implemented
	char file_name[MAX_NAME_LEN + strlen(".bin")];
	strcpy(file_name, book->class_name);
	strcat(file_name, ".bin");

	FILE *f = fopen(file_name, "wb");
	if(f==NULL){
	    return -1;
	}
	int results = write_gradebook_to_binary_aux(book->root, f);
		
//	fwrite(book, sizeof(book), 1, f);
	fclose(f);
	return results;
}

gradebook_t *read_gradebook_from_binary(const char *file_name) {
    // TODO: Not yet implemented
	 FILE *f = fopen(file_name, "rb");
	if(f==NULL){
		return NULL;
	} else {
	char name [strlen(file_name)];
	int len = strlen(file_name);
	strcpy(name, file_name);
	name[len-4] = '\0';
	gradebook_t *book = create_gradebook(name);
	char namen [MAX_NAME_LEN];
	int score = 0;
	int length = 0;;
	while(fread(&length, sizeof(int), 1, f) > 0){
		fread(namen, sizeof(char), length, f);
		namen[length] = '\0';
		fread(&score, sizeof(int), 1, f);
		add_score(book, namen, score);
	}

	fclose(f);
return book;
}}

