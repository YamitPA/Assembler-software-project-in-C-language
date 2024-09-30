#include "heder.h"


int elementsInArray = 0;


/*The function receives a line read from the file. It returns an array of arrays that contains in each cell a word. The purpose of this function is to help the other functions access and parse each word on the line more easily.*/
char** lineToArray(char *line)
{
	char **theCommand = NULL;
	char *pointer = line;
	char *copyOfLine;
	char *wordStart;
	size_t wordLength;
	int cell = 0;

	/*Update the cell counter to 0.*/
	elementsInArray = 0;

	/*Returning NULL if the line is empty.*/
	if (strlen(line) == 0)
	{
        	return NULL;
    	}

	/*We will copy the original sentence so as not to make changes to it.*/
	copyOfLine = malloc(strlen(line) + 1);
	
	if (copyOfLine == NULL)
        {
		printf("No space for copyOfLine.\n");
		return NULL;
	}

	strcpy(copyOfLine, line);

	/*Looping through the line.*/
	while(*pointer != '\0')
	{
		/*If we read a white character we will move on to the next character.*/
		if(isspace(*pointer) || *pointer == ',' || *pointer == '\t')
		{
			pointer++;
		}

		/*If we encounter "=", we add it as a separate word.*/
		else if (*pointer == '=')
        	{
			/*Setting the start of the word to the current position.*/
           		wordStart = pointer;

			/*Length of the word is one character.*/
            		wordLength = 1;

            		theCommand = realloc(theCommand, (cell + 1) * sizeof(char *));

            		if (theCommand == NULL)
            		{
                		printf("No space for theCommand.\n");
                		free_array(theCommand);
                		free(copyOfLine);
                		return NULL;
            		}

			/*Allocating memory for the new cell.*/
            		theCommand[cell] = malloc(2 * sizeof(char));

            		if(theCommand[cell] == NULL)
            		{
                		printf("Failed to allocate memory for theCommand[%d].\n", cell);
                		free_array(theCommand);
                		free(copyOfLine);
                		return NULL;
            		}

            		strncpy(theCommand[cell], wordStart, wordLength);

            		theCommand[cell][wordLength] = '\0';

            		elementsInArray++;
            		cell++;
            		pointer++;
        	}

		/*If we read a character that is not white.*/
		else
		{
			/*Setting the start of the word to the current position.*/
			wordStart = pointer;

			/*Advance the pointer as long as no white character, tab, comma, or equal sign is read.*/
			while (!isspace(*pointer) && *pointer != ',' && *pointer != '\t' && *pointer != '=' && 				*pointer != '\0')
			{
				pointer++;
			}

			/*Calculating the length of the current word.*/
			wordLength = pointer - wordStart;

			/*Skip words consisting only of whitespace characters.*/
    			if (wordLength > 0)
			{
				/*We will allocate space for the array we want to return.*/
				theCommand = realloc(theCommand, (cell + 1) * sizeof(char *));

				if (theCommand == NULL)
        			{
					printf("No space for theCommand.\n");

                			free_array(theCommand);
					free(copyOfLine);

					return NULL;
				}

				theCommand[cell] = malloc((wordLength + 1) * sizeof(char));

				if (theCommand[cell] == NULL)
            			{
                			printf("Failed to allocate memory for theCommand[%d].\n", cell);

                			free_array(theCommand);
					free(copyOfLine);

                			return NULL;
            			}

				/*Copy the word into the new cell.*/
				strncpy(theCommand[cell], wordStart, wordLength);

				/*Add NULL to mark the end of the string.*/
				theCommand[cell][wordLength] = '\0';

				/*Update how many cells are in the array.*/
				elementsInArray++;

				cell++;
			}
	
		}/*End of if we read a character that is not white.*/

	}/*We went over the whole line*/

	free(copyOfLine);

	return theCommand;

}/*End lineToArray*/






/*A function that prints on which line the error occurred.*/
void error_in_line(int rownum)
{
	printf("Error: in line %d\n",rownum);
}



/*A function that checks whether a sentence is an empty line. If it is an empty line, it will returns 0, else it returns 1.*/
int is_empty_sentence(char *line)
{
	while (*line != '\0')
	{
		/*The character is not white.*/
        	if (!isspace(*line))
		{
            		return 1; 
        	}

        	line++;
    	}

	/*The line is all white characters.*/
	return 0;
}



