//----------------------------------------------------------------------------
//Name: Arun Sangar
//Course: CPSC 223C
//Date: 5 November 2015
//Quiz 10.1
//----------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct node NODE;

struct node
{
	int info;
	NODE *next;
};

//ordered list functions
void OListClear(NODE**);
void OListPush(NODE**, int);
void OListDisplay(NODE**);
void OListDestruct(NODE**);
void OListInsertData(NODE**, char);
bool OListFindNode(NODE**, int);


//program functions
void Union(NODE**, NODE**, NODE**);
void Intersection(NODE**, NODE**, NODE**);

int main()
{
	//print date and time
	time_t t = time(NULL);

	//define set A and B and temporary storage for input
	NODE *setA, *setB;
	int temp;

	//initialize the oredered lists
	OListClear(&setA);
	OListClear(&setB);

	//get data for set A
	OListInsertData(&setA, 'A');

	//get data for set B
	OListInsertData(&setB, 'B');

	//define sets A(union)B and A(intersection)B
	NODE *setAUB, *setAIB;

	//intialize both ordered lists
	OListClear(&setAUB);
	OListClear(&setAIB);

	//call function to find the union of the two sets
	//(exists in either)
	Union(&setAUB, &setA, &setB);

	//call function to find the intersection fo the two sets
	//(exists in both)
	Intersection(&setAIB, &setA, &setB);

	printf("\n");

	//display set A
	printf("Set A: ");
	OListDisplay(&setA);

	//display set B
	printf("Set B: ");
	OListDisplay(&setB);

	//display union of A and B
	printf("Set A U B: ");
	OListDisplay(&setAUB);

	//display intersection of A and B
	printf("Set A I B: ");
	OListDisplay(&setAIB);

	//clear allocated memory of all the ordered lists
	OListDestruct(&setA);
	OListDestruct(&setB);
	OListDestruct(&setAUB);
	OListDestruct(&setAIB);

	return 0;
}

void OListClear(NODE** olist)
{
	*olist = NULL;
}

void OListPush(NODE** olist, int x)
{
	//allocate memory for new node and declare a cursor for the loop
	NODE* t = (NODE*)malloc(sizeof(NODE)), *cursor = *olist, *follow = *olist;

	//move data into the new node
	t->info = x;
	t->next = NULL;

	//find the insertion point
	while (cursor != NULL && cursor->info < t->info)
	{
		follow = cursor;
		cursor = cursor->next;
	}

	//check if the new node should be placed in the first position
	if (cursor == *olist)
	{
		t->next = cursor;
		*olist = t;
	}
	//check if the new node should be placed in the last position
	else if (cursor == NULL)
	{
		follow->next = t;
	}
	//executes if the new node should be placed somewhere in the middle
	else
	{
		t->next = cursor;
		follow->next = t;
	}
}

void OListDisplay(NODE** olist)
{
	//declare a cursor
	NODE* cursor = *olist;

	//loop through every node in the ordered list
	while (cursor != NULL)
	{
		//print the current node's info
		printf("%d->", cursor->info);
		//move the cursor to the next node
		cursor = cursor->next;
	}
	//print end message
	printf("NULL\n");
}

void OListInsertData(NODE** olist, char setName)
{
	int temp;

	//ask user to input datas into the ordered list
	printf("Enter data for set %c:\n", setName);
	printf(" Enter a group of positive integers with -1 at the end: ");

	//get data until user types '-1'
	scanf("%d", &temp);
	while (temp != -1)
	{
		OListPush(&(*olist), temp);
		scanf("%d", &temp);
	}
}

bool OListFindNode(NODE** olist, int x)
{
	NODE* cursor = *olist;

	//check every node for a match
	while (cursor != NULL)
	{
		//return true if found
		if (cursor->info == x) return true;
		cursor = cursor->next;
	}

	//if not found, return false
	return false;
}

void OListDestruct(NODE** olist)
{
	//decalre a cursor and temporary storage
	NODE* cursor = *olist, *temp;

	while (cursor != NULL)
	{
		//save the pointer to the next node in temporary storage
		temp = cursor->next;
		//free the allocated memory
		free(cursor);
		//set the cursor as the next node
		cursor = temp;
	}
}

void Union(NODE** setAUB, NODE** setA, NODE** setB)
{
	NODE* cursor = *setA;

	//push all elements from set A onto the union set
	while (cursor != NULL)
	{
		OListPush(&(*setAUB), cursor->info);
		//move the next element in set A
		cursor = cursor->next;
	}

	//reset the cursor to the start of set B
	cursor = *setB;

	//go through each element in set B
	while (cursor != NULL)
	{
		//check if the element already exists in the union set
		if(!OListFindNode(&(*setAUB),cursor->info)) 
			//push the element if it doesn't exist in the union set
			OListPush(&(*setAUB), cursor->info);
		//move to the next node in set B
		cursor = cursor->next;
	}

}

void Intersection(NODE** setAIB, NODE** setA, NODE** setB)
{
	NODE* cursor = *setA;

	//check e every node in set A
	while (cursor != NULL)
	{
		//if the current node in set A exists in set B
		if (OListFindNode(&(*setB), cursor->info))
			//push the element onto the stack
			OListPush(&(*setAIB), cursor->info);
		//move the cursor to the next node in set A
		cursor = cursor->next;
	}

	//There is no need to check set B because we already know that those
	//extra elements in B do not exist in set A
}

/*
Output:

Enter data for set A:
Enter a group of positive integers with -1 at the end: 9 2 7 3 -1
Enter data for set B:
Enter a group of positive integers with -1 at the end: 2 8 7 3 11 10 -1

Set A: 2->3->7->9->NULL
Set B: 2->3->7->8->10->11->NULL
Set A U B: 2->3->7->8->9->10->11->NULL
Set A I B: 2->3->7->NULL

*/