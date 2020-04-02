void traverse_ast(ASTnode* root);
void insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, int offset);
void traverse_ast_recurse(ASTnode* root, symbolTable* stable, hashNode* symbolForest[]);
symbolTableEntry* getSymbolTableEntry(symbolTableNode* stNode, char* name);

#define VAR_SYMBOLTABLE_SIZE 100
# define MAX_MODULES 64
# define MAX_SCOPES 64
# define MAX_SCOPE_VARS 64


