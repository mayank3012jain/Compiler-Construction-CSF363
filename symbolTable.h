//GROUP 10
//2017A7PS0179P - MAYANK JAIN
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE

#ifndef symboltable_h
#define symboltable_h
 
#include "astDef.h"
#include "symbolTableDef.h"
#include "ast.h"

void traverse_ast(ASTnode* root, moduleHashNode* symbolForest[]);
void traverse_ast_recurse(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[]);
void traverse_ast_recurse2(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[], int scope);

int checkKeyword(char *name, ASTnode* ast);
int hashGivenIndex(char str[], int lowerIndex, int higherIndex);
symbolTableNode* allocateSymbolTable(symbolTableNode* parent, int offset, int scopeStart, char key[]);
symbolTableNode* insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[],ASTnode* moduleRoot);
int insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, symbolTableEntry* startIndexDyn, symbolTableEntry* endIndexDyn, int offset, int isAssigned, int isReturn, int isStatic,ASTnode* ast);
moduleHashNode* getModuleHashNode(char *name, moduleHashNode* symbolForest[],int lineNumb);
symbolTableEntry* getSymbolTableEntry(symbolTableNode* stNode, char* name);

symbolTableEntry* isDeclared(varHashNode* varHashTable[], char* name);
int checkFunctionReturnType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]);
int checkFunctionParameterType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]);
void check_module_dec(char* name, moduleHashNode *symbolForest[], ASTnode* ast);
int check_type(ASTnode* root, symbolTableNode* stable);

whileList* checkWhileExprn(ASTnode* root, symbolTableNode* stable, whileList* list);
void checkWhileIsAssigned(symbolTableNode* stable, whileList* list, int line);


//print
void printSymbolForest(moduleHashNode* symbolForest[]);
int printModuleHashNode(moduleHashNode* modhash, int correction);
int printSymbolTableNode(symbolTableNode* symNode, moduleHashNode* modhash, int correction);
int printVarHashTable(varHashNode* varht[], symbolTableNode* symNode, moduleHashNode* modhash, int correction);
int printVarHashNode(varHashNode* varhn, symbolTableNode* symNode, moduleHashNode* modhash, int correction);
int printSymbolTableEntry(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash, int correction);
void printArray(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash);

void activationSizeAll(moduleHashNode* symbolForest[]);
int activationSizeUtil(symbolTableNode* stable);
void activationSizePrint(char* name, int size);



#define VAR_SYMBOLTABLE_SIZE 100
# define MAX_MODULES 64
# define MAX_SCOPES 64
# define MAX_SCOPE_VARS 64


#endif