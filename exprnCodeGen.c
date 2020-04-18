#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include "codeGen.h"
#include "astDef.h"


//return type, interm_counter - t_counter variabe name where it will return
int genExpr(ASTnode *node, FILE *fptr, int interm_counter, symbolTableNode* stable, int *retOffset) {	
	//maaaadarchooooooood mayank
	// return -1;
    if (node->label == NUM_NODE) {
		fprintf(fptr, "\t mov word[t%d], %d\n", interm_counter, node->syntaxTreeNode->value.num);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return INT;
	}
	else if (node->label == RNUM_NODE) {
		fprintf(fptr, "\t mov dword[t%d], %f\n", interm_counter, node->syntaxTreeNode->value.rnum);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return FLOAT;
	}
	else if (node->label == TRUE_NODE) {
		fprintf(fptr, "\t mov byte[t%d], 1\n", interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return BOOL;
	}
	else if (node->label == FALSE_NODE) {
		fprintf(fptr, "\t mov byte [t%d], 0\n", interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return BOOL;
	}
	else if(node->label == ID_NODE) {

		symbolTableEntry* entry = getSymbolTableEntry(stable,node->syntaxTreeNode->lexeme);
		char* offset = getReturnOffset(entry->name, stable, retOffset);

		if(entry->type == INT){
			fprintf(fptr, "\t mov word  r8w, [%s]\n", offset);
			fprintf(fptr, "\t mov word [t%d], r8w\n", interm_counter);
		}
		if(entry->type == FLOAT){
			fprintf(fptr, "\t mov dword  r8d, [%s]\n", offset);
			fprintf(fptr, "\t mov dword [t%d], r8d\n", interm_counter);
		}
		if(entry->type == BOOL){
			fprintf(fptr, "\t mov byte  r8b, [%s]\n", offset);
			fprintf(fptr, "\t mov byte [t%d], r8b\n", interm_counter);
		}
        // fprintf(t_ptr, "t%d, ", interm_counter);
        return entry->type;
    }
    else if(node->label == VARIDNUM_NODE){
		ASTnode* id = node->firstChild;
		ASTnode* ind = node->firstChild->sibling;

        symbolTableEntry* entry = getSymbolTableEntry(stable,node->firstChild->syntaxTreeNode->lexeme);
		// If array

		//TODO:
        if(ind){
            // If array indexed by variable
            if(ind->label==ID_NODE){
				// Load index
				char* offset = getReturnOffset(ind->syntaxTreeNode->lexeme, stable, retOffset);
				//check dynamic range

				fprintf(fptr, "\txor rsi, rsi\n");//check for id type
				fprintf(fptr, "\tmov si, word [%s]\n", offset);
				fprintf(fptr, "\tcmp si, %d\n", entry->endIndex);
				fprintf(fptr, "\tjg _exit\n");
				fprintf(fptr, "\tcmp si, %d\n", entry->startIndex);
				fprintf(fptr, "\tjl _exit\n");
			}
			// If array indexed by NUM
			else{
				// Load index
                fprintf(fptr, "\txor rsi, rsi\n");
				fprintf(fptr, "\tmov si, %d\n", ind->syntaxTreeNode->value.num);
			}
            // Get actual index
            fprintf(fptr, "\tsub rsi, %d\n", entry->startIndex);
			fprintf(fptr, "\timul rsi, %d\n", DATA_TYPE_SIZES[entry->type]);

			//load the address of element
			fprintf(fptr, "\tmov r12, rbp\n");//hardcoded for now
        	fprintf(fptr, "\tsub r12, %d\n", entry->offset);
        	fprintf(fptr, "\tsub r12, rsi\n");
			
			if(entry->type==INT){				
				// Load array value            
				fprintf(fptr, "\t mov r8w, word[r12]\n");
				fprintf(fptr, "\t mov word [t%d], r8w\n", interm_counter);
			}
			if(entry->type==FLOAT){
				// Load array value
				fprintf(fptr, "\t mov r8d, dword[r12]\n");
				fprintf(fptr, "\t mov dword [t%d], r8d\n", interm_counter);
			}
			if(entry->type==BOOL){
				// Load array value
				fprintf(fptr, "\t mov r8b, byte[r12]\n");
				fprintf(fptr, "\t mov byte [t%d], r8b\n", interm_counter);
			}
			return entry->type;
        }
        // If normal ID
        else{
			char* offset = getReturnOffset(entry->name, stable, retOffset);
			if(entry->type == INT){
				fprintf(fptr, "\t mov word  r8w, [%s]\n", offset);
				fprintf(fptr, "\t mov word [t%d], r8w\n", interm_counter);
			}
			if(entry->type == FLOAT){
				fprintf(fptr, "\t mov dword  r8d, [%s]\n", offset);
				fprintf(fptr, "\t mov dword [t%d], r8d\n", interm_counter);
			}
			if(entry->type == BOOL){
				fprintf(fptr, "\t mov byte  r8b, [%s]\n", offset);
				fprintf(fptr, "\t mov byte [t%d], r8b\n", interm_counter);
			}
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return entry->type;
        }
        
    }
    else if(node->label == PLUS_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1, stable, retOffset);
        // If binary
        if(node->firstChild->sibling){
			if(type==INT){	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset);
				fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t add r8w, [t%d]\n",interm_counter+2);
				fprintf(fptr, "\t mov word [t%d], r8w\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\t finit\n");	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset);
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
				fprintf(fptr, "\t fadd\n");
				fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
        }
		// If unary
		else{
			if(type==INT){
				fprintf(fptr, "\t mov word r9w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t cmp r9w, 0\n");
				char* rand1 = new_label();
				fprintf(fptr, "\t jge positive_%s\n",rand1);
				fprintf(fptr, "\t xor r8w, r8w\n");
				fprintf(fptr, "\t sub r8w, r9w\n");
				fprintf(fptr, "\t mov r9w, r8w\n");
				fprintf(fptr, "positive_%s:\n",rand1);
				fprintf(fptr, "\t mov word r9w, [t%d]\n",interm_counter);
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\t finit\n");
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t fabs\n");
				fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
				return type;
			}
		}
    }
    else if(node->label == MINUS_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1, stable, retOffset);
        // If binary
        if(node->firstChild->sibling){
			if(type==INT){	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset);
				fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t sub r8w, [t%d]\n",interm_counter+2);
				fprintf(fptr, "\t mov word [t%d], r8w\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
			if(type==FLOAT){	
				fprintf(fptr, "\t finit\n");
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset);
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
				fprintf(fptr, "\t fsub\n");
				fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
        }
		// If unary
		else{
			if(type==INT){
				fprintf(fptr, "\t xor r8w, r8w\n");
				fprintf(fptr, "\t mov word r9w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t sub r8w, r9w\n");
				fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter);
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\t finit\n");
				fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\t fchs\n");
				fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
				return type;
			}
		}
    }
	else if(node->label == MUL_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr, interm_counter+2,stable, retOffset);
		if(type==INT){
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t mov word r9w, [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t imul r8w, r9w\n");
			fprintf(fptr, "\t mov word [t%d], r8w\n", interm_counter);
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return type;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fmul\n");
			fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
			return type;
		}
    }
	else if(node->label == DIV_NODE){
        // float div deal later
		int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr, interm_counter+2,stable, retOffset);
		if(type==INT){
			fprintf(fptr, "\t mov word ax, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t mov dx, 0\n");
			fprintf(fptr, "\t mov word r9w, [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t idiv r9w\n");
			fprintf(fptr, "\t mov word [t%d], ax\n", interm_counter);
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return type;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fdiv\n");
			fprintf(fptr, "\t fstp dword [t%d]\n",interm_counter);
			return type;
		}
    }
	else if(node->label == AND_NODE){
        genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		fprintf(fptr, "\t mov byte r8b, [t%d]\n",interm_counter+1);
		fprintf(fptr, "\t and r8b, [t%d]\n",interm_counter+2);
		fprintf(fptr, "\t mov byte [t%d], r8b\n",interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
		return BOOL;
    }
	else if(node->label == OR_NODE){
        genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		fprintf(fptr, "\t mov byte r8b, [t%d]\n",interm_counter+1);
		fprintf(fptr, "\t or r8b, [t%d]\n",interm_counter+2);
		fprintf(fptr, "\t mov byte [t%d], r8b\n",interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
		return BOOL;
    }
	else if(node->label == LT_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		if(type==INT){	
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t cmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label();
			fprintf(fptr, "\t jl true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jl true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == LE_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		if(type==INT){	
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t cmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jle true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jle true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == GT_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		if(type==INT){	
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t cmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jg true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label();
			fprintf(fptr, "\t jg true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == GE_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		if(type==INT){	
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t cmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jge true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t jge true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == EQ_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset);
		if(type==INT){	
			fprintf(fptr, "\t mov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t cmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t je true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\t finit\n");
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\t fld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\t fcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\t je true%s \n",rand1);
			fprintf(fptr, "\t mov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\t jmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
}