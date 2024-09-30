#include "heder.h"



/*The operations array contains one record for each operation. Each operation is translated directly into a 	binary base. Each record has four values: the operation keyword (in a string), a binary code (in a decimal 	number) and according to the table in the sponsor we will fill in some legal methods for each operation.*/
static operation operations[] = {
    	{"mov", 00000000, {1,1,1,1}, {0,1,1,1}},
    	{"cmp", 00000001, {1,1,1,1}, {1,1,1,1}},
    	{"add", 00000010, {1,1,1,1}, {0,1,1,1}},
    	{"sub", 00000011, {1,1,1,1}, {0,1,1,1}},
    	{"not", 00000100, {0,0,0,0}, {0,1,1,1}},
    	{"clr", 00000101, {0,0,0,0}, {0,1,1,1}},
    	{"lea", 00000110, {0,1,1,0}, {0,1,1,1}},
    	{"inc", 00000111, {0,0,0,0}, {0,1,1,1}},
    	{"dec", 00001000, {0,0,0,0}, {0,1,1,1}},
    	{"jmp", 00001001, {0,0,0,0}, {0,1,0,1}},
    	{"bne", 00001010, {0,0,0,0}, {0,1,0,1}},
    	{"red", 00001011, {0,0,0,0}, {0,1,1,1}},
    	{"prn", 00001100, {0,0,0,0}, {1,1,1,1}},
    	{"jsr", 00001101, {0,0,0,0}, {0,1,0,1}},
    	{"rts", 00001110, {0,0,0,0}, {0,0,0,0}},
    	{"hlt", 00001111, {0,0,0,0}, {0,0,0,0}},
};



/*A function that searches for an action by name.*/
operation* find_operation_by_name(char *nameop, int rowNumber)
{
	int i;

	/*Explanation of the loop conditions: The size of the array operations is measured in bytes, while the size 		of one record of type operation is measured in structures. This division gives us the number of records in 		the array.*/
	for (i = 0; i < sizeof(operations) / sizeof(operation); i++)
	{
        	if (strcmp(operations[i].keyword, nameop) == 0)
		{
            		return &operations[i];
        	}
    	}

	/*If we did not find the name we will print an error message and return null.*/
	error_in_line(rowNumber);
	printf("Operation name: '%s', does not exist.\n", nameop);
    	return NULL;
}



/*A function that accepts the name of an operation and returns at which index in the array of operations the name of the operation is located. If the operation name does not exist, it will return 1.*/
int get_op_index(char *op)
{
	int i = 0;

	while (i < SIZE_OPERNAME)
	{
        	if (strcmp(operations[i].keyword, op) == 0)
            	return i;
        	i++;
    	}

	/*the operation name does not exist.*/
    	return -1;
}




/*A function that returns the binary code of an action by name.*/
unsigned int get_operation_binary_code(char *name, int rowNumber)
{
	operation *oper; 

	/*Find the desired action name.*/
	oper = find_operation_by_name(name, rowNumber);

	/*If it does not exist we will print an error message and return a negative number because no operation name 		is represented by a negative number.*/
	if (oper == NULL)
	{
		error_in_line(rowNumber);
		printf("Error: action name: '%s', does not exist.\n", name);
        	return -1;
    	}

	/*If it exists we will return the binary value of the operation code name.*/
	return oper->binary_code;
}



/*The function checks whether the operation is of the type that receives two operands. If so, it will return 0 otherwise it will return 1.*/
int two_operands_grop (char *op)
{
	int i;

	/*An array containing all the names of operations that receive two operands.*/
	char *twoOperands[TWO_OPERANDS_GROP] = {"mov", "cmp", "add", "sub", "lea"};

	/*We will loop over the array and see if the operation name is one of those that receive two operands.*/
	for(i = 0; i < TWO_OPERANDS_GROP; i++)
	{
		if(strcmp(op, twoOperands[i]) == 0)
		{
			return 0;
		}
	}

	return 1;

}






/*The function checks whether the operation is of the type that receives one operand. If so, it will return 0 otherwise it will return 1.*/
int one_operands_grop(char *op)
{
	int i;

	/*An array containing all the names of operations that receive one operand.*/
	char *oneOperands[ONE_OPERANDS_GROP] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};

	/*We will loop over the array and see if the operation name is one of those that receive one operand.*/
	for(i = 0; i < ONE_OPERANDS_GROP; i++)
	{
		if(strcmp(op, oneOperands[i]) == 0)
		{
			return 0;
		}
	}

	return 1;

}





