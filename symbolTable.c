#include "astDef.h"
#include "ast.h"
#include"symbolTable.h"

//types checks for module
// 1. The types and the number of parameters returned by a function must be the same as that of the parameters used in invoking the function.
// 2. The parameters being returned by a function must be assigned a value. If a parameter does not get a value assigned within the function definition, it should be reported as an error.
// 3. The function that does not return any value, must be invoked appropriately.
// 4. Function input parameters passed while invoking it should be of the same type as those used in the function definition.
// 5. Function overloading is not allowed.
// 6. A function declaration for a function being used (say F1) by another (say F2) must precede the definition of the function using it(i.e. F2), only if the function definition of F1 does not precede the definition of F2.
// 7. If the function definition of F1 precedes function definition of F2(the one which uses F1), then the function declaration of F1 is redundant and is not valid.
// 8. The function cannot be invoked recursively.

//for table of size 5, pass 0 and 4
int hashGivenIndex(char str[], int lowerIndex, int higherIndex){
    // const int PRIME = 199;
    int length = strlen(str);
    int ans=0;
    for (int i=0; i<length; i++){
        ans += str[i]*(i+1);
    }
    ans %= (higherIndex-lowerIndex)+1;
    return ans+lowerIndex;
}

void traverse_ast(ASTnode* root){
    moduleHashNode* symbolForest[MAX_MODULES];
    symbolForest[0] = malloc(sizeof(hashNode));
    symbolForest[0]->tablePtr = malloc(sizeof(symbolTableNode));
    symbolTableNode* stable = symbolForest[0]->tablePtr;
    stable->parent = NULL;
    stable->running_offset = 0;
    //change
    traverse_ast_recurse(root,stable,symbolForest);
}

