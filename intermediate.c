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
	str_cpy(t->result_s,result);
	t->result_entry = result_entry;
	t->arg1_entry= arg1_entry;
	t->arg2_entry= arg2_entry;
    t->next = NULL;
    
	return t;
}

tupleList* genIR(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode *root){
    tupleList* list = allocateTupleList();
    genIR_recurse(symbolForest, stable, root, list);
}

tupleList* genIR_recurse(moduleHashNode* symbolForest[], symbolTableNode* stable, ASTnode* root){
    
    
    if(root->firstChild!=NULL){
        genIR_recurse(symbolForest, stable, root->firstChild, list);
    }
    if(root->sibling != NULL){
        genIR_recurse(symbolForest, stable, root->sibling, list);
    }    
    return NULL;
    
}

tupleList* iostmtTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){

	tuple* temp;
	if(root->label == PRINT_STMT_NODE)
		temp = allocateTuple(PRINT, root->firstChild->syntaxTreeNode->lexeme, NULL, NULL, getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme), NULL, NULL, NULL);
	else
		temp = allocateTuple(GET, root->firstChild->syntaxTreeNode->lexeme, NULL, NULL, getSymbolTableEntry(stable, root->firstChild->syntaxTreeNode->lexeme), NULL, NULL, NULL);
	
	tupleList* list = allocateTupleList(temp, temp,1);
	return list;
}

tupleList* foriterstmtTuple(moduleHashNode* symbolForest[], symbolTableNode *stable, ASTnode* root){
	
	char* lab1 = new_label();
	char* lab2 = new_label();
	char* var1 = new_variable();
	tupleList* list;
	
	tuple* tup1 = allocateTuple(FOR,root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme,root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme, var1, getSymbolTableEntry(stable, root->firstChild->sibling->firstChild->syntaxTreeNode->lexeme), getSymbolTableEntry(stable, root->firstChild->sibling->firstChild->sibling->syntaxTreeNode->lexeme), NULL, NULL);
	tup1->next = allocateTuple(GOTO, lab2, NULL, NULL, NULL, NULL, NULL, NULL);
	tup1->next->next = allocateTuple(LABEL, lab1, NULL, NULL, NULL, NULL,NULL,NULL);
	
	tupleList* tu = genIR_recurse(symbolForest, stable, root->firstChild);
	tup1->next->next->next = tu->head;

	if(tu->last != NULL){
		tu->last->next = allocateTuple(GOTO, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tu->last->next->next = allocateTuple(LABEL, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tu->last->next->next, 5+tu->total_tuples);
	}
	else{
		tup1->next->next->next = allocateTuple(GOTO, lab1, NULL, NULL, NULL, NULL, NULL, NULL);
		tup1->next->next->next->next = allocateTuple(LABEL, lab2, NULL, NULL, NULL, NULL,NULL,NULL);
		list = allocateTupleList(tup1, tup1->next->next->next->next, 5);
	}

	return list;
}