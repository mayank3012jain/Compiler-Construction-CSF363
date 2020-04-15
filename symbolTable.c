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

void traverse_ast(ASTnode* root, moduleHashNode* symbolForest[]){
        for(int i=0; i<MAX_MODULES; i++){
        symbolForest[i] = NULL;
    }
    
    symbolTableNode* stable = NULL;
    
    traverse_ast_recurse(root,stable,symbolForest);
    printf("\n\n***************starting second traversal************\n");
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

    if(root->label == MODULEDEF_HEADER_NODE){
        symbolTableNode* newsymNode = allocateSymbolTable(stable, 0, root->syntaxTreeNode->lineNumber, stable->key);
        int i=0;
        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = newsymNode;
        if(root->firstChild){
            traverse_ast_recurse(root->firstChild, newsymNode, symbolForest);
            stable->scopeEnd = newsymNode->scopeEnd;
            return;
        }
    }

    if(root->label == INPUTPLIST_HEADER_NODE){       
        ASTnode* ast = root->firstChild;

        //Dynamic array?
        while(ast != NULL){
            if(ast->firstChild->label == INTEGER_NODE || ast->firstChild->label == BOOLEAN_NODE || 
                ast->firstChild->label == REAL_NODE){
                int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->label, 0, 0, 0, NULL, NULL, stable->running_offset, 1,0, 0, ast->firstChild);
                stable->running_offset += x;//check once again
            }
            //array
            else{ 
                //dynamic
                if(ast->firstChild->firstChild->firstChild->label == ID_NODE){
                    symbolTableEntry* startIn = getSymbolTableEntry(stable, ast->firstChild->firstChild->firstChild->syntaxTreeNode->lexeme);
                    symbolTableEntry* endIn = getSymbolTableEntry(stable,ast->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->lexeme);
                    //check for NULL
                    if(startIn==NULL || endIn== NULL){
                        printf("Line numbe- %d Error- index variable not declared", ast->firstChild->firstChild->firstChild->syntaxTreeNode->lineNumber);
                    }
                    int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                        ast->firstChild->firstChild->sibling->label, 1, 0, 0,
                        startIn, endIn, stable->running_offset, 1,0, 0, ast->firstChild->sibling);
                }
                //static
                else{
                    int x = insert_into_stable(stable->varHashTable, ast->firstChild->sibling->syntaxTreeNode->lexeme, 
                            ast->firstChild->firstChild->sibling->label, 1, 
                            ast->firstChild->firstChild->firstChild->syntaxTreeNode->value.num, 
                            ast->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num,NULL, NULL, stable->running_offset, 1,0,1, ast->firstChild->sibling);
                    stable->running_offset += x;
                }
            }
            ast = ast->sibling;
        }
        if(root->sibling != NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);  
        }
        else{
            return;
        }
    }

    if(root->label == RET_HEADER_NODE){       
        ASTnode* ast = root->firstChild;
        while(ast!= NULL){
            int x = insert_into_stable(stable->varHashTable, ast->firstChild->syntaxTreeNode->lexeme, 
                    ast->label, 0, 0, 0, NULL, NULL, stable->running_offset, 0,1,0, ast->firstChild);
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
                    printf("Line %d: Error - Returned parameters not assigned\n",temp->scopeEnd);
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
        whileList* exprnCheckList = NULL;
        exprnCheckList = checkWhileExprn(root->firstChild, stable, exprnCheckList);
        
        symbolTableNode* temp = allocateSymbolTable(stable,0, root->firstChild->syntaxTreeNode->lineNumber+1, stable->key);
        int i = 0;

        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;

        //Check for a valid boolean expression
        // ASTnode* node = root->firstChild;

        traverse_ast_recurse(root->firstChild, temp, symbolForest);

        checkWhileIsAssigned(stable, exprnCheckList);

        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }else{
            stable->scopeEnd = temp->scopeEnd +1;
            return;
        }
    }
    // If new scope
    if(root->label == CONDITIONALSTMT_NODE){

        //Checks 
        //1. ID should be declared in the previous scope
        //2. If ID is boolean, default should be NULL
        //3. If ID is integer, default should not be NULL
        //4. ID should not be real
        //5. Value type in individual case statement
        //6. **Case value in individual case statement??


        symbolTableNode* temp = allocateSymbolTable(stable,0, root->firstChild->syntaxTreeNode->lineNumber +1, stable->key);
        int i = 0;

        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;

        if(getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme) == NULL){
            //Raise Error
            printf("Line %d: Error - ID [%s] not declared before SWITCH scope", root->firstChild->syntaxTreeNode->lineNumber,root->firstChild->syntaxTreeNode->lexeme);
            if(root->sibling!=NULL){
                return traverse_ast_recurse(root->sibling, stable, symbolForest);
            }else{
                stable->scopeEnd = stable->scopeEnd +1;
                return;
            }
        }

        if(check_type(root->firstChild,stable) == FLOAT){
            //Raise Error
            printf("Line %d: Error - Switch value has a real variable\n", root->firstChild->syntaxTreeNode->lineNumber);
            if(root->sibling!=NULL){
                return traverse_ast_recurse(root->sibling, stable, symbolForest);
            }else{
                stable->scopeEnd = stable->scopeEnd +1;
                return;
            }
        }

        if(check_type(root->firstChild,stable) == INT){

            if (root->firstChild->sibling->sibling == NULL){
                //Raise Error
                printf("Line %d: Error - Default statement does not exist with a INT value.\n", root->firstChild->syntaxTreeNode->lineNumber);
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            while(node != NULL){
                if (node->label != NUM_NODE){
                    //Raise Error
                    printf("Line %d: Error - Case Statement of Integer Switch has not integer value\n", node->syntaxTreeNode->lineNumber);
                    node = node->sibling;
                    continue;
                }
                traverse_ast_recurse(node->firstChild, temp, symbolForest);
                node = node->sibling;
            }

            //Recurse on dflt
            if(root->firstChild->sibling->sibling != NULL)
                traverse_ast_recurse(root->firstChild->sibling->sibling, temp, symbolForest);

        }
        else if(check_type(root->firstChild,stable) == BOOL){
            int trueFlag = 0, falseFlag = 0;

            if (root->firstChild->sibling->sibling != NULL){
                // Raise Error
                printf("Line %d: Error - Default statement exists with a BOOLEAN value.\n", root->firstChild->sibling->sibling->syntaxTreeNode->lineNumber);
            }

            ASTnode *node = root->firstChild->sibling->firstChild;

            while(node != NULL){

                if(node->label == TRUE_NODE && trueFlag == 0){
                    trueFlag = 1;
                    traverse_ast_recurse(node->firstChild, temp, symbolForest);
                }
                else if(node->label == FALSE_NODE && falseFlag == 0){
                    falseFlag = 1;
                    traverse_ast_recurse(node->firstChild, temp, symbolForest);
                }
                else if((node->label == TRUE_NODE && trueFlag == 1) || (node->label == FALSE_NODE && falseFlag == 1)){
                    printf("Line %d: - Error: Case already declared\n", node->syntaxTreeNode->lineNumber);
                }
                else{
                    printf("Line %d: - Error: Case Not Valid\n", node->syntaxTreeNode->lineNumber);
                }

                node = node->sibling;
            }

        }
        
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }else{
            stable->scopeEnd = temp->scopeEnd +1;
            return;
        }
    }// If new scope

    if(root->label== FORITERATIVESTMT_NODE){       
        symbolTableNode* temp = allocateSymbolTable(stable,0, root->firstChild->syntaxTreeNode->lineNumber +1, stable->key);
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
            printf("Line %d: Error - Loop variable [%s] is changed inside the loop\n", temp->scopeEnd,loopVar);
        }
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else{
            stable->scopeEnd = temp->scopeEnd +1;
            return;
        }
    }

    // If new variable
    if(root->label==DECLARESTMT_NODE){
        int isArray, type, startIndex, endIndex, isStatic; 
        isArray=type=startIndex=endIndex=0;
        isStatic =1;
        symbolTableEntry *stIn, *endIn;
        stIn = endIn = NULL;
        if(root->firstChild->label == DATATYPE_ARRAY_NODE){
            isArray = 1;
            type = root->firstChild->firstChild->sibling->label;
            startIndex = root->firstChild->firstChild->firstChild->syntaxTreeNode->value.num;
            endIndex = root->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num;
            if(root->firstChild->firstChild->firstChild->label==ID_NODE){
                stIn = getSymbolTableEntry(stable, root->firstChild->firstChild->firstChild->syntaxTreeNode->lexeme);
                endIn = getSymbolTableEntry(stable, root->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->lexeme);
                isStatic = 0;
            }
        }
        else{
            type = root->firstChild->label;
        }
        ASTnode* temp = root->firstChild->sibling;
        do{
            char* name = temp->syntaxTreeNode->lexeme;
            int x = insert_into_stable(stable->varHashTable, name, type, isArray, startIndex, endIndex, stIn, endIn,  stable->running_offset,0,0, isStatic, temp);
            temp = temp->sibling;
            stable->running_offset += x;

        }while(temp != NULL);
        
        if(root->sibling){
            return traverse_ast_recurse(root->sibling,stable,symbolForest);
        }else{
            stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
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
            
                // printf("\n***Line: %d %d %d\t", root->syntaxTreeNode->lineNumber, lhs, rhs);
                
                if(lhs != rhs && (lhs!=-1 && rhs!= -1)){
                    printf("Line %d: Error - Assignop type Mismatch \n", root->firstChild->syntaxTreeNode->lineNumber);
                }
                
                //Dynamic me aur changes
                if(lhs>=10 && rhs>=10){
                    symbolTableEntry* lhsEntry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
                    symbolTableEntry* rhsEntry = getSymbolTableEntry(stable, root->firstChild->sibling->syntaxTreeNode->lexeme);
                    if(lhsEntry->endIndex-lhsEntry->startIndex != rhsEntry->endIndex-rhsEntry->startIndex){
                        printf("Line %d: Error - Assignop type (array size) Mismatch \n", root->firstChild->syntaxTreeNode->lineNumber);
                    }
                }
            }
            entry->isAssigned = 1;
        }
        if(root->sibling != NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else{
            stable->scopeEnd = root->firstChild->syntaxTreeNode->lineNumber + 1;
            return;
        }
    }

    if(root->label == ASSIGNOP_ARRAY_NODE){

        symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->firstChild->syntaxTreeNode->lexeme);
        if(entry == NULL){
            // Raise Error
            printf("Line %d: Error - ID %s not declared before use\n", root->firstChild->firstChild->syntaxTreeNode->lineNumber, root->firstChild->firstChild->syntaxTreeNode->lexeme);
        }
        else{
            int lhs = check_type(root->firstChild, stable);
            int rhs = check_type(root->firstChild->sibling, stable);
            
            // printf("\n***Line: %d %d %d\t", root->syntaxTreeNode->lineNumber, lhs, rhs);
            if(lhs != rhs && (lhs!=-1 && rhs!= -1)){
                printf("Line %d: Error - Assignop type Mismatch \n", root->firstChild->firstChild->syntaxTreeNode->lineNumber);
            }

            entry->isAssigned = 1;
        }


        if(root->sibling != NULL){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }
        else{
            stable->scopeEnd = root->firstChild->firstChild->syntaxTreeNode->lineNumber + 1;
            return;
        }
    }


    else if(root->label == MODULEREUSESTMT_NODE){

        ASTnode* temp = root->firstChild->firstChild;
        
        if(strcmp(root->firstChild->sibling->syntaxTreeNode->lexeme,stable->key)==0){
            printf("Line %d Error - Recursive calls not allowed\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            if(root->sibling){
                return traverse_ast_recurse(root->sibling, stable, symbolForest);
            }else{
                stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
                return;
            }
        }

        while(temp!= NULL){
            symbolTableEntry* entry = getSymbolTableEntry(stable, temp->syntaxTreeNode->lexeme);

            if(entry == NULL){
                // Raise Error
                // printf("Line %d: Error - MODULE ID %s not declared before use\n", temp->syntaxTreeNode->lineNumber, temp->syntaxTreeNode->lexeme);
                if(root->sibling){
                    return traverse_ast_recurse(root->sibling, stable, symbolForest);
                }else{
                    stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
                    return;
                }
            }
            else{
                entry->isAssigned = 1;
            }
            temp = temp->sibling;
        }
        //call function to check whether type of return formal parameters match actual parameters
        moduleHashNode* moduleRoot = getModuleHashNode(root->firstChild->sibling->syntaxTreeNode->lexeme, symbolForest, root->firstChild->sibling->syntaxTreeNode->lineNumber);
        if(moduleRoot == NULL){
            if(root->sibling){
                return traverse_ast_recurse(root->sibling, stable, symbolForest);
            }
            else{
                stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
                return;
            }
        }
        else{
            moduleRoot->isUsed = 1;
        }
        if(root->sibling){
            return traverse_ast_recurse(root->sibling, stable, symbolForest);
        }else{
            stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
            return;
        }
        
    }

    if(root->label == PRINT_STMT_NODE){
        if(root->sibling==NULL){
            stable->scopeEnd = root->syntaxTreeNode->lineNumber+1;
        }
    }

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
        moduleHashNode* moduleRoot = getModuleHashNode(root->firstChild->sibling->syntaxTreeNode->lexeme, symbolForest, root->firstChild->sibling->syntaxTreeNode->lineNumber);

        if(strcmp(root->firstChild->sibling->syntaxTreeNode->lexeme,stable->key)==0){
            // printf("Line %d Error - Recursive calls not allowed\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            if(root->sibling){
                return traverse_ast_recurse2(root->sibling, stable, symbolForest, scope);
            }else{
                // stable->scopeEnd = root->firstChild->sibling->syntaxTreeNode->lineNumber +1;
                return;
            }
        }

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
        moduleHashNode* mhNode = getModuleHashNode(root->firstChild->syntaxTreeNode->lexeme, symbolForest, root->firstChild->syntaxTreeNode->lineNumber);
        stable = mhNode->tablePtr;
        traverse_ast_recurse2(root->firstChild->sibling->sibling->sibling, stable, symbolForest, 0);
        if(root->sibling){
            return traverse_ast_recurse2(root->sibling, NULL, symbolForest, 0);
        }
        else
            return;
    }

    if(root->label == DRIVER_MOD_NODE){
        moduleHashNode* mhNode = getModuleHashNode("driverFunctionNode", symbolForest, root->syntaxTreeNode->lineNumber-1);
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
           return traverse_ast_recurse2(root->sibling, stable, symbolForest, scope +1);
        }else
            return;
    }

    if(root->label == WHILEITERATIVESTMT_NODE){
        // stable = stable->childList[scope];
        traverse_ast_recurse2(root->firstChild->sibling, stable->childList[scope], symbolForest, 0);
        if(root->sibling){
           return traverse_ast_recurse2(root->sibling, stable, symbolForest, scope +1);
        }else
            return;
    }

    if(root->label == CONDITIONALSTMT_NODE){
        // stable = stable->childList[scope];
        if((getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme) != NULL) && (check_type(root->firstChild,stable) != FLOAT)){
            traverse_ast_recurse2(root->firstChild->sibling, stable->childList[scope], symbolForest, 0);
        }

        if(root->sibling){
            return traverse_ast_recurse2(root->sibling, stable, symbolForest, scope +1);
        }else
            return;
        
    }

    if(root->label == MODULEDEF_HEADER_NODE){
        return traverse_ast_recurse2(root->firstChild, stable->childList[scope], symbolForest, 0);
    }

    if(root->firstChild != NULL)
        traverse_ast_recurse2(root->firstChild, stable, symbolForest, scope);
    if(root->sibling != NULL)
        traverse_ast_recurse2(root->sibling, stable, symbolForest, scope);

    return;
}