/*The function checks whether the sentence starts with ';'. If so, the function will return 0 otherwise it will return 1.*/
int starts_with_semicolon(char *line, dataSymbols *arraySymbols, char **arrayLine)
{
	char *copeLine = malloc(strlen(line) + 1);

	if (copeLine == NULL)
	{
        	printf("Memory allocation failed\n");
		free_arraySymbols(arraySymbols);
		free_array(arrayLine);
        	exit(1);
    	}

	strcpy(copeLine, line);

	remove_blanks(copeLine);

    	/*Checking whether the first character in the sentence is ';'.*/
	if (copeLine[0] == ';')
	{
		free(copeLine);
		/*The sentence begins with a semicolon.*/
        	return 0; 
    	}

	else
	{
		free(copeLine);
		/*The sentence does not start with a semicolon.*/
        	return 1; 
    	}
}



/*A function that returns at which index the word directive is found.*/
int index_directive (char *line)
{
	char **tempLine;
	int i;

	/*An array that contains the directive commands.*/
	char *directiveSen[] = {".data", ".string", ".entry", ".extern"};

	tempLine = lineToArray(line);

	if(tempLine == NULL)
	{
		return -1;
	}

	for (i = 0; i < NUM_DIRECTIVE_COMMANS; i++)
	{
		if(strcmp(tempLine[ZERO],directiveSen[i]) == 0)
		{
			free_array(tempLine);
			/*Return the index let the command be found.*/
			return ZERO;
		}

		if(strcmp(tempLine[ONE],directiveSen[i]) == 0)
		{
			free_array(tempLine);
			/*Return the index let the command be found.*/
			return ONE;
		}
	}

	/*This is not a directive sentence.*/
	return -1;
}


/*A function that checks whether it is a '.data' directive. If so it will return 0 if it's an unlabeled directive, 1 if it's a labeled directive and -1 if it's not a data directive.*/
int is_data(char *line)
{
	char **tempLine;
	int isData = -1;
	int elements = 0;

	tempLine = lineToArray(line);

	elements = count_elements(tempLine);

	if(tempLine != NULL)
	{
		/*Unlabeled '.data' directive.*/
		if(strcmp(tempLine[ZERO], ".data") == 0)
		{
			isData = 0;
		}

		/*Labeled '.data' directive, make sure that there is at all an element in index 1.*/
		else if(elements >= 2 && strcmp(tempLine[ONE], ".data") == 0)
		{
			isData = 1;
		}

		free_array(tempLine);
	}

	 return isData;

}





/*A function that checks whether it is a '.entry' directive. If so it will return 0 if it's an unlabeled directive, 1 if it's a labeled directive and -1 if it's not a data directive.*/
int is_entry(char *line)
{
	char **tempLine;
	int entryType = -1;
	int elements = 0;

	tempLine = lineToArray(line);

	elements = count_elements(tempLine);

	if(tempLine != NULL)
    	{

		/*Unlabeled '.entry' directive.*/
		if(strcmp(tempLine[ZERO], ".entry") == 0)
		{
			entryType = 0;
		}

		/*Labeled '.entry' directive, We will make sure that there is at all an element in index 1.*/
		if(elements >= 2 && strcmp(tempLine[ONE], ".entry") == 0)
		{
			entryType = 1;
		}

		free_array(tempLine);
	}

	return entryType;

}






/*A function that checks whether it is a '.extern' directive. If so it will return 0 if it's an unlabeled directive, 1 if it's a labeled directive and -1 if it's not a data directive.*/
int is_extern(char *line)
{
	char **tempLine;
	int elements = 0;

	tempLine = lineToArray(line);

	elements = count_elements(tempLine);

	if(tempLine == NULL)
	{
		return -1;
	}

	/*Unlabeled '.extern' directive.*/
	if(strcmp(tempLine[ZERO], ".extern") == 0)
	{
		free_array(tempLine);
		return 0;
	}

	/*Labeled '.extern' directive, We will make sure that there is at all an element in index 1.*/
	if(elements >= 2 && strcmp(tempLine[ONE], ".extern") == 0)
	{
		free_array(tempLine);
		return 1;
	}


	/*Not a '.extern' directive.*/
	free_array(tempLine);
	return -1;

}



