#include "symbolTable.h"
#include "symbolTableDef.h"
#define STRING_SIZE 200

int LABEL_NO = 1;
int VARIABLE_NO = 1;
int GLOBALOFFSET =0;

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]){
    

    fprintf(fptr, "\textern\tscanf\n");
    fprintf(fptr, "\texter\tprintf\n");

    fprintf(fptr, "SECTION\t.data\n");
    fprintf(fptr, "printI\tdb\"%d\" , 10, 0");
    fprintf(fptr, "printR\tdb\"%f\" , 10, 0");
    fprintf(fptr, "getI\tdb\"%d\" , 10, 0");
    fprintf(fptr, "getR\tdb\"%f\" , 10, 0");
    
    fprintf(fptr, "SECTION\t.text\n");
    fprintf(fptr, "\tglobal\tmain\nmain:\n");

    // generate();
    traverse_code_gen(root, fptr, symbolForest);

    fprintf(fptr,"SECTION\t.bss\n");
    printSymbolForestCodeGen(symbolForest, fptr);
    

}

void printSymbolForestCodeGen(moduleHashNode* symbolForest[], FILE* fptr){

    for(int i=0; i<MAX_MODULES; i++){
        if(symbolForest[i] != NULL){
            printModuleHashNodeCodeGen(symbolForest[i], fptr);
        }
    }    
}

void printModuleHashNodeCodeGen(moduleHashNode* modhash, FILE* fptr){

    if(modhash == NULL){
        return;
    }
    moduleHashNode* temp = modhash;

    while(temp != NULL){
        // printf("Module Name: [%s], isUsed: [%d], isDefined: [%d] astNode: [%s]\n",temp->key, temp->isUsed, temp->isDefined, nodeNameString[temp->moduleAst->label]);
        printSymbolTableNodeCodeGen(temp->tablePtr, temp, fptr);
        temp = temp->next;
    }
    return;
}

void printSymbolTableNodeCodeGen(symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr){
    if(symNode == NULL){
        return;
    }

    // printf("\n**new stable started in %s**", modhash->key);
    printVarHashTableCodeGen(symNode->varHashTable, symNode, modhash, fptr);
    int i = 0;

    while(i<MAX_SCOPES){
        if(symNode->childList[i] != NULL){
            printSymbolTableNodeCodeGen(symNode->childList[i], modhash, fptr);
        }
        i++;
    }

    return;
}

void printVarHashTableCodeGen(varHashNode* varht[], symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr){

    if(varht == NULL){
        return;
    }
    
    for(int i=0; i<VAR_SYMBOLTABLE_SIZE; i++){
        if(varht[i] != NULL){
            printVarHashNodeCodeGen(varht[i], symNode, modhash, fptr);
        }
    }

    return;
}

void printVarHashNodeCodeGen(varHashNode* varhn, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr){

    if (varhn == NULL)
        return;

    printSymbolTableEntryCodeGen(varhn->entryPtr, symNode, modhash, fptr);
    varHashNode *temp = varhn;
    while(temp->next != NULL){
        temp = temp->next;
        printVarHashNodeCodeGen(temp, symNode, modhash, fptr);
    }
    return; 
}

void printSymbolTableEntryCodeGen(symbolTableEntry* symEntry, symbolTableNode* symNode, moduleHashNode* modhash, FILE* fptr){

    if(DATA_TYPE_SIZES[symEntry->type] == 1){
        fprintf(fptr,"\t%s\tres%c\t%d\n",generateBssLexeme(symNode,symEntry->name), 'b',1);
    }
    else if(DATA_TYPE_SIZES[symEntry->type] == 2){
        fprintf(fptr,"\t%s\tres%c\t%d\n",generateBssLexeme(symNode,symEntry->name), 'w',1);
    }
    else if(DATA_TYPE_SIZES[symEntry->type] == 4){
        fprintf(fptr,"\t%s\tres%c\t%d\n",generateBssLexeme(symNode,symEntry->name), 'd',1);
    }
}

