#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR_LENGTH 50
#define ELEMENT_NOT_FOUND 2
#define INVALID_INPUT 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_ERROR -2
#define SCANF_ERROR -3
#define FSCANF_ERROR -4
#define HASH_FULL -5
#define HASH_EMPTY -6
#define NULL_PTR_ERROR -7

struct _courseNode;
typedef struct _courseNode* coursePosition;
typedef struct _courseNode* course;
typedef struct _courseNode {
	int id;
	char name[MAX_CHAR_LENGTH];
	int ects;
	coursePosition next;
} courseNode;

struct _courseHash;
typedef struct _courseHash* courseHashTable;
typedef struct _courseHash {
	int size;
	course* hash;
} courseHash;

struct _courseStatus;
typedef struct _courseStatus* courseStatusPosition;
typedef struct _courseStatus {
	coursePosition course;
	int grade;
	courseStatusPosition next;
} courseStatus;

struct _student;
typedef struct _student* studentPosition;
typedef struct _student {
	char name[MAX_CHAR_LENGTH];
	char surname[MAX_CHAR_LENGTH];
	int  birthYear;
	float averageGrade;
	int totalEcts;
	int currentEcts;
	char fileName[MAX_CHAR_LENGTH];
	courseStatusPosition courses;
	studentPosition next;
} student;

struct _studentStack;
typedef struct _studentStack* studentStackPosition;
typedef struct _studentStack {
	studentPosition student;
	studentStackPosition next;
} studentStack;

courseHashTable initializeHash(int);
coursePosition findInHash(int, courseHashTable);
studentPosition findPreviousInList(studentPosition, studentPosition);
studentPosition findInList(studentPosition);
float averageGrade(studentPosition);
int totalEcts(studentPosition);
int currentEcts(studentPosition);
int addToHash(int, char*, int, courseHashTable);
int addToList(char*, char*, int, char*, studentPosition);
int addStudent(studentPosition, courseHashTable);
int addCourse(courseHashTable);
int readStudentFile(char *, studentPosition, courseHashTable);
int readMainFile(studentPosition, courseHashTable);
int readCourseFile(courseHashTable);
int splitList(studentPosition, studentPosition*, studentPosition*);
int sortAlphabetically(studentPosition*);
int sortByAverageGrade(studentPosition*);
studentPosition mergeSortAlphabetically(studentPosition, studentPosition);
studentPosition mergeSortByAverageGrade(studentPosition, studentPosition);
int printAscending(studentPosition);
int printDescending(studentPosition);
int printStudent(studentPosition);
int printCourses(courseHashTable);
int printCourse(courseHashTable, studentPosition);
int writeStudent(studentPosition, char*);
int writeStudents(studentPosition);
int writeStudentsInCourse(coursePosition, studentPosition);
int writeStudentCourses(studentPosition, char*);
int writeCourse(int, char*, int);
int writeCourses(courseHashTable);
int deleteStudent(studentPosition);
int deleteCourse(studentPosition, courseHashTable);
int deleteCourseStatusFromStudents(coursePosition, studentPosition);
int freeStudentCourseStatus(studentPosition);
int freeStudent(studentPosition, studentPosition);
int freeAllStudents(studentPosition);
int freeHashTable(courseHashTable);

