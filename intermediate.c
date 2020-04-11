#include "intermediate.h"

int LABEL_NO = 1;
int VARIABLE_NO = 1;

char* new_label(){
	char* str = (char*)malloc(sizeof(char)*10);
	sprintf(str, "L%d", LABEL_NO);
	LABEL_NO++;

	return str;
}

tupleList* allocateTupleList(tuple* head, tuple* last, int total_tuples){

	tupleList* tu = (tupleList*)malloc(sizeof(tupleList));
	tu->head = head;
	tu->last = last;
	tu->total_tuples = total_tuples;

	return tu;
}

char* new_variable(){
	char* str = (char*)malloc(sizeof(char)*10);
	sprintf(str, "t%d",VARIABLE_NO);

	VARIABLE_NO++;

	return str;
}

void addTuple(tupleList* list, tuple* tuple){
	
	if(list->head==NULL){
		list->head = tuple;
		list->last = tuple;
	}
	else{
		list->last->next = tuple;
		list->last = tuple;
	}

	list->total_tuples += 1;
}

tuple* allocateTuple(int label, char* arg1, char* arg2, char* result, symbolTableEntry* arg1_entry, symbolTableEntry* arg2_entry, symbolTableEntry* result_entry, tuple* next){

    tuple* t = (tuple*)malloc(sizeof(tuple));
    
    t->label = label;
	strcpy(t->arg1_s, arg1);
	strcpy(t->arg2_s, arg2);
	strcpy(t->result_s,result);
	t->result_entry = result_entry;
	t->arg1_entry= arg1_entry;
	t->arg2_entry= arg2_entry;
    t->next = NULL;
    
	return t;
}

// tupleList* genIR(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode *root){
//     tupleList* list = allocateTupleList(NULL, NULL, 0);
//     genIR_recurse(symbolForest, stable, root);
// }

tupleList* genIR_recurse(moduleHashNode* symbolForest[], symbolTableNode* stable, ASTnode* root){
    
    
    if(root->firstChild!=NULL){
        genIR_recurse(symbolForest, stable, root->firstChild);
    }
    if(root->sibling != NULL){
        genIR_recurse(symbolForest, stable, root->sibling);
    }    
    return NULL;
    
}

tupleList* iostmtTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){

	tuple* temp;
	if(root->label == PRINT_STMT_NODE)
		temp = allocateTuple(PRINT, root->firstChild->syntaxTreeNode->lexeme, NULL, NULL, getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme), NULL, NULL, NULL);
	else
		temp = allocateTuple(GET_L, root->firstChild->syntaxTreeNode->lexeme, NULL, NULL, getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme), NULL, NULL, NULL);
	
	tupleList* list = allocateTupleList(temp, temp,1);
	return list;
}

tupleList* foriterstmtTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){
	
	char* lab1 = new_label();
	char* lab2 = new_label();
	char* var1 = new_variable();
	tupleList* list;
	
	tuple* tup1 = allocateTuple(FOR,root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme,root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme, var1, getSymbolTableEntry(stable, root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme), getSymbolTableEntry(stable, root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme), NULL, NULL);
	tup1->next = allocateTuple(GOTO_L, lab2, NULL, NULL, NULL, NULL, NULL, NULL);
	tup1->next->next = allocateTuple(LABEL_L, lab1, NULL, NULL, NULL, NULL,NULL,NULL);
	
	tupleList* tu = genIR_recurse(symbolForest, stable, root->firstChild);

	if(tu != NULL){
		tup1->next->next->next = tu->head;
		tu->last->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tu->last->next->next = allocateTuple(LABEL_L, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tu->last->next->next, 5+tu->total_tuples);
	}
	else{
		tup1->next->next->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tup1->next->next->next->next = allocateTuple(LABEL_L, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tup1->next->next->next->next, 5);
	}

	return list;
}

tupleList* whileiterstmtTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){

	char* lab1 = new_label();
	char* lab2 = new_label();
	tupleList* list;
	
	tuple *tup1 = allocateTuple(WHILE_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
	tupleList* tu = genIR_recurse(symbolForest, stable, root->firstChild);
	tup1->next = tu->head;
	tu->last->next = allocateTuple(GOTO_L, lab2, NULL, NULL, NULL, NULL,NULL,NULL);//when bool is false

	tupleList* tu2 = genIR_recurse(symbolForest, stable, root->firstChild->sibling);

	if(tu2 != NULL){
		tu->last->next->next = tu2->head;
		tu2->last->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tu2->last->next->next = allocateTuple(LABEL_L, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tu2->last->next->next, 4+tu->total_tuples+tu2->total_tuples);
	}
	else{
		tu->last->next->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tu->last->next->next->next = allocateTuple(LABEL_L, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tu->last->next->next->next, 4+tu->total_tuples);
	}

	return list;
}

tupleList* switchCaseTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){
	
	int total_tuples = 1;
	char* lab1 = new_label(); //for next statement
	char* lab2 = new_label(); // for default;
	char* var1 = new_variable();
	tupleList *list;
	tuple* tup1 = allocateTuple(SWITCH_L, root->firstChild->syntaxTreeNode->lexeme, NULL, var1, getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme), NULL, NULL, NULL);
	tuple* travtup = tup1;
	char *travlab = new_label();

	ASTnode* temp = root->firstChild->sibling->firstChild;

	while(temp != NULL){

		travtup->next = allocateTuple(LABEL_L, travlab, NULL, NULL, NULL, NULL, NULL, NULL);
		travtup->next->next = allocateTuple(CASE_L, temp->syntaxTreeNode->lexeme, var1, NULL, NULL, NULL, NULL, NULL);
		
		if(temp->sibling != NULL){
			travlab = new_label();
		}
		else{
			travlab = lab2;
		}
		
		travtup->next->next->next = allocateTuple(GOTO_L, travlab, NULL, NULL, NULL, NULL, NULL, NULL);
		
		travtup = travtup->next->next->next;
		tupleList* tu = genIR_recurse(symbolForest, stable, root->firstChild);

		if(tu != NULL){
			travtup->next = tu->head;
			travtup = tu->last;
		}

		travtup->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		travtup = travtup->next;
		temp = temp->sibling;
		total_tuples += tu->total_tuples+4 ;
	}

	if(root->firstChild->sibling->sibling != NULL){

		travtup->next = allocateTuple(LABEL_L, lab2, NULL, NULL, NULL, NULL, NULL, NULL);

		tupleList* tu = genIR_recurse(symbolForest, stable, root->firstChild->sibling->sibling);

		if(tu != NULL){
			travtup->next->next = tu->head;
			tu->last->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
			travtup = tu->last->next;
		}
		else{
			travtup->next->next = allocateTuple(GOTO_L, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
			travtup = travtup->next->next;
		}

		total_tuples += tu->total_tuples+1;
	}

	list = allocateTupleList(tup1, travtup, total_tuples);

	return list;
}