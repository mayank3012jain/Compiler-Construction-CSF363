#ifndef ast_h
#define ast_h

#include "astDef.h"

ASTnode* allocateAstNode(int label, ASTnode* parent, ASTnode* firstChild, ASTnode* sibling, ptree_node* pt);
ASTnode* populateAST(ptree_node* root, ASTnode* parent, ASTnode* inhert);
void printAST(ASTnode* root);
void printASTNode(ASTnode* node);

#endif
