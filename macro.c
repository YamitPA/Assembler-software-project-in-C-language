#include "heder.h"

/*This file contains functions that handle the implementation of the macro.*/




/*A function that checks whether a macro name exists in our macro list.*/
nodeMacro* find_the_node_macro(macroList *ourMacroList, char *nameNode)
{
	nodeMacro *current;

	current = ourMacroList->head;

	/*Let's start at the top of the list.*/
	while (current != NULL)
	{
		/*If we found the name of the macro we will return the node.*/
        	if (strcmp(current->name, nameNode) == 0)
		{
            		return current;
        	}

        current = current->next;
    	}

	return NULL; 
}



/*A function to initialize a macro node.*/
void initNodeMacro(nodeMacro *newNode)
{
    newNode->linesNum = 0;
    newNode->lines = NULL;
    newNode->next = NULL;
}



/*A function to add a new node to the linked list containing the macros. This function creates a new `nodeMacro` structure, initializes it with the given name, and appends it to the end of the specified `macroList`.*/
void addNodeToMacroList(macroList *ourMacroList, char *name)
{
	nodeMacro *newNode;
	nodeMacro *current;

	/*Verification of details.*/
	if (ourMacroList == NULL || name == NULL)
	{
        	printf("Invalid parameters.\n");
        	return;
    	}

	/*Allocate memory for a new node*/
	newNode = malloc(sizeof(nodeMacro));

    	if (newNode == NULL)
	{
        	printf("Error: Memory allocation for newNode failed .\n");
        	return;
    	}

	/*Set the node's name and initialize its fields.*/
	strncpy(newNode->name, name, MAX_MACRO_NAME_LENGTH - 1);

    	newNode->name[MAX_MACRO_NAME_LENGTH - 1] = '\0';
	
	initNodeMacro(newNode);

	/*Insert the node into the linked list.*/
	if (ourMacroList->head == NULL)
	{
		/*List is empty, make the new node the head.*/
		ourMacroList->head = newNode;
    	}	

	else
	{
		/*Traverse to the end of the list and append the new node.*/
		current = ourMacroList->head;

        	while (current->next != NULL)
        	{
           		current = current->next;
        	}

        	current->next = newNode;
   	}
}




/*A function to add a line from the file to the macro. This function takes a pointer to a `nodeMacro` structure and a pointer to a character string representing a line from a file. It allocates memory to store the line in the `lines` array of the node and updates the `linesNum` field to reflect the addition.*/
void addLineToNodeMacro(nodeMacro *currentNode, char *line)
{
	int i;
	char **temp;


	/*Reallocate memory for lines array to accommodate the new line.*/
	temp = realloc(currentNode->lines, (currentNode->linesNum + 1) * sizeof(char *));

	if (temp == NULL)
	{
        	printf("Error: Memory allocation for currentNode->lines failed.\n");

		/*Free previously allocated memory.*/
    		for (i = 0; i < currentNode->linesNum; ++i)
		{
        		free(currentNode->lines[i]);
    		}

		free(currentNode->lines);
        	exit(1);
    	}

	currentNode->lines = temp;

	/*Allocate memory for the new line.*/
	currentNode->lines[currentNode->linesNum] = malloc(strlen(line) + 1);

    	if(currentNode->lines[currentNode->linesNum] == NULL)
	{
        	printf("Error: Memory allocation for currentNode->lines[%d] failed.\n", currentNode->linesNum);

		/*Free previously allocated memory.*/
    		for (i = 0; i < currentNode->linesNum; ++i)
		{
        		free(currentNode->lines[i]);
		}

		free(currentNode->lines);
        	exit(1);
    	}

	/*Copy the line content to the allocated memory.*/
	strcpy(currentNode->lines[currentNode->linesNum], line);

	/*Increment the line count.*/
    	(currentNode->linesNum)++;
}


