#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MAX_LINE 80 /* maximum chars per line */
#define MAX_LABEL 31 /* maximum number of labels */
#define MEMORY_ADRRES 100
#define MAX_FILE_NAME 30

typedef enum { FALSE, TRUE } boolean;
enum { CODE, DATA };
enum { A, R, E}; /* ARE */
typedef enum {
    LABEL, DATA_N, DATA_S, EXTERN, ENTRY, OTHER
}type; 
typedef struct { /* stores commands such as mov, cmp, inc etc. */
    char name[5]; /* 4 chars + '\0' */
    int opcode; /* code between 0 and 15 */
    int funct; /*The function value of the operation */
	int num_of_operands;
    int addressing[2][3]; /* table of 2*3 permittable source/destinations operands: immediate, direct, indirect, register */
}operation;

typedef struct { /* we store labels in this structure */
    char name[MAX_LABEL];
    unsigned int addres :12; /* address of label */
    unsigned int kind : 1; /* is it CODE or DATA */
    unsigned int isextern : 1; /* if it's an .extern label put 1 else 0 */
    unsigned int entry : 1; /* if it's an .entry label put 1 else 0 */ 
}symbol_struct;

typedef struct { /* Size of memory for command prompt In machine language */
unsigned int memory : 12; /* 12 ? */
}mem;

typedef struct { /* Size of memory for command prompt In machine language */
char are; 
}a;

typedef struct { /* command word */   
	unsigned int destAddress : 2;
	unsigned int sourceAddress : 2;
	unsigned int funct : 4;
	unsigned int opcode : 4;
}w;
typedef union { 
    w w1;
    mem mem1;
}wor;

typedef struct { /* line to print */
	int flag;
	wor word ;
	char are;	
}lineToPrint;

typedef struct { /* we store external labels in this structure */
    char name[80];
    int line;
}ext;
typedef struct { /* we store external labels in this structure */
    char name[MAX_LABEL];
    int line;
}en;