/*A function that checks whether it is a '.string' directive. If so it will return 0 if it's an unlabeled directive, 1 if it's a labeled directive and -1 if it's not a data directive.*/
int is_string(char *line)
{
	char **tempLine;
	int isString = -1;
	int elements = 0;


	tempLine = lineToArray(line);

	elements = count_elements(tempLine);

	if(tempLine != NULL)
	{
		/*Unlabeled '.string' directive.*/
		if(strcmp(tempLine[ZERO], ".string") == 0)
		{
			isString = 0;
		}

		/*Labeled '.string' directive, make sure that there is at all an element in index 1.*/
		if(elements >= 2 && strcmp(tempLine[ONE], ".string") == 0)
		{
			isString = 1;
		}

		/*Freeing memory allocated for tempLine.*/
		free_array(tempLine);
	}

	return isString;

}



/*A function that checks whether a sentence is a instruction sentence. If so it returns 0, otherwise it returns 1.*/
int is_instruction_sentence(char *line)
{
	/*Check using a helper function whether the first word or the second word in the sentence contains a 		operation word.*/
	if((index_instruction(line) == ZERO) || (index_instruction(line) == ONE))
	{
		return 0;
	}
	
	/*The value obtained is -1.*/
	else
	{
		/*This is not a instruction sentence.*/
		return 1;
	}	
}




/*A function that returns at which index the word operation is found.*/
int index_instruction (char *line)
{
	char **tempLine;
	int i;
	int result = -1;
	int elements = 0;

	/*An array that contains the 16 operation names.*/
	char *operName[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "hlt", "rts"};

	tempLine = lineToArray(line);

	elements = count_elements(tempLine);

	if (tempLine == NULL)
	{
		return -1; 
	}

	for (i = 0; i < SIZE_OPERNAME; i++)
	{
		if(strcmp(tempLine[ZERO],operName[i]) == 0)
		{
			/*Result gets the index where we found the action statement.*/
			result = 0;
			break;
		}

		else if(elements >= 2 && strcmp(tempLine[ONE],operName[i]) == 0)
		{
			/*Result gets the index where we found the action statement.*/
			result = 1;
			break;
		}
	}

	/*Freeing memory allocated for tempLine.*/
	free_array(tempLine);

	return result;
}




/*Checking whether the sentence is a define sentence. If it is a define, it will return 0.*/
int is_define_sentence(char *line)
{
	char **tempLine;
	int isDefine = 1;

	tempLine = lineToArray(line);

	if (tempLine != NULL)
	{
		if(strcmp(tempLine[0], ".define") == 0)
		{
			isDefine = 0;
		}

		free_array(tempLine);
	}

	return isDefine;	 
}



/*A function that receives the name of a file and returns a pointer to the name of the file with the desired extension.*/
char* builde_new_file_name(char *string, char *name)
{
	char *cp;
	char *copyString;

	copyString = malloc(FILENAME_MAX * sizeof(char));

	if(copyString == NULL)
	{
		printf("No space for copyString.\n");
	}

	else 
	{
		/*We will copy the file name to a temporary variable.*/
		strcpy(copyString, string);

		/*Find the place where there is a dot in the file name.*/
		cp = strchr(copyString, '.');

		/*Copy the desired extension to the end of the file name.*/
		if (cp != NULL)
        	{
            		strcpy(cp, name);
        	}

		/*If there is no dot, it means that the file name was saved in an illegal way.*/
		else
		{
			printf("Invalid file name.");
		}	
	}

	return copyString;
}


/*The function checks the number of characters in each line. If there are more than 81 including '\0' we will print an error message and return 1. Otherwise we will return 0.*/
int is_line_size_correct(char *line, int rowNumber)
{
	if(strlen(line) > MAX_LEN_LINE)
	{
		error_in_line(rowNumber);

		printf("Error: The line have more than the maximum number (%d) of characters allowed in line.\n" , MAX_LEN_LINE - 1);

		return 1;
	}

	return 0;
}




