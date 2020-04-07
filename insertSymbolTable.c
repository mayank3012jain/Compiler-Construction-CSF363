#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include "astDef.h"

int checkKeyword(char *name){
    
    int ind = hash(name);
    hashNode* temp = symbolTable[ind];
    while(temp!=NULL){
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("ID same as Keyword");
        }
        temp = temp->next;
    }
    return -1; //Change  if req    
}

symbolTableNode* allocateSymbolTable(symbolTableNode* parent, int offset){

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

    return stable;
}

int insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, int offset, int isAssigned, int isReturn){

    //check in keywords
    if(checkKeyword(name)!=-1){
        return -1;
    }
    int ind = hashGivenIndex(name, 0, VAR_SYMBOLTABLE_SIZE-1);
    varHashNode* temp = varHashTable[ind];

    if(temp!=NULL){
        while(temp->next != NULL){
            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("ID declared again [%s]\n", name);
                return -1;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("ID declared again [%s]\n", name);
            return -1;
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
    temp->entryPtr->offset = offset;
    temp->entryPtr->isAssigned = isAssigned;
    temp->entryPtr->isReturn = isReturn;

    // calculate size of entry
    int width = 0;
    if(isArray){
        int size = DATA_TYPE_SIZES[type];
        width = (endIndex - startIndex) * size;
    }
    else{
        width = DATA_TYPE_SIZES[type];
    }

    return width;
} 

symbolTableNode* insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[], ASTnode* moduleRoot){

    if(checkKeyword(name)!=-1){
        return NULL;
    }
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];

    if(temp!=NULL){

        while(temp->next != NULL){

            if(strcmp(temp->key, name)==0){
                
                if(temp->isDefined == 1){
                    //Raise Error
                    printf("Module defined again.\n");
                    return NULL;
                }
                else if(temp->isUsed == 0){
                    //Raise Error
                    printf("Module declared but not used before Definition\n");
                    return NULL;
                }
                else{
                    temp->isDefined == 1;
                    return temp->tablePtr;
                }
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){

            if(temp->isDefined == 1){
                //Raise Error
                printf("Module defined again.\n");
                return NULL;
            }
            else if(temp->isUsed == 0){
                //Raise Error
                printf("Module Declared but not Used before Definition\n");
                return NULL;
            }
            else{
                temp->isDefined == 1;
                return temp->tablePtr;
            }
        }

        temp->next = (moduleHashNode*)malloc(sizeof(moduleHashNode));
        temp = temp->next;
        // printf("temp wasnt null for %s", keywords[i]);
    }
    else{
        symbolForest[ind] = (moduleHashNode*)malloc(sizeof(moduleHashNode));
        temp = symbolForest[ind];
        // printf("temp was NULL for %s", keywords[i]);
    }

    strcpy(temp->key, name);
    temp->isUsed = 0;
    temp->isDefined = 1;
    temp->tablePtr = allocateSymbolTable(NULL,0);
    temp->next = NULL;
    temp->moduleAst= moduleRoot;

    return temp->tablePtr;
}

//Returns error if module already declared
//Creates a new node otherwise
void check_module_dec(char* name, moduleHashNode *symbolForest[], ASTnode* ast){
    
    if(checkKeyword(name)!=-1){
        return;
    }

    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];

    if(temp!=NULL){

        while(temp->next != NULL){

            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("Declaration Repeated\n");
                return;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Declaration Repeated\n");
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
    temp->tablePtr = allocateSymbolTable(NULL, 0);
    temp->next = NULL;
    temp->moduleAst = ast;
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