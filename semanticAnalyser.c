#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include "astDef.h"

int checkKeyword(char *name, ASTnode* ast){
    
    int ind = hash(name);
    hashNode* temp = symbolTable[ind];
    while(temp!=NULL){
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Line %d Error - ID same as Keyword\n", ast->syntaxTreeNode->lineNumber);
        }
        temp = temp->next;
    }
    return -1; //Change  if req    
}

symbolTableNode* allocateSymbolTable(symbolTableNode* parent, int offset, int scopeStart, char key[]){

    symbolTableNode* stable = (symbolTableNode*)malloc(sizeof(symbolTableNode));

    int i = 0;
    while(i<VAR_SYMBOLTABLE_SIZE){
        stable->varHashTable[i] = NULL;
        i++;
    }
    i=0;
    while(i<MAX_SCOPES){
        stable->childList[i] = NULL;
        i++;
    }
    stable->parent = parent;
    stable->running_offset = offset;
    stable->scopeStart=scopeStart;
    strcpy(stable->key, key);
    if(parent){
        stable->nest= parent->nest +1;
    }else{
        stable->nest= 0;
    }

    return stable;
}

int insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, symbolTableEntry* startIndexDyn, symbolTableEntry* endIndexDyn, int offset, int isAssigned, int isReturn, int isStatic,ASTnode* ast){

    //check in keywords
    if(checkKeyword(name, ast)!=-1){
        return -1;
    }
    int ind = hashGivenIndex(name, 0, VAR_SYMBOLTABLE_SIZE-1);
    varHashNode* temp = varHashTable[ind];

    if(temp!=NULL){
        while(temp->next != NULL){
            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("Line: %d: Error - ID declared again [%s]\n", ast->syntaxTreeNode->lineNumber,name);
                return 0;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Line: %d: Error - ID declared again [%s]\n", ast->syntaxTreeNode->lineNumber,name);
            return 0;
        }
        temp->next = (varHashNode*)malloc(sizeof(varHashNode));
        temp = temp->next;
        // printf("temp wasnt null for %s", keywords[i]);
    }else{
        varHashTable[ind] = (varHashNode*)malloc(sizeof(varHashNode));
        temp = varHashTable[ind];
        // printf("temp was NULL for %s", keywords[i]);
    }

    temp->next = NULL;
    strcpy(temp->key, name);
    temp->entryPtr = (symbolTableEntry*)malloc(sizeof(symbolTableEntry));
    strcpy(temp->entryPtr->name,name);
    temp->entryPtr->type = type;
    temp->entryPtr->isArray = isArray;
    temp->entryPtr->startIndex = startIndex;
    temp->entryPtr->endIndex = endIndex;
    temp->entryPtr->startIndexDyn = startIndexDyn;
    temp->entryPtr->endIndexDyn = endIndexDyn;
    temp->entryPtr->offset = offset;
    temp->entryPtr->isAssigned = isAssigned;
    temp->entryPtr->isReturn = isReturn;
    temp->entryPtr->isStatic = isStatic;

    // calculate size of entry
    int width = 0;
    if(isArray){
        int size = DATA_TYPE_SIZES[type];
        width = ((endIndex - startIndex + 1) * size) +1;
        if(isReturn==2){
            width=5;
        }else if(isStatic==0){
            width=1;
        }
    }
    else{
        width = DATA_TYPE_SIZES[type];
    }
   
    temp->entryPtr->width = width;
    return width;
} 

