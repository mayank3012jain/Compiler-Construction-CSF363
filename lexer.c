//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#include<stdio.h>
#include "lexer.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
// enum token {ID,ERROR,NUM,RNUM};
// int check_keyword(char[],int,int);

int hash(char str[]){
    // const int PRIME = 199;
    int length = strlen(str);
    int ans =0;
    for (int i=0; i<length; i++){
        ans += str[i]*(i+1);
    }
    ans %= SIZELOOKTBL;
    return ans;
}

int getEnumVal(char key[]){
    int ind = hash(key);
    hashNode* head = lookupTable[ind];
    hashNode* temp = head;
    while(temp != NULL && strcmp(key, temp->key)!=0){
        //printf("%s %d\n",temp->key, ind);
        temp = temp->next;
    }
    if(temp == NULL){
        // printf("$$$$$$$$$$$$Does Not exist$$$$$$$$$$\n");
        return -1;
    }
    //*tag = temp->tag;
    return temp->value;
}

void populateLookupTable(){
    
    //EOF is enum value of EOF
    for (int i=0; i<COUNTT; i++){
        int ind = hash(termString[i]);
        hashNode* temp = lookupTable[ind];
        if(temp!=NULL){
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = (hashNode*)malloc(sizeof(hashNode));
            temp = temp->next;
            // printf("temp wasnt null for %s", termString[i]);
        }else{
            lookupTable[ind] = (hashNode*)malloc(sizeof(hashNode));
            temp = lookupTable[ind];
            // printf("temp was NULL for %s", termString[i]);
        }
        temp->next=NULL;
        strcpy(temp->key, termString[i]);
        temp->value = i;
        temp->tag = 0;
    }

    for (int i=0; i<countnt; i++){
        int ind = hash(NtermString[i]);
        hashNode* temp = lookupTable[ind];
        if(temp!=NULL){
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = (hashNode*)malloc(sizeof(hashNode));
            temp = temp->next;
            //printf("temp wasnt null for %s", NtermString[i]);
        }else{
            lookupTable[ind] = (hashNode*)malloc(sizeof(hashNode));
            temp = lookupTable[ind];
            //printf("temp was NULL for %s", NtermString[i]);
        }
        temp->next=NULL;
        strcpy(temp->key, NtermString[i]);
        temp->value = i;
        temp->tag = 1;
    }
}

void populateSymbolTable(){
    for (int i=0; i<SIZEKEYWORDS; i++){
        int ind = hash(keywords[i]);
        hashNode* temp = symbolTable[ind];
        if(temp!=NULL){
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = (hashNode*)malloc(sizeof(hashNode));
            temp = temp->next;
            // printf("temp wasnt null for %s", keywords[i]);
        }else{
            symbolTable[ind] = (hashNode*)malloc(sizeof(hashNode));
            temp = symbolTable[ind];
            // printf("temp was NULL for %s", keywords[i]);
        }
        temp->next=NULL;
        strcpy(temp->key, keywords[i]);
        temp->value = i+INTEGER;
        temp->tag = 0;
        // printf("\n%s, %d", keywords[temp->value], temp->value);
    }
} 

void bufferRefill(char input[], FILE* fin){
    char read_char;
    int read_ptr=0;
    while(read_ptr<BUFFER_SIZE-1 &&(read_char = fgetc(fin)) != EOF){
        input[read_ptr] = read_char;
        read_ptr++;
    }
    if(read_ptr<BUFFER_SIZE-1){
        input[read_ptr]=EOF;
    }
    while(input[read_ptr]!=' ' && input[read_ptr]!='\t' && input[read_ptr]!='\n'){
        read_ptr--;
    }
    input[read_ptr+1] = '\0';
    fseek(fin, -((BUFFER_SIZE)-read_ptr+2), SEEK_CUR);


}

void getNextToken(char input[], int* indx, int* lineNumber, lex* tokenRet, FILE* fin){

    if(input[*indx] == '\0'){
        //buffer refill
        bufferRefill(input, fin);
        *indx = *indx - BUFFER_SIZE +1;
    }
    // int indx = 0;
    char c = input[*indx];
    lex cur;
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
        cur = identifier_dfa(input, indx, *lineNumber);
        cur.line = *lineNumber;
        // return cur;
        memcpy(tokenRet,&cur,sizeof(lex));
    }else if((c>='0' && c<='9')){
        cur = number_dfa(input, indx, *lineNumber);
        cur.line = *lineNumber;
        memcpy(tokenRet,&cur,sizeof(lex));
    }// comment
    else if(c=='*'){
        *indx = *indx + 1;
        c = input[*indx];
        if(c=='*'){
            do{
                *indx = *indx +1;
                if(input[*indx]=='\n'){
                    *lineNumber = *lineNumber +1;
                }
            }while(input[*indx+2] != '\0' && !(input[*indx]== '*' && input[*indx+1]== '*' ));
            *indx = *indx +2;
            return getNextToken(input, indx, lineNumber, tokenRet, fin);
        }else{
            cur.token = MUL;
            cur.lexeme[0] = '*';
            cur.lexeme[1] = '\0';
            cur.line = *lineNumber;
            // *indx = *indx +1;
            memcpy(tokenRet,&cur,sizeof(lex));
        }
    }// whitespace
    else if(c==' ' || c== '\t'){
        (*indx)++;
        getNextToken(input,indx,lineNumber,tokenRet, fin);
    }// newline
    else if(c=='\n'){
        *lineNumber = *lineNumber+1;
        *indx = *indx +1;
        getNextToken(input,indx,lineNumber,tokenRet, fin);
    }// EOF
    else if(c==EOF){
        cur.token = DOLLAR;
        cur.lexeme[0] = '\0';
        cur.line = *lineNumber;
        memcpy(tokenRet,&cur,sizeof(lex));
    }
    else{
    	cur = operator_dfa(input,indx,lineNumber);
    	memcpy(tokenRet,&cur,sizeof(lex));
    }
}

