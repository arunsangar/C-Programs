//---------------------------------------------------------------------------------------
//Name: Arun Sangar
//Date: 29 October 2015
//Course: CPSC 223C
//Project: 9.1
//
//Purpose: Practice with Binary Search Trees
//---------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct node NODE;

struct node
{
	char month[15];
	int numDays;
	NODE* leftChild;
	NODE* rightChild;
};

void BSTClear(NODE**);
void BSTInsert(NODE**, char*, int);
void BSTDisplayInorder(NODE**);
void BSTDisplayLeaves(NODE**);
void BSTDisplayFullParent(NODE**);
void BSTDisplayHalfParent(NODE**);
int  BSTNumNodes(NODE**);
int  BSTHeight(NODE**);
int  Max(int, int);

int main()
{
	//print date and time
	time_t t = time(NULL);
	printf(ctime(&t));

	//open text file for reading data
	FILE* ftxt = fopen("data.txt", "r");

	//define root of the BST and temporary storage
	NODE* root;
	char month[15];
	int numDays;

	//intialize the BST
	BSTClear(&root);

	//read all the data from the text file and insert into the BST
	while (!feof(ftxt))
	{
		fscanf(ftxt, "%s%d", month, &numDays);
		BSTInsert(&root, month, numDays);
	}

	//display the BST with the inorder traversal
	printf("%15s   %8s\n", "Month's Name", "No. Days");
	BSTDisplayInorder(&root);

	//display leaves of the BST
	printf("Leaves of the tree are: ");
	BSTDisplayLeaves(&root);
	printf("\n");

	//display parents with two children
	printf("Parents with two children are: ");
	BSTDisplayFullParent(&root);
	printf("\n");

	//display parents with one child
	printf("Parents with only one child are: ");
	BSTDisplayHalfParent(&root);
	printf("\n");

	//display the number of nodes
	printf("There are %d nodes in the tree\n", BSTNumNodes(&root));

	//display the height of the BST
	printf("The height of the tree is %d\n", BSTHeight(&root));

	return 0;
}

void BSTClear(NODE** root) { *root = NULL; };

void BSTInsert(NODE** node, char* month, int numDays)
{
	//insert the node if the current position is null
	if (*node == NULL)
	{
		//allocate memory for new node
		*node = (NODE*)malloc(sizeof(NODE));

		//copy data into the new node
		strcpy((*node)->month, month);
		(*node)->numDays = numDays;

		//intialize the new node's children
		(*node)->leftChild = NULL;
		(*node)->rightChild = NULL;
	}
	//else keep checking the BST for an insertion point
	else
	{
		//if data is less than the current node, check the left child
		if (strcmp(month, (*node)->month) < 0) 
			BSTInsert(&(*node)->leftChild, month, numDays);
		//if data is greater than the current node, check the right child
		if (strcmp(month, (*node)->month) > 0)
			BSTInsert(&(*node)->rightChild, month, numDays);
	}
}

void BSTDisplayInorder(NODE** node)
{
	//check if the node exists
	if (*node != NULL)
	{
		//display left child first
		BSTDisplayInorder(&(*node)->leftChild);
		//display current node
		printf("     %-10s\t    %2d\n", (*node)->month, (*node)->numDays);
		//display right child last
		BSTDisplayInorder(&(*node)->rightChild);
	}
}

void BSTDisplayLeaves(NODE** node)
{
	//if both children are null, then display the leaf node
	if ((*node)->leftChild == NULL && (*node)->rightChild == NULL)
		printf("%s ", (*node)->month);

	//else keep checking for leaf nodes
	else
	{
		//check the left child, if it's not empty
		if ((*node)->leftChild != NULL)  BSTDisplayLeaves(&(*node)->leftChild);
		//check the right child, if it's not empty
		if ((*node)->rightChild != NULL) BSTDisplayLeaves(&(*node)->rightChild);
	}
}

void BSTDisplayFullParent(NODE** node)
{
	//if the current node has two children, display the full parent node
	if ((*node)->leftChild != NULL && (*node)->rightChild != NULL)
		printf("%s ", (*node)->month);

	//keep checking current node's children for more full parents

	//check left child, if it's not empty
	if ((*node)->leftChild != NULL)  BSTDisplayFullParent(&(*node)->leftChild);
	//check right child, if it's not empty
	if ((*node)->rightChild != NULL) BSTDisplayFullParent(&(*node)->rightChild);
}

void BSTDisplayHalfParent(NODE** node)
{
	//using the "exclusive or" operator, check if the one of the children exists
	//note: this is a bitwise operator
	//display the half parent, if only one child exists
	if ((*node)->leftChild == NULL ^ (*node)->rightChild == NULL)
		printf("%s ", (*node)->month);

	//keep checking current node's children for more half parents

	//check left child, if it's not empty
	if ((*node)->leftChild != NULL)  BSTDisplayHalfParent(&(*node)->leftChild);
	//check right child, if it's not empty
	if ((*node)->rightChild != NULL) BSTDisplayHalfParent(&(*node)->rightChild);
}

int BSTNumNodes(NODE** node)
{
	//return 0 if the node is empty, it does not count towards the number of nodes
	if (*node == NULL) return 0;
	//else check the children and add one to the number of nodes
	else return 1 + BSTNumNodes(&(*node)->leftChild) + BSTNumNodes(&(*node)->rightChild);
}

int BSTHeight(NODE** node)
{
	//return 0 if the node is empty, it does not count towards the height
	if (*node == NULL) return 0;
	//else add one to the maximum height of the two child nodes
	return 1 + Max(BSTHeight(&(*node)->leftChild), BSTHeight(&(*node)->rightChild));
}

//helper function, finds max of two integers
int Max(int a, int b) { return a > b ? a : b; }

/*
Output:

Thu Nov  5 00:13:51 2015
   Month's Name   No. Days
	 April          30
	 August         31
	 December       31
	 February       28
	 January        31
	 July           31
	 June           30
	 March          31
	 May            31
	 November       30
	 October        31
	 September      30
Leaves of the tree are: December July November
Parents with two children are: January March
Parents with only one child are: February April August June May September October
There are 12 nodes in the tree
The height of the tree is 6

*/