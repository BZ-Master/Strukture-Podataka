#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_CHAR_LENGTH 20
#define EXIT_SUCCESS 0
#define FILE_ERROR -1
#define FSCANF_ERROR -2
#define MALLOC_ERROR -3
#define SCANF_ERROR -4

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

bool isDateLater(Date, Date);
int sortItem(List, List);
int sortReceipt(Position, Position);
int readFile(char*, Position);
int readReceipts(Position);
int printReceipts(Position);
int freeReceipts(Position);
int inquiry(Position);

int main() {
	Receipt head = {
		{0, 0, 0},
		{"", 0, 0.0, NULL},
		NULL
	};
	readReceipts(&head);
	printReceipts(&head);
	inquiry(&head);
	freeReceipts(&head);
	return EXIT_SUCCESS;
}

bool isDateLater(Date date1, Date date2) {
	if (date1.year > date2.year)
		return true;
	else if (date1.year == date2.year && date1.month > date2.month)
		return true;
	else if (date1.year == date2.year && date1.month == date2.month && date1.day >= date2.day)
		return true;
	else
		return false;
}

int sortItem(List newItem, List head) {
	List current = head;
	if (current->next == NULL) {
		newItem->next = current->next;
		current->next = newItem;
		return EXIT_SUCCESS;
	}
	current = current->next;
	List previous = head;
	char newItemName[MAX_CHAR_LENGTH] = "", currentItemName[MAX_CHAR_LENGTH] = "";
	strcpy(newItemName, newItem->name);
	_strupr(newItemName);
	int strcmpResult = 0;
	while (current != NULL) {
		strcpy(currentItemName, current->name);
		_strupr(currentItemName);
		strcmpResult = strcmp(newItemName, currentItemName);
		if (strcmpResult < 0) break;
		current = current->next;
		previous = previous->next;
	}
	previous->next = newItem;
	newItem->next = current;
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
	Position previous = head;
	while (current != NULL) {
		if (isDateLater(current->date, newReceipt->date) && isDateLater(newReceipt->date, previous->date)) break;
		else current = current->next; previous = previous->next;
	}
	newReceipt->next = current;
	previous->next = newReceipt;
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
	char dateString[11] = "";

	if (fscanf(receipt, "%10s", dateString) != 1) {
		printf("fscanf error!\n");
		return FSCANF_ERROR;
	}

	if (sscanf(dateString, "%d-%d-%d", &newReceipt->date.year, &newReceipt->date.month, &newReceipt->date.day) != 3) {
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
		if (fscanf(file, " %19s", fileName) != 1) {
			printf("fscanf error!\n");
			return FSCANF_ERROR;
		}
		readFile(fileName, head);
	}
	fclose(file);
	return EXIT_SUCCESS;
}

int printReceipts(Position head) {
	Position current = head->next;
	while (current != NULL) {
		printf("%d.%d.%d.\n", current->date.day, current->date.month, current->date.year);
		List currentItem = current->item.next;
		while (currentItem != NULL) {
			printf("%s\t%d\t%.2f\n", currentItem->name, currentItem->amount, currentItem->price);
			currentItem = currentItem->next;
		}
		current = current->next;
		printf("\n");
	}
	return EXIT_SUCCESS;
}

int freeReceipts(Position head) {
	Position previous = head;
	Position current = head->next;
	Position receiptToFree = NULL;
	List headItem = NULL;
	List itemToFree = NULL;
	while (current != NULL) {
		headItem = &current->item;
		while (itemToFree != NULL) {
			itemToFree = &current->item.next;
			headItem->next = itemToFree->next;
			itemToFree->next = NULL;
			free(itemToFree);
		}
		receiptToFree = current;
		previous->next = current->next;
		current->next = NULL;
		free(receiptToFree);
		current = previous->next;
	}
	return EXIT_SUCCESS;
}

int inquiry(Position head) {
	Date start = { 0, 0, 0 };
	Date end = { 0, 0, 0 };
	char item[MAX_CHAR_LENGTH] = "";
	printf("Enter the name of an item: ");
	if (scanf("%s", item) != 1) 
	{
		printf("scanf error!\n");
		return SCANF_ERROR;
	};
	printf("Enter start date in format YYYY-MM-DD: ");
	if (scanf("%d-%d-%d", &start.year, &start.month, &start.day) != 3) 
	{
		printf("scanf error!\n");
		return SCANF_ERROR;
	};
	printf("Enter end date in format YYYY-MM-DD: ");
	if (scanf("%d-%d-%d", &end.year, &end.month, &end.day) != 3) 
	{
		printf("scanf error!\n");
		return SCANF_ERROR;
	};
	Position current = head->next;
	float totalPrice = 0.0;
	int totalAmount = 0;
	printf("\nInquiry Result:\nItem: %s\nTotal amount: %d\nTotal spent: %.2f\n", item, totalAmount, totalPrice);
	return EXIT_SUCCESS;
}