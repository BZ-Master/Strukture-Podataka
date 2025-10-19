#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 25
#define ERROR_USER 1
#define EXIT_SUCCESS 0
#define ERROR_MALLOC -1
#define ERROR_SCANF -2

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
Position findBySurname(Position, char *);
int deletePerson(Position, char *);

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

	printf("Choose Action from the list:\n1 - Add new Person to the top of the list\n2 - Print list\n3 - Add new Person to the end of the list\n4 - Find a Person by surname\n5 - Delete a Person from the list\n");

	do {
		printf("Type a number 1-5 for listed actions or 0 to end program: ");
		if (!scanf("%d", &action)) {
			printf("Scanf error while choosing action\n");
			exit(ERROR_SCANF);
			continue;
		}
		switch (action) 
		{
			case 0:
				printf("Program ended!\n");
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
				if(!scanf("%s", surname)) {
					printf("Scanf error while entering surname to find\n");
					exit(ERROR_SCANF);
				}
				else {
					if (findBySurname(&head, surname) == NULL) printf("Person not found!\n");
	
					else printf("Person found!\n");
				}
				break;

			case 5:
				printf("Enter surname of a Person to delete: ");
				if (!scanf("%s", surname)) {
					printf("Scanf error while entering surname to delete\n");
					exit(ERROR_SCANF);
				}
				else {
					if (deletePerson(&head, surname) == EXIT_SUCCESS) printf("Person with surname %s deleted!\n", surname);
					else printf("Person not found!\n");
				}
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
	if (!scanf("%s", newPerson->name)) 
	{ 
		printf("Scanf error");
		exit(ERROR_SCANF); 
	}

	printf("New person's surname: ");
	if (!scanf("%s", newPerson->surname)) 
	{
		printf("Scanf error");
		exit(ERROR_SCANF);
	}

	printf("New person's birth year: ");
	if (!scanf("%d", &newPerson->birthYear)) 
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
		printf("Memory allocation error");
		return ERROR_MALLOC;
	}
	newPerson->next = head->next;
	head->next = newPerson;
	return EXIT_SUCCESS;
}

int printList(Position head) 
{
	if (head->next == NULL) {
		printf("Linked list is empty!");
		return EXIT_SUCCESS;
	}

	Position temp = head->next;

	printf("Name\tSurname\tBirth Year\n");
	while (temp != NULL) {
		printf("%s\t%s\t%d\n", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int addToEnd(Position head)
{
	Position newPerson = createPerson();
	if (!newPerson) {
		printf("Memory allocation error");
		return ERROR_MALLOC;
	}

	while (head->next!=NULL) {
		head = head->next;
	}

	newPerson->next = head->next;
	head->next = newPerson;
	return EXIT_SUCCESS;
}

Position findBySurname(Position head, char *surname)
{
	if (head->next == NULL) {
		printf("Linked list is empty\n");
		return NULL;
	}
	
	while (head->next != NULL)
	{
		if (strcmp(head->surname, surname) != 0)
			head = head->next;
		else
			return head;
	}

	return NULL;
}

int deletePerson(Position head, char *surname)
{
	Position personToDelete = findBySurname(&head, surname);
	if (personToDelete == NULL) return ERROR_USER;

	Position previousPerson = NULL;

	while (head->next != NULL)
	{
		if (strcmp(head->surname, surname) == 0)
			previousPerson = head;
			break;
	}

	if (previousPerson == NULL) {
		printf("Previous person of the person to delete not found\n");
		return ERROR_USER;
	}

	previousPerson->next = personToDelete->next;
	personToDelete->next = NULL;
	free(personToDelete);
	return EXIT_SUCCESS;
}