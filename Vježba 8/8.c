#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define EMPTY_TREE 2
#define EMPTY_QUEUE 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

struct _Node;
typedef struct _Node* Child;
typedef struct _Node {
	int value;
	Child left;
	Child right;
} Node;

struct _Queue;
typedef struct _Queue* Position;
typedef struct _Queue {
	Child node;
	Position next;
} Queue;

int enqueue(Position, Child);
int dequeue(Position);
Child findMin(Child);
Child findMax(Child);
Child insertElement(int, Child);
int printInorder(Child);
int printPreorder(Child);
int printPostorder(Child);
int printLevelOrder(Child, Position);
Child deleteElement(int, Child);
Child searchTree(int, Child);
int deleteTree(Child);

int main() {
	Child root = NULL;

	root = insertElement(5, root);
	root = insertElement(3, root);
	root = insertElement(8, root);
	root = insertElement(2, root);
	root = insertElement(4, root);
	root = insertElement(7, root);
	root = insertElement(9, root);
	root = insertElement(1, root);
	root = insertElement(6, root);

	printf("Inorder: ");
	printInorder(root);
	printf("\n");
	printf("Preorder: ");
	printPreorder(root);
	printf("\n");
	printf("Postorder: ");
	printPostorder(root);
	printf("\n");

	Queue levelOrder = {NULL, NULL};
	Position head = &levelOrder;
	printf("Level order:\n");
	printLevelOrder(root, head);

	Child search1 = searchTree(6, root);
	if (search1 != NULL) printf("found\n");
	else printf("not found\n");
	Child search2 = searchTree(11, root);
	if (search2 != NULL) printf("found\n");
	else printf("not found\n");

	root = deleteElement(7, root);
	printPreorder(root);
	printf("\n");
	root = deleteElement(6, root);
	printPreorder(root);
	printf("\n");

	deleteTree(root);
	return EXIT_SUCCESS;
}

int enqueue(Position head, Child node) {
	Position current = head;
	while (current->next != NULL) current = current->next;
	Position newEl = (Position)malloc(sizeof(Queue));
	if (newEl == NULL) {
		printf("malloc error!\n");
		return MALLOC_ERROR;
	}
	newEl->node = node;
	newEl->next = NULL;
	current->next = newEl;
	return EXIT_SUCCESS;
}

int dequeue(Position head) {
	if (head->next == NULL) {
		printf("Queue is empty!\n");
		return EMPTY_QUEUE;
	}
	Position current = head->next;
	head->next = current->next;
	current->next = NULL;
	free(current);
	return EXIT_SUCCESS;
}

Child findMin(Child root) {
	if (root == NULL) return NULL;
	if (root->left == NULL) return root;
	return findMin(root->left);
}

Child findMax(Child node) {
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

int printInorder(Child root) {
	if (root == NULL) return EXIT_SUCCESS;
	if (root->left != NULL) printInorder(root->left);
	printf("%d ", root->value);
	if (root->right != NULL) printInorder(root->right);
	return EXIT_SUCCESS;
}

int printPreorder(Child root) {
	if (root == NULL) return EXIT_SUCCESS;
	printf("%d ", root->value);
	if (root->left != NULL) printPreorder(root->left);
	if (root->right != NULL) printPreorder(root->right);
	return EXIT_SUCCESS;
}

int printPostorder(Child root) {
	if (root == NULL) return EXIT_SUCCESS;
	if (root->left != NULL) printPostorder(root->left);
	if (root->right != NULL) printPostorder(root->right);
	printf("%d ", root->value);
	return EXIT_SUCCESS;
}

int printLevelOrder(Child root, Position head) {
	if (root == NULL) {
		printf("Tree is empty");
		return EMPTY_TREE; 
	}
	enqueue(head, root);
	while (head->next != NULL) {
		Child current = head->next->node;
		printf("%d ", current->value);
		if (current->left != NULL) enqueue(head, current->left);
		if (current->right != NULL) enqueue(head, current->right);
		dequeue(head);
	}
	printf("\n");
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

Child searchTree(int x, Child root) {
	if (root == NULL) return NULL;
	if (root->value > x) return searchTree(x, root->left);
	if (root->value < x) return searchTree(x, root->right);
	return root;
}

int deleteTree(Child root) {
	if (root != NULL) {
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
	return EXIT_SUCCESS;
}