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

    if(root == NULL){
        return NULL;
    }
    int rule = root->rule;
    ASTnode *syn, *inh, *current, *temp, *temp2;//check if correct
    int index;
      
    switch(rule+1){
        
        //Epsilon rules

        case 3: //moduledeclarations -> EPSILON
        case 6: //othermodules -> EPSILON
        case 10: //ret -> EPSILON
        case 13: //n1 -> EPSILON
        case 16: //n2 -> EPSILON
        case 27: //statements -> EPSILON
        case 43: //whichid -> EPSILON
        case 55: //optional -> EPSILON
        case 58: //n3 -> EPSILON
        case 66: //n7 -> EPSILON
        case 70: //n8 -> EPSILON
        case 73: //n4 -> EPSILON
        case 76: //n5 -> EPSILON
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

        case 42: //whichid -> SQBO ind SQBC
            current = populateAST(root->children[1],parent);
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
            current = allocateAstNode(ID_NODE, parent, NULL, NULL, root->children[2]);
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
            current = allocateAstNode(INPUTPLIST_NODE, parent, NULL, NULL, NULL);
            if(current!=NULL){
                current-> firstChild = populateAST(root->children[2], current);
                current->firstChild->sibling =  allocateAstNode(ID_NODE,current,NULL, NULL,root->children[0]);
                current-> sibling = populateAST(root->children[3], parent);
            
            }else{
                printf("Error inputplist");
            }
            break;

        case 12: //n1 -> COMMA ID COLON datatype n11
            current = allocateAstNode(INPUTPLIST_NODE, parent, NULL, NULL, NULL);
            if(current!=NULL){
                current->firstChild = populateAST(root->children[3], parent);
                current->firstChild->sibling = allocateAstNode(ID_NODE,current,NULL, NULL,root->children[1]);
                current->sibling = populateAST(root->children[4], parent);
            }else{
                printf("Error n1");
            }
            break;

        case 14: //outputplist -> ID COLON type n2
            current = populateAST(root->children[2], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID_NODE,current,NULL, NULL,root->children[0]);
                current-> sibling = populateAST(root->children[3], parent);
            
            }else{
                printf("Error outputplist");
            }
            break;

        case 15: //n2 -> COMMA ID COLON type n21
            current = populateAST(root->children[3], parent);
            if(current!=NULL){
                current-> firstChild = allocateAstNode(ID_NODE,current,NULL, NULL,root->children[1]);
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
            current = allocateAstNode(ID_NODE,parent,NULL, NULL,root->children[0]);
            if(current!=NULL){
                current->sibling = populateAST(root->children[1], parent);
            }else{
                printf("Error idplist");
            }
            break;

        case 57: //n3 -> COMMA ID n31
            current = allocateAstNode(ID_NODE,parent,NULL, NULL,root->children[1]);
            if(current!=NULL){
                current-> sibling = populateAST(root->children[2], parent);
            }else{
                printf("Error n3");
            }
            break;

        case 93: //casestmts -> CASE value COLON statements BREAK SEMICOL n9
            
            current = populateAST(root->children[1], parent);
            if(current!=NULL){
                current-> firstChild = populateAST(root->children[3], current);
                current-> sibling = populateAST(root->children[6], parent);
            
            }else{
                printf("Error casestmts");
            }
            break;

        case 94: //n9 -> CASE value COLON statements BREAK SEMICOL n91
            current = populateAST(root->children[1], parent);
            if(current!=NULL){
                current-> firstChild = populateAST(root->children[3], current);
                current-> sibling = populateAST(root->children[6], parent);
            
            }else{
                printf("Error n9");
            }
            break;
            
        //Multiple children

        case 1: //program -> moduledeclarations othermodules drivermodule othermodules1
            current = allocateAstNode(PROGRAM_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(MODULEDEC_HEADER_NODE, current, NULL, NULL, NULL);
            current->firstChild->firstChild = populateAST(root->children[0], current->firstChild);
            current->firstChild->sibling = allocateAstNode(OTHERMOD_HEADER_NODE, current, NULL, NULL, NULL);
            current->firstChild->sibling->firstChild = populateAST(root->children[1], current->firstChild->sibling);
            current->firstChild->sibling->sibling = allocateAstNode(DRIVER_MOD_NODE, current, NULL, NULL, NULL);
            current->firstChild->sibling->sibling->firstChild = populateAST(root->children[2], current->firstChild->sibling->sibling);
            current->firstChild->sibling->sibling->sibling = allocateAstNode(OTHERMOD_HEADER_NODE, current, NULL, NULL, NULL);
            current->firstChild->sibling->sibling->sibling->firstChild = populateAST(root->children[3], current->firstChild->sibling->sibling->sibling);
            
            break;

        case 8: //module -> DEF MODULE ID ENDDEF TAKES INPUT SQBO inputplist SQBC SEMICOL ret moduledef
            current = allocateAstNode(MODULE_NODE, parent, NULL, NULL, root);
            //check the last Parameter of allocateASTNode
            current->firstChild = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[2]);
            current->firstChild->sibling = allocateAstNode(INPUTPLIST_HEADER_NODE, current, NULL, NULL, root);
            current->firstChild->sibling->firstChild = populateAST(root->children[7], current->firstChild->sibling);
            current->firstChild->sibling->sibling = allocateAstNode(RET_HEADER_NODE, current, NULL, NULL, root);
            current->firstChild->sibling->sibling->firstChild = populateAST(root->children[10], current->firstChild->sibling->sibling);
            current->firstChild->sibling->sibling->sibling = allocateAstNode(MODULEDEF_HEADER_NODE, current, NULL, NULL, root);
            current->firstChild->sibling->sibling->sibling->firstChild = populateAST(root->children[11], current->firstChild->sibling->sibling->sibling);
            break;

        case 20://datatype -> ARRAY SQBO rangearrays SQBC OF type
            current = allocateAstNode(DATATYPE_ARRAY_NODE, parent, NULL, NULL, root);
            current->firstChild =  populateAST(root->children[2], current);
            current->firstChild->sibling = populateAST(root->children[5], current);
            break;

        case 21: //rangearrays -> ind RANGEOP ind1
            current = allocateAstNode(RANGEARRAYS_NODE, parent, NULL, NULL, root);
            current->firstChild = populateAST(root->children[0], current);
            current->firstChild->sibling = populateAST(root->children[2], current);
            break;
            
        case 53: // modulereusestmt -> optional USE MODULE ID WITH PARAMETERS idlist SEMICOL
            current = allocateAstNode(MODULEREUSESTMT_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(OPTIONAL_RETURN_NODE, current, NULL, NULL, NULL);
            current->firstChild->firstChild = populateAST(root->children[0], current->firstChild);
            current->firstChild->sibling = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[3]);
            current->firstChild->sibling->sibling = allocateAstNode(IDLIST_NODE, current, NULL, NULL, NULL);
            current->firstChild->sibling->sibling->firstChild = populateAST(root->children[6], current->firstChild->sibling->sibling);
            break;
            
        
        case 33: //iostmt -> GET_VALUE BO ID BC SEMICOL
            current = allocateAstNode(GET_STMT_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[2]);
            break;

        case 34: //iostmt -> PRINT BO var BC SEMICOL
            current = allocateAstNode(PRINT_STMT_NODE, parent, NULL, NULL, root);
            current->firstChild = populateAST(root->children[2], current);
            break;

        case 37: //varidnum -> ID whichid
            current = allocateAstNode(VARIDNUM_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[0]);
            temp = populateAST(root->children[1], current);
            if (temp != NULL)
                current->firstChild->sibling = temp;
            break; 
            
        //assignment statement rules 46 to 50, assignstmt_node will have its 2 children as lhs and rhs of assignop
        
        case 46: //assignstmt -> ID whichstmt
            current = populateAST(root->children[1], current);
            if(current->label == ASSIGNOP_NODE){
                inh = allocateAstNode(ID_NODE, NULL, NULL, NULL, root->children[0]);
                syn = current->firstChild;
                current->firstChild = inh;
                inh->parent = current;
                current->firstChild->sibling = syn;
            }else if(current->label == ASSIGNOP_ARRAY_NODE){
                inh = allocateAstNode(ID_NODE, NULL, NULL, NULL, root->children[0]);
                syn = current->firstChild;
                current->firstChild = allocateAstNode(ID_ARRAY_NODE, current, inh, syn->sibling, NULL);
                inh->parent = current->firstChild;
                inh->sibling = syn;
                syn->sibling = NULL;
                syn->parent = current->firstChild;
            }else{
                printf("Error assignop");
            }
            break;
            
        case 47: //whichstmt -> lvalueidstmt
            current = populateAST(root->children[0],parent);
            break;

        case 48: //whichstmt -> lvaluearrstmt
            current = populateAST(root->children[0],parent);
            break;

        case 49: //lvalueidstmt -> ASSIGNOP expression SEMICOL
            current = allocateAstNode(ASSIGNOP_NODE, parent, NULL, NULL, root->children[0]);
            current->firstChild = populateAST(root->children[1], current);
            break;
        
        case 50: //lvaluearrstmt -> SQBO ind SQBC ASSIGNOP expression SEMICOL
            current = allocateAstNode(ASSIGNOP_ARRAY_NODE, parent, NULL, NULL, root->children[0]);
            current->firstChild = populateAST(root->children[1], current);
            current->firstChild->sibling = populateAST(root->children[4], current);
            break;

        // change?
        case 61: //u -> op1 newnt
            current = populateAST(root->children[0], current);
            current->firstChild= populateAST(root->children[1], current);
            break;

        case 91: //declarestmt -> DECLARE idlist COLON datatype SEMICOL
            current = allocateAstNode(DECLARESTMT_NODE, parent, NULL, NULL, root);

            current->firstChild = populateAST(root->children[3],current);
            current->firstChild->sibling = populateAST(root->children[1],current);
            break;
            
        case 92: //conditionalstmt -> SWITCH BO ID BC START casestmts dflt END
            current = allocateAstNode(CONDITIONALSTMT_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[2]);
            current->firstChild->sibling = allocateAstNode(CASESTMTS_HEADER_NODE, current, NULL, NULL, NULL); 
            current->firstChild->sibling->firstChild = populateAST(root->children[5], current->firstChild->sibling);
            temp = populateAST(root->children[6], current);
            if (temp != NULL)
                current->firstChild->sibling->sibling = temp;
            
            break;
    
        case 101: //iterativestmt -> FOR BO ID IN range BC START statements END
            current = allocateAstNode(FORITERATIVESTMT_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(ID_NODE, current, NULL, NULL, root->children[2]);
            current->firstChild->sibling = populateAST(root->children[4], current);
            current->firstChild->sibling->sibling = populateAST(root->children[7], current);
            break;

        case 102: //iterativestmt -> WHILE BO arithmeticorbooleanexpr BC START statements END
            current = allocateAstNode(WHILEITERATIVESTMT_NODE, parent, NULL, NULL, root);
            current->firstChild = populateAST(root->children[2], current);
            current->firstChild->sibling = populateAST(root->children[5], current);
            break; 
    
        case 103: //range -> NUM RANGEOP NUM1
            current = allocateAstNode(RANGE_NODE, parent, NULL, NULL, root);
            current->firstChild = allocateAstNode(NUM_NODE, current, NULL, NULL, root->children[0]);
            current->firstChild->sibling = allocateAstNode(NUM_NODE, current, NULL, NULL, root->children[2]);
            break;

        //Expression rules

        case 75: //n5 -> op2 factor n51
            current = populateAST(root->children[0], current);
            current->firstChild = populateAST(root->children[1], current);
            temp = populateAST(root->children[2], current);
            
            if (temp != NULL){ 
                current->firstChild->sibling = temp->firstChild;
                temp->firstChild = current->firstChild;
                current->firstChild = temp;
                temp->firstChild->parent = temp;
            }
            break;

        case 74: //term -> factor n5
            current = populateAST(root->children[1], parent);
            temp = populateAST(root->children[0], current);

            if (current == NULL){
                current = temp;
            }
            else
            {
                temp->sibling = current->firstChild;
                temp->parent = current;
                current->firstChild = temp;
            }
            break;
            
        case 72: //n4 -> op1 term n41
            current = populateAST(root->children[0], current);
            current->firstChild = populateAST(root->children[1], current);
            temp = populateAST(root->children[2], current);
            
            if (temp != NULL){ 
                current->firstChild->sibling = temp->firstChild;
                temp->firstChild = current->firstChild;
                current->firstChild = temp;
                temp->firstChild->parent = temp;
            }
            break;

        case 71: //arithmeticexpr -> term n4
            current = populateAST(root->children[1], parent);
            temp = populateAST(root->children[0], current);

            if (current == NULL){
                current = temp;
            }
            else
            {
                temp->sibling = current->firstChild;
                temp->parent = current;
                current->firstChild = temp;
            }
            break;

        case 69: //n8 -> relationalop arithmeticexpr
            current = populateAST(root->children[0], current);
            current->firstChild = populateAST(root->children[1], current);
            break;


        case 67: //anyterm -> arithmeticexpr n8
            current = populateAST(root->children[1], parent);
            ASTnode *temp = populateAST(root->children[0], parent);
            if (current == NULL){
                current = temp;
            }
            else{
                temp->sibling = current->firstChild;
                temp->parent = current;
                current->firstChild = temp;
            }
            break;//current ka first chid change nahi kiya?//fixed
            

        case 65: //n7 -> logicalop anyterm n71
            current = populateAST(root->children[0], parent);
            current->firstChild = populateAST(root->children[1], current);
            temp = populateAST(root->children[2], current);
            
            if (temp != NULL){ 
                current->firstChild->sibling = temp->firstChild;
                temp->firstChild = current->firstChild;
                current->firstChild = temp;
                temp->firstChild->parent = temp;
            }
            break;

        case 64: //arithmeticorbooleanexpr -> anyterm n7
            current = populateAST(root->children[1], parent);
            temp = populateAST(root->children[0], current);

            if (current == NULL){
                current = temp;
            }
            else
            {
                temp->sibling = current->firstChild;
                temp->parent = current;
                current->firstChild = temp;
            }
            break;

        default:
            printf("Error in parsetree rule number\n");

    }

    //Returning

    if (current != NULL)
        current->parent = parent;

    return current;     
}


void printAST(ASTnode* root){
    printASTNode(root);
    if(root->firstChild!=NULL){
        printAST(root->firstChild);
    }
    if(root->sibling!=NULL){
        printAST(root->sibling);
    }
    return;

}

void printASTNode(ASTnode* node){
    if(node->parent){
    //     if(node->syntaxTreeNode){
    //         printf("\nlabelEnum: %d Label:%s Parent:%d %s \t\t SyntaxTreeLex: %s", node->label, nodeNameString[node->label], node->parent->label, nodeNameString[node->parent->label], node->syntaxTreeNode->lexeme);
    //     }else{
            printf("\nlabelEnum: %d Label:%s Parent:%d %s", node->label, nodeNameString[node->label], node->parent->label, nodeNameString[node->parent->label]);
    //    }
    }else{
        printf("\nlabelEnum: %d Label:%s Parent:%d %s", node->label, nodeNameString[node->label], -1, "NULL");
    }
}