/*The following function accepts a string and removes white characters (spaces, tabs, new lines, etc.) from the original string.*/
void remove_blanks(char *line)
{
	int i;
	int j;

	j = 0;

	for(i = 0; line[i] != '\0'  ; i++)
	{
		/*If the character is not a white space*/
		if(isspace(line[i]) == 0) 
		{
			/*Take the character in the i place and place it in the j place. If you reached a white 			character, advance only the i.*/
			line[j] = line[i]; 
			j++;
		}
 
	}

	/* Put at the end of the new sentence '\0' */
	line[j] = '\0';

}



/*The function checks whether the line where mcd appears is a line with only 2 words (one should be "mcd" and the other should be the name of mcd). If there are more or less words we will return 1 and if there are 2 words we will return 0.*/
int legal_mcr_name(char *line)
{
	char *pointToChar;
	int count_words;

	/*Set a pointer to the beginning of the word.*/
	pointToChar = line;

	count_words = 0;

	while(*pointToChar != '\0')
	{
		/*If we encountered a space character or an tab, we will go through all the spaces or tabs to 			reach the beginning of the next word. In case we have reached the next word we will add one to 			the word count.*/
		if(*pointToChar == ' ' || *pointToChar == '	')
		{
			while((*pointToChar == ' ' || *pointToChar == '	') && pointToChar != '\0')
			{
				pointToChar++;
			}
			
			count_words++;
		}

		/*in a char and we will continue until reaching space or tab.*/
		else 
		{	
			pointToChar++;
		}
	}

	/*The number of words is different from 2, we will return 1.*/
	if(count_words > NUM_CMD_WORDS_IN_LINE)
	{
		return 1;
	}

	else 
		return 0;
}



/*The function accepts the line without the ".data" at the beginning of the line, and returns 1 if there are no commas and 0 if not. It should be noted that the function returns 0 even if there are too many commas. For this purpose there is another function that checks this condition. Now we are only interested in knowing if a comma is missing.*/
int missing_comma(char *line, int rowNumber)
{
	char *lineCopy;
	int count_commas;
	char *pointToSpace = NULL;

	lineCopy = malloc((strlen(line) + 1) * sizeof(char *));

	if(lineCopy == NULL)
	{
		printf("No space for lineCopy.\n");
		return 1;
	}

	/*In order not to disrupt the line itself, we will perform tests on its copy.*/
	strcpy(lineCopy, line);

	pointToSpace = lineCopy;

	count_commas = 0;

	/*As long as we haven't reached the end of the line.*/
	while(*pointToSpace != '\0')
	{
		if(*pointToSpace != ' ')
		{
			/*As long as we read a character that is not a space or a comma or we have not reached 				the end of the line, we will continue until we receive one of these.*/
			while(*pointToSpace != '\0' && !isspace(*pointToSpace) && *pointToSpace != ',')
			{
				pointToSpace++;
			}

			/*If we have reached a character that is a space...*/
			if(isspace(*pointToSpace))
			{
			
				/*We will continue to read spaces or commas...*/
				while(*pointToSpace != '\0' && (isspace(*pointToSpace) || *pointToSpace == ','))
				{
					pointToSpace++;

					/*Count the commas we reached.*/
					if(*pointToSpace == ',')
					{
						count_commas++;
					}
				}

				/*If we reached a space it means that we have passed one word and therefore 					there should necessarily have been a comma. If the number of commas is still 					equal to zero, it means that a comma is missing.*/
				if(count_commas == 0 && *pointToSpace != '\0')
				{
					error_in_line(rowNumber);
					printf("Missing comma.\n");
					free(lineCopy);
					return 1;
				}

				/*Reset and start the process again to find a word and then to search for a 					comma.*/
				count_commas = 0;
			}

			/*If we have reached a comma we will continue to the next word.*/
			else if(*pointToSpace == ',')
			{
				while(*pointToSpace != '\0' && (isspace(*pointToSpace) || *pointToSpace == ','))
				{
					pointToSpace++;
				}
			}
		}

		if(*pointToSpace != '\0')
		{
			pointToSpace++;
		}

	}

	free(lineCopy);
	return 0;

}




