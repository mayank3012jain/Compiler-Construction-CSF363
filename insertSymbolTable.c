#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include "astDef.h"

int checkKeyword(char * name){
    
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

int insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, int offset, int isAssigned, int isReturn){

    //check in keywords
    if(checkKeyword(name)==-1){
        return -1;
    }
    int ind = hashGivenIndex(name, 0, VAR_SYMBOLTABLE_SIZE-1);
    varHashNode* temp = varHashTable[ind];

    if(temp!=NULL){
        while(temp->next != NULL){
            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("ID declared again.");
                return -1;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("ID declared again.");
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
    emp->entryPtr->type = type;
    emp->entryPtr->isArray = isArray;
    emp->entryPtr->startIndex = startIndex;
    emp->entryPtr->endIndex = endIndex;
    emp->entryPtr->offset = offset;
    emp->entryPtr->isAssigned = isAssigned;
    emp->entryPtr->isReturn = isReturn;

    return 0;
} 

int insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[], moduleHashNode* next){

    if(checkKeyword(name)==-1){
        return -1;
    }
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];
    SymbolTableNode* stable;

    if(temp!=NULL){

        while(temp->next != NULL){
            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("Module declared again.");
                return -1;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Module declared again.");
            return -1;
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
    temp->tableptr = (SymbolTableNode*)malloc(sizeof(symbolTableNode)); // change value to tablePtr
    stable = temp->tablePtr;
    stable->parent = NULL;
    temp->next = NULL;
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
        symbolTableEntry ans = isDeclared(stNode->varHashTable, name)
        if(ans == NULL){
            tempST = tempST->parent;
        }
        else{
            return ans;
        }
    }
    return NULL;
}