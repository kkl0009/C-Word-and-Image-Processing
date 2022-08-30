/*
* Name: Kollin Labowski
* Date: October 26, 2020
* Class: CS 350
* Description: The following program takes text from a specified input file and prints out 5 tables displaying the
* contents of the file. The actual displays of each individual table are detailed below. The program makes use of a
* LinkedList data structure, which has been created using a structure. See below for more details on each feature.
*/

//Include statements used to provide various functionality to the program
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

//Constants used mainly to standardize lengths for arrays
const int ALPHABET_SIZE = 26;
const int WORD_LENGTH = 20;
const int FILE_NAME_LENGTH = 40;

//Used to access both the input and output files
FILE *filePtr;
FILE *outPtr;

//Used to determine the number of occurrences of each letter and each word length respectively
int letterOccurences[26];
int lengthOccurences[20];

//Structure used to simulate a node in a LinkedList structure
struct List
{
	char word[20];
	int occurences;
	struct List* next;
};

//Used to refer to individual pieces of data within the List
typedef struct List* Node;

//Refers to the head of the main LinkedList
Node head;

//Function prototypes: see below for individual functionality
Node makeNode();
void addNode(Node startNode, char addedWord[WORD_LENGTH]);
void cleanWord(char inWord[WORD_LENGTH]);
void printTables();
void outputTables();

/*
* Main method. This is where the input and optional output files are read in and where the main 
* LinkedList is created and filled with values.
*/
void main(int argc, char *argv[])
{
	//Initialize the input (and optionally output) file names
	char inFileName[FILE_NAME_LENGTH];
	for(int i = 0;  i < FILE_NAME_LENGTH; i++)
	{
		inFileName[i] = argv[1][i];
	}
	char outFileName[FILE_NAME_LENGTH];
	if(argc == 3)
	{
		for(int i = 0;  i < FILE_NAME_LENGTH; i++)
		{
			outFileName[i] = argv[2][i];
		}
	}

	//Open the input file if it exists
	filePtr = fopen(inFileName, "r");
	if(filePtr == NULL)
	{
		printf("Input file name is invalid");
		return;
	}

	//Fill the tracking arrays with values to avoid errors
	for(int i = 0; i < ALPHABET_SIZE; i++)
		letterOccurences[i] = 0;

	for(int i = 0; i < WORD_LENGTH; i++)
		lengthOccurences[i] = 0;

	//Begin to make the List
	head = makeNode();
	Node cur;
	char curWord[WORD_LENGTH];
	for(int i = 0; i < WORD_LENGTH; i++)
		curWord[i] = ' ';

	//Fill the List with values until the end of the file has been reached
	while(!feof(filePtr))
	{
		fscanf(filePtr, "%s", curWord);
		cleanWord(curWord);
		for(int i = 0; curWord[i] != '\0'; i++)
		{
			if(!ispunct(curWord[i]) && curWord[i] != ' ')
				letterOccurences[curWord[i] - 97]++;
			if(i + 1 < WORD_LENGTH && (curWord[i + 1] == ' ' || curWord[i + 1] == '\0'))
				lengthOccurences[i]++;
		}
		addNode(head, curWord);
	}

	fclose(filePtr);
	
	cur = head;
	
	//If the optional output file was given, print to the file; otherwise print to console
	if(argc < 3)
	{
		printTables();
	}
	else
	{
		outPtr = fopen(outFileName, "w");
		if(outPtr == NULL)
		{
			printf("Output file name is invalid");
			return;
		}
		outputTables();
	}
}

/*
* This method allocates memory to create a new node, and is called every time a new one is made
*/
Node makeNode()
{
	Node temp;
	temp = (Node)malloc(sizeof(struct List));
	temp->next = NULL;
}

