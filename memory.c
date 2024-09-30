#include "heder.h"


/*Enum for the bit-place representation of the code words.*/
enum {zero, one, two, three, four, five, six, seven, eight, nine, ten, eleven, twelve, thirteen};

	
/*ArrayInstructions size counter.*/
int sizeArrayInstructions = 0;

/*ArrayData size counter.*/
int sizeArrayData = 0;


void resetMemoryCounters(unsigned *IC, unsigned *DC)
{
    *IC = 0;
    *DC = 0;
}


/*A function that increments the value of the IC.*/
void increaseInstructionCounter(int amount, unsigned *IC)
{
	*IC = *IC + amount;
}


/*A function that increments the value of the DC.*/
void increaseDataCounter(int amount, unsigned *DC)
{
    *DC = *DC + amount;
}





/*A function that accepts a number and inserts its binary value into the array.*/
void int_to_binary(int num, long int *binary)
{ 
	int i;

	/*This code will go through all the members of the array and fill it in the appropriate way.*/
	for (i = SIZE_BINARY_CODE - 1; i >= 0; i--)
	{
		/*Received only binary characters ('0' or '1').*/
        	binary[i] = (num & 1);

		/*Move the number to the right.*/
        	num >>= 1;  
    	}

}


/*A function that accepts a single character and returns its ascii value.*/
int char_to_ascii(char c)
{
    return (int)c;
}



