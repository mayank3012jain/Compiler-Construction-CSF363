#include "intermediate.h"

void add_tuple(tupleList* list, tuple* tuple){
	
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

tuple* allocateTuple(char* arg1, char* arg2, char* result, symbolTableEntry* arg1_entry, symbolTableEntry* arg2_entry, symbolTableEntry* result_entry, int label, tuple* next){

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

tupleList* genIR_recurse(moduleHashNode* symbolForest[], symbolTableNode* stable, ASTnode* root, tupleList* list){
    
    
    if(root->firstChild!=NULL){
        genIR_recurse(symbolForest, stable, root->firstChild, list);
    }
    if(root->sibling != NULL){
        genIR_recurse(symbolForest, stable, root->sibling, list);
    }    
    return NULL;
    
}