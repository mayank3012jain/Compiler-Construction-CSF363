//GROUP 10
//2017A7PS0179P - MAYANK JAIN
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE

#ifndef codeGen_h
#define codeGen_h

#include "symbolTableDef.h"

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]);
void printSymbolForestCodeGen(moduleHashNode* symbolForest[], FILE* fptr);
void printModuleHashNodeCodeGen(moduleHashNode* modhash, FILE* fptr);
// void printSymbolTableNodeCodeGen(symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
// void printVarHashTableCodeGen(varHashNode* varht[], symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
// void printVarHashNodeCodeGen(varHashNode* varhn, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
// void printSymbolTableEntryCodeGen(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
char* generateBssLexeme(symbolTableNode* stable, char* name);
char* new_label();
char* new_variable();
char* getReturnOffset(char* name, symbolTableNode* symNode, int* retOffset, int size);
void traverse_code_gen(ASTnode* root, FILE *fptr, moduleHashNode* symbolForest[]);
void statementsCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset, int size);
void printStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable,moduleHashNode* symbolForest[], int* retOffset, int size);
void getStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable,moduleHashNode* symbolForest[], int* retOffset, int size);
void forIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset, int size);
void whileIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset, int size);
void conditionalStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset, int size);
void printIDList(FILE *fptr, ASTnode* node, symbolTableNode *stable, moduleHashNode* symbolForest[], int* retOffset, int size);
void modulereuseStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int* retOffset, int size);
void pushMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void loadIntoMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void storeIntoReg(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void cmpRegMem(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void assignopArrayCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size);
void assignopCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size);
void moduleCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *scope);
int returnCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *scope, int *retOffset);
void declareStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int* retOffset, int size);
// int genArithExpr(ASTnode *node, FILE *fptr);
int genExpr(ASTnode *node, FILE *fptr, int interm_counter, symbolTableNode* stable, int* retOffset, int size);
void pushRetParameters(FILE *fptr, ASTnode* node, symbolTableNode *stable, moduleHashNode* symbolForest[], int* retOffset, int size);

void getArrayElement(char* reg, char* reg1, char* i, symbolTableEntry* arrSymEntry, symbolTableNode* stable, FILE* fptr, int* retOffset, int size);

#endif