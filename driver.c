//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "codeGen.h"
#include <time.h>
#include<stdio.h>
#include<stdlib.h>

hashNode* symbolTable[SIZELOOKTBL];
hashNode* lookupTable[SIZELOOKTBL];

char* keywords[] = {"integer","real","boolean","of","array","start","end","declare","module","driver","program","record","tagged","union","get_value","print","use","with","parameters","true","false","takes","input","returns","AND","OR","for","in","switch","case","break","default","while"};
char* termString[] = {"EPSILON","DOLLAR","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","DRIVERDEF","ENDDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","RNUM","NUM","ERROR","COUNTT"};
char* NtermString[] = {"program","moduledeclarations","moduledeclaration","othermodules","drivermodule","module","ret","inputplist","n1","outputplist","n2","datatype","rangearrays","type","moduledef","statements","statement","iostmt","boolconstt","varidnum","var","whichid","simplestmt","assignstmt","whichstmt","lvalueidstmt","lvaluearrstmt","ind","modulereusestmt","optional","idlist","n3","expression", "u","newnt","arithmeticorbooleanexpr","n7","anyterm","n8","arithmeticexpr","n4","term","n5","factor","op1","op2","logicalop","relationalop","declarestmt","conditionalstmt","casestmts","n9","value","dflt","iterativestmt","range","countnt"};
char* nodeNameString[] = {"BOOLEAN_NODE","REAL_NODE","INTEGER_NODE","INPUTPLIST_HEADER_NODE","RET_HEADER_NODE","MODULEDEF_HEADER_NODE","MODULEDEC_HEADER_NODE","OTHERMOD_HEADER_NODE","DRIVER_MOD_NODE","TRUE_NODE","FALSE_NODE","NUM_NODE","RNUM_NODE","ID_NODE","PLUS_NODE","MINUS_NODE","MUL_NODE","DIV_NODE","AND_NODE","OR_NODE","LT_NODE","LE_NODE","GT_NODE","GE_NODE","EQ_NODE","NE_NODE","PROGRAM_NODE","MODULE_NODE","DATATYPE_ARRAY_NODE","RANGEARRAYS_NODE","MODULEREUSESTMT_NODE","GET_STMT_NODE","PRINT_STMT_NODE","VARIDNUM_NODE","ASSIGNOP_NODE","ASSIGNOP_ARRAY_NODE","ID_ARRAY_NODE","U_NODE","DECLARESTMT_NODE","CONDITIONALSTMT_NODE","FORITERATIVESTMT_NODE","WHILEITERATIVESTMT_NODE","RANGE_NODE","RELATIONALOP_NODE","LOGICALOP_NODE","IDLIST_NODE","OPTIONAL_RETURN_NODE","INPUTPLIST_NODE","CASESTMTS_HEADER_NODE"};
int DATA_TYPE_SIZES[] = {1, 4, 2};
int ASTNODES = 0;
int PARSETREENODES = 0;

int main(int argc, char* argv[]){

	int cases= 1;

	FILE *fin = fopen(argv[1], "r");

    if (fin == NULL){
        printf("Error opening input file\n");
        exit(0);
    }
	FILE *fout = fopen(argv[2],"w");
    if (fout == NULL){
        printf("Error opening output file\n");
        exit(0);
    }
    char read_buffer[BUFFER_SIZE];
    char read_char;
    int read_ptr = 0;

    int grammarIndex[TABLESIZE];
    FIRST first;
    FOLLOW follow;
    GRAMMAR gr;
    PARSE_TABLE pt;
    ptree_node* syntaxTree;
    ASTnode* ast;
    moduleHashNode* symbolForest[MAX_MODULES];

    while(read_ptr<BUFFER_SIZE-1 && (read_char = fgetc(fin)) != EOF){
        read_buffer[read_ptr] = read_char;
        read_ptr++;
    }

    if(read_ptr<BUFFER_SIZE-1){
        read_buffer[read_ptr] = EOF;
    }else{
        read_buffer[read_ptr] = '\0';
    }
    // printf("%s         ..........  ", read_buffer);

    populateLookupTable();
    populateSymbolTable();
    create_grammar(gr, grammarIndex, "grammar.txt");
    populateFirst(first,grammarIndex, gr);
    allFollow(first, grammarIndex, gr, follow);
    populate_parse_table(gr,first, follow, pt);
    syntaxTree = callBoth(gr,pt, read_buffer, first, follow, fin);
    ast = populateAST(syntaxTree, NULL, NULL);
    traverse_ast(ast, symbolForest);

    printf("\nALL MODULES WORKING CORRECTLY ON ALL GIVEN TESTCASES\n");
    printf("\t1.Grammar Table constructed from Grammar.txt\n");
    printf("\t2.First and Follow sets are automated from grammar table\n");
    printf("\t3.Parse Table generated from First and Follow sets\n");
    printf("\t4.Both Lexical and Syntax Analysis modules implemented\n");
    printf("\t5.Error Recovery also implemented\n\n");

	while(cases != 0){

        printf("Enter a case [0: To exit], [1: To print without comments], [2: Call Lexer], [3: Call Both Lexer and Parser], [4: Print time to parse]:  ");
		scanf("%d",&cases);

		switch(cases){

			case 1: 
                    lexerCall(read_buffer, fin);
					break;

			case 2: 
                    printTree(syntaxTree);
					break;

			case 3: 
                    printAST(ast);
				    break;

            case 4: 
                    printf("Parse Tree Number of Nodes = %d\n", PARSETREENODES);
                    printf("Allocated Memory = %lu bytes\n", PARSETREENODES*sizeof(ptree_node));
                    printf("AST Number of Nodes = %d\n", ASTNODES);
                    printf("Allocated Memory = %lu bytes\n", ASTNODES*sizeof(ASTnode));
                    printf("Compression ratio = %lf\n",(((double)PARSETREENODES*sizeof(ptree_node)-ASTNODES*sizeof(ASTnode)) * 100)/(PARSETREENODES*sizeof(ptree_node)));
                    break;
			
            case 5:
                    printSymbolForest(symbolForest);
				    break;

            case 6:
                
                break;

            case 7:

                break;

            case 8: 
                callTime(gr, pt, read_buffer, first, follow, fin);
				break;

            case 9:
                    initializeCodeGen(ast,fout,symbolForest);
                break;


            default: exit(0);//return 0;
		}
	}
    fclose(fin);
    fclose(fout);

}


