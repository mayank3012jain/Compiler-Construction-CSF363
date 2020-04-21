//GROUP 10
//2017A7PS0179P - MAYANK JAIN
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE

#ifndef parserDefHeader
#define parserDefHeader

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include"lexerDef.h"
#define SETSIZE 150
#define TABLESIZE 150 
#define MAXKEYLEN 50
#define MAXRULESIZE 200
#define SIZELOOKTBL 199
#define TERMINAL 0
#define NON_TERMINAL 1
#define CHILDREN_SIZE 15

extern int PARSETREENODES;

//Type definitions
typedef struct gnode{
    int term;
    unsigned int tag:1; // 1 for NT, 0 for T
    struct gnode *next;
} gnode; //GRAMMAR

typedef struct{
    uint64_t val;
} fnode; //FIRST and FOLLOW

typedef struct hashNode{
    char key[MAXKEYLEN];
    int value;
    int tag:1;
    struct hashNode* next;
}hashNode; //LOOKUP TABLE


typedef struct ptree_node{
    char lexeme[LEX_LENGTH];
    int rule;
    int lineNumber;
    int token;
    number value;
    char symbol[100];
    unsigned int isLeaf;
    struct ptree_node* parentNode;
    struct ptree_node* children[CHILDREN_SIZE];
}ptree_node;

typedef struct stack_node{ 
    int term;
    unsigned int tag:1;
    struct ptree_node* ptr;
    struct stack_node* next;
}stack_node;

typedef int PARSE_TABLE[countnt][COUNTT];
typedef gnode* GRAMMAR[TABLESIZE];
typedef fnode FOLLOW[TABLESIZE];
typedef fnode FIRST[TABLESIZE];


extern hashNode* lookupTable[];
extern hashNode* symbolTable[];

#endif