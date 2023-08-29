#include "project.h"
#include "global.h"
/*project made by:
Sima Shor   211726922
Tehilla Rosenfeld 212013957
This program receives a code file written in assembly language, the file with the extension "PS" and converts the code to binary using two transitions
In case the code is correct the three files with the following extensions are obtained:
1) .ob - object code
2) .ent - entries label codes
3) .ext - external label codes
*/
int IC=MEMORY_ADRRES;
int DC=0;
int ICF=0;/*final IC*/
int DCF=0;/*final DC*/
int nums=0; /* number of symbols */
int numd=0; /* number of lines of data */
int numc=0; /* number of lines of code */
int numen=0; /* number of symbols = entry */
int numext=0; /* number of symbols = extern */
int IC2=MEMORY_ADRRES; /* IC To the second passage */
symbol_struct* symbol_table; /* pointer to the symbol table */
mem* wd; /* pointer to the storage location of the external symbols - that we need to print - in memory */
lineToPrint* code_tabel;
operation operations[16]={{"mov",0,0,2,{{0,1,3},{1,3,-1}}},
					 {"cmp",1,0,2,{{0,1,3},{0,1,3}}},
				 	 {"add",2,10,2,{{0,1,3},{1,3,-1}}},
					 {"sub",2,11,2,{{0,1,3},{1,3,-1}}},
					 {"lea",4,0,2,{{1,-1,-1},{1,3,-1}}},
					 {"clr",5,10,1,{{-1,-1,-1},{1,3,-1}}},
					 {"not",5,11,1,{{-1,-1,-1},{1,3,-1}}},
					 {"inc",5,12,1,{{-1,-1,-1},{1,3,-1}}},
					 {"dec",5,13,1,{{-1,-1,-1},{1,3,-1}}},
					 {"jmp",9,10,1,{{-1,-1,-1},{1,2,-1}}},
					 {"bne",9,11,1,{{-1,-1,-1},{1,2,-1}}},
					 {"jsr",9,12,1,{{-1,-1,-1},{1,2,-1}}},
					 {"red",12,0,1,{{-1,-1,-1},{1,3,-1}}},
					 {"prn",13,0,1,{{-1,-1,-1},{0,1,3}}},
					 {"rts",14,0,0,{{-1,-1,-1},{-1,-1,-1}}},
					 {"stop",15,0,0,{{-1,-1,-1},{-1,-1,-1}}}};
ext* extPrint;/*extern to print*/
en* enprint;/*entry to print*/
lineToPrint* code_tabel;
int main(int argc, char* argv[])
{
    FILE* fd;
    int allright;
    char name[MAX_FILE_NAME];
    char namer[MAX_FILE_NAME+8];
    if (argc == 1)
        printf("\nYou did not enter a file name.\n");
    while (--argc)
    {
        strcpy(name, argv[argc]);
        strcpy(namer, name);
        strcat(namer, ".as");/*add .as to the file name*/
        fd = fopen(namer, "r");
		if (fd == NULL)
      		printf("\nThe file\"%s.as\"does not exist.\n", name);
		else
		{
        	allright = firstPass(fd);/*sending the file to pass1*/
			if(allright==1)
	    		printf("error in first move %d",allright);
         else if (allright == 0)   /*there is not error*/
         {
         	rewind(fd);
            allright = secPass(fd);
				if(allright==1)
	    			printf("error in second move %d",allright);
            if (allright == 0)   /*there is not error*/
            	output(name);
         }
        	fclose(fd);
        }
    }
	free(wd);
	free(extPrint);
	free(code_tabel);
	return 0;
}

