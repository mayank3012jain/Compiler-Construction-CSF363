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
void statements(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope);
void printStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable);
void getStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable);
void forIterStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable);
void whileIterStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable);
void conditionalStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable);

#endif