/*The function checks whether the operation is of the type that receives no operands. If so, it will return 0 otherwise it will return 1.*/
int no_operands_grop (char *op)
{
	int i;

	/*An array containing all the names of operations that receive no operands.*/
	char *noOperands[NO_OPERANDS_GROP] = {"rts", "hlt"};

	/*We will loop over the array and see if the operation name is one of those that receive no operands.*/
	for(i = 0; i < NO_OPERANDS_GROP; i++)
	{
		if(strcmp(op, noOperands[i]) == 0)
		{
			return 0;
		}
	}

	return 1;

}



/*A function that accepts a statement and returns 0 if there are two operands separated by a comma and 1 if not.*/
int is_two_operands(char *sentence, int rowNumber)
{
	int word = 0;
	int i = 0;

	/*Iterate over each character in the sentence.*/
    	while(sentence[i] != '\0')
    	{
        	/*If the character is a comma or space.*/
        	if ((sentence[i] == ',' || isspace(sentence[i])) && sentence[i] != '\0')
        	{
			/*Move forward until reach the next word.*/
			while((isspace(sentence[i]) || sentence[i] == ',') && sentence[i] != '\0')
			{
				i++;
			}
        	}

		else
		{
			/*Move to the end of the word.*/
			while(!isspace(sentence[i]) && sentence[i] != ',' && sentence[i] != '\0')
			{
				i++;
			}

			/*Move forward until reach the next word or till '\0'.*/
			while((isspace(sentence[i]) || sentence[i] == ',') && sentence[i] != '\0')
			{
				i++;
			}

		
			/*Add a word to the count.*/
			word++;
			
		}
    	}

	/*Check if there are exactly two words.*/ 
	if(word == TWO)
	{
		return 0;
	}

	/*The number of operands is not two.*/
	error_in_line(rowNumber);
	printf("Note that the operation should take two operands.\n");
	return 1;
}





/*A function that checks whether the entered operands are of the type of immediate addressing method, if so it returns 0; else it returns 1.*/
int is_immediate(char *op)
{
	remove_blanks(op);

	if(op[0] == '#')
	{
		/*This is an immediate intervention method.*/
        	return 0;
    	}
    
    	else
    	{
        	/*This is not an immediate intervention method.*/
        	return 1;
    	}
}




/*A function that checks whether the entered operands are of the type of direct addressing method. If there is a label that has not yet been defined, it will be inserted into the symbol table.*/
int is_direct(char *op, dataSymbols *arraySymbols, int rowNumber)
{
	/*If found in the symbol table.*/
	if(is_in_symbol_table(op, arraySymbols) == 1)
        {
		if(get_symbol_type(op, arraySymbols) == data || get_symbol_type(op, arraySymbols) == code || 			get_symbol_type(op, arraySymbols) == Extern || get_symbol_type(op, arraySymbols) == entry) 
		{
                	/*This is a direct address method.*/
                	return 0;
        	}
	}
    
        /*If it is not a fixed index or register addressing method, and if we did not find the operand as a label 		in the symbol table, it means that it is a label that has not yet been defined. Therefore we will enter 	it in the symbol table and later we will complete the details when the label is defined.*/
        if (is_fixed_index(op, rowNumber) == 1)
        {
                if(is_register(op) == 1)
                {
			if(is_in_symbol_table(op, arraySymbols) == 0)
			{
                      		add_node_to_arraySymbols(op, non, '\0', arraySymbols); 
                      		return 0;
			}
                }
        }

        /*This is not a direct address method.*/
        return 1;
}



/*A function that checks whether the operands are of fixed index addressing type.*/
int is_fixed_index(char *op, int rowNumber)
{
        int openingCount = 0;
        int closingCount = 0;

        while (*op)
        {
		if (*op == '[')
           	{
                	openingCount++;
            	}

            	else if (*op == ']')
            	{
                	closingCount++;
            	}
        
            	op++;
        }
    

        if(closingCount == 0 && openingCount == 0)
        {
            /*This is not a fixed index addressing method.*/
            return 1;
        }
    
        /*Check for '[' without a matching ']'.*/
        if(closingCount < openingCount)
        {
		error_in_line(rowNumber);
            	printf("Error: Found '[' without matching ']'.\n");
            	return 1;
        }

           
        if (closingCount == openingCount)
        {
            /*This is a fixed index addressing method.*/
            return 0;
        }

	return 1;
}




