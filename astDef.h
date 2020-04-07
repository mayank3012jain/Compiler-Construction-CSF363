#ifndef astDef_h
#define astDef_h

#include "parserDef.h"
#include "lexerDef.h"

#define LABELSIZE 20

#define VAR_SYMBOLTABLE_SIZE 100
#define MAX_MODULES 64
#define MAX_SCOPES 64
#define MAX_SCOPE_VARS 64

// typedef union attributeType;
//enum type {INTEGER,REAL,BOOLEAN};

extern char* nodeNameString[];
enum nodeName {BOOLEAN_NODE, REAL_NODE, INTEGER_NODE, INPUTPLIST_HEADER_NODE, RET_HEADER_NODE, MODULEDEF_HEADER_NODE, MODULEDEC_HEADER_NODE, OTHERMOD_HEADER_NODE, DRIVER_MOD_NODE, TRUE_NODE, FALSE_NODE, NUM_NODE, RNUM_NODE, ID_NODE, PLUS_NODE, MINUS_NODE, MUL_NODE, DIV_NODE, AND_NODE, OR_NODE, LT_NODE, LE_NODE, GT_NODE, GE_NODE, EQ_NODE, NE_NODE, PROGRAM_NODE, MODULE_NODE, DATATYPE_ARRAY_NODE, RANGEARRAYS_NODE, MODULEREUSESTMT_NODE, GET_STMT_NODE, PRINT_STMT_NODE, VARIDNUM_NODE, ASSIGNOP_NODE, ASSIGNOP_ARRAY_NODE, ID_ARRAY_NODE, U_NODE, DECLARESTMT_NODE, CONDITIONALSTMT_NODE, FORITERATIVESTMT_NODE, WHILEITERATIVESTMT_NODE, RANGE_NODE, RELATIONALOP_NODE, LOGICALOP_NODE,IDLIST_NODE, OPTIONAL_RETURN_NODE, INPUTPLIST_NODE, CASESTMTS_HEADER_NODE};

enum dataTypes {BOOL, FLOAT, INT};
extern int DATA_TYPE_SIZES[];

typedef struct ASTnode{
    int label;
    int isLeaf;
    struct ASTnode* firstChild;
    struct ASTnode* sibling;
    struct ASTnode* parent;
    int numberChildren;
    ptree_node* syntaxTreeNode;
}ASTnode;

typedef struct symbolTableNode{
    struct varHashNode* varHashTable[VAR_SYMBOLTABLE_SIZE]; //change this
    struct symbolTableNode* childList[MAX_SCOPES]; //change this
    struct symbolTableNode* parent;
    int running_offset;
} symbolTableNode;
//change and put node everywhere

typedef struct symbolTableEntry {
    char name[50];//variable name
    int type;
    int assigned; //semantic error or not if value not assigned
    int isArray;
    int startIndex, endIndex;
    int offset;
    int isAssigned;
    int isReturn;
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


/**
typedef struct ptree_node{
    char lexeme[LEX_LENGTH];
    int lineNumber;
    int token;
    int rule;
    number value;
    char symbol[100];
    unsigned int isLeaf;
    struct ptree_node* parentNode;
    struct ptree_node* children[CHILDREN_SIZE];
}ptree_node;
**/


// union attributeType{
//     ptree_node pnode;
//     ASTnode ast;
//     char label[LABELSIZE];
// };

#endif
