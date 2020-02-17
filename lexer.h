#include<stdio.h>
#define LEX_LENGTH 100
#define BUFFER_SIZE 10000

enum term{E,INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,PROGRAM,RECORD,TAGGED,UNION,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE,TAKES,INPUT,RETURNS,AND,OR,FOR,IN,SWITCH,CASE,BREAK,DEFAULT,WHILE,PLUS,MINUS,MUL,DIV,LT,LE,GE,GT,EQ,NE,DEF,DRIVERDEF,ENDDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK,ID,RNUM,NUM,ERROR,ENDOFFILE};
enum nterm{program,moduledeclarations,moduledeclaration,othermodules,drivermodule,module,ret,inputplist,inputplistnew,outputplist,outputplistnew,datatype,type,moduledef,statements,statement,iostmt,var,whichid,simplestmt,assignmentstmt,whichstmt,lvalueidstmt,lvaluearrstmt,ind,modulereusestmt,optional,idlist,idlistnew,expression,btermrec,bterm,aterm,arithmeticexpr,aenew,term,termnew,factor,unturm,pmop,logicalop,mdop,relationalop,decalrestmt,conditionalstmt,casestmts,csnew,value,dflt,iterativestmt,range,expressionnew};

typedef union{
    int num;
    float rnum;
}number;

typedef struct{
    // char token[20];
    int token;
    char lexeme[LEX_LENGTH];
    int line;
    number value;
    unsigned int tag:1;
}lex;

lex getNextToken(char*, int*, int*);
lex operator_dfa(char*, int*, int*);
lex identifier_dfa(char*, int*);
lex number_dfa(char*, int*);
