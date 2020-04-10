#ifndef intermediatedef_h
#define intermediatedef_h

#include "astDef.h"

typedef struct tuple{
    int label; //astNode label or operator as required
    
    char arg1_s[LEX_LENGTH];
    symbolTableEntry* arg1_entry;

    char arg2_s[LEX_LENGTH];
    symbolTableEntry* arg2_entry;
    
    char result_s[LEX_LENGTH];
    symbolTableEntry* result_entry;

    struct tuple* next;

}tuple;

typedef struct tupleList{
	tuple* head;
	tuple* last;
	int total_tuples; //unknown purpose	
}tupleList;

#endif