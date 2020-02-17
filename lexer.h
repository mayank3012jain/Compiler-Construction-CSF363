#include<stdio.h>
#define LEX_LENGTH 100

enum term{E,INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,PROGRAM,RECORD,TAGGED,UNION,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TRUE,FALSE,TAKES,INPUT,RETURNS,AND,OR,FOR,IN,SWITCH,CASE,BREAK,DEFAULT,WHILE,PLUS,MINUS,MUL,DIV,LT,LE,GE,GT,EQ,NE,DEF,DRIVERDEF,ENDDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK,ERROR};
enum nterm{program,modueldeclarations,modueldeclaration,othermodules,drivermodule,module,ret,inputplist,inputplistnew,outputplist,outputplistnew,datatype,type,moduledef,statements,statement,iostmt,var,whichid,simplestmt,assignmentstmt,whichstmt,lvalueidstmt,lvaluearrstmt,index,modulereusestmt,optional,idlist,idlistnew,expression,btermrec,bterm,aterm,arithmeticexpr,aenew,term,termnew,factor,unturm,pmop,logicalop,mdop,relationalop,decalrestmt,conditionalstmt,casestmts,csnew,value,default,iterativestmt,range,expressionnew}

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
    unisgned int tag:1;
}lex;