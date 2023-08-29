#include "project.h"
#include "global.h"


boolean correct (char* cor,int line)
{
	int i=0;
	if(cor[strlen(cor)-1]==':')
		cor[strlen(cor)-1]='\0';
	for(i=0;i<16;i++)
		if(strcmp(cor,operations[i].name)==0)
		{
			printf("error: in line %d The name of the label is an action name\n",line);
			return FALSE;
		}
	if(cor[0]=='r'/* && ((cor[1]-'0')>='0' && (cor[1]-'0')<='7')*/ && cor[2]=='\0')
	{
		printf("error: in line %d label has same name as register\n",line);
		return FALSE;
	}
	return TRUE;
}


/*allocate space for symbol and enter a symbol into symbols table */
boolean enterSymbolTable(char* namel, int addresl, int kindl, boolean external, int line)
{
    int i = 0;
    if ((notinside(symbol_table, &i, namel)) == FALSE)
    {
		if (external == TRUE)  
      {      
			if (symbol_table[i].isextern == FALSE)
    		{
				printf("\nerror: in line %d  An internal label with this name already exists\n", line);
				return FALSE;
			}
    	}
		else
		{
             printf("\nerror: in line %d  the label already exists\n", line);
             return FALSE;
		}
    }
    if ((nums) == 0)
    {
      symbol_table = (symbol_struct*)malloc(sizeof(symbol_struct));
      if (symbol_table == NULL)
      {
            printf("\nerror in line %d Allocation failed\n", line); 
				exit(1);
      }
		nums++;
    }
    else
    {
        symbol_table = (symbol_struct*)realloc(symbol_table, (nums+1) * sizeof(symbol_struct));
		nums++;
        if (symbol_table == NULL)
        {
            printf("\nerror in line %d Allocation failed\n", line);
            exit(1);
        }
    }
	if(namel[strlen(namel)-1]==':')
		namel[strlen(namel)-1]='\0';
    strcpy(symbol_table[nums - 1].name, namel);
    symbol_table[nums - 1].addres = addresl;
    symbol_table[nums - 1].isextern = external;
    symbol_table[nums - 1].entry = FALSE;
    if (kindl != -1)
        symbol_table[nums - 1].kind = kindl;
    return TRUE;
}


/* check if label is legal */
boolean labelIsLegal(char word[])
{
    int i;
    if (word[strlen(word) - 1] != ':')
        return FALSE;
    if (isalpha(word[0]))
    {
		    for (i = 1; i < (strlen(word) - 1); i++)
		        if ((!(isalpha(word[i]))) && (!(isdigit(word[i]))))
		            return FALSE;
		if(word[0]=='r' && (word[1]>='0' && word[1]<='7') && word[2]=='\0')/*check if the lable name is not like a register name*/
		{
			return FALSE;
		}
		return TRUE;
    }
return FALSE;
}


/* check the type of the current word */
type wordClassification(char* str)
{
	int j;
    if ((strcmp(str, ".data")) == 0)
        return DATA_N;
    if ((strcmp(str, ".string")) == 0)
        return DATA_S;
    if ((strcmp(str, ".extern")) == 0)
        return EXTERN;
    if ((strcmp(str, ".entry")) == 0)
        return ENTRY;
	for(j=0;j<16;j++)
	{
		if(strcmp(operations[j].name,str)==0)
			return OTHER;
	}
    return LABEL;
}

/* check if symbol is'nt in symbol-table, holds a pointer to the position of the label in the table */
boolean notinside(symbol_struct* table, int* num, char namel[])/*pass 2*/
{
	char* temp;
	char* temp1;
    for (*num = 0; *num < nums; (*num)++)
    {
		temp=table[*num].name;
		temp1=namel;
      if (strcmp(temp , temp1) == 0)
         return FALSE;

    }
    return TRUE;
}
/* check if extern label is legal */
char* legalExternSymbol(char str[],int i)
{
    str[i] = ':';
    str[i + 1] = '\0';
    if (labelIsLegal(str)==TRUE)
    {
        str[i] = '\0';
        return str;
    }
    return NULL;
}

