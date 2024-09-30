#include "heder.h"


/*This file contains helper functions for parsing symbols.*/

int sizeArraySymbols = FIRST_ALLOCATE;




/*A function that checks whether a symbol name exists in arraySymbols.*/
nodeDataSymbols* find_the_node_symbol(dataSymbols *arraySymbols, char *nameNode)
{
	nodeDataSymbols *current;

	current = arraySymbols->head;

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



/*A function that checks whether a string is already in the symbol table. If so it returns 1 if not it returns 0.*/
int is_in_symbol_table(char *name, dataSymbols *arraySymbols)
{
	/*If there is a ':' as in the label we will remove it and then search if it is a saved word.*/
	if (name[strlen(name) - 1] == ':')
	{
        	name[strlen(name) - 1] = '\0'; 
	}

	/*If the symbol does not exist in the arraySymbols return 0.*/
	if(find_the_node_symbol(arraySymbols, name) == NULL)
	{
		return 0;
	}

	/*We received an address of a node, so it already exists in the arraySymbols.*/
	return 1;
}



/*A function that receives the name of a symbol and returns the type of the symbol.*/
SymbolType get_symbol_type(char *symbolName, dataSymbols *arraySymbols)
{
	nodeDataSymbols *symbolNode;

	/*Find the node with the appropriate symbol name.*/
	symbolNode = find_the_node_symbol(arraySymbols, symbolName);

	/*The node does not exist.*/
	if(symbolNode == NULL)
	{
		return -1;
	}

	/*Return the type of the symbol.*/
	return symbolNode->type;

}





/*Function to find the data address by symbol name.*/
int find_dataAddr(dataSymbols *arraySymbols, char *name)
{
	nodeDataSymbols *symbolNode;

	/*Find the node with the appropriate symbol name.*/
	symbolNode = find_the_node_symbol(arraySymbols, name);

	/*The node does not exist.*/
	if(symbolNode == NULL)
	{
		printf("The symbol does not exist in the arraySymbols.\n");
		return -1;
	}

	/*Return the type of the symbol.*/
	return symbolNode->dataAddr;
}




/*A function that checks if the string is the same as one of the reserved names. If so it returns 1 otherwise it returns 0.*/
int is_reserved_words(char *s)
{
	int i;
	nodeMacro *macro;

	/*Define all saved names in array.*/
	char *reservedWords[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "hlt", "rts", ".define", ".data", ".string", ".entry", ".extern", "r0", "r1" ,"r2", "r3", "r4", "r5", "r6", "r7"};

	remove_blanks(s);
 

	/*If there is a ':' as in the label we will remove it and then search if it is a saved word.*/
	if (s[strlen(s) - 1] - 1 == ':')
	{
        	s[strlen(s) - 1] = '\0'; 
	}

	for (i = 0; i < NUM_RESERVED_WORDS; i++)
	{
		/*If the string is equal to the saved name we will return 1.*/
		if(strcmp(s, reservedWords[i]) == 0)
		{
			return 1;
		}
	}

	/*Check if there is a macro with the same name in the list of macros.*/
	macro = find_the_node_macro(&ourMacroList, s);

	/*A macro by that name exists, so return 1.*/
	if(macro != NULL)
	{
		return 1;
	}

	return 0;
}



/*A function that checks if a string is an equal sign. If so, it returns 0, else it returns 1.*/
int is_equal_sign(char *s)
{
	if(strcmp(s, "=") == 0)
	{
		return 0;
	}

	else
	{
		return 1;
	}
}




/*The function checks whether the string is a number. If so its returns 0, else its returns 1.*/
int is_number(char *s)
{
	char *endptr;

	if (s == NULL || *s == '\0')
	{
		/*The string is not a number.*/
        	return 1; 
    	}

	/*Attempt to convert to double (floating-point)*/
	strtol(s, &endptr, 10);

	if (*endptr != '\0')
	{
		/*The string is not a number.*/
		return 1;
	}

	/*The string is a number.*/
	return 0;

}



/*A function that converts a string to a number.*/
int s_to_num(char *s)
{
	return atoi(s);	
}



/*A function to add a new node to the linked list containing the symbols. This function creates a new `nodeDataSymbols` structure, initializes it with the given name, and appends it to the end of arraySymbols. If the addition was successful the function will return 0 otherwise it will return 1.*/
int add_node_to_arraySymbols(char *name, int type, int dataAddr, dataSymbols *arraySymbols)
{
	nodeDataSymbols *newNode;
	nodeDataSymbols *current;
	nodeDataSymbols *existingNode;

	/*Verification of details.*/
	if (arraySymbols == NULL || name == NULL)
	{
        	printf("Invalid parameters.\n");
        	return 1;
    	}

	/*If the symbol has already been entered into the symbol table.*/
	if(is_in_symbol_table(name, arraySymbols) == 1)
	{
		/*If there is no definition for the dataAddr of the symbol.*/
		if(find_dataAddr(arraySymbols, name) == '\0')
		{
			/*Find the node with the requested name.*/
			existingNode = find_the_node_symbol(arraySymbols, name);

			/*Enter its relevant value.*/
			existingNode->type = type;

			/*Enter his dataAddr.*/
			existingNode->dataAddr = dataAddr;
		}

		return 0;
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

	newNode->type = type;

	newNode->dataAddr = dataAddr;

	newNode->next = NULL;

	/*Insert the node into the linked list.*/
	if (arraySymbols->head == NULL)
	{
		/*List is empty, make the new node the head.*/
		arraySymbols->head = newNode;
    	}	

	else
	{
		/*Traverse to the end of the list and append the new node.*/
		current = arraySymbols->head;

        	while (current->next != NULL)
        	{
           		current = current->next;
        	}

        	current->next = newNode;
   	}

	return 0;
}




/*A function that accepts the name of a symbol and changes its type.*/
void change_symbol_type(char *symbolName, SymbolType newType, dataSymbols *arraySymbols)
{
	nodeDataSymbols *symbolNode;

	/*Find the node with the appropriate symbol name.*/
	symbolNode = find_the_node_symbol(arraySymbols, symbolName);

	/*The node does not exist.*/
	if(symbolNode == NULL)
	{
		printf("The symbol does not exist in the arraySymbols.\n");
		return;
	}

	/*Change the value of the type to the desired value.*/
	symbolNode->type = newType;
}





/*Function to check if there is an extern symbol in the linked list. If there is extern symbol it returns 0. Otherwise it will returns 1.*/
int is_extern_symbol_present(dataSymbols *arraySymbols)
{
	nodeDataSymbols *current; 

	current = arraySymbols->head;

	/*Traverse the linked list.*/
	while (current != NULL)
	{
		if (current->type == Extern)
		{
			/*Extern symbol found.*/
            		return 0; 
        	}
		
        	current = current->next;
    	}

	/*Not Extern symbol not found.*/
    	return 1; 
}





/*Function to check if there is an entry symbol in the linked list. If there is entry symbol it returns 0. Otherwise it will returns 1.*/
int is_entry_symbol_present(dataSymbols *arraySymbols)
{
	nodeDataSymbols *current; 

	current = arraySymbols->head;

	/*Traverse the linked list.*/
	while (current != NULL)
	{
		if (current->type == entry)
		{
			/*entry symbol found.*/
            		return 0; 
        	}
		
        	current = current->next;
    	}

	/*Not entry symbol not found.*/
    	return 1; 
}



/*A function that accepts arraySymbols and frees all its elements.*/
void free_arraySymbols(dataSymbols *arraySymbols)
{
    	/*A temporary pointer to a cell in the list.*/ 
	nodeDataSymbols *current = arraySymbols->head;

    	/*A pointer to the cell to be freed.*/
	nodeDataSymbols *temp;


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
	arraySymbols->head = NULL;

}

