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

void traverse_ast(ASTnode* root){
    moduleHashNode* symbolForest[MAX_MODULES];
    for(int i=0; i<MAX_MODULES; i++){
        symbolForest[i] = NULL;
    }
    
    symbolTableNode* stable = NULL;
    
    traverse_ast_recurse(root,stable,symbolForest);
    // printf("\n\n***************starting second traversal************\n");
    traverse_ast_recurse2(root,stable,symbolForest,0);
    
    printSymbolForest(symbolForest);
}

void traverse_ast_recurse(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[]){ 

    if(root == NULL){
        //Raise Error
        printf("\nCALLED ON NULL ROOT\n");
        return;
    }
    // printASTNode(root);

    if(root->label == INPUTPLIST_HEADER_NODE){       
        ASTnode* ast = root->firstChild;
        //Dynamic array?
        while(ast != NULL){
            if(ast->firstChild->label == INTEGER_NODE || ast->firstChild->label == BOOLEAN_NODE || 
                ast->firstChild->label == REAL_NODE){
                int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->label, 0, 0, 0, stable->running_offset, 1,0, ast->firstChild);
                stable->running_offset += x;//check once again
            }
            else{ 
                int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->firstChild->sibling->label, 1, 
                        ast->firstChild->firstChild->firstChild->syntaxTreeNode->value.num, 
                        ast->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num, stable->running_offset, 1,0, ast->firstChild->sibling);
                stable->running_offset += x;
            }
            ast = ast->sibling;
        }
        if(root->sibling != NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);  
        }
    }

    if(root->label == RET_HEADER_NODE){       
        ASTnode* ast = root->firstChild;
        while(ast!= NULL){
            int x = insert_into_stable(stable->varHashTable, ast->firstChild->syntaxTreeNode->lexeme, 
                    ast->label, 0, 0, 0, stable->running_offset, 0,1, ast->firstChild);
                    stable->running_offset += x;
            ast = ast->sibling;
        }
        if (root->sibling != NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        return;
    }
    // If function
    if(root->label == MODULE_NODE){
        
        char* lexeme = root->firstChild->syntaxTreeNode->lexeme;
        symbolTableNode* temp = insert_into_moduleHashNode(lexeme, symbolForest, root);

        //temp is NULL if 
        //1. Declared and defined before use
        //2. Module already defined
        if (temp != NULL){
            temp->parent = NULL;
            temp->running_offset = 0;
            traverse_ast_recurse(root->firstChild->sibling, temp, symbolForest); //inputplist
            // traverse_ast_recurse(root->firstChild->sibling->sibling, temp, symbolForest); //ret
            // traverse_ast_recurse(root->firstChild->sibling->sibling->sibling, temp, symbolForest); //stmts

            // Checking is the returned parameters are assigned or not.
            ASTnode* ret = root->firstChild->sibling->sibling->firstChild;
            while(ret!= NULL){
                symbolTableEntry* entr = isDeclared(temp->varHashTable, ret->firstChild->syntaxTreeNode->lexeme);
                if(entr != NULL && entr->isAssigned == 0){
                    //Raise Error
                    printf("Line %d: Error - Returned parameters not assigned\n",ret->firstChild->syntaxTreeNode->lineNumber);
                }
                ret = ret->sibling;
            }
        }
        
        if(root->sibling){
            return traverse_ast_recurse(root->sibling,stable,symbolForest);
        }else{
            return;
        }
        //check for assignment before Return
    }

    if(root->label == DRIVER_MOD_NODE){
        stable = insert_into_moduleHashNode("driverFunctionNode", symbolForest, root);

    }

    if(root->label == MODULEDEC_HEADER_NODE){
        ASTnode* mdec = root->firstChild;
        while(mdec != NULL){

            check_module_dec(mdec->syntaxTreeNode->lexeme,symbolForest, mdec);
            mdec = mdec->sibling;
        }
        if(root->sibling!= NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }else{
            return;
        }
    }

    // If new scope
    if(root->label == WHILEITERATIVESTMT_NODE){       
        symbolTableNode* temp = allocateSymbolTable(stable,0);
        int i = 0;

        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;

        //Check for a valid boolean expression
        // ASTnode* node = root->firstChild;

        traverse_ast_recurse(root->firstChild, temp, symbolForest);//kya in teeno me firstchild hi statements hai?
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
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
        
        if(getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme) == NULL){
            //Raise Error
            printf("Line %d: Error - ID [%s] not declared before SWITCH scope", root->firstChild->syntaxTreeNode->lineNumber,root->firstChild->syntaxTreeNode->lexeme);
            return;
        }

        if(check_type(root->firstChild,stable) == FLOAT){
            //Raise Error
            printf("Line %d: Error - Switch value has a real variable\n", root->firstChild->syntaxTreeNode->lineNumber);
            return;
        }

        symbolTableNode* temp = allocateSymbolTable(stable,0);
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
                printf("Line %d: Error - Default statement does not exist with a INT value.\n", root->firstChild->syntaxTreeNode->lineNumber);
                return;
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            while(node != NULL){
                if (node->label != NUM_NODE){
                    //Raise Error
                    printf("Line %d: Error - Case Statement of Integer Switch has not integer value\n", node->syntaxTreeNode->lineNumber);
                    free(stable->childList[i]);
                    return;
                }
                traverse_ast_recurse(node->firstChild, temp, symbolForest);
                node = node->sibling;
            }

            //Recurse on dflt
            return traverse_ast_recurse(root->firstChild->sibling->sibling, temp, symbolForest);
        }
        else if(check_type(root->firstChild,stable) == BOOL){

            if (root->firstChild->sibling->sibling != NULL){
                //Raise Error
                free(stable->childList[i]);
                printf("Line %d: Error - Default statement exists with a BOOLEAN value.\n", root->firstChild->sibling->sibling->syntaxTreeNode->lineNumber);
                return;
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            if(node == NULL || node->sibling == NULL || node->sibling->sibling != NULL){
                free(stable->childList[i]);
                printf("Line %d: Error - Switch with BOOLEAN value should have exactly 2 cases.\n", root->firstChild->syntaxTreeNode->lineNumber);
                return;
            }
            if((node->label == TRUE_NODE && node->sibling->label == FALSE_NODE) || (node->label == FALSE_NODE && node->sibling->label == TRUE_NODE)){
                traverse_ast_recurse(node->firstChild, temp, symbolForest);
                traverse_ast_recurse(node->sibling->firstChild, temp, symbolForest);
            }
            else{
                //Raise Error
                printf("Line %d: Error - Error in Case Statements of TRUE and FALSE\n", root->firstChild->syntaxTreeNode->lineNumber);
            }

        }
        
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
    }// If new scope

    if(root->label== FORITERATIVESTMT_NODE){       
        symbolTableNode* temp = allocateSymbolTable(stable,0);
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
        traverse_ast_recurse(root->firstChild->sibling, temp, symbolForest);//kya in teeno me firstchild hi statements hai?//sibling added
        if(loopVarEntry->isAssigned==1){
            printf("Line %d: Error - Loop variable [%s] is changed inside the loop", root->firstChild->syntaxTreeNode->lineNumber,loopVar);
        }
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else{
            return;//added
        }
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
            int x = insert_into_stable(stable->varHashTable, name, type, isArray, startIndex, endIndex, stable->running_offset,0,0, temp);
            temp = temp->sibling;
            stable->running_offset += x;
        }while(temp != NULL);
        if(root->sibling){
            return traverse_ast_recurse(root->sibling,stable,symbolForest);
        }else{
            return;
        }
    }
    // check for assignment before declaration;
    if((root->label == ASSIGNOP_NODE && root->firstChild->label == ID_NODE) || (root->label ==  GET_STMT_NODE)){
        // pass root(AST), 
        symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
        if(entry == NULL){
            // Raise Error
            printf("Line %d: Error - ID %s not declared before use\n", root->firstChild->syntaxTreeNode->lineNumber, root->firstChild->syntaxTreeNode->lexeme);
        }
        else{
            // printf("\n[%s]\n", root->firstChild->syntaxTreeNode->lexeme);
            if(root->label==ASSIGNOP_NODE){
                int lhs = check_type(root->firstChild, stable);
                int rhs = check_type(root->firstChild->sibling, stable);
            
                if(lhs != rhs && (lhs!=-1 && rhs!= -1)){
                    printf("Line %d: Error - Assignop type Mismatch \n", root->firstChild->syntaxTreeNode->lineNumber);
                }
            }
            entry->isAssigned = 1;
        }
        
    }
    else if(root->label == MODULEREUSESTMT_NODE){

        
        ASTnode* temp = root->firstChild->firstChild;
        
        while(temp!= NULL){
            symbolTableEntry* entry = getSymbolTableEntry(stable, temp->syntaxTreeNode->lexeme);

            if(entry == NULL){
                // Raise Error
                // printf("Line %d: Error - MODULE ID %s not declared before use\n", temp->syntaxTreeNode->lineNumber, temp->syntaxTreeNode->lexeme);
                if(root->sibling){
                    return traverse_ast_recurse(root->sibling, stable, symbolForest);
                }else{
                    return;
                }
            }
            else{
                entry->isAssigned = 1;
            }
            temp = temp->sibling;
        }
        //call function to check whether type of return formal parameters match actual parameters
        moduleHashNode* moduleRoot = getModuleHashNode(root->firstChild->sibling->syntaxTreeNode->lexeme, symbolForest);
        if(moduleRoot == NULL){
            if(root->sibling){
                return traverse_ast_recurse(root->sibling, stable, symbolForest);
            }
            else
                return;
        }
        else{
            moduleRoot->isUsed = 1;
        }
        if(root->sibling){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }else{
            return;
        }
        
    }
    // expression type check
    // if(root->label == AND_NODE || root->label == OR_NODE){
    //     int op1 = check_type(root->firstChild, stable);
    //     int op2 = check_type(root->firstChild->sibling, stable);
    //     if(op1==-1 || op2==-1){
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     }            
    //     if(op1!=BOOL || op2!=BOOL)
    //         printf("Line %d: Error - bool mismatch\n", root->syntaxTreeNode->lineNumber);
    //     if (root->sibling != NULL)
    //         return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //     else
    //         return;
    // }
    // if(root->label == GE_NODE || root->label == LE_NODE || root->label == GT_NODE 
    //     || root->label == LT_NODE || root->label == EQ_NODE){
    //     int op1 = check_type(root->firstChild, stable);
    //     int op2 = check_type(root->firstChild->sibling, stable);
    //     if(op1==-1 || op2==-1){
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     } 
    //     if(op1==BOOL || op2==BOOL)
    //         printf("Line %d: Error - relop type mismatch\n", root->syntaxTreeNode->lineNumber);
    //     if (root->sibling != NULL)    
    //         return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //     else
    //         return;
    // }
    // if(root->label == MUL_NODE || root->label == DIV_NODE){
    //     int op1 = check_type(root->firstChild, stable);
    //     int op2 = check_type(root->firstChild->sibling, stable);
    //     if(op1==-1 || op2==-1){
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     } 
    //     if(op1==BOOL || op2==BOOL)
    //         printf("Line %d: Error - MUL/DIV type mismatch\n", root->syntaxTreeNode->lineNumber);
    //     if (root->sibling != NULL)
    //         return traverse_ast_recurse(root->sibling, stable, symbolForest);     
    //     else
    //         return;
    // }

    // if(root->label == PLUS_NODE || root->label == MINUS_NODE){
    //     int op1 = check_type(root->firstChild, stable);
    //     if(op1==-1){
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     } 
    //     // unary
    //     if(root->firstChild->sibling == NULL){
    //         if(op1 == BOOL)
    //             printf("Line %d: Error - unary PLUS/MINUS type mismatch\n", root->syntaxTreeNode->lineNumber );
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     }
    //     // binary
    //     else{
    //         int op2 = check_type(root->firstChild->sibling, stable);
    //         if(op2==-1){
    //             if (root->sibling != NULL)
    //                 return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //             else
    //                 return;
    //         } 
    //         if(op1==BOOL || op2==BOOL)
    //             printf("Line %d: Error - binary PLUS/MINUS type mismatch\n", root->syntaxTreeNode->lineNumber);
    //         if (root->sibling != NULL)
    //             return traverse_ast_recurse(root->sibling, stable, symbolForest);
    //         else
    //             return;
    //     }
        
    // }

    // continue if no match
    if(root->firstChild != NULL)
        traverse_ast_recurse(root->firstChild, stable, symbolForest);
    if(root->sibling != NULL)
        traverse_ast_recurse(root->sibling, stable, symbolForest);
}

