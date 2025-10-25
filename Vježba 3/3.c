#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 25
#define ERROR_USER 1
#define EXIT_SUCCESS 0
#define ERROR_MALLOC -1
#define ERROR_SCANF -2
#define ERROR_FILE -3
#define ERROR_FSCANF -4

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;
} Person;

Position createPerson();
int addToBeginning(Position);
int printList(Position);
int addToEnd(Position);
Position findBySurname(Position, char*);
Position findPreviousPersonBySurname(Position, char*);
int deletePerson(Position, char*);
int addAfter(Position, char *);
int addBefore(Position, char*);
int swapElements(Position, Position, Position);
int sortList(Position);
int writeListIntoFile(Position);
int readListFromFile(Position);

int main()
{
	Person head = {
		.name = "",
		.surname = "",
		.birthYear = 0,
		.next = NULL
	};

	int action = -1;
	char surname[MAX_LENGTH] = "";

	printf("Choose Action from the list:\n1 - Add new Person to the top of the list\n2 - Print list\n3 - Add new Person to the end of the list\n4 - Find a Person by surname\n5 - Delete a Person from the list\n6 - Add a Person after another Person\n7 - Add a Person before another Person\n8 - Sort list by surnames\n9 - Wite list into a file\n10 - Read list from a file\n");

	do {
		printf("Type a number 1-5 for listed actions or 0 to end program: ");
		if (!scanf(" %d", &action)) {
			printf("Scanf error while choosing action\n");
			exit(ERROR_SCANF);
			continue;
		}
		switch (action)
		{
		case 0:
			printf("Program ended by user!\n");
			break;

		case 1:
			addToBeginning(&head);
			break;

		case 2:
			printList(&head);
			break;

		case 3:
			addToEnd(&head);
			break;

		case 4:
			printf("Enter surname of a Person to find: ");
			if (!scanf("%s", surname)) {
				printf("Scanf error while entering surname to find\n");
				exit(ERROR_SCANF);
			}
			else
				findBySurname(&head, surname);
			break;

		case 5:
			printf("Enter surname of a Person to delete: ");
			if (!scanf("%s", surname)) {
				printf("Scanf error while entering surname to delete\n");
				exit(ERROR_SCANF);
			}
			else
				deletePerson(&head, surname);
			break;

		case 6:
			printf("Enter surname of Person to add new Person after: ");
			if (!scanf("%s", surname)) {
				printf("Scanf error while entering surname to add after\n");
				exit(ERROR_SCANF);
			}
			addAfter(&head, surname);
			break;

		case 7:
			printf("Enter surname of Person to add new Person before: ");
			if (!scanf("%s", surname)) {
				printf("Scanf error while entering surname to add before\n");
				exit(ERROR_SCANF);
			}
			addBefore(&head, surname);
			break;

		case 8:
			sortList(&head);
			break;

		case 9:
			writeListIntoFile(&head);
			break;

		case 10:
			readListFromFile(&head);
			break;

		default:
			printf("Invalid input! Try again!\n");
			break;
		}
	} while (action != 0);

	return EXIT_SUCCESS;
}

Position createPerson()
{
	Position newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson) {
		printf("Memory allocation error");
		exit(ERROR_MALLOC);
	}

	printf("New person's name: ");
	if (!scanf(" %s", newPerson->name))
	{
		printf("Scanf error");
		exit(ERROR_SCANF);
	}

	printf("New person's surname: ");
	if (!scanf(" %s", newPerson->surname))
	{
		printf("Scanf error");
		exit(ERROR_SCANF);
	}

	printf("New person's birth year: ");
	if (!scanf(" %d", &newPerson->birthYear))
	{
		printf("Scanf error");
		exit(ERROR_SCANF);
	}

	return newPerson;
}

int addToBeginning(Position head)
{
	Position newPerson = createPerson();
	if (!newPerson) {
		printf("Memory allocation error\n");
		return ERROR_MALLOC;
	}
	newPerson->next = head->next;
	head->next = newPerson;
	return EXIT_SUCCESS;
}

