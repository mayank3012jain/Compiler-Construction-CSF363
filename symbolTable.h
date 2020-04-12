#ifndef symboltable_h
#define symboltable_h
 
#include "astDef.h"
#include "ast.h"

void traverse_ast(ASTnode* root);
int checkKeyword(char *name, ASTnode* ast);
int insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, int offset, int isAssigned, int isReturn, ASTnode* ast);
void traverse_ast_recurse(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[]);
symbolTableEntry* getSymbolTableEntry(symbolTableNode* stNode, char* name);
symbolTableNode* insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[],ASTnode* moduleRoot);
symbolTableEntry* isDeclared(varHashNode* varHashTable[], char* name);
moduleHashNode* getModuleHashNode(char *name, moduleHashNode* symbolForest[]);
int checkFunctionReturnType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]);
int checkFunctionParameterType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]);
// symbolTableNode* getSymbolTableNode(char *name, moduleHashNode* symbolForest[]);
void check_module_dec(char* name, moduleHashNode *symbolForest[], ASTnode* ast);
int check_type(ASTnode* root, symbolTableNode* stable);
int hashGivenIndex(char str[], int lowerIndex, int higherIndex);
symbolTableNode* allocateSymbolTable(symbolTableNode* parent, int offset);
void traverse_ast_recurse2(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[], int scope);
void printSymbolForest(moduleHashNode* symbolForest[]);
void printModuleHashNode(moduleHashNode* modhash);
void printSymbolTableNode(symbolTableNode* symNode);
void printVarHashTable(varHashNode* varht[]);
void printVarHashNode(varHashNode* varhn);
void printSymbolTableEntry(symbolTableEntry* symEntry);



#define VAR_SYMBOLTABLE_SIZE 100
# define MAX_MODULES 64
# define MAX_SCOPES 64
# define MAX_SCOPE_VARS 64


#endif