void traverse_ast_recurse(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[]){    
    if(root->label == INPUTPLIST_HEADER_NODE){       
        ASTnode* ast = root->firstChild;
        //Dynamic array?
        while(ast != NULL){
            if(ast->firstChild->label == INTEGER_NODE || ast->firstChild->label == BOOLEAN_NODE || 
                ast->firstChild->label == REAL_NODE){
                int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->label, 0, 0, 0, stable->running_offset, 1,0);
                stable->running_offset += x;//check once again
            }
            else{ 
                int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->firstChild->sibling->label, 1, 
                        ast->firstChild->firstChild->firstChild->syntaxTreeNode->value.num, 
                        ast->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num, stable->running_offset, 1,0);
                stable->running_offset += x;
            }
            ast = ast->sibling;
        }        
    }

    if(root->label == RET_HEADER_NODE){       
        ASTnode* ast = root->firstChild;
        while(ast!= NULL){
            int x = insert_into_stable(stable->varHashTable, ast->firstChild->syntaxTreeNode->lexeme, 
                    ast->label, 0, 0, 0, stable->running_offset, 0,1);
                    stable->running_offset += x;
            ast = ast->sibling;
        }
    }
    // If function
    if(root->label == MODULE_NODE){
        char* lexeme = root->firstChild->syntaxTreeNode->lexeme;
        symbolTableNode* temp = insert_into_moduleHashNode(lexeme, symbolForest);
        temp->parent = NULL;
        temp->running_offset = 0;
        traverse_ast_recurse(root->firstChild->sibling, temp, symbolForest); //inputplist
        traverse_ast_recurse(root->firstChild->sibling->sibling, temp, symbolForest); //ret
        traverse_ast_recurse(root->firstChild->sibling->sibling->sibling, temp, symbolForest); //stmts

        // Checking is the returned parameters are assigned or not.
        ASTnode* ret = root->firstChild->sibling->sibling->firstChild;
        while(ret!= NULL){
            symbolTableEntry* entr = isDeclared(stable->varHashTable, ret->firstChild->syntaxTreeNode->lexeme);
            if(entr != NULL && entr->isAssigned == 0){
                //Raise Error
                printf("Returned parameters not assigned\n");
            }
            ret = ret->sibling;
        }

        if(root->sibling)
            return traverse_ast_recurse(root->sibling,stable,symbolForest);
        else
            return;
        //check for assignment before Return
    }
    // If new scope
    if(root->label == WHILEITERATIVESTMT_NODE){       
        symbolTableNode* temp = (symbolTableNode*)malloc(sizeof(symbolTableNode));
        temp->parent = stable;
        temp->running_offset = 0;
        int i = 0;
        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;
        traverse_ast_recurse(root->firstChild, temp, symbolForest);//kya in teeno me firstchild hi statements hai?
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else
            return;
    }
    // If new scope
    if(root->label == CONDITIONALSTMT_NODE){

        //Free karne se error na aaye ye check karna hai

        //Checks 
        //1. ID should be declared in the previous scope
        //2. If ID is boolean, default should be NULL
        //3. If ID is integer, default should not be NULL
        //4. ID should not be real
        //5. Value type in individual case statement
        //6. **Case value in individual case statement??
        
        if(isDeclared(stable->varHashTable, root->firstChild->syntaxTreeNode->lexeme) == NULL){
            //Raise Error
            printf("ID not declared before SWITCH scope");
            return;
        }

        if(check_type(root->firstChild,stable) == FLOAT){
            //Raise Error
            printf("Switch value has a real variable\n");
            return;
        }

        symbolTableNode* temp = (symbolTableNode*)malloc(sizeof(symbolTableNode));
        temp->parent = stable;
        temp->running_offset = 0;
        int i = 0;
        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;

        if(check_type(root->firstChild,stable) == INT){

            if (root->firstChild->sibling->sibling == NULL){
                //Raise Error
                free(stable->childList[i]);
                printf("Default statement does not exist with a INT value.\n");
                return;
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            while(node != NULL){
                if (node->label != NUM_NODE){
                    //Raise Error
                    printf("Case Statement of Integer Switch has not integer value\n");
                    free(stable->childList[i]);
                    return;
                }
                traverse_ast_recurse(node->firstChild, temp, symbolForest);
                node = node->sibling;
            }

            //Recurse on dflt
            traverse_ast_recurse((root->firstChild->sibling->sibling, temp, symbolForest);
        }
        else if(check_type(root->firstChild,stable) == BOOL){

            if (root->firstChild->sibling->sibling != NULL){
                //Raise Error
                free(stable->childList[i]);
                printf("Default statement exists with a BOOLEAN value.\n");
                return;
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            if(node == NULL || node->sibling == NULL || node->sibling->sibling != NULL){
                free(stable->childList[i]);
                printf("Default statement exists with a BOOLEAN value.\n");
                return;
            }
            if((node->label == TRUE_NODE && node->sibling == FALSE_NODE) || (node->label == FALSE_NODE && node->sibling == TRUE_NODE)){
                traverse_ast_recurse(node->firstChild, temp, symbolForest);
                traverse_ast_recurse(node->sibling->firstChild, temp, symbolForest);
            }
            else{
                //Raise Error
                printf("Error in Case Statements of TRUE and FALSE\n");
            }

        }
        
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else
            return;
    }// If new scope

    if(root->label== FORITERATIVESTMT_NODE){       
        symbolTableNode* temp = (symbolTableNode*)malloc(sizeof(symbolTableNode));
        temp->parent = stable;
        temp->running_offset = 0;
        int i = 0;
        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;
        
        //check loop variable not changed inside
        char* loopVar = root->firstChild->syntaxTreeNode->lexeme;
        symbolTableEntry* loopVarEntry = getSymbolTableEntry(stable, loopVar);
        int tempIsAssigned = loopVarEntry->isAssigned;
        loopVarEntry->isAssigned = 0;
        traverse_ast_recurse(root->firstChild->sibling, temp, symbolForest);//kya in teeno me firstchild hi statements hai?
        if(loopVarEntry->isAssigned==1){
            printf("Loop variable %s is changed inside the loop", loopVar);
        }
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else
            return;
    }
    // If new variable
    if(root->label==DECLARESTMT_NODE){
        int isArray, type, startIndex, endIndex; 
        isArray=type=startIndex=endIndex=0;
        if(root->firstChild->label == DATATYPE_ARRAY_NODE){
            isArray = 1;
            type = root->firstChild->firstChild->sibling->label;
            startIndex = root->firstChild->firstChild->firstChild->syntaxTreeNode->value.num;
            endIndex = root->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num;
        }
        else{
            type = root->firstChild->label;
        }
        ASTnode* temp = root->firstChild->sibling;
        do{
            char* name = temp->syntaxTreeNode->lexeme;
            temp = temp->sibling;
            int x = insert_into_stable(stable->varHashTable, name, type, 
                    isArray, startIndex, endIndex, stable->running_offset,0,0);
            stable->running_offset += x;
        }while(temp != NULL);
        return traverse_ast_recurse(root->sibling,stable,symbolForest);
    }
    // check for assignment before declaration;
    if((root->label == ASSIGNOP_NODE && root->firstChild->label == ID) || (root->label ==  GET_STMT_NODE)){
        // pass root(AST), 
        symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
        if(entry == NULL){
            // Raise Error
            printf("ID not declared before ");
        }
        else{
            entry->isAssigned = 1;
        }
    }else if(root->label == MODULEREUSESTMT_NODE){

        //call function to check whether type of return formal parameters match actual parameters

        ASTnode* temp = root->firstChild->firstChild;
        
        while(temp!= NULL){
            symbolTableEntry* entry = getSymbolTableEntry(stable, temp->syntaxTreeNode->lexeme);

            if(entry == NULL){
                // Raise Error
                printf("ID not declared before");
            }
            else{
                entry->isAssigned = 1;
            }
            temp = temp->sibling;
        }
    }
    // expression type check
    if(root->label == AND_NODE || root->label == OR_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1!=BOOL || op2!=BOOL)
            printf("Error: bool mismatch");
        return traverse_ast_recurse(root->sibling, stable, symbolForest);
    }
    if(root->label == GE_NODE || root->label == LE_NODE || root->label == GT_NODE 
        || root->label == LT_NODE || root->label == EQ_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==BOOL || op2==BOOL)
            printf("Error: relop type mismatch");
        return travese_ast_recurse(root->sibling, stable, symbolForest);
    }
    if(root->label == MUL_NODE || root->label == DIV_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==BOOL || op2==BOOL)
            printf("Error: MUL/DIV type mismatch");
        return travese_ast_recurse(root->sibling, stable, symbolForest);     
    }

    if(root->label == PLUS_NODE || root->label == MINUS_NODE){
        int op1 = check_type(root->firstChild, stable);
        // unary
        if(root->firstChild == NULL){
            if(op1 == BOOL)
                printf("Error: unary PLUS/MINUS type mismatch");
            return travese_ast_recurse(root->sibling, stable, symbolForest);
        }
        // binary
        else{
            int op2 = check_type(root->firstChild->sibling, stable);
            if(op1==BOOL || op2==BOOL)
                printf("Error: binary PLUS/MINUS type mismatch");
            return travese_ast_recurse(root->sibling, stable, symbolForest);
        }
    }

    // continue if no match
    if(root->firstChild != NULL)
        traverse_ast_recurse(root->firstChild, stable, symbolForest);
    if(root->sibling != NULL)
        travese_ast_recurse(root->sibling, stable, symbolForest);

}