symbolTableNode* insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[], ASTnode* moduleRoot){

    if(checkKeyword(name, moduleRoot)!=-1){
        return NULL;
    }
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    int flag = 0;
    moduleHashNode* temp = symbolForest[ind];

    if(temp!=NULL){

        while(temp->next != NULL){

            if(strcmp(temp->key, name)==0){
                
                if(temp->isDefined == 1){
                    //Raise Error
                    printf("Line %d: Error - Module [%s] defined again.\n", moduleRoot->firstChild->syntaxTreeNode->lineNumber, name);
                    return NULL;
                }
                else if(temp->isUsed == 0){
                    //Raise Error
                    
                    printf("Line %d: Error - Module [%s] declared but not used before Definition\n", moduleRoot->firstChild->syntaxTreeNode->lineNumber, name);
                    // return NULL;
                    flag =1;
                    break;
                }
                else{
                    temp->isDefined = 1;
                    flag=1;
                    break;
                }
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0  && flag == 0){

            if(temp->isDefined == 1){
                //Raise Error
                printf("Line %d: Error - Module [%s] defined again.\n", moduleRoot->firstChild->syntaxTreeNode->lineNumber, name);
                return NULL;
            }
            else if(temp->isUsed == 0){
                //Raise Error
                printf("Line %d: Error - Module [%s] declared but not used before Definition\n", moduleRoot->firstChild->syntaxTreeNode->lineNumber, name);
                flag = 1;
                // return NULL;
            }
            else{
                temp->isDefined = 1;
                flag=1;
                // return temp->tablePtr;
            }
        }
        if(flag == 0){
            temp->next = (moduleHashNode*)malloc(sizeof(moduleHashNode));
            temp = temp->next;
        }
        // printf("temp wasnt null for %s", keywords[i]);
    }
    else{
        symbolForest[ind] = (moduleHashNode*)malloc(sizeof(moduleHashNode));
        temp = symbolForest[ind];
        // printf("temp was NULL for %s", keywords[i]);
    }

    strcpy(temp->key, name);
    if(flag==0){
        temp->next = NULL;
    }
    temp->isUsed = 0;
    temp->isDefined = 1;
    if(strcmp("driverFunctionNode", name)==0){
        temp->tablePtr = allocateSymbolTable(NULL,0,moduleRoot->syntaxTreeNode->lineNumber, name);
    }
    else
        temp->tablePtr = allocateSymbolTable(NULL,0,moduleRoot->firstChild->sibling->sibling->sibling->syntaxTreeNode->lineNumber, name);
    
    temp->moduleAst= moduleRoot;

    return temp->tablePtr;
}

//Returns error if module already declared
//Creates a new node otherwise
void check_module_dec(char* name, moduleHashNode *symbolForest[], ASTnode* ast){
    
    if(checkKeyword(name, ast)!=-1){
        return;
    }

    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];

    if(temp!=NULL){

        while(temp->next != NULL){

            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("Line %d: Error - Module [%s] Declaration Repeated\n", ast->syntaxTreeNode->lineNumber, name);
                return;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Line %d: Error - Module [%s] Declaration Repeated\n", ast->syntaxTreeNode->lineNumber, name);
            return;
        }

        temp->next = (moduleHashNode*)malloc(sizeof(moduleHashNode));
        temp = temp->next;

    }
    else{
        symbolForest[ind] = (moduleHashNode*)malloc(sizeof(moduleHashNode));
        temp = symbolForest[ind];
    }

    strcpy(temp->key, name);
    temp->isUsed = 0;
    temp->isDefined = 0;
    temp->tablePtr = NULL;
    temp->next = NULL;
    temp->moduleAst = NULL;
}

symbolTableEntry* isDeclared(varHashNode* varHashTable[], char* name){

    int ind = hashGivenIndex(name, 0, VAR_SYMBOLTABLE_SIZE-1);
    varHashNode* temp = varHashTable[ind];

    while(temp!=NULL){
        if(strcmp(temp->key, name)==0){
            //FOUND
            return temp->entryPtr;
        }
        temp = temp->next;
    }

    return NULL;
}

//get the symbol table entry corresponding to an ID, return NULL if not present
symbolTableEntry* getSymbolTableEntry(symbolTableNode* stNode, char* name){
    //check in all hashtables
    symbolTableNode* tempST = stNode;
    while(tempST != NULL){
        symbolTableEntry* ans = isDeclared(tempST->varHashTable, name);
        if(ans == NULL){
            tempST = tempST->parent;
        }
        else{
            return ans;
        }
    }
    return NULL;
}

