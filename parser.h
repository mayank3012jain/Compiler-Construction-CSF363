//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#ifndef grammarHeader
#define grammarHeader
#include"lexer.h"
#include"parserDef.h"
//Function declarations
gnode* createNode(char* str);
void create_grammar(GRAMMAR gr, int grammarIndex[], char *filename);
long long unsigned int getFirst(int nt, FIRST first, int grammarIndex[], GRAMMAR gr);
void populateFirst(FIRST first, int grammarIndex[], GRAMMAR gr);
void printfirst(int i, unsigned long long int z,char* termString[], char* NtermString[]);
void follow(int enumindx, FIRST fr, GRAMMAR g,FOLLOW fl);
void allFollow(FIRST fr, int grammarIndex[], GRAMMAR gr, FOLLOW fl);
// void printfirst(int enumindx, unsigned long long int value);
void populate_parse_table(GRAMMAR gr, FIRST fst, FOLLOW fol, PARSE_TABLE pt);
stack_node* pop(stack_node* head);
stack_node* push(gnode* arr[], ptree_node* ptrs[], int len, stack_node* head);
ptree_node* create_tree_node(char* lexeme, int rule,int lineNumber, int token, char* symbol, unsigned int isLeaf, ptree_node* parent);
ptree_node* make_parse_tree(GRAMMAR gr, PARSE_TABLE pt,char* input, FIRST first, FOLLOW follow, FILE* fin, int*);
void printNode(ptree_node* node);
void printTree(ptree_node* head);

#endif