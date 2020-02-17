#define LEX_LENGTH 100

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