void traverse_ast_recurse2(ASTnode* root, symbolTableNode* stable, moduleHashNode* symbolForest[], int scope){

    if(!root){
        printf("\ntraverse 2 called on null");
        return;
    }
    // printASTNode(root);

    if(root->label == MODULEREUSESTMT_NODE){

        //call function to check whether type of return formal parameters match actual parameters
        moduleHashNode* moduleRoot = getModuleHashNode(root->firstChild->sibling->syntaxTreeNode->lexeme, symbolForest);

        if(moduleRoot != NULL){
            checkFunctionReturnType(moduleRoot->moduleAst, root, stable, symbolForest);
            checkFunctionParameterType(moduleRoot->moduleAst, root, stable, symbolForest);
        }
        if (root->sibling != NULL)
            return traverse_ast_recurse2(root->sibling, stable, symbolForest, scope);
        else
            return;
    }

    if(root->label == MODULE_NODE){
        moduleHashNode* mhNode = getModuleHashNode(root->firstChild->syntaxTreeNode->lexeme, symbolForest);
        stable = mhNode->tablePtr;
        traverse_ast_recurse2(root->firstChild->sibling->sibling->sibling, stable, symbolForest, 0);
        if(root->sibling){
            return traverse_ast_recurse2(root->sibling, NULL, symbolForest, 0);
        }
        else
            return;
    }

    if(root->label == DRIVER_MOD_NODE){
        moduleHashNode* mhNode = getModuleHashNode("driverFunctionNode", symbolForest);
        stable = mhNode->tablePtr;
        traverse_ast_recurse2(root->firstChild, stable, symbolForest, 0);
        if(root->sibling != NULL)
            return traverse_ast_recurse2(root->sibling, NULL, symbolForest, 0);
        else
            return;
    }

    if(root->label == FORITERATIVESTMT_NODE){
        // stable = stable->childList[scope];
        traverse_ast_recurse2(root->firstChild->sibling->sibling, stable->childList[scope], symbolForest, 0);
        if(root->sibling){
           return traverse_ast_recurse2(root->sibling, stable->childList[scope], symbolForest, scope +1);
        }else
            return;
    }

    if(root->label == WHILEITERATIVESTMT_NODE){
        // stable = stable->childList[scope];
        traverse_ast_recurse2(root->firstChild->sibling, stable->childList[scope], symbolForest, 0);
        if(root->sibling){
           return traverse_ast_recurse2(root->sibling, stable->childList[scope], symbolForest, scope +1);
        }else
            return;
    }

    if(root->label == CONDITIONALSTMT_NODE){
        // stable = stable->childList[scope];
        traverse_ast_recurse2(root->firstChild->sibling, stable->childList[scope], symbolForest, 0);
        if(root->sibling){
           return traverse_ast_recurse2(root->sibling, stable->childList[scope], symbolForest, scope +1);
        }else
            return;
    }

    if(root->firstChild != NULL)
        traverse_ast_recurse2(root->firstChild, stable, symbolForest, scope);
    if(root->sibling != NULL)
        traverse_ast_recurse2(root->sibling, stable, symbolForest, scope);

    return;
}