#include<stdio.h>
#include "lexer.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

// enum token {ID,ERROR,NUM,RNUM};
lex getNextToken(char input[], int* indx, int* lineNumber){
    // int indx = 0;
    char c = input[*indx];
    lex cur;
    // while (c != '\0'){
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
        cur = identifier_dfa(input, indx);
        //call parser
        cur.line = *lineNumber;
        return cur;
    }else if((c>='0' && c<=9+'0')){
        cur = number_dfa(input, indx);
        //call parser
    }// comment
    else if(c=='*'){
        *indx = *indx + 1;
        c = input[*indx];
        if(c=='*'){
            do{
                *indx = *indx +1;
                if(input[*indx]!='\n'){
                    *lineNumber = *lineNumber +1;
                }
            }while(!(input[*indx]== '*' && input[*indx+1]== '*' ));
            *indx = *indx +2;
            return getNextToken(input, indx, lineNumber);
        }else{
            cur.token = MUL;
            cur.lexeme[0] = '*';
            cur.lexeme[1] = '\0';
            cur.line = *lineNumber;
            *indx = *indx +1;
            return cur;
        }
    }
    else{
    	cur = operator_dfa(input,indx,lineNumber;
    	return cur;
    }
}

// lex number_dfa(char* c, int* id){
//     int id_base = *id;
//     (*id)++;
//     // state 36
//     while(c[*id] >= '0' && c[*id]<='9'){
//         (*id)++;
//     }
//     // state 38
//     if (c[*id]== '.'){
//         (*id)++;
//     }
//     // state 37
//     else{
//         // return NUM
//         lex ls;
//         ls.token = NUM;
//         strncpy(ls.lexeme, &c[id_base], *id-id_base);
//         ls.lexeme[*id-id_base] = '\0';
//         ls.value.num = atoi(ls.lexeme);
//         return ls;
//     }
//     // state 40
//     if(c[*id] >= '0' && c[*id]<='9')
//         while(c[*id] >= '0' && c[*id]<='9'){
//             (*id)++;
//         }
//     // state 39
//     else{
//         (*id)--;
//         // return NUM
//         lex ls;
//         ls.token = RNUM;
//         strncpy(ls.lexeme, &c[id_base], *id-id_base);
//         ls.lexeme[*id-id_base] = '\0';
//         ls.value.rnum = strtof(ls.lexeme, NULL);
//         return ls;
//     }
//     if(c[*id] == 'E' || c[*id] == 'e'){
//         // state 41
//         (*id)++;
//     }
//     else if(c==' ' || c== '\t'){
//         *indx++;
//         getNextToken(input, indx,lineNumber);
//     }
//     else if(c=='\0'){
//         cur.token = ENDOFFILE;
//         return cur;
//     }
//     else{
//        cur = operator_dfa(input,indx, lineNumber);
//        // cur.line = lineNumber;
//        return cur;
//     }
// }

