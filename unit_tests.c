#include "lexer.c"
#include <stdio.h>
void test_number(){
    int id = 0;
    lex ls = number_dfa("123",&id);
    if (ls.value.num != 123) printf("Error 6\n");
    id =0;
    ls = number_dfa("112.3",&id);
    if (ls.value.rnum != 112.3f) printf("%f Error 8\n",ls.value.rnum);
    id =0;
    ls = number_dfa("1.23e-1",&id);
    if (ls.value.rnum != .123f) printf("%f Error 10\n", ls.value.rnum);
    id = 0;
    ls = number_dfa("1e3",&id);
    if (ls.value.num != 1) printf("%d Error 12\n",ls.value.num);
    return;
}

void test_operator(char* termString[]){
    int id=0,lineno=0;
    char s[10];
    scanf("%s",s);
    lex ls = operator_dfa(s,&id,&lineno);
    printf("%s ERROR\n", termString[ls.token]);
}

int main(){
    char* termString[] = {"E","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","DRIVERDEF","ENDDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","RNUM","NUM","ERROR"};

    test_number();
    test_operator(termString);
}