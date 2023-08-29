#include "project.h"
#include "global.h"
/* the second passage */
char c;
int secPass(FILE* fd)
{
    boolean notError = TRUE;
    int line = 1;
	char curword[MAX_LABEL];
	int i=0;
	int h=0;
	boolean entryFlag=FALSE;
	type  kind;
	char* tmp;
	c=fgetc(fd);
	while (c!=EOF)
	{
			while (c == ' ' || c == '\t')/*skip space*/  
			  		c = fgetc(fd);  
			if(c== ';')/*note*/
			 	while((c=fgetc(fd))!='\n');/*skip*/
			while(c=='\n')/*end of line*/
			{
			  line++;
			  c=fgetc(fd);/*read the next char*/
			}
			i=0;
			while((c != EOF) && c != ' ' && c != '\n' && c != '\t' && c != ';') /*inside word*/
			{
				curword[i]=c;/*save the word into an array*/
				i++;
				c=fgetc(fd);
			}
			curword[i]='\0';
			while (c == ' ' || c == '\t')/*skip space*/  
			  	c = fgetc(fd);
			tmp=curword;
			kind = wordClassification(tmp);/*find the type of the current word*/
			if(kind == LABEL) /*LABEL*/
			{
				if(entryFlag==TRUE)
				{
					if (notinside(symbol_table, &h,tmp))/*if this label not exist in the symbol table yet*/
					{
						printf("\neror: in line %d export a non-existent label\n", line);
						notError = FALSE;
					}
					else
						symbol_table[h].entry = TRUE;/*Icon not found*/
						entryFlag=FALSE;
				}
			}
			else if ((kind == EXTERN) || (kind == DATA_S) || (kind == DATA_N) || (kind == LABEL && entryFlag==FALSE)) 
			   while((c=fgetc(fd))!='\n');/*skip*/
			else 
			if (kind == ENTRY) /* add to 'entry' table */
			{
				numen++;
				entryFlag=TRUE;   
			}
			else if (inCode2(fd, line,curword) == FALSE)/*Handles a line of code*/
				   notError = FALSE;
		
	} /* end of while */
	if (notError == FALSE)
	{
	    printf("error exist2");
	    return 1;
	}
	return 0;
}
/*Handles a line of code*/
boolean inCode2(FILE* fd, int line,char* word3)
{
    boolean thernteerror = TRUE;
    int j;
    for (j = 0; j < 16; j++)
    {
        if (strcmp(operations[j].name, word3) == 0)/*check if it is an instruction*/
        {
		    insertCode(line);/*allocate space*/
				break;
        }
    }
    thernteerror = numCommLine2(fd, j, line);/*Makes binary coding of the operand words' information words*/
    return thernteerror;
}
/*allocate space*/
void insertCode(int line)
{
	IC2++;
    if (numc == 0)
        code_tabel = (lineToPrint*)malloc(sizeof(lineToPrint));
    else
        code_tabel = (lineToPrint*)realloc(code_tabel, (numc + 1) * sizeof(lineToPrint));
    if (code_tabel == NULL)
    {
        printf("\nerror: in line %d \" Allocation failed \"  \n", line);
        exit(1);
    }
    numc++;
}

boolean numCommLine2(FILE* fd, int action, int line)/*Makes binary coding of the operand words' information words*/
{
    boolean notEror = TRUE;
    int operand = operations[action].num_of_operands;
    int code;
    code = numc - 1;
    code_tabel[code].word.w1.opcode = operations[action].opcode;
    code_tabel[code].word.w1.funct = operations[action].funct;
    code_tabel[code].are = 'A';
	code_tabel[code].flag=1;
	if(operand==0)
	{
		code_tabel[code].word.w1.sourceAddress = 0;
		code_tabel[code].word.w1.destAddress = 0;
	code_tabel[code].flag=1;
	}
	else if(operand==1)
	{
		code_tabel[code].word.w1.sourceAddress = 0;
		code_tabel[code].word.w1.destAddress = opaddres(fd, &notEror,line);
	code_tabel[code].flag=1;
	}
	else if(operand==2)
	{
		code_tabel[code].word.w1.sourceAddress = opaddres(fd, &notEror,line);
		code_tabel[code].word.w1.destAddress = opaddres(fd, &notEror ,line);
	code_tabel[code].flag=1;
	}
    
    return notEror;
}

