#include "heder.h"

/*This file handles everything related to creating a pre-assembler file.*/



/*A function that is responsible for building the pre-assemble file with the replacement of macros and checking the correctness of line length.*/
FILE* make_pre_assembler_file(FILE *ifp, char *currentFileName)
{
	char line[ACTUAL_LEN_LINE];
	char *macroName = NULL;
	char *lineWithoutBlanks = NULL;
	char *currentLine = NULL;
	char *fileName;
	FILE *ofp = NULL;
	int hasMacroFlag = 0;
	int rowNumber = 1;
	int i = 0;
	int errorFlag = 0;
	nodeMacro *theNodeMacro;


	/*A call to a function that returns the name I want the file to have.*/
	fileName = builde_new_file_name(currentFileName, ".am");

	if (fileName == NULL)
	{
        	return NULL;
	}

	/*Opening the new file to open.*/
	ofp = fopen(fileName, "w");

	if(ofp == NULL)
	{	
		remove_file(fileName);
		free (fileName);
		return NULL;
	}


	/*Pick up the lines from the user's document.*/
	while(fgets(line, ACTUAL_LEN_LINE , ifp) != NULL)
	{
		/*Checking if the length of the line is correct, if not, the function will print en error.*/
		if(is_line_size_correct(line, rowNumber) == 1)
		{
				errorFlag = 1;
		}
		
		lineWithoutBlanks = malloc(ACTUAL_LEN_LINE * sizeof(char));

		if(lineWithoutBlanks == NULL)
		{
			error_in_line(rowNumber);
			printf("Failed to allocate memory for lineWithoutBlanks.\n");
			fclose(ofp);
			remove_file(fileName);
			free(lineWithoutBlanks);
			return NULL;
		}

		/*We will copy the original line to a local variable so as not to make changes to the source.*/
		strcpy(lineWithoutBlanks, line);

		/*Removing white characters from the line.*/
		remove_blanks(lineWithoutBlanks);	

		/*We are inside a macro definition*/
		if (hasMacroFlag == 1)
		{
            		if (strncmp(lineWithoutBlanks, "endmcr", CHAR_AT_ENDMCR) == 0)
			{
                		/*End of macro definition.*/
                		hasMacroFlag = 0;
            		}
			else
			{
                		/*Add the line to the macro definition.*/
                		addLineToNodeMacro(find_the_node_macro(&ourMacroList, macroName), line);
            		}
		}

		/*We are not inside a macro definition.*/
		else 
		{
			/*If the line is a macro definition.*/
            		if (strncmp(lineWithoutBlanks, "mcr", CHAR_AT_MCR) == 0) 
			{
				/*We will send the sentence to a function that will check if cmd is valid.*/
				if(legal_mcr_name(line) == 1)
				{
					error_in_line(rowNumber);
					printf("Invalid mcr.\n");
					errorFlag = 1;
				}

				macroName = malloc(MAX_MACRO_NAME_LENGTH * sizeof(char));

				if(macroName == NULL)
				{
					printf("Failed to allocate memory for macroName.\n");
					fclose(ofp);
					remove_file(fileName);
					free(fileName);
					free(lineWithoutBlanks);
					return NULL;
				}

                		/*Start of a new macro definition.*/
                		strncpy(macroName, lineWithoutBlanks + CHAR_AT_MCR,  MAX_MACRO_NAME_LENGTH);

				macroName[MAX_MACRO_NAME_LENGTH - 1] = '\0';
	
				/*We will check if the macro name exists in our list of macros. If so we will get 					the node for the macro and if not, we will get NULL.*/
				theNodeMacro = find_the_node_macro(&ourMacroList, macroName);

				/*Check if the macro is in the table.*/
                		if (theNodeMacro != NULL)
				{
					error_in_line(rowNumber);
					printf("Two identical macro names - %s.\n", macroName);
					errorFlag = 1;
				}

				/*There is no macro in the table with this name.*/
				else 
				{
					addNodeToMacroList(&ourMacroList, macroName);

					hasMacroFlag = 1;
				}

			}/*End of 'if' the line is a macro definition.*/

			/*If the current line is macro readable.*/
			else if((theNodeMacro = find_the_node_macro(&ourMacroList, lineWithoutBlanks)) != NULL)
			{
				/*Delete the current line.*/
				line[0] = '\0';

				/*We will copy all the lines of mcr to the file.*/
                    		while(i < theNodeMacro->linesNum)
				{	
					fprintf(ofp,"%s", theNodeMacro->lines[i]);
					i++;
				}
				
				/*Reset i for the next macro call.*/
				i = 0;

			}/*End of 'if' line is a macro readable.*/

			/*If the line is not a macro definition and we are not inside a macro. This is a normal 			line.*/
			else 
			{
				/*Allocate memory for currentLine.*/
				currentLine = malloc(strlen(line) + 1);

				if (currentLine == NULL)
            			{
                			printf("Failed to allocate memory for currentLine.\n");
					fclose(ofp);
					remove_file(fileName);
					free(macroName);
					free(fileName);
					free(lineWithoutBlanks);
					return NULL;
            			}

				/*Copy line to currentLine.*/
            			strcpy(currentLine, line);

				/*Print currentLine to the output file.*/
				fprintf(ofp, "%s", currentLine);	

				/*Free memory allocated for currentLine.*/
				free(currentLine);
			}

		}/*End 'else' not inside a macro definition.*/

		rowNumber++;
		free(lineWithoutBlanks);

		
	}/*End while loop.*/

	if(errorFlag == 1)
	{
		printf("Errors were found when retiring the macros.\n");
		fclose(ofp);
		free(macroName);
		remove_file(fileName);
		free(fileName);
		return NULL;
	}

	free(macroName);
	free(fileName);
	fclose(ofp);
	return ofp;
}/*End make_pre_assembler_file*/





/*The function deletes a file and outputs a print message whether the deletion was successful or not.*/
void remove_file(char *filename)
{
	int status;

	status = remove(filename);

    	if (status == 0)
	{
        	printf("The file '%s' deleted successfully.\n", filename);
   	}

	else
	{
        	printf("Error: The file '%s' cannot be deleted.\n", filename);
    	}
	
}




