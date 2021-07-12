//---------------------------------------------------------------------------------------
//Name: Arun Sangar
//Date: 29 October 2015
//Course: CPSC 223C
//Project: 9.2
//
//Purpose: Practice with Binary Search Trees
//---------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct node NODE;

struct node
{
	int   id;
	char  name[15];
	float balance;
	NODE* leftChild;
	NODE* rightChild;
};

//BST functions
void  BSTClear(NODE**);
void  BSTInsert(NODE**, int, char*, float);
void  BSTDisplayInorder(NODE**);
void  BSTDelete(NODE**, int);

//program functions
void  CopyTXTToBST(char*, NODE**);
void  DisplayMenu();
void  Deposit(NODE**);
void  Withdraw(NODE**);
void  DisplayAccount(NODE**);

//helper functions
NODE* FindNode(NODE**, int);
NODE* FindLeftMost(NODE**);
int   GetID();

int main()
{
	//print time and date
	time_t t = time(NULL);
	printf(ctime(&t));

	NODE* root;

	//initialize BST
	BSTClear(&root);

	CopyTXTToBST("data.txt", &root);

	printf("\tID\t    NAME\tBALANCE\n");
	BSTDisplayInorder(&root);

	char menuChoice;

	//loops until users chooses exit case
	while (1)
	{
		DisplayMenu();

		//get user's menu choice
		printf("Enter a menu option: ");
		scanf("%c", &menuChoice);

		//choose corresponding function to user choice
		switch (menuChoice)
		{
		case 'a': Deposit(&root); break;
		case 'b': Withdraw(&root); break;
		case 'c': DisplayAccount(&root); break;
		case 'd': BSTDelete(&root, GetID()); break;
		case 'e': printf("\tID\t    NAME\tBALANCE\n"); 
			      BSTDisplayInorder(&root); break;
		case 'f': goto end; break;
		default : printf("That is not a valid menu option."); break;
		}
		//remove junk off input array
		scanf("%c", &menuChoice);
	}

//loop exit point
end:

	return 0;
}

void BSTClear(NODE** root) { *root = NULL; };

void BSTInsert(NODE** node, int id,  char *name, float balance)
{
	//insert the node if the current position is null
	if (*node == NULL)
	{
		//allocate memory for new node
		*node = (NODE*)malloc(sizeof(NODE));

		//copy data into the new node
		(*node)->id = id;
		strcpy((*node)->name, name);
		(*node)->balance = balance;

		//intialize the new node's children
		(*node)->leftChild = NULL;
		(*node)->rightChild = NULL;
	}
	//else keep checking the BST for an insertion point
	else
	{
		//if data is less than the current node, check the left child
		if (id < (*node)->id)
			BSTInsert(&(*node)->leftChild, id, name, balance);
		//if data is greater than the current node, check the right child
		if (id > (*node)->id)
			BSTInsert(&(*node)->rightChild, id, name, balance);
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
		printf("\t%2d\t%-10s\t%4.2f\n", (*node)->id, (*node)->name, (*node)->balance);
		//display right child last
		BSTDisplayInorder(&(*node)->rightChild);
	}
}

void BSTDelete(NODE** node, int id) 
{
	//if the node was not found, do not delete anything
	if (*node == NULL) return;

	//if the id is less than the current node's,
	//move further down the left subtree
	if (id < (*node)->id) BSTDelete(&(*node)->leftChild, id);
	
	//if the id is greater than the current node's,
	//move further down the right subtree
	else if (id > (*node)->id) BSTDelete(&(*node)->rightChild, id);

	//if the id is equal to the current node's,
	//delete the node!
	else
	{
		//define temporary variable to hold the node for deletion
		NODE* temp = *node;

		//case 1: the node has no children
		//case 2: the node has one child
		if ((*node)->leftChild == NULL || (*node)->rightChild == NULL)
		{
			//in case 1: set the node to the node's right child, which is NULL
			//in case 2: set the node to the non-empty child node
			if ((*node)->leftChild == NULL) *node = (*node)->rightChild;
			else if ((*node)->rightChild == NULL) *node = (*node)->leftChild;

			//free the memory of the deleted node
			free(temp);
		}

		//case 3: the node has two children
		else
		{
			//find the left most node of the right subtree
			//i.e. the minimum node in the right subtree
			//note: this function also removes the connection between 
			//      the returned node and the parent
			NODE* leftMost = FindLeftMost(&(*node)->rightChild);

			//move the minimum node from the right subtree to the 
			//position of the node set for deletion
			*node = leftMost;

			//set children of the new node to the children of the old node
			leftMost->leftChild = temp->leftChild;
			leftMost->rightChild = temp->rightChild;

			//free the memory of the deleted node
			free(temp);
		}

		printf("Account #%d is now closed\n", id);
	}
}

void CopyTXTToBST(char* filename, NODE** root)
{
	//declare temporary storage
	int id;
	char name[15];
	float balance;

	//open the text file for reading
	FILE* ftxt = fopen(filename, "r");

	//read all the data from the txt file, then insert into the BST
	while (!feof(ftxt))
	{
		fscanf(ftxt, "%d%s%f", &id, name, &balance);
		BSTInsert(&(*root), id, name, balance);
	}

	//close text file
	fclose(ftxt);
}