/*A function that checks if there are extra comma characters in the string. An unnecessary comma is a comma that appears at the beginning or at the end of the string or there are two in a row or more consecutive. The function will return 0 if everything is correct in terms of commas and 1 if not.*/
int check_legal_commas(char *line, int rowNumber)
{
	int i;
	char copyLine[MAX_LEN_LINE];
	int errowFlag;

	errowFlag = 0;


	/*In order not to change the source sentence, we will copy it to another variable.*/
	strcpy(copyLine,line);

	/*Remove all spaces from the sentence.*/
	remove_blanks(copyLine);

	/*If there is a comma at the beginning of the sentence, it is invalid and we will return 1.*/
	if(copyLine[ZERO] == ',')
	{
		error_in_line(rowNumber);
		printf("Unnecessary comma at the beginning of defining values.\n");
		errowFlag = 1;
	}

	/*If there are two commas one after the other, it is invalid and we will return 1.*/
	for (i = 0; copyLine[i] != '\0'; i++)
	{
		if (copyLine[i] == ',' && copyLine[i+1] == ',')
		{
			error_in_line(rowNumber);
			printf("Two commas one after the other.\n");
			errowFlag = 1;
		}	
	}

	/*Index i is at the end of the line, if instead of i there is a comma, it is a comma at the end of the line 		and it is invalid.*/
	if (copyLine[i] == ',')
	{
		error_in_line(rowNumber);
		printf("Unnecessary comma at the end of the line.\n");
		errowFlag = 1;
	}

	/*There were errors in commas.*/
	if(errowFlag == 1)
	{
		return 1;
	}

	/*The commas are correct.*/
	else 
	return 0;
}







/*A function that accepts a sentence and checks whether there is a comma between the first two words, if so it returns 1 and otherwise it returns 0.*/
int check_comma_between(char *sentence, int rowNumber)
{
	int i;
	int j = 0;
    

	/*If there is a comma at the beginning of the sentence or a white character, we will move forward until 	we reach a character that is not a comma or a white character, that is, to the beginning of the first 		word.*/
	if(sentence[j] == ',' || isspace(sentence[j]))
	{
		while((isspace(sentence[j]) || sentence[j] == ',') && sentence[j] != '\0')
                {
                    	j++;
                }
	}

	/*Go over the sentence from the first word.*/
	for (i = j; sentence[i] != '\0'; i++) 
        {
		/*If a comma is found, we return 1*/
		if (sentence[i] == ',')
            	{
			error_in_line(rowNumber);
                	return 1;
            	}

		/*If there is a white character.*/
            	else if (isspace(sentence[i]))
            	{
			/*We will move forward in the sentence until we reach a character that is not a white 				character.*/
                	while(isspace(sentence[i]) && sentence[i] != '\0')
                	{
                    		i++;
                	}
            
			/*If a comma is found return 1.*/
                	if(sentence[i] == ',')
               		{
				error_in_line(rowNumber);
                    		return 1;
                	}
            
			/*If it is a character that is not a comma or a white character, it means that there is 			a new word and there was no pick that separated the first word from the second, so 				return 0.*/
                	else
                	{
                    		return 0;
                	}
            	}
        }

	return 0;
}




/*A function that accepts a sentence and returns a copy of it without the first word.*/
/*char* remove_first_word(char *line)
{
	int firstWordLength = 0;
	char *result;
    */

	/*If the sentence is empty we cannot remove one word from it.*/
	/*if(count_words(line) == 0)
	{
		return NULL;
	}*/
    
	/*If there is a space or a tab in the first place of line, we will move forward until we reach the word 	itself that we wish to remove.*/
	/*if(line[firstWordLength] == ' ' || line[firstWordLength] == '\t')
	{
		while(line[firstWordLength] == ' ' || line[firstWordLength] == '\t')
		{
			firstWordLength++;
		}
	}*/


	/*Find the length of the first word.*/
	/*while (line[firstWordLength] != ' ' && line[firstWordLength] != '\t' && line[firstWordLength] != '\0') 
	{
		firstWordLength++;
	}*/

	/*Assign the space to the new string and copy the sentence without the first word.*/
	/*result = (char*)malloc(strlen(line) - firstWordLength + 1);

	if (result != NULL)
	{*/
		/*Reset the memory.*/
        	/*memset(result, 0, strlen(line) - firstWordLength + 1);*/

		/*Copy the sentence without the first word.*/
		/*strcpy(result, line + firstWordLength + 1);*/
	/*}

	return result;
}*/