int check_type(ASTnode* root, symbolTableNode* stable){
    if(root->label == VARIDNUM_NODE){
        symbolTableEntry* temp = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
        // If undeclared
        // both varidnum and its child id have lexeme
        if(temp == NULL){
            printf("Line %d: Error - Undeclared variable: [%s]\n", root->firstChild->syntaxTreeNode->lineNumber, root->firstChild->syntaxTreeNode->lexeme);
            return -1;
        }
        // If array
        if(root->firstChild->sibling){
            // If array indexed by variable
            if(root->firstChild->sibling->label==ID_NODE){    
                symbolTableEntry* index = getSymbolTableEntry(stable, root->firstChild->sibling->syntaxTreeNode->lexeme);
                // If indexing variable doesn't exist in scope
                if(index == NULL){
                    printf("Line %d: Error - Indexing variable [%s] doesn't exist in scope\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->lexeme);
                    return -1; //print error
                }
                // If indexing variable exists and is not int
                if(index->type != INT){
                    printf("Line %d: Error - indexing variable [%s] exists and is not int\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->lexeme);
                    return -1; //print error
                }
                // Bound check

                // if(index->isAssigned<temp->startIndex || index->isAssigned>temp->endIndex){
                //     printf("Line %d: Error - Index Not In Range\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
                //     return -1; //print error
                // }
                // If everything ok
                return temp->type;
            }
            // If array indexed by int constant
            else if(root->firstChild->sibling->label==NUM_NODE){
                int num = root->firstChild->sibling->syntaxTreeNode->value.num;
                // Bound check error
                if(num<temp->startIndex || num>temp->endIndex){
                    printf("Line %d: Error - NUM %d Not In Range\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->value.num);
                    return -1; // print error
                }
                return temp->type;
            }
            // If array indexed by real or float constant
            else{
                printf("Line %d: Error - Indexed passed is not an Integer\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
                return -1; // print error
            }
        }
        // If not array
        else{
            if(temp->isArray == 1){
                return temp->type+10;
            }
            return temp->type;
        }
    }
    
    // Probably wont be executed
    if(root->label == ID_NODE){
        symbolTableEntry* temp = getSymbolTableEntry(stable, root->syntaxTreeNode->lexeme);
        if(temp == NULL){
            printf("Line %d: Error - Undeclared variable: [%s]\n", root->syntaxTreeNode->lineNumber,root->syntaxTreeNode->lexeme);
            return -1;
        }
        if(temp->isArray == 1){
            return temp->type+10;
        }
        return temp->type;
    }

    //for ASSIGNOP_ARRAY_NODE
    if(root->label == ID_ARRAY_NODE){

        symbolTableEntry* temp = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
        
        if(temp == NULL){
            printf("Line %d: Error - Undeclared variable: [%s]\n", root->firstChild->syntaxTreeNode->lineNumber, root->firstChild->syntaxTreeNode->lexeme);
            return -1;
        }
        
        if(root->firstChild->sibling->label==ID_NODE){    
            symbolTableEntry* index = getSymbolTableEntry(stable, root->firstChild->sibling->syntaxTreeNode->lexeme);
            // If indexing variable doesn't exist in scope
            if(index == NULL){
                printf("Line %d: Error - Indexing variable [%s] doesn't exist in scope\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->lexeme);
                return -1; //print error
            }
            // If indexing variable exists and is not int
            if(index->type != INT){
                printf("Line %d: Error - indexing variable [%s] exists and is not int\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->lexeme);
                return -1; //print error
            }
            // Bound check
            // if(index->isAssigned<temp->startIndex || index->isAssigned>temp->endIndex){
            //     printf("Line %d: Error - Index Not In Range\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            //     return -1; //print error
            // }
            // If everything ok
            return temp->type;
        }
        // If array indexed by int constant
        else if(root->firstChild->sibling->label==NUM_NODE){
            int num = root->firstChild->sibling->syntaxTreeNode->value.num;
            // Bound check error
            if(num<temp->startIndex || num>temp->endIndex){
                printf("Line %d: Error - NUM %d Not In Range\n", root->firstChild->sibling->syntaxTreeNode->lineNumber, root->firstChild->sibling->syntaxTreeNode->value.num);
                return -1; // print error
            }
            return temp->type;
        }
        else{
            printf("Line %d: Error - Indexed passed is not an Integer or variable\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1; // print error
        }
    }
    
    if(root->label == RNUM_NODE)
        return FLOAT;

    if(root->label == NUM_NODE)
        return INT;
    
    if(root->label == TRUE_NODE || root->label == FALSE_NODE)
        return BOOL;

    if(root->label == AND_NODE || root->label == OR_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==-1 || op2==-1)
            return -1;
        if(op1>=10 || op2>=10){
            printf("Line %d: Error - Operation on array variables\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        if(op1==BOOL && op2==BOOL)
            return BOOL;
        else{
            printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
    }

    if(root->label == GE_NODE || root->label == LE_NODE || root->label == GT_NODE 
        || root->label == LT_NODE || root->label == EQ_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==-1 || op2==-1)
            return -1;
        if(op1>=10 || op2>=10){
            printf("Line %d: Error - Operation on array variables\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        if(op1==BOOL || op2==BOOL){
            printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        if(op1 != op2){
            printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        else
            return BOOL;
    }

    if(root->label == MUL_NODE || root->label == DIV_NODE){
        int op1 = check_type(root->firstChild, stable);
        int op2 = check_type(root->firstChild->sibling, stable);
        if(op1==-1 || op2==-1)
            return -1;
        if(op1==BOOL || op2==BOOL){
            printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        if(op1>=10 || op2>=10){
            printf("Line %d: Error - Operation on array variables\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
        if(op1==INT && op2==INT)
            return INT;
        if(op1==FLOAT && op2==FLOAT)
            return FLOAT;

        printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
        return -1;       
    }

    if(root->label == PLUS_NODE || root->label == MINUS_NODE){
        int op1 = check_type(root->firstChild, stable);
        // unary
        if(root->firstChild->sibling == NULL){
            if(op1==-1)
                return -1;
            if(op1 == BOOL)
                return -1;
            else
                return op1;
        }
        // binary
        else{
            int op2 = check_type(root->firstChild->sibling, stable);
            if(op1==-1 || op2==-1)
                return -1;
            if(op1==BOOL || op2==BOOL){
                printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
                return -1;
            }
            if(op1>=10 || op2>=10){
                printf("Line %d: Error - Operation on array variables\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
                return -1;
            }
            if(op1==FLOAT && op2==FLOAT)
                return FLOAT;      
            if(op1==INT && op2==INT)
                return INT;

            printf("Line %d: Error - Type Mismatch\n", root->firstChild->sibling->syntaxTreeNode->lineNumber);
        }
    }
    return -1;
}

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

//Printing the symbol table

void printSymbolForest(moduleHashNode* symbolForest[]){

    for(int i=0; i<MAX_MODULES; i++){
        if(symbolForest[i] != NULL){
            printModuleHashNode(symbolForest[i]);
        }
    }    
}

void printModuleHashNode(moduleHashNode* modhash){

    if(modhash == NULL){
        return;
    }
    moduleHashNode* temp = modhash;

    while(temp != NULL){
        printf("Module Name: [%s], isUsed: [%d], isDefined: [%d] astNode: [%s]\n",temp->key, temp->isUsed, temp->isDefined, nodeNameString[temp->moduleAst->label]);
        printSymbolTableNode(temp->tablePtr, temp);
        temp = temp->next;
    }
    return;
}

void printSymbolTableNode(symbolTableNode* symNode, moduleHashNode* modhash){
    if(symNode == NULL){
        return;
    }

    printf("\n**new stable started in %s**", modhash->key);
    printVarHashTable(symNode->varHashTable, symNode, modhash);
    int i = 0;

    while(i<MAX_SCOPES){
        if(symNode->childList[i] != NULL){
            printSymbolTableNode(symNode->childList[i], modhash);
        }
        i++;
    }

    return;
}

void printVarHashTable(varHashNode* varht[], symbolTableNode* symNode, moduleHashNode* modhash){

    if(varht == NULL){
        return;
    }
    
    for(int i=0; i<VAR_SYMBOLTABLE_SIZE; i++){
        if(varht[i] != NULL){
            printVarHashNode(varht[i], symNode, modhash);
        }
    }

    return;
}

void printVarHashNode(varHashNode* varhn, symbolTableNode* symNode, moduleHashNode* modhash){

    if (varhn == NULL)
        return;

    printSymbolTableEntry(varhn->entryPtr, symNode, modhash);
    varHashNode *temp = varhn;
    while(temp->next != NULL){
        temp = temp->next;
        printVarHashNode(temp, symNode, modhash);
    }
    return; 
}

void printSymbolTableEntry(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash){

    if(symEntry == NULL){
        return;
    }
    if(symEntry->isArray == 1){
        if(symEntry->isStatic == 1){
            printf("\tVar Name: [%s], Scope: [%s, %d-%d], Width: [%d], isArray: YES, static array [%d-%d], Type: [%s], offset: [%d], nesting level: [%d], isAssigned: [%d], isReturn: [%d]\n", symEntry->name, modhash->key, symNode->scopeStart, symNode->scopeEnd, symEntry->width, symEntry->startIndex, symEntry->endIndex, nodeNameString[symEntry->type], symEntry->offset, symNode->nest, symEntry->isAssigned, symEntry->isReturn);
        }
        else{
            printf("\tVar Name: [%s], Scope: [%s, %d-%d], Width: [%d], isArray: YES, dynamic array [%s-%s], Type: [%s], offset: [%d], nesting level: [%d], isAssigned: [%d], isReturn: [%d]\n", symEntry->name,  modhash->key, symNode->scopeStart, symNode->scopeEnd, 1,symEntry->startIndexDyn->name, symEntry->endIndexDyn->name, nodeNameString[symEntry->type], symEntry->offset, symNode->nest, symEntry->isAssigned, symEntry->isReturn);
        }
    }
    else{
        printf("\tVar Name: [%s], Scope: [%s, %d-%d], Width: [%d], isArray: NO, ------ [--], Type: [%s], offset: [%d], nesting level: [%d], isAssigned: [%d], isReturn: [%d]\n", symEntry->name, modhash->key, symNode->scopeStart, symNode->scopeEnd, DATA_TYPE_SIZES[symEntry->type], nodeNameString[symEntry->type], symEntry->offset, symNode->nest, symEntry->isAssigned, symEntry->isReturn);
    }
    
    return;
}

int checkFunctionReturnType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    ASTnode *formalPar = moduleRoot->firstChild->sibling->sibling->firstChild;
    ASTnode *actualPar = reuseStmtRoot->firstChild->firstChild;
    symbolTableNode* moduleST = getModuleHashNode(moduleRoot->firstChild->syntaxTreeNode->lexeme, symbolForest, moduleRoot->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
    while(formalPar != NULL){
        symbolTableEntry* apEntry = getSymbolTableEntry(stable, actualPar->syntaxTreeNode->lexeme);
        if(apEntry==NULL){
            //ERROR
            printf("Line %d: Error - Variable %s assigned before declaration\n", actualPar->syntaxTreeNode->lineNumber, actualPar->syntaxTreeNode->lexeme);
            formalPar = formalPar->sibling;
            actualPar = actualPar->sibling;
            if((actualPar == NULL && formalPar!=NULL) || (actualPar != NULL && formalPar ==NULL)){
                printf("Line %d: Error - Number of return parameters differ.\n", reuseStmtRoot->firstChild->sibling->syntaxTreeNode->lineNumber);
                return -1;
            }
            continue;
        }
        symbolTableEntry* fpEntry = getSymbolTableEntry(moduleST, formalPar->firstChild->syntaxTreeNode->lexeme);

        if(fpEntry->type != apEntry->type){
            //ERROR
            printf("Line %d: Error - Type of %s differs from return type of function\n",actualPar->syntaxTreeNode->lineNumber, actualPar->syntaxTreeNode->lexeme);
        }
        if(fpEntry->isArray || apEntry->isArray){
            printf("Line %d: Error - Array variable returned\n", actualPar->syntaxTreeNode->lineNumber);
        }

        formalPar = formalPar->sibling;
        actualPar = actualPar->sibling;
        if((actualPar == NULL && formalPar!=NULL) || (actualPar != NULL && formalPar ==NULL)){
            printf("Line %d: Error - Number of return parameters differ.\n", reuseStmtRoot->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
    }
    
    return 1;
}

int checkFunctionParameterType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    ASTnode *formalPar = moduleRoot->firstChild->sibling->firstChild;//inputplist
    ASTnode *actualPar = reuseStmtRoot->firstChild->sibling->sibling->firstChild;//IDLIST_NODE
    symbolTableNode* moduleST = getModuleHashNode(moduleRoot->firstChild->syntaxTreeNode->lexeme, symbolForest, moduleRoot->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
    while(formalPar != NULL){
        symbolTableEntry* apEntry = getSymbolTableEntry(stable, actualPar->syntaxTreeNode->lexeme);
        if(apEntry==NULL){
            //ERROR
            printf("Line %d: Error - Variable %s used before declaration\n", actualPar->syntaxTreeNode->lineNumber, actualPar->syntaxTreeNode->lexeme);
            formalPar = formalPar->sibling;
            actualPar = actualPar->sibling;
            if((actualPar == NULL && formalPar!=NULL) || (actualPar != NULL && formalPar ==NULL)){
                printf("Line %d: Error - Number of input parameters differ.\n", reuseStmtRoot->firstChild->sibling->syntaxTreeNode->lineNumber);
                return -1;
            }
            continue;
        }
        symbolTableEntry* fpEntry = getSymbolTableEntry(moduleST, formalPar->firstChild->sibling->syntaxTreeNode->lexeme);

        if(fpEntry->type != apEntry->type){
            //ERROR
            printf("Line %d: Error - Type of %s differs from input type of function\n", actualPar->syntaxTreeNode->lineNumber, actualPar->syntaxTreeNode->lexeme);
        }else{
            if(fpEntry->isArray != apEntry->isArray){
                //ERROR
                //TODO
                printf("Line %d: Error - Type Mismatch\n", actualPar->syntaxTreeNode->lineNumber);
            }
            if (fpEntry->isArray==1 &&(fpEntry->endIndex-fpEntry->startIndex != apEntry->endIndex- apEntry->startIndex)){
                printf("Line %d: Error - Type Mismatch\n", actualPar->syntaxTreeNode->lineNumber);
            }
        }

        formalPar = formalPar->sibling;
        actualPar = actualPar->sibling;
        if((actualPar == NULL && formalPar!=NULL) || (actualPar != NULL && formalPar ==NULL)){
            printf("Line %d: Error - Number of input parameters differ.\n", reuseStmtRoot->firstChild->sibling->syntaxTreeNode->lineNumber);
            return -1;
        }
    }
    
    return 1;
}

moduleHashNode* getModuleHashNode(char *name, moduleHashNode* symbolForest[], int lineNumb){
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];
    symbolTableNode* stable;

    while(temp!=NULL){
        if(strcmp(temp->key, name)==0){
            return temp;
        }
        temp = temp->next;
    }
    printf("Line %d: Error - Module [%s] Not Found\n", lineNumb, name);
    return NULL;
}


whileList* checkWhileExprn(ASTnode* root, symbolTableNode* stable, whileList* list){
    // ASTnode* exprn = root->firstChild;
    if(root->label == ID_NODE){
        whileList* temp = (whileList*)malloc(sizeof(whileList));
        temp->varEntry = getSymbolTableEntry(stable, root->syntaxTreeNode->lexeme);
        temp->oldIsAssigned = temp->varEntry->isAssigned;
        temp->next = list;
        temp->varEntry->isAssigned = 0;
        return temp;
    }
    if(root->firstChild){
        list = checkWhileExprn(root->firstChild, stable, list);
    }
    if (root->sibling!=NULL && root->parent->label!=WHILEITERATIVESTMT_NODE){
        list = checkWhileExprn(root->sibling, stable, list);
    }
    return list;
    
}

void checkWhileIsAssigned(symbolTableNode* stable, whileList* list){
    whileList* temp = list;
    int flag =0;
    while(temp){
        char* var = temp->varEntry->name;
        symbolTableEntry* newEntry = getSymbolTableEntry(stable, var);
        if(newEntry->isAssigned==1){
            flag=1;
        }
        newEntry->isAssigned=temp->oldIsAssigned;
        temp = temp->next;
    }
    if(flag==0){
        printf("Line %d ERROR- While variable not changed\n", stable->scopeEnd);
    }
    return;
}