/*A function that inserts the values ​​of '.data' and '.string' into the data array. Notice that the sentence that the function accepts is without a label if there is one and without the directive word.*/
void insert_arrayData(char *sentence, char *typeSentence, long int arrayData[][SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber, unsigned int *DC, char **arrayLine)
{
	char **tempLine;
	char *sentenceWithotQuotes = remove_quotes(sentence, arraySymbols, arrayLine);
	int i = 0;
	int k = 0;
	int ascii = 0;
	int temp = 0;
	int numToBinary = 0;

	/*Assuming 14 bits.*/
	long int binary[SIZE_BINARY_CODE]; 
	int elements;



	/*Checking whether there is room for the insertion of additional words.*/
	if(sizeArrayInstructions + sizeArrayData >= MAX_WORDS)
	{
		printf("The word limit is over, the program will stop now.\n");
		free_arraySymbols(arraySymbols);
		free(sentenceWithotQuotes);
		free_array(arrayLine);
		exit(1);
	}


	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(sentence);

	if(tempLine == NULL)
	{
		free(sentenceWithotQuotes);
		return;
	}

	elements = count_elements(tempLine);

	/*We must enter numerical values or define.*/
	if(strcmp(".data", typeSentence) == 0)
	{
		while(i < elements)
		{
			/*If the string does not represent a number, it is most likely a definition.*/
			if(is_number(tempLine[i]) == 1)
			{
				/*The value appears in the symbol table.*/
				if(is_in_symbol_table(tempLine[i], arraySymbols) == 1)
				{
					/*If the value type is defined.*/
					if(get_symbol_type(tempLine[i], arraySymbols) == define)
					{
						/*Replace the define with its value according to the 							symbol table.*/
						temp = find_dataAddr(arraySymbols,tempLine[i]);

						sprintf(tempLine[i], "%d", temp);
					}
				}

				/*Not found in the symbol table.*/
				else 
				{
					error_in_line(rowNumber);
					printf("Invalid value in the '.data' directive.\n");
				}
			}

			/*Convert the number in the string to an integer.*/
			numToBinary = s_to_num(tempLine[i]);

			/*Convert the number to 14-bit binary form.*/
			int_to_binary(numToBinary, binary);

			/*Checking whether there is room for the insertion of additional words.*/
			if(sizeArrayInstructions + sizeArrayData >= MAX_WORDS)
			{
				printf("The word limit is over, the program will stop now.\n");
				free_arraySymbols(arraySymbols);
				free(sentenceWithotQuotes);
				free_array(tempLine);
				free_array(arrayLine);
				exit(1);
			}

			/*Copy the binary code into the array.*/
			memcpy(arrayData[*DC], binary, SIZE_BINARY_CODE * sizeof(long int));

			/*Increase the data counter.*/
			(*DC)++;

			/*Increase the arrayData size counter.*/
			sizeArrayData++;

			i++;
		}
	}

	
	/*We must enter numerical values.*/
	if(strcmp(".string", typeSentence) == 0)
	{
		/*Go through each character in the string.*/
		while(sentenceWithotQuotes[k] != '\0')
		{
			/*If the character is not a white character.*/
			if(!isspace(sentenceWithotQuotes[k]))
			{
				/*Return the ascii code of the character.*/
				ascii = char_to_ascii(sentenceWithotQuotes[k]);

				int_to_binary(ascii, binary);

				/*Copy the binary code into the array.*/
				memcpy(arrayData[*DC], binary, SIZE_BINARY_CODE * sizeof(long int));

				(*DC)++;	
			}

			k++;

			/*Increase the arrayData size counter.*/
			sizeArrayData++;
		}

		/*Insert the value 0 after the end of the string to indicate its end.*/
		arrayData[*DC][0]= 0;

		/*Increase the arrayData size counter.*/
		sizeArrayData++;

		(*DC)++;

	}

	free_array(tempLine);
	free(sentenceWithotQuotes);


}






/*A function that inserts a binary value into the arrayInstructions.*/
void insert_arrayInstructions(long int *binary_code, long int arrayInstructions[][SIZE_BINARY_CODE], unsigned int *IC, dataSymbols *arraySymbols, char **arrayLine)
{
	int i;


	for (i = 0; i < SIZE_BINARY_CODE; ++i)
	{
        	arrayInstructions[*IC][i] = binary_code[i];
    	}


	/*Checking whether there is room for the insertion of additional words.*/
	if(sizeArrayInstructions + sizeArrayData >= MAX_WORDS)
	{
		printf("The word limit is over, the program will stop now.\n");
		free_arraySymbols(arraySymbols);
		free_array(arrayLine);
		exit(1);
	}

	(*IC)++;

	/*Increase the arrayInstructions size counter.*/
	sizeArrayInstructions++;
}




/*A function that calculates the number of words of a given instruction. The function returns the value of the number of words. This value can be at least 1 and at most 5.*/
int calculate_num_words(char *line, dataSymbols *arraySymbols, int rowNumber)
{
	int numWords = 0;
	char **tempLine;
	int i = 1;
	int j = 2;
	int elements = 0;

	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(line);
 
	if (tempLine != NULL)
	{
		elements = count_elements(tempLine);

		/*The operation is the first word in the sentence.*/
		if(index_instruction(line) == 0)
		{
			/*There is one operand in the statement.*/
			if(count_words(line) == INST_ONEOPERAND_NOLABEL)
			{
				/*Immediate address method adds one word.*/
				if(is_immediate(tempLine[ONE]) == 0)
				{
					numWords++;
				}

				/*Direct address method adds one word.*/
				else if(is_direct(tempLine[ONE], arraySymbols, rowNumber) == 0)
				{
					numWords++;
				}

				/*Fixed index address method adds two words.*/
				else if(is_fixed_index(tempLine[ONE], rowNumber) == 0)
				{
					numWords = numWords + WORDS_FIXED_INDEX;
				}

				/*Register address method adds one word.*/
				else if(is_register(tempLine[ONE]) == 0)
				{
					numWords++;
				}

				/*If it is not any of the methods above, leave one place in case it is a label 					that has not yet been defined and will be defined later, so it will be a direct 				addressing method. If it is not a legal label, the program will not continue to 				build the words anyway.*/
				else
				{
					numWords++;
				}
			}

			/*There is two operands in the statement.*/
			else if(count_words(line) == INST_TWOOPERAND_NOLABEL)
			{
				/*If both operands are of register type, add one word.*/
				if(is_register(tempLine[ONE]) == 0 && is_register(tempLine[TWO]) == 0)
				{
					numWords++;
				}

				else
				{
					while(i < elements)
					{
						/*Immediate address method adds one word.*/
						if(is_immediate(tempLine[i]) == 0)
						{
							numWords++;
						}

						/*Direct address method adds one word.*/
						else if(is_direct(tempLine[i], arraySymbols, rowNumber) == 0)
						{
							numWords++;
						}

						/*Fixed index address method adds two words.*/
						else if(is_fixed_index(tempLine[i], rowNumber) == 0)
						{
							numWords = numWords + WORDS_FIXED_INDEX;
						}

						/*Register address method adds one word.*/
						else if(is_register(tempLine[i]) == 0)
						{
								numWords++;
						}

						/*If it is not any of the methods above, leave one place in case 							it is a label that has not yet been defined and will be defined 						later, so it will be a direct addressing method. If it is not a 						legal label, the program will not continue to build the words 							anyway.*/
						else
						{
							numWords++;
						}

						i++;
					}
				}

			}
		}/*End of the operation is the first word in the sentence.*/

		/*The operation is the second word in the sentence.*/
		if(index_instruction(line) == 1)
		{
			/*There is one operand in the statement.*/
			if(count_words(line) == INST_ONEOPERAND_WITHLABEL)
			{
				/*Immediate address method adds one word.*/
				if(is_immediate(tempLine[TWO]) == 0)
				{
					numWords++;
				}

				/*Direct address method adds one word.*/
				else if(is_direct(tempLine[TWO], arraySymbols, rowNumber) == 0)
				{
					numWords++;
				}

				/*Fixed index address method adds two words.*/
				else if(is_fixed_index(tempLine[TWO], rowNumber) == 0)
				{
					numWords = numWords + WORDS_FIXED_INDEX;
				}

				/*Register address method adds one word.*/
				else if(is_register(tempLine[TWO]) == 0)
				{
					numWords++;
				}

				/*If it is not any of the methods above, leave one place in case it is a label 					that has not yet been defined and will be defined later, so it will be a direct 				addressing method. If it is not a legal label, the program will not continue to 				build the words anyway.*/
				else
				{
					numWords++;
				}
			}

			/*There is two operands in the statement.*/
			else if(count_words(line) == INST_TWOOPERAND_WITHLABEL)
			{
				/*If both operands are of register type, add one word.*/
				if(is_register(tempLine[TWO]) == 0 && is_register
				(tempLine[TARGET_OP_WITH_LABEL]) == 0)	
				{
					numWords++;
				}

				else
				{
					while(j < elements)
					{
						/*Immediate address method adds one word.*/
						if(is_immediate(tempLine[j]) == 0)
						{
							numWords++;
						}

						/*Direct address method adds one word.*/
						else if(is_direct(tempLine[j], arraySymbols, rowNumber) == 0)
						{
							numWords++;
						}

						/*Fixed index address method adds two words.*/
						else if(is_fixed_index(tempLine[j], rowNumber) == 0)
						{
							numWords = numWords + WORDS_FIXED_INDEX;
						}

						/*Register address method adds one word.*/
						else if(is_register(tempLine[j]) == 0)
						{
								numWords++;
						}

						/*If it is not any of the methods above, leave one place in case 							it is a label that has not yet been defined and will be defined 						later, so it will be a direct addressing method. If it is not a 						legal label, the program will not continue to build the words 							anyway.*/
						else
						{
							numWords++;
						}

						j++;
					}
				}

			}/*End of there is two operands in the statement.*/
	
		}/*End of the operation is the second word in the sentence.*/

		free_array(tempLine);
	}

	return numWords;
}




/*A function that accepts an array and inserts the address of the operand into the requested indexes.*/
void enter_address_method(long int *binary_code, char *operand, int i, int j, dataSymbols *arraySymbols, int rowNumber)
{
	/*If any white characters remain, we will remove them.*/
	remove_blanks(operand);

	/*Immediate operand value is 0.*/
	if(is_immediate(operand) == 0)
	{
		binary_code[i] = 0;
		binary_code[j] = 0;
	}

	/*Direct operand value is 1.*/
	else if(is_direct(operand, arraySymbols, rowNumber) == 0)
	{
		binary_code[i] = 0;
		binary_code[j] = 1;
	}

	/*Fixed index operand value is 2.*/
	else if(is_fixed_index(operand, rowNumber) == 0)
	{
		binary_code[i] = 1;
		binary_code[j] = 0;
	}

	/*Register operand value is 3.*/
	else if(is_register(operand) == 0)
	{
		binary_code[i] = 1;
		binary_code[j] = 1;
	}

	/*If it's not any of the methods above then it's probably a label that hasn't been defined. If the label 		is invalid, the file building process will not continue, so we can assume that it is a label and the 		addressing method is direct.*/
	else
	{
		binary_code[i] = 0;
		binary_code[j] = 1;
	}
}


/*A function that accepts an array and inserts the address of the operation into the indexes.*/
void enter_address_operation(long int *binary_code, char *operation)
{
	/*If any white characters remain, we will remove them.*/
	remove_blanks(operation);

	if(strcmp(operation, "mov") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 0, 0, 0);
	}

	else if(strcmp(operation, "cmp") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 0, 0, 1);
	}

	else if(strcmp(operation, "add") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 0, 1, 0);
	}

	else if(strcmp(operation, "sub") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 0, 1, 1);
	}

	else if(strcmp(operation, "not") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 1, 0, 0);
	}

	else if(strcmp(operation, "clr") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 1, 0, 1);
	}

	else if(strcmp(operation, "lea") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 1, 1, 0);
	}

	else if(strcmp(operation, "inc") == 0)
	{
		helper_enter_address_operation(binary_code, 0, 1, 1, 1);
	}

	else if(strcmp(operation, "dec") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 0, 0, 0);
	}

	else if(strcmp(operation, "jmp") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 0, 0, 1);
	}

	else if(strcmp(operation, "bne") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 0, 1, 0);
	}

	else if(strcmp(operation, "red") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 0, 1, 1);
	}

	else if(strcmp(operation, "prn") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 1, 0, 0);
	}

	else if(strcmp(operation, "jsr") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 1, 0, 1);
	}

	else if(strcmp(operation, "rts") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 1, 1, 0);
	}

	else if(strcmp(operation, "hlt") == 0)
	{
		helper_enter_address_operation(binary_code, 1, 1, 1, 1);
	}

}