/*A function that checks whether the operands are of register addressing type.*/
int is_register(char *op)
{
        int i;
        
        /*An array containing all register names.*/
        char *regist[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
        
        /*We will go through the entire array and see if the operand is of register type.*/
        for (i = 0; i < NUM_REGISTERS ; i++)
        {
                /*The operand is of register addressing type.*/
                if(strcmp(op,regist[i]) == 0)
                {
                        return 0;
                }
        }
        
        /*The operand is not of register addressing type.*/
        return 1;
}





/*A function that turns on a bit indicating which addressing method the operand receives.*/
AddressMethodsOptions turns_method (char *op, dataSymbols *arraySymbols, int rowNumber)
{
        AddressMethodsOptions address = {0,0,0,0};
        
        /*Checking if the operand is in an immediate addressing method.*/
        if(is_immediate(op) == 0)
        {
                /*Light a chip in the first place.*/
                address.immediate = 1;
        }
        
        /*Checking if the operand is in a direct addressing method.*/
        else if(is_direct(op, arraySymbols, rowNumber) == 0)
        {
                /*Light a chip in the second place.*/
                address.direct = 1;
        }
        
         /*Checking if the operand is in a fixed index addressing method.*/
        else if(is_fixed_index(op, rowNumber) == 0)
        {
                /*Light a chip in the third place.*/
                address.index = 1;
        }
        
        else if(is_register(op) == 0)
        {
                /*Light a chip in the fourth place.*/
                address.regist = 1;
        }
        
        return address;
   
}





/*A function that checks whether the addressing method is appropriate and legal for the operation. If so it's returns 0 and 1 if it is unvalid.*/
int valid_operand_to_operation(char *operation, char *operand, int src, int des, dataSymbols *arraySymbols, int rowNumber)
{
	AddressMethodsOptions addressop;
	int indexOp;

        /*Find the addressing method of the operand.*/
        addressop = turns_method(operand, arraySymbols, rowNumber);

	indexOp = get_op_index(operation);

        if(indexOp == -1)
        {
                error_in_line(rowNumber);
                printf ("The operation name is unvalid.\n");
		return 1;
        }
        
        
        if(src == 1)
        {
                if (((operations[indexOp].src.immediate & addressop.immediate) != 0) ||
            	((operations[indexOp].src.direct & addressop.direct) != 0) ||
            	((operations[indexOp].src.index & addressop.index) != 0) ||
            	((operations[indexOp].src.regist & addressop.regist) != 0))
                {
                    /*The address method for the operation is valid.*/
                    return 0;
                }
                
                
                else
		{
			/*The addressing method is invalid.*/
			return 1;
		}
        }
        
        else if(des == 1)
        {
                if (((operations[indexOp].des.immediate & addressop.immediate) != 0) ||
            	((operations[indexOp].des.direct & addressop.direct) != 0) ||
            	((operations[indexOp].des.index & addressop.index) != 0) ||
            	((operations[indexOp].des.regist & addressop.regist) != 0))
                {
                    /*The address method for the operation is valid.*/
                    return 0;
                }
                

                else
		{
			/*The addressing method is invalid.*/
			return 1;
		}
        }

	return 1;
}




/*The function receives an instruction statement and checks whether the operands are suitable and valid for the operation. It returns 0 if the sentece is valid and 1 if not.*/
int valid_instruction_sentence(char *line, int rowNumber, dataSymbols *arraySymbols)
{
	char **tempLine;
	char *withoutFirstWord;
	char *withoutTwotWords; 
	int errorFlag = 0;

	/*We will put each word in a separate cell in the array to more easily access each data.*/
	tempLine = lineToArray(line);

	/*Check if memory allocation was successful.*/
	if (tempLine == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}

	/*Remove the data instruction for convenience for the following functions.*/
	withoutFirstWord = remove_first_word(line);

	/*The operation is the first word in the sentence.*/
	if(index_instruction(line) == 0)
	{
		/*Checking whether there is a comma after the instruction.*/
		if(check_comma_between(line,rowNumber) == 1)
		{
			printf("Unnecessary comma after the instruction.\n");
			errorFlag = 1;
		}

		/*Check if a comma is missing.*/
		if(missing_comma(withoutFirstWord, rowNumber) == 1)
		{
			errorFlag = 1;
		}

		/*Check whether there are too many commas or there are no commas.*/
		if(check_legal_commas(line, rowNumber) == 1)
		{
			errorFlag = 1;
		}

		/*There are no operands in the statement.*/
		if(count_words(line) == INST_NOOPERAND_NOLABEL)
		{
			/*The operation should not receive operands.*/
			if(no_operands_grop(tempLine[ZERO]) == 1)
			{
				error_in_line(rowNumber);
				printf("Operands must be inserted for this operation.\n");
				errorFlag = 1;
			}
		}

		/*There is one operand in the statement.*/
		else if(count_words(line) == INST_ONEOPERAND_NOLABEL)
		{
			/*If the operation is of a type that does not accept one operand.*/
			if(one_operands_grop(tempLine[ZERO]) == 1)
			{
				error_in_line(rowNumber);
				printf("This operation should take one operand.\n");
				errorFlag = 1;
			}

		}

		/*There are two operands in the statement.*/
		if(count_words(line) == INST_TWOOPERAND_NOLABEL)
		{
			if(is_two_operands(withoutFirstWord, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*If the inserted operand is of a type that should not receive two operands.*/
			if(two_operands_grop(tempLine[ZERO]) == 1)
			{
				error_in_line(rowNumber);
				printf("This operation should not have two operands.\n");
				errorFlag = 1;
			}
		}

		/*If there are more than two operands.*/
		if(count_words(line) > INST_TWOOPERAND_NOLABEL)
		{
			error_in_line(rowNumber);
			printf("Too many operands entered.\n");
			errorFlag = 1;
		}

	}

	/*The operation is the second word in the sentence.*/
	if(index_instruction(line) == 1)
	{
		/*Removing the label and the data instruction for convenience for the following functions.*/
		withoutTwotWords = remove_two_words(line);

		/*Checking whether there is a comma after the label.*/
		if(check_comma_between(line, rowNumber) == 1)
		{
			printf("Unnecessary comma after the label.\n");
			errorFlag = 1;
		}

		/*Checking whether there is a comma after the instruction.*/
		if(check_comma_between(withoutFirstWord,rowNumber) == 1)
		{
			printf("Unnecessary comma after the instruction.\n");
			errorFlag = 1;
		}

		/*Check if a comma is missing.*/
		if(missing_comma(withoutTwotWords, rowNumber) == 1)
		{
			errorFlag = 1;
		}

		/*Check whether there are too many commas or there are no commas.*/
		if(check_legal_commas(line, rowNumber) == 1)
		{
			errorFlag = 1;
		}

		/*There are no operands in the statement.*/
		if(count_words(line) == INST_NOOPERAND_WITHLABEL)
		{
			/*The operation should not receive operands.*/
			if(no_operands_grop(tempLine[ONE]) == 1)
			{
				error_in_line(rowNumber);
				printf("Operands must be inserted for this operation.\n");
				errorFlag = 1;
			}
		}

		/*There is one operand in the statement.*/
		if(count_words(line) == INST_ONEOPERAND_WITHLABEL)
		{
			/*The operation should not receive operands.*/
			if(one_operands_grop(tempLine[ONE]) == 1)
			{
				error_in_line(rowNumber);
				printf("This operation should take one operand.\n");
				errorFlag = 1;
			}

		}
		
		/*There are two operands in the statement.*/
		if(count_words(line) == INST_TWOOPERAND_WITHLABEL)
		{
			if(is_two_operands(withoutTwotWords, rowNumber) == 1)
			{
				errorFlag = 1;
			}

			/*If the inserted operand is of a type that should not receive two operands.*/
			if(two_operands_grop(tempLine[ONE]) == 1)
			{
				error_in_line(rowNumber);
				printf("This operation should not have two operands.\n");
				errorFlag = 1;
			}
		}

		/*If there are more than two operands.*/
		if(count_words(line) > INST_TWOOPERAND_WITHLABEL)
		{
			error_in_line(rowNumber);
			printf("Too many operands entered.\n");
			errorFlag = 1;
		}

		free(withoutTwotWords);
	}

	free_array(tempLine);
	free(withoutFirstWord);
	return errorFlag;
}


/*A function that prints an error if the operand is not one of the addressing methods.*/
int is_operand_addressing_methods(char *operand,  int rowNumber, dataSymbols *arraySymbols)
{
	int methods = 0;

	/*If the operand is of an immediate operand type, we will change the flag that will indicate that the 		operand has an addressing method.*/
	if(is_immediate(operand) == 0)
	{
		methods = 1;
	}

	/*If the operand is of a direct operand type, we will change the flag that will indicate that the 		operand has an addressing method.*/
	if(is_direct(operand, arraySymbols, rowNumber) == 0)
	{
		methods = 1;
	}

	/*If the operand is of a fixed index operand type, we will change the flag that will indicate that the 		operand has an addressing method.*/
	if(is_fixed_index(operand, rowNumber) == 0)
	{
		methods = 1;
	}

	/*If the operand is of a register operand type, we will change the flag that will indicate that the 		operand has an addressing method.*/
	if(is_register(operand) == 0)
	{
		methods = 1;
	}


	/*No addressing method is suitable for the operand, therefore it is invalid and we will issue an error 		message.*/
	if(methods == 0)
	{
		error_in_line(rowNumber);
		printf("the operand '%s' does not fit any addressing method and is therefore invalid.\n", 			operand);

		return 1;
	}

	return 0;


}
