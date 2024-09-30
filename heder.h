#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "dataStructures.h"




#define MAX_LEN_LINE 81 /*Legal length of line.*/
#define ACTUAL_LEN_LINE 90 /*Exceeding line length.*/
#define MAX_MACRO_NAME_LENGTH 20 /*Maximum length of the macro name.*/
#define CHAR_AT_MCR 3 /*The number of characters in the word mcr.*/
#define CHAR_AT_ENDMCR 6 /*The number of characters in the word endmcr.*/
#define NUM_CMD_WORDS_IN_LINE 2 /*The amount of words allowed in the cmr line*/
#define FIRST_ALLOCATE 1 /*Initial array size*/
#define MAX_WORDS 4096 /*The possible word limit.*/
#define DEFINE_NAME_IN_ARRAY 1 /*When a define command line is displayed inside an array, the name of the define will be at index 1 in the array.*/ 
#define DEFINE_EQUAL_SING 2 /*When a define command line is displayed inside an array, the equal sing of the define will be at index 2 in the array.*/
#define DEFINE_NUMBER 3 /*When a valid define row is inserted into the array, index number 3 will have the number that the define is set to be.*/
#define DEFINE_EMPTY_VALUE 4 /*When a define command line is displayed inside an array, in index 4 there should be '\0'.*/ 
#define NUM_RESERVED_WORDS 29 /*The number of reserved words in the language.*/
#define NUM_DIRECTIVE_COMMANS 4 /*The number of directive that can be.*/
#define ZERO 0 /*for convenient use.*/
#define ONE 1 /*for convenient use.*/
#define TWO 2 /*for convenient use.*/
#define SIZE_OPERNAME 16 /*The number of aperation.*/
#define LABEL_LENGTH 32 /*Maximum length of a label.*/
#define WORDS_STRING_WITHOUT_LABEL 2 /*The number of words that will be in a directive of type string if it has no label.*/
#define WORDS_STRING_WITH_LABEL 3 /*The number of words that will be in a directive of type string if it has label.*/
#define WORDS_ENTRY_EXTERN 2 /*The number of words that will be in a directive of type entry.*/
#define WORDS_ENTRY_EXTERN_LABEL 3 /*The number of words that will be in a directive of type entry when ther is a label in the bigging.*/
#define TWO_OPERANDS_GROP 5 /*The number of operations that receive two operands.*/
#define ONE_OPERANDS_GROP 9 /*The number of operations that receive one operand.*/
#define NO_OPERANDS_GROP 2  /*The number of operations that receive no operands.*/
#define ONE_OPERAND_LABEL 3 /*The number of words that should be in an operation statement that accepts one operand with a label definition.*/
#define SOURCE_OP_WITH_LABEL 2 /*The therd place (index 2) is where the target operand appears if there is a label.*/
#define TARGET_OP_WITH_LABEL 3 /*The fourth place (index 3) is where the target operand appears if there is a label.*/
#define NUM_REGISTERS 8 /*The number of registers.*/
#define MEMORY_START 100 /*The initial value of the instruction counter.*/
#define SIZE_ARRAY 1000 /*The size of the arrays arrayInstructions and arrayData.*/
#define SIZE_BINARY_CODE 14 /*Binary word size.*/
#define INST_NOOPERAND_NOLABEL 1 /*The number of words in an instruction statement if no label and no operands at all.*/
#define INST_ONEOPERAND_NOLABEL 2 /*The number of words in an instruction statement if there is no label and there is one operand.*/
#define INST_TWOOPERAND_NOLABEL 3 /*The number of words in an instruction statement if there is no label and there are two operands.*/
#define INST_NOOPERAND_WITHLABEL 2 /*The number of words in an instruction statement if there is a label and no operands at all.*/
#define INST_ONEOPERAND_WITHLABEL 3 /*The number of words in an instruction statement if there is a label and there is one operand.*/
#define INST_TWOOPERAND_WITHLABEL 4 /*The number of words in an instruction statement if there is a label and there are two operands.*/
#define WORDS_FIXED_INDEX 2 /*The number of words that the fixed index addressing method adds.*/
#define BIT_FOR_ARE 2 /*The number of bits needed for ARE*/
#define ADDR_PRINT_FORMAT 4 /*The number of digits that should be in the object file address print format.*/
#define SIZE_4_ENCRYPTED 7 /*The number of bits needed to print a number in encrypted base 4 format.*/ 
#define SUFFIX_LENGTH 3 /*File extension length.*/




