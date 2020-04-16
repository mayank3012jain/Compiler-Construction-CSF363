#include "symbolTable.h"
#include "symbolTableDef.h"
#include "codeGen.h"
#define STRING_SIZE 200

int LABEL_NO = 1;
int VARIABLE_NO = 1;
int GLOBALOFFSET =0;

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]){

    fprintf(fptr, "extern\tscanf\n");
    fprintf(fptr, "extern\tprintf\n\n");

    fprintf(fptr, "SECTION\t.data\n");

    fprintf(fptr, "\tprintI\tdb\t\'%%d\', 10, 0\n");
    fprintf(fptr, "\tprintR\tdb\t\'%%f\', 10, 0\n");
    fprintf(fptr, "\tgetI\tdb\t\'%%d\', 10, 0\n");
    fprintf(fptr, "\tgetR\tdb\t\'%%f\', 10, 0\n\n");
    
    fprintf(fptr, "SECTION\t.text\n");
    fprintf(fptr, "\tglobal\tmain\n\nmain:\n");

    // generate();
    traverse_code_gen(root, fptr, symbolForest);

    fprintf(fptr,"\nSECTION\t.bss\n");
    printSymbolForestCodeGen(symbolForest, fptr);

}

void storeIntoReg(char* reg, char* mem,symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr){

    fprintf(fptr,"\tmov %s, %s\n", reg, mem);
    return;
}   

void loadIntoMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr){

    fprintf(fptr,"\tmov %s, %s\n",mem,reg);
    return;
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

void traverse_code_gen(ASTnode* root, FILE *fptr, moduleHashNode* symbolForest[]){
	symbolTableNode* stable;
    int scope;
    ASTnode * temp = root->firstChild;
	while(temp != NULL){	
		if(temp->label == DRIVER_MOD_NODE){
            stable = getModuleHashNode("driverFunctionNode", symbolForest, temp->syntaxTreeNode->lineNumber)->tablePtr;
            scope = 0;
			ASTnode* node = temp->firstChild;
			while(node != NULL){
                statementsCodeGen(node, fptr, stable, &scope, symbolForest);
                node = node->sibling;
                // scope++;
            }
		}
		// else if(temp->label==MODULE_NODE){
		// 	// call func start code
        //     scope = 0;
        //     stable = getModuleHashNode(temp->firstChild->syntaxTreeNode->lexeme, symbolForest, temp->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
		// 	ASTnode* node = temp->firstChild;
		// 	while(node != NULL){
        //         statementsCodeGen(node, fptr, stable, &scope);
        //         node = node->sibling;
        //     }
        // }
			// call return code
		temp = temp->sibling;
	}
}

void statementsCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]){
    if (root->label == PRINT_STMT_NODE){
        printStmtCodeGen(root, fptr, stable, symbolForest);
    }
    else if (root->label == GET_STMT_NODE){
        getStmtCodeGen(root, fptr, stable, symbolForest);
    }
    else if (root->label == FORITERATIVESTMT_NODE){
        forIterStmtCodeGen(root, fptr, stable->childList[*scope], scope,symbolForest);
        (*scope)++;
    }
    else if (root->label == WHILEITERATIVESTMT_NODE){
        whileIterStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest);
        (*scope)++;
    }
    else if (root->label == CONDITIONALSTMT_NODE){
        conditionalStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest);
        (*scope)++;
    }
    else if (root->label == ASSIGNOP_NODE){
        assignopCodeGen(root,fptr,stable,scope,symbolForest);
    }
    else if (root->label == ASSIGNOP_ARRAY_NODE){
        assignopArrayCodeGen(root,fptr,stable,scope,symbolForest);
    }
    // else if (root->label == MODULEREUSESTMT_NODE){
    //     modulereuseStmtCodeGen(root, fptr, stable, symbolForest);
    // }
}

void printStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf

    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    // symbolTableNode* symNode = getModuleHashNode(root->firstChild->syntaxTreeNode->lexeme, symbolForest, root->firstChild->syntaxTreeNode->lineNumber)->tablePtr;
    int offset = GLOBALOFFSET - entry->offset;

    if(entry->type==INT || entry->type==BOOL){
        storeIntoReg("r8w",NULL, entry, stable,symbolForest, fptr);
        // fprintf(fptr, "\tmov\tr8w,\t%s\n", generateBssLexeme(stable,entry->name));
        fprintf(fptr, "\tpush\tr8w\n");
        fprintf(fptr, "\tpush\tprintI\n");
        //add msg in data fptr also "%d",10, 0
        fprintf(fptr, "\tcall\t_printf\n");    
    }else if(entry->type==REAL){
        // fprintf(fptr, "\tmov\tr8w,\t%s\n", generateBssLexeme(stable,entry->name));
        storeIntoReg("r8w",NULL, entry, stable,symbolForest, fptr);
        fprintf(fptr, "\tpush\tr8w\n");
        fprintf(fptr, "\tpush\tprintR\n");
        //add msg in data fptr also  msgReal: db "%f",10, 0
        fprintf(fptr, "\tcall\t_printf\n");
    }    
}

void getStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf


    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    // symbolTableNode* symNode = getModuleHashNode(root->firstChild->syntaxTreeNode->lexeme, symbolForest, root->firstChild->syntaxTreeNode->lineNumber)->tablePtr;

    if(entry->type==INT || entry->type==BOOL){
        // fprintf(fptr, "\tmov\tr8w,\t%s\n", generateBssLexeme(stable,entry->name));
        storeIntoReg("r8w",NULL, entry, stable,symbolForest, fptr);
        fprintf(fptr, "\tpush\tr8w\n");
        fprintf(fptr, "\tpush\tgetI\n");
        //add msg in data fptr also "%d",10, 0
        fprintf(fptr, "\tcall\t_scanf\n");    

    }else if(entry->type==REAL){
        // fprintf(fptr, "\tmov\tr8w,\t%s\n", generateBssLexeme(stable,entry->name));
        storeIntoReg("r8w",NULL, entry, stable,symbolForest, fptr);
        fprintf(fptr, "\tpush\tr8w\n");
        fprintf(fptr, "\tpush\tgetR\n");
        //add msg in data fptr also  msgReal: db "%f",10, 0
        fprintf(fptr, "\tcall\t_scanf\n");    
    }
}

void forIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[]){

    
    char* startRange = root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme;
    char* endRange = root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme;
    symbolTableEntry* id = getSymbolTableEntry(stable,root->firstChild->syntaxTreeNode->lexeme);

    //mov r1, startRange
    fprintf(fptr, "\tmov\tr8w,\t%s\n", startRange); //register name???

    //store ID,r1
    // int offset = GLOBALOFFSET - getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme)->offset;
    // char s[MAXKEYLEN];
    // sprintf(s, "%d", offset);
    // fprintf(fptr, "\tmov\t%d,\tr8w\n", offset);
    loadIntoMem("r8w",NULL, id, stable,symbolForest, fptr);

    //l1: cmp r1, endindex
    char* label = new_label();
    fprintf(fptr, "%s: \n\tcmp\tr8w,\t%s\n", label, endRange);
    
    //jg l2 (next statement)
    char* labelNext = new_label();
    fprintf(fptr, "\tjg\tr8w,\t%s\n", labelNext);

    //for loop statements
    //TODO:call recursive function
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest);

    //load r1, ID
    // fprintf(fptr, "\tmov\tr8w,\t%d\n", offset);
    storeIntoReg("r8w",NULL,id,stable,symbolForest, fptr);

    //inc r1
    fprintf(fptr, "\tinc\tr8w\n");

    //store ID, r1
    // fprintf(fptr, "\tmov\t%d,\tr8w\n", offset);
    loadIntoMem("r8w",NULL,id,stable,symbolForest,fptr);

    //jump l1
    fprintf(fptr, "\tjmp\t%s\n", label);
   
    //l2: next statement
    fprintf(fptr, "\n%s:\n", labelNext);

    return;
}

void whileIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]){

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

    fprintf(fptr, "\tmov\tr8w, [t%d]\n",genArithExpr(root->firstChild, fptr));

    // cmp
    fprintf(fptr, "\tcmp\tr8w, %d\n",1);
    
    //jump if less than
    fprintf(fptr, "\tjl\t%s\n", label2);

    // call statements
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest);

    //jmp label1
    fprintf(fptr, "\tjmp\t%s\n", label1);

    //Label2
    fprintf(fptr, "\n%s:\n", label2);

    return;
}

void assignopCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]){

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);

    if(symEntry->isArray == 0){

        if(symEntry->type==INT){
            fprintf(fptr, "\tmov\tr8w, [t%d]\n",genArithExpr(root->firstChild->sibling, fptr));
            loadIntoMem("r8w",NULL,symEntry, stable, symbolForest, fptr);
            // fprintf(fptr, "\tmov\t[%s], r8w\n",root->firstChild->syntaxTreeNode->lexeme);
        }
        else if(symEntry->type==FLOAT){
            fprintf(fptr, "\tmov\tr8d, [t%d]\n",genArithExpr(root->firstChild->sibling, fptr));
            loadIntoMem("r8d",NULL,symEntry, stable, symbolForest, fptr);
        }
        else{
            fprintf(fptr, "\tmov\tr8b, [t%d]\n",genArithExpr(root->firstChild->sibling, fptr));
            loadIntoMem("r8b",NULL,symEntry, stable, symbolForest, fptr);        
        }
    }
}

void assignopArrayCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]){
    //check generateBssLexeme

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->firstChild->syntaxTreeNode->lexeme);

    if(symEntry->type==INT){
        fprintf(fptr, "\tmov\tr8w, [t%d]\n",genArithExpr(root->firstChild, fptr));
        // loadIntoMem("r8w",NULL,symEntry, stable, symbolForest, fptr);
        fprintf(fptr, "\tmov\t[%s+%d], r8w\n",generateBssLexeme(stable,symEntry->name), root->firstChild->firstChild->sibling->syntaxTreeNode->value.num);
    }
    else if(symEntry->type==FLOAT){
        fprintf(fptr, "\tmov\tr8d, [t%d]\n",genArithExpr(root->firstChild, fptr));
        // loadIntoMem("r8d",NULL,symEntry, stable, symbolForest, fptr);
        fprintf(fptr, "\tmov\t[%s+%d], r8w\n",generateBssLexeme(stable,symEntry->name), root->firstChild->firstChild->sibling->syntaxTreeNode->value.num);
    }
    else{
        fprintf(fptr, "\tmov\tr8b, [t%d]\n",genArithExpr(root->firstChild, fptr));
        // loadIntoMem("r8b",NULL,symEntry, stable, symbolForest, fptr);    
        fprintf(fptr, "\tmov\t[%s+%d], r8w\n",generateBssLexeme(stable,symEntry->name), root->firstChild->firstChild->sibling->syntaxTreeNode->value.num);
    }
}

void conditionalStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[]){
    
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
    symbolTableEntry* switchVarOffset = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    // load r1, ID
    // fprintf(fptr, "\tmov\tr8w,\t%d\n", switchVarOffset);
    storeIntoReg("r8w",NULL,switchVarOffset,stable,symbolForest,fptr);
    
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
        statementsCodeGen(temp->firstChild, fptr, stable, scope, symbolForest);

        //jmp nextStmt
        fprintf(fptr, "jne\t%s\n", labelNextStmt);
        //nextCase: loop while
        fprintf(fptr, "%s:\n", label);

        temp= temp->sibling;
    }
    //call default
    statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest);
    //nextStmt:
    fprintf(fptr, "\n%s:\n", labelNextStmt);
}

void modulereuseStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[]){

    int offset = 0;
    printIDList(fptr, root->firstChild->sibling->sibling->firstChild, stable, &offset, symbolForest);

    fprintf(fptr, "\tcall\t%s\n", root->firstChild->sibling->syntaxTreeNode->lexeme);
    fprintf(fptr, "\tadd\tesp, %d\n", offset);
}

void pushMem(char* reg, char* mem, symbolTableEntry* symEntry, symbolTableNode* stable, moduleHashNode* symbolForest[], FILE* fptr){
    
    fprintf(fptr,"\tpush\t%s",generateBssLexeme(stable,symEntry->name));
    return;
}
void declareStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable){
    
    ASTnode* typeNode = root->firstChild;
    ASTnode* id = typeNode->sibling;
    char* s[3];
    if(typeNode->label == INTEGER_NODE){
        strcpy(s, "r8w");
    }else if(typeNode->label == REAL_NODE){//TODO: change reg
        strcpy(s, "r8d");
    }else if(typeNode->label == BOOLEAN_NODE){
        strcpy(s, "r8b");
    }
    //push any value?
    while(id){
        fprintf(fptr, "\tpush\t%s");
        id = id->sibling;

    }
}

void moduleCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[]){
    //pop inputplist
    //copy stack pointer to module base pointer
    fprintf(fptr, "")
}


