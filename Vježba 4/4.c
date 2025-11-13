#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXIT_SUCCESS 0
#define FILE_ERROR -1
#define FSCANF_ERROR -2
#define MALLOC_ERROR -3

struct _Polynom;
typedef struct _Polynom* Position;
typedef struct _Polynom
{
	int coef;
	int exp;
	Position next;
} Polynom;

int allocatePolynom(Position, int, int);
int readFile(Position, Position);
int printPolynom(Position);
int addPolynoms(Position, Position, Position);
int multiplyPolynoms(Position, Position, Position);
int freeAllocatedMemory(Position);

int main() {
	Polynom firstPolynom = {
		.coef = 0,
		.exp = 0,
		.next = NULL
	};

	Polynom secondPolynom = {
		.coef = 0,
		.exp = 0,
		.next = NULL
	};

	readFile(&firstPolynom, &secondPolynom);
	printf("1st polynom: ");
	printPolynom(&firstPolynom);
	printf("2nd polynom: ");
	printPolynom(&secondPolynom);

	Polynom sum = {
		.coef = 0,
		.exp = 0,
		.next = NULL
	};

	addPolynoms(&firstPolynom, &secondPolynom, &sum);
	printf("Sum of polynoms: ");
	printPolynom(&sum);

	Polynom product = {
		.coef = 0,
		.exp = 0,
		.next = NULL
	};

	multiplyPolynoms(&firstPolynom, &secondPolynom, &product);
	printf("Product of polynoms: ");
	printPolynom(&product);

	freeAllocatedMemory(&firstPolynom);                 
	freeAllocatedMemory(&secondPolynom);
	freeAllocatedMemory(&sum);
	freeAllocatedMemory(&product);
	return 0;
}

int allocatePolynom(Position head, int newCoef, int newExp) {
	Position newPolynom = (Position)malloc(sizeof(Polynom));
	if (!newPolynom) {
		printf("Memory allocation error!\n");
		exit(MALLOC_ERROR);
	}
	newPolynom->coef = newCoef;
	newPolynom->exp = newExp;
	newPolynom->next = NULL;

	Position previous = head;
	Position current = head->next;

	while (current != NULL && current->exp > newExp) {
		previous = current;
		current = current->next;
	}

	previous->next = newPolynom;
	newPolynom->next = current;

	return EXIT_SUCCESS;
}

int readFile(Position polynom1, Position polynom2) {
	FILE* file;
	file = fopen("polynom.txt", "r");
	if (file == NULL) {
		printf("Error while trying to open the file!\n");
		exit(FILE_ERROR);
	}

	int tempCoef = 0, tempExp = 0;
	while (!feof(file)) {
		if (fscanf(file, "%d %d", &tempCoef, &tempExp) != 2) {
			printf("Error while reading file with fscanf\n");
			fclose(file);
			exit(FSCANF_ERROR);
		}
		allocatePolynom(polynom1, tempCoef, tempExp);
		if (fgetc(file) == '\n') break; 
		fseek(file, -1, SEEK_CUR);
	}

	while (!feof(file)) {
		if (fscanf(file, "%d %d", &tempCoef, &tempExp) != 2) {
			printf("Error while reading file with fscanf\n");
			fclose(file);
			exit(FSCANF_ERROR);
		}
		allocatePolynom(polynom2, tempCoef, tempExp);
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int printPolynom(Position head) {
	Position current = head->next;
	while (current != NULL) {
		printf("%dx^%d ", current->coef, current->exp);
		if (current->next != NULL) printf("+ ");
		current = current->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int addPolynoms(Position polynom1, Position polynom2, Position sum) {
	Position current1 = polynom1->next;
	Position current2 = polynom2->next;

	while (current1 != NULL) {
		while (current2 != NULL && current1->exp != current2->exp) {
			current2 = current2->next;
		}
		if (current2 == NULL) {
			allocatePolynom(sum, current1->coef, current1->exp);
		}
		else allocatePolynom(sum, current1->coef + current2->coef, current1->exp);
		current2 = polynom2->next;
		current1 = current1->next;
	}

	//checking if any member of polynom2 was skipped
	Position currentSum = sum->next;
	int expPresent = 0;
	while (current2 != NULL) {
		expPresent = 0;
		while (currentSum != NULL) {
			if (current2->exp == currentSum->exp) {
				expPresent = 1;
				break;
			}
			currentSum = currentSum->next;
		}
		if (expPresent == 0) {
			allocatePolynom(sum, current2->coef, current2->exp);
		}
		expPresent = 0;
		current2 = current2->next;
		currentSum = sum->next;
	}

	return EXIT_SUCCESS;
}

int multiplyPolynoms(Position polynom1, Position polynom2, Position product) {
	Position current1 = polynom1->next;
	Position current2 = polynom2->next;

	while (current1 != NULL) {
		current2 = polynom2->next;
		while (current2 != NULL) {
			allocatePolynom(product, current1->coef * current2->coef, current1->exp + current2->exp);
			current2 = current2->next;
		}
		current1 = current1->next;
	}

	//add elements of polynom with same exponent
	Position currentProduct = product->next;
	while (currentProduct != NULL && currentProduct->next != NULL) {
		Position nextProduct = currentProduct->next;
		if (currentProduct->exp == nextProduct->exp) {  
			currentProduct->coef += nextProduct->coef;
			currentProduct->next = nextProduct->next;
			free(nextProduct);
		}
		else {
			currentProduct = currentProduct->next;
		}
	}

	return EXIT_SUCCESS;
}

int freeAllocatedMemory(Position head) {
	Position current = head->next;
	while (current != NULL) {
		Position previous = current;
		current = current->next;
		free(previous);
	}
	return EXIT_SUCCESS;
}