/*The list where we will save all the macros.*/
macroList ourMacroList;

/*The list where we will save all the extern label.*/
externList ourExternList;




/*Functions from maim.*/
int is_file_name_inserted(int argc);
int allocation_succeeded (char *alloc);

/*Functions from preAssembler.*/
FILE* make_pre_assembler_file(FILE *ifp, char *currentFileName);
void remove_file(char *filename);

/*Functions from macro.*/
nodeMacro* find_the_node_macro(macroList *ourMacroList, char *nameNode);
void initNodeMacro(nodeMacro *newNode);
void addNodeToMacroList(macroList *ourMacroList, char *name);
void addLineToNodeMacro(nodeMacro *currentNode, char *line);

/*Functions from input.*/
char** lineToArray(char *line);
void error_in_line(int rownum);
int is_empty_sentence(char *line);
int starts_with_semicolon(char *line, dataSymbols *arraySymbols, char **arrayLine);
int index_directive (char *line); 
int is_data(char *line);
int is_string(char *line);
int is_entry(char *line);
int is_extern(char *line);
int is_instruction_sentence(char *line);
int index_instruction (char *line);
int is_define_sentence(char *line);
char* builde_new_file_name(char *string, char *name); 
int is_line_size_correct(char *line, int rowNumber);
void remove_blanks(char *line);
int legal_mcr_name(char *line);
int missing_comma(char *line, int rowNumber);
int check_legal_commas(char *line, int rowNumber);
int check_comma_between(char *sentence, int rowNumber);
char* remove_first_word(char *line);
char* remove_two_words(char *line);
int are_all_numbers(char *input, int rowNumber, dataSymbols *arraySymbols);
int is_valid_number(char *number, int rowNumber);
int is_valid_string(char *sentence, int rowNumber);
int legal_label(char* label, int rowNumber, dataSymbols *arraySymbols, int isDefinition);
int count_words(char *sentence);
int has_commas(char *sentence);
char* remove_quotes(char *str, dataSymbols *arraySymbols, char **tempLine);
void free_array(char **array);
void free_word(char *word);
int extract_fixed_index(char *input, char *word, dataSymbols *arraySymbols);
int extract_register(char *regist);
int count_elements(char **array);

/*Functions from legalSentence.*/
int is_define_valid(char *line, dataSymbols *arraySymbols, int rownum);
int is_directive_sentence_valid(char *line, dataSymbols *arraySymbols, int rownum);
int is_instruction_sentence_valid(char *line, dataSymbols *arraySymbols, int rowNumber);

/*Functions from symbol.*/
nodeDataSymbols* find_the_node_symbol(dataSymbols *arraySymbols, char *nameNode);
int is_in_symbol_table(char *name, dataSymbols *arraySymbols);
SymbolType get_symbol_type(char *symbolName, dataSymbols *arraySymbols);
int find_dataAddr(dataSymbols *arraySymbols, char *name);
int is_reserved_words(char *s);
int is_equal_sign(char *s);
int is_number(char *s);
int s_to_num(char *s);
int add_node_to_arraySymbols(char *name, int type, int dataAddr, dataSymbols *arraySymbols);
void change_symbol_type(char *symbolName, SymbolType newType, dataSymbols *arraySymbols);
int is_extern_entry_symbol_present(dataSymbols *arraySymbols);
int is_extern_symbol_present(dataSymbols *arraySymbols);
int is_entry_symbol_present(dataSymbols *arraySymbols);
void free_arraySymbols(dataSymbols *arraySymbols);