//To print in reverse order
void printIDList(FILE *fptr, ASTnode* node, symbolTableNode *stable, int *offset, moduleHashNode* symbolForest[]){
    if (node == NULL){
        return;
    }

    printIDList(fptr, node->sibling, stable, offset, symbolForest);

    // char *var = generateBssLexeme(stable, node->syntaxTreeNode->lexeme);
    symbolTableEntry* symEntry = getSymbolTableEntry(stable, node->syntaxTreeNode->lexeme);
    if(DATA_TYPE_SIZES[symEntry->type] == 1){
        // fprintf(fptr,"\tpush\tbyte\t%s\n", var);
        pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
        *offset += 1;
    }
    else if(DATA_TYPE_SIZES[symEntry->type] == 2){
        // fprintf(fptr,"\tpush\tword\t%s\n", var);
        pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
        *offset += 2;
    }
    else if(DATA_TYPE_SIZES[symEntry->type] == 4){
        // fprintf(fptr,"\tpush\tdword\t%s\n", var);
        pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
        *offset += 4;
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
        fprintf(fptr,"\t%s\tres%c\t%d\n",modhash->key, 'w',1);
        temp = temp->next;
    }
    return;
}



//AATO
// set this var to 0 before every expression call
static int interm_counter=0;

int genArithExpr(ASTnode *node, FILE *fptr) {
	
    if (node->label == NUM_NODE) {
		fprintf(fptr, "\t mov word [t%d], %d\n", ++interm_counter, node->syntaxTreeNode->value.num);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return interm_counter;
	}
	else if (node->label == RNUM_NODE) {
		fprintf(fptr, "\t mov dword [t%d], %f\n", ++interm_counter, node->syntaxTreeNode->value.rnum);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return interm_counter;
	}
	else if (node->label == TRUE_NODE) {
		fprintf(fptr, "\t mov byte [t%d], 1\n", ++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return interm_counter;
	}
	else if (node->label == FALSE_NODE) {
		fprintf(fptr, "\t mov byte [t%d], 0\n", ++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return interm_counter;
	}
	else if(node->label == ID_NODE) {
		// split into 2 movs
        fprintf(fptr, "\t mov \t [t%d], [%s]\n", ++interm_counter, node->syntaxTreeNode->lexeme);
        // fprintf(t_ptr, "t%d, ", interm_counter);
        return interm_counter;
    }
    else if(node->label == VARIDNUM_NODE){
        // If array
        if(node->firstChild->sibling){
            // If array indexed by variable
            if(node->firstChild->sibling->label==ID_NODE){   
                // Load index
                fprintf(fptr, "\t mov word si, [%s]\n", node->firstChild->sibling->syntaxTreeNode->lexeme);
                // Load array value //split
                fprintf(fptr, "\t mov ?? [t%d], [%s+si]\n", ++interm_counter, node->firstChild->syntaxTreeNode->lexeme);
                // fprintf(t_ptr, "t%d, ", interm_counter);
                return interm_counter;
            }
            // If array indexed by NUM
            else{
                // Load index
                fprintf(fptr, "\t mov si, %d\n", node->firstChild->sibling->syntaxTreeNode->value.num);
                // Load array value
                fprintf(fptr, "\t mov ?? [t%d], [%s+si]\n", ++interm_counter, node->firstChild->syntaxTreeNode->lexeme);
                // fprintf(t_ptr, "t%d, ", interm_counter);
                return interm_counter;
            }            
        }
        // If normal ID
        else{
            // split into 2 movs
            fprintf(fptr, "\t mov ?? [t%d], [%s]\n", ++interm_counter, node->syntaxTreeNode->lexeme);
            // fprintf(t_ptr, "t%d, ", interm_counter);
            return interm_counter;
        }
        
    }
    else if(node->label == PLUS_NODE){
        int left = genArithExpr(node->firstChild, fptr);
        // If binary
        if(node->firstChild->sibling){
            // mem to mem ops are not permitted; fix later
            int right = genArithExpr(node->firstChild->sibling, fptr);
            fprintf(fptr, "\t add ?? [t%d], [t%d]\n",left,right);
            fprintf(fptr, "\t mov ?? [t%d], [t%d]\n",++interm_counter,left);
            // fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
            return interm_counter;
        }
    }
    else if(node->label == MINUS_NODE){
        int left = genArithExpr(node->firstChild, fptr);
        // If binary
        if(node->firstChild->sibling){
            // mem to mem ops are not permitted; fix later
            int right = genArithExpr(node->firstChild->sibling, fptr);
            fprintf(fptr, "\t sub ?? [t%d], [t%d]\n",left,right);
            fprintf(fptr, "\t mov ?? [t%d], [t%d]\n",++interm_counter,left);
            // fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
            return interm_counter;
        }
    }
	else if(node->label == MUL_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		// mem to mem ops are not permitted; fix later
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word r8w, [t%d]\n",left);
		fprintf(fptr, "\t mov word r9w, [t%d]\n",right);
		fprintf(fptr, "\t imul r8w, r9w\n");
		fprintf(fptr, "\t mov word [t%d], r8w\n",++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == DIV_NODE){
        // float div deal later
		int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word ax, [t%d]\n",left);
		fprintf(fptr, "\t mov dx, 0\n");
		fprintf(fptr, "\t mov word r9w, [t%d]\n",right);
		fprintf(fptr, "\t idiv r9w\n");
		fprintf(fptr, "\t mov word [t%d], ax\n",++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == AND_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov byte r8b, [t%d]\n",left);
		fprintf(fptr, "\t and r8b, [t%d]\n",right);
		fprintf(fptr, "\t mov byte [t%d], r8b\n",++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == OR_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov byte r8b, [t%d]\n",left);
		fprintf(fptr, "\t or r8b, [t%d]\n",right);
		fprintf(fptr, "\t mov byte [t%d], r8b\n",++interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == LT_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word r8w, [t%d]\n",left);
		fprintf(fptr, "\t cmp r8w, [t%d]\n",right);
		int rand1; // init with rand nos
		fprintf(fptr, "\t jl true%d \n",rand1);
		fprintf(fptr, "\t jge false%d \n",rand1);
		fprintf(fptr, "false%d: mov byte [t%d], 0\n",rand1,++interm_counter);
		fprintf(fptr, "\tjmp exit%d\n",rand1);
		fprintf(fptr, "true%d: mov byte [t%d], 1\n",rand1,interm_counter);
		fprintf(fptr, "exit%d:\n",rand1);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == LE_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word r8w, [t%d\n]",left);
		fprintf(fptr, "\t cmp r8w, [t%d\n]",right);
		int rand1; // init with rand nos
		fprintf(fptr, "\t jle true%d \n",rand1);
		fprintf(fptr, "\t jg false%d \n",rand1);
		fprintf(fptr, "false%d: mov byte [t%d], 0\n",rand1,++interm_counter);
		fprintf(fptr, "\tjmp exit%d\n",rand1);
		fprintf(fptr, "true%d: mov byte [t%d], 1\n",rand1,interm_counter);
		fprintf(fptr, "exit%d:\n",rand1);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == GT_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word r8w, [t%d]\n",left);
		fprintf(fptr, "\t cmp r8w, [t%d]\n",right);
		int rand1; // init with rand nos
		fprintf(fptr, "\t jg true%d \n",rand1);
		fprintf(fptr, "\t jle false%d \n",rand1);
		fprintf(fptr, "false%d: mov byte [t%d], 0\n",rand1,++interm_counter);
		fprintf(fptr, "\tjmp exit%d\n",rand1);
		fprintf(fptr, "true%d: mov byte [t%d], 1\n",rand1,interm_counter);
		fprintf(fptr, "exit%d:\n",rand1);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	else if(node->label == GE_NODE){
        int left = genArithExpr(node->firstChild, fptr);
		int right = genArithExpr(node->firstChild->sibling, fptr);
		fprintf(fptr, "\t mov word r8w, [t%d]",left);
		fprintf(fptr, "\t cmp r8w, [t%d]",right);
		int rand1; // init with rand no
		fprintf(fptr, "\t jge true%d ",rand1);
		fprintf(fptr, "\t jl false%d ",rand1);
		fprintf(fptr, "false%d: mov byte [t%d], 0",rand1,++interm_counter);
		fprintf(fptr, "\t jmp exit%d",rand1);
		fprintf(fptr, "true%d: mov byte [t%d], 1\n",rand1,interm_counter);
		fprintf(fptr, "exit%d:\n",rand1);
		// fprintf(t_ptr, "t%d, ", interm_counter); // will depend on operand datatype
		return interm_counter;
    }
	// GT, GE, EQ
}