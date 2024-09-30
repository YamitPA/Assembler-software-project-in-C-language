#include "heder.h"




void build_object(char *fileName, long int arrayInstructions[SIZE_ARRAY][SIZE_BINARY_CODE], long int arrayData[SIZE_ARRAY][SIZE_BINARY_CODE], FILE *ofp, int sizeArrayInstructions, int sizeArrayData , dataSymbols *arraySymbols)
{
	FILE *objectFile;
	char *nameFileOb;
	char *base4_instruction;
	char *withZeros;
	unsigned int IC = MEMORY_START;
	int i = 0;
	int j = 0;

	nameFileOb = malloc(FILENAME_MAX * sizeof(char));
		
	if(allocation_succeeded(nameFileOb) == 1)
	{
		return;
	}

	/*We will copy the name of the file received from the user to our variable so as not to make changes 		to it.*/
	strcpy(nameFileOb, fileName);

	/*The name of the file we received is with the .am extension, so we will remove the extension.*/
	nameFileOb[strlen(fileName) - SUFFIX_LENGTH] = '\0';

	/*Add an extension to the file name.*/
	strcat(nameFileOb, ".ob");

	objectFile = fopen(nameFileOb, "w");

	if(objectFile == NULL)
	{	
		printf("Cannot open file %s.ob.\n\n", fileName);
		free(nameFileOb);
		return;
	} 

	/*Print the file header.*/
	fprintf(objectFile, "%d %d\n", sizeArrayInstructions, sizeArrayData);

	/*Print the arrayInstructions according to the appropriate format.*/
	while(i < sizeArrayInstructions)
	{
		withZeros = add_leading_zeros(IC, arraySymbols);

		/*Print the address in the appropriate format.*/
		fprintf(objectFile,"%s ", withZeros);

		base4_instruction = binary_to_base4(arrayInstructions[i]);

		/*Print the contents of the encrypted base 4 word.*/
		fprintf(objectFile, "%s\n", base4_instruction);

		free(withZeros);
		free(base4_instruction);

		IC++;
		i++;
	}

	/*Print the arrayData according to the appropriate format.*/
	while(j < sizeArrayData)
	{
		withZeros = add_leading_zeros(IC, arraySymbols);

		/*Print the address in the appropriate format.*/
		fprintf(objectFile, "%s ", withZeros);

		base4_instruction = binary_to_base4(arrayData[j]);

		/*Print the contents of the encrypted base 4 word.*/
		fprintf(objectFile, "%s\n", base4_instruction);

		free(withZeros);
		free(base4_instruction);

		IC++;
		j++;
	}

	fclose(objectFile);
	free(nameFileOb);

}



/*A function that returns how many cells in an array are filled.*/
int size_array(long int array[][SIZE_BINARY_CODE])
{
	int count = 0;
	int i;

	/*Go through the entire array.*/
	for (i = 0; i < SIZE_ARRAY; ++i)
	{
		if (array[i][0] != '\0')
		{
        		count++;
		}
    	}

	return count;

}




/*A function that receives the address and returns it in a format suitable for printing with a leading 0.*/
char* add_leading_zeros(int num, dataSymbols *arraySymbols)
{
	char* numberStr = malloc(ADDR_PRINT_FORMAT + 1);

	if (numberStr == NULL)
	{
		printf("Error: Memory allocation for numberStr failed.\n");
		free_arraySymbols(arraySymbols);
		exit(1);
	}

	/*There is exactly the number of digits that should be in the number, so we will return it.*/
	if(count_digits(num) >= ADDR_PRINT_FORMAT)
	{
		sprintf(numberStr, "%d", num);
		return numberStr;	
	}

	/*There are fewer than the number of digits that should be in the number.*/
	else
	{
		/*We must add only one 0 at the beginning, since the address inevitably starts with 100, which 			has 3 digits, so if there are less than four, there are exactly 3.*/

		/*Convert the number into a string and add 0 in the beginning.*/
		sprintf(numberStr, "0%d", num);

		/*Return the string.*/
		return numberStr;	
	}

}





/*A function that returns the number of digits present in some number.*/
int count_digits(int num)
{
	int count = 0;

	/*Go over the number as long as it is not zero.*/
	while (num != 0)
	{
		/*Extracts the digit that is in the lowest digit.*/
        	num /= 10;

		/*Updating the digit counter.*/
        	count++;
    	}

    	return count;
}



/*A function to convert a binary number to encrypted base 4.*/
char* binary_to_base4(long int binary[])
{
	/* Creating an empty string with the size of the number of bits for encrypted base 4 format plus 1 for 		'\0'. */
    	char* base4 = malloc((SIZE_4_ENCRYPTED + 1) * sizeof(char));

	int decimal = 0;
    	int base = 1;
	int i;

	if (base4 == NULL)
	{
        	printf("Memory allocation failed\n");
        	return NULL;
    	}

	/*Converting binary number to decimal.*/
	for (i = SIZE_BINARY_CODE - 1; i >= 0; i--)
	{
        	decimal = decimal + binary[i] * base;
        	base = base * 2;
    	}

	/*Converting decimal number to encrypted base 4.*/
	for (i = 0; i < SIZE_4_ENCRYPTED; i++)
	{
		/*Verify if the binary array has enough bits to represent a base4 digit.*/
        	if (decimal == 0 && i >= SIZE_BINARY_CODE)
        	{
			/*No more bits to convert.*/
			 break; 
		}

        	switch(decimal % 4)
		{
            		case 0:
                		base4[SIZE_4_ENCRYPTED - 1 - i] = '*';
                		break;

            		case 1:
                		base4[SIZE_4_ENCRYPTED - 1 - i] = '#';
                		break;

            		case 2:
                		base4[SIZE_4_ENCRYPTED - 1 - i] = '%';
                		break;

            		case 3:
                		base4[SIZE_4_ENCRYPTED - 1 - i] = '!';
                		break;
        	}

		decimal = decimal / 4;

    	}

	/*Add a string termination marker.*/
	base4[SIZE_4_ENCRYPTED] = '\0';

	/*Returning the base 4 encrypted string.*/
	return base4;

}
