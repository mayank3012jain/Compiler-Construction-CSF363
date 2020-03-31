#ifndef astDef_h
#define astDef_h

#include "parserDef.h"

#define LABELSIZE 20

// typedef union attributeType;
//enum type {INTEGER,REAL,BOOLEAN};

extern char* nodeNameString[];
enum nodeName {INPUTPLIST_HEADER_NODE, RET_HEADER_NODE, MODULEDEF_HEADER_NODE, MODULEDEC_HEADER_NODE, OTHERMOD_HEADER_NODE, DRIVER_MOD_NODE, MODULEDECLARATIONS_NODE, INTEGER_NODE, REAL_NODE, BOOLEAN_NODE, TRUE_NODE, FALSE_NODE, NUM_NODE, RNUM_NODE, ID_NODE, PLUS_NODE, MINUS_NODE, MUL_NODE, DIV_NODE, AND_NODE, OR_NODE, LT_NODE, LE_NODE, GT_NODE, GE_NODE, EQ_NODE, NE_NODE, PROGRAM_NODE, MODULE_NODE, DATATYPE_ARRAY_NODE, RANGEARRAYS_NODE, MODEULEREUSESTMT_NODE, GET_STMT_NODE, PRINT_STMT_NODE, VARIDNUM_NODE, ASSIGNOP_NODE, ASSIGNOP_ARRAY_NODE, ID_ARRAY_NODE, U_NODE, OP1_NODE, DECLARESTMT_NODE, CONDITIONALSTMT_NODE, FORITERATIVESTMT_NODE, WHILEITERATIVESTMT_NODE, RANGE_NODE, OP2_NODE, RELATIONALOP_NODE, LOGICALOP_NODE,IDLIST_NODE, OPTIONAL_RETURN_NODE, INPUTPLIST_NODE};

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
}ASTnode;

// typedef struct symbolTable {
//     char name[50];//variable name
//     int type;
//     int assigned; //semantic error or not if value not assigned
//     int isArray;
//     int startIndex, endIndex;
//     int offset;    
// } symbolTable;

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