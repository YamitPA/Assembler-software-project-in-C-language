#include <stdio.h>
#include <string.h>

#define MAX_MACRO_NAME_LENGTH 20 /*Maximum length of the macro name.*/
#define OPERATION_SIZE 4 /*The size of the operation keyword*/
#define LABEL_LENGTH 32 /*Maximum length of a label.*/



/*This file contains structures for storing information about assembly instructions, labels, symbols and binary representation.*/






/*A structure that holds all existing addressing methods. The value of each method can be 0 or 1 which tells us if the address method is valid for any operation called from the file.*/
typedef struct {

    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int regist : 1;

} AddressMethodsOptions;




/*A data structure that holds the 16 operation codes.*/
typedef struct{

	/*the keyword for the operation name.*/
    	char keyword[OPERATION_SIZE];

	/*A variable that will store the binary code of the operation.*/
    	unsigned int binary_code;

	/*address of the source operand.*/
	AddressMethodsOptions src;

	/*address of the destination operand.*/
	AddressMethodsOptions des;

}operation;


/*Different types the symbol can have.*/
typedef enum {
	define,
	Extern,
	entry,
	code,
	data, 
	non
}SymbolType;



/*Each symbol in line will be stored in an array, each cell of which is a symbol type.*/
typedef struct nodeDataSymbols{

	char name[LABEL_LENGTH];

	SymbolType type;

	/*where the current node points to.*/
	struct nodeDataSymbols *next;

	/*The address of the symbol.*/ 
	int dataAddr; 

} nodeDataSymbols;




/*Setting a new list header.*/
typedef struct
{
	nodeDataSymbols *head; 
}dataSymbols;


/*A structure that holds a node of a macro.*/
typedef struct nodeMacro
{
	/*The name of the macro.*/
	char name[MAX_MACRO_NAME_LENGTH];

	/*An array that contains the lines of the macro.*/
	char **lines;

	/*where the current node points to.*/
	struct nodeMacro *next;

	/*The number of rows in the macro.*/
	int linesNum;

} nodeMacro;




/*Setting a new list header.*/
typedef struct
{

	nodeMacro *head;

}macroList;





/*A structure that holds a node of a extern.*/
typedef struct nodeExtern
{
	/*The name of the label.*/
	char name[LABEL_LENGTH];

	/*where the current node points to.*/
	struct nodeExtern *next;

	/*The address of the extern label.*/
	int address;

} nodeExtern;



/*Setting a new list header.*/
typedef struct
{

	nodeExtern *head;

}externList;



