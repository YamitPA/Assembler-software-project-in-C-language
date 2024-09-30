all: assembler
assembler: main.c preAssembler.c macro.c input.c symbol.c operation.c legalSentence.c memory.c firstRead.c secondRead.c buildObject.c buildEntries.c buildExternals.c dataStructures.h heder.h
	gcc -ansi -Wall -g -pedantic -o assembler main.c preAssembler.c macro.c input.c symbol.c operation.c legalSentence.c memory.c firstRead.c secondRead.c buildObject.c buildEntries.c buildExternals.c