/*A helper function for a function enter_address_operation. The array defined in Maman is the opposite of the array defined in the system, therefore indexes 6-9 in Maman are 4-7 in the system.*/
void helper_enter_address_operation(long int *binary_code, int i, int j, int k, int x)
{
	binary_code[four] = i;

	binary_code[five] = j;

	binary_code[six] = k;

	binary_code[seven] = x;
}




/*This function constructs the binary code of the first word of an instruction.*/
void binary_word_instruction(char *operation, char *opSrc, char *opDes, dataSymbols *arraySymbols, long int *binary_code, int rowNumber)
{
	/*Reset the array.*/
	memset(binary_code, 0, SIZE_BINARY_CODE * sizeof(long int)); 


	/*If there is a target operand, we will find its addressing method and insert the appropriate code into 	the appropriate bits.*/
	if(opDes != '\0')
	{
		enter_address_method(binary_code, opDes, ten, eleven, arraySymbols, rowNumber);
	}

	/*If there is a source operand, we will find its addressing method and insert the appropriate code into 	the appropriate bits.*/
	if(opSrc != '\0')
	{
		enter_address_method(binary_code, opSrc, eight, nine, arraySymbols, rowNumber);
	}

	/*Enter the binary value of the operation.*/
	enter_address_operation(binary_code, operation);

	
}