/*A function that accepts a sentence and returns a copy of it without the first word.*/
char* remove_first_word(char *line)
{
	int firstWordLength = 0;
	char *result;
    

	/*If the sentence is empty we cannot remove one word from it.*/
	if(count_words(line) == 0)
	{
		return NULL;
	}

	/*If there is a space or a tab or a comma in the first place of line, we will move forward until we 		reach the word itself that we wish to remove.*/
	if(isspace(line[firstWordLength]) || line[firstWordLength] == ',')
	{
		while((isspace(line[firstWordLength]) || line[firstWordLength] == ',') && line[firstWordLength]
		!= '\0')
		{
			firstWordLength++;
		}
	}


	/*Find the length of the first word.*/
	while (!isspace(line[firstWordLength]) && line[firstWordLength] != ',' && line[firstWordLength] != '=' 		&& line[firstWordLength] != '\0') 
	{
		firstWordLength++;
	}

	/*Assign the space to the new string and copy the sentence without the first word.*/
	result = (char*)malloc(strlen(line) - firstWordLength + 1);

	if (result != NULL)
	{
		/*Reset the memory.*/
        	memset(result, 0, strlen(line) - firstWordLength + 1);

		/*Copy the sentence without the first word.*/
		strcpy(result, line + firstWordLength + 1);
	}

	return result;
}



/*A function that accepts a sentence and returns a copy of the sentence without the first two words.*/
char* remove_two_words(char *line)
{
	char *temp;
	char *result;

	/*If the sentence is empty or it have one word we cannot remove two words from it.*/
	if(count_words(line) == 0 || count_words(line) == 1)
	{
		return NULL;
	}

	/*Remove using a helper function the first word and store the new sentence in a temporary variable.*/
	temp = remove_first_word(line);

	/*Removing from the sentence that is in the temporary variable the first word (it is the second word in 	the original sentence).*/
	result = remove_first_word(temp);

	/*Free the memory allocated for temp.*/
	free(temp); 

	return result;
}


/*A function that receives an input and checks whether the entire input contains only numbers. Commas are also allowed. If an error is found, the value 1 will be returned, otherwise 0 will be returned.*/
int are_all_numbers(char *input, int rowNumber, dataSymbols *arraySymbols)
{
	int errorFlag = 0;
	char nonNumericChars[MAX_LEN_LINE];
	int nonNumericIndex = 0;
	int symbolType = -1;


   	while (*input)
	{
        	/*If the current character is neither a digit nor a comma, the string does not contain only 			numbers.*/
        	if (!isdigit(*input) && (*input != ',') && (*input != '+') && (*input != '-') && 
		!isspace(*input))
		{
			/*We will insert all the characters that are in the sequence that are not numbers, ',' , 				'+' or '-'.*/
			while(!isdigit(*input) && (*input != ',') && (*input != '+') && (*input != '-')  && 
			!isspace(*input))
			{
				/*Add the character to the array of non-numeric characters.*/
				nonNumericChars[nonNumericIndex++] = *input;

				input++;
			}

			/*Terminate the non-numeric string.*/
			nonNumericChars[nonNumericIndex] = '\0';

			nonNumericIndex++;

			/*If the string is in the symbol table, we will verify that it is a define.*/
			if(is_in_symbol_table(nonNumericChars, arraySymbols) == 1)
			{
				symbolType = get_symbol_type(nonNumericChars, arraySymbols);

				/*The symbol is not of type define.*/
				if(symbolType != 0)
				{
					error_in_line(rowNumber);
					printf("One or more of the values ​​is not a number.\n");
            				errorFlag = 1;
				}

			}

			/*The string is not defined and is invalid.*/
			else
			{
				error_in_line(rowNumber);
				printf("One or more of the values ​​is not a number.\n");
            			errorFlag = 1;
			}
        	}

       		input++;
    	}

	/*One of the values ​​is not a number.*/
	if(errorFlag == 1)
	{
    		return 1;
	}

	/*No error found.*/
	else return 0;
}