/*Functions from operation.*/
operation* find_operation_by_name(char *nameop, int rowNumber);
int get_op_index(char *op);
unsigned int get_operation_binary_code(char *name, int rowNumber);
int two_operands_grop (char *op);
int one_operands_grop (char *op);
int no_operands_grop (char *op);
int is_two_operands (char *sentence, int rowNumber);
int is_immediate(char *op);
int is_direct(char *op, dataSymbols *arraySymbols, int rowNumber);
int is_fixed_index(char *op, int rowNumber);
int is_register(char *op);
AddressMethodsOptions turns_method (char *op, dataSymbols *arraySymbols, int rowNumber);
int valid_operand_to_operation(char *operation, char *operand, int src, int des, dataSymbols *arraySymbols, int rowNumber);
int valid_instruction_sentence(char *line, int rowNumber, dataSymbols *arraySymbols);
int is_operand_addressing_methods(char *operand,  int rowNumber, dataSymbols *arraySymbols);


/*Functions from memory.*/
void resetMemoryCounters(unsigned *IC, unsigned *DC);
void increaseInstructionCounter(int amount, unsigned *IC);
void increaseDataCounter(int amount, unsigned *DC);
void int_to_binary(int num, long int *binary);
int char_to_ascii(char c);
void insert_arrayData(char *sentence, char *typeSentence, long int arrayData[][SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber, unsigned int *DC, char **temp);
void insert_arrayInstructions(long int *binary_code, long int arrayInstructions[][SIZE_BINARY_CODE], unsigned int *IC, dataSymbols *arraySymbols, char **arrayLine);
int calculate_num_words(char *line, dataSymbols *arraySymbols, int rowNumber);
void enter_address_method(long int *binary_code, char *operand, int i, int j, dataSymbols *arraySymbols, int rowNumber);
void enter_address_operation(long int *binary_code, char *operation);
void helper_enter_address_operation(long int *binary_code, int i, int j, int k, int x);
void binary_word_instruction(char *operation, char *opSrc, char *opDes, dataSymbols *arraySymbols, long int *binary_code, int rowNumber);
void operand_to_binary(int number, long int *binary);
void register_to_binary(long int *binary, int start_index, int end_index, int number);
int enter_immediate(char *operand, long int binary[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber);
int enter_direct(char *operand, long int binary[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber);
int enter_fixed_index(char *operand, long int binary[SIZE_BINARY_CODE], long int binary1[SIZE_BINARY_CODE], dataSymbols *arraySymbols, int rowNumber);
int binary_operand_instruction(char *operand, char *operand1, dataSymbols *arraySymbols, int rowNumber, long int binary[SIZE_BINARY_CODE], long int binary1[SIZE_BINARY_CODE]);
int return_size_arrayInstructions();
int return_size_arrayData();



/*Functions from firstRead.*/
void run_first_pass(FILE *ofp, char *fileName);

/*Functions from secondRead.*/
int run_second_pass(FILE *ofp, unsigned int *IC, dataSymbols *arraySymbols, externList *ourExternList, long int arrayInstructions[SIZE_ARRAY][SIZE_BINARY_CODE], long int arrayData[SIZE_ARRAY][SIZE_BINARY_CODE], char *fileName);

/*Functions from buildeObject*/
void build_object(char *fileName, long int arrayInstructions[SIZE_ARRAY][SIZE_BINARY_CODE], long int arrayData[SIZE_ARRAY][SIZE_BINARY_CODE], FILE *ofp, int sizeArrayInstructions, int sizeArrayData, dataSymbols *arraySymbols);
int size_array(long int array[][SIZE_BINARY_CODE]);
int count_digits(int num);
char* add_leading_zeros(int num, dataSymbols *arraySymbols);
char* binary_to_base4(long int binary[]);

/*Functions from buildeEntries*/
void build_entries(dataSymbols *arraySymbols,  char *fileName);

/*Functions from buildeExterns*/
nodeExtern* find_the_nodeExtern(externList *ourExternList, char *nameNode);
int is_in_externList(char *name, externList *ourExternList);
int add_node_to_externList(char *name, int address, externList *ourExternList);
void enter_extern(char *name, int address, externList *ourExternList, dataSymbols *arraySymbols, int rowNumber);
void build_externals(char *fileName, FILE *ofp, dataSymbols *arraySymbols, externList *ourExternList);
void free_externList(externList *ourExternList);
