//GROUP 10
//2017A7PS0179P - MAYANK JAIN
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE

#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include "codeGen.h"
#include "astDef.h"

//return type, interm_counter - t_counter variabe name where it will return
int genExpr(ASTnode *node, FILE *fptr, int interm_counter, symbolTableNode* stable, int *retOffset, int size) {	

    if (node->label == NUM_NODE) {
		fprintf(fptr, "\tmov word[t%d], %d\n", interm_counter, node->syntaxTreeNode->value.num);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return INT;
	}
	else if (node->label == RNUM_NODE) {
		fprintf(fptr, "\tmov dword[t%d], %f\n", interm_counter, node->syntaxTreeNode->value.rnum);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return FLOAT;
	}
	else if (node->label == TRUE_NODE) {
		fprintf(fptr, "\tmov byte[t%d], 1\n", interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return BOOL;
	}
	else if (node->label == FALSE_NODE) {
		fprintf(fptr, "\tmov byte [t%d], 0\n", interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
        return BOOL;
	}
	else if(node->label == ID_NODE) {

		symbolTableEntry* entry = getSymbolTableEntry(stable,node->syntaxTreeNode->lexeme);
		char* offset = getReturnOffset(entry->name, stable, retOffset, size);

		if(entry->type == INT){
			fprintf(fptr, "\tmov word  r8w, [%s]\n", offset);
			fprintf(fptr, "\tmov word [t%d], r8w\n", interm_counter);
		}
		if(entry->type == FLOAT){
			fprintf(fptr, "\tmov dword  r8d, [%s]\n", offset);
			fprintf(fptr, "\tmov dword [t%d], r8d\n", interm_counter);
		}
		if(entry->type == BOOL){
			fprintf(fptr, "\tmov byte  r8b, [%s]\n", offset);
			fprintf(fptr, "\tmov byte [t%d], r8b\n", interm_counter);
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
				fprintf(fptr, "\txor rsi, rsi\n");//check for id type
				char* offset = getReturnOffset(ind->syntaxTreeNode->lexeme, stable, retOffset, size);
				//check dynamic range
				if(entry->isStatic == 1){
					fprintf(fptr, "\tmov si, word [%s]\n", offset);
					
					char* label = new_label();
					char* label1 = new_label();
                    fprintf(fptr, "\tcmp si, %d\n", entry->endIndex);
                    fprintf(fptr, "\tjg %s\n", label1);
                    fprintf(fptr, "\tcmp si, %d\n", entry->startIndex);
                    fprintf(fptr, "\tjge %s\n", label);
                    
                    fprintf(fptr ,"%s: \n", label1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printError");
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                    fprintf(fptr, "\tjmp _exit\n");
                    fprintf(fptr ,"%s: \n", label);
				}
				else{
					char* startOff = getReturnOffset(entry->startIndexDyn->name, stable, retOffset, size);
					char* endOff = getReturnOffset(entry->endIndexDyn->name, stable, retOffset, size);

					char* label = new_label();
					char* label1 = new_label();
                    fprintf(fptr, "\tmov si, word [%s]\n", offset);
					fprintf(fptr, "\tcmp si, word[%s]\n", endOff);
                    fprintf(fptr, "\tjg %s\n", label1);
                    fprintf(fptr, "\tcmp si, word[%s]\n", startOff);
                    fprintf(fptr, "\tjge %s\n", label);
                    
                    fprintf(fptr ,"%s: \n", label1);
                    fprintf(fptr, "\tmov rdi, %s\n", "printError");
                    fprintf(fptr, "\tmov rsi, 0\n");
                    fprintf(fptr, "\tmov rax, 0\n");
                    fprintf(fptr, "\tcall printf\n");
                    fprintf(fptr, "\tjmp _exit\n");
                    fprintf(fptr ,"%s: \n", label);
				}
			}
			// If array indexed by NUM
			else{
				// Load index
                fprintf(fptr, "\txor rsi, rsi\n");
				fprintf(fptr, "\tmov si, %d\n", ind->syntaxTreeNode->value.num);
			}
			
            // Get actual index
			if(entry->isStatic == 1){
				fprintf(fptr, "\tsub rsi, %d\n", entry->startIndex);
				// fprintf(fptr, "\timul rsi, %d\n", DATA_TYPE_SIZES[entry->type]);
			}
			else{
				char* startOffArr = getReturnOffset(entry->startIndexDyn->name, stable, retOffset, size);
				fprintf(fptr, "\tsub si, word[%s]\n", startOffArr);
			}

			// fprintf(fptr, "\tmov r12, rbp\n");

			// if(entry->isStatic == 1){
			// //load the address of element
			// 	fprintf(fptr, "\tsub r12, %d\n", entry->offset+size+DATA_TYPE_SIZES[entry->type]);
			// 	fprintf(fptr, "\tsub r12, rsi\n");
			// }
			// else{
			getArrayElement("r15","r12","rsi",entry,stable,fptr,retOffset, size);
			// }
			
			if(entry->type==INT){				
				// Load array value            
				fprintf(fptr, "\tmov r8w, word[r12]\n");
				fprintf(fptr, "\tmov word [t%d], r8w\n", interm_counter);
			}
			if(entry->type==FLOAT){
				// Load array value
				fprintf(fptr, "\tmov r8d, dword[r12]\n");
				fprintf(fptr, "\tmov dword [t%d], r8d\n", interm_counter);
			}
			if(entry->type==BOOL){
				// Load array value
				fprintf(fptr, "\tmov r8b, byte[r12]\n");
				fprintf(fptr, "\tmov byte [t%d], r8b\n", interm_counter);
			}
			return entry->type;
        }
        // If normal ID
        else{
			char* offset = getReturnOffset(entry->name, stable, retOffset,size);
			if(entry->type == INT){
				fprintf(fptr, "\tmov word  r8w, [%s]\n", offset);
				fprintf(fptr, "\tmov word [t%d], r8w\n", interm_counter);
			}
			if(entry->type == FLOAT){
				fprintf(fptr, "\tmov dword  r8d, [%s]\n", offset);
				fprintf(fptr, "\tmov dword [t%d], r8d\n", interm_counter);
			}
			if(entry->type == BOOL){
				fprintf(fptr, "\tmov byte  r8b, [%s]\n", offset);
				fprintf(fptr, "\tmov byte [t%d], r8b\n", interm_counter);
			}
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return entry->type;
        }
        
    }
    else if(node->label == PLUS_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1, stable, retOffset,size);
        // If binary
        if(node->firstChild->sibling){
			if(type==INT){	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset,size);
				fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tadd r8w, [t%d]\n",interm_counter+2);
				fprintf(fptr, "\tmov word [t%d], r8w\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\tfinit\n");	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset,size);
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
				fprintf(fptr, "\tfadd\n");
				fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
        }
		// If unary
		else{
			if(type==INT){
				fprintf(fptr, "\tmov word r9w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tcmp r9w, 0\n");
				char* rand1 = new_label();
				fprintf(fptr, "\tjge positive_%s\n",rand1);
				fprintf(fptr, "\txor r8w, r8w\n");
				fprintf(fptr, "\tsub r8w, r9w\n");
				fprintf(fptr, "\tmov r9w, r8w\n");
				fprintf(fptr, "positive_%s:\n",rand1);
				fprintf(fptr, "\tmov word r9w, [t%d]\n",interm_counter);
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\tfinit\n");
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tfabs\n");
				fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
				return type;
			}
		}
    }
    else if(node->label == MINUS_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1, stable, retOffset,size);
        // If binary
        if(node->firstChild->sibling){
			if(type==INT){	
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset,size);
				fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tsub r8w, [t%d]\n",interm_counter+2);
				fprintf(fptr, "\tmov word [t%d], r8w\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
			if(type==FLOAT){	
				fprintf(fptr, "\tfinit\n");
				genExpr(node->firstChild->sibling, fptr, interm_counter+2, stable, retOffset,size);
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
				fprintf(fptr, "\tfsub\n");
				fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
				// fprintf(t_ptr, "t%d, ", interm_counter); 
				return type;
			}
        }
		// If unary
		else{
			if(type==INT){
				fprintf(fptr, "\txor r8w, r8w\n");
				fprintf(fptr, "\tmov word r9w, [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tsub r8w, r9w\n");
				fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter);
				return type;
			}
			if(type==FLOAT){
				fprintf(fptr, "\tfinit\n");
				fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
				fprintf(fptr, "\tfchs\n");
				fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
				return type;
			}
		}
    }
	else if(node->label == MUL_NODE){
        int type = genExpr(node->firstChild, fptr, interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr, interm_counter+2,stable, retOffset,size);
		if(type==INT){
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tmov word r9w, [t%d]\n",interm_counter+2);
			fprintf(fptr, "\timul r8w, r9w\n");
			fprintf(fptr, "\tmov word [t%d], r8w\n", interm_counter);
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return type;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfmul\n");
			fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
			return type;
		}
    }
	else if(node->label == DIV_NODE){
        // float div deal later
		int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr, interm_counter+2,stable, retOffset,size);
		if(type==INT){
			fprintf(fptr, "\tmov word ax, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tmov dx, 0\n");
			fprintf(fptr, "\tmov word r9w, [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tidiv r9w\n");
			fprintf(fptr, "\tmov word [t%d], ax\n", interm_counter);
			// fprintf(t_ptr, "t%d, ", interm_counter);
			return type;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfdiv\n");
			fprintf(fptr, "\tfstp dword [t%d]\n",interm_counter);
			return type;
		}
    }
	else if(node->label == AND_NODE){
        genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		fprintf(fptr, "\tmov byte r8b, [t%d]\n",interm_counter+1);
		fprintf(fptr, "\tand r8b, [t%d]\n",interm_counter+2);
		fprintf(fptr, "\tmov byte [t%d], r8b\n",interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
		return BOOL;
    }
	else if(node->label == OR_NODE){
        genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		fprintf(fptr, "\tmov byte r8b, [t%d]\n",interm_counter+1);
		fprintf(fptr, "\tor r8b, [t%d]\n",interm_counter+2);
		fprintf(fptr, "\tmov byte [t%d], r8b\n",interm_counter);
		// fprintf(t_ptr, "t%d, ", interm_counter);
		return BOOL;
    }
	else if(node->label == LT_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		if(type==INT){	
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tcmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label();
			fprintf(fptr, "\tjl true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjl true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == LE_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		if(type==INT){	
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tcmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjle true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjle true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == GT_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		if(type==INT){	
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tcmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjg true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label();
			fprintf(fptr, "\tjg true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == GE_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		if(type==INT){	
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tcmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjge true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n", interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tjge true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }
	else if(node->label == EQ_NODE){
        int type = genExpr(node->firstChild, fptr,interm_counter+1,stable, retOffset,size);
		genExpr(node->firstChild->sibling, fptr,interm_counter+2,stable, retOffset,size);
		if(type==INT){	
			fprintf(fptr, "\tmov word r8w, [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tcmp r8w, [t%d]\n",interm_counter+2);
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tje true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
		else if(type==FLOAT){
			fprintf(fptr, "\tfinit\n");
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+1);
			fprintf(fptr, "\tfld dword [t%d]\n",interm_counter+2);
			fprintf(fptr, "\tfcomi st0, st1 ;compare and set eflags\n");
			char* rand1 = new_label(); // init with rand nos
			fprintf(fptr, "\tje true%s \n",rand1);
			fprintf(fptr, "\tmov byte [t%d], 0\n",interm_counter);
			fprintf(fptr, "\tjmp exit%s\n",rand1);
			fprintf(fptr, "true%s: mov byte [t%d], 1\n",rand1,interm_counter);
			fprintf(fptr, "exit%s:\n",rand1);
			return BOOL;
		}
    }

	return 0;
}