lex operator_dfa(char input[], int* indx,int *lineNumber){
    lex cur;
    int base = *indx;
    char errorLexeme[10];
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
                        if(input[*indx+1]=='>'){
                            cur.token = DRIVERENDDEF;
                            cur.lexeme[0] = '>';
                            cur.lexeme[1] = '>';
                            cur.lexeme[2] = '>';
                            cur.lexeme[3] = '\0';
                            cur.line = *lineNumber;
                            *indx = *indx + 2;
                            return cur;
                        }
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
                        strncpy(errorLexeme, input+base, *indx - base);
                        errorLexeme[*indx - base] = '\0';
                        printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", *lineNumber, errorLexeme);
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
                        if(input[*indx+1]=='<'){
                            cur.token = DRIVERDEF;
                            cur.lexeme[0] = '<';
                            cur.lexeme[1] = '<';
                            cur.lexeme[2] = '<';
                            cur.lexeme[3] = '\0';
                            cur.line = *lineNumber;
                            *indx = *indx + 2;
                            return cur;
                        }
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
                        strncpy(errorLexeme, input+base, *indx - base);
                        errorLexeme[*indx - base] = '\0';
                        printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", *lineNumber, errorLexeme);
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
                        strncpy(errorLexeme, input+base, *indx - base);
                        errorLexeme[*indx - base] = '\0';
                        printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", *lineNumber, errorLexeme);
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
            // case '\n':
            //     *lineNumber = *lineNumber+1;
            //     *indx = *indx +1;
            //     return getNextToken(input,indx,lineNumber);
            //     break;
            default:
                cur.token = ERROR;
                *indx = *indx +1;
                strncpy(errorLexeme, input+base, *indx - base);
                errorLexeme[*indx - base] = '\0';
                printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", *lineNumber, errorLexeme);
                return cur;
            
        }
}

lex identifier_dfa(char input[], int* indx, int lineNumber){
    int tok,id_base = *indx;
    char tok_name[100];
    (*indx)++;
    while(((input[*indx] >= 'A' && input[*indx]<='Z') || (input[*indx] >= 'a' && input[*indx]<='z') || 
    (input[*indx] >= '0' && input[*indx]<='9') || input[*indx]=='_')){
        (*indx)++;
    }
    strncpy(tok_name,&input[id_base],*indx-id_base);
    tok_name[*indx-id_base] = '\0';
    if((tok=check_keyword(tok_name,*indx,id_base))!=-1){
        lex ls;
        ls.token = tok;
        strcpy(ls.lexeme,tok_name);
        return ls; 
    } 
    if (*indx - id_base <= 20){
        lex ls;
        ls.token = ID;
        // strncpy(ls.lexeme,&input[id_base],*indx-id_base);
        // ls.lexeme[*indx-id_base] = '\0';
        strcpy(ls.lexeme,tok_name);
        return ls;
    }
    else{
        lex ls;
        ls.token = ERROR;
        strcpy(ls.lexeme,tok_name);
        printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", lineNumber, tok_name);
        return ls;
    }
}

lex number_dfa(char input[], int* indx, int lineNumber){
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
    else{//ERROR???
        (*indx)--;
        // return NUM
        lex ls;
        ls.token = NUM;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base] = '\0';
        ls.value.rnum = atoi(ls.lexeme);
        // printf("\nLexical error. Line number: %d. Lexeme: %s\n", lineNumber, ls.lexeme);
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
        ls.lexeme[*indx-id_base] = '\0';
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
        else{//change ERROR??
            // *indx-=2;
            // return RNUM
            lex ls;
            ls.token = ERROR;
            ls.line = lineNumber;
            strncpy(ls.lexeme, &input[id_base], *indx-id_base);
            ls.lexeme[*indx-id_base] = '\0';
            // ls.value.rnum = strtof(ls.lexeme, NULL);
            printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", lineNumber, ls.lexeme);
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
        // (*indx)--;
        // return RNUM;
        lex ls;
        ls.token = ERROR;
        strncpy(ls.lexeme, &input[id_base], *indx-id_base);
        ls.lexeme[*indx-id_base] = '\0';
        ls.value.rnum = strtof(ls.lexeme, NULL);
        printf("\nLEXICAL ERROR at line %d. Lexeme: %s\n", lineNumber, ls.lexeme);
        return ls;
    }
    // state 46
    // return RNUM
    lex ls;
    ls.token = RNUM;
    strncpy(ls.lexeme, &input[id_base], *indx-id_base);
    ls.lexeme[*indx-id_base] = '\0';
    ls.value.rnum = strtof(ls.lexeme, NULL);
    return ls;
}

int check_keyword(char key[],int start, int end){

    int ind = hash(key);
    hashNode* head = symbolTable[ind];
    hashNode* temp = head;
    while(temp != NULL && strcmp(key, temp->key)!=0){
        //printf("%s %d\n",temp->key, ind);
        temp = temp->next;
    }
    if(temp == NULL){
        // printf("$$$$$$$$$$$$Does Not exist$$$$$$$$$$\n");
        return -1;
    }
    //*tag = temp->tag;
    return temp->value;
}