void DisplayMenu()
{
	printf("\n");
	printf("---------------Menu---------------\n");
	printf("  a. Enter ID number to deposit\n");
	printf("  b. Enter ID number to withdraw\n");
	printf("  c. Enter ID number to see your balance\n");
	printf("  d. Enter ID number to close account\n");
	printf("  e. Display all accounts\n");
	printf("  f. Exit\n");
}

void Deposit(NODE** root)
{
	//get account id and find the user's account
	NODE* node = FindNode(&(*root), GetID());

	//check if the account number is active
	if (node == NULL)
	{
		printf("That account is not active\n");
		return;
	}

	float deposit;

	//get deposit amount from user
	printf("%s, how much would you like to deposit? ", node->name);
	scanf("%f", &deposit);

	//add the deposit amount to the account
	node->balance += deposit;

	//display account info
	printf("\tID\t    NAME\tBALANCE\n");
	printf("\t%2d\t%-10s\t%4.2f\n", node->id, node->name, node->balance);
}

void Withdraw(NODE** root)
{
	//get account id and find the user's account
	NODE* node = FindNode(&(*root), GetID());

	//check if the account is active
	if (node == NULL)
	{
		printf("That account is not active\n");
		return;
	}

	float withdrawal;

	//get withdrawal amount from user
	printf("%s, how much would you like to withdraw? ", node->name);
	scanf("%f", &withdrawal);

	//subtract withdrawal amount from the account
	node->balance -= withdrawal;

	//display account info
	printf("\tID\t    NAME\tBALANCE\n");
	printf("\t%2d\t%-10s\t%4.2f\n", node->id, node->name, node->balance);
}

void DisplayAccount(NODE** root)
{
	//get account id number and find the user's account
	NODE* node = FindNode(&(*root), GetID());

	//check if the account is active
	if (node == NULL)
	{
		printf("That account is not active\n");
		return;
	}

	//display account info
	printf("\tID\t    NAME\tBALANCE\n");
	printf("\t%2d\t%-10s\t%4.2f\n", node->id, node->name, node->balance);
}

NODE* FindNode(NODE** node, int id)
{
	if (*node == NULL) return NULL;

	//if the node is found
	if (id == (*node)->id) return *node;

	//if the id number is less than the current node's
	//move further down the left subtree
	if (id <  (*node)->id) return FindNode(&(*node)->leftChild, id);

	//if the id number is greater than the current node's
	//move further down the right subtree
	if (id >  (*node)->id) return FindNode(&(*node)->rightChild, id); 
}

NODE* FindLeftMost(NODE** node)
{
	//this will remove the connection between the left most child and it's parent
	//then returns the node removed from the right subtree
	if ((*node)->leftChild == NULL)
	{
		NODE* temp = *node;
		*node = NULL;
		return temp;
	}
	//if the node is not the left most, continue down the tree to the left child
	else return FindLeftMost(&(*node)->leftChild);
}

int GetID()
{
	int id;

	//get account number from user
	printf("Enter an id number: ");
	scanf("%d", &id);

	//keep looping until user gives a valid account number
	while (id < 10 || id > 90 || id % 10 != 0)
	{
		printf("That was not valid, enter a new id number: ");
		scanf("%d", &id);
	}

	//return the account number
	return id;
}

/*
Output:

Thu Nov  5 03:11:39 2015
		ID          NAME        BALANCE
		10      Nixon,Richard   777.77
		20      Ragan,Ronald    999.25
		30      Ford,Jerald     222.44
		40      Bush,George_W   789.10
		50      Smith,John      123.45
		60      Obama,Barack    688.90
		70      Clinton,Bill    456.78
		90      Bush,George     793.22

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: a
Enter an id number: 40
Bush,George_W, how much would you like to deposit? 100.00
ID          NAME        BALANCE
40      Bush,George_W   889.10

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: b
Enter an id number: 20
Ragan,Ronald, how much would you like to withdraw? 35.25
ID          NAME        BALANCE
20      Ragan,Ronald    964.00

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: c
Enter an id number: 70
ID          NAME        BALANCE
70      Clinton,Bill    456.78

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: d
Enter an id number: 50
Account #50 is now closed

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: e
		ID          NAME        BALANCE
		10      Nixon,Richard   777.77
		20      Ragan,Ronald    964.00
		30      Ford,Jerald     222.44
		40      Bush,George_W   889.10
		60      Obama,Barack    688.90
		70      Clinton,Bill    456.78
		90      Bush,George     793.22

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: a
Enter an id number: 50
That account is not active

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: c
Enter an id number: 64
That was not valid, enter a new id number: 35
That was not valid, enter a new id number: 40
ID          NAME        BALANCE
40      Bush,George_W   889.10

---------------Menu---------------
  a. Enter ID number to deposit
  b. Enter ID number to withdraw
  c. Enter ID number to see your balance
  d. Enter ID number to close account
  e. Display all accounts
  f. Exit
Enter a menu option: f


*/