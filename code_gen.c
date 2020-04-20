#include "symbolTable.h"
#include "symbolTableDef.h"
#include "codeGen.h"
#define STRING_SIZE 200

int LABEL_NO = 1;
int VARIABLE_NO = 1;
int GLOBAL =0;
int STACK_VARIABLE_NO =0;

void initializeCodeGen(ASTnode* root, FILE* fptr, moduleHashNode* symbolForest[]){

    fprintf(fptr, "extern\tscanf\n");
    fprintf(fptr, "extern\tprintf\n\n");

    fprintf(fptr, "section\t.data\n");
    fprintf(fptr, "\tprintTRUE\tdb\t\'Output: TRUE\', 10, 0\n");
    fprintf(fptr, "\tprintFALSE\tdb\t\'Output: FALSE\', 10, 0\n");
    fprintf(fptr, "\tprintI\tdb\t\'Output: %%d\', 10, 0\n");
    fprintf(fptr, "\tprintR\tdb\t\'Output: %%f\', 10, 0\n");

    fprintf(fptr, "\tprintINPUTINT\tdb\t\'Input: Enter an INTEGER value:\', 10, 0\n");
    fprintf(fptr, "\tprintINPUTBOOL\tdb\t\'Input: Enter a BOOLEAN (0/1) Value:\', 10, 0\n");
    fprintf(fptr, "\tprintINPUTINTARR\tdb\t\'Input: Enter %%d INTEGER values:\', 10, 0\n");
    fprintf(fptr, "\tprintINPUTBOOLARR\tdb\t\'Input: Enter %%d BOOLEAN (0/1) values:\', 10, 0\n");

    fprintf(fptr, "\tprintArrUtil1\tdb\t\'Output: \', 0\n");
    fprintf(fptr, "\tprintIArr\tdb\t\'%%d \', 0\n");
    fprintf(fptr, "\tprintRArr\tdb\t\'%%f \', 0\n");
    fprintf(fptr, "\tprintArrUtil2\tdb\t\' \', 10, 0\n");

    fprintf(fptr, "\tgetI\tdb\t\'%%d\', 0\n");
    fprintf(fptr, "\tgetR\tdb\t\'%%f\', 0\n");

    fprintf(fptr, "\tprintTRUEArr\tdb\t\'TRUE \', 0\n");
    fprintf(fptr, "\tprintFALSEArr\tdb\t\'FALSE \', 0\n\n");

    fprintf(fptr, "\tprintError\tdb\t\'Error- Out of bounds \', 10, 0\n\n");
    
    fprintf(fptr,"\tbufferInt\tdb\t0\n");
	fprintf(fptr,"\toutputInt\tdb\t0\n");
    
    for(int i=0; i<40; i++){
        fprintf(fptr, "\tt%d\tdw\t0\n",i);
        fprintf(fptr, "\t_st%d\tdq\t0\n",i);//check size
    }

    fprintf(fptr,"\nsection\t.bss\n\n");

    fprintf(fptr, "section\t.text\n");
    fprintf(fptr, "bits 64\n\n");
    fprintf(fptr, "global\tmain\n\nmain:\n");

    // generate();
    traverse_code_gen(root, fptr, symbolForest);

    // printSymbolForestCodeGen(symbolForest, fptr);

    fprintf(fptr, "\n_exit:\n\tmov rax, 1\n\tmov rbx, 0\n\tint 80h\n");
	
	
}

char* getReturnOffset(char* name, symbolTableNode* symNode, int* retOffset, int size){

    symbolTableEntry* symEntry = getSymbolTableEntry(symNode, name);
    char* retc = (char*)malloc(sizeof(char)*STRING_SIZE);

    if(symEntry->isArray == 0){
        if(symEntry->isReturn == 1){ //outputplist
            sprintf(retc, "%s - %d", "rbp", symEntry->offset - (*retOffset) + symEntry->width);//ebp
        }
        else if (symEntry->isReturn == 2){ //inputplist
            sprintf(retc, "%s + %d", "rbp", symEntry->offset + 16);
        }
        else{ //local
            sprintf(retc, "%s - %d", "rbp", symEntry->offset + size + symEntry->width);//rbp
        }
    }
    else{//array
        if(symEntry->isStatic == 0){//dynamic arrays
            if(symEntry->isReturn==0){//local
                sprintf(retc,"%s - %d", "rbp", symEntry->offset+size+8);
            }
            else{//input
                sprintf(retc,"%s + %d", "rbp", symEntry->offset+16);
            }
        }
        else{//static
            if(symEntry->isReturn==0){//local
                sprintf(retc, "%s - %d", "rbp", symEntry->offset + size + DATA_TYPE_SIZES[symEntry->type]);//rbp
            }
            else{
                sprintf(retc, "%s + %d", "rbp", 16 + symEntry->offset);
                //mov r1, qword[rbp+16+offset]
                //mov t0, word/byte[r1+i]; i is for element, r1 is base address of array
            }
            
        }
    }
    
    return retc;
}

