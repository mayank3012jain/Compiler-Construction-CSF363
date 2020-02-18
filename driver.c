#include <stddef.h>
#include "lexer.c"

int main(){
    FILE *fp = fopen("input.txt", "r");
    char* termString[] = {"E","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","DRIVERDEF","ENDDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","RNUM","NUM","ERROR","EOF"};
    char read_buffer[BUFFER_SIZE];
    char read_char;
    int read_ptr = 0;

    while((read_char = fgetc(fp)) != EOF){
        read_buffer[read_ptr] = read_char;
        read_ptr++;
    }

    int LINE_NUMBER = 1;
    int line = 1;
    read_buffer[++read_ptr] = '\0';
    read_ptr = 0;
    lex* tokenReturn = malloc(sizeof(lex));
    while(1){
        getNextToken(read_buffer, &read_ptr, &LINE_NUMBER, tokenReturn);
        if(LINE_NUMBER != line){
           line = LINE_NUMBER;
            printf("\n");
        }
        printf(termString[tokenReturn->token]);
        printf(" ");
        if (tokenReturn->token == ENDOFFILE)
        	break;
    }
}
