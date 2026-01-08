#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAX_CHAR_LENGTH 50
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_ERROR -2
#define SCANF_ERROR -3
#define FSCANF_ERROR -4

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
int addToHash(int, char*, int, courseHashTable);
int deleteHash();
//citanje
int readStudentFile(char *);
int readMainFile();
int readCourseFile(courseHashTable);
//sortiranje studenata
int sortAlphabetically();
int sortByAverageGrade();
//ISPIS
int printAscending(studentPosition); //red
int printDescending(studentPosition, studentStackPosition); //stog
int printStudent(studentPosition); //printa name surname birthyear coursesstatus ects(polozeno/ukupno) averagegrade
int printCourse(courseHashTable); //printa id name ects i popis studenata
int printStudentListToFile(); //ispisuje listu studenata u txt file
//dodavanje
int addCourse(); //dodaje kolegij i ispisuje ga u courses.txt
int addStudent(); //dodaje studenta i njegove kolegije i ispisuje podatke u students.txt i ispisuje kolegije u studentovu datoteku
int deleteStudent(); 

int main() {
	courseHashTable hashTable = initializeHash(103);
	readCourseFile(hashTable);
	printCourse(hashTable);
	return EXIT_SUCCESS;
}

courseHashTable initializeHash(int hashSize) {
	courseHashTable hashTable = (courseHashTable)malloc(sizeof(courseHash));
	if (hashTable == NULL) {
		printf("malloc error while allocating hashTable!\n");
		exit(MALLOC_ERROR);
	}
	hashTable->size = hashSize;
	hashTable->hash = (course *)malloc(hashTable->size*sizeof(course));
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

int readStudentFile(char* fileName) {
	FILE* studentFile = fopen(fileName, "r");
	if (!studentFile) {
		printf("Error while openning student file!\n");
		return FILE_ERROR;
	}
	fclose(studentFile);
	return EXIT_SUCCESS;
}

int readMainFile() {
	FILE* file = fopen("students.txt", "r");
	if (!file) {
		printf("Error while openning main student file!\n");
		return FILE_ERROR;
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
			printf("fscanf error!\n");
			return FSCANF_ERROR;
		}
		addToHash(id, name, ects, hashTable);
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int sortAlphabetically() {
	return EXIT_SUCCESS;
}
int sortByAverageGrade() {
	return EXIT_SUCCESS;
}

int printAscending(studentPosition head) {
	studentPosition current = head->next;
	printf("Surname\tName\tBirth Year\tAverage grade\tECTS\n");
	while (current != NULL) {
		printf("%s\t%s\t%d\t%f\t%d/%d\n", current->surname, current->name, current->birthYear, current->averageGrade, current->currentEcts, current->totalEcts);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

int printDescending(studentPosition head, studentStackPosition stackHead) {
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
	}
	studentStackPosition currentStackEl = stackHead->next;
	printf("Surname\tName\tBirth Year\tAverage grade\tECTS\n");
	while (currentStackEl != NULL) {
		printf("%s\t%s\t%d\t%f\t%d/%d\n", currentStackEl->student->surname, currentStackEl->student->name, currentStackEl->student->birthYear, currentStackEl->student->averageGrade, currentStackEl->student->currentEcts, currentStackEl->student->totalEcts);
		stackHead->next = currentStackEl->next;
		free(currentStackEl);
		currentStackEl = stackHead->next;
	}
	return EXIT_SUCCESS;
}

int printCourse(courseHashTable hashTable) {
	int i = 0;
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
	printf("STUDENT CARD\n-----------------------------------------------\n");
	printf("Name: %s\n", student->name);
	printf("Surname: %s\n", student->surname);
	printf("Birth Year: %d\n", student->birthYear);
	printf("Current ECTS: %d\n", student->currentEcts);
	printf("Total ECTS: %d\n", student->totalEcts);
	printf("Average grade: %f\n", student->averageGrade);
	courseStatusPosition current = student->courses->next;
	printf("COURSES STATUS:\nID\tName\tGrade\n");
	while (current != NULL) {
		printf("%d\t%n\t%d\n", current->course->id, current->course->name, current->grade);
		current = current->next;
	}
	return EXIT_SUCCESS;
}
int addCourse() {
	return EXIT_SUCCESS;
}
int addStudent() {
	return EXIT_SUCCESS;
}
int printStudentListToFile() {
	return EXIT_SUCCESS;
}
int deleteStudent() {
	return EXIT_SUCCESS;
}
int deleteHash() {
	return EXIT_SUCCESS;
}