char* generateBssLexeme(symbolTableNode* stable, char* name){

    char* temp = (char*)malloc(sizeof(char)*STRING_SIZE);
    sprintf(temp,"%s",name);
    symbolTableNode* par;
    symbolTableNode* curr = stable;
    int i;

    while(curr->parent!= NULL){
        i=0;
        par = curr->parent;
        while(par->childList[i]!=NULL){
            if(curr == par->childList[i]){
                sprintf(temp, "_%d", i);
                break;
            }
            i++;
        }
        curr = curr->parent;
    }
    sprintf(temp, "_%s", curr->key);
    return temp;
}

char* new_label(){
	char* str = (char*)malloc(sizeof(char)*10);
	sprintf(str, "_L%d", LABEL_NO);
	LABEL_NO++;

	return str;
}


char* new_variable(){
	char* str = (char*)malloc(sizeof(char)*10);
	sprintf(str, "_t%d",VARIABLE_NO);
	VARIABLE_NO++;

	return str;
}

void traverse_code_gen(ASTnode* root, FILE *fptr, moduleHashNode* symbolForest[]){
	symbolTableNode* stable;
    ASTnode * temp = root->firstChild;
	while(temp != NULL){	
		if(temp->label == DRIVER_MOD_NODE){
            stable = getModuleHashNode("driverFunctionNode", symbolForest, temp->syntaxTreeNode->lineNumber);
            int scope = 0;
			ASTnode* node = temp->firstChild;
			while(node != NULL){
                statements(node, fptr, stable, &scope);
                node = node->sibling;
                // scope++;
            }
		}
		else if(temp->label==MODULE_NODE){
			// call func start code
            int scope = 0;
            stable = getModuleHashNode(temp->firstChild->syntaxTreeNode->lexeme, symbolForest, temp->firstChild->syntaxTreeNode->lineNumber);
			ASTnode* node = temp->firstChild;
			while(node != NULL){
                statements(node, fptr, stable, &scope);
                node = node->sibling;
            }
        }
			// call return code
		temp = temp->sibling;
	}
}

void statements(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope){
    if (root->label == PRINT_STMT_NODE){
        printStmt(root, fptr, stable->childList[*scope]);
    }
    else if (root->label == GET_STMT_NODE){
        getStmt(root, fptr, stable->childList[*scope]);
    }
    else if (root->label == FORITERATIVESTMT_NODE){
        forIterStmt(root, fptr, stable->childList[*scope]);
        (*scope)++;
    }
    else if (root->label == WHILEITERATIVESTMT_NODE){
        whileIterStmt(root, fptr, stable->childList[*scope]);
        scope++;
    }
    else if (root->label == CONDITIONALSTMT_NODE){
        conditionalStmt(root, fptr, stable->childList[*scope]);
        scope++;
    }
    else if (root->label == MODULEREUSESTMT_NODE){
        modulereuseStmt(root, fptr, stable->childList[*scope]);
    }
    else if (root->label == DECLARESTMT_NODE){
        declareStmt(root, fptr, stable->childList[*scope]);
    }
}



void printStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf

    symbolTableEntry* entry = getSymbolTableEntry(statements, root->firstChild);
    int offset = GLOBALOFFSET - entry->offset;

    if(entry->type==INT || entry->type==BOOL){
        fprintf(fptr, "mov\tr8w\tesp-%d\n", offset);
        fprintf(fptr, "push\tr8w\n");
        fprintf(fptr, "push\tprintI\n");
        //add msg in data fptr also "%d",10, 0
        fprintf(fptr, "call\tprintf\n");    
    }else if(entry->type==REAL){
        fprintf(fptr, "mov\tr8w\t%d\n", offset);
        fprintf(fptr, "push\tr8w\n");
        fprintf(fptr, "push\tprintR\n");
        //add msg in data fptr also  msgReal: db "%f",10, 0
        fprintf(fptr, "call\tprintf\n");
    }    
}

void getStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf


    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild);
    int offset = entry->offset;

    if(entry->type==INT || entry->type==BOOL){
        fprintf(fptr, "mov\tr8w\t%d\n", offset);
        fprintf(fptr, "push\tr8w\n");
        fprintf(fptr, "push\t getI\n");
        //add msg in data fptr also "%d",10, 0
        fprintf(fptr, "call\t_scanf\n");    
    }else if(entry->type==REAL){
        fprintf(fptr, "mov\tr8w\t%d\n", offset);
        fprintf(fptr, "push\tr8w\n");
        fprintf(fptr, "push\tgetR\n");
        //add msg in data fptr also  msgReal: db "%f",10, 0
        fprintf(fptr, "call\t_scanf\n");    
    }
}

void forIterStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope){

    //mov r1, startRange
    char* startRange = root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme;
    fprintf(fptr, "\tmov\tr8w,\t%s\n", startRange); //register name???
    
    //store ID,r1
    int offset = GLOBALOFFSET - getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme)->offset;
    // char s[MAXKEYLEN];
    // sprintf(s, "%d", offset);
    fprintf(fptr, "\tmov\t%d,\tr8w\n", offset);

    //l1: cmp r1, endindex
    char* label = new_label();
    char* endRange = root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme;
    fprintf(fptr, "%s: \n\tcmp\tr8w,\t%s\n", label, endRange);

    //jg l2 (next statement)
    char* labelNext = new_label();
    fprintf(fptr, "\tjg\tr8w,\t%s\n", labelNext);

    //for loop statements
    //TODO:call recursive function
    statements(root->firstChild->sibling->sibling, fptr, stable, &scope);

    //load r1, ID
    fprintf(fptr, "\tmov\tr8w,\t%d\n", offset);

    //inc r1
    fprintf(fptr, "\tinc\tr8w\n");

    //store ID, r1
    fprintf(fptr, "\tmov\t%d,\tr8w\n", offset);

    //jump l1
    fprintf(fptr, "\tjmp\t%s\n", label);
   
    //l2: next statement
    fprintf(fptr, "\n%s:\n", labelNext);

    return;
}

void whileIterStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope){

    //label1
    //call for boolean() - it gets back "1" or "0"
    // char check = boolean(root->firstChild);
    // cmp check with 1
    // jl (next statement)label2
    // call statements
    // jmp label1
    // label2

    char* label1 = new_label();
    char* label2 = new_label();

    //label1
    fprintf(fptr, "\n%s:\n", label1);
    //assigning value
    fprintf(fptr, "\tmov\tr8w,\t%s\n",boolean(root->firstChild, fptr, stable));

    // cmp
    fprintf(fptr, "\tcmp\tr8w,\t%s\n",1);
    
    //jump if less than
    fprintf(fptr, "\tjl\t%s\n", label2);

    // call statements
    statements(root->firstChild->sibling->sibling, fptr, stable, &scope);

    //jmp label1
    fprintf(fptr, "\tjmp\t%s\n", label1);

    //Label2
    fprintf(fptr, "\n%s:\n", label2);

    return;
}

void conditionalStmt(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope){
    
    // load r1, ID
    //while temp(casestmt)
    //cmp r1, val
    //jne nextCase
    //call internal statements
    //jmp nextStmt
    //nextCase: loop while
    //call default
    //nextStmt:
    
    ASTnode* temp = root->firstChild->sibling->firstChild;

    char* labelNextStmt = new_label();
    int switchVarOffset;
    switchVarOffset = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    // load r1, ID
    fprintf(fptr, "\tmov\tr8w,\t%d\n", switchVarOffset);
    
    while(temp){
        int caseValue;
        if(temp->label== INTEGER_NODE){
            caseValue = temp->syntaxTreeNode->value.num;
        }else{
            caseValue = strcmp(temp->syntaxTreeNode->lexeme, "FALSE");
        }

        //cmp r1, val
        fprintf(fptr, "\tcmp\tr8w,\t%s\n", caseValue);

        char* label = new_label();
        //jne nextCase
        fprintf(fptr, "jne\t%s\n", label);

        // call statements
        statements(temp->firstChild, fptr, stable, &scope);

        //jmp nextStmt
        fprintf(fptr, "jne\t%s\n", labelNextStmt);
        //nextCase: loop while
        fprintf(fptr, "%s:\n", label);


        temp= temp->sibling;
    }
    //call default
    statements(root->firstChild->sibling->sibling, fptr, stable);
    //nextStmt:
    fprintf(fptr, "\n%s:\n", labelNextStmt);
}
