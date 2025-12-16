#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define EXIT_SUCCESS 0
//#define MALLOC_ERROR -1

struct _Node;
typedef struct _Node* Child;
typedef struct _Node* Position;
typedef struct _Node {
	int value;
	Child left;
	Child right;
} Node;

Position findMin(Child);
Position findMax(Child);
Child insertElement(int, Child);
int printInorder(Position);
int printPreorder(Position);
int printPostorder(Position);
int printLevelOrder(Position);
Child deleteElement(int, Child);
Position searchTree(int, Position);
int deleteTree(Position);

int main() {
	Position root = NULL;

	root = insertElement(4, root);
	root = insertElement(9, root);
	root = insertElement(2, root);
	root = insertElement(10, root);
	root = insertElement(6, root);
	root = insertElement(3, root);
	root = insertElement(1, root);
	root = insertElement(5, root);
	root = insertElement(7, root);

	searchTree(6, root);
	searchTree(11, root);

	root = deleteElement(7, root);
	root = deleteElement(6, root);

	deleteTree(root);
	return EXIT_SUCCESS;
}

Position findMin(Child root) {
	if (root == NULL) return NULL;
	if (root->left == NULL) return root;
	return findMin(root->left);
}

Position findMax(Child node) {
	Child root = node;
	if (root != NULL) {
		while (root->right != NULL) {
			root = root->right;
		}
	}
	return root;
}

Child insertElement(int x, Child root) {
	if (root == NULL) {
		root = (Child)malloc(sizeof(Node));
		if (root == NULL) {
			printf("malloc error!\n");
			return NULL;
		}
		root->value = x;
		root->left = NULL;
		root->right = NULL;
	}
	else if (root->value > x) root->left = insertElement(x, root->left);
	else if (root->value < x) root->right = insertElement(x, root->right);
	return root;
}

int printInorder(Position root) {
	return EXIT_SUCCESS;
}

int printPreorder(Position root) {
	return EXIT_SUCCESS;
}

int printPostorder(Position root) {
	return EXIT_SUCCESS;
}

int printLevelOrder(Position root) {
	return EXIT_SUCCESS;
}

Child deleteElement(int x, Child root) {
	Child temp = NULL;
	if (root == NULL) {
		printf("Element with value %d not found!\n", x);
	}
	else if (root->value > x) root->left = deleteElement(x, root->left);
	else if (root->value < x) root->right = deleteElement(x, root->right);
	else if (root->left != NULL && root->right != NULL) {
		temp = findMin(root->right);
		root->value = temp->value;
		root->right = deleteElement(root->value, root->right);
	}
	else {
		temp = root;
		if (root->left == NULL) root = root->right;
		else root = root->left;
		free(temp);
	}
	return root;
}

Position searchTree(int x, Position root) {
	if (root == NULL) return NULL;
	if (root->value > x) return searchTree(x, root->left);
	if (root->value < x) return searchTree(x, root->right);
	return root;
}

int deleteTree(Position root) {
	if (root != NULL) {
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
	return EXIT_SUCCESS;
}