int main() {
	student head = { "", "", 0, 0.0, 0, 0, NULL, NULL };
	courseHashTable hashTable = initializeHash(13);
	readCourseFile(hashTable);
	readMainFile(&head, hashTable);

	int choice = 0;
	studentPosition student = NULL;

	printf("0 - End program\n1 - Print student list alphabetically ascending\n2 - Print student list alphabetically descending\n3 - Print student list by average grade ascending\n4 - Print student list by average grade descending\n5 - Print student info\n6 - Print course list\n7 - Print course info\n8 - Add student\n9 - Add course\n10 - Delete student\n11 - Delete course\n");
	do {
		printf("Enter a number that corresponds to a function: ");
		if (scanf("%d", &choice) != 1) {
			printf("scanf error!\n");
			break;
		}
		printf("\n");
		switch (choice) {
		case 0:
			printf("Ending program...\n");
			break;
		case 1:
			sortAlphabetically(&head.next);
			printAscending(&head);
			break;
		case 2:
			sortAlphabetically(&head.next);
			printDescending(&head);
			break;
		case 3:
			sortByAverageGrade(&head.next);
			printAscending(&head);
			break;
		case 4:
			sortByAverageGrade(&head.next);
			printDescending(&head);
			break;
		case 5: 
			student = findInList(&head);
			if (student == NULL) printf("Student not found!\n");
			else printStudent(student);
			break;
		case 6:
			printCourses(hashTable);
			break;
		case 7:
			printCourse(hashTable, &head);
			break;
		case 8:
			addStudent(&head, hashTable);
			break;
		case 9:
			addCourse(hashTable);
			break;
		case 10:
			deleteStudent(&head);
			break;
		case 11:
			deleteCourse(&head, hashTable);
			break;
		default:
			printf("Invalid input!\n");
			break;
		}
		printf("\n");
	} while (choice != 0);

	freeAllStudents(&head);
	freeHashTable(hashTable);
	return EXIT_SUCCESS;
}

courseHashTable initializeHash(int hashSize) {
	courseHashTable hashTable = (courseHashTable)malloc(sizeof(courseHash));
	if (hashTable == NULL) {
		printf("malloc error while allocating hashTable!\n");
		exit(MALLOC_ERROR);
	}
	hashTable->size = hashSize;
	hashTable->hash = (course*)malloc(hashTable->size * sizeof(course));
	if (hashTable->hash == NULL) {
		printf("malloc error while allocating hashTable!\n");
		free(hashTable);
		exit(MALLOC_ERROR);
	}
	int i = 0;
	for (i = 0; i < hashTable->size; i++)
		hashTable->hash[i] = NULL;
	return hashTable;
}

coursePosition findInHash(int key, courseHashTable hashTable) {
	coursePosition current = hashTable->hash[key % hashTable->size];
	while (current != NULL) {
		if (current->id == key)
			return current;
		current = current->next;
	}
	return NULL;
}

studentPosition findInList(studentPosition head) {
	studentPosition current = head->next;
	char name[MAX_CHAR_LENGTH] = "", surname[MAX_CHAR_LENGTH] = "";
	int year = 0;
	printf("Input name, surname and birth year (ex. John Doe 1999): ");
	if (scanf("%49s %49s %d", name, surname, &year) != 3) {
		printf("scanf error!\n");
		return NULL;
	}

	while (current != NULL) {
		if (strcmp(surname, current->surname) == 0 && strcmp(name, current->name) == 0 && year == current->birthYear) return current;
		current = current->next;
	}
	return NULL;
}

int addToList(char* surname, char* name, int year, char* fileName, studentPosition lastEl) {
	studentPosition newEl = (studentPosition)malloc(sizeof(student));
	if (newEl == NULL) {
		printf("malloc error while allocating memory for new student!\n");
		return MALLOC_ERROR;
	}

	strcpy(newEl->name, name);
	strcpy(newEl->surname, surname);
	newEl->birthYear = year;
	newEl->averageGrade = 0.0;
	newEl->currentEcts = 0;
	newEl->totalEcts = 0;
	strcpy(newEl->fileName, fileName);
	newEl->courses = (courseStatusPosition)malloc(sizeof(courseStatus));
	if (newEl->courses == NULL) {
		printf("malloc error while allocating memory for course status head!\n");
		free(newEl);
		return MALLOC_ERROR;
	}
	newEl->courses->course = NULL;
	newEl->courses->grade = 0;
	newEl->courses->next = NULL;
	newEl->next = NULL;
	lastEl->next = newEl;
	return EXIT_SUCCESS;
}

int addToHash(int key, char* name, int ects, courseHashTable hashTable) {
	coursePosition newNode = (coursePosition)malloc(sizeof(courseNode));
	if (newNode == NULL) {
		printf("malloc error when allocating memory for new element of hash table!\n");
		return MALLOC_ERROR;
	}
	newNode->id = key;
	strcpy(newNode->name, name);
	newNode->ects = ects;
	newNode->next = hashTable->hash[key % hashTable->size];
	hashTable->hash[key % hashTable->size] = newNode;
	return EXIT_SUCCESS;
}

