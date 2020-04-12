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
    
    if(root->label == ASSIGNOP_NODE){
        assignstmtIRGen(root, list, stable);
        if(root->sibling){
            return genIR_recurse(symbolForest, stable, root->sibling, list);
        }else{
            return list;
        }
    }

    if(root->firstChild!=NULL){
        genIR_recurse(symbolForest, stable, root->firstChild, list);
    }
    if(root->sibling != NULL){
        genIR_recurse(symbolForest, stable, root->sibling, list);
    }    
    return NULL;
    
}

enum op{ ASSIGNOP_VALUE_OP};
void assignstmtIRGen(ASTnode* root, tupleList* list, symbolTableNode* stable){

}

char* assignstmtIRGen_util(ASTnode* root, tupleList* list, symbolTableNode* stable){
    if(root->isLeaf==1){
        if(root->label==ID_NODE){
            return root->syntaxTreeNode->lexeme;
        }else if(root->label==NUM_NODE){
            char* t = genTempVar();
            insert_into_stable(stable->varHashTable, t, INT, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(NULL, NULL, t, NULL, NULL, getSymbolTableEntry(stable, t) ,ASSIGNOP_VALUE_OP, NULL);//value kahan dun?
            list->last->next = tup;
            list->last = tup;
            return t;
        }else if(root->label==RNUM_NODE){
            char* t = genTempVar();
            insert_into_stable(stable->varHashTable, t, FLOAT, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(NULL, NULL, t, NULL, NULL, getSymbolTableEntry(stable, t) ,ASSIGNOP_VALUE_OP, NULL);//value kahan dun?
            list->last->next = tup;
            list->last = tup;
            return t;
        }else if(root->label==TRUE_NODE){
            char* t = genTempVar();
            insert_into_stable(stable->varHashTable, t, BOOL, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(NULL, NULL, t, NULL, NULL, getSymbolTableEntry(stable, t) ,ASSIGNOP_VALUE_OP, NULL);//value kahan dun?
            list->last->next = tup;
            list->last = tup;
            return t;
        }else if(root->label==FALSE_NODE){
            char* t = genTempVar();
            insert_into_stable(stable->varHashTable, t, BOOL, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(NULL, NULL, t, NULL, NULL, getSymbolTableEntry(stable, t) ,ASSIGNOP_VALUE_OP, NULL);//value kahan dun?
            list->last->next = tup;
            list->last = tup;
            return t;
        }

    }

    if(root->label == PLUS_NODE || root->label == MINUS_NODE || root->label == MUL_NODE || root->label == DIV_NODE){
            char* l = assignstmtIRGen_util(root->firstChild, list, stable);
            char* r = assignstmtIRGen_util(root->firstChild->sibling, list, stable);
            char* t = genTempVar();
            symbolTableEntry* lEntry = getSymbolTableEntry(stable, l);
            insert_into_stable(stable->varHashTable, t, lEntry->type, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(l, r, t, lEntry, getSymbolTableEntry(stable, r), getSymbolTableEntry(stable, t), root->label, NULL);
            list->last->next = tup;
            list->last = tup;
            return t;
            
    }

    if(root->label == LE_NODE || root->label == GE_NODE || root->label == LT_NODE || root->label == GT_NODE || root->label == EQ_NODE || root->label == NE_NODE ){ // NOT NODE nahi hai kya?
            char* l = assignstmtIRGen_util(root->firstChild, list, stable);
            char* r = assignstmtIRGen_util(root->firstChild->sibling, list, stable);
            char* t = genTempVar();
            symbolTableEntry* lEntry = getSymbolTableEntry(stable, l);
            insert_into_stable(stable->varHashTable, t, BOOL, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(l, r, t, lEntry, getSymbolTableEntry(stable, r), getSymbolTableEntry(stable, t), root->label, NULL);
            list->last->next = tup;
            list->last = tup;
            return t;
            
    }

    if(root->label == AND_NODE || root->label == OR_NODE ){ // NOT NODE nahi hai kya?
            char* l = assignstmtIRGen_util(root->firstChild, list, stable);
            char* r = assignstmtIRGen_util(root->firstChild->sibling, list, stable);
            char* t = genTempVar();
            symbolTableEntry* lEntry = getSymbolTableEntry(stable, l);
            insert_into_stable(stable->varHashTable, t, BOOL, 0, 0, 0, 0, 1, 0);//offset?
            tuple* tup = allocateTuple(l, r, t, lEntry, getSymbolTableEntry(stable, r), getSymbolTableEntry(stable, t), root->label, NULL);
            list->last->next = tup;
            list->last = tup;
            return t;
            
    }

}