#ifndef astDef_h
#define astDef_h

#include parserDef.h

#define LABELSIZE 20

typedef union attributeType;
enum type {INTEGER,REAL,BOOLEAN};
enum nodeName {MODULEDECLARATIONS_NODE, }

typedef struct ASTnode{
    int label;
    int isLeaf;
    struct ASTnode* firstChild;
    struct ASTnode* sibling;
    struct ASTnode* parent;
    int numberChildren;
    ptree_node* syntaxTreeNode;
    // attributeType inh;//what will be the type
    // attributeType syn;//what will be the type
    // int inhtag;
    // int syntag;
} ASTnode;

typedef struct symbolTable {
    char name[50];//variable name
    int type;
    int assigned; //semantic error or not if value not assigned
    int isArray;
    int startIndex, endIndex;
    int offset;    
} symbolTable;

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


union attributeType{
    ptree_node pnode;
    ASTnode ast;
    char label[LABELSIZE];
};

#endif