int printList(Position current)
{
	if (current->next == NULL) {
		printf("Linked list is empty!\n");
		return ERROR_USER;
	}

	current = current->next;
	printf("Name\tSurname\tBirth Year\n");
	while (current != NULL) {
		printf("%s\t%s\t%d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int addToEnd(Position currentPerson)
{
	Position newPerson = createPerson();
	if (!newPerson) {
		printf("Memory allocation error\n");
		return ERROR_MALLOC;
	}

	while (currentPerson->next != NULL) {
		currentPerson = currentPerson->next;
	}

	newPerson->next = currentPerson->next;
	currentPerson->next = newPerson;
	return EXIT_SUCCESS;
}

Position findBySurname(Position current, char* surname)
{
	if (current->next == NULL) {
		printf("Linked list is empty\n");
		return NULL;
	}

	while (current->next != NULL)
	{
		if (strcmp(current->next->surname, surname) != 0)
			current = current->next;
		else
		{
			printf("Person with surname %s found!\n", current->next->surname);
			return current->next;
		}

	}
	printf("Person with surname %s not found!\n", surname);
	return NULL;
}

Position findPreviousPersonBySurname(Position head, char* surname) {
	Position previousPerson = head;
	Position person = findBySurname(head, surname);
	while (previousPerson->next != NULL && previousPerson->next != person)
		previousPerson = previousPerson->next;
	return previousPerson;
}

int deletePerson(Position head, char* surname)
{
	Position personToDelete = findBySurname(head, surname);
	if (personToDelete == NULL) return ERROR_USER;

	Position previousPerson = findPreviousPersonBySurname(head, surname);

	previousPerson->next = personToDelete->next;
	personToDelete->next = NULL;
	free(personToDelete);
	printf("Person with surname %s deleted!\n", surname);
	return EXIT_SUCCESS;
}

int addAfter(Position head, char *surname) {
	Position personToAddAfter = findBySurname(head, surname);
	if (personToAddAfter == NULL) return ERROR_USER;

	Position newPerson = createPerson();
	if (!newPerson) {
		printf("Memory allocation error\n");
		return ERROR_MALLOC;
	}

	newPerson->next = personToAddAfter->next;
	personToAddAfter->next = newPerson;
	return EXIT_SUCCESS;
}
int addBefore(Position head, char *surname) {
	Position personToAddBefore = findBySurname(head, surname);
	if (personToAddBefore == NULL) return ERROR_USER;

	Position previousPerson = findPreviousPersonBySurname(head, surname);

	Position newPerson = createPerson();
	if (!newPerson) {
		printf("Memory allocation error\n");
		return ERROR_MALLOC;
	}

	newPerson->next = personToAddBefore;
	previousPerson->next = newPerson;
	return EXIT_SUCCESS;
}

int swapElements(Position head, Position element1, Position element2) {
	Position previousPerson1 = findPreviousPersonBySurname(head, element1->surname);
	Position previousPerson2 = findPreviousPersonBySurname(head, element2->surname);
	Position nextPerson1 = element1->next;
	Position nextPerson2 = element2->next;
	previousPerson1->next = element2;
	element2->next = nextPerson1;
	previousPerson2->next = element1;
	element1->next = nextPerson2;
	return EXIT_SUCCESS;
}

int sortList(Position head) {
	if (head->next == NULL) {
		printf("Linked list is empty!\n");
		return ERROR_USER;
	}

	if (head->next->next == NULL) {
		printf("Linked list has only one element!\n");
		return ERROR_USER;
	}

	Position iElement = head->next, jElement = NULL;
	int strcmpResult = 0, strcmpNameResult = 0;
	char iSurname[MAX_LENGTH] = "", jSurname[MAX_LENGTH] = "", iName[MAX_LENGTH] = "", jName[MAX_LENGTH] = "";
	while (iElement != NULL) {
		jElement = iElement->next;
		while (jElement != NULL) {
			strcpy(iSurname, iElement->surname);
			strcpy(jSurname, jElement->surname);
			_strupr(iSurname);
			_strupr(jSurname);
			strcmpResult = strcmp(iSurname, jSurname);
			if (strcmpResult > 0) swapElements(head, iElement, jElement);
			else if (strcmpResult == 0) {
				strcpy(iName, iElement->name);
				strcpy(jName, jElement->name);
				_strupr(iName);
				_strupr(jName);
				strcmpNameResult = strcmp(iName, jName);
				if (strcmpNameResult > 0) swapElements(head, iElement, jElement);
				else if (strcmpNameResult == 0) {
					if (iElement->birthYear > jElement->birthYear) swapElements(head, iElement, jElement);
				}
			}
			jElement = jElement->next;
		}
		iElement = iElement->next;
	}
	return EXIT_SUCCESS;
}

int writeListIntoFile(Position current) {
	if (current->next == NULL) {
		printf("Linked list is empty!\n");
		return ERROR_USER;
	}

	FILE* file = fopen("Persons.txt", "w");
	if (file == NULL) {
		printf("File failed to open\n");
		return ERROR_FILE;
	}

	fprintf(file ,"Name\tSurname\tBirth Year\n");
	current = current->next;
	while (current != NULL) {
		fprintf(file, "%s\t%s\t%d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}

	fclose(file);
	printf("List successfully written into a file!\n");
	return EXIT_SUCCESS;
}
int readListFromFile(Position head) {
	FILE* file = fopen("Persons.txt", "r");
	if (file == NULL) {
		printf("File failed to open\n");
		return ERROR_FILE;
	}

	if (fgetc(file) == EOF) {
		printf("File is empty!\n");
		return ERROR_USER;
	}

	Position temporaryPerson = NULL;
	while (head->next != NULL) {
		temporaryPerson = head->next;
		head->next = head->next->next;
		free(temporaryPerson);
		printf("Person deleted!\n");
	}
	printf("Linked list deleted!\n");

	Position newPerson = NULL, previousPerson = head;
	while (fgetc(file) != '\n') continue;
	while (!feof(file)) {
		newPerson = (Position)malloc(sizeof(Person));
		if (!newPerson) {
			printf("Memory allocation error");
			exit(ERROR_MALLOC);
		}

		if (!fscanf(file, " %s %s %d ", newPerson->name, newPerson->surname, &newPerson->birthYear)) {
			printf("Fscanf error!\n");
			fclose(file);
			return ERROR_FSCANF;
		}

		newPerson->next = NULL;
		previousPerson->next = newPerson;
		previousPerson = previousPerson->next;
	}

	fclose(file);
	printf("Successfully read list from a file!\n");
	return EXIT_SUCCESS;
}