//reg and reg1 8byte // returns address value in reg1
void getArrayElement(char* reg, char* reg1, char* i, symbolTableEntry* arrSymEntry, symbolTableNode* stable, FILE* fptr, int* retOffset, int size){
    if(arrSymEntry->isStatic==0){//dynamic
        if(arrSymEntry->isReturn == 0){//local
            //mov reg,i
            //imul reg, sizeofele
            //mov r1, [offset]
            //sub reg, r1
            char* offset = getReturnOffset(arrSymEntry->name, stable, retOffset, size);
            fprintf(fptr, "\tmov %s, %s\n", reg, i);
            // fprintf(fptr,"\tadd %s, 1\n",reg);//done in declare
            fprintf(fptr, "\timul %s, %d\n", reg, DATA_TYPE_SIZES[arrSymEntry->type]);
            fprintf(fptr, "\tmov %s, qword[%s]\n", reg1, offset);
            // fprintf(fptr, "\timul %s, %d\n", reg, DATA_TYPE_SIZES[arrSymEntry->type]);
            fprintf(fptr,"\tsub %s, %s\n",reg1, reg);
        }
        else{//input
        //
            
        }
    }
    else{//static
        
    }
    
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

char* new_stack_variable(){
	char* str = (char*)malloc(sizeof(char)*10);
	sprintf(str, "_st%d",VARIABLE_NO);
	STACK_VARIABLE_NO++;

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
		//declare local var
		fprintf(fptr, "\t sub rsp, %d\n", stable->running_offset);
        while(node != NULL){
            statementsCodeGen(node, fptr, stable, &scope, symbolForest,0, 0);
            node = node->sibling;
            // scope++;
        }
        fprintf(fptr, "\tjmp _exit\n");
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

void statementsCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size){
    
    if(root == NULL){
        printf("Called on NULL\n");
        return;
    }
    
    fprintf(fptr, "\n");

    if(root->label == DECLARESTMT_NODE){
        declareStmtCodeGen(root, fptr, stable, retOffset, size);
    }

    if (root->label == PRINT_STMT_NODE){
        printStmtCodeGen(root, fptr, stable, symbolForest, retOffset, size);
    }
    else if (root->label == GET_STMT_NODE){
        getStmtCodeGen(root, fptr, stable, symbolForest, retOffset, size);
    }
    else if (root->label == FORITERATIVESTMT_NODE){
        forIterStmtCodeGen(root, fptr, stable->childList[*scope], scope,symbolForest, retOffset, size);
        (*scope)++;
    }
    else if (root->label == WHILEITERATIVESTMT_NODE){
        whileIterStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest, retOffset, size);
        (*scope)++;
    }
    else if (root->label == CONDITIONALSTMT_NODE){
        conditionalStmtCodeGen(root, fptr, stable->childList[*scope], scope, symbolForest, retOffset, size);
        (*scope)++;
    }
    else if (root->label == ASSIGNOP_NODE){
        assignopCodeGen(root,fptr,stable,scope,symbolForest, retOffset, size);
    }
    else if (root->label == ASSIGNOP_ARRAY_NODE){
        assignopArrayCodeGen(root,fptr,stable,scope,symbolForest, retOffset, size);
    }
    else if (root->label == MODULEREUSESTMT_NODE){
        modulereuseStmtCodeGen(root, fptr, stable, symbolForest, retOffset,size);
    }
    // if(root->sibling != NULL){
    //     statementsCodeGen(root->sibling, fptr, stable, scope, symbolForest, reOffset);
    // }
}

void printStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int* retOffset, int size){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call printf

    symbolTableEntry *entry, *entryIndex;
    
    if(root->firstChild->label == NUM_NODE){
        fprintf(fptr, "\tmovsx rax, %d\n", root->firstChild->syntaxTreeNode->value.num);
        fprintf(fptr, "\tmov rdi,%s\n", "printI");
        fprintf(fptr, "\tmov rsi, rax\n");
        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tcall printf\n");
        return;
    }
    if(root->firstChild->label == TRUE_NODE){
        fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
        fprintf(fptr, "\tmov rsi, 0\n");
        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tcall printf\n");
        return;
    }
    else if(root->firstChild->label == FALSE_NODE){
        fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
        fprintf(fptr, "\tmov rsi, 0\n");
        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tcall printf\n");
        return;
    }

    entry = getSymbolTableEntry(stable, root->firstChild->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(entry->name, stable, retOffset, size);

    if(entry->isArray == 0){
        if(entry->type==INT){
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tmov ax, word[%s]\n", offset);
            fprintf(fptr, "\tmovsx rax, ax\n");
            fprintf(fptr, "\tmov rdi,%s\n", "printI");
            fprintf(fptr, "\tmov rsi, rax\n");
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tcall printf\n");
        }
        else if(entry->type==FLOAT){
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tmov ax, dword[%s]\n", offset);
            fprintf(fptr, "\tmovsx rax, ax\n");
            fprintf(fptr, "\tmov rdi,%s\n", "printR");
            fprintf(fptr, "\tmov rsi, rax\n");
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tcall printf\n");
        }
        else if(entry->type==BOOL){
            //mov r8w, offset
            //sub r8w, 1
            //jz l1
            //push TRUE
            //jmp l2
            //l1: push FALSE
            //l2: call printf
            char *l1 = new_label(), *l2 = new_label();  
            fprintf(fptr, "\tmov r8b, byte[%s]\n", offset);
            fprintf(fptr, "\tsub r8b, 1\n");
            fprintf(fptr, "\tjz %s\n", l1);
            fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
            fprintf(fptr, "\tjmp %s\n", l2);
            fprintf(fptr, "%s:\n", l1);
            fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
            fprintf(fptr, "%s:\n", l2);
            fprintf(fptr, "\tmov rsi, 0\n");
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tcall printf\n");
        }
    }    

    else{
        
        //Array
        if(entry->isStatic == 1){

            //Printing the whole array    
            if(root->firstChild->firstChild->sibling == NULL){
                char* label = new_label();
                char* label2 = new_label();

                fprintf(fptr, "\tmov rdi, %s\n", "printArrUtil1");
				fprintf(fptr, "\tmov rsi, 0\n");
				fprintf(fptr, "\tmov rax, 0\n");
				fprintf(fptr, "\tcall printf\n");

                fprintf(fptr, "\tmov r13, %d\n", 0);
                fprintf(fptr, "\tmov r12, rbp\n");
                //If statements for return, iplist, oplist
                fprintf(fptr, "\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
                fprintf(fptr, "%s:\n", label);
                fprintf(fptr, "\tcmp r13, %d\n",entry->endIndex-entry->startIndex+1);
                fprintf(fptr, "\tjge %s\n", label2);
                fprintf(fptr, "\tmov rax, 0\n");

                if(entry->type == INT){
                    fprintf(fptr, "\tmov ax, word[r12]\n");
                    fprintf(fptr, "\tmovsx rax, ax\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printIArr");
                    fprintf(fptr, "\tmov rsi, rax\n");
                }
                else if(entry->type == BOOL){
                    char *l1 = new_label(), *l2 = new_label();
                    fprintf(fptr, "\tmov r8b, byte[%s]\n", offset);
                    fprintf(fptr, "\tsub r8b, 1\n");
                    fprintf(fptr, "\tjz %s\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
                    fprintf(fptr, "\tjmp %s\n", l2);
                    fprintf(fptr, "%s:\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
                    fprintf(fptr, "%s:\n", l2);
                    fprintf(fptr, "\tmov rsi, 0\n");
                }
                else if(entry->type == FLOAT){
                    fprintf(fptr, "\tmov ax, dword[r12]\n");
                    fprintf(fptr, "\tmovsx rax, ax\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printRArr");
                    fprintf(fptr, "\tmov rsi, rax\n");
                }

                fprintf(fptr, "\tmov rax, 0\n");
                fprintf(fptr, "\tcall printf\n");
                fprintf(fptr, "\tsub r12, %d\n", DATA_TYPE_SIZES[entry->type]);
                fprintf(fptr, "\tadd r13, 1\n");
                fprintf(fptr, "\tjmp %s\n", label);
                fprintf(fptr, "%s:\n", label2);

                fprintf(fptr, "\tmov rdi, %s\n", "printArrUtil2");
				fprintf(fptr, "\tmov rsi, 0\n");
				fprintf(fptr, "\tmov rax, 0\n");
				fprintf(fptr, "\tcall printf\n");
            }

            else{

                fprintf(fptr,"\tmov r12, rbp\n");
                fprintf(fptr,"\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
                fprintf(fptr, "\tmov rax, 0\n");
                
                //Array with static index
                if(root->firstChild->firstChild->sibling->label == NUM_NODE){
                    fprintf(fptr, "\tsub r12, %d\n", (DATA_TYPE_SIZES[INT])*(root->firstChild->firstChild->sibling->syntaxTreeNode->value.num - entry->startIndex));
                }

                //Array with dynamic index
                else{
                    
                    entryIndex = getSymbolTableEntry(stable, root->firstChild->firstChild->sibling->syntaxTreeNode->lexeme);
                    char* offsetIndex = getReturnOffset(root->firstChild->firstChild->sibling->syntaxTreeNode->lexeme, stable,retOffset,size);

                    fprintf(fptr, "\tmov r13, 0\n");
                    fprintf(fptr, "\tmov r13w, word[%s]\n", offsetIndex);
                    // fprintf(fptr, "\tsub r13w, %d\n", entryIndex->offset);
                    //cmp index
                    // fprintf(fptr,"\tmov r15w, word[r13]\n");
                    char* label = new_label();
                    char* label1 = new_label();
                    fprintf(fptr, "\tcmp r13, %d\n", entry->endIndex);
                    fprintf(fptr, "\tjg %s\n", label1);
                    fprintf(fptr, "\tcmp r13, %d\n", entry->startIndex);
                    fprintf(fptr, "\tjge %s\n", label);
                    
                    fprintf(fptr ,"%s: \n", label1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printError");
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                    fprintf(fptr, "\tjmp _exit\n");
                    fprintf(fptr ,"%s: \n", label);
                    

                    fprintf(fptr, "\tsub r13, %d\n", entry->startIndex);
                    fprintf(fptr, "\timul r13, %d\n", DATA_TYPE_SIZES[entry->type]);
                    fprintf(fptr, "\tsub r12, r13\n");
                }

                if(entry->type == INT){
                    fprintf(fptr, "\tmov ax, word[r12]\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printI");
                    fprintf(fptr, "\tmov rsi, rax\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                }
                else{
                    char *l1 = new_label(), *l2 = new_label();  
                    fprintf(fptr, "\tmov r8b, byte[r12]\n");
                    fprintf(fptr, "\tsub r8b, 1\n");
                    fprintf(fptr, "\tjz %s\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
                    fprintf(fptr, "\tjmp %s\n", l2);
                    fprintf(fptr, "%s:\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
                    fprintf(fptr, "%s:\n", l2);
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                }
            }
        }
        else{//dynamic 

            //Printing the whole array    
            if(root->firstChild->firstChild->sibling == NULL){
                char* label = new_label();
                char* label2 = new_label();

                fprintf(fptr, "\tmov rdi, %s\n", "printArrUtil1");
				fprintf(fptr, "\tmov rsi, 0\n");
				fprintf(fptr, "\tmov rax, 0\n");
				fprintf(fptr, "\tcall printf\n");

                fprintf(fptr, "\tmov r13, %d\n", 0);
                // fprintf(fptr, "\tmov r12, rbp\n");
                // //If statements for return, iplist, oplist
                // fprintf(fptr, "\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
                getArrayElement("r15", "r12", "r13", entry, stable, fptr, retOffset, size);
                //mov tn, startind - endind
                fprintf(fptr, "\tmov r1w, word[%s]\n", getReturnOffset(entry->startIndexDyn->name, stable, retOffset, size));
                fprintf(fptr, "\tmov r2w, word[%s]\n", getReturnOffset(entry->endIndexDyn->name, stable, retOffset, size));
                fprintf(fptr, "\tsub r2w, r1w\n");
                char* var = new_variable();
                fprintf(fptr, "\tmov word [%s], r1w\n", var);
                
                fprintf(fptr, "%s:\n", label);
                fprintf(fptr, "\tmov r15w, word [%s]\n", var);
                fprintf(fptr, "\tcmp r13, r15\n");
                fprintf(fptr, "\tjge %s\n", label2);
                fprintf(fptr, "\tmov rax, 0\n");
                
                if(entry->type == INT){
                    fprintf(fptr, "\tmov ax, word[r12]\n");
                    fprintf(fptr, "\tmovsx rax, ax\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printIArr");
                    fprintf(fptr, "\tmov rsi, rax\n");
                }
                else if(entry->type == BOOL){
                    char *l1 = new_label(), *l2 = new_label();
                    fprintf(fptr, "\tmov r8b, byte[%s]\n", offset);
                    fprintf(fptr, "\tsub r8b, 1\n");
                    fprintf(fptr, "\tjz %s\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
                    fprintf(fptr, "\tjmp %s\n", l2);
                    fprintf(fptr, "%s:\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
                    fprintf(fptr, "%s:\n", l2);
                    fprintf(fptr, "\tmov rsi, 0\n");
                }
                else if(entry->type == FLOAT){
                    fprintf(fptr, "\tmov ax, dword[r12]\n");
                    fprintf(fptr, "\tmovsx rax, ax\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printRArr");
                    fprintf(fptr, "\tmov rsi, rax\n");
                }

                fprintf(fptr, "\tmov rax, 0\n");
                fprintf(fptr, "\tcall printf\n");
                fprintf(fptr, "\tsub r12, %d\n", DATA_TYPE_SIZES[entry->type]);
                fprintf(fptr, "\tadd r13, 1\n");
                fprintf(fptr, "\tjmp %s\n", label);
                fprintf(fptr, "%s:\n", label2);

                fprintf(fptr, "\tmov rdi, %s\n", "printArrUtil2");
				fprintf(fptr, "\tmov rsi, 0\n");
				fprintf(fptr, "\tmov rax, 0\n");
				fprintf(fptr, "\tcall printf\n");
            }

            else{

                // fprintf(fptr,"\tmov r12, rbp\n");
                // fprintf(fptr,"\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
                fprintf(fptr, "\tmov r13, 0\n");
                getArrayElement("r15", "r12", "r13", entry, stable, fptr, retOffset, size);
                fprintf(fptr, "\tmov rax, 0\n");
                
                //Array with static index
                if(root->firstChild->firstChild->sibling->label == NUM_NODE){
                    fprintf(fptr, "\tsub r12, %d\n", (DATA_TYPE_SIZES[INT])*(root->firstChild->firstChild->sibling->syntaxTreeNode->value.num - entry->startIndex));
                }

                //Array with dynamic index
                else{
                    
                    entryIndex = getSymbolTableEntry(stable, root->firstChild->firstChild->sibling->syntaxTreeNode->lexeme);
                    char* offsetIndex = getReturnOffset(root->firstChild->firstChild->sibling->syntaxTreeNode->lexeme, stable,retOffset,size);

                    fprintf(fptr, "\tmov r13, 0\n");
                    fprintf(fptr, "\tmov r13w, word[%s]\n", offsetIndex);
                    // fprintf(fptr, "\tsub r13w, %d\n", entryIndex->offset);
                    //cmp index
                    // fprintf(fptr,"\tmov r15w, word[r13]\n");
                    fprintf(fptr, "\tmov r1w, word[%s]\n", getReturnOffset(entry->startIndexDyn->name, stable, retOffset, size));
                    fprintf(fptr, "\tmov r2w, word[%s]\n", getReturnOffset(entry->endIndexDyn->name, stable, retOffset, size));
                
                    char* label = new_label();
                    char* label1 = new_label();
                    fprintf(fptr, "\tcmp r13, r2w\n");
                    fprintf(fptr, "\tjg %s\n", label1);
                    fprintf(fptr, "\tcmp r13, r1w\n");
                    fprintf(fptr, "\tjge %s\n", label);

                    fprintf(fptr ,"%s: \n", label1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printError");
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                    fprintf(fptr, "\tjmp _exit\n");
                    fprintf(fptr ,"%s: \n", label);

                    fprintf(fptr, "\tsub r13, r1w\n");
                    fprintf(fptr, "\timul r13, %d\n", DATA_TYPE_SIZES[entry->type]);
                    fprintf(fptr, "\tsub r12, r13\n");
                }

                if(entry->type == INT){
                    fprintf(fptr, "\tmov ax, word[r12]\n");
                    fprintf(fptr, "\tmov rdi,%s\n", "printI");
                    fprintf(fptr, "\tmov rsi, rax\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                }
                else{
                    char *l1 = new_label(), *l2 = new_label();  
                    fprintf(fptr, "\tmov r8b, byte[r12]\n");
                    fprintf(fptr, "\tsub r8b, 1\n");
                    fprintf(fptr, "\tjz %s\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printFALSE");
                    fprintf(fptr, "\tjmp %s\n", l2);
                    fprintf(fptr, "%s:\n", l1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printTRUE");
                    fprintf(fptr, "%s:\n", l2);
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                }
            }
        }
            
        
    }
}

void getStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *retOffset, int size){
    //mov r1, offset
    //push r1
    //push msg ;msg had to be declared in .data msgInt- "%d",10, 0 msgfloat- "%f",10, 0 
    //call scanf

    symbolTableEntry* entry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(entry->name, stable, retOffset,size);

	if(entry->isArray == 0){
		char *s;
		if(entry->type == INT){
			s = "printINPUTINT";
		}else{
			s = "printINPUTBOOL";
		}

		fprintf(fptr, "\tmov rdi, %s\n", s);
		fprintf(fptr, "\tmov rsi, 0\n");
		fprintf(fptr, "\txor rax, rax\n");
		fprintf(fptr, "\tcall printf\n");
	}

    if(entry->isArray == 0){

        fprintf(fptr, "\tmov rsi,\tbufferInt\n");
        fprintf(fptr, "\tmov rdi, getI\n");
        fprintf(fptr, "\tmov al, 0\n");
        fprintf(fptr, "\tcall scanf\n");  
        
        if(entry->type==INT){
            fprintf(fptr, "\tmov bx, word[bufferInt]\n");
            fprintf(fptr, "\tmov word[%s], bx\n", offset);
        }
        else if (entry->type==BOOL){
            fprintf(fptr, "\tmov r8b, byte[bufferInt]\n");
            fprintf(fptr, "\tmov byte[%s], r8b\n" ,offset);
		}
    }

    else{
        // if(entry->isStatic == 1){
        char *s;
        char* var1 = "t39";

        // int ele = entry->endIndex - entry->startIndex +1;

        if(entry->isStatic == 1)
            fprintf(fptr,"\tmov word[%s], %d\n", var1,entry->endIndex-entry->startIndex+1);
        else{
            char* leftInd = getReturnOffset(entry->startIndexDyn->name,stable, retOffset, size);
            char* rightInd = getReturnOffset(entry->endIndexDyn->name,stable, retOffset, size);
            fprintf(fptr,"\tmov r14, 0\n");
            fprintf(fptr,"\tmov r14w, word[%s]\n",rightInd);
            fprintf(fptr,"\tsub r14w, word[%s]\n",leftInd);
            fprintf(fptr,"\tadd r14w, 1\n");
            fprintf(fptr,"\tmov word[%s],r14w\n",var1);
        }

        if(entry->type == INT){
            s = "printINPUTINTARR";
        }else{
            s = "printINPUTBOOLARR";
        }

        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tmov ax, word[%s]\n", var1);
        fprintf(fptr, "\tmovsx rax, ax\n");
        fprintf(fptr, "\tmov rdi,%s\n", s);//%%daal print mein data segment mein mera save nhi ho rha
        fprintf(fptr, "\tmov rsi, rax\n");
        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tcall printf\n");

        char* label = new_label();
        char* label2 = new_label();

        fprintf(fptr, "\tmov r13, %d\n", 0);
        fprintf(fptr, "\tmov r12, rbp\n");


        // if(entry->isReturn == 0){//local
        //     fprintf(fptr, "\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);   
        // }
        // else if(entry->isReturn == 1){//return
        //     fprintf(fptr, "\tsub r12, %d\n", entry->offset-(*retOffset)+DATA_TYPE_SIZES[entry->type]);   
        // }
        // else{//inputplist
        //     fprintf(fptr, "\add r12, %d\n", 16 + entry->offset);   
        // }
        
        if(entry->isStatic == 0){
            getArrayElement("r15","r12","r13",entry,stable,fptr,retOffset,size);
        }
        else{
            fprintf(fptr,"\tmov r12, rbp\n");
            if(entry->isReturn == 0){
                fprintf(fptr,"\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
            }
            else{
                //from inputplist and static
            }
        }

        fprintf(fptr, "%s:\n", label);

        fprintf(fptr, "\tcmp r13w, word[%s]\n",var1);
        fprintf(fptr, "\tjge %s\n", label2);
        fprintf(fptr, "\tmov rsi,\tbufferInt\n");
        fprintf(fptr, "\tmov rdi, getI\n");
        fprintf(fptr, "\tmov al, 0\n");
        fprintf(fptr, "\tcall scanf\n");

        if(entry->type == INT){
            fprintf(fptr, "\tmov bx, word[bufferInt]\n");
            fprintf(fptr, "\tmov word [r12], bx\n");
        }
        else if(entry->type == BOOL){
            fprintf(fptr, "\tmov r8b, byte[bufferInt]\n");
            fprintf(fptr, "\tmov byte [r12], r8b\n");
        }

        fprintf(fptr, "\tsub r12, %d\n", DATA_TYPE_SIZES[entry->type]);
        fprintf(fptr, "\tadd r13, 1\n");
        fprintf(fptr, "\tjmp %s\n", label);
        fprintf(fptr, "%s:\n", label2);
        // }
    }
}

void forIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope,moduleHashNode* symbolForest[], int* retOffset, int size){

    
    int startRange = root->firstChild->sibling->firstChild->syntaxTreeNode->value.num;
    int endRange = root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->value.num;
    symbolTableEntry* id = getSymbolTableEntry(stable,root->firstChild->syntaxTreeNode->lexeme);

    //mov r1, startRange
    fprintf(fptr, "\tmov r8w, %d\n", startRange); //register name???

    //store ID,r1
    char* offset = getReturnOffset(id->name, stable, retOffset, size);
    fprintf(fptr, "\tmov word[%s], r8w\n", offset);
    
    //l1: cmp r1, endindex
    char* label = new_label();
    fprintf(fptr, "%s: \n\tcmp r8w, %d\n", label, endRange);
    
    //jg l2 (next statement)
    char* labelNext = new_label();
    fprintf(fptr, "\tjg %s\n", labelNext);

    //for loop statements
    //TODO:call recursive function
    ASTnode* temp = root->firstChild->sibling->sibling;
    while(temp!= NULL){
        statementsCodeGen(temp, fptr, stable, scope, symbolForest, retOffset,size);
        temp = temp->sibling;
    }

    //load r1, ID
    fprintf(fptr, "\tmov r8w, word[%s]\n", offset);

    //inc r1
    fprintf(fptr, "\tinc r8w\n");

    //store ID, r1
    fprintf(fptr, "\tmov word[%s], r8w\n", offset);

    //jump l1
    fprintf(fptr, "\tjmp %s\n", label);
   
    //l2: next statement
    fprintf(fptr, "\n%s:\n", labelNext);

    return;
}

void whileIterStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size){

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
    genExpr(root->firstChild, fptr, 0, stable, retOffset,size);

    fprintf(fptr, "\tmov\tr8w, [t%d]\n",0);

    // cmp
    fprintf(fptr, "\tcmp\tr8w, %d\n",1);
    
    //jump if less than
    fprintf(fptr, "\tjl\t%s\n", label2);

    // call statements
    ASTnode* temp = root->firstChild->sibling->sibling;
    while(temp!= NULL){
        statementsCodeGen(temp, fptr, stable, scope, symbolForest, retOffset,size);
        temp = temp->sibling;
    }
    //jmp label1
    fprintf(fptr, "\tjmp\t%s\n", label1);

    //Label2
    fprintf(fptr, "\n%s:\n", label2);

    return;
}

void assignopCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size){

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme);
    char* offset = getReturnOffset(symEntry->name, stable, retOffset,size);
    genExpr(root->firstChild->sibling, fptr, 0, stable, retOffset,size);

    if(symEntry->isArray == 0){

        if(symEntry->type==INT){
            // printf("****%d*****\n",size+symEntry->offset); 
            fprintf(fptr, "\tmov\tbx, [t%d]\n",0);
            fprintf(fptr, "\tmov word[%s], bx\n" ,offset);
        }
        else{
            fprintf(fptr, "\tmov\tr8b, [t%d]\n",0);
            fprintf(fptr, "\tmov byte[%s], r8b\n" ,offset);
        }
    }

    else{
        char* label = new_label();
        char* label2 = new_label();
        symbolTableEntry* symEntryExpr = getSymbolTableEntry(stable, root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme);

        // fprintf(fptr,"\tmov r12, rbp\n");
        // fprintf(fptr,"\tmov r13, rbp\n");
        // fprintf(fptr,"\tsub r12, %d\n", symEntry->offset+size+DATA_TYPE_SIZES[symEntry->type]);
        // fprintf(fptr,"\tsub r13, %d\n", symEntryExpr->offset+size+DATA_TYPE_SIZES[symEntryExpr->type]);
        // fprintf(fptr,"\tmov r14, 0\n");
        char* var = "t40";

        fprintf(fptr,"\tmov r9, 0\n");

        if(symEntry->isStatic == 1){
            fprintf(fptr,"\tmov word[%s], %d\n", var,symEntry->endIndex-symEntry->startIndex+1);
        }
        else{
            char* leftInd = getReturnOffset(symEntry->startIndexDyn->name,stable, retOffset, size);
            char* rightInd = getReturnOffset(symEntry->endIndexDyn->name,stable, retOffset, size);
            fprintf(fptr,"\tmov r14, 0\n");
            fprintf(fptr,"\tmov r14w, word[%s]\n",rightInd);
            fprintf(fptr,"\tsub r14w, word[%s]\n",leftInd);
            fprintf(fptr,"\tadd r14w, 1\n");
            fprintf(fptr,"\tmov word[%s],r14w\n",var);
        }

        fprintf(fptr,"%s:\n", label);
        fprintf(fptr,"\tcmp r14, word[%s]\n",var);
        fprintf(fptr, "\tjge %s\n", label2);
        
        if(symEntry->isStatic == 0){
            getArrayElement("r15","r12","r9",symEntry,stable,fptr,retOffset,size);
        }
        else{
            fprintf(fptr,"\tmov r12, rbp\n");
            if(symEntry->isReturn == 0){
                fprintf(fptr,"\tsub r12, %d\n", symEntry->offset+size+DATA_TYPE_SIZES[symEntry->type]);
            }
            else{
                //from inputplist and static
            }
        }
        if(symEntryExpr->isStatic == 0){
            getArrayElement("r15","r13","r9",symEntryExpr,stable,fptr,retOffset,size);
        }
        else{
            fprintf(fptr,"\tmov r13, rbp\n");
            if(symEntry->isReturn == 0){
                fprintf(fptr,"\tsub r13, %d\n", symEntryExpr->offset+size+DATA_TYPE_SIZES[symEntryExpr->type]);
            }
            else{
                //from inputplist and static
            }
        }
        
        if(symEntry->type == INT){
            fprintf(fptr,"\tmov r8w, word[r13]\n");
            fprintf(fptr,"\tmov word[r12], r8w\n");
        }
        else if (symEntry->type == FLOAT){
            fprintf(fptr,"\tmov r8d, dword[r13]\n");
            fprintf(fptr,"\tmov dword[r12], r8d\n");
        }
        else{
            fprintf(fptr,"\tmov r8b, byte[r13]\n");
            fprintf(fptr,"\tmov byte[r12], r8b\n");
        }
        
        fprintf(fptr,"\tsub r12, %d\n", DATA_TYPE_SIZES[symEntry->type]);
        fprintf(fptr,"\tsub r13, %d\n", DATA_TYPE_SIZES[symEntryExpr->type]);
        fprintf(fptr,"\tadd r9, 1\n");
        fprintf(fptr, "\tjmp %s\n", label);
        fprintf(fptr,"%s:\n", label2);
    }
}

void assignopArrayCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size){

    symbolTableEntry* symEntry = getSymbolTableEntry(stable, root->firstChild->firstChild->syntaxTreeNode->lexeme);
    symbolTableEntry* symEntryInd = getSymbolTableEntry(stable, root->firstChild->firstChild->sibling->syntaxTreeNode->lexeme);

    genExpr(root->firstChild->sibling, fptr, 0, stable, retOffset,size);

    if(symEntry->isStatic == 1){
        char* offset = getReturnOffset(symEntry->name, stable, retOffset,size);
        fprintf(fptr,"\tmov r12, rbp\n");
        fprintf(fptr,"\tsub r12, %d\n", symEntry->offset+size+DATA_TYPE_SIZES[symEntry->type]);//for array

        if(root->firstChild->firstChild->sibling->label == NUM_NODE){//
            fprintf(fptr,"\tsub r12, %d\n", (root->firstChild->firstChild->sibling->syntaxTreeNode->value.num - symEntry->startIndex)*DATA_TYPE_SIZES[symEntry->type]); //For index
        }
        
        else{
            
            fprintf(fptr, "\tmov r13, rbp\n");

            if(symEntryInd->isReturn == 0){ //local
                fprintf(fptr, "\tsub r13, %d\n" ,size+symEntryInd->offset+symEntryInd->width);
            }
            else if(symEntryInd->isReturn == 1){ //return
                fprintf(fptr, "\tsub r13, %d\n", symEntryInd->offset - (*retOffset)+ symEntryInd->width);
            }
            else{ //inputplist
                fprintf(fptr, "\tadd r13, %d\n", 16 + symEntryInd->offset);
            }
            //cmp dynamic indexing
            fprintf(fptr, "\tmov r14, 0\n");
            fprintf(fptr,"\tmov r14w, word[r13]\n");

            char* label = new_label();
            char* label1 = new_label();
            fprintf(fptr, "\tcmp r14w, %d\n", symEntry->endIndex);
            fprintf(fptr, "\tjg %s\n", label1);
            fprintf(fptr, "\tcmp r14w, %d\n", symEntry->startIndex);
            fprintf(fptr, "\tjge %s\n", label);
            
            fprintf(fptr ,"%s: \n", label1);
            fprintf(fptr, "\tmov rdi, %s\n", "printError");
            fprintf(fptr, "\tmov rsi, 0\n");
            fprintf(fptr, "\tmov rax, 0\n");
            fprintf(fptr, "\tcall printf\n");
            fprintf(fptr, "\tjmp _exit\n");
            fprintf(fptr ,"%s: \n", label);

            fprintf(fptr, "\tsub r14, %d\n", symEntry->startIndex);
            fprintf(fptr, "\timul r14, %d\n", DATA_TYPE_SIZES[symEntry->type]);
            // fprintf(fptr, "\tsub r12, %d\n", symEntry->offset+size);
            fprintf(fptr, "\tsub r12, r14\n");
        }

        if(symEntry->type == INT){
            fprintf(fptr,"\tmov r8w, word[t0]\n");
            fprintf(fptr,"\tmov word[r12], r8w\n");
        }
        else if(symEntry->type == FLOAT){
            fprintf(fptr,"\tmov r8d, dword[t0]\n");
            fprintf(fptr,"\tmov dword[r12], r8d\n");
        }
        else{
            fprintf(fptr,"\tmov r8b, byte[t0]\n");
            fprintf(fptr,"\tmov byte[r12], r8b\n");
        }
        // fprint(fptr,"\t\n");
    }
    else{//dynamic
        //mov regi, [offset of semEntryInd]
        //mov r1, [offset of startInd]
        //sub regi, r1
        char* startIndOffset = getReturnOffset(symEntry->startIndexDyn->name, stable, retOffset, size);
        char* endIndOffset = getReturnOffset(symEntry->endIndexDyn->name, stable, retOffset, size);
        fprintf(fptr,"\tmov r12, 0\n");
        fprintf(fptr,"\tmov r13, 0\n");

        fprintf(fptr,"\tmov r13w, word[%s]\n", startIndOffset);
        if(root->firstChild->firstChild->sibling->label == NUM_NODE){//
        
            fprintf(fptr,"\tmov r12w, %d\n", root->firstChild->firstChild->sibling->syntaxTreeNode->value.num); //For index
            
        }else{//ind is variable
            char* actualIndOffset = getReturnOffset(symEntryInd->name, stable, retOffset, size);
            fprintf(fptr,"\tmov r12w, word[%s]\n", actualIndOffset);
        }

        
        char* label = new_label();
        char* label1 = new_label();
        fprintf(fptr,"\tmov r14w, word[%s]\n",startIndOffset);
        fprintf(fptr,"\tcmp r12w, r14w\n");
        fprintf(fptr, "\tjl %s\n", label1);
        fprintf(fptr,"\tmov r14w, word[%s]\n",endIndOffset);
        fprintf(fptr,"\tcmp r12w, r14w\n");
        fprintf(fptr, "\tjle %s\n", label);

        fprintf(fptr ,"%s: \n", label1);
        fprintf(fptr, "\tmov rdi, %s\n", "printError");
        fprintf(fptr, "\tmov rsi, 0\n");
        fprintf(fptr, "\tmov rax, 0\n");
        fprintf(fptr, "\tcall printf\n");
        fprintf(fptr, "\tjmp _exit\n");
        fprintf(fptr ,"%s: \n", label);


        fprintf(fptr,"\tsub r12w, r13w\n");

        getArrayElement("r10", "r11", "r12", symEntry, stable, fptr, retOffset, size);

        if(symEntry->type == INT){
            fprintf(fptr,"\tmov r8w, word[t0]\n");
            fprintf(fptr,"\tmov word[r11], r8w\n");
        }
        else if(symEntry->type == FLOAT){
            fprintf(fptr,"\tmov r8d, dword[t0]\n");
            fprintf(fptr,"\tmov dword[r11], r8d\n");
        }
        else{
            fprintf(fptr,"\tmov r8b, byte[t0]\n");
            fprintf(fptr,"\tmov byte[r11], r8b\n");
        }
    }
    
    return;
}

void conditionalStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int *scope, moduleHashNode* symbolForest[], int* retOffset, int size){
    
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
    char* offset = getReturnOffset(switchVar->name, stable, retOffset,size);

    // storeIntoReg("r8w",NULL,switchVarOffset,stable,symbolForest,fptr);
    
    while(temp){
		
        int caseValue = 0;
        if(temp->label== NUM_NODE){
            caseValue = temp->syntaxTreeNode->value.num;
        }else{
            caseValue = strcmp(temp->syntaxTreeNode->lexeme, "FALSE");
        }

        //cmp r1, val
		if(switchVar->type == INT){
			fprintf(fptr, "\tmov r8w, word[%s]\n", offset);
            fprintf(fptr, "\tcmp\tr8w,\t%d\n", caseValue);
		}
			
		else{
			fprintf(fptr, "\tmov r8b, byte[%s]\n", offset);
            fprintf(fptr, "\tcmp\tr8b,\t%d\n", caseValue);
		}

        char* label = new_label();
        //jne nextCase
        fprintf(fptr, "\tjne\t%s\n", label);

        // call statements
        ASTnode* withinCase = temp->firstChild;
        while(withinCase){
            statementsCodeGen(withinCase, fptr, stable, scope, symbolForest, retOffset, size);
            withinCase = withinCase->sibling;
        }

        //jmp nextStmt
        fprintf(fptr, "\tjne\t%s\n", labelNextStmt);
        //nextCase: loop while
        fprintf(fptr, "%s:\n", label);

        temp= temp->sibling;
    }
    //call default
    if(switchVar->type == INT){
        statementsCodeGen(root->firstChild->sibling->sibling, fptr, stable, scope, symbolForest, retOffset, size);
    }
    //nextStmt:
    fprintf(fptr, "\n%s:\n", labelNextStmt);
}

void modulereuseStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int* retOffset, int size){

	char* var = new_stack_variable();
    fprintf(fptr, "\tmov [%s], rsp\n", var);
	pushRetParameters(fptr, root->firstChild->firstChild, stable, symbolForest, retOffset, size);
    printIDList(fptr, root->firstChild->sibling->sibling->firstChild, stable, symbolForest, retOffset, size);
    fprintf(fptr, "\tcall\t%s\n", root->firstChild->sibling->syntaxTreeNode->lexeme);
	fprintf(fptr, "\tmov rsp, [%s]\n", var);
    // fprintf(fptr, "\tadd\trsp, %d\n", offset);
}

void pushRetParameters(FILE *fptr, ASTnode* node, symbolTableNode *stable, moduleHashNode* symbolForest[], int* retOffset, int size){
	//node is idlist node

    if (node == NULL){
        // fprintf(fptr, "\n; return parameters pushed\n");
        return;
    }

    pushRetParameters(fptr, node->sibling, stable, symbolForest, retOffset, size);

    // char *var = generateBssLexeme(stable, node->syntaxTreeNode->lexeme);
    symbolTableEntry* symEntry = getSymbolTableEntry(stable, node->syntaxTreeNode->lexeme);

    fprintf(fptr,"\tmov r8, rbp\n");

    if(symEntry->isReturn == 0){ //local
        fprintf(fptr, "\tsub r8, %d\n", size+symEntry->offset+symEntry->width);
        fprintf(fptr, "\tpush r8\n");
    }
    else if(symEntry->isReturn == 1){ //return
        fprintf(fptr, "\tsub r8, %d\n", symEntry->offset - (*retOffset) + symEntry->width);
        fprintf(fptr, "\tpush r8\n");
    }
    else{ //inputplist
        fprintf(fptr, "\tadd r8, %d\n", 16+symEntry->offset);
        fprintf(fptr, "\tpush r8\n");
    }
		// pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
		// *offset += BOOL;
    // }
	// else if(symEntry->type == INT){
	// 	if(symEntry->isReturn == 0){ //local
	// 		fprintf(fptr, "\tsub r8, %d\n", size+symEntry->offset);
    //         fprintf(fptr, "\tpush r8\n");
	// 	}
	// 	else if(symEntry->isReturn == 1){ //return
	// 		fprintf(fptr, "\tsub r8, %d\n", symEntry->offset - (*retOffset));
    //         fprintf(fptr, "\tpush r8\n");
	// 	}
	// 	else{ //inputplist
	// 		fprintf(fptr, "\tadd r8, %d\n", 16+symEntry->offset);
    //         fprintf(fptr, "\tpush r8\n");
	// 	}
	// 	// pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
	// 	// *offset += INT;
	// }
	
    return;
}

void declareStmtCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, int* retOffset, int size){
    
    ASTnode* typeNode = root->firstChild;
    ASTnode* temp = typeNode->sibling;//id
    while(temp){
        symbolTableEntry *newEntry = getSymbolTableEntry(stable, temp->syntaxTreeNode->lexeme);
        
        if(newEntry->isArray==1){
            if(newEntry->isStatic==0){//dynamic
                if(newEntry->isReturn==0){//local
                    symbolTableEntry *startIndEnt = newEntry->startIndexDyn;
                    symbolTableEntry *endIndEnt = newEntry->endIndexDyn;
                    char* startOffset = getReturnOffset(startIndEnt->name,stable, retOffset, size);
                    char* endOffset = getReturnOffset(endIndEnt->name,stable, retOffset, size);
                    char* arrAddLocation = getReturnOffset(newEntry->name, stable, retOffset, size);
                    fprintf(fptr,"\tmov r8, rsp\n");
                    fprintf(fptr,"\tsub r8, %d\n",DATA_TYPE_SIZES[newEntry->type]);
                    fprintf(fptr,"\tmov qword[%s], r8\n", arrAddLocation);
                    fprintf(fptr, "\tmov r8w, word[%s]\n", startOffset);
                    fprintf(fptr,"\tmov r9, 0\n");
                    fprintf(fptr, "\tmov r9w, word[%s]\n", endOffset);
                    fprintf(fptr,"\tsub r9w, r8w\n");//size of array
                    fprintf(fptr,"\timul r9w, %d\n",DATA_TYPE_SIZES[newEntry->type]);//numberofele*data_type_size
                    fprintf(fptr,"\tsub rsp, r9\n");
                }
            }
        }
        temp=temp->sibling;
    }

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

    int returnOffset = 0; //Size of inputplist
    fprintf(fptr, "%s:\n", root->firstChild->syntaxTreeNode->lexeme);
    fprintf(fptr, "\tpush rbp\n");
    fprintf(fptr, "\tmov rbp, rsp\n");
    // fprintf(fptr, "\tpush rbp\n");
    // fprintf(fptr, "\tmov rbx, rsp\n");
    // fprintf(fptr,"\tmov\t_%s_ret, %s", root->firstChild->syntaxTreeNode->lexeme, "rsp");
    int size = returnCodeGen(root->firstChild->sibling->sibling, fptr, stable, symbolForest, scope, &returnOffset);//size of return parameters
    // fprintf(fptr,"\tmov rbx, %s", "rsp");

	//declare local variables
	fprintf(fptr, "\t sub rsp, %d\n", stable->childList[0]->running_offset);

    ASTnode* temp = root->firstChild->sibling->sibling->sibling->firstChild;
    while(temp != NULL){
        if(stable->key == "driverFunctionNode")
            statementsCodeGen(temp, fptr, stable, scope, symbolForest, &returnOffset, size);
        else{
            statementsCodeGen(temp, fptr, stable->childList[0], scope, symbolForest, &returnOffset, size);            
        }
        temp = temp->sibling;
    }

	//for assigning return parameters back
	symbolTableEntry* symEntry;

	temp = root->firstChild->sibling->sibling->firstChild;
	int i = 16+returnOffset;
	//mov r12, [rbp+16+retoffset+(8*i)];
	//mov [r12], r1para
	while(temp != NULL){

		symEntry = getSymbolTableEntry(stable, temp->firstChild->syntaxTreeNode->lexeme);
		fprintf(fptr,"\tmov r12, [rbp+%d]\n", i);//callee function adress

		if(temp->label == INTEGER_NODE){
			fprintf(fptr,"\tmov r8w, word[rbp-%d]\n", symEntry->offset - returnOffset + symEntry->width);
			fprintf(fptr, "\tmov word[r12], r8w\n");
		}
		else{
			fprintf(fptr,"\tmov r8b, byte[rbp-%d]\n", symEntry->offset - returnOffset + symEntry->width);
			fprintf(fptr, "\tmov byte[r12], r8b\n");
		}

		temp = temp->sibling;
		i += 8;
	}

    fprintf(fptr, "\tleave\n");
    fprintf(fptr, "\tret\n");

    return; 
}

int returnCodeGen(ASTnode* root, FILE *fptr, symbolTableNode* stable, moduleHashNode* symbolForest[], int *scope, int *retOffset){

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
    return size;
}
//To print in reverse order
void printIDList(FILE *fptr, ASTnode* node, symbolTableNode *stable, moduleHashNode* symbolForest[], int* retOffset, int size){
	//node is idlistNode

    if (node == NULL){
        return;
    }

    printIDList(fptr, node->sibling, stable, symbolForest, retOffset, size);

    // char *var = generateBssLexeme(stable, node->syntaxTreeNode->lexeme);
    symbolTableEntry* symEntry = getSymbolTableEntry(stable, node->syntaxTreeNode->lexeme);

    if(symEntry->isArray == 1){
        if(symEntry->isStatic == 1){
            //Baad me dekhenge isko
        }
        else{
            //fatt jayegi
        }
        // *offset += 5;
    }
    else{
		char* offset = getReturnOffset(symEntry->name,stable,retOffset,size);

        if(symEntry->type == BOOL){
            fprintf(fptr,"\tsub rsp, 1\n");
			fprintf(fptr,"\tmov r9b, byte[%s]\n", offset);
			fprintf(fptr,"\tmov byte[rsp], r9b\n");
            // pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
            // *offset += BOOL;
        }
        else if(symEntry->type == INT){
			fprintf(fptr,"\tpush word[%s]\n", offset);
			// pushMem(NULL,NULL,symEntry,stable,symbolForest,fptr);
            // *offset += INT;
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