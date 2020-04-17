#include "symbolTable.h"
#include "symbolTableDef.h"
#include "codeGen.h"
#define STRING_SIZE 200

int LABEL_NO = 1;
int VARIABLE_NO = 1;
int GLOBAL =0;

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]){

    fprintf(fptr, "extern\tscanf\n");
    fprintf(fptr, "extern\tprintf\n\n");

    fprintf(fptr, "section\t.data\n");

    fprintf(fptr, "\tprintI\tdb\t\'%%d\', 10, 0\n");
    fprintf(fptr, "\tprintR\tdb\t\'%%f\', 10, 0\n");
    fprintf(fptr, "\tgetI\tdb\t\'%%d\', 10, 0\n");
    fprintf(fptr, "\tgetR\tdb\t\'%%f\', 10, 0\n");
    fprintf(fptr, "\tprintTRUE\tdb\t\'TRUE\', 10, 0\n");
    fprintf(fptr, "\tprintFALSE\tdb\t\'FALSE\', 10, 0\n\n");
    
    for(int i=0; i<40; i++){
        fprintf(fptr, "\tt%d\tdb\t0\n",i);
    }

    fprintf(fptr,"\nsection\t.bss\n\n");

    fprintf(fptr, "section\t.text\n");
    fprintf(fptr, "bits 64\n\n");
    fprintf(fptr, "global\tmain\n\nmain:\n");

    // generate();
    traverse_code_gen(root, fptr, symbolForest);

    // printSymbolForestCodeGen(symbolForest, fptr);

}

char* getReturnOffset(char* name, symbolTableNode* symNode, int* retOffset){

    symbolTableEntry* symEntry = getSymbolTableEntry(symNode, name);

    char* retc = (char*)malloc(sizeof(char)*STRING_SIZE);

    if(symEntry->isReturn == 1){ //outputplist
        sprintf(retc, "%s - %d", "rbx", symEntry->offset- *retOffset);//ebp
    }
    else if (symEntry->isReturn == 2){ //inputplist
        sprintf(retc, "%s + %d", "rbx", symEntry->offset+12);
    }
    else{ //local
        sprintf(retc, "%s - %d", "rbp", symEntry->offset);//rbp
    }
    
    return retc;
}

void cmpRegMem(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr){

    fprintf(fptr,"\tcmp %s, %s\n", reg, mem);
    return;
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

//root = PROGRAM
void traverse_code_gen(ASTnode* root, FILE *fptr, moduleHashNode* symbolForest[]){

	symbolTableNode* stable;
    int scope;
    ASTnode * temp = root->firstChild->sibling->sibling;
    if(temp->label == DRIVER_MOD_NODE){
        stable = getModuleHashNode("driverFunctionNode", symbolForest, temp->syntaxTreeNode->lineNumber)->tablePtr;
        scope = 0;
        ASTnode* node = temp->firstChild;
        fprintf(fptr,"\tpush rbp\n");
        fprintf(fptr,"\tmov rbp, rsp\n");
        while(node != NULL){
            statementsCodeGen(node, fptr, stable, &scope, symbolForest,0);
            node = node->sibling;
            // scope++;
        }
    }
    temp = root->firstChild->sibling->firstChild;
    while(temp){
        // call func start code
        scope = 0;
        stable = getModuleHashNode(temp->firstChild->syntaxTreeNode->lexeme, symbolForest, temp->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
        moduleCodeGen(temp, fptr, stable, symbolForest, &scope);
        
        temp = temp->sibling;
    }

    temp = root->firstChild->sibling->sibling->sibling->firstChild;
    while(temp){
        // call func start code
        scope = 0;
        stable = getModuleHashNode(temp->firstChild->syntaxTreeNode->lexeme, symbolForest, temp->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
        moduleCodeGen(temp, fptr, stable, symbolForest, &scope);
        
        temp = temp->sibling;
    }

}

void statementsCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset){
    
    if(root == NULL){
        printf("Called on NULL\n");
        return;
    }

    if(root->label == DECLARESTMT_NODE){
        declareStmtCodeGen(root, fptr, stable, retOffset);
    }

    if (root->label == PRINT_STMT_NODE){
        printStmtCodeGen(root, fptr, stable, symbolForest, retOffset);
    }
    else if (root->label == GET_STMT_NODE){
        getStmtCodeGen(root, fptr, stable, symbolForest, retOffset);
    }
    else if (root->label == FORITERATIVESTMT_NODE){
        forIterStmtCodeGen(root, fptr, stable->childList[*scope], scope,symbolForest, retOffset);
        (*scope)++;
    }
    else if (root->label == WHILEITERATIVESTMT_NODE){
        whileIterStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest, retOffset);
        (*scope)++;
    }
    else if (root->label == CONDITIONALSTMT_NODE){
        conditionalStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest, retOffset);
        (*scope)++;
    }
    else if (root->label == ASSIGNOP_NODE){
        assignopCodeGen(root,fptr,stable,scope,symbolForest, retOffset);
    }
    else if (root->label == ASSIGNOP_ARRAY_NODE){
        assignopArrayCodeGen(root,fptr,stable,scope,symbolForest, retOffset);
    }
    // else if (root->label == MODULEREUSESTMT_NODE){
    //     modulereuseStmtCodeGen(root, fptr, stable, symbolForest);
    // }
    // if(root->sibling != NULL){
    //     statementsCodeGen(root->sibling, fptr, stable, scope, symbolForest, reOffset);
    // }
}

void printStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int* retOffset){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf

    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(entry->name, stable, retOffset);
    if(entry->type==INT){
        fprintf(fptr, "\tmov r8, %s\n", offset);
        fprintf(fptr, "\tpush r8\n");
        fprintf(fptr, "\tpush %s\n", "printI");
        fprintf(fptr, "\tcall printf\n");
    }else if(entry->type==FLOAT){
        fprintf(fptr, "\tmov r8, %s\n", offset);
        fprintf(fptr, "\tpush r8\n");
        fprintf(fptr, "\tpush %s\n", "printR");
        fprintf(fptr, "\tcall printf\n");
    }else if(entry->type==BOOL){
        //mov r8w, offset
        //sub r8w, 1
        //jz l1
        //push TRUE
        //jmp l2
        //l1: push FALSE
        //l2: call printf
        char *l1 = new_label(), *l2 = new_label();  
        fprintf(fptr, "\tmov r8, %s\n", offset);
        fprintf(fptr, "\tsub r8, 1\n");
        fprintf(fptr, "\tjz %s\n", l1);
        fprintf(fptr, "\tpush %s\n", "printFALSE");
        fprintf(fptr, "\tjmp %s\n", l2);
        fprintf(fptr, "%s:\n", l1);
        fprintf(fptr, "\tpush %s\n", "printTRUE");
        fprintf(fptr, "%s:\n", l2);
        fprintf(fptr, "\tcall printf\n");
        
    }    
}

void getStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *retOffset){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call scanf

    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(entry->name, stable, retOffset);
    if(entry->type==INT || entry->type==BOOL){
        fprintf(fptr, "\tmov r8,\t%s\n", offset);
        fprintf(fptr, "\tpush r8\n");
        fprintf(fptr, "\tpush getI\n");
        //add msg in data fptr also "%d",10, 0
        fprintf(fptr, "\tcall scanf\n");    

    }else if(entry->type==REAL){
        fprintf(fptr, "\tmov r8,\t%s\n", offset);
        fprintf(fptr, "\tpush r8\n");
        fprintf(fptr, "\tpush getR\n");
        //add msg in data fptr also  msgReal: db "%f",10, 0
        fprintf(fptr, "\tcall scanf\n");    
    }
}

void forIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset){

    
    char* startRange = root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme;
    char* endRange = root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme;
    symbolTableEntry* id = getSymbolTableEntry(stable,root->firstChild->syntaxTreeNode->lexeme);

    //mov r1, startRange
    fprintf(fptr, "\tmov\tr8w,\t%s\n", startRange); //register name???

    //store ID,r1
    char* offset = getReturnOffset(id->name, stable, retOffset);
    fprintf(fptr, "\tmov\t[%s],\tr8w\n", offset);
    
    //l1: cmp r1, endindex
    char* label = new_label();
    fprintf(fptr, "%s: \n\tcmp\tr8w,\t%s\n", label, endRange);
    
    //jg l2 (next statement)
    char* labelNext = new_label();
    fprintf(fptr, "\tjg\tr8w,\t%s\n", labelNext);

    //for loop statements
    //TODO:call recursive function
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest, retOffset);

    //load r1, ID
    fprintf(fptr, "\tmov\tr8w,\t[%s]\n", offset);

    //inc r1
    fprintf(fptr, "\tinc\tr8w\n");

    //store ID, r1
    fprintf(fptr, "\tmov\t[%s],\tr8w\n", offset);

    //jump l1
    fprintf(fptr, "\tjmp\t%s\n", label);
   
    //l2: next statement
    fprintf(fptr, "\n%s:\n", labelNext);

    return;
}

void whileIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset){

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
    genExpr(root->firstChild, fptr, 0, stable, retOffset);

    fprintf(fptr, "\tmov\tr8w, [t%d]\n",0);

    // cmp
    fprintf(fptr, "\tcmp\tr8w, %d\n",1);
    
    //jump if less than
    fprintf(fptr, "\tjl\t%s\n", label2);

    // call statements
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest,retOffset);

    //jmp label1
    fprintf(fptr, "\tjmp\t%s\n", label1);

    //Label2
    fprintf(fptr, "\n%s:\n", label2);

    return;
}

void assignopCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset){

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(symEntry->name, stable, retOffset);
    genExpr(root->firstChild->sibling, fptr, 0, stable, retOffset);

    if(symEntry->isArray == 0){

        if(symEntry->type==INT){
            
            fprintf(fptr, "\tmov\tr8w, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8w\n",offset);
        }
        else if(symEntry->type==FLOAT){
            fprintf(fptr, "\tmov\tr8d, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8d\n",offset);
        }
        else{
            fprintf(fptr, "\tmov\tr8b, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8b\n",offset);
        }
    }
}

void assignopArrayCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset){
    //check generateBssLexeme

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(symEntry->name, stable, retOffset);
    genExpr(root->firstChild->sibling, fptr, 0, stable, retOffset);

    if(symEntry->isArray == 0){

        if(symEntry->type==INT){
            
            fprintf(fptr, "\tmov\tr8w, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8w\n",offset);//offset is wrong, add number of element
        }
        else if(symEntry->type==FLOAT){
            fprintf(fptr, "\tmov\tr8d, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8d\n",offset);
        }
        else{
            fprintf(fptr, "\tmov\tr8b, [t%d]\n",0);
            fprintf(fptr, "\tmov\t[%s], r8b\n",offset);
        }
    }
}

void conditionalStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset){
    
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
    // int switchVarOffset;
    symbolTableEntry* switchVar = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    // load r1, ID
    char* offset = getReturnOffset(switchVar->name, stable, retOffset);
    fprintf(fptr, "\tmov\tr8w,\t[%s]\n", offset);
    // storeIntoReg("r8w",NULL,switchVarOffset,stable,symbolForest,fptr);
    
    while(temp){
        int caseValue;
        if(temp->label== INTEGER_NODE){
            caseValue = temp->syntaxTreeNode->value.num;
        }else{
            caseValue = strcmp(temp->syntaxTreeNode->lexeme, "FALSE");
        }

        //cmp r1, val
        fprintf(fptr, "\tcmp\tr8w,\t%d\n", caseValue);

        char* label = new_label();
        //jne nextCase
        fprintf(fptr, "jne\t%s\n", label);

        // call statements
        statementsCodeGen(temp->firstChild, fptr, stable, scope, symbolForest, retOffset);

        //jmp nextStmt
        fprintf(fptr, "jne\t%s\n", labelNextStmt);
        //nextCase: loop while
        fprintf(fptr, "%s:\n", label);

        temp= temp->sibling;
    }
    //call default
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest, retOffset);
    //nextStmt:
    fprintf(fptr, "\n%s:\n", labelNextStmt);
}

void modulereuseStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int* retOffset){

    printIDList(fptr, root->firstChild->sibling->sibling->firstChild, stable, symbolForest, retOffset);

    fprintf(fptr, "\tcall\t%s\n", root->firstChild->sibling->syntaxTreeNode->lexeme);
    // fprintf(fptr, "\tadd\trsp, %d\n", offset);
}

void declareStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int* retOffset){
    
    ASTnode* typeNode = root->firstChild;
    ASTnode* temp = typeNode->sibling;
    int size = 0;
    while(temp){

        size++;
        
        temp=temp->sibling;
    }
    size *= DATA_TYPE_SIZES[typeNode->label];
    fprintf(fptr, "\tsub\trsp, %d\n", size);
}

void moduleCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *scope){
    //push ebp    
    //mov ebp, rsp
    //store rsp in modulebss_1
    //call on return_node
    //store rsp in modulebss_2
    //call Statements
    //leave
    //ret
    
    int returnOffset = 0;
    ASTnode* temp = root->firstChild->sibling->sibling->sibling->firstChild;
    fprintf(fptr, "%s:\n", root->firstChild->syntaxTreeNode->lexeme);
    fprintf(fptr, "\tpush\trbx\n");
    fprintf(fptr, "\tpush\trbp\n");
    fprintf(fptr, "\tmov\trbx, rsp\n");
    // fprintf(fptr,"\tmov\t_%s_ret, %s", root->firstChild->syntaxTreeNode->lexeme, "rsp");
    returnCodeGen(root->firstChild->sibling->sibling, fptr, stable, symbolForest, scope, &returnOffset);//check scope
    fprintf(fptr,"\tmov rbx, %s", "rsp");

    while(temp != NULL){
        statementsCodeGen(temp, fptr, stable, scope, symbolForest, &returnOffset);
        temp = temp->sibling;
    }

    fprintf(fptr, "\tleave\n");
    fprintf(fptr, "\tret\n");

    return; 
}

void returnCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *scope, int *retOffset){

    if(root->label!= RET_HEADER_NODE){
        printf("************************************");
    }

    ASTnode* temp = root->firstChild;
    *retOffset = getSymbolTableEntry(stable, temp->firstChild->syntaxTreeNode->lexeme)->offset;
    
    int size = 0;
    while(temp){

        if(temp->label == INTEGER_NODE){
            size += DATA_TYPE_SIZES[INT];
        }
        else if(temp->label == REAL_NODE){//TODO: change reg
            size += DATA_TYPE_SIZES[FLOAT];
        }
        else if(temp->label == BOOLEAN_NODE){
            size += DATA_TYPE_SIZES[BOOL];
        }
        
        temp=temp->sibling;
    }
    
    fprintf(fptr, "\tsub\trsp, %d\n", size);
    return;
}

//To print in reverse order
void printIDList(FILE *fptr, ASTnode* node, symbolTableNode *stable, moduleHashNode* symbolForest[], int* retOffset){
    if (node == NULL){
        return;
    }

    printIDList(fptr, node->sibling, stable, symbolForest, retOffset);

    // char *var = generateBssLexeme(stable, node->syntaxTreeNode->lexeme);
    symbolTableEntry* symEntry = getSymbolTableEntry(stable, node->syntaxTreeNode->lexeme);

    if(symEntry->isArray == 1){
        if(symEntry->isStatic == 1){
            
        }
        else{
            //fatt jayegi
        }
        // *offset += 5;
    }
    else{
        if(DATA_TYPE_SIZES[symEntry->type] == BOOL){
            fprintf(fptr,"\tpush\t byte[%s]\n", getReturnOffset(symEntry->name, stable, retOffset));
            // pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
            // *offset += BOOL;
        }
        else if(DATA_TYPE_SIZES[symEntry->type] == INT){
            fprintf(fptr,"\tpush\t word[%s]\n", getReturnOffset(symEntry->name, stable, retOffset));
            // pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
            // *offset += INT;
        }
        else if(DATA_TYPE_SIZES[symEntry->type] == FLOAT){
            fprintf(fptr,"\tpush\t dword[%s]\n", getReturnOffset(symEntry->name, stable, retOffset));
            // pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
            // *offset += FLOAT;
        }
    }
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
        fprintf(fptr,"\t_%s_ret\tres%c\t%d\n",modhash->key, 'w',1);
        fprintf(fptr,"\t_%s_local\tres%c\t%d\n",modhash->key, 'w',1);
        temp = temp->next;
    }
    return;
}