/*
* This method adds a node to the main List, at the end of the List. If the value is already in the List,
* it instead increments the occurences field.
*/
void addNode(Node startNode, char addedWord[WORD_LENGTH])
{
	Node temp, current;
	temp = makeNode();
	for(int i = 0; i < WORD_LENGTH; i++)
	{
		temp->word[i] = addedWord[i];
	}
	if(startNode == NULL)
		startNode = temp;
	else
	{
		current = startNode;
		while(current->next != NULL)
		{
			current = current->next;

			if(strcmp(current->word, temp->word) == 0)
			{
				current->occurences = current->occurences + 1;
				temp = NULL;
				return;
			}
		}
		current->next = temp;
	}
	temp -> occurences = 1;
}

/*
* Converts an input word to a form usable by the program, so lowercase and omitting punctuation at the end
*/
void cleanWord(char inWord[WORD_LENGTH])
{
	for(int i = 0; i < WORD_LENGTH; i++)
	{
		inWord[i] = tolower(inWord[i]);
		if(inWord[i] == '\0' && i - 1 >= 0 && ispunct(inWord[i - 1]))
		{
			inWord[i] = ' ';
			inWord[i - 1] = '\0';
			break;
		}
	}
}

/*
* Adds a node to a new List which sorts in alphabetical order by word as nodes are added in
*/
void wordAddNode(Node startNode, Node addNode)
{
	Node temp = startNode;
	while(temp -> next != NULL && strcmp((temp -> next) -> word, addNode -> word) < 0)
	{
		temp = temp -> next;
	}
	//printf("\n%s\n", temp -> word);
	Node placeholder = temp -> next;
	temp -> next = addNode;
	addNode -> next = placeholder;
}

/*
* Similar to the previous method, however instead sorts the new List by the occurences variable as values are added
*/
void numAddNode(Node startNode, Node addNode)
{
	Node temp = startNode;
	while(temp -> next != NULL && (temp -> next) -> occurences < addNode -> occurences)
	{
		temp = temp -> next;
	}
	//printf("\n%s\n", temp -> word);
	Node placeholder = temp -> next;
	temp -> next = addNode;
	addNode -> next = placeholder;
}

/*
* Prints out all 5 tables to the console
*/
void printTables()
{
	//TABLE 1A
	//Table A prints the number of occurences of each letter of the alphabet

	printf("\nTable 1a:\n");
	printf("----------------\n");

	for(int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("| %c | %8d |\n", i + 65, letterOccurences[i]);
	}

	printf("----------------\n");

	//TABLE 1B
	//Table B prints the number of occurences of each word length, capping the table at the highest length

	//Find highest nonzero amount
	int cap = WORD_LENGTH - 1;

	while(cap - 1 >= 0 && lengthOccurences[cap - 1] == 0)
	{
		cap--;
	}

	printf("\nTable 1b:\n");
	printf("-----------------\n");

	for(int i = 0; i < cap; i++)
	{
		printf("| %2d | %8d |\n", i + 1, lengthOccurences[i]);
	}

	printf("-----------------\n");

	//TABLE 1C
	//Table C prints the number of occurences of each word sorted by order of appearence

	printf("\nTable 1c:\n");

	Node cur;
	cur = head;

	Node newHead = makeNode();
	Node newHead2 = makeNode();

	printf("-----------------------------------\n");

	while(cur->next != NULL)
	{
		cur = cur->next;
		printf("| %20s | %8d |\n", cur->word, cur->occurences);

		Node tempNode = makeNode();
		Node tempNode2 = makeNode();

		for(int i = 0; i < WORD_LENGTH; i++)
			tempNode -> word[i] = cur -> word[i];
		tempNode -> occurences = cur -> occurences;
		wordAddNode(newHead, tempNode);

		for(int i = 0; i < WORD_LENGTH; i++)
			tempNode2 -> word[i] = cur -> word[i];
		tempNode2 -> occurences = cur -> occurences;
		numAddNode(newHead2, tempNode2);
	}

	printf("-----------------------------------\n");

	//TABLE 1D
	//Table D prints the number of occurences of each word sorted in alphabetical order

	printf("\nTable 1d:\n");

	printf("-----------------------------------\n");

	cur = newHead;

	while(cur->next != NULL)
	{
		cur = cur->next;
		printf("| %20s | %8d |\n", cur->word, cur->occurences);
	}

	printf("-----------------------------------\n");

	//TABLE 1E
	//Table E prints the number of occurences of each word sorted by number of occurences

	printf("\nTable 1e:\n");

	printf("-----------------------------------\n");

	cur = newHead2;

	while(cur->next != NULL)
	{
		cur = cur->next;
		printf("| %20s | %8d |\n", cur->word, cur->occurences);
	}

	printf("-----------------------------------\n");
}

