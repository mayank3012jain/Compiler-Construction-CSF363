#include"lexer.h"

lex getNextToken(char input[], int* index, int* lineNumber){
    // int index = 0;
    char c = input[*index];
    lex cur;
    // while (c != '\0'){
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
        cur = identifier_dfa(input, index);
        //call parser
        cur.line = lineNumber;
        return cur;
    }else if((c>='0' && c<=9+'0')){
        cur = foo;
        //call parser
    }else if(c=='*'){
        *index = *index + 1;
        c = input[*index];
        if(c=='*'){
            do{
                *index = *index +1;
                if(input[*index]!='\n'){
                    *lineNumber = *lineNumber +1;
                }
            }while(!(input[*index]== '*' && input[*index+1]== '*' ));
            *index = *index +2;
            return getNextToken(input, index, lineNumber);
        }else{
            //cur.token = 
            cur.lexeme[0] = '*';
            cur.lexeme[1] = '\0';
            cur.line = *lineNumber;
            *index = *index +1;
            return cur;
        }

    }
    else{
        switch(c){
            case '+':
                // cur.token = 
                cur.lexeme[0] = '+';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case '-':
                // cur.token =
                cur.lexeme[0] = '-';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case '>':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '=':
                        // cur.token =
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    case '>':
                        // cur.token =
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '>';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token =
                        cur.lexeme[0] = '>';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case '/':
                // cur.token =
                cur.lexeme[0] = '/';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case '!':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '=':
                        // cur.token =
                        cur.lexeme[0] = '!';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token = ERROR
                        cur.line = *lineNumber;
                        return cur;
                        break;                            
                }
                break;
            case ';':
                // cur.token =
                cur.lexeme[0] = ';';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case '<':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '=':
                        // cur.token =
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    case '>':
                        // cur.token =
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '<';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token =
                        cur.lexeme[0] = '<';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case ',':
                // cur.token =
                cur.lexeme[0] = ',';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break; 
            case '=':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '=':
                        // cur.token =
                        cur.lexeme[0] = '=';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token = ERROR
                        cur.line = *lineNumber;
                        return cur;
                        break;                            
                }
                break; 
            case '[':
                // cur.token =
                cur.lexeme[0] = '[';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;            
            case ':':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '=':
                        // cur.token =
                        cur.lexeme[0] = ':';
                        cur.lexeme[1] = '=';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token =
                        cur.lexeme[0] = ':';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case '.':
                *index = *index +1;
                c= input[*index];
                switch(c){
                    case '.':
                        // cur.token =
                        cur.lexeme[0] = '.';
                        cur.lexeme[1] = '.';
                        cur.lexeme[2] = '\0';
                        cur.line = *lineNumber;
                        *index = *index+1;
                        return cur;
                        break;
                    default:
                        // cur.token = ERROR
                        cur.lexeme[0] = '.';
                        cur.lexeme[1] = '\0';
                        cur.line = *lineNumber;
                        return cur;
                        break;
                        
                }
                break;
            case ']':
                // cur.token =
                cur.lexeme[0] = ']';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case '(':
                // cur.token =
                cur.lexeme[0] = '(';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
            case ')':
                // cur.token =
                cur.lexeme[0] = ')';
                cur.lexeme[1] = '\0';
                cur.line = *lineNumber;
                *index = *index +1;
                return cur;
                break;
        }
    }
    // }
}