void callComments(char read_buffer[]){

    int read_ptr = 0;
    int flag = 0;

    while(read_buffer[read_ptr]!= '\0'){

        if(read_buffer[read_ptr] == '*' && read_buffer[read_ptr+1] == '*'){

            read_ptr+=2;
            flag = 1;
            while(read_buffer[read_ptr] != '\0' && read_buffer[read_ptr+1] != '\0' && !(read_buffer[read_ptr] == '*' && read_buffer[read_ptr+1] == '*')){
                if(read_buffer[read_ptr] == '\n'){
                    flag= 1;
                    printf("\n");
                }
                read_ptr++;
            }
            if(read_buffer[read_ptr] != '\0' && read_buffer[read_ptr+1] != '\0' && read_buffer[read_ptr] == '*' && read_buffer[read_ptr+1] == '*'){
                read_ptr += 2;
                flag = 0;
                // printf("....%c",read_buffer[read_ptr]);
            }
        }
        if(flag == 0&& read_buffer[read_ptr] != EOF){
            printf("%c",read_buffer[read_ptr]);}
        read_ptr++;
    }
}

void lexerCall(char read_buffer[], FILE* fin){

    int LINE_NUMBER = 1;
    int line = 1;
    int read_ptr = 0;

    lex* tokenReturn = malloc(sizeof(lex));
    //printf("line no\t\tlexeme\t\ttoken name\n");

    while(1){
        getNextToken(read_buffer, &read_ptr, &LINE_NUMBER, tokenReturn, fin);
        printf("%d\t%s\t%s\n",tokenReturn->line,tokenReturn->lexeme,termString[tokenReturn->token]);
        if (tokenReturn->token == DOLLAR)
            break;
    }
}

ptree_node* callBoth(GRAMMAR gr, PARSE_TABLE pt, char* read_buffer, FIRST first, FOLLOW follow, FILE* fin){

    return make_parse_tree(gr, pt,read_buffer, first, follow, fin);
}

void callTime(GRAMMAR gr, PARSE_TABLE pt, char* read_buffer, FIRST first, FOLLOW follow, FILE* fin){

    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    start_time = clock();

    callBoth(gr, pt, read_buffer, first, follow, fin);

    end_time = clock();

    total_CPU_time  =  (double) (end_time - start_time);
    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

    printf("\n\nTime Taken in seconds: %f\tin ticks: %f\n", total_CPU_time_in_seconds, total_CPU_time);
 // Print both total_CPU_time and total_CPU_time_in_seconds 
}

void activationSizeAll(moduleHashNode* symbolForest[]){
    for(int i=0; i<MAX_MODULES; i++){
        moduleHashNode* temp = symbolForest[i];
        while(temp != NULL){
            int ans = activationSizeUtil(temp->tablePtr);
            if(strcmp(temp->key, "driverFunctionNode")!=0){
                ans += temp->tablePtr->running_offset;
            }
            activationSizePrint(temp->key, ans);
            temp = temp->next;
        }
    }
    printf("\n");
}

int activationSizeUtil(symbolTableNode* stable){
    symbolTableNode* temp;
    int i=0;
    for(i=0; i<MAX_SCOPES; i++){
        if(stable->childList[i] == NULL){
            break;
        }
    }
    if(i==0){
        return stable->running_offset;
    }
    return activationSizeUtil(stable->childList[i-1]);
}

void activationSizePrint(char* name, int size){
    printf("\nModuleName: %23s , Size: %d", name, size);
}