int check_type(ASTnode* root, symbolTableNode* stable){
    if(root->label == ID_NODE){
        symbolTableEntry* temp = getSymbolTableEntry(stable, root->syntaxTreeNode->lexeme);
        return temp->type;
    }
    
    if(root->label == RNUM_NODE)
        return FLOAT;

    if(root->label == NUM_NODE)
        return INT;
    
    if(root->label == AND_NODE || root->label == OR_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==BOOL && op2==BOOL)
            return BOOL;
        else
            return -1;
    }

    if(root->label == GE_NODE || root->label == LE_NODE || root->label == GT_NODE 
        || root->label == LT_NODE || root->label == EQ_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==BOOL || op2==BOOL)
            return -1;
        else
            return BOOL;
    }

    if(root->label == MUL_NODE || root->label == DIV_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==BOOL || op2==BOOL)
            return -1;
        if(op1==INT && op2==INT)
            return INT;
        if(op1==FLOAT || op2==FLOAT)
            return FLOAT;       
    }

    if(root->label == PLUS_NODE || root->label == MINUS_NODE){
        int op1 = check_type(root->firstChild, stable);
        // unary
        if(root->firstChild == NULL){
            if(op1 == BOOL)
                return -1;
            else
                return op1;
        }
        // binary
        else{
            int op2 = check_type(root->firstChild->sibling, stable);
            if(op1==BOOL || op2==BOOL)
                return -1;
            if(op1==FLOAT || op2==FLOAT)
                return FLOAT;      
            if(op1==INT && op2==INT)
                return INT;
        }
    }
}
