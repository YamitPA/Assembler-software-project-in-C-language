#include "heder.h"

/*Functions that check for each type of statement whether it is valid.*/





/*The function checks if the define sentence is valid. If so, it inserts the define code into the symbol table and returns 0. Otherwise it returns 1.*/
int is_define_valid(char *line, dataSymbols *arraySymbols, int rownum)
{
	/*We will save a flag that will mark if the definition line is correct. If the value of the flag is 0, it 		means that the definition is correct and therefore we can add the definition to the symbol table. If the 		value of the flag is equal to 1, the definition is incorrect and therefore we will not insert it into the 		symbol table.*/
	int flagError = 0;

	/*A variable that will hold the value of a define if it is valid.*/
	int dataDefine;

	/*Analyze the line of the define on a temporary variable.*/
	char **tempLine;
	int errorMode;

	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(line);

	if(tempLine == NULL)
	{
		return 1;
	}

	/*If the number of words in the sentence contains more than the 4 words, then there are more words than 	necessary.*/
	if(count_elements(tempLine) > DEFINE_EMPTY_VALUE)
	{
		error_in_line(rownum);
		printf("Too many values ​​in the define command.\n");
		flagError = 1;
	}

	/*If the number of words in the sentence contains less than the 4 words, then there are not enough words 		for define sentence.*/
	if(count_elements(tempLine) < DEFINE_EMPTY_VALUE)
	{
		error_in_line(rownum);
		printf("Missing information in define sentence.\n");
		flagError = 1;
	}

	/*We will check if the name of the define is already in the symbol table.*/
	if(is_in_symbol_table(tempLine[DEFINE_NAME_IN_ARRAY], arraySymbols) == 1)
	{
		error_in_line(rownum);
		printf("The define name %s has already been defined before.\n", tempLine[DEFINE_NAME_IN_ARRAY]);
		flagError = 1;
	}

	/*We will check if the name of the define is reserved word.*/
	if(is_reserved_words(tempLine[DEFINE_NAME_IN_ARRAY]) == 1)
	{
		error_in_line(rownum);
		printf("The define name %s is a reserved word.\n",tempLine[DEFINE_NAME_IN_ARRAY]);
		flagError = 1;			
	}
		
	/*If missing an equal sign.*/
	if(is_equal_sign(tempLine[DEFINE_EQUAL_SING]) == 1)
	{
		error_in_line(rownum);
		printf("In the definition an equal sign is missing.\n");
		flagError = 1;
	}

	/*If there is no number for the define order.*/
	if(is_number(tempLine[DEFINE_NUMBER]) == 1)
	{
		error_in_line(rownum);
		printf("There is no number for the define order.\n");
		flagError = 1;
	}

	/*If we haven't gotten an error so far, that means the string can be converted to a number.*/
	if(flagError == 0)
	{
		dataDefine = s_to_num(tempLine[DEFINE_NUMBER]);

		/*Insert the symbol into the array of symbols.*/
		errorMode = add_node_to_arraySymbols(tempLine[DEFINE_NAME_IN_ARRAY], define, dataDefine, arraySymbols);

		/*Insertion into symbol table failed.*/
		if(errorMode == 1)
		{
			free_array(tempLine);
			return 1;
		}

		free_array(tempLine);
		return 0;
	}

	/*There were errors and no define value was inserted into the symbol table.*/
	free_array(tempLine);
	return 1;	
}







