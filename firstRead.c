#include "heder.h"





/*In the array of instructions, assembler inserts the coding of machine instructions that were called during moving on to the source file.*/
long int arrayInstructions[SIZE_ARRAY][SIZE_BINARY_CODE];

/*In the data array, the assembler inserts the encoding of the data read from a file the source.*/
long int arrayData[SIZE_ARRAY][SIZE_BINARY_CODE];

unsigned int IC = 0;
unsigned int DC = 0;

/*The list where we will save all the symbols.*/
dataSymbols arraySymbols;


void run_first_pass(FILE *ofp, char *fileName)
{
	/* Memory address counters.*/
	int rowNumber = 1;
	int errorFlag = 0;
	int errorRunSecond = 0;
	int L = 0;
	char **tempLine;
	char *withoutFirstWord; 
	char *withoutTwotWords;  

	/*String for storing line that we read from file.*/
	char line[MAX_LEN_LINE];

	nodeDataSymbols *current;

	current = arraySymbols.head;


	/*Add an extension to the file name.*/
	strcat(fileName, ".am");

        /*Open the file '.am'.*/
        ofp = fopen(fileName, "r");

        /*Check if the file reopened successfully.*/
        if (ofp == NULL)
	{   
            	fprintf(stderr, "Error: Could not reopen file %s\n", fileName);
		free_arraySymbols(&arraySymbols);
            	exit(1);
        }

	/*Resetting counters.*/
	resetMemoryCounters(&IC, &DC);

	/*Read a line from the file each time until the end of the file.*/
	while (fgets(line, sizeof(line), ofp) != NULL) 
	{
		/*Put each word in a separate cell in the array to more easily access each data.*/
		tempLine = lineToArray(line);

		if(tempLine != NULL)
		{
			/*Remove the data instruction for convenience for the following functions.*/
			withoutFirstWord = remove_first_word(line);

			/*Removing the label and the data instruction for convenience for the following 			functions.*/
			withoutTwotWords = remove_two_words(line);

			/*An empty line, nothing to analyze.*/
			if(is_empty_sentence(line) == 0)
			{
				rowNumber++;
				free_array(tempLine);
				free(withoutFirstWord);
				free(withoutTwotWords);
				continue;
			}

			/*The sentence starts with semicolon, nothing to analyze.*/
			else if(starts_with_semicolon(line, &arraySymbols, tempLine) == 0)
			{
				rowNumber++;
				free_array(tempLine);
				free(withoutFirstWord);
				free(withoutTwotWords);
				continue;
			}

			/*Is the sentence a definition sentence.*/
			else if(is_define_sentence(line) == 0)
			{
				/*If the define statement is invalid, change the flag to 1. If the define 					statement was valid, the definition value was inserted into the symbol table.*/
				if(is_define_valid(line, &arraySymbols, rowNumber) == 1)
				{
					errorFlag = 1;
				}
			}

			/*If the sentence is a '.data' directive or a .'string' directive.*/
			else if(is_data(line) != -1 || is_string(line) != -1)
			{
				/*If the first field has a label.*/
				if((strcmp(tempLine[ONE], ".data") == 0) || (strcmp(tempLine[ONE], ".string") == 					0))
				{
					/*Check whether the label has been defined in a valid way, including 						checking whether the label has already been defined before.*/
					if(legal_label(tempLine[ZERO],rowNumber, &arraySymbols, 1) == 0)
					{
						add_node_to_arraySymbols(tempLine[ZERO], data, DC, &arraySymbols);
					}

					/*The label is invalid.*/
					else 
					{
						error_in_line(rowNumber);
						printf("The label is invalid.\n");
						errorFlag = 1;
					}
				}
			
				/*If the directive sentence is valid.*/
				if (is_directive_sentence_valid(line, &arraySymbols, rowNumber) == 0)
				{
					/*If there is no label in the sentence.*/
					if(is_data(line) == 0 || is_string(line) == 0)
					{
						/*Insert all the values of the directive sentence into the data 						array. For this we will send only the values themselves and 							remove the name of the directive from the sentence.*/
						insert_arrayData(withoutFirstWord, tempLine[ZERO], arrayData, 							&arraySymbols, rowNumber, &DC, tempLine);
					}

					/*If there is label in the sentence.*/
					if(is_data(line) == 1 || is_string(line) == 1)
					{
						/*Insert all the values of the directive sentence into the data 						array. For this we will send only the values themselves and 							remove the label and the name of the directive from the 						sentence.*/
						insert_arrayData(withoutTwotWords, tempLine[ONE], arrayData, 							&arraySymbols, rowNumber, &DC, tempLine);
					}
				}
			}/*End of if the sentence is a '.data' directive or a .'string' directive.*/
		

			/*If the sentence is a '.entry' directive.*/
			else if(is_entry(line) != -1)
			{
				if(is_entry(line) == 0)
				{
					/*Check the legality of the label.*/
					legal_label(tempLine[ONE], rowNumber, &arraySymbols, 0);
				}
				
				if(is_entry(line) == 1)
				{
					/*Check the legality of the label.*/
					legal_label(tempLine[TWO], rowNumber, &arraySymbols, 0);
				}
				
				/*If the label is already in the symbol table.*/
				if(is_entry(line) == 0 && is_in_symbol_table(tempLine[ONE], &arraySymbols) == 1)
				{ 
					/*If the label value has already been defined in this file as an extern 					label, we will print an error message.*/
					if(get_symbol_type(tempLine[ONE], &arraySymbols) == Extern)
					{
						error_in_line(rowNumber);
						printf("The entry expression is also defined in this file as extern.\n");
						errorFlag = 1;
					}
				}

				/*If the label is already in the symbol table.*/
				if(is_entry(line) == 1 && is_in_symbol_table(tempLine[TWO], &arraySymbols) == 1)
				{ 
					/*If the label value has already been defined in this file as an extern 					label, we will print an error message.*/
					if(get_symbol_type(tempLine[TWO], &arraySymbols) == Extern)
					{
						error_in_line(rowNumber);
						printf("The entry expression is also defined in this file as extern.\n");
						errorFlag = 1;
					}
				}

				/*If the symbol is not in the symbol table insert it.*/
				else if(is_entry(line) == 0 && is_in_symbol_table(tempLine[ONE], &arraySymbols) 				== 0)
				{
					add_node_to_arraySymbols(tempLine[ONE], entry, '\0', &arraySymbols);
				}

				/*If the symbol is not in the symbol table insert it.*/
				else if(is_entry(line) == 1 && is_in_symbol_table(tempLine[TWO], &arraySymbols) 				== 0)
				{
					add_node_to_arraySymbols(tempLine[ONE], entry, '\0', &arraySymbols);
				}

			}/*End of if the sentence is a '.entry' directive.*/


			/*If the sentence is a .'extern' directive.*/
			else if(is_extern(line) != -1)
			{ 
				if(is_extern(line) == 0)
				{
					/*Check the legality of the label.*/
					legal_label(tempLine[ONE], rowNumber, &arraySymbols, 0);
				}
				
				if(is_extern(line) == 1)
				{
					/*Check the legality of the label.*/
					legal_label(tempLine[TWO], rowNumber, &arraySymbols, 0);
				}

				/*If the label value has already been defined in this file as an entry label, we 					will print an error message.*/
				if(is_extern(line) == 0 && get_symbol_type(tempLine[ONE], &arraySymbols) == entry)
				{
					error_in_line(rowNumber);
					printf("The extern expression is also defined in this file as entry.\n");
					errorFlag = 1;
				}

				/*If the label value has already been defined in this file as an entry label, we 					will print an error message.*/
				else if(is_extern(line) == 1 && get_symbol_type(tempLine[TWO], &arraySymbols) == 					entry)
				{
					error_in_line(rowNumber);
					printf("The extern expression is also defined in this file as entry.\n");
					errorFlag = 1;
				}

				/*The extern is defined as the first word in the sentence and the label was not 				previously defined in the file.*/
				else if(is_extern(line) == 0 && is_in_symbol_table(tempLine[ONE], &arraySymbols) 					== 0)	
				{
					add_node_to_arraySymbols(tempLine[ONE], Extern, 0, &arraySymbols);
				}

				/*The extern is defined as the seconed word in the sentence and the label was not 					previously defined in the file.*/
				else if(is_extern(line) == 1 && is_in_symbol_table(tempLine[TWO], &arraySymbols) 					== 0)	
				{
					add_node_to_arraySymbols(tempLine[TWO], Extern, 0, &arraySymbols);
				}

				/*The extern is defined as the first word in the sentence and the label was 					previously defined in the file.*/
				else if(is_extern(line) == 0 && is_in_symbol_table(tempLine[ONE], &arraySymbols) 					== 1)	
				{
					/*Indicate in the symbol table that the label is of Extern type.*/
					change_symbol_type(tempLine[ONE], Extern, &arraySymbols);
				}

				/*The extern is defined as the seconed word in the sentence and the label was 					previously defined in the file.*/
				else if(is_extern(line) == 1 && is_in_symbol_table(tempLine[TWO], &arraySymbols) 					== 1)
				{
					/*Indicate in the symbol table that the label is of Extern type.*/
					change_symbol_type(tempLine[TWO], Extern, &arraySymbols);
				}

			}/*End of if the sentence is a .'extern' directive.*/



			/*If it is not an instruction sentence in which an action opration is defined, then there 				is an error (all other options have already been checked and if we have come this far 				there should necessarily be an instruction sentence).*/
			else if(is_instruction_sentence(line) == 1)
			{
				error_in_line(rowNumber);
				printf("Invalid syntax.\n");

				/*Checking whether there are illegal commas.*/
				check_legal_commas(line, rowNumber);

				errorFlag = 1;
			}

			/*If it is an instruction sentence.*/
			else if(is_instruction_sentence(line) == 0)
			{
				/*The instruction statement is correct and the operands are correct for the 					operation.*/
				if(valid_instruction_sentence(line, rowNumber, &arraySymbols) == 0)
				{
					/*If there is a label at the beginning of the sentence and it is not 						defined in a legal way.*/
					if(index_instruction(line) == 1 && legal_label(tempLine[ZERO],rowNumber, 						&arraySymbols, 1) == 1)
					{
						errorFlag = 1;
					}

					/*L represents the number of words occupied by the machine code of a 						given instruction.*/
					L = calculate_num_words(line, &arraySymbols, rowNumber);

					/*If there is a label at the beginning of the sentence and it is defined 						in a legal way.*/
					if(index_instruction(line) == 1 && errorFlag == 0)
					{
						int long binary[SIZE_BINARY_CODE];

						/*Insert the label into the symbol table.*/
						add_node_to_arraySymbols(tempLine[ZERO], code, IC + MEMORY_START, 							&arraySymbols);

						/*There are two operands in the statement.*/
						if(count_words(line) == INST_TWOOPERAND_WITHLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ONE], tempLine[TWO],
							tempLine[TARGET_OP_WITH_LABEL], &arraySymbols, binary, 								rowNumber);
						}

						/*There is one operand in the statement.*/
						else if(count_words(line) == INST_ONEOPERAND_WITHLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ONE], '\0', 
							tempLine[TWO], &arraySymbols, binary, rowNumber);
						}

						/*There are no operands in the statement.*/
						else if(count_words(line) == INST_NOOPERAND_WITHLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ONE], '\0', '\0', 								&arraySymbols, binary, rowNumber);
						}

						/*Insert the binary word we obtained for the instruction into the 							instruction set.*/
						insert_arrayInstructions(binary, arrayInstructions, &IC, 							&arraySymbols, tempLine);
					}

					else if(index_instruction(line) == 0)
					{
						int long binary[SIZE_BINARY_CODE]; 

						/*There are two operands in the statement.*/
						if(count_words(line) == INST_TWOOPERAND_NOLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ZERO], tempLine[ONE],
							tempLine[TWO], &arraySymbols, binary, rowNumber);
						}

						/*There is one operand in the statement.*/
						else if(count_words(line) == INST_ONEOPERAND_NOLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ZERO], '\0',
							tempLine[ONE], &arraySymbols, binary, rowNumber);
						}

						/*There are no operands in the statement.*/
						else if(count_words(line) == INST_NOOPERAND_NOLABEL)
						{
							/*Build the binary code of the first word of the 								instruction.*/
							binary_word_instruction(tempLine[ZERO], '\0', '\0', 								&arraySymbols, binary, rowNumber);
						}


						/*Insert the binary word we obtained for the instruction into the 							instruction set.*/
						insert_arrayInstructions(binary, arrayInstructions, &IC, 							&arraySymbols, tempLine);
					}

					IC = IC + L;
				}

				/*The instruction statement is invalid and the operands are invalid for the 					operation.*/
				else
				{
					errorFlag = 1;
				}

			}/*End if it is an instruction sentence.*/

			rowNumber++;
			free_array(tempLine);
			free(withoutFirstWord);
			free(withoutTwotWords);
		}

		else
		{
			rowNumber++;
			free_array(tempLine);
		}


	}/*End of read a line from the file each time until the end of the file.*/



	current = arraySymbols.head; 

	/*Go through the symbol table and add IC to each label of data or string.*/
	while (current != NULL)
	{
        	if (current->type == data)
		{
            		current->dataAddr += IC + MEMORY_START;
        	}

        	current = current->next;
    	}


	/*Continue to the second passage.*/
	errorRunSecond = run_second_pass(ofp, &IC, &arraySymbols, &ourExternList, arrayInstructions, arrayData, 	fileName);

	/*There were errors during the first or the second run.*/
	if(errorRunSecond == 1 || errorFlag == 1)
	{
		printf("Errors were detected in the file, the program is stopping now or it will continue to read the following files.\n");

		/*Release all assignments of the symbol array.*/
		free_arraySymbols(&arraySymbols);

		fclose(ofp);

		return;
	}
	
	/*Release all assignments of the symbol array.*/
	free_arraySymbols(&arraySymbols);


}

