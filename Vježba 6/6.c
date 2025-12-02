#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX_CHAR_LENGTH 20
#define EXIT_SUCCESS 0
#define FILE_ERROR -1
#define FSCANF_ERROR -2
#define MALLOC_ERROR -3

typedef struct _Date{
	int year;
	int month;
	int day;
} Date;

struct _Item;
typedef struct _Item* List;
typedef struct _Item {
	char name[MAX_CHAR_LENGTH];
	int amount;
	float price;
	List next;
} Item;

struct _Receipt;
typedef struct _Receipt* Position;
typedef struct _Receipt {
	Date date;
	Item item;
	Position next;
} Receipt;

int sortItem(List, List);
int sortReceipt(Position, Position);
int readFile(char*, Position);
int readReceipts(Position);

int main() {
	Receipt head = {
		NULL,
		NULL,
		NULL
	};
	readReceipts(&head);
	//printf("%d", readReceipts(&head));
	return EXIT_SUCCESS;
}

int sortItem(List newItem, List head) {
	List current = head;
	if (current->next == NULL) {
		newItem->next = current->next;
		current->next = newItem;
		return EXIT_SUCCESS;
	}
	current = current->next;
	char newItemName[MAX_CHAR_LENGTH] = "", currentItemName[MAX_CHAR_LENGTH] = "";
	strcpy(newItemName, newItem->name);
	_strupr(newItemName);
	int strcmpResult = 0;
	while (current != NULL) {
		strcpy(currentItemName, current->name);
		_strupr(currentItemName);
		strcmpResult = strcmp(newItemName, currentItemName);
		if (strcmpResult < 0) {
			newItem->next = current->next;
			current->next = newItem;
			break;
		}
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int sortReceipt(Position newReceipt, Position head) {
	Position current = head;
	if (current->next == NULL) { 
		newReceipt->next = current->next; 
		current->next = newReceipt; 
		return EXIT_SUCCESS;
	}
	current = current->next;
	while (current != NULL) {
		if (current->date.year < newReceipt->date.year) { newReceipt->next = current->next; current->next = newReceipt; }
		else if (current->date.month < newReceipt->date.month) { newReceipt->next = current->next; current->next = newReceipt; }
		else if (current->date.day <= newReceipt->date.day) { newReceipt->next = current->next; current->next = newReceipt; }
		else current = current->next;
	}
	return EXIT_SUCCESS;
}

int readFile(char* fileName, Position head) {
	FILE* receipt = fopen(fileName, "r");
	if (receipt == NULL) {
		printf("Error while openning file %s!\n", fileName);
		return FILE_ERROR;
	}

	Position newReceipt = (Position)malloc(sizeof(Receipt));
	if (!newReceipt) {
		printf("Memory allocation error!\n");
		return MALLOC_ERROR;
	}
	if (fscanf(receipt, "%d-%d-%d", &newReceipt->date.year, &newReceipt->date.month, &newReceipt->date.day) != 3) {
		printf("fscanf error!\n");
		return FSCANF_ERROR;
	}

	newReceipt->item.name[0] = '\0';
	newReceipt->item.amount	= 0;
	newReceipt->item.price = 0.0;
	newReceipt->item.next = NULL;

	while (!feof(receipt)) {
		List newItem = (List)malloc(sizeof(Item));
		if (!newItem) {
			printf("Memory allocation error!\n");
			return MALLOC_ERROR;
		}

		if (fscanf(receipt, "%s %d %f", newItem->name, &newItem->amount, &newItem->price) != 3) {
			printf("fscanf error!\n");
			return FSCANF_ERROR;
		}

		sortItem(newItem, &newReceipt->item);
	}

	newReceipt->next = NULL;
	sortReceipt(newReceipt, head);
	fclose(receipt);
	return EXIT_SUCCESS;
}

int readReceipts(Position head) {
	FILE* file = fopen("racuni.txt", "r");
	if (file == NULL) {
		printf("Error while openning file racuni.txt!\n");
		return FILE_ERROR;
	}
	char fileName[MAX_CHAR_LENGTH];
	while (!feof(file)) {
		if (fscanf(file, " %s", fileName) != 1) {
			printf("fscanf error!\n");
			return FSCANF_ERROR;
		}
		readFile(fileName, head);
	}
	fclose(file);
	return EXIT_SUCCESS;
}