#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 50
#define USER_ERROR 1
#define EXIT_SUCCESS 0
#define FILE_ERROR -1
#define FSCANF_ERROR -2
#define MALLOC_ERROR -3

struct _stackItem;
typedef struct _stackItem* Position;
typedef struct _stackItem
{
	int number;
	Position next;
} stackItem;

int readFile(Position);
int pop(Position);
int push(Position, int);
int printList(Position);

int main() {
	stackItem head = {
		.number = NULL,
		.next = NULL
	};

	readFile(&head);
	free(head.next);
	return EXIT_SUCCESS;
}

int printList(Position head) {
	Position current = head->next;
	while (current != NULL) {
		printf("%d ", current->number);
		current = current->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int push(Position head, int numberToPush) {
	Position current = head;
	while (current->next != NULL) current = current->next;
	Position newStackItem = (Position)malloc(sizeof(stackItem));
	if (!newStackItem) {
		printf("Memory allocation error!\n");
		return MALLOC_ERROR;
	}
	newStackItem->number = numberToPush;
	newStackItem->next = NULL;
	current->next = newStackItem;
	printList(head);
	return EXIT_SUCCESS;
}

int pop(Position head) {
	Position current = head;
	if (current->next == NULL) {
		printf("Empty stack!\n");
		exit(USER_ERROR);
	}

	while (current->next->next != NULL) current = current->next;

	Position itemToPop = current->next;
	int num = itemToPop->number;
	current->next = NULL;
	free(itemToPop);
	return num;
}

int readFile(Position head) {
	FILE* file = fopen("postfix.txt", "r");
	if (file == NULL) {
		printf("File openning error!\n");
		return FILE_ERROR;
	}

	char string[MAX_LENGTH] = "";
	fgets(string, MAX_LENGTH, file);
	printf("%s\n", string);

	char input = ' ', nextInput = ' ';
	int firstNum = 0, secondNum = 0, i = 0, numberToPush = 0, stringLength = strlen(string);
	while (i < stringLength) 
	{
		input = *(string + i);

		if ((input >= '0') && (input <= '9')) 
		{
			numberToPush = (int)input - 48;
			nextInput = *(string + i + 1);
			while ((nextInput != ' ') && (nextInput >= '0') && (nextInput <= '9')){
				numberToPush = numberToPush * 10 + ((int)nextInput - 48);
				i++;
				nextInput = *(string + i + 1);
			}
			push(head, numberToPush);
		}

		else 
		{
			switch (input) {
				case '+':
					secondNum = pop(head);
					firstNum = pop(head);
					push(head, firstNum + secondNum);
					break;
				case '-':
					secondNum = pop(head);
					firstNum = pop(head);
					push(head, firstNum - secondNum);
					break;
				case '*':
					secondNum = pop(head);
					firstNum = pop(head);
					push(head, firstNum * secondNum);
					break;
				case '/':
					secondNum = pop(head);
					firstNum = pop(head);
					push(head, firstNum / secondNum);
					break;
				case ' ':
					break;
				default:
					printf("Unknown character!\n");
					return USER_ERROR;
			}
		}
		i++;
	}

	fclose(file);
	return EXIT_SUCCESS;
}