/*
* Output the tables to the given output file
*/
void outputTables()
{
	//TABLE 1A
	//Table A prints the number of occurences of each letter of the alphabet

	fprintf(outPtr, "\nTable 1a:\n");
	fprintf(outPtr, "----------------\n");

	for(int i = 0; i < ALPHABET_SIZE; i++)
	{
		fprintf(outPtr, "| %c | %8d |\n", i + 65, letterOccurences[i]);
	}

	fprintf(outPtr, "----------------\n");

	//TABLE 1B
	//Table B prints the number of occurences of each word length, capping the table at the highest length

	//Find highest nonzero amount
	int cap = WORD_LENGTH - 1;

	while(cap - 1 >= 0 && lengthOccurences[cap - 1] == 0)
	{
		cap--;
	}

	fprintf(outPtr, "\nTable 1b:\n");
	fprintf(outPtr, "-----------------\n");

	for(int i = 0; i < cap; i++)
	{
		fprintf(outPtr, "| %2d | %8d |\n", i + 1, lengthOccurences[i]);
	}

	fprintf(outPtr, "-----------------\n");

	//TABLE 1C
	//Table C prints the number of occurences of each word sorted by order of appearence

	fprintf(outPtr, "\nTable 1c:\n");

	Node cur;
	cur = head;

	Node newHead = makeNode();
	Node newHead2 = makeNode();

	fprintf(outPtr, "-----------------------------------\n");

	while(cur->next != NULL)
	{
		cur = cur->next;
		fprintf(outPtr, "| %20s | %8d |\n", cur->word, cur->occurences);

		Node tempNode = makeNode();
		Node tempNode2 = makeNode();

		for(int i = 0; i < WORD_LENGTH; i++)
			tempNode -> word[i] = cur -> word[i];
		tempNode -> occurences = cur -> occurences;
		wordAddNode(newHead, tempNode);

		for(int i = 0; i < WORD_LENGTH; i++)
			tempNode2 -> word[i] = cur -> word[i];
		tempNode2 -> occurences = cur -> occurences;
		numAddNode(newHead2, tempNode2);
	}

	fprintf(outPtr, "-----------------------------------\n");

	//TABLE 1D
	//Table D prints the number of occurences of each word sorted in alphabetical order

	fprintf(outPtr, "\nTable 1d:\n");

	fprintf(outPtr, "-----------------------------------\n");

	cur = newHead;

	while(cur->next != NULL)
	{
		cur = cur->next;
		fprintf(outPtr, "| %20s | %8d |\n", cur->word, cur->occurences);
	}

	fprintf(outPtr, "-----------------------------------\n");

	//TABLE 1E
	//Table E prints the number of occurences of each word sorted by number of occurences

	fprintf(outPtr, "\nTable 1e:\n");

	fprintf(outPtr, "-----------------------------------\n");

	cur = newHead2;

	while(cur->next != NULL)
	{
		cur = cur->next;
		fprintf(outPtr, "| %20s | %8d |\n", cur->word, cur->occurences);
	}

	fprintf(outPtr, "-----------------------------------\n");

	printf("Successfully printed to file");
}
//End of Program