int opaddres (FILE* fd, boolean* noteror, int line)
{
	int try;
	boolean minus = FALSE;
	int jumpLines;
	int add;
	char wordw[MAX_LABEL + 2];
	char label[MAX_LABEL + 1];
	int sum = 0;
	int i =0;
	int code;
	code = numc - 1;
	unsigned int musk=4095;
	while (c == ' ' || c == '\t'||c==',')/*skip space*/  
      		c = fgetc(fd);
	while ((c != ' ') && (c != '\t') && (c != '\n') && (c != ','))/*inside word*/
	{
		wordw[i] = c;/*save the word into an array*/
		i++;
		c=fgetc(fd);
	}
	wordw[i] = '\0';
	if((wordw[0]=='r') && (wordw[2]=='\0'))/*This is a register*/
	{
		insertCode(line);/*allocate space*/
		try=(wordw[1]-'0');
		code_tabel[numc - 1].word.mem1.memory= (01 << (try));
		code_tabel[numc - 1].are='A';
		code_tabel[code].flag=0;
		return 3;				
	}
	else if (wordw[0]== '#')/*Immediate address*/
	{
		i = 1;
		if ((wordw[i]) == '-')
		{
			minus = TRUE;
		        i++;
		}
		for (; i < strlen(wordw); i++)
			sum = (sum * 10) + (wordw[i] - '0');
		if (minus == TRUE)
			sum *= (-1);
		insertCode(line);/*allocate space*/
		sum = sum & musk;
		code_tabel[numc - 1].word.mem1.memory = sum;/* sum = binary */
		code_tabel[numc - 1].are='A';
		code_tabel[code].flag=0;
		return 0;
	}
	 else if (wordw[0] == '%')/*Relative addressing*/
	 {
		 strcpy(label, wordw + 1);
		 i = 0;
		 if (notinside(symbol_table, &i, label))/*not exist in the symbl table yet*/
		 {
		 	printf("\nerror: in line %d The label %s does not exist", line, label);
		    noteror = FALSE;
		 }
		 else/*already exist in the symbl table */
		 {
		            jumpLines = (symbol_table[i].addres-IC2);/* address of label */
		            insertCode(line);/*allocate space*/
		            code_tabel[numc - 1].word.mem1.memory = (jumpLines & musk);/*enter the label's address the symbols tabel and keep it in length of 12 bits*/
					code_tabel[code].flag=0;
					if(symbol_table[i].isextern)/*if extern put 'E'*/
					{
						if (numext == 0)
		                    extPrint = (ext*)malloc(sizeof(ext));
		                else
		                    extPrint = (ext*)realloc(extPrint, (numext + 1) * sizeof(ext));
		                if (extPrint == NULL)
		                {
		                    printf("\nerror: in line %d \" Allocation failed \"  \n", line);
		                    exit(1);
		                }
		                numext++;
		                strcpy(extPrint[numext - 1].name, symbol_table[i].name);
		                extPrint[numext - 1].line = IC2-1;
							code_tabel[numc - 1].are='E';
					}
					else
						code_tabel[numc - 1].are='R';
					return 2;
		}
	 }
	  else
	  {
			i = 0;
			if (notinside(symbol_table, &i, wordw))/*not exist in the symbl table yet*/
		    {
		    	printf("\nerror: in line %d unidentified label\n", line);
		        noteror = FALSE;
			}
		    else/*already exist in the symbl table */
		    {
		    	add = symbol_table[i].addres;
		        insertCode(line);/*allocate space*/
		        if (symbol_table[i].isextern)
				{
					if (numext == 0)
                        extPrint = (ext*)malloc(sizeof(ext));
                    else
                        extPrint = (ext*)realloc(extPrint, (numext + 1) * sizeof(ext));
                    if (extPrint == NULL)
                    {
                        printf("\nerror: in line %d \" Allocation failed \"  \n", line);
                        exit(1);
                    }
                    numext++;
                    strcpy(extPrint[numext - 1].name, symbol_table[i].name);
                    extPrint[numext - 1].line = IC2-1;
		        	code_tabel[numc - 1].are='E';
				}
		        else
		        	code_tabel[numc - 1].are='R';
			code_tabel[numc - 1].word.mem1.memory=add;
				return 1;
				
		   }
	  }
	
	  while (c == ' ' || c == '\t')/*skip space*/  
      		c = fgetc(fd); 
	  if (c == '\n')
		    line++;
return -1;
}
void enterExtern(int line)/* allocate space*/
{
	if (numext == 0)
		extPrint = (ext*)malloc(sizeof(ext));
	else
		extPrint = (ext*)realloc(extPrint, (numext + 1) * sizeof(ext));
	if (extPrint == NULL)
	{
		printf("\nerror: in line %d \" Allocation failed \"  \n", line);
		exit(1);
	}
	numext++;
}


