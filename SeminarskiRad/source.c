#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAX_CHAR_LENGTH 50
#define HASH_FULL 1
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
coursePosition findInHash(int, courseHashTable);
int addToHash(int, char*, int, courseHashTable);
int addToList(char*, char*, int, studentPosition);
int readStudentFile(char *, studentPosition, courseHashTable);
int readMainFile(studentPosition, courseHashTable);
int readCourseFile(courseHashTable);
int sortAlphabetically();
int sortByAverageGrade();
int printAscending(studentPosition);
int printDescending(studentPosition, studentStackPosition);
int printStudent(studentPosition);
int printCourses(courseHashTable, studentPosition);
int printCourse(coursePosition, studentPosition);
int writeStudents(coursePosition, studentPosition);
int writeCourse(int, char*, int);
int addCourse(courseHashTable);
int addStudent(); //dodaje studenta i njegove kolegije i ispisuje podatke u students.txt i ispisuje kolegije u studentovu datoteku
int deleteStudent();
int deleteHash();

int main() {
	student head = { "", "", 0, 0.0, 0, 0, NULL, NULL };
	studentStack stackHead = { NULL, NULL };
	courseHashTable hashTable = initializeHash(103);
	readCourseFile(hashTable);
	readMainFile(&head, hashTable);
	printf("\n");
	//printAscending(&head);
	//printf("\n");
	//printDescending(&head, &stackHead);
	//printf("\n");
	//studentPosition current = &head;
	//current = current->next;
	//while (current != NULL) {
	//	printStudent(current);
	//	printf("\n");
	//	current = current->next;
	//}
	//addCourse(hashTable);
	/*printCourses(hashTable, &head);*/
	//writeStudents(hashTable->hash[12%103], &head);
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

coursePosition findInHash(int key, courseHashTable hashTable) {
	coursePosition current = hashTable->hash[key % hashTable->size];
	if (current == NULL || key != current->id) {
		return NULL;
	}
	return current;
}

int addToList(char* surname, char* name, int year, studentPosition lastEl) {
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
	newEl->courses = (courseStatusPosition)malloc(sizeof(courseStatus));
	if (newEl->courses == NULL) {
		printf("malloc error while allocating memory for course status head!\n");
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
	int totalECTS = 0, currentECTS = 0, gradeSum = 0, num = 0;
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

		gradeSum += grade;
		num++;
		totalECTS += current->course->ects;
		if (grade > 1) currentECTS += current->course->ects;
	}

	student->averageGrade = (float)gradeSum / num;
	student->totalEcts = totalECTS;
	student->currentEcts = currentECTS;

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
		if (fscanf(file, "%s %s %d %s", surname, name, &year, fileName) != 4) {
			printf("fscanf error while reading courses!\n");
			return FSCANF_ERROR;
		}
		if (addToList(surname, name, year, current) == EXIT_SUCCESS) {
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
		if (fscanf(file, "%d %s %d", &id, name, &ects) != 3) {
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
	printf("Surname\tName\tBirth Year\tAverage Grade\tECTS\n");
	while (current != NULL) {
		printf("%s\t%s\t%d\t\t%.2f\t\t%d/%d\n", current->surname, current->name, current->birthYear, current->averageGrade, current->currentEcts, current->totalEcts);
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
	return EXIT_SUCCESS;
}

int printCourse(coursePosition hashEl, studentPosition head) {
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
	return EXIT_SUCCESS;
}

int printCourses(courseHashTable hashTable, studentPosition head) {
	int i = 0;
	for (i = 0; i < hashTable->size; i++) {
		coursePosition current = hashTable->hash[i];
		while (current != NULL) {
			//printf("%3d\t%s\t%d\n", hashTable->hash[i]->id, hashTable->hash[i]->name, hashTable->hash[i]->ects);
			printCourse(current, head);
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
	fprintf(file, "\n%3d\t%s\t%d", id, name, ects);
	fclose(file);
	return EXIT_SUCCESS;
}

int addCourse(courseHashTable hashTable) {
	int id = 0, ects = 0;
	char s[MAX_CHAR_LENGTH] = "";

	printf("Unesite ime kolegija: ");
	if (scanf("%s", s) != 1) {
		printf("scanf error!");
		return SCANF_ERROR;
	}

	printf("Unesite broj ECTS bodova koji nosi kolegij: ");
	if (scanf("%d", &ects) != 1) {
		printf("scanf error!");
		return SCANF_ERROR;
	}

	while (id < hashTable->size) {
		if (hashTable->hash[id % hashTable->size] == NULL) {
			addToHash(id, s, ects, hashTable);
			writeCourse(id, s, ects);
			return EXIT_SUCCESS;
		}
		id++;
	}

	printf("No free ID found / Hash table full!");
	return HASH_FULL;
}

int writeStudents(coursePosition course, studentPosition head) {
	char fileName[MAX_CHAR_LENGTH - 4] = "";
	printf("Name the file to store student list: ");
	if (scanf("%s", fileName) != 1) {
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

int addStudent() {
	return EXIT_SUCCESS;
}
int deleteStudent() {
	return EXIT_SUCCESS;
}
int deleteHash() {
	return EXIT_SUCCESS;
}