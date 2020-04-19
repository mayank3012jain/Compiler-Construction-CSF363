//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#ifndef lexerHeader
#define lexerHeader
#include"lexerDef.h"
#include"parserDef.h"
//Function declarations
void callComments(char read_buffer[]);
void lexerCall(char read_buffer[], FILE* fin);
ptree_node* callBoth(GRAMMAR gr, PARSE_TABLE pt, char* read_buffer, FIRST first, FOLLOW follow, FILE* fin);
void callTime(GRAMMAR gr, PARSE_TABLE pt, char* read_buffer, FIRST first, FOLLOW follow, FILE* fin);
int hash(char str[]);
int getEnumVal(char key[]);
void populateLookupTable();
void populateSymbolTable();
void getNextToken(char*, int*, int*, lex*, FILE* fin, int*);
int check_keyword(char[],int,int);
lex operator_dfa(char*, int*, int*, int*);
lex identifier_dfa(char*, int*, int, int*);
lex number_dfa(char*, int*, int, int*);
void bufferRefill(char input[], FILE* fin);

#endif