#include "project.h"
#include "global.h"
/* The first passage: */
int firstPass(FILE* fd)
{
    boolean notError = TRUE;
    int line = 1;
	char curword[MAX_LABEL]; /* array that picks up the current word*/  
   char savelabel[MAX_LABEL]; /* array that picks up the label (if any) */  
    char arrlabel[MAX_LABEL];
    char* la;
    char* la2;
	char* temp;
    char* externlabel;
    int flag_label = FALSE; /* flag of label */
    int i = 0;
    char c;
    boolean ok;
	int t;
	type kind;
    c=fgetc(fd);
    while (c!=EOF)/* read the file */
    {
	while (c == ' ' || c == '\t')/*skip spaces*/  
	  c = fgetc(fd);
	if(c== ';')/*note*/
		while((c=fgetc(fd))!='\n');/*skip the note*/
	if(c=='\n')
	{
	 line++;
	 c=fgetc(fd);
	}
	if(c==EOF)
		break;
	i=0;
	while((c != EOF) && c != ' ' && c != '\n' && c != '\t' && c != ';' && c!=',' )/*inside word*/
	{
		curword[i]=c;/*enter the word to an array*/
		i++;
		c=fgetc(fd);
	}
	curword[i]='\0';
	if(c==EOF)
		break;
	while (c == ' ' || c == '\t')/*skip spaces*/  
	c=fgetc(fd);
	temp=curword;
	   kind = wordClassification(temp);/*check the type of the word*/
	   if (kind == LABEL)
	   {
	   	ok=correct(temp,line);
			if(ok==FALSE)
				notError=FALSE;
				
		   flag_label = TRUE;
		   la = curword; /* get the name of the label */
		   strcpy(savelabel,la);/* save the word as a label name*/
	   }
	   else if (kind == DATA_N || kind == DATA_S)
	   {
		  if (flag_label == TRUE)
	  	  {
		       if (enterSymbolTable(savelabel, DC, DATA, FALSE, line) == FALSE)
		           notError = FALSE;
			   flag_label=FALSE;/*turn off the flag*/
	 	  }
		   if (kind == DATA_N)
		       if (inData(fd, line,c) == FALSE)
		           notError = FALSE;
		   if (kind == DATA_S)
		       if (inString(fd, line,c) == FALSE)
		           notError = FALSE;	
		c=fgetc(fd);
		   line++;
	   }
	   else if (kind == ENTRY)/*if this entry skip. It will be taken care of in pass2 */
	{
	while((c=fgetc(fd))!='\n');/*skip*/
	c=fgetc(fd);
		       line++;
	}
	else if (kind == EXTERN)
	{
		i=0;
		while((c!= EOF) && c != ' ' && c != '\n' && c != '\t' && c != ';')
		{
			arrlabel[i]=c;
			i++;
			c=fgetc(fd);
		}
		arrlabel[i]='\0';
		la2=arrlabel;
		externlabel = legalExternSymbol(arrlabel,i); /* enter in 'externlabel' the outer label */
		if (externlabel == NULL)
		{
			printf("\nerror: in line %d invalid external label\n ", line);
			notError = FALSE;
		}
		else
		{
			if (enterSymbolTable(la2, 0, -1, TRUE, line) == FALSE)
			  notError = FALSE;
			line++;
		}
	}
	else
	{
		   i=0;
		   if (flag_label == TRUE)
		   {
			savelabel[i-1]='\0';
		        enterSymbolTable(savelabel, IC, CODE, FALSE, line);
			flag_label=FALSE;/*turn off the flag*/
		   }
		   if (inCode(fd,curword, line,c) == FALSE)
		       notError = FALSE;
	c=fgetc(fd);
	   }
		} /* end of while */
		if (notError == FALSE)
		{
		    printf("errors exist");
		    return 1;
		}
		ICF = IC;
		DCF = DC;
		for (t = 0; t < nums; t++)
		{
		    if (symbol_table[t].isextern == FALSE)
		        if (symbol_table[t].kind == DATA)
		            symbol_table[t].addres =symbol_table[t].addres+ ICF;
		}
		return 0;
}


/* goes through a data type line */
boolean inData(FILE *fd,int line,char ch)
{
    int sum;
    int minus;
    boolean digit;
	if(ch=='\n')
		return FALSE;
    while (ch!='\n')
    {
        sum = 0;
        digit = FALSE;
		minus=1;
        while (ch == ' ' || ch == '\t')
            ch= fgetc(fd);
        if (ch == '-' || ((ch== '+') && (minus == 1)))
        {
			if(ch=='-')
				minus=0;
			ch= fgetc(fd);
        }
        while (isdigit(ch))
        {
            digit = TRUE;
            sum = (sum * 10) + ((ch) - '0');
            ch = fgetc(fd);
        }
		if (((ch) == ' ') || (ch == '\t') || (ch == ',') || (ch == '\n'))
		{
		   while (ch == ' ' || ch == '\t')
			   ch = fgetc(fd);
		   if (minus==0)
			   sum *= (-1);
		   if(digit==TRUE)
		   {
				dataAlloc(DATA_N, '0',sum, line);
			    DC++;
		   }
		   else
		   {
               printf("\nerror: in line %d Missing comma1\",\"",line);
               return FALSE;
           }
		   if (ch == ',')
		   {
			    ch = fgetc(fd);
				digit=FALSE;
		   }
		   if (ch == '\n')
		   {
				line++;
				if (digit)
		  			return TRUE;
				else
					return FALSE;
		  }      
	  }
	  else
	  {
	  	 printf("\nerror: in line %d Missing comma2\",\"",line);
		 return FALSE;
	  }
  }
		return TRUE ;
}