/*This code puts the value of the number in binary form into the 12 bits (from 2 to 13).*/
void operand_to_binary(int number, long int *binary)
{ 
	int i;

	/*The code will go from the last index to two bits before the end (in which there will be an ARE value), 		and fill it in the appropriate way.*/
	for (i = SIZE_BINARY_CODE - 1 - BIT_FOR_ARE; i >= 0; i--)
	{
		/*Received only binary characters ('0' or '1').*/
        	binary[i] = (number & 1);

		/*Move the number to the right.*/
        	number >>= 1;  
    	}

}




/*A function inserts the register value into the array according to the requested indexes. The function only works for positive numbers.*/
void register_to_binary(long int *binary, int start_index, int end_index, int number)
{
	/*We will start from the last index where we will insert the last digit of the number.*/
	int i = end_index;

	/*From the current digit to the first digit of the number, we will enter in the reverse order.*/
	while(i >= start_index && number > 0)
	{
		/*The binary number is the remainder of dividing the number by 2.*/
        	binary[i] = number % 2;

		/*Divide the number by two to move to the next digit.*/
        	number /= 2;

		/*Lower the index to move to the next number in the array.*/
        	i--;
    	}

}


/*A helper function for inserting an operand of immediate type into the binary code. If the insertion was successful, 0 will be returned, otherwise -1 will be returned.*/
int enter_immediate(char *operand, long int binary[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber)
{
	/*A string that we can change the operand to.*/
	char *editing;
	int saveNum;
	int opValue;


	editing = malloc(strlen(operand) * sizeof(char));

	if(editing == NULL)
	{
		printf("Error: allocating space for 'editing' failure.\n");
		return -1;
	}

	/*We will pass the entire operand without the # symbol that is in the first character.*/
	strcpy(editing, &operand[ONE]); 

	/*The operand is an integer.*/
	if(is_number(editing) == 0)
	{
		/*Convert the number in the string to an integer.*/
		saveNum = s_to_num(editing);

		/*Enter the value of the number into the 12 bits.*/
		operand_to_binary(saveNum, binary);

		free(editing);

		return 0;
	}

	/*The operand is not a number but probably a define, but we will check that.*/
	else
	{
		/*Found in the symbol table as define.*/
		if(is_in_symbol_table(editing, arraySymbols) == 1 && get_symbol_type(editing, arraySymbols) == 			define)
		{
			opValue = find_dataAddr(arraySymbols, editing);

			/*Enter the value of the number into the 12 bits.*/
			operand_to_binary(opValue, binary);

			free(editing);

			return 0;
		}

		/*An undefined label is therefore invalid to represent a number.*/
		else
		{
			error_in_line(rowNumber);
			printf("Undefined value.\n");
			free(editing);
			return -1;
		}
	}
}




/*A helper function for inserting an operand of direct type into the binary code. If the insertion was successful, 0 will be returned, otherwise -1 will be returned.*/
int enter_direct(char *operand, long int binary[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber)
{
	int opValue;

	if(is_in_symbol_table(operand, arraySymbols) == 1)
	{
		/*If the label is in the symbol table, take its value.*/
		opValue = find_dataAddr(arraySymbols, operand);

		/*Enter the value of the number into the 12 bits.*/
		operand_to_binary(opValue, binary);

		if(get_symbol_type(operand, arraySymbols) == Extern)
		{
			binary[twelve] = 0;
			binary[thirteen] = 1;
		}

		else
		{
			binary[twelve] = 1;
			binary[thirteen] = 0;
		}

		return 0;
	}

	/*An undefined label.*/
	else
	{
		error_in_line(rowNumber);
		printf("Undefined value.\n");
		return -1;
	}
}




/*A helper function for inserting an operand of fixed index type into the binary code. If the insertion was successful, 0 will be returned, otherwise -1 will be returned.*/
int enter_fixed_index(char *operand, long int binary[SIZE_BINARY_CODE], long int binary1[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber)
{
	char *word;
	int toExtr;
	int opValue;


	word = malloc(strlen(operand) * sizeof(char));

	if (word == NULL)
	{
		printf("Error: allocating space for 'word' failure.\n");
		return -1;
	}

	/*Extract the array and the number from the operand.*/
	toExtr = extract_fixed_index(operand, word, arraySymbols);

	if(toExtr != INT_MIN)
	{
		if(is_in_symbol_table(word, arraySymbols) == 1)
		{
			/*If the array is in the symbol table, take its value.*/
			opValue = find_dataAddr(arraySymbols, word);

			/*Enter the value of the number into the 12 bits.*/
			operand_to_binary(opValue, binary);

			if(get_symbol_type(word, arraySymbols) == Extern)
			{
				binary[twelve] = 0;
				binary[thirteen] = 1;
			}

			else
			{
				binary[twelve] = 1;
				binary[thirteen] = 0;
			}
		}

		/*If the first word is valid, build the second word. Enter the value of the number into the 12 			bits.*/
		operand_to_binary(toExtr, binary1);

		/*The ARE value will be 00 for the second word.*/
		binary1[twelve] = 0;
		binary1[thirteen] = 0;

		/*Two words entered successfully.*/
		free(word);
		return 0;
	}

	/*Extraction failed meaning the expression is not legally defined.*/
	else 
	{
		error_in_line(rowNumber);
		printf("Undefined value.\n");
		free(word);
		return -1;
	}
}




/*A function that inserts the code words into arrays. If it failed to insert the word code it will return -1. If it succeeded and there is only one word, it will return 0. If there are two words generated, it will return 2. For the case that the operand is of register type, 'operand' represents source operand and 'operand1' represents destination operand.*/
int binary_operand_instruction(char *operand, char *operand1, dataSymbols *arraySymbols, int rowNumber, long int binary[SIZE_BINARY_CODE], long int binary1[SIZE_BINARY_CODE])
{
	int numRegist;
	int j;

	if(operand != '\0')
	{
		remove_blanks(operand);
	}

	if(operand1 != '\0')
	{
		remove_blanks(operand1);
	}

	/*Initializing the arrays to zeros.*/
	for (j = 0; j < SIZE_BINARY_CODE; ++j)
	{
        	binary[j] = 0;
		binary1[j] = 0;
    	}
	
	/*Immediate intervention method.*/
	if(operand != '\0' && is_immediate(operand) == 0)
	{
		if(enter_immediate(operand, binary, arraySymbols, rowNumber) == 0)
		{
			return 0;
		}

		else
			return -1;
	}/*End of immediate intervention method.*/


	/*Immediate intervention method.*/
	if(operand1 != '\0' && is_immediate(operand1) == 0)
	{
		if(enter_immediate(operand1, binary, arraySymbols, rowNumber) == 0)
		{
			return 0;
		}

		else
			return -1;

	}/*End of immediate intervention method.*/


	/*Direct intervention method.*/
	if(operand != '\0' && is_direct(operand, arraySymbols, rowNumber) == 0)
	{
		if(enter_direct(operand, binary, arraySymbols, rowNumber) == 0)
		{
			return 0;
		}

		else
			return -1;

	}/*End of direct intervention method.*/

	/*Direct intervention method.*/
	if(operand1 != '\0' && is_direct(operand1, arraySymbols, rowNumber) == 0)
	{
		if(enter_direct(operand1, binary, arraySymbols, rowNumber) == 0)
		{
			return 0;
		}

		else
			return -1;

	}/*End of direct intervention method.*/

	/*Fixed index intervention method.*/
	if(operand != '\0' && is_fixed_index(operand, rowNumber) == 0)
	{
		if(enter_fixed_index(operand, binary, binary1, arraySymbols, rowNumber) == 0)
		{
			/*Return two words entered successfully.*/
			return 2;
		}

		else
			return -1;
	}/*End fixed index intervention method.*/


	/*Fixed index intervention method.*/
	if(operand1 != '\0' && is_fixed_index(operand1, rowNumber) == 0)
	{
		if(enter_fixed_index(operand1, binary, binary1, arraySymbols, rowNumber) == 0)
		{
			/*Return two words entered successfully.*/
			return 2;
		}

		else
			return -1;
	}/*End fixed index intervention method.*/


	if(operand != '\0' && operand1 != '\0' && is_register(operand) == 0)
	{
		/*Extract the number from the register.*/
		numRegist = extract_register(operand);

		/*Extraction failed, operand is not a register.*/
		if(numRegist == -1)
		{
			error_in_line(rowNumber);
			printf("Undefined value.\n");
			return -1;
		}

		/*We will insert the numerical value of the register into the array starting from index five to 		index seven (six to eight in the system).*/
		register_to_binary(binary, six, eight, numRegist);

		/*Extract the number from the register.*/
		numRegist = extract_register(operand1);

		/*Extraction failed, operand is not a register.*/
		if(numRegist == -1)
		{
			error_in_line(rowNumber);
			printf("Undefined value.\n");
			return -1;
		}

		/*We will insert the numerical value of the register into the array starting from index two to 			index four(nine to eleven in the system).*/
		register_to_binary(binary, nine, eleven, numRegist);

		/*The words was entered successfully.*/
		return 0;
	}


	/*If the source operand is a register.*/
	if(operand != '\0' && is_register(operand) == 0)
	{
		/*Extract the number from the register.*/
		numRegist = extract_register(operand);

		/*Extraction failed, operand is not a register.*/
		if(numRegist == -1)
		{
			error_in_line(rowNumber);
			printf("Undefined value.\n");
			return -1;
		}

		/*We will insert the numerical value of the register into the array starting from index five to 		index seven (six to eight in the system).*/
		register_to_binary(binary, six, eight, numRegist);

		/*The word was entered successfully.*/
		return 0;
	}

	/*If the destination operand is a register.*/
	if(operand1 != '\0' && is_register(operand1) == 0)
	{
		/*Extract the number from the register.*/
		numRegist = extract_register(operand1);

		/*Extraction failed, operand is not a register.*/
		if(numRegist == -1)
		{
			error_in_line(rowNumber);
			printf("Undefined value.\n");
			return -1;
		}

		/*We will insert the numerical value of the register into the array starting from index two to 			index four(nine to eleven in the system).*/
		register_to_binary(binary, nine, eleven, numRegist);

		/*The word was entered successfully.*/
		return 0;
	}

	return 1;
		
}


/*Return the size of sizeArrayInstructions.*/
int return_size_arrayInstructions()
{
	return sizeArrayInstructions;
}


/*Return the size of sizeArrayData*/
int return_size_arrayData()
{
	return sizeArrayData;
}
