# Assembler - graduation project in a laboratory course in systems programming
Final course project (Written in C)-
I built an assembler for an assembly language.
 I actually converted assembly code to binary presentation 
 
**Input**: file with assembly code

**Output**: a file with code in machine language - binary. 

The assembler I programed worked using 2 phases: 
in the first phase over the program received as input, I identified the labels appearing in the program and gave each label a numerical value which is the address in memory that the label presents.

 In the second phase using the label values, the operation codes and the register numbers I built the machine chart.
Code in assembly contains operation names and operands on which the operation is executed.
Now I need to convert each operation name and each operand to binary presentation.
I built a chart where all the action names are listed and each action name has a binary code. The chart is a fixed chart which means that the binary codes are already known in advance, Unlike the operands that are not known in advance, because the memory addresses are not known in advance.
The addresses can only be known after I have scanned the entire program and have all the labels.
This is the reason why I have to go through the code twice.
In the first phase I built a table of all the labels with the numerical values that belong to them and in the second phase I replaced all the labels that appear in the code, with their numerical values.


The work space I used was Ubuntu, which is a version of Linux, as a virtual machine
(Only after linking and loading I can run the code - I didn't have to do that, just return a machine code file)


The way I split the code into files:
Besides the files Pass 1 and Pass 2 that I already talked about, There are 4 more files:

**Main.c**: get the file with the assembly code, runs the 2 phases, then returns the file with the machine code
**Global.h**: All global variables are defined in it, so that we can use them in all files
**Write.c**: produces the output to the file
**Metpass.c**: handle the label chart


