#include "project.h"
#include "global.h"

/* This method produces the output of the three files */
void output(char name[])
{
    FILE * fdob, * fdex, * fden;
    int line = MEMORY_ADRRES;
    int i;
    char nameob[MAX_FILE_NAME];
    char nameen[MAX_FILE_NAME];
    char nameex[MAX_FILE_NAME];
    strcpy(nameob, name);
    strcat(nameob, ".ob");
    strcpy(nameex, name);
    strcat(nameex, ".ext");
    strcpy(nameen, name);
    strcat(nameen, ".ent");
    fdob = fopen(nameob, "w");
    if (numext != 0)
        fdex = fopen(nameex, "w");
    if (numen != 0)
        fden = fopen(nameen, "w");
    for (i = 0; i < numext; i++) /* print the file .ext */
    {
        fprintf(fdex, "\n%s ", extPrint[i].name);
        fprintf(fdex, " %04d", extPrint[i].line);
    }
    for (i = 0; i < nums; i++) /* print the file .ent */
    {
        if (symbol_table[i].entry == TRUE)
        {
            fprintf(fden, "\n%s ", symbol_table[i].name);
            fprintf(fden, " %04d", symbol_table[i].addres);
        }
    }
    fprintf(fdob, "%7d %d", ICF - MEMORY_ADRRES, DCF); /* print the finals: IC, DC */
    for (i = 0; i < numc; i++)
    {
        fprintf(fdob, "\n");
        fprintf(fdob, " %04d ", line);
		fprintf(fdob, "%03x  ",code_tabel[i].word.mem1.memory);
	fprintf(fdob, " %c",code_tabel[i].are );
        line++;
    }
    for (i = 0; i < numd; i++) /* print the data */
    {
        fprintf(fdob, "\n");
        fprintf(fdob, "%04d ", line);  /*convert to hexa */
		fprintf(fdob, " %03x ",wd[i].memory);
	fprintf(fdob, " %c",'A' );
        line++;
    } 
    fclose(fdob);
    if (numext != 0)
        fclose(fdex);
    if (numen != 0)
        fclose(fden);
}

