//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#ifndef lexerDefHeader
#define lexerDefHeader
#include<stdio.h>
#include<string.h>
#define LEX_LENGTH 100
#define BUFFER_SIZE 50000
#define SIZEKEYWORDS 33 //NOTE: Change here if any keyword is added/subtracted
//Enums for mapping strings to integers
enum term{EPSILON,DOLLAR,INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,PROGRAM,RECORD,TAGGED,UNION,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE,TAKES,INPUT,RETURNS,AND,OR,FOR,IN,SWITCH,CASE,BREAK,DEFAULT,WHILE,PLUS,MINUS,MUL,DIV,LT,LE,GE,GT,EQ,NE,DEF,DRIVERDEF,ENDDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK,ID,RNUM,NUM,ERROR,COUNTT};
enum nterm{program,moduledeclarations,moduledeclaration,othermodules,drivermodule,module,ret,inputplist,n1,outputplist,n2,datatype,rangearrays,type,moduledef,statements,statement,iostmt,boolconstt,varidnum,var,whichid,simplestmt,assignstmt,whichstmt,lvalueidstmt,lvaluearrstmt,ind,modulereusestmt,optional,idlist,n3,expression,u,newnt,arithmeticorbooleanexpr,n7,anyterm,n8,arithmeticexpr,n4,term,n5,factor,op1,op2,logicalop,relationalop,declarestmt,conditionalstmt,casestmts,n9,value,dflt,iterativestmt,range,countnt};

//Type defintions
typedef union{
    int num;
    float rnum;
} number; //Value field in return structure

typedef struct{
    // char token[20];
    int token;
    char lexeme[LEX_LENGTH];
    int line;
    number value;
    unsigned int tag:1;
} lex; //Return structure for lexemes/tokens


//Global variables
extern char* keywords[];
extern char* termString[]; //For getting enum->string
extern char* NtermString[]; //For getting enum->string 
// extern hashNode* symbolTable[];

#endif
