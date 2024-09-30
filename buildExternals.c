#include "heder.h"



/*A function that checks whether a label exists in extern list.*/
nodeExtern* find_the_nodeExtern(externList *ourExternList, char *nameNode)
{
	nodeExtern *current;

	current = ourExternList->head;

	/*Start at the top of the list.*/
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



/*A function that checks whether a string is already in the extern list. If so it returns 1 if not it returns 0.*/
int is_in_externList(char *name, externList *ourExternList)
{
	/*If there is a ':' as in the label we will remove it and then search if it is a saved word.*/
	if (name[strlen(name) - 1] == ':')
	{
        	name[strlen(name) - 1] = '\0'; 
	}

	/*If the label does not exist in the arraySymbols return 0.*/
	if(find_the_nodeExtern(ourExternList, name) == NULL)
	{
		return 0;
	}

	/*We received an address of a node, so it already exists in ourExternList.*/
	return 1;
}



/*A function to add a new node to the linked list containing the external labels. This function creates a new `nodeDataSymbols` structure, initializes it with the given name, and appends it to the end of arraySymbols. If the addition was successful the function will return 0 otherwise it will return 1.*/
int add_node_to_externList(char *name, int address, externList *ourExternList)
{
	nodeExtern *newNode;
	nodeExtern *current;

	/*Verification of details.*/
	if (ourExternList == NULL || name == NULL)
	{
        	printf("Invalid parameters.\n");
        	return 1;
    	}

	/*Allocate memory for a new node*/
	newNode = malloc(sizeof(nodeDataSymbols));

    	if (newNode == NULL)
	{
        	printf("Error: Memory allocation for newNode failed .\n");
        	return 1;
    	}

	/*Set the node's name and initialize its fields.*/
	strncpy(newNode->name, name, LABEL_LENGTH - 1);

    	newNode->name[LABEL_LENGTH - 1] = '\0';

	newNode->address = address;

	newNode->next = NULL;

	/*Insert the node into the linked list.*/
	if (ourExternList->head == NULL)
	{
		/*List is empty, make the new node the head.*/
		ourExternList->head = newNode;
    	}	

	else
	{
		/*Traverse to the end of the list and append the new node.*/
		current = ourExternList->head;

        	while (current->next != NULL)
        	{
           		current = current->next;
        	}

        	current->next = newNode;
   	}

	return 0;
}



/*A function that checks whether a label is extern type and if so it adds it to the extern list.*/
void enter_extern(char *name, int address, externList *ourExternList, dataSymbols *arraySymbols, int rowNumber)
{
	char *word;

	

	/*If it is a fixed index, the label should be extracted.*/
	if(is_fixed_index(name, rowNumber) == 0)
	{
		word = malloc(strlen(name) * sizeof(char));

		if (word == NULL)
		{
			printf("Error: allocating space for 'word' failure.\n");
			return;
		}

		/*Extract the array and the number from the operand.*/
		 extract_fixed_index(name, word, arraySymbols);

		if(get_symbol_type(word, arraySymbols) == Extern)
		{
			add_node_to_externList(word, address, ourExternList);
		}

		free(word);
	}


	if(get_symbol_type(name, arraySymbols) == Extern)
	{
		add_node_to_externList(name, address, ourExternList);
	}

}



/*A function that builds the external file if there are labels of type external.*/
void build_externals(char *fileName, FILE *ofp, dataSymbols *arraySymbols, externList *ourExternList)
{
	int isExtern = 0;
	char *nameFileExt;
	char *withZeros;
	FILE *externFile;
	nodeExtern *current;
	

	/*Check if there is an Extern type icon.*/
	if(is_extern_symbol_present(arraySymbols) == 0)
	{
		/*There is an Extern type symbol so change the flag to 1.*/
		isExtern = 1;
	}

	/*If an Extern type icon is present.*/
	if(isExtern == 1)
	{
		nameFileExt = malloc(FILENAME_MAX * sizeof(char));
		
		if(allocation_succeeded(nameFileExt) == 1)
		{
			free_externList(ourExternList);
			return;
		}

		/*We will copy the name of the file received from the user to our variable so as not to make 			changes to it.*/
		strcpy(nameFileExt, fileName);

		/*The name of the file we received is with the .am extension, so we will remove the extension.*/
		nameFileExt[strlen(fileName) - SUFFIX_LENGTH] = '\0';

		/*Add an extension to the file name.*/
		strcat(nameFileExt, ".ext");

		externFile = fopen(nameFileExt, "w");

		if(externFile == NULL)
		{	
			printf("Cannot open file %s.ext.\n\n", fileName);
			free_externList(ourExternList);
			free(nameFileExt);
			return;
		}

		/*Traverse to the end of the list and append the new node.*/
		current = ourExternList->head;

		/*Iterate through the linked list.*/
		while (current != NULL)
		{ 
			/*Remove any leading or trailing blanks from the symbol name.*/
    			remove_blanks(current->name);
    			
                	/*Print the label name.*/
                	fprintf(externFile, "%s ", current->name);

			withZeros = add_leading_zeros(current->address + MEMORY_START, arraySymbols);

                	/*Print the label name.*/
                	fprintf(externFile, "%s\n", withZeros);

			free(withZeros);

    			/*Move to the next node in the linked list.*/
    			current = current->next;
		} 

	}/*End of if an Extern type icon is present.*/

	fclose(externFile);
	free(nameFileExt);
	free_externList(ourExternList);
}




/*A function that accepts arraySymbols and frees all its elements.*/
void free_externList(externList *ourExternList)
{
    	/*A temporary pointer to a cell in the list.*/ 
	nodeExtern *current = ourExternList->head;

    	/*A pointer to the cell to be freed.*/
	nodeExtern *temp;


	/*Iterate through the list.*/
	while (current != NULL)
	{
        	/*Store the address of the current node.*/
        	temp = current;

        	/*Move to the next node.*/
        	current = current->next;

        	/*Free the memory of the current node*/
        	free(temp);
    	}

	/*Set the list's head pointer to NULL after all nodes are freed.*/
	ourExternList->head = NULL;

}


