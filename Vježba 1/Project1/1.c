#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FILE_NOT_OPENED (-1)
#define MAX_CHAR 20
#define MAX_POINTS 50

struct _Student;
typedef struct _Student* Position;
typedef struct _Student
{
	char name[MAX_CHAR];
	char surname[MAX_CHAR];
	int points;
	Position next;
} Student;

int main() {
	int numberOfStudents = 0, i = 0;

	FILE* inputFile = fopen("students.txt", "r");
	if (inputFile == NULL) {
		printf("ERROR!\n");
		return FILE_NOT_OPENED;
	}

	while (!feof(inputFile)) {
		if (fgetc(inputFile) == '\n')
			numberOfStudents++;
	}
	rewind(inputFile);

	Student* students = (Student*)malloc(numberOfStudents * sizeof(Student));

	for (i = 0; i < numberOfStudents; i++) {
		fscanf(inputFile, "%s %s %d", students[i].name, students[i].surname, &students[i].points);
	}

	printf("Name\tSurname\t\tAbsolute\tRelative\n");
	for (i = 0; i < numberOfStudents; i++) {
		printf("%s\t%s\t%d\t\t%.2f\n", students[i].name, students[i].surname, students[i].points, (float)students[i].points/MAX_POINTS*100);
	}

	free(students);
	fclose(inputFile);
	return 0;
}