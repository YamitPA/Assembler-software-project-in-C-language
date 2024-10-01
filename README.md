# Assembler-software-project-in-C-language
A project carried out during the degree to write assembler software for a pre-defined assembler language, in the C language


Overview
This project implements an assembler for a custom assembly language. It reads .as files, parses them, and translates them into various output formats including object files, external values, and entry values. The project is divided into multiple stages such as macro handling, first and second parsing reads, and memory and object building.

Project Structure
The project is organized into several key files:

main.c: The main entry point of the program.
buildEntries.c, buildExternals.c, buildObject.c: Handles the creation of the output files.
firstRead.c, secondRead.c: Manages the two-step reading process of the assembly instructions.
macro.c: Handles macro processing.
memory.c: Manages memory allocation and usage.
symbol.c: Maintains symbol tables for labels and instructions.
operation.c: Handles assembly language operations.
preAssembler.c: Handles the pre-assembly process (e.g., macro expansion).
heder.h, dataStructures.h: Header files that define important data structures and function prototypes used across the project.
Example Files
Valid Examples: Found in the good examples folder.
Invalid Examples: Provided in the invalid1 example and invalid2 example folders for testing error handling.
Sample Assembly Files:
ps.as
test.as
Usage Instructions
1. Compile the Project
To compile the entire project, run the following command using the provided makefile:
make
2. Run the Assembler
Once compiled, you can run the assembler on a sample .as file like this:
./assembler file_name.as

For example:
./assembler ps.as

System Requirements
A C compiler such as gcc or any other standard C compiler.
A POSIX-compatible operating system like Linux or macOS.
Output Files
Based on the input .as files, the program generates:

Object files (.ob)
External values (.ext)
Entry values (.ent)