int readStudentFile(char* fileName, studentPosition student, courseHashTable hashTable) {
	FILE* studentFile = fopen(fileName, "r");
	if (!studentFile) {
		printf("Error while openning student file!\n");
		return FILE_ERROR;
	}

	int id = 0, grade = 0;
	courseStatusPosition current = student->courses;
	while (!feof(studentFile)) {
		if (fscanf(studentFile, "%d %d", &id, &grade) != 2) {
			printf("fscanf error while reading student's file!\n");
			return FSCANF_ERROR;
		}
		coursePosition course = findInHash(id, hashTable);
		if (course == NULL) {
			printf("Course with ID %d not found!\n", id);
			continue;
		}
		courseStatusPosition newCourseStatus = (courseStatusPosition)malloc(sizeof(courseStatus));
		if (newCourseStatus == NULL) {
			printf("malloc error while allocating memory for course status!\n");
			return MALLOC_ERROR;
		}
		newCourseStatus->grade = grade;
		newCourseStatus->course = course;
		newCourseStatus->next = NULL;
		current->next = newCourseStatus;
		current = current->next;
	}

	student->totalEcts = totalEcts(student);
	student->currentEcts = currentEcts(student);
	student->averageGrade = averageGrade(student);

	fclose(studentFile);
	return EXIT_SUCCESS;
}