/*The function checks if the directive sentence is valid. If so, it returns 0, otherwise it returns 1.*/
int is_directive_sentence_valid(char *line, dataSymbols *arraySymbols, int rowNumber)
{
	char **tempLine;
	char *withoutFirstWord;
	char *withoutTwotWords;
	char char_value;
	int numWords;
	int errorFlag;
	int value;
	int i = 0;
	int j = 2;
	int elements = 0;

	numWords = 0;
	errorFlag = 0;

	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(line);

	if(tempLine == NULL)
	{
		return 1;
	}

	elements = count_elements(tempLine);

	/*Remove the data instruction for convenience for the following functions.*/
	withoutFirstWord = remove_first_word(line);

	/*Removing the label and the data instruction for convenience for the following functions.*/
	withoutTwotWords = remove_two_words(line);

	/*If the prompt code is in the first word.*/
	if(index_directive(line) == ZERO)
	{
		/*If the prompt code is ".data".*/
		if(strcmp(tempLine[ZERO], ".data") == 0)
		{
			/*Checking whether there is a comma adjacent to ".data".*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after '.data'.\n");
				errorFlag = 1;
			}

			/*Check if a comma is missing.*/
			if(missing_comma(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}
			
			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether all the values after the ".data" are indeed numbers only.*/
			if(are_all_numbers(withoutFirstWord, rowNumber, arraySymbols) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether the numbers themselves are valid (if there is no '+' or '-' at the end of 				a number or two in a row).*/
			while(i < elements)
			{
				/*If it is a definite found in the symbol table.*/
				if(is_in_symbol_table(tempLine[j], arraySymbols) == 1)
				{
					/*Find the value of the exponential.*/
					value = find_dataAddr(arraySymbols, tempLine[j]);

					/*Convert the number to a string.*/
					sprintf(&char_value, "%c", value);

					if(is_valid_number(&char_value, rowNumber) == 1)
					{
						errorFlag = 1;
					}

				}				

				/*Otherwise we will check if the number is correct.*/
				else if(is_valid_number(tempLine[i], rowNumber) == 1)
				{
					errorFlag = 1;
				}

				i++;
			}

		}/*End of If the prompt code is ".data".*/


		/*If the prompt code is ".string".*/
		else if(strcmp(tempLine[ZERO], ".string") == 0)
		{
			/*Checking whether there is a comma adjacent to ".string".*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after '.string'.\n");
				errorFlag = 1;
			}

			numWords = count_words(line);

			/*Checking whether the string is valid.*/
			if(is_valid_string(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}

		}/*End of If the prompt code is ".string".*/


		/*If the prompt code is ".entry" or ".extern".*/
		else if((strcmp(tempLine[ZERO], ".entry") == 0) || (strcmp(tempLine[ZERO], ".extern") == 0))
		{
			/*Checking whether there is a comma adjacent to ".entry" or ".extern".*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after label statement.\n");
				errorFlag = 1;
			}

			/*Checking whether there are more or fewer words than the required number of words in an 				entry/extern type prompt.*/
			numWords = count_words(line);

			if(numWords != WORDS_ENTRY_EXTERN)
			{
				error_in_line(rowNumber);
				printf("The number of words in the sentence does not match the type of sentence.\n");
				errorFlag = 1;
			}

			/*Checking whether the label is valid.*/
			if(legal_label(tempLine[ONE], rowNumber, arraySymbols, 0) == 1)
			{
				errorFlag = 1;
			}

		}/*End of If the prompt code is ".entry" or ".extern".*/

	}/*End of if the prompt code is in the first word.*/


	/*If the prompt code is in the second word, this means that there is a label and another treatment is 		needed.*/
	else if(index_directive(line) == ONE)
	{
		/*If the prompt code is ".data".*/
        	if(strcmp(tempLine[ONE], ".data") == 0)
		{
			/*Checking whether there is a comma after the label.*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after label.\n");
				errorFlag = 1;
			}

			/*Checking whether there is a comma adjacent to ".data".*/
			if(check_comma_between(withoutFirstWord, rowNumber) == 1)
			{
				printf("Unnecessary comma after '.data'.\n");
				errorFlag = 1;
			}

			/*Check if a comma is missing.*/
			if(missing_comma(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}


			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether all the values after the ".data" are indeed numbers only.*/
			if(are_all_numbers(withoutTwotWords, rowNumber, arraySymbols) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether the numbers themselves are valid (if there is no '+' or '-' at the end 				of a number or two in a row).*/
			while(j < elements)
			{
				/*If it is a definite found in the symbol table.*/
				if(is_in_symbol_table(tempLine[j], arraySymbols) == 1)
				{
					/*Find the value of the exponential.*/
					value = find_dataAddr(arraySymbols, tempLine[j]);

					/*Convert the number to a string.*/
					sprintf(&char_value, "%d", value);

					if(is_valid_number(&char_value, rowNumber) == 1)
					{
						errorFlag = 1;
					}

				}

				/*Otherwise we will check if the number is correct.*/
				else if(is_valid_number(tempLine[j], rowNumber) == 1)
				{
					errorFlag = 1;
				}

				j++;
			}

		}/*End of If the prompt code is ".data".*/


		/*If the prompt code is ".string".*/
		else if(strcmp(tempLine[ONE], ".string") == 0)
		{
			/*Checking whether there is a comma after the label.*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after label.\n");
				errorFlag = 1;
			}

			/*Checking whether there is a comma adjacent to ".string".*/
			if(check_comma_between(withoutFirstWord, rowNumber) == 1)
			{
				printf("Unnecessary comma after '.string'.\n");
				errorFlag = 1;
			}

			numWords = count_words(line);

			/*Checking whether the string is valid.*/
			if(is_valid_string(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}


		}/*End of If the prompt code is ".string".*/

		/*If the prompt code is ".entry" or ".extern".*/
		else if((strcmp(tempLine[ZERO], ".entry") == 0) || (strcmp(tempLine[ZERO], ".extern") == 0))
		{
			/*If the ".entry" or ".extern" is in the second place in the sentence, it means that 				there is a label at the beginning and the assembler will ignore it, so we will warn the 			user about it.*/
			error_in_line(rowNumber);
			printf("There is a label before the command, the assembler will ignore this label.\n");

			/*Checking whether there is a comma adjacent to ".entry" or ".extern".*/
			if(check_comma_between(withoutFirstWord, rowNumber) == 1)
			{
				printf("Unnecessary comma after label statement.\n");
				errorFlag = 1;
			}

			/*Checking whether the label is valid.*/
			if(legal_label(tempLine[TWO], rowNumber, arraySymbols, 0) == 1)
			{
				errorFlag = 1;
			}

		}/*End of If the prompt code is ".entry" or ".extern".*/

	}/*End of if the prompt code is in the second word.*/

	/*There were no errors.*/
	if(errorFlag == 0)
	{
		free(withoutFirstWord);
		free(withoutTwotWords);
		free_array(tempLine);
		return 0;
	}

	/*There were errors.*/
	free(withoutFirstWord);
	free(withoutTwotWords);
	free_array(tempLine);
	return 1;
}








/*The function checks if the instruction sentence is valid. If so, it returns 0, otherwise it returns 1.*/
int is_instruction_sentence_valid(char *line, dataSymbols *arraySymbols, int rowNumber)
{
	char **tempLine;
	int errorFlag;
	char *withoutFirstWord;
	char *withoutTwotWords;
			
	errorFlag = 0;

	/*Remove the operation for convenience for the following functions.*/
	withoutFirstWord = remove_first_word(line);

	/*Removing the label and the data instruction for convenience for the following functions.*/
	withoutTwotWords = remove_two_words(line);

	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(line);

	/*The instruction statement appears unlabeled at the beginning.*/
	if(index_instruction(line) == ZERO)
	{
		/*The operation instruction is of a type that receives two operands.*/
		if(two_operands_grop(tempLine[ZERO]) == 0)
		{
			/*Checking whether there is a comma adjacent to the instruction.*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after instruction.\n");
				errorFlag = 1;
			}

			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*There are no two operands separated by a comma.*/
			if(is_two_operands(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether the first operand (in this case the source operand) is suitable and legal 				for the operation.*/
			if(valid_operand_to_operation(tempLine[ZERO], tempLine[ONE], 1, 0, arraySymbols, 				rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The source operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[ONE]);
				errorFlag = 1;
			}

			/*Checking whether the second operand (in this case the target operand) is suitable and 			legal for the operation.*/
			if(valid_operand_to_operation(tempLine[ZERO], tempLine[TWO], 0, 1, arraySymbols, 				rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The target operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[TWO]);
				errorFlag = 1;
			}

		}/*End of operation of type that receives two operands.*/


		/*The operation instruction is of a type that receives one operand.*/
		if(one_operands_grop(tempLine[ZERO]) == 0)
		{
			/*Checking if there is a comma. According to the structure of the sentence for an 				operation that receives one operand, there should not be a comma in the sentence.*/
			if(has_commas(line) == 1)
			{
				error_in_line(rowNumber);
				printf("Unnecessary comma.\n");
				errorFlag = 1;
			}

			/*According to the structure of the sentence for an operation that receives one operand, 				if there is no label, the sentence should contain only two words: the name of the 				operation and one operand.*/
			if(count_words(line) != TWO)
			{
				error_in_line(rowNumber);
				printf("The operation name should be followed by one operand.\n");
				errorFlag = 1;
			}

			/*Checking whether the operand (in this case it is target operand) is suitable 				and legal for the operation.*/
			if(valid_operand_to_operation(tempLine[ZERO], tempLine[ONE], 0, 1, arraySymbols, 				rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The target operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[ONE]);
				errorFlag = 1;
			}

		}/*End of operation of type that receives one operand.*/


		/*The operation instruction is of a type that receives no operand.*/
		if(no_operands_grop(tempLine[ZERO]) == 0)
		{
			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(line, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*According to the structure of the sentence for an operation that receives no operands, if 				there is no label, the sentence should contain only one words - the name of the operation.*/
			if(count_words(line) != ONE)
			{
				error_in_line(rowNumber);
				printf("This operator takes no arguments.\n");
				errorFlag = 1;
			}
		}/*End of operation of type that receives no operand.*/

	}/*End of if the instruction statement appears unlabeled at the beginning.*/


	/*The instruction statement appears with label befor it.*/
	if(index_instruction(line) == ONE)
	{
		/*The operation instruction is of a type that receives two operands.*/
		if(two_operands_grop(tempLine[ONE]) == 0)
		{
			/*Checking whether there is a comma after the label.*/
			if(check_comma_between(line, rowNumber) == 1)
			{
				printf("Unnecessary comma after label.\n");
				errorFlag = 1;
			}

			/*Checking whether there is a comma adjacent to the instruction.*/
			if(check_comma_between(withoutFirstWord, rowNumber) == 1)
			{
				printf("Unnecessary comma after instruction.\n");
				errorFlag = 1;
			}

			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*There are no two operands separated by a comma.*/
			if(is_two_operands(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*Check if the label is valid.*/
			if(legal_label(tempLine[ZERO], rowNumber, arraySymbols, 1) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether the first operand (in this case the source operand) is suitable and legal 				for the operation.*/
			if(valid_operand_to_operation(tempLine[ONE], tempLine[SOURCE_OP_WITH_LABEL], 1, 0, 				arraySymbols, rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The source operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[SOURCE_OP_WITH_LABEL]);
				errorFlag = 1;
			}

			/*Checking whether the second operand (in this case the target operand) is suitable and 			legal for the operation.*/
			if(valid_operand_to_operation(tempLine[ONE], tempLine[TARGET_OP_WITH_LABEL], 0, 1, 				arraySymbols, rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The target operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[TARGET_OP_WITH_LABEL]);
				errorFlag = 1;
			}

		}/*End of operation of type that receives two operands.*/


		/*The operation instruction is of a type that receives one operand.*/
		if(one_operands_grop(tempLine[ONE]) == 0)
		{
			/*Checking if there is a comma. According to the structure of the sentence for an 				operation that receives one operand, there should not be a comma in the sentence.*/
			if(has_commas(line) == 1)
			{
				error_in_line(rowNumber);
				printf("Unnecessary comma.\n");
				errorFlag = 1;
			}

			/*According to the structure of the sentence for an operation that receives one operand, 				if there is no label, the sentence should contain only two words: the name of the 				operation and one operand.*/
			if(count_words(line) != ONE_OPERAND_LABEL)
			{
				error_in_line(rowNumber);
				printf("The operation name should be followed by one operand.\n");
				errorFlag = 1;
			}

			/*Check if the label is valid.*/
			if(legal_label(tempLine[ZERO], rowNumber, arraySymbols, 1) == 1)
			{
				errorFlag = 1;
			}

			/*Checking whether the operand (in this case the target operand) is suitable and legal 				for the operation.*/
			if(valid_operand_to_operation(tempLine[ONE], tempLine[TWO], 0, 1, 				arraySymbols, rowNumber) != 0)
			{
				error_in_line(rowNumber);
				printf("The target operand '%s' does not match the desired operation and is not valid for it.\n", tempLine[TWO]);
				errorFlag = 1;
			}

		}/*End of operation of type that receives one operand.*/

		/*The operation instruction is of a type that receives no operand.*/
		if(no_operands_grop(tempLine[ONE]) == 0)
		{
			/*Check whether there are too many commas or there are no commas.*/
			if(check_legal_commas(line, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*According to the structure of the sentence for an operation that receives no operands, if 				there is label, the sentence should contain two words - the label and the name of the 				operation.*/
			if(count_words(line) != TWO)
			{
				error_in_line(rowNumber);
				printf("This operator takes no arguments.\n");
				errorFlag = 1;
			}
		}/*End of operation of type that receives no operand.*/

	}/*End of if the instruction statement appears with label befor it.*/

	
	/*There was no error.*/
	if(errorFlag == 0)
	{
		free(withoutFirstWord);
		free(withoutTwotWords);
		free_array(tempLine);
		return 0;
	}


	/*There was error.*/
	free(withoutFirstWord);
	free(withoutTwotWords);
	free_array(tempLine);
	return 1;
}
