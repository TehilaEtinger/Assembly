/* global variables, declated in main.c */
extern int IC ;/*instruction counter*/
extern int DC ;/*Data counter*/
extern int ICF;/*final instruction*/
extern int DCF;/*final data*/
extern int nums ;/*number of string*/
extern int numd ;/*number of data*/
extern int numc;/*number of code*/
extern int numen;/*number of entry*/
extern int numext;/*number of external*/
extern int IC2;
extern operation operations[16];
extern symbol_struct* symbol_table;
extern mem* wd;
extern ext* extPrint;
extern en* enprint;
extern lineToPrint* code_tabel;

/*statments of global functions*/
boolean labelIsLegal(char[]);
boolean notinside(symbol_struct*, int*, char[]);/* check if symbol is'nt in symbol-table and save the location of the label in the table */
type wordClassification(char*);
boolean inData(FILE*, int ,char);/* Data line handling */
boolean inString(FILE*, int, char);/* String line handling */
void dataAlloc(type, char,int,int);/* allocates space for data */
char* legalExternSymbol(char[], int);
boolean enterSymbolTable(char*, int, int , boolean, int );
int numCommLine1(int, FILE*, int,char);/* check addres of operands and return the number of command lines in first pass*/
boolean inCode(FILE*,char*, int,char);/* Code line handling in first pass */
boolean numCommLine2(FILE* ,int ,  int);/* check addres of operands and return the number of command lines in second pass*/
boolean inCode2(FILE*, int,char []);/* Code line handling in second pass */
char *stringWord(char* cur);/***/
void insertCode(int);/*allocate space*/
int firstPass(FILE* fd);/*Makes the first pass*/
int secPass(FILE* fd);/*Makes the second pass*/
void output(char name[]);/* produces the output of the three files */
void exTabel (int line);
int opaddres (FILE* , boolean* , int );
boolean correct (char* ,int );