int readMainFile(studentPosition head, courseHashTable hashTable) {
	FILE* file = fopen("students.txt", "r");
	if (!file) {
		printf("Error while openning main student file!\n");
		return FILE_ERROR;
	}
	char name[MAX_CHAR_LENGTH] = "";
	char surname[MAX_CHAR_LENGTH] = "";
	int year = 0;
	char fileName[MAX_CHAR_LENGTH] = "";

	studentPosition current = head;
	while (current->next != NULL) current = current->next;

	while (!feof(file)) {
		if (fscanf(file, "%49s %49s %d %49s", surname, name, &year, fileName) != 4) {
			printf("fscanf error while reading courses!\n");
			return FSCANF_ERROR;
		}
		if (addToList(surname, name, year, fileName,current) == EXIT_SUCCESS) {
			current = current->next;
			readStudentFile(fileName, current, hashTable);
		}
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int readCourseFile(courseHashTable hashTable) {
	FILE* file = fopen("courses.txt", "r");
	if (!file) {
		printf("Error while openning courses file!\n");
		return FILE_ERROR;
	}

	int id = 0;
	char name[MAX_CHAR_LENGTH] = "";
	int ects = 0;
	while (!feof(file)) {
		if (fscanf(file, "%d %49s %d", &id, name, &ects) != 3) {
			printf("fscanf error while reading course file!\n");
			return FSCANF_ERROR;
		}
		addToHash(id, name, ects, hashTable);
	}

	fclose(file);
	return EXIT_SUCCESS;
}

float averageGrade(studentPosition student) {
	int num = 0, sum = 0;
	courseStatusPosition current = student->courses->next;
	while (current != NULL) {
		sum += current->grade;
		num++;
		current = current->next;
	}
	if (num > 0) return ((float)sum / num);
	return 0.0;
}

int totalEcts(studentPosition student) {
	int total = 0;
	courseStatusPosition current = student->courses->next;
	while (current != NULL) {
		total += current->course->ects;
		current = current->next;
	}
	return total;
}

int currentEcts(studentPosition student) {
	int sum = 0;
	courseStatusPosition current = student->courses->next;
	while (current != NULL) {
		if(current->grade > 1) sum += current->course->ects;
		current = current->next;
	}
	return sum;
}

int splitList(studentPosition start, studentPosition* front, studentPosition* back) {
	studentPosition fast = start->next;
	studentPosition slow = start;

	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	*front = start;
	*back = slow->next;
	slow->next = NULL;
	return EXIT_SUCCESS;
}

int sortAlphabetically(studentPosition* start) {
	if (*start == NULL || (*start)->next == NULL)
		return EXIT_SUCCESS;

	studentPosition frontHalf = NULL;
	studentPosition backHalf = NULL;

	splitList(*start, &frontHalf, &backHalf);

	sortAlphabetically(&frontHalf);
	sortAlphabetically(&backHalf);

	*start = mergeSortAlphabetically(frontHalf, backHalf);
	return EXIT_SUCCESS;
}


studentPosition mergeSortAlphabetically(studentPosition a, studentPosition b) {
	if (a == NULL) return b;
	else if (b == NULL) return a;

	if (strcmp(a->surname, b->surname) < 0 || 
		(strcmp(a->surname, b->surname) == 0 && strcmp(a->name, b->name) < 0) || 
		(strcmp(a->surname, b->surname) == 0 && strcmp(a->name, b->name) == 0 && a->birthYear < b->birthYear)) {
		a->next = mergeSortAlphabetically(a->next, b);
		return a;
	}
	else {
		b->next = mergeSortAlphabetically(a, b->next);
		return b;
	}	
}

int sortByAverageGrade(studentPosition* start) {
	if (*start == NULL || (*start)->next == NULL)
		return EXIT_SUCCESS;

	studentPosition frontHalf = NULL;
	studentPosition backHalf = NULL;

	splitList(*start, &frontHalf, &backHalf);

	sortByAverageGrade(&frontHalf);
	sortByAverageGrade(&backHalf);

	*start = mergeSortByAverageGrade(frontHalf, backHalf);
	return EXIT_SUCCESS;
}


studentPosition mergeSortByAverageGrade(studentPosition a, studentPosition b) {
	if (a == NULL) return b;
	else if (b == NULL) return a;

	if (a->averageGrade < b->averageGrade ||
		(a->averageGrade == b->averageGrade &&
			(strcmp(a->surname, b->surname) < 0 ||
				(strcmp(a->surname, b->surname) == 0 &&
					(strcmp(a->name, b->name) < 0 ||
						(strcmp(a->name, b->name) == 0 &&
							a->birthYear < b->birthYear)))))) 
	{
		a->next = mergeSortByAverageGrade(a->next, b);
		return a;
	}
	else {
		b->next = mergeSortByAverageGrade(a, b->next);
		return b;
	}
}

int printAscending(studentPosition head) {
	studentPosition current = head->next;
	printf("Surname\tName\tBirth Year\tAverage Grade\tECTS\n");
	while (current != NULL) {
		printf("%s\t%s\t%d\t\t%.2f\t\t%d/%d\n", current->surname, current->name, current->birthYear, current->averageGrade, current->currentEcts, current->totalEcts);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int printDescending(studentPosition head) {
	studentStackPosition stackHead = (studentStackPosition)malloc(sizeof(studentStack));
	stackHead->student = NULL;
	stackHead->next = NULL;
	studentPosition current = head->next;
	while (current != NULL) {
		studentStackPosition newElement = (studentStackPosition)malloc(sizeof(studentStack));
		if (newElement == NULL) {
			printf("malloc error while allocating student stack!\n");
			return MALLOC_ERROR;
		}
		newElement->student = current;
		newElement->next = stackHead->next;
		stackHead->next = newElement;
		current = current->next;
	}
	studentStackPosition currentStackEl = stackHead->next;
	printf("Surname\tName\tBirth Year\tAverage Grade\tECTS\n");
	while (currentStackEl != NULL) {
		printf("%s\t%s\t%d\t\t%.2f\t\t%d/%d\n", currentStackEl->student->surname, currentStackEl->student->name, currentStackEl->student->birthYear, currentStackEl->student->averageGrade, currentStackEl->student->currentEcts, currentStackEl->student->totalEcts);
		stackHead->next = currentStackEl->next;
		currentStackEl->student = NULL;
		free(currentStackEl);
		currentStackEl = stackHead->next;
	}
	free(stackHead);
	return EXIT_SUCCESS;
}

int printCourse(courseHashTable hashTable, studentPosition head) {
	int id = 0;
	printf("Enter course ID: ");
	if (scanf("%d", &id) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}

	getchar();

	coursePosition hashEl = findInHash(id, hashTable);
	if (hashEl == NULL) {
		printf("Course with ID %d not found!\n", id);
		return ELEMENT_NOT_FOUND;
	}
	printf("COURSE INFO\n-----------------------------------------------------\n");
	printf("Course ID: %3d\n", hashEl->id);
	printf("Course name: %s\n", hashEl->name);
	printf("ECTS: %d\n", hashEl->ects);
	printf("STUDENTS ENROLLED:\nName\tSurname\tBirth Year\tGrade\n");
	studentPosition current = head->next;
	while (current != NULL) {
		courseStatusPosition currentCourse = current->courses->next;
		while (currentCourse != NULL) {
			if (currentCourse->course == hashEl) {
				printf("%s\t%s\t%d\t\t%d\n", current->name, current->surname, current->birthYear, currentCourse->grade);
				break;
			}
			currentCourse = currentCourse->next;
		}
		current = current->next;
	}
	printf("\n");

	char confirm = ' ';
	printf("\nDo you wish to write the student list in a file? [y/n]: ");
	if (scanf("%c", &confirm) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}

	switch (confirm) {
	case 'y':
		writeStudentsInCourse(hashEl, head);
		break;
	case 'n':
		break;
	default:
		printf("INVALID INPUT!\n");
	}

	return EXIT_SUCCESS;
}

int printCourses(courseHashTable hashTable) {
	int i = 0;
	printf(" ID\tName\tECTS\n");
	for (i = 0; i < hashTable->size; i++) {
		coursePosition current = hashTable->hash[i];
		while (current != NULL) {
			printf("%3d\t%s\t%d\n", hashTable->hash[i]->id, hashTable->hash[i]->name, hashTable->hash[i]->ects);
			current = current->next;
		}
	}
	return EXIT_SUCCESS;
}

int printStudent(studentPosition student) {
	printf("STUDENT CARD\n-----------------------------------------------------\n");
	printf("Name: %s\n", student->name);
	printf("Surname: %s\n", student->surname);
	printf("Birth Year: %d\n", student->birthYear);
	printf("Current ECTS: %d\n", student->currentEcts);
	printf("Total ECTS: %d\n", student->totalEcts);
	printf("Average grade: %.2f\n", student->averageGrade);
	courseStatusPosition current = student->courses->next;
	printf("COURSES STATUS:\nID\tName\t\tGrade\n");
	while (current != NULL) {
		printf("%3d\t%s\t%d\n", current->course->id, current->course->name, current->grade);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int writeCourse(int id, char* name, int ects) {
	FILE* file = fopen("courses.txt", "a");
	if (!file) {
		printf("Error while openning courses file!\n");
		return FILE_ERROR;
	}
	fprintf(file, "%3d\t%s\t%d\n", id, name, ects);
	fclose(file);
	return EXIT_SUCCESS;
}

int addCourse(courseHashTable hashTable) {
	int id = 0, ects = 0;
	char s[MAX_CHAR_LENGTH] = "";

	printf("Enter course name: ");
	if (scanf("%49s", s) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}

	printf("Enter the number of ECTS: ");
	if (scanf("%d", &ects) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}

	while (id < hashTable->size) {
		if (findInHash(id, hashTable) == NULL) {
			addToHash(id, s, ects, hashTable);
			writeCourse(id, s, ects);
			return EXIT_SUCCESS;
		}
		id++;
	}

	printf("No free ID found / Hash table full!\n");
	return HASH_FULL;
}

int writeStudent(studentPosition newStudent, char* fileName) {
	FILE* file = fopen("students.txt", "a");
	if (!file) {
		printf("Error while openning main student file!\n");
		return FILE_ERROR;
	}

	fprintf(file, "\n%s\t%s\t%d\t%s", newStudent->surname, newStudent->name, newStudent->birthYear, fileName);

	fclose(file);
	return EXIT_SUCCESS;
}

int writeStudents(studentPosition head) {
	FILE* file = fopen("students.txt", "w");
	if (!file) {
		printf("Error while openning main student file!\n");
		return FILE_ERROR;
	}

	studentPosition current = head->next;
	while (current != NULL) {
		fprintf(file, "%s\t%s\t%d\t%s", current->surname, current->name, current->birthYear, current->fileName);
		current = current->next;
		if (current != NULL) fprintf(file, "\n");
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int writeStudentsInCourse(coursePosition course, studentPosition head) {
	char fileName[MAX_CHAR_LENGTH] = "";
	printf("Name the file to store student list (without .txt): ");
	if (scanf("%45s", fileName) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}
	strncat(fileName, ".txt", 4);
	FILE* file = fopen(fileName, "w");
	if (!file) {
		printf("Error while openning courses file!\n");
		return FILE_ERROR;
	}

	studentPosition current = head->next;
	while (current != NULL) {
		courseStatusPosition currentCourse = current->courses->next;
		while (currentCourse != NULL) {
			if (currentCourse->course == course) {
				fprintf(file, "%s\t%s\t%d\t\t%d\n", current->name, current->surname, current->birthYear, currentCourse->grade);
				break;
			}
			currentCourse = currentCourse->next;
		}
		current = current->next;
	}
	printf("\n");

	fclose(file);
	return EXIT_SUCCESS;
}

int writeStudentCourses(studentPosition student, char* fileName) {
	FILE* file = fopen(fileName, "w");
	if (!file) {
		printf("Error while openning courses file!\n");
		return FILE_ERROR;
	}

	courseStatusPosition current = student->courses->next;
	while (current != NULL) {
		fprintf(file, "%d\t%d", current->course->id, current->grade);
		current = current->next;
		if (current != NULL) fprintf(file, "\n");
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int writeCourses(courseHashTable hashTable) {
	FILE* file = fopen("courses.txt", "w");
	if (!file) {
		printf("Error while openning courses file!\n");
		return FILE_ERROR;
	}

	int i = 0;
	for (i = 0; i < hashTable->size; i++) {
		coursePosition current = hashTable->hash[i];
		while (current != NULL) {
			fprintf(file, "%d %s %d", current->id, current->name, current->ects);
			current = current->next;
			if (current != NULL) fprintf(file, "\n");
		}
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int addStudent(studentPosition head, courseHashTable hashTable) {
	char name[MAX_CHAR_LENGTH] = "";
	char surname[MAX_CHAR_LENGTH] ="";
	int birthYear = 0;

	printf("Enter name surname and birth year (example: John Doe 1999): ");
	if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}

	studentPosition lastEl = head;
	while (lastEl->next != NULL) lastEl = lastEl->next;

	if(addToList(surname, name, birthYear, "", lastEl) != EXIT_SUCCESS) return MALLOC_ERROR;
	
	studentPosition currentStudent = lastEl->next;
	courseStatusPosition current = currentStudent->courses;
	int choice = 0, grade = 0;
	printf("Enter courses info:\n");
	do {
		printf("Enter course ID (enter any negative number to end course inputing): ");
		if (scanf("%d", &choice) != 1) {
			printf("scanf error\n!");
			return SCANF_ERROR;
		}

		if (choice < 0) break;

		coursePosition course = findInHash(choice, hashTable);
		if (course == NULL) { 
			printf("Course does not exist! Try again!\n");
			continue;
		}
		else {
			printf("Enter course grade: ");
			if (scanf("%d", &grade) != 1) {
				printf("scanf error\n!");
				return SCANF_ERROR;
			}
			courseStatusPosition newCourseStatus = (courseStatusPosition)malloc(sizeof(courseStatus));
			if (newCourseStatus == NULL) {
				printf("malloc error while allocating memory for course status!\n");
				return MALLOC_ERROR;
			}
			newCourseStatus->grade = grade;
			newCourseStatus->course = course;
			newCourseStatus->next = NULL;
			current->next = newCourseStatus;
			current = current->next;
		}
	} while (choice >= 0);

	currentStudent->totalEcts = totalEcts(currentStudent);
	currentStudent->currentEcts = currentEcts(currentStudent);
	currentStudent->averageGrade = averageGrade(currentStudent);

	char fileName[MAX_CHAR_LENGTH] = "";
	printf("Name the file to store students courses (without .txt): ");
	if (scanf("%45s", fileName) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}
	strncat(fileName, ".txt", 4);
	strcpy(currentStudent->fileName, fileName);
	writeStudentCourses(currentStudent, fileName);
	writeStudent(currentStudent, fileName);

	return EXIT_SUCCESS;
}

studentPosition findPreviousInList(studentPosition current, studentPosition head) {
	studentPosition prev = head;
	while (prev != NULL && prev->next != NULL) {
		if (prev->next == current) return prev;
		prev = prev->next;
	}
	return NULL;
}

int freeStudentCourseStatus(studentPosition student) {
	courseStatusPosition head = student->courses;
	courseStatusPosition current = head->next;
	while (current != NULL) {
		head->next = current->next;
		current->next = NULL;
		free(current);
		current = head->next;
	}
	free(head);
	return EXIT_SUCCESS;
}

int freeStudent(studentPosition student, studentPosition head) {
	studentPosition previous = findPreviousInList(student, head);
	if (previous == NULL) return NULL_PTR_ERROR;
	previous->next = student->next;
	student->next = NULL;
	freeStudentCourseStatus(student);
	free(student);
	return EXIT_SUCCESS;
}

int freeAllStudents(studentPosition head) {
	studentPosition current = head->next;
	while (current != NULL) {
		freeStudent(current, head);
		current = head->next;
	}
	return EXIT_SUCCESS;
}

int freeHashTable(courseHashTable hashTable) {
	if (hashTable == NULL)
		return HASH_EMPTY;

	for (int i = 0; i < hashTable->size; i++) {
		coursePosition current = hashTable->hash[i];
		while (current != NULL) {
			coursePosition courseToFree = current;
			current = current->next;
			free(courseToFree);
		}
		hashTable->hash[i] = NULL;
	}

	free(hashTable->hash);
	free(hashTable);

	return EXIT_SUCCESS;
}

int deleteCourseStatusFromStudents(coursePosition course, studentPosition head) {
	studentPosition student = head->next;
	while (student != NULL) {
		courseStatusPosition prev = student->courses;
		courseStatusPosition current = student->courses->next;

		while (current != NULL) {
			if (current->course == course) {
				prev->next = current->next;
				free(current);
				writeStudentCourses(student, student->fileName);
				student->totalEcts = totalEcts(student);
				student->currentEcts = currentEcts(student);
				student->averageGrade = averageGrade(student);
				break;
			}
			prev = current;
			current = current->next;
		}
		student = student->next;
	}

	return EXIT_SUCCESS;
}

int deleteStudent(studentPosition head) {
	char surname[MAX_CHAR_LENGTH] = "";
	printf("Input surname of the student you want to delete: ");
	if (scanf("%49s", surname) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}
	getchar();

	studentPosition current = head->next;
	char confirm = ' ';
	while (current != NULL) {
		if (strcmp(surname, current->surname) == 0) {
			printStudent(current);
			printf("\nIs this the student you want to delete? [y/n]: ");
			if (scanf("%c", &confirm) != 1) {
				printf("scanf error!\n");
				return SCANF_ERROR;
			}

			switch (confirm) {
			case 'y':
				if (remove(current->fileName) == 0) printf("Student's filer successfully deleted!\n");
				else printf("Student's file couldn't be deleted!\n");
				freeStudent(current, head);
				writeStudents(head);
				printf("Student deleted!\n");
				return EXIT_SUCCESS;
			case 'n':
				break;
			default:
				printf("INVALID INPUT!\n");
				return INVALID_INPUT;
			}
		}
		current = current->next;
	}
	printf("Student to delete not found!\n");
	return ELEMENT_NOT_FOUND;
}

int deleteCourse(studentPosition head, courseHashTable hashTable) {
	int id = 0;
	printf("Enter ID of the course you want to delete: ");
	if (scanf("%d", &id) != 1) {
		printf("scanf error!\n");
		return SCANF_ERROR;
	}
	coursePosition courseToDelete = findInHash(id, hashTable);
	if (courseToDelete == NULL) {
		printf("Course with ID %d not found!\n", id);
		return ELEMENT_NOT_FOUND;
	}

	deleteCourseStatusFromStudents(courseToDelete, head);
	
	int index = id % hashTable->size;
	coursePosition current = hashTable->hash[index];
	coursePosition prev = NULL;

	while (current != NULL) {
		if (current->id == id) {
		    if (prev == NULL)
			    hashTable->hash[index] = current->next;
			else 
				prev->next = current->next;
			break;
		}
		prev = current;
		current = current->next;
	}

	free(courseToDelete);
	writeCourses(hashTable);
	printf("Course deleted!\n");
	return EXIT_SUCCESS;
}