/*A function that checks whether the numbers are valid.*/
int is_valid_number(char *number, int rowNumber)
{
	while (*number)
	{
		/*Settle at the start of a number and make sure it's a valid number.*/
            	if (*number == '+' || *number == '-')
            	{
                	number++;
            	}

		/*Check if the current character is not a digit.*/
            	if (!isdigit(*number))
            	{
                	return 1;
            	}
        
            	/*Jump to the end of the number.*/
            	while (isdigit(*number))
            	{
                	number++;
            	}
            
            	/*If there is an invalid character after the number.*/
            	if (*number != ',' && !isspace(*number) && *number != '\0')
            	{
                	error_in_line(rowNumber);
                	printf ("Invalid character after number.\n");
                	return 1;
            	}
        }

        /*No error found.*/
        return 0;
}




/*A function that checks whether the string is valid. If so it returns 0 and otherwise it returns 1.*/
int is_valid_string(char *sentence, int rowNumber) 
{
        int errorFlag = 0;
        
	/*In order to check if there is a "" at the beginning and end of the word, we will make sure that there 	are no spaces.*/
	remove_blanks(sentence);

        /*Checking whether the string starts and ends with double commas.*/
        if (*sentence != '"' || sentence[strlen(sentence) - 1] != '"')
        {
            error_in_line(rowNumber);
            printf("Missing a dash at the beginning or end of the word.\n");
            errorFlag = 1;  
        }
        
        if(errorFlag == 1)
        {
            return 1;
        }
        
        /*No error found.*/
        return 0;
}





/*A function that checks whether a label is valid, it returns 0 if so, and 1 if not.*/
int legal_label(char* label, int rowNumber, dataSymbols *arraySymbols, int isDefinition)
{
        int errorFlag;
	int k = 0;
        int i;
        int length;
	int removeColon = 0;

	errorFlag = 0;
        length = strlen(label);


	/*Skip all white characters until the beginning of the label.*/
	while(isspace(label[k]) && label[k] != '\0')
	{
		k++;
	}


	/*Checking whether the label starts with an uppercase or lowercase alphabetic letter.*/
        if (!isalpha(label[k]))
        {
            error_in_line(rowNumber);
            printf("The label does not start with the letter.\n");
            errorFlag = 1;
        }

	/*If this is a label definition, check for colons.*/
	if(isDefinition == 1)
	{
		/*Checking whether there are colons at the end of the word.*/
        	if (length > 0 && label[length - 1] == ':')
       		{

            		/*Checking whether they are adjacent to the end of the word.*/
            		if (length == 1 || (length > 1 && label[length - 2] == ' '))
            		{
               	 		error_in_line(rowNumber);
               	 		printf("Ther is space before colon in the label '%s'.\n", label);
                	 	errorFlag = 1;
            		}
            
        	}
        
        	else
        	{
            		error_in_line(rowNumber);
            		printf("No colon at the end of the label '%s'.\n", label);
            		errorFlag = 1;
        	}
	}


        /*If there is a ':' as in the label we will remove it and then search if it is a saved word.*/
	if (label[length - 1] == ':')
	{
        	label[length - 1] = '\0'; 
		removeColon = 1;
	}

        /*Is the length of the label longer than allowed.*/
        if (strlen(label) > LABEL_LENGTH)
        {
            error_in_line(rowNumber);
            printf("The length of the label longer than allowed.\n");
            errorFlag = 1;
        }

        
        /*Checking that after the first character there is a series of alphabetic letters or numbers.*/
        for (i = 1; label[i] != '\0'; i++)
        {
            if (!isalnum(label[i]))
            {
                error_in_line(rowNumber);
                printf("The label '%s' has a character that is not an alphabetic letter or a number.\n", label);
                errorFlag = 1;

            }
        }
        
        /*Checking whether the name of the label has already been defined in the past and is in the symbol 		table, as the name of a label, macro or define.*/
        if(isDefinition == 1 && is_in_symbol_table(label, arraySymbols) == 1 && find_dataAddr(arraySymbols, 		label) != '\0')
        {
            error_in_line(rowNumber);
            printf("The label name '%s' has already been defined previously.\n", label);
            errorFlag = 1;
        }
        
        /*Checking whether the name of the label is a reserved word*/
        if(is_reserved_words(label) == 1)
        {
            error_in_line(rowNumber);
            printf("The label name '%s' is a name reserved by the system and cannot be used.\n", label);
            errorFlag = 1;
        }

	/*If we took down the colons we will return them.*/
	if(removeColon == 1)
	{
		strcpy(&label[length - 1], ":"); 
	}
        
        if(errorFlag == 1)
        {
            return 1;
        }
        
        else return 0;
}




