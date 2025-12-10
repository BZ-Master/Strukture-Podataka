#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR_LENGTH 50
#define NULL_DIRECTORY_ERROR 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct _Directory;
typedef struct _Directory* directoryPosition;
typedef struct _Directory {
	char name[MAX_CHAR_LENGTH];
	directoryPosition nextSibling;
	directoryPosition firstChild;
} directory;

struct _Stack;
typedef struct _Stack* stackPosition;
typedef struct _Stack {
	directoryPosition level;
	stackPosition next;
} stack;

int push(stackPosition, stackPosition);
int pop(stackPosition);
int createDirectory(char*, directoryPosition);
int changeDirectory(char*, directoryPosition, stackPosition);
int printDirectory(directoryPosition);
int printDirectoryPath(stackPosition);
int freeStack(stackPosition);
int freeDirectories(directoryPosition);

int main() {
	directory headDirectory = {"", NULL, NULL};
	stack headStack = { &headDirectory, NULL };

	directoryPosition root = (directoryPosition)malloc(sizeof(directory));
	if (root == NULL) return MALLOC_ERROR;
	headDirectory.nextSibling = root;
	strcpy(root->name, "C:");
	root->firstChild = NULL;
	root->nextSibling = NULL;

	stackPosition rootLevel = (stackPosition)malloc(sizeof(stack));
	if (rootLevel == NULL) return MALLOC_ERROR;

	rootLevel->level = root;
	rootLevel->next = NULL;
	push(rootLevel, &headStack);

	int cmd = 0;
	char newDirName[MAX_CHAR_LENGTH] = "";
	directoryPosition currentDirectory = root;
	printf("1 - md\n2 - cd\n3 - cd..\n4 - dir\n5 - exit\n");
	do {
		if (scanf("%d", &cmd) != 1) {
			printf("scanf error!\n");
			freeDirectories(&headDirectory);
			freeStack(&headStack);
			return SCANF_ERROR;
		}

		switch (cmd) {
			case 1:
				printf("md ");
				if (scanf("%s", newDirName) != 1) {
					printf("scanf error!\n");
					freeDirectories(&headDirectory);
					freeStack(&headStack);
					return SCANF_ERROR;
				}
				createDirectory(newDirName, currentDirectory);
				break;
			case 2:
				printf("cd ");
				if (scanf("%s", newDirName) != 1) {
					printf("scanf error!\n");
					freeDirectories(&headDirectory);
					freeStack(&headStack);
					return SCANF_ERROR;
				}
				changeDirectory(newDirName, currentDirectory, &headStack);
				stackPosition stackItem = &headStack;
				while (stackItem->next != NULL) stackItem = stackItem->next;
				currentDirectory = stackItem->level;
				printf("Current directory: ");
				printDirectoryPath(&headStack);
				break;
			case 3:
				printf("cd..\n");
				break;
			case 4:
				printf("dir\n");
				printDirectory(currentDirectory);
				break;
			case 5:
				printf("Exiting...\n");
				break;
			default:
				printf("Command not recognised!\n");
				break;
		}
	} while (cmd != 5);

	freeDirectories(root);
	headDirectory.firstChild = NULL;
	freeStack(&headStack);
	return EXIT_SUCCESS;
}

int push(stackPosition newElement, stackPosition head) {
	stackPosition current = head;
	while (current->next != NULL) current = current->next;
	newElement->next = current->next;
	current->next = newElement;
	return EXIT_SUCCESS;
}

int pop(stackPosition head) {
	stackPosition current = head->next;
	stackPosition previous = head;
	if (current == NULL) {
		printf("Nothing to pop!\n");
		return EXIT_SUCCESS;
	}
	while (current->next != NULL) { current = current->next; previous = previous->next; }
	previous->next = NULL;
	current->next = NULL;
	free(current);
	return EXIT_SUCCESS;
}

int createDirectory(char* name, directoryPosition parent) {
	if (parent == NULL) {
		printf("Parent directory is NULL!\n");
		return NULL_DIRECTORY_ERROR;
	}

	directoryPosition newDirectory = (directoryPosition)malloc(sizeof(directory));
	if (newDirectory == NULL) {
		printf("malloc error!\n");
		return MALLOC_ERROR;
	}
	strcpy(newDirectory->name, name);
	newDirectory->nextSibling = NULL;
	newDirectory->firstChild = NULL;
	
	if (parent->firstChild == NULL) {
		parent->firstChild = newDirectory;
		return EXIT_SUCCESS;
	}

	directoryPosition previousSibling = parent->firstChild;
	while (previousSibling->nextSibling != NULL) previousSibling = previousSibling->nextSibling;
	previousSibling->nextSibling = newDirectory;
	return EXIT_SUCCESS;
}

int changeDirectory(char* name, directoryPosition currentDirectory, stackPosition head) {
	if (currentDirectory == NULL) {
		printf("Directory is NULL!\n");
		return NULL_DIRECTORY_ERROR;
	}
	directoryPosition current = currentDirectory->firstChild;
	while (current != NULL) {
		if (strcmp(name, current->name) == 0) {
			stackPosition newStackItem = (stackPosition)malloc(sizeof(stack));
			if (newStackItem == NULL) return MALLOC_ERROR;
			newStackItem->level = current;
			newStackItem->next = NULL;
			push(newStackItem, head);
			return EXIT_SUCCESS;
		}
		current = current->nextSibling;
	}
	printf("Directory not found!\n");
	return EXIT_SUCCESS;
}

int printDirectory(directoryPosition currentDirectory) {
	if (currentDirectory == NULL) {
		printf("Directory is NULL!\n");
		return NULL_DIRECTORY_ERROR;
	}
	directoryPosition current = currentDirectory->firstChild;
	while (current != NULL) {
		printf("%s ", current->name);
		current = current->nextSibling;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int printDirectoryPath(stackPosition head) {
	stackPosition current = head->next;
	while (current != NULL) {
		printf("%s\\", current->level->name);
		current = current->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int freeStack(stackPosition head) {
	stackPosition current = head->next;
	while (current != NULL) {
		stackPosition itemToFree = current;
		current = current->next;
		head->next = current;
		itemToFree->next = NULL;
		free(itemToFree);
	}
	return EXIT_SUCCESS;
}

int freeDirectories(directoryPosition root) {
	directoryPosition current = root->firstChild;
	directoryPosition directoryToFree = root;
	while (current != NULL) {
		directoryPosition next = current->nextSibling;
		freeDirectories(current);
		current = next;
	}
	free(directoryToFree);
	return EXIT_SUCCESS;
}