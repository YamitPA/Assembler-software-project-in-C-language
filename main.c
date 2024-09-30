#include "heder.h"

/*main function that activates the program.*/
int main(int argc, char *argv[])
{
	/*A pointer to the readable file .as we received from the user.*/
	FILE *ifp;

	/*A pointer to the file .am we will create.*/
	FILE *ofp;

	char *currentFileName;

	/*No source files entered.*/
	if(is_file_name_inserted(argc) == 1)
	{
		return 1;
	}

	/*Go through all the source files.*/
	while (--argc > 0)
	{
		currentFileName = malloc(FILENAME_MAX * sizeof(char));
		
		if(allocation_succeeded(currentFileName) == 1)
		{
			return 1;
		}


		/*We will copy the name of the file received from the user to our variable so as not to make 			changes to it.*/
		strcpy(currentFileName, argv[argc]);

		/*Add an extension to the file name.*/
		strcat(currentFileName, ".as");

		if((ifp = fopen(currentFileName, "r")) == NULL)
		{	
			printf("Cannot open file %s.as.\n\n", argv[argc]);
		}


		else 
		{
			/*Execute macros retirement.*/
			ofp = make_pre_assembler_file(ifp,currentFileName);
			
			/*The macro retirement file was not built.*/
			if(ofp == NULL)
			{
				printf("The %s.am file was not created due to errors in the source file.\nThe program will stop now.\n\n", argv[argc]);
				fclose(ifp);
				exit(1);
			}

			/*The readers retirement was successful.*/
			else
			{
				/*Start a first pass on the source file.*/
				run_first_pass(ofp, argv[argc]);

				fclose(ifp);

				/*If the first pass is done successfully, the second pass will continue as well, 					followed by the building of the files.*/
			}
	
		}
	}/*All source files have been read.*/

	free(currentFileName);

	return 0;
}




/*A function that checks if the name of a file has been entered, if not we will return 1.*/
int is_file_name_inserted(int argc)
{
	/*No file name inserted.*/
	if (argc == 1)
	{
		printf("No file name inserted.\n");
		return 1;
	}

	return 0;
}


int allocation_succeeded (char *alloc)
{
	/*Memory allocation failed.*/
        if (alloc == NULL)
	{
            	printf("Error: Memory allocation for %s failed.\n", alloc);
            	return 1;
        }

	/*Memory allocation succeeded.*/
	return 0;
}