lex operator_dfa(char input[], int* indx,int *lineNumber){
    lex cur;
    switch(input[*indx]){
            case '+':
                cur.token = PLUS;
                cur.lexeme[0] = '+';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '-':
                cur.token = MINUS;
                cur.lexeme[0] = '-';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '>':
                *indx = *indx +1;
                // c = input[*indx];
                switch(input[*indx]){
                    case '=':
                        cur.token = GE;
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    case '>':
                        cur.token = ENDDEF;
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '>';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = GT;
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case '/':
                cur.token = DIV;
                cur.lexeme[0] = '/';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '!':
                *indx = *indx +1;
                // c= input[*indx];
                switch(input[*indx]){
                    case '=':
                        cur.token = NE;
                        cur.lexeme[0] = '!';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = ERROR;
                        cur.line = *lineNumber;
                        return cur;
                        break;                            
                }
                break;
            case ';':
                cur.token = SEMICOL;
                cur.lexeme[0] = ';';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '<':
                *indx = *indx +1;
                // c= input[*indx];
                switch(input[*indx]){
                    case '=':
                        cur.token = LE;
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    case '<':
                        cur.token = DEF;
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '<';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = LT;
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case ',':
                cur.token = COMMA;
                cur.lexeme[0] = ',';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break; 
            case '=':
                *indx = *indx +1;
                // c= input[*indx];
                switch(input[*indx]){
                    case '=':
                        cur.token = EQ;
                        cur.lexeme[0] = '=';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = ERROR;
                        cur.line = *lineNumber;
                        return cur;
                        break;                            
                }
                break; 
            case '[':
                cur.token = SQBO;
                cur.lexeme[0] = '[';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;            
            case ':':
                *indx = *indx +1;
                // c= input[*indx];
                switch(input[*indx]){
                    case '=':
                        cur.token = ASSIGNOP;
                        cur.lexeme[0] = ':';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = COLON;
                        cur.lexeme[0] = ':';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case '.':
                *indx = *indx +1;
                // c= input[*indx];
                switch(input[*indx]){
                    case '.':
                        cur.token = RANGEOP;
                        cur.lexeme[0] = '.';
                        cur.lexeme[1] = '.';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *indx = *indx+1;
                        return cur;
                        break;
                    default:
                        cur.token = ERROR;
                        cur.lexeme[0] = '.';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case ']':
                cur.token = SQBC;
                cur.lexeme[0] = ']';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '(':
                cur.token = BO;
                cur.lexeme[0] = '(';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case ')':
                cur.token = BC;
                cur.lexeme[0] = ')';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *indx = *indx +1;
                return cur;
                break;
            case '\n':
                *lineNumber = *lineNumber+1;
                *indx = *indx +1;
                return getNextToken(input,indx,lineNumber);
                break;

            
        }
}

lex identifier_dfa(char input[], int* indx){
    int id_base = *indx;
    // int len;
    (*indx)++;
    while(((input[*indx] >= 'A' && input[*indx]<='Z') || (input[*indx] >= 'a' && input[*indx]<='z') || 
    (input[*indx] >= '0' && input[*indx]<='9') || input[*indx]=='_')){
        (*indx)++;
        // len++;
    }
    // Check of input[id_base -> id] is a keyword in table
    // if(keyword) return 
    // else
    if (*indx - id_base <= 20){
        lex ls;
        ls.token = ID;
        strncpy(ls.lexeme,&input[id_base],*indx-id_base);
        ls.lexeme[*indx-id_base] = '\0';
        // (*indx)++;
        return ls;
    }
    else{
        lex ls;
        ls.token = ERROR;
        strcpy(ls.lexeme,"ERROR");
        // (*indx)++;
        return ls;
    }
}

lex number_dfa(char input[], int* indx){
    int id_base = *indx;
    (*indx)++;
    // state 36
    while(input[*indx] >= '0' && input[*indx]<='9'){
        (*indx)++;
    }
    // state 38
    if (input[*indx]== '.'){
        (*indx)++;
    }
    // state 37
    else{
        // return NUM
        lex ls;
        ls.token = NUM;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base] = '\0';
        ls.value.num = atoi(ls.lexeme);
        return ls;
    }
    // state 40
    if(input[*indx] >= '0' && input[*indx]<='9')
        while(input[*indx] >= '0' && input[*indx]<='9'){
            (*indx)++;
        }
    // state 39
    else{
        (*indx)--;
        // return NUM
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    if(input[*indx] == 'E' || input[*indx] == 'e'){
        // state 41
        (*indx)++;
    }
    // state 46
    else{
        // return RNUM
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base+1] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    // state 43
    if(input[*indx] == '+' || input[*indx] == '-'){
        (*indx)++;
        // state 45
        if (input[*indx] >= '0' && input[*indx]<='9'){
                while(input[*indx] >= '0' && input[*indx]<='9'){
                (*indx)++;
            }
        }
        // state 44
        else{
            *indx-=2;
            // return RNUM
            lex ls;
            ls.token = RNUM;
            strncpy(ls.lexeme, &input[id_base], *indx-id_base);
            ls.lexeme[*indx-id_base+1] = '\0';
            ls.value.rnum = strtof(ls.lexeme, NULL);
            return ls;
        }
    }
    // state 45
    else if(input[*indx] >= '0' && input[*indx]<='9'){
        if (input[*indx] >= '0' && input[*indx]<='9'){
                while(input[*indx] >= '0' && input[*indx]<='9'){
                (*indx)++;
            }
        }
    }
    // state 42
    else{
        (*indx)--;
        // return RNUM;
        lex ls;
        ls.token = RNUM;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base+1] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        return ls;
    }
    // state 46
    // return RNUM
    lex ls;
    ls.token = RNUM;
    strncpy(ls.lexeme, &input[id_base], *indx-id_base);
    ls.lexeme[*indx-id_base+1] = '\0';
    ls.value.rnum = strtof(ls.lexeme, NULL);
    return ls;
}
