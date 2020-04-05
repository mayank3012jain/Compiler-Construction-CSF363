#include "symbolTable.h"

int checkFunctionReturnType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    ASTnode *formalPar = moduleRoot->firstChild->sibling->sibling->firstChild;
    ASTnode *actualPar = reuseStmtRoot->firstChild->firstChild;
    symbolTableNode* moduleST = getModuleHashNode(moduleRoot->firstChild->syntaxTreeNode->lexeme, symbolForest)->tablePtr;
    while(formalPar != NULL){
        symbolTableEntry* apEntry = getSymbolTableEntry(stable, actualPar->syntaxTreeNode->lexeme);
        if(apEntry==NULL){
            //ERROR
            printf("Variable %s assigned before declaration", actualPar->syntaxTreeNode->lexeme);
        }
        symbolTableEntry* fpEntry = getSymbolTableEntry(moduleST, formalPar->firstChild->syntaxTreeNode->lexeme);

        if(fpEntry->type != apEntry->type){
            //ERROR
            printf("Type of %s differs from return type of function",actualPar->syntaxTreeNode->lexeme);
        }

        formalPar = formalPar->sibling;
        actualPar = actualPar->sibling;
        if(actualPar == NULL){
            printf("Number of return parameters differ.");
            return -1;
        }
    }
    if(actualPar != NULL){
        printf("Number of return parameters differ.");
        return -1;
    }
    return 1;
}

int checkFunctionParameterType(ASTnode* moduleRoot, ASTnode* reuseStmtRoot, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    ASTnode *formalPar = moduleRoot->firstChild->sibling->firstChild;//inputplist
    ASTnode *actualPar = reuseStmtRoot->firstChild->sibling->sibling->firstChild;//IDLIST_NODE
    symbolTableNode* moduleST = getModuleHashNode(moduleRoot->firstChild->syntaxTreeNode->lexeme, symbolForest)->tablePtr;
    while(formalPar != NULL){
        symbolTableEntry* apEntry = getSymbolTableEntry(stable, actualPar->syntaxTreeNode->lexeme);
        if(apEntry==NULL){
            //ERROR
            printf("Variable %s used before declaration", actualPar->syntaxTreeNode->lexeme);
        }
        symbolTableEntry* fpEntry = getSymbolTableEntry(moduleST, formalPar->firstChild->sibling->syntaxTreeNode->lexeme);

        if(fpEntry->type != apEntry->type){
            //ERROR
            printf("Type of %s differs from return type of function",actualPar->syntaxTreeNode->lexeme);
        }else{
            if(fpEntry->isArray != apEntry->isArray){
                //ERROR
                //TODO
            }
        }

        formalPar = formalPar->sibling;
        actualPar = actualPar->sibling;
        if(actualPar == NULL){
            printf("Number of return parameters differ.");
            return -1;
        }
    }
    if(actualPar != NULL){
        printf("Number of return parameters differ.");
        return -1;
    }
    return 1;
}

//get symbolTable for a given module
moduleHashNode* getModuleHashNode(char *name, moduleHashNode* symbolForest[]){
    int ind = hashGivenIndex(name, 1, MAX_MODULES-1);
    moduleHashNode* temp = symbolForest[ind];
    symbolTableNode* stable;

    while(temp!=NULL){
        if(strcmp(temp->key, name)==0){
            return temp;
        }
        temp = temp->next;
    }
    printf("Module not found\n");
    return NULL;
}