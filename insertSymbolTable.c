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

symbolTableNode* insert_into_moduleHashNode(char *name, moduleHashNode* symbolForest[]){

    if(checkKeyword(name)==-1){
        return NULL;
    }
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];
    symbolTableNode* stable;

    if(temp!=NULL){

        while(temp->next != NULL){
            if(strcmp(temp->key, name)==0){
                //Raise Error
                printf("Module declared again.\n");
                return NULL;
            }
            temp = temp->next;
        }
        if(strcmp(temp->key, name)==0){
            //Raise Error
            printf("Module declared again.\n");
            return NULL;
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
    temp->tablePtr = (symbolTableNode*)malloc(sizeof(symbolTableNode)); // change value to tablePtr
    stable = temp->tablePtr;
    stable->parent = NULL;
    temp->next = NULL;

    return stable;
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
        symbolTableEntry* ans = isDeclared(stNode->varHashTable, name);
        if(ans == NULL){
            tempST = tempST->parent;
        }
        else{
            return ans;
        }
    }
    return NULL;
}