/* allocates space for data */
void dataAlloc(type data, char ch,int num, int line)
{
    if (numd == 0)
    {
        wd = (mem*)malloc(sizeof(mem));
        if (wd == NULL)
        {
            printf("\nerror: in line %d \" Allocation failed \"  \n", line);
            exit(1);
        }
    }
    else
    {
        wd = (mem*)realloc(wd, (numd + 1) * sizeof(mem));
        if (wd == NULL)
        {
            printf("\nerror: in line %d \" Allocation failed \"  \n", line);
            exit(1);
        }
    }
    if (data == DATA_N)
	{
        wd[numd].memory = num;
	}
    else if (data == DATA_S)
    {
        wd[numd].memory = ch;
	}
    numd++;
	}
boolean inString (FILE* fd,int line, char ch)/*store the string in the table of data and string*/
{
	while (ch == ' ' || ch == '\t')
	  ch = fgetc(fd);
	if(ch=='"')
	{
		while((ch=fgetc(fd))!='"')
		{
			if(ch=='\n')/*in string can be \t or \n*/
			{
			  printf("error: in line %d missing: \" ", line);
			  return FALSE;
			}
			dataAlloc(DATA_S,ch,0,line);
			DC++;
		}
	dataAlloc(DATA_S,'\0',0,line);
	DC++;
	}
	else
	{
		printf("error: in line %d missing: \" ", line);
		return FALSE;
	}
return TRUE;
}
/* goes through a code type line */
boolean inCode(FILE* fd,char* word1, int line,char c)
{
    boolean L;
    boolean legalname = TRUE;
    int j;
    int command;
    for (j = 0; j < 16; j++)
    {
        if (strcmp(operations[j].name,word1) == 0)
        {
            legalname = TRUE;
            command = j;
			break;
        }
    }
    if (legalname == FALSE)
    {
        printf("\nerror: in line %d invalid command3\n", line);
        return FALSE;
    }
    else
    {
        L = numCommLine1(command, fd, line,c);
        if (L == -1)
            return FALSE;
        IC += L;
        return TRUE;
    }
return FALSE; 

}
/* check addres of operands and return the number of command lines */
int numCommLine1(int action, FILE *fd, int line,char ch)
{
int add = 5;
    char wordw[MAX_LABEL + 2];
    int operand = 0;
    int i = 0;
    int j=0;
    operand=operations[action].num_of_operands;
    if(operand==1)
       j=1;
    while (operand > 0)
    {
        while ((ch == ' ') || (ch== '\t'))
            ch=fgetc(fd);
    while ((ch != ' ') && (ch != '\t') && (ch != '\n') && (ch != ','))
    {
    		wordw[i] = ch;
            i++;
            ch=fgetc(fd);
     }
        wordw[i] = '\0';
        if ((wordw[0] == 'r')  && (wordw[2] == '\0'))
        {
			if(((wordw[1] - '0') <= 7) && ((wordw[1] - '0') >= 0))
			{
				add = 3;
			}
			else
			{
				printf("\n error: in line %d Invalid register \n", line);
				return -1;
			}
		}
        else if (wordw[0]=='#')
        {
            if ((isdigit(wordw[1])) || (wordw[1] == '-'))
            {
            for (i = 2; i <= strlen(wordw)-1; i++)
            {
                if (!(isdigit(wordw[i])))
                {
            printf("\n error: in line %d invalid character in immediate address \n", line);
            return -1;
                }
            }
            add = 0;
            }
    		else
            {
				printf("\n error: in line %d invalid character in immediate address \n", line);
                return -1;
            }
        }
        else if (wordw[0] == '%')
              add = 2;
        else
              add = 1;
    for(i=0;i<3;i++)
	{
		if(operations[action].addressing[j][i]==add)
		{
		  operand--;
		  break;
		}
		else if(operations[action].addressing[j][i]==-1)
		{
		  printf("\nerror: in line %d missing comma3\n", line);
		  return -1;
		}
	}
	j++;
	while ((ch == ' ') || (ch== '\t'))
		   ch=fgetc(fd);
	if(operand==1)
	{
	if (ch == ',')
	  ch=fgetc(fd);
	else
	{
	printf("\nerror: in line %d missing comma4\n", line);
	return -1;
	}
	}
	else
	{
		if(ch=='\n')
	{
		  line++;
	}
	else
	{
	printf("\nerror: in line %d missing comma5\n", line);
	   return -1;
	}
	}
	   } /* end of while */
    return operations[action].num_of_operands + 1;
}


