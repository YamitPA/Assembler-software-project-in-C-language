# Assembler-software-project-in-C-language
A project carried out during the degree to write assembler software for a pre-defined assembler language, in the C language


Description
This project is an assembler for a custom assembly language. It reads .as assembly files, interprets them, and translates them into the appropriate output format. The project includes several main stages, including reading commands from input files, handling macros, building memory and objects, and more.

Project Structure
The code is divided into several files, each responsible for a specific part of the project:

main.c: The main file that starts the program execution.
buildEntries.c, buildExternals.c, buildObject.c: Files responsible for creating the project's output files.
firstRead.c, secondRead.c: Files responsible for the first and second reads of the assembly commands.
macro.c: Handles macros in the assembly code.
memory.c: Manages memory allocation.
symbol.c: Handles symbol tables.
operation.c: Manages the assembler's operations.
preAssembler.c: Handles the pre-assembler stage.
heder.h, dataStructures.h: Header files containing necessary definitions and structures for the program.
Examples
The project includes several example files:

The good examples folder contains valid assembly file examples.
The invalid1 example and invalid2 example folders contain invalid assembly examples for testing errors.
Example files:

ps.as, test.as: Sample assembly files for use with the program.
Usage
Use the provided makefile to compile all files and run the program.
You can run the program with the provided sample assembly files, such as ps.as or test.as.
bash
Copy code
make
./assembler file_name.as
System Requirements
A C compiler like gcc is required to compile the project.
POSIX-compatible operating system (Linux or macOS).
Output Structure
The program generates object files, external value files, and entry value files based on the input assembly files.
