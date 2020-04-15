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
void printIDList(FILE *fptr, ASTnode* node, symbolTableNode *stable, int *offset, moduleHashNode* symbolForest[]);
void modulereuseStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[]);
void pushMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void loadIntoMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void storeIntoReg(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void cmpRegMem(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr);
void assignopArrayCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]);
void assignopCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]);
int genArithExpr(ASTnode *node, FILE *fptr);

#endif