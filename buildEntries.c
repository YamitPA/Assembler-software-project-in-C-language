#include "heder.h"

void build_entries(dataSymbols *arraySymbols, char *fileName)
{
	int isEntry = 0;
	FILE *entryFile;
	char *nameFileEnt;
	char *withZeros;
	nodeDataSymbols *current = arraySymbols->head;

	/*Check if there is an entry type icon.*/
	if(is_entry_symbol_present(arraySymbols) == 0)
	{
		/*There is an entry type symbol so change the flag to 1.*/
		isEntry = 1;
	}

	/*If an entry type icon is present.*/
	if(isEntry == 1)
	{
		nameFileEnt = malloc(FILENAME_MAX * sizeof(char));
		
		if(allocation_succeeded(nameFileEnt) == 1)
		{
			return;
		}

		/*We will copy the name of the file received from the user to our variable so as not to make 			changes to it.*/
		strcpy(nameFileEnt, fileName);

		/*The name of the file we received is with the .am extension, so we will remove the extension.*/
		nameFileEnt[strlen(fileName) - SUFFIX_LENGTH] = '\0';

		/*Add an extension to the file name.*/
		strcat(nameFileEnt, ".ent");

		entryFile = fopen(nameFileEnt, "w");

		if(entryFile == NULL)
		{	
			printf("Cannot open file %s.ent.\n\n", fileName);
			free(nameFileEnt);
			return;
		}

		/*Iterate through the linked list.*/
		while (current != NULL)
		{ 
    			/*If the symbol is of type entry.*/
    			if (current->type == entry)
			{
        			/*Print the name of the entry type symbol.*/
        			fprintf(entryFile, "%s ", current->name);

				withZeros = add_leading_zeros(current->dataAddr, arraySymbols);

        			/*Print the entry's value.*/
        			fprintf(entryFile, "%s\n", withZeros);

				free(withZeros);
   			}

    			/* Move to the next node in the linked list. */
    			current = current->next;

		}/*End of the transition on the symbol table.*/
	}

	fclose(entryFile);
	free(nameFileEnt);
}
