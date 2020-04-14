#ifndef codeGen_h
#define codeGen_h

#include "symbolTableDef.h"

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]);
void printSymbolForestCodeGen(moduleHashNode* symbolForest[], FILE* fptr);
void printModuleHashNodeCodeGen(moduleHashNode* modhash, FILE* fptr);
void printSymbolTableNodeCodeGen(symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
void printVarHashTableCodeGen(varHashNode* varht[], symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
void printVarHashNodeCodeGen(varHashNode* varhn, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
void printSymbolTableEntryCodeGen(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr);
char* generateBssLexeme(symbolTableNode* stable, char* name);
char* new_label();
char* new_variable();
void traverse_code_gen(ASTnode* root, FILE *fptr, moduleHashNode* symbolForest[]);
void statementsCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[]);
void printStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable,moduleHashNode* symbolForest[]);
void getStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable,moduleHashNode* symbolForest[]);
void forIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[]);
void whileIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[]);
void conditionalStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[]);

#endif