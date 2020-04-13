#ifndef symboltabledef_h
#define symboltabledef_h

#include "astDef.h"

#define VAR_SYMBOLTABLE_SIZE 100
#define MAX_MODULES 64
#define MAX_SCOPES 64
#define MAX_SCOPE_VARS 64

typedef struct symbolTableNode{
    struct varHashNode* varHashTable[VAR_SYMBOLTABLE_SIZE]; //change this
    struct symbolTableNode* childList[MAX_SCOPES]; //change this
    struct symbolTableNode* parent;
    int running_offset;
    int scopeStart;
    int scopeEnd;
    int nest;
    char key[MAXKEYLEN];
} symbolTableNode;
//change and put node everywhere

typedef struct symbolTableEntry {
    char name[50]; //variable name
    int type;
    int assigned; //semantic error or not if value not assigned
    int isArray;
    int startIndex, endIndex;
    struct symbolTableEntry *startIndexDyn, *endIndexDyn;
    int offset;
    int isAssigned;
    int isReturn;
    int isStatic;
} symbolTableEntry;

typedef struct moduleHashNode{
    char key[MAXKEYLEN];
    symbolTableNode* tablePtr;
    ASTnode* moduleAst;
    struct moduleHashNode* next;
    int isDefined;
    int isUsed;
}moduleHashNode; //SymbolTable TABLE

typedef struct varHashNode{
    char key[MAXKEYLEN];
    symbolTableEntry* entryPtr;
    struct varHashNode* next;
}varHashNode;

#endif