/*A function to count the number of words in a sentence.*/ 
int count_words(char *sentence)
{
        int wordCount = 0;
        int inWord = 0;
        int i;

        /*Go over the sentence characters.*/ 
        for (i = 0; i < strlen(sentence); ++i) 
        {
            /*Checking whether the character is a white character or a space.*/ 
            if (sentence[i] == ' ' || sentence[i] == '\t' || sentence[i] == ',' || sentence[i] == '\n') 
            {
                /*The character is a space or a white character, not currently in the word.*/
                inWord = 0;    
            }
            
            else if (!inWord)
            {
                /*Another character, the character is the beginning of a new word.*/
                inWord = 1;
                
                /*The word count.*/
                ++wordCount;
            }
        }

        /*Return the number of words in the sentence.*/
        return wordCount;
}




/*Function to check if a sentence contains commas. */
int has_commas(char *sentence)
{
	while (*sentence)
	{
        	if (*sentence == ',')
        	{
			/*Found a comma.*/
            		return 1;  
        	}
        	sentence++;
   	}

	/*No commas found.*/ 
    	return 0;  
}



/*A function that receives an operand of the fixed index address type and separates the array from the array index. The function returns the dataAddr of the operand and if it isent exists it returns INT_MIN.*/
int extract_fixed_index(char *input, char *word, dataSymbols *arraySymbols)
{
	int number = INT_MIN;
	char temp[LABEL_LENGTH];

	/*Extracting the word and number from the type.*/ 
	sscanf(input, "%[^[][%d]", word, &number);

	/*If it is an integer.*/
	if (number != INT_MIN)
	{
            	return number;
    	}

	/*If the number is not an integer, then it is probably defined as a constant and we will have to find 		its value.*/ 
	else
	{
        	/*Extracting a word instead of an integer.*/ 
        	sscanf(input, "%[^[][%[^]]", word, temp);

		/*If the value is in the symbol table.*/
		if(is_in_symbol_table(temp, arraySymbols) == 1)
		{
			/*Finding and return the numerical value of the definition.*/
			return find_dataAddr(arraySymbols, temp);
		}
    	}

	/*Number extraction failed due to invalid setting. Returned a number that cannot be received from pages 	or from the user because they only work if whole improvements are made.*/
	return INT_MIN;
}




/*A function that returns the number of the register. If it is not a register it will return -1.*/
int extract_register(char *regist)
{

	/*If the first character is not 'r' or the length of the string is not 2, it is invalid.*/
	if (regist[0] != 'r' || strlen(regist) != 2)
	{
		/*The string is invalid and is not a register.*/
        	return -1; 
    	}
    
 	/*Returning the number represented by the second character in the string as an integer.*/
	return regist[1] - '0';
}



/*A function that receives a two-dimensional array and checks how many cells are filled in it.*/
int count_elements(char **array)
{
	return elementsInArray;
}



/*A function that accepts a string with a " sign at the beginning and end and returns the string without this sign.*/
char* remove_quotes(char *str, dataSymbols *arraySymbols, char **tempLine)
{
	int len;

	char *new_str;

	/*Remove white characters.*/
	remove_blanks(str);

	len = strlen(str);

	new_str = malloc(len - 1);

	/*Check if memory allocation was successful*/
	if (new_str == NULL)
	{
        	printf("Memory allocation failed\n");
		free_arraySymbols(arraySymbols);
		free_array(tempLine);
        	exit(1);
    	}

	/*Copy characters from str to new_str, excluding the first and last characters.*/
	strncpy(new_str, str + 1, len - 2);

	/*Add null terminator to new_str.*/
	new_str[len - 2] = '\0';

	/*Return the new string.*/
	return new_str;
}



/*A function that receives a two-dimensional array from which the aging must be released and indicates a release to each cell in the array and finally to the array itself.*/
void free_array(char **array)
{
	int i = 0;
	int length = count_elements(array);

	/*Freeing all array values.*/
	while (i < length)
	{
		free(array[i]);
        	i++;
    	}

	/*The release of the array itself.*/
    	free(array);

}

