#include "heder.h"


/*Second pass on the file.*/
int run_second_pass(FILE *ofp, unsigned int *IC, dataSymbols *arraySymbols, externList *ourExternList, long int arrayInstructions[SIZE_ARRAY][SIZE_BINARY_CODE], long int arrayData[SIZE_ARRAY][SIZE_BINARY_CODE], char *fileName)
{
	char **tempLine;
	int rowNumber = 1;
	int errorFlag = 0;

	/*Two arrays for holding the code word.*/
	long int binary[SIZE_BINARY_CODE];
	long int binary1[SIZE_BINARY_CODE];

	/*String for storing line that we read from file.*/
	char line[MAX_LEN_LINE];
	int arrayInstructionsSize = 0;
	int arrayDataSize = 0;


	*IC = 0;

	rewind(ofp);


	/*Read a line from the file each time until the end of the file.*/
	while (fgets(line, sizeof(line), ofp) != NULL) 
	{

		/*Put each word in a separate cell in the array to more easily access each data.*/
		tempLine = lineToArray(line);

		if(tempLine == NULL)
		{
			rowNumber++;
			continue;
		}

		/*An empty line, nothing to analyze.*/
		if(is_empty_sentence(line) == 0)
		{
			rowNumber++;
			free_array(tempLine);
			continue;
		}

		/*If the first field is not a reserved word then it is a symbol.*/
		if(is_reserved_words(tempLine[ZERO]) == 0)
		{
			/*If the label is of the extern type, it will be included in the extern list*/
			enter_extern(tempLine[ZERO], (*IC)+1, ourExternList, arraySymbols, rowNumber);

			/*If it is one of the following instructions, we will exit the loop and read the next 				line.*/
			if((is_data(line) == 0) || (is_string(line) == 0) || (is_extern(line) == 0))
			{
				rowNumber++;
				free_array(tempLine);
				continue;
			}

			else if(is_entry(line) == 0)
			{
				/*If the label is in the symbol table, change its type to entry.*/
				if(is_in_symbol_table(tempLine[TWO], arraySymbols) == 1)
				{
					change_symbol_type(tempLine[TWO], entry, arraySymbols);
				}

				/*There is a label that was defined as internal but was not actually defined in 				the file and therefore gave an error message.*/
				else 
				{
					error_in_line(rowNumber);
					printf("There is a label that was defined as internal but was not actually defined in the file.\n");
					errorFlag = 1;
				}
			}
		}/*If the first field is a symbol.*/
 
		/*If the first field is not a symbol.*/
		else if(is_reserved_words(tempLine[ZERO]) == 1)
		{
			/*If it is one of the following instructions, we will exit the loop and read the next 				line.*/
			if((is_data(line) == 0) || (is_string(line) == 0) || (is_extern(line) == 0))
			{
				rowNumber++;
				free_array(tempLine);
				continue;
			}

			else if(is_entry(line) == 0)
			{
				/*If the label is in the symbol table, change its type to entry.*/
				if(is_in_symbol_table(tempLine[ONE], arraySymbols) == 1)
				{
					change_symbol_type(tempLine[ONE], entry, arraySymbols);
				}

				/*There is a label that was defined as internal but was not actually defined in 				the file and therefore gave an error message.*/
				else 
				{
					error_in_line(rowNumber);
					printf("There is a label that was defined as internal but was not actually defined in the file.\n");
					errorFlag = 1;
				}
			}
		}/*If the first field is not a symbol.*/

		/*If the sentence is an instruction sentence type.*/
		if(is_instruction_sentence(line) == 0)
		{
			/*If the instruction command is in the first field in the sentence.*/
			if(index_instruction(line) == 0)
			{
				/*There are two operands in the statement.*/
				if(count_words(line) == INST_TWOOPERAND_NOLABEL)
				{		
					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ZERO], tempLine[ONE], 1, 0, 						arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n", tempLine[ONE]);
						errorFlag = 1;
					}

					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ZERO], tempLine[TWO], 0, 1, 						arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n", tempLine[TWO]);
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[ONE], rowNumber, arraySymbols) 						== 1)
					{
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[TWO], rowNumber, arraySymbols) 						== 1)
					{
						errorFlag = 1;
					}

					/*Both operands are registers.*/
					if(is_register(tempLine[ONE]) == 0 && is_register(tempLine[TWO]) == 0)
					{
						/*Build the code words for the two registers, the result should 						be only in array 'binary'.*/
						if(binary_operand_instruction(tempLine[ONE], tempLine[TWO], 							arraySymbols, rowNumber, &binary[SIZE_BINARY_CODE],
						&binary1[SIZE_BINARY_CODE]) == 0)
						{
							(*IC)++;

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*The code word was not successfully constructed.*/
						else 
						{
							error_in_line(rowNumber);
							printf("The code word was not constructed due to a syntax error.\n");
							errorFlag = 1;
						}
					}

					else
					{
						/*A code word was constructed.*/
						if(binary_operand_instruction(tempLine[ONE], '\0', arraySymbols, 							rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) 							== 0)
						{
							(*IC)++;

							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[ONE], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*Two code words were constructed.*/
						else if(binary_operand_instruction(tempLine[ONE], '\0', 						arraySymbols, rowNumber, &binary[SIZE_BINARY_CODE],
						&binary1[SIZE_BINARY_CODE]) == 2)
						{
							(*IC)++;						

							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[ONE], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the first code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);

							/*Insert the second code word into the 								arrayInstructions.*/
							insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						

						/*A code word was constructed.*/
						if(binary_operand_instruction('\0', tempLine[TWO], arraySymbols, 							rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) 							== 0)
						{
							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TWO], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*Two code words were constructed.*/
						else if(binary_operand_instruction('\0', tempLine[TWO], 						arraySymbols, rowNumber, &binary[SIZE_BINARY_CODE],
						&binary1[SIZE_BINARY_CODE]) == 2)
						{
							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TWO], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the first code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);

							/*Insert the second code word into the 								arrayInstructions.*/
							insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}
					}
				}

				/*If one operand exists.*/
				else if(count_words(line) == INST_ONEOPERAND_NOLABEL)
				{
					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ZERO], tempLine[ONE], 0, 1, 						arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n", tempLine[ONE]);
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[ONE], rowNumber, arraySymbols) 						== 1)
					{
						errorFlag = 1;
					}

					/*A code word was constructed.*/
					if(binary_operand_instruction('\0', tempLine[ONE], arraySymbols, 						rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 0)
					{
						(*IC)++;

						/*If the operand is of extern type, it will be included in the 							extern list*/
						enter_extern(tempLine[ONE], (*IC), ourExternList, arraySymbols, 						rowNumber);

						/*Insert the code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);
					}

					/*Two code words were constructed.*/
					else if(binary_operand_instruction('\0', tempLine[ONE], arraySymbols, 						rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 2)
					{
						(*IC)++;

						/*If the operand is of extern type, it will be included in the 							extern list*/
						enter_extern(tempLine[ONE], (*IC), ourExternList, arraySymbols, 						rowNumber);

						/*Insert the first code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);

						/*Insert the second code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);
					}
				}

				/*There are no operands so there is no need to add words.*/
				else
				{
					(*IC)++;
				}

			}/*End of if the instruction command is in the first field in the sentence.*/


			/*If the instruction command is in the second field in the sentence.*/
			else if(index_instruction(line) == 1)
			{
				/*There are two operands in the statement.*/
				if(count_words(line) == INST_TWOOPERAND_WITHLABEL)
				{
					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ONE], tempLine[TWO], 1, 0, 						arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n", tempLine[TWO]);
						errorFlag = 1;
					}

					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ONE], 
					tempLine[TARGET_OP_WITH_LABEL], 0, 1, arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n",
						tempLine[TARGET_OP_WITH_LABEL]);
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[TWO], rowNumber, arraySymbols) 						== 1)
					{
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[TARGET_OP_WITH_LABEL], 						rowNumber, arraySymbols) == 1)
					{
						errorFlag = 1;
					}

					/*Both operands are registers.*/
					if(is_register(tempLine[TWO]) == 0 &&
 					is_register(tempLine[TARGET_OP_WITH_LABEL])==0)
					{
						/*Build the code words for the two registers, the result should 						be only in array 'binary'.*/
						if(binary_operand_instruction(tempLine[TWO],
						tempLine[TARGET_OP_WITH_LABEL], arraySymbols, rowNumber,
						&binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 0)
						{
							(*IC)++;

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*The code word was not successfully constructed.*/
						else 
						{
							error_in_line(rowNumber);
							printf("The code word was not constructed due to a syntax error.\n");
							errorFlag = 1;
						}
					}

					else
					{
						/*A code word was constructed.*/
						if(binary_operand_instruction(tempLine[TWO], '\0', arraySymbols, 							rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) 							== 0)
						{
							(*IC)++;

							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TWO], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*Two code words were constructed.*/
						else if(binary_operand_instruction(tempLine[TWO], '\0', 						arraySymbols, rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 2)
						{
							(*IC)++;

							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TWO], (*IC), ourExternList, 								arraySymbols, rowNumber);

							/*Insert the first code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);

							/*Insert the second code word into the 								arrayInstructions.*/
							insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*A code word was constructed.*/
						if(binary_operand_instruction('\0', 
						tempLine[TARGET_OP_WITH_LABEL], arraySymbols, rowNumber,
						&binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 0)
						{
							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TARGET_OP_WITH_LABEL], (*IC), 								ourExternList, arraySymbols, rowNumber);

							/*Insert the code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

						/*Two code words were constructed.*/
						else if(binary_operand_instruction
						('\0', tempLine[TARGET_OP_WITH_LABEL], arraySymbols, rowNumber, 						&binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 2)
						{

							/*If the operand is of extern type, it will be included 							in the extern list*/
							enter_extern(tempLine[TARGET_OP_WITH_LABEL], (*IC), 								ourExternList, arraySymbols, rowNumber);

							/*Insert the first code word into the arrayInstructions.*/
							insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);

							/*Insert the second code word into the 								arrayInstructions.*/
							insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 								arrayInstructions, IC, arraySymbols, tempLine);
						}

					}
				}

				/*If one operand exists.*/
				else if(count_words(line) == INST_ONEOPERAND_WITHLABEL)
				{
					/*If the operand does not matches the operation.*/
					if(valid_operand_to_operation(tempLine[ONE], tempLine[TWO], 0, 1, 						arraySymbols, rowNumber) == 1)
					{
						error_in_line(rowNumber);
						printf("Invalid operand '%s' for operation.\n", tempLine[TWO]);
						errorFlag = 1;
					}

					/*If the operand does not fit any addressing method, it is invalid.*/
					if(is_operand_addressing_methods(tempLine[TWO], rowNumber, arraySymbols) 						== 1)
					{
						errorFlag = 1;
					}

					/*A code word was constructed.*/
					if(binary_operand_instruction('\0', tempLine[TWO], arraySymbols, 						rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 0)
					{
						(*IC)++;

						/*If the operand is of extern type, it will be included in the 							extern list*/
						enter_extern(tempLine[TWO], (*IC), ourExternList, arraySymbols, 						rowNumber);

						/*Insert the code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);

					}

					/*Two code words were constructed.*/
					else if(binary_operand_instruction('\0', tempLine[TWO], arraySymbols, 						rowNumber, &binary[SIZE_BINARY_CODE], &binary1[SIZE_BINARY_CODE]) == 2)
					{
						(*IC)++;

						/*If the operand is of extern type, it will be included in the 							extern list*/
						enter_extern(tempLine[TWO], (*IC), ourExternList, arraySymbols, 						rowNumber);

						/*Insert the first code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);

						/*Insert the second code word into the arrayInstructions.*/
						insert_arrayInstructions(&binary1[SIZE_BINARY_CODE], 							arrayInstructions, IC, arraySymbols, tempLine);

					}
				}

				/*There are no operands so there is no need to add words.*/
				else
				{
					(*IC)++;
				}

			}/*End of if the instruction command is in the second field in the sentence.*/

		}/*End of if the sentence is an instruction sentence type.*/

		rowNumber++;
		free_array(tempLine);

	}/*End of read a line from the file each time until the end of the file.*/

	/*Errors were found during the program.*/
	if(errorFlag  == 1)
	{
		return 1;
	}

	/*If there were no errors build the object file, the entry file and the extern file.*/
	else 
	{
		arrayInstructionsSize = return_size_arrayInstructions();
		arrayDataSize = return_size_arrayData();

		/*Build an object file.*/
		build_object(fileName, arrayInstructions, arrayData, ofp, arrayInstructionsSize, arrayDataSize, 		arraySymbols);

		/*Build an entry file.*/
		build_entries(arraySymbols, fileName);

		/*Build an extern file.*/
		build_externals(fileName, ofp, arraySymbols, ourExternList);

	}

	return 0;

}
