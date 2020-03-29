#include "ast.h"
#include "astDef.h"
#include "parser.h"
#include "lexer.h"

ASTnode* allocateAstNode(int label, ASTnode* parent, ASTnode* firstChild, ASTnode* sibling, ptree_node* pt)
{
    ASTnode* node = (ASTnode*) malloc(sizeof(ASTnode));
    node->label = label;
    node->parent = parent;
    node->firstChild = firstChild;
    node->sibling = sibling;
    node->syntaxTreeNode = pt;
    return node;	
}

//traverse tree and create AST
ASTnode* populateAST(ptree_node* root, ASTnode* parent){

    int rule = root->rule;
    ASTnode* syn, inh, current;
      
    switch(rule-1){
        
        //Epsilon rules

        case 3: //moduledeclarations -> EPSILON
        case 6: //othermodules -> EPSILON
        case 10: //ret -> EPSILON
        case 13: //n1 -> EPSILON
        case 16: //n2 -> EPSILON
        case 27: //statements -> EPSILON
        case 55: //optional -> EPSILON
        case 58: //n3 -> EPSILON
        case 95: //n9 -> EPSILON
        case 100: //dflt -> EPSILON
            current = NULL;
            break;

        //Child passing to parent directly for collapsing long chains

        case 7: //drivermodule -> DRIVERDEF DRIVER PROGRAM DRIVERENDDEF moduledef
            current = populateAST(root->children[4], parent);
            break;

        case 9: //ret -> RETURNS SQBO outputplist SQBC SEMICOL
            current = populateAST(root->children[2], parent);
            break;

        case 25: //moduledef -> START statements END
            current = populateAST(root->children[1], parent);
            break;

        case 28: //statement -> iostmt
            current = populateAST(root->children[0], parent);
            break;
            
        case 29: //statement -> simplestmt
            current = populateAST(root->children[0], parent);
            break;
             
        case 30: //statement -> declarestmt
            current = populateAST(root->children[0], parent);
            break;         

        case 31: //statement->conditionalstmt
            current = populateAST(root->children[0], parent);
            break;

        case 32: //statement -> iterativestmt
            current = populateAST(root->children[0], parent);
            break;
            
        case 40: //var -> varidnum
            current = populateAST(root->children[0], parent);
            break;
    
        case 41: //var -> boolconstt
            current = populateAST(root->children[0], parent);
            break;
        
        case 44: //simplestmt -> assignstmt 
            current = populateAST(root->children[0], parent);
            break;
            
        case 45: //simplestmt -> modulereusestmt
            current = populateAST(root->children[0], parent);
            break;
        
        case 54: //optional -> SQBO idlist SQBC ASSIGNOP
            current = populateAST(root->children[1], parent);
            break;

        case 59: //expression -> arithmeticorbooleanexpr
            current = populateAST(root->children[0], parent);
            break;

        case 60: //expression -> u
            current = populateAST(root->children[0], parent);
            break;

        case 62: //newnt -> BO arithmeticexpr BC
            current = populateAST(root->children[1], parent);
            break;

        case 63: //newnt -> varidnum
            current = populateAST(root->children[0], parent);
            break;

        case 68: //anyterm -> boolconstt
            current = populateAST(root->children[0], parent);
            break;

        case 77: //factor -> BO arithmeticorbooleanexpr BC
            current = populateAST(root->children[1], parent);
            break;

        case 78: //factor -> varidnum
            current = populateAST(root->children[0], parent);
            break;

        case 99: //dflt -> DEFAULT COLON statements BREAK SEMICOL
            current = populateAST(root->children[2], parent);
            break;
            
        //Create leaf nodes using syntax tree nodes

        case 4: //moduledeclaration -> DECLARE MODULE ID SEMICOL
            current = allocateAstNode(MODULEDECLARATIONS_NODE, parent, NULL, NULL, root->children[2]);
            break;

        case 17: //datatype -> INTEGER
            current = allocateAstNode(INTEGER_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 18: //datatype -> REAL
            current = allocateAstNode(REAL_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 19: //datatype -> BOOLEAN
            current = allocateAstNode(BOOLEAN_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 22: //type -> INTEGER
            current = allocateAstNode(INTEGER_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 23: // type -> REAL
            current = allocateAstNode(REAL_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 24: //type -> BOOLEAN
            current = allocateAstNode(BOOLEAN_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 35: //boolconstt -> TRUE
            current = allocateAstNode(TRUE_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 36: //boolconstt -> FALSE
            current = allocateAstNode(FALSE_NODE, parent, NULL, NULL, root->children[0]);
            break;
                    
        case 38: //varidnum -> NUM
            current = allocateAstNode(NUM_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 39: //varidnum -> RNUM
            current = allocateAstNode(RNUM_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 51: //ind -> NUM
            current = allocateAstNode(NUM_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 52: //ind -> ID
            current = allocateAstNode(ID_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 79: //op1->PLUS
            current = allocateAstNode(PLUS_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 80: //op1 -> MINUS
            current = allocateAstNode(MINUS_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 81: //op2 -> MUL
            current = allocateAstNode(MUL_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 82: //op2 -> DIV
            current = allocateAstNode(DIV_NODE, parent, NULL, NULL, root->children[0]);
            break;
        
        case 83: //logicalop -> AND
            current = allocateAstNode(AND_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 84: //logicalop -> OR
            current = allocateAstNode(OR_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 85: //relationalop -> LT        
            current = allocateAstNode(LT_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 86: //relationalop -> LE
            current = allocateAstNode(LE_NODE, parent, NULL, NULL, root->children[0]);
            break; 

        case 87: ///relationalop -> GT
            current = allocateAstNode(GT_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 88: //relationalop -> GE
            current = allocateAstNode(GE_NODE, parent, NULL, NULL, root->children[0]);
            break;
        
        case 89: //relationalop -> EQ
            current = allocateAstNode(EQ_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 90: //relationalop -> NE
            current = allocateAstNode(NE_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 96: //value -> NUM
            current = allocateAstNode(NUM_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 97: //value -> TRUE
            current = allocateAstNode(TRUE_NODE, parent, NULL, NULL, root->children[0]);
            break;

        case 98: //value -> FALSE
            current = allocateAstNode(FALSE_NODE, parent, NULL, NULL, root->children[0]);
            break;

        //Linking the List 
        
        case 2: //moduledeclarations -> moduledeclaration moduledeclarations1
            current = populateAST(root->children[0], parent);
            if(current!=NULL){
                current-> sibling = populateAST(root->children[1], parent);
            }else{
                printf("Error moduledec");
            }
            break;
            
        case 5: //othermodules -> module othermodules1
            current = populateAST(root->children[0], parent);
            if(current!=NULL){
                current-> sibling = populateAST(root->children[1], parent);
            }else{
                printf("Error module");
            }
            break;
        
        case 11: //inputplist -> ID COLON datatype n1
            current = populateAST(root->children[2], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID,current,NULL, NULL,root->children[0]);
                current-> sibling = populateAST(root->children[3], parent);
            
            }else{
                printf("Error inputplist");
            }
            break;

        case 12: //n1 -> COMMA ID COLON datatype n11
            current = populateAST(root->children[3], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID,current,NULL, NULL,root->children[0]);
                current-> sibling = populateAST(root->children[4], parent);
            
            }else{
                printf("Error n1");
            }
            break;

        case 14: //outputplist -> ID COLON type n2
            current = populateAST(root->children[2], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID,current,NULL, NULL,root->children[0]);
                current-> sibling = populateAST(root->children[3], parent);
            
            }else{
                printf("Error outputplist");
            }
            break;

        case 15: //n2 -> COMMA ID COLON type n21
            current = populateAST(root->children[3], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID,current,NULL, NULL,root->children[1]);
                current-> sibling = populateAST(root->children[4], parent);
            
            }else{
                printf("Error outputplist");
            }
            break;

        case 26: //statements -> statement statements1
            current = populateAST(root->children[0], parent);
            if(current!=NULL){
                current-> sibling = populateAST(root->children[1], parent);
            }else{
                printf("Error statements");
            }
            break;

        case 56: //idlist -> ID n3
            current = allocateAstNode(ID,root,NULL, NULL,root->children[0]);
            if(current!=NULL){
                current->sibling = populateAST(root->children[1], parent);
            }else{
                printf("Error idplist");
            }
            break;

        case 57: //n3 -> COMMA ID n31
            current = allocateAstNode(ID,root,NULL, NULL,root->children[1]);
            if(current!=NULL){
                current-> sibling = populateAST(root->children[2], parent);
            }else{
                printf("Error n3");
            }
            break;

        case 93: //casestmts -> CASE value COLON statements BREAK SEMICOL n9
            current = populateAST(root->children[1], parent);
            if(current!=NULL){
                current-> firstChild = populateAST(root->children[3], value);
                current-> sibling = populateAST(root->children[6], parent);
            
            }else{
                printf("Error casestmts");
            }
            break;

        case 94: //n9 -> CASE value COLON statements BREAK SEMICOL n91
            current = populateAST(root->children[1], parent);
            if(current!=NULL){
                current-> firstChild = populateAST(root->children[3], value);
                current-> sibling = populateAST(root->children[6], parent);
            
            }else{
                printf("Error n9");
            }
            break;
            
        //Multiple children

        case 1: //program -> moduledeclarations othermodules drivermodule othermodules1
            current = allocateAstNode(PROGRAM_NODE, parent, NULL, NULL, root);
            if(current!=NULL){
                int index = 0;
                ASTnode* temp = populateAST(root->children[index],current);
                ASTnode* temp2 = current;
                while(index < 4){
                    while(index < 4 && temp == NULL){
                        index++;
                        temp = populateAST(root->children[index], current);
                    }
                    if(index == 4){
                        break;
                    }
                    else{
                        if(temp2 == current){
                            current->firstChild = temp;
                            temp2 = temp;
                        }else{
                            temp2->sibling = temp;
                            temp2 = temp;
                        }
                    }
                }
            }
            break;

        
    }
       
     
}