#include <string.h>
#include <stdlib.h>
#include <stddef.h>
enum token {ID,ERROR,NUM,RNUM};
typedef union{
    int num;
    float rnum;
}number;
typedef struct{
    // enum token token;
    int token;
    char lexeme[100]; 
    int line;
    number value;
}lex;

lex identifier_dfa(char* c, int* id){
    int id_base = *id;
    // int len;
    (*id)++;
    while(((c[*id] >= 'A' && c[*id]<='Z') || (c[*id] >= 'a' && c[*id]<='z') || 
    (c[*id] >= '0' && c[*id]<='9') || c[*id]=='_')){
        (*id)++;
        // len++;
    }
    // Check of c[id_base -> id] is a keyword in table
    // if(keyword) return 
    // else
    if (*id - id_base <= 20){
        lex ls;
        ls.token = ID;
        strncpy(ls.lexeme,&c[id_base],*id-id_base);
        ls.lexeme[*id-id_base] = '\0';
        // (*id)++;
        return ls;
    }
    else{
        lex ls;
        ls.token = ERROR;
        strcpy(ls.lexeme,"ERROR");
        // (*id)++;
        return ls;
    }
}

lex number_dfa(char* c, int* id){
    int id_base = *id;
    (*id)++;
    // state 36
    while(c[*id] >= '0' && c[*id]<='9'){
        (*id)++;
    }
    // state 38
    if (c[*id]== '.'){
        (*id)++;
    }
    // state 37
    else{
        // return NUM
        lex ls;
        ls.token = NUM;
        strncpy(ls.lexeme, &c[id_base], *id-id_base);
        ls.lexeme[*id-id_base] = '\0';
        ls.value.num = atoi(ls.lexeme);
        return ls;
    }
    // state 40
    if(c[*id] >= '0' && c[*id]<='9')
        while(c[*id] >= '0' && c[*id]<='9'){
            (*id)++;
        }
    // state 39
    else{
        (*id)--;
        // return NUM
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &c[id_base], *id-id_base);
        ls.lexeme[*id-id_base] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    if(c[*id] == 'E' || c[*id] == 'e'){
        // state 41
        (*id)++;
    }
    // state 46
    else{
        // return RNUM
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &c[id_base], *id-id_base);
        ls.lexeme[*id-id_base+1] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    // state 43
    if(c[*id] == '+' || c[*id] == '-'){
        (*id)++;
        // state 45
        if (c[*id] >= '0' && c[*id]<='9'){
                while(c[*id] >= '0' && c[*id]<='9'){
                (*id)++;
            }
        }
        // state 44
        else{
            *id-=2;
            // return RNUM
            lex ls;
            ls.token = RNUM;
            strncpy(ls.lexeme, &c[id_base], *id-id_base);
            ls.lexeme[*id-id_base+1] = '\0';
            ls.value.rnum = strtof(ls.lexeme, NULL);
            return ls;
        }
    }
    // state 45
    else if(c[*id] >= '0' && c[*id]<='9'){
        if (c[*id] >= '0' && c[*id]<='9'){
                while(c[*id] >= '0' && c[*id]<='9'){
                (*id)++;
            }
        }
    }
    // state 42
    else{
        (*id)--;
        // return RNUM;
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &c[id_base], *id-id_base);
        ls.lexeme[*id-id_base+1] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    // state 46
    // return RNUM
    lex ls;
    ls.token = RNUM;
    strncpy(ls.lexeme, &c[id_base], *id-id_base);
    ls.lexeme[*id-id_base+1] = '\0';
    ls.value.rnum = strtof(ls.lexeme, NULL);
    return ls;
}

