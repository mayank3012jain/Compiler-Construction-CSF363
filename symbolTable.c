

#include "astDef.h"
#include"ast.h"
#include"symbolTable.h"

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

void insert_into_stable(varHashNode* varHashTable[], char* name, int type, int isArray, int startIndex, int endIndex, int offset){

}


void traverse_ast(ASTnode* root){
    moduleHashNode* symbolForest[MAX_MODULES];
    symbolForest[0] = malloc(sizeof(hashNode));
    symbolForest[0]->tablePtr = malloc(sizeof(symbolTable));
    symbolTable* stable = symbolForest[0]->tablePtr;
    stable->parent = NULL;
    traverse_ast_recurse(root,stable,symbolForest);
}

void traverse_ast_recurse(ASTnode* root, symbolTable* stable, hashNode* symbolForest[]){
    // hashNode* symbolForest = malloc(sizeof(hashNode));

    // If variable -> add entry to symbol table
    // calculate size of datatype
    
    // If function
    if(root->label == MODULE_NODE){

        char* token = root->firstChild->syntaxTreeNode->token;
        int hash_val = hashGivenIndex(token,1,MAX_MODULES);
        strcpy(symbolForest[hash_val]->key, token);
        symbolForest[hash_val]->value = malloc(sizeof(symbolTable)); // change value to tablePtr
        stable = symbolForest[hash_val]->value;
        stable->parent = NULL;
        // insert func params to symbol table


        traverse_ast_recurse(root->firstChild->sibling->sibling->sibling, stable, symbolForest);

        //check for assignment before Return
    }

    // If new scope
    if(root->label == CONDITIONALSTMT_NODE || root->label== FORITERATIVESTMT_NODE || root->label == WHILEITERATIVESTMT_NODE){
        
        symbolTable* temp = malloc(sizeof(symbolTable));
        temp->parent = stable;
        int i = 0;
        while(i<MAX_SCOPES){
            if(stable->childList[i] == NULL)
                break;
            i++;
        }
        stable->childList[i] = temp;
        stable = temp;
        temp = temp->parent;
        traverse_ast_recurse(root->firstChild, stable, symbolForest);
        if(root->sibling!=NULL){
            return traverse_ast_recurse(root->sibling, temp, symbolForest);
        }
        else
            return;
    }

    // If new variable
    if(root->label==DECLARESTMT_NODE){
        // calculate size of variable
        int i=0;
        while(i<MAX_SCOPE_VARS){
            if(stable->entryList[i] == NULL){
                break;
            i++;
            }
        }
        int offset;
        if(i==0)
            offset=0;
        else{
            if(stable->entryList[i-1]->isArray == 1){
                offset = (stable->entryList[i-1]->endIndex - 
                        stable->entryList[i-1]->startIndex) * 
                        DATA_TYPE_SIZES[stable->entryList[i-1]->type];
                offset += stable->entryList[i-1]->offset;        
            }
            else
                offset = stable->entryList[i-1]->offset + 
                        DATA_TYPE_SIZES[stable->entryList[i-1]->type];
        }
        // calculate data type & size
        // do
        // // get var name
        // // insert into entryList
        // until child of node has no more siblings
        int isArray, type, size, width, startIndex, endIndex; isArray=type=size=width=startIndex=endIndex=0;
        if(root->firstChild->label == DATATYPE_ARRAY_NODE){
            isArray = 1;
            type = root->firstChild->firstChild->sibling->label;
            size = DATA_TYPE_SIZES[type];
            startIndex = root->firstChild->firstChild->firstChild->syntaxTreeNode->value.num;
            endIndex = root->firstChild->firstChild->firstChild->sibling->syntaxTreeNode->value.num;
            width = (endIndex - startIndex) * size;
        }
        else{
            type = root->firstChild->label;
            size = DATA_TYPE_SIZES[type];
            width = size;
        }
        ASTnode* temp = root->firstChild->sibling;
        do{
            char* name = temp->syntaxTreeNode->lexeme;
            temp = temp->sibling;
            insert_into_stable(stable->entryList, name, type, isArray, startIndex, endIndex, offset);
            offset += width;
        }while(temp != NULL);
    }

    //check for assignment before declaration;

    if((root->label == ASSGNOP_NODE && root->firstChild->label == ID) || (root->label ==  GET_STMT_NODE)){
        //pass root(AST), 
        symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
        if(entry == NULL){
            //Raise Error
            printf("ID not declared before ");
        }
        else{
            entry->isAssigned = 1;
        }
    }else if(root->label == MODULEREUSE_NODE){

        ASTNode* temp = root->firstChild->firstChild;
        
        while(temp!= NULL){
            symbolTableEntry* entry = getSymbolTableEntry(stable, temp->syntaxTreeNode->lexeme);

            if(entry == NULL){
                //Raise Error
                printf("ID not declared before");
            }
            else{
                entry->isAssigned = 1;
            }
            temp = temp->sibling;
        }
    }

}
