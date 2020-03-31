//GROUP 10
//2017A7PS0143P - UJJWAL GANDHI
//2017A7PS0157P - ADITYA MITHAL
//2017A7PS0101P - ATMADEEP BANERJEE
//2017A7PS0179P - MAYANK JAIN

#include "lexer.h"
#include "parser.h"

gnode* createNode(char* str){
    gnode *node = (gnode*)malloc(sizeof(gnode));
    node->term = getEnumVal(str);
    node->next = NULL;
    node->tag = (str[0] >= 65 && str[0] <= 90) ? 0 : 1; 
    return node;
}

void create_grammar(GRAMMAR gr, int grammarIndex[], char *filename){
    int current = 0;
    char rule[200];
    gnode *temp;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL){
        printf("ERROR WHILE OPENING FILE\n");
        return;
    }

    while (fgets(rule, MAXRULESIZE, fp) != NULL){
        char* token = strtok(rule, " ");

        gr[current] = createNode(token);
        temp = gr[current];

        //To populate grammarIndex array
        if (current == 0 || gr[current]->term != gr[current-1]->term)
            grammarIndex[gr[current]->term] = current;

        while(token != NULL){
            token = strtok(NULL, " \n");
            if(token == NULL)
                break;

            temp->next = createNode(token);
            temp = temp->next;
        }
        
        temp->next = NULL; //The grammar rule gets finished
        current++; //Go to next rule
    }

    if (current<TABLESIZE-1)
        gr[current] = NULL;

    fclose(fp);
}

long long unsigned int getFirst(int nt, FIRST first, int grammarIndex[], GRAMMAR gr){

    // printf("Enter %s NT with %d index\n", NtermString[nt], grammarIndex[nt]);
    if (first[nt].val != 0)
        return first[nt].val;
    
    int indx = grammarIndex[nt];
    while(gr[indx]!=NULL && gr[indx]->term == nt){
        // printf("Yes1 %s %d %\n",NtermString[nt],  indx);
        int nullFlag = 1;//, removeFlag = 0;
        gnode *rhs = gr[indx]->next;

        if (rhs->tag == 0){
            first[nt].val |= ((__uint128_t)1 << rhs->term);
            nullFlag = 0;
        }

        else{
            uint64_t nextFirst = getFirst(rhs->term, first, grammarIndex, gr);
            // printfirst(rhs->term, nextFirst, termString, NtermString);
            // printf("%llu\n", nextFirst);
            if (nextFirst % 2 != 0){
                nextFirst -= 1;
                // removeFlag = 1;
            }
            else{
                nullFlag = 0;
                // printf("Nullflag = 0\n");
            }
            
            first[nt].val |= nextFirst; 
            
            while (rhs->next != NULL && nullFlag == 1){
                // printf("      YES2 with %s nt, %d index\n", NtermString[nt], grammarIndex[nt]);
                rhs = rhs->next;

                if (rhs->tag == 0){
                    first[nt].val |= ((__uint128_t)1 << rhs->term);
                    nullFlag = 0;
                    break;
                }
                else if (rhs->term != gr[indx]->term){
                    // printf("Calling %d with %d\n", gr[indx]->term, rhs->term);
                    nextFirst = getFirst(rhs->term, first, grammarIndex, gr);
                    // printfirst(rhs->term, nextFirst, termString, NtermString);
                    // printf("%llu\n", nextFirst);
                    if (nextFirst % 2 != 0){
                        nextFirst -= 1;
                        // removeFlag = 1;
                    }
                    else
                        nullFlag = 0;
                    
                    first[nt].val |= nextFirst;
                }
            }
        }

        if (nullFlag == 1)
            first[nt].val |= 1;

        indx++;
    }

    // printf("  Exit %s with %d index\n", NtermString[nt], grammarIndex[nt]);
    return first[nt].val;
}

void populateFirst(FIRST first, int grammarIndex[], GRAMMAR gr){

    for (int i=0; i<countnt; i++){
        first[i].val = 0;
    }

    //For all non-terminals 
    for (int i=0; i<countnt; i++){
        getFirst(i, first, grammarIndex, gr);
       // printfirst(i, z, termString, NtermString);
    }
}

void printfirst(int i, unsigned long long int z,char* termString[], char* NtermString[]){
     printf("%s ----> ", NtermString[i]);
        for(int j=0; j<63; j++){
            if((z&((uint64_t)1<<j))>0){
                printf("%s ", termString[j]);
            }
        }
        printf("\n");
}

void follow(int enumindx, FIRST fr, GRAMMAR g,FOLLOW fl){

    // printf("Entering follow of %s\n", NtermString[enumindx]);

    if (fl[enumindx].val != 0)
        return;
    
    fnode* fo = (fnode*)malloc(sizeof(fnode));
    fo->val=0;
    // int countRule = 0;

    for(int j=0; (g[j] != NULL); j++){

        gnode* n;
        gnode* nnex;
        n = g[j]->next;

        while(n!= NULL){

            if(n->term == enumindx && n->tag == 1){

                if(n->next != NULL){

                    nnex = n->next;

                    if(nnex->tag == 1)
                        fo->val |= fr[nnex->term].val;
                    else
                        fo->val |= ((uint64_t)1 << nnex->term);
                    
                    // fo->whichRule[countRule] = j;
                    // countRule++;
                    while((nnex!= NULL && /*(nnex->term == 0 && nnex->tag == 0)) || */(nnex->tag == 1) && (fr[nnex->term].val%2 == 1))){
                        
                        if(nnex->next != NULL){
                            if(nnex->next->tag == 1)
                                fo->val |= fr[nnex->next->term].val;
                            else{
                                fo->val |= ((uint64_t)1 << nnex->next->term);
                            }

                        }
                        else{
                            if(fl[g[j]->term].val == 0 && (g[j]->term != enumindx && g[j]->tag == 1)){
                                follow(g[j]->term,fr,g,fl);
                            }
                            fo->val |= fl[g[j]->term].val;
                            break;
                        }

                        nnex = nnex->next;
                    }

                }

                else{

                    if(fl[g[j]->term].val == 0 && (g[j]->term != enumindx && g[j]->tag == 1))
                        follow(g[j]->term,fr,g,fl);
                            
                    fo->val |= fl[g[j]->term].val;

                    break;

                }
            }

            n = n->next;
        }
    }
    fl[enumindx].val = (fo->val % 2 == 0) ? fo->val : fo->val - 1;

}

void allFollow(FIRST fr, int grammarIndex[], GRAMMAR gr, FOLLOW fl){

    fl[0].val = ((uint64_t)1 << DOLLAR);
    for(int j=1; j<countnt; j++){
        fl[j].val = 0;
    }

    int j=0;

    while(j<countnt){

        if(fl[j].val == 0){
            follow(j,fr,gr,fl);
        }
        j++;

    }

    // for (int i=0; i<countnt; i++){
    //     unsigned long long int z = ("%llu\n", fl[i].val);
    //     printfirst(i, z, termString, NtermString);
    // }

}

void populate_parse_table(GRAMMAR gr, FIRST fst, FOLLOW fol, PARSE_TABLE pt){
    // iterate over grammar rules
    PARSE_TABLE table;
    for(int i=0;i<countnt;i++)
        for(int j=0;j<COUNTT;j++)
            table[i][j] = -1;
    int rule;
    for(rule=0;;rule++){
        if(gr[rule] == NULL) break;
        gnode *A = gr[rule];//->next;
        gnode *alpha=A->next;
        // while alpha exists keep finding first
        while(alpha!=NULL){
            // if terminal
            if (alpha->tag == 0){
                // if terminal and not epsilon
                if(alpha->term!=0)
                    table[A->term][alpha->term] = rule;
                // epsilon
                else
                    alpha=NULL;
                break;
            }

            __uint128_t term_first = fst[alpha->term].val;
            // for a in fst[alpha->term] add M[A,a] = A->alpha
            for(int i=2; i<COUNTT; i++){
                if((term_first & ((__uint128_t)1<<i))!=0){
                    // table[A->term][i] = fst[alpha->term].whichRule[i];
                    table[A->term][i] = rule;
                }
            }
            // check if first(alpha) has null. If yes then continue
            if (term_first%2 == 1){
                alpha = alpha->next;
                continue;
            }
            else{
                break;
            }
        }
        // if alpha is NULL, entire rule has been traversed so follow of LHS has to be included
        if(alpha == NULL){
            __uint128_t term_foll = fol[A->term].val;
            // for b in fol(A) add M[A,b] = A->alpha
            for(int i=1; i<COUNTT; i++){
                if((term_foll & ((__uint128_t)1<<i))!=0){                    
                    table[A->term][i] = rule;
                }
            }
            // if ((term_foll & (__uint128_t)1) != 0){
            //     table[A->term][DOLLAR] = rule;
            // }
        }
    }
    memcpy(pt,table,COUNTT*countnt*sizeof(int));
}

stack_node* pop(stack_node* head){
    stack_node* temp = head;
    head = head->next;
    free(temp);
    return head;
}

stack_node* push(gnode* arr[], ptree_node* ptrs[], int len, stack_node* head){
    // gr points to rhs of rule
    // make array
    for(int i=len-1;i>=0;i--){
        if(arr[i]->term==EPSILON && arr[i]->tag==TERMINAL)
            continue;
        stack_node* node = malloc(sizeof(stack_node));
        node->term=arr[i]->term;
        node->tag = arr[i]->tag;
        node->ptr = ptrs[i];
        node->next=head;
        head=node;
    }
    return head;
}

ptree_node* create_tree_node(char* lexeme, int rule, int lineNumber, int token, char* symbol, unsigned int isLeaf, ptree_node* parent){
    ptree_node* node = malloc(sizeof(ptree_node));
    for(int i=0;i<CHILDREN_SIZE;i++){
        node->children[i] = NULL;
    }
    strcpy(node->lexeme,lexeme);
    node->lineNumber=lineNumber;
    node->token = token;
    node->rule = rule;
    // node->value = value;
    node->isLeaf=1-isLeaf;
    strcpy(node->symbol,symbol);
    node->parentNode = parent;
    return node;
}

ptree_node* make_parse_tree(GRAMMAR gr, PARSE_TABLE pt,char* input, FIRST first, FOLLOW follow, FILE* fin){
    
    char* termString[] = {"EPSILON","DOLLAR","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","DRIVERDEF","ENDDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","ID","RNUM","NUM","ERROR","COUNTT"};
    char* NtermString[] = {"program","moduledeclarations","moduledeclaration","othermodules","drivermodule","module","ret","inputplist","n1","outputplist","n2","datatype","rangearrays","type","moduledef","statements","statement","iostmt","boolconstt","varidnum","var","whichid","simplestmt","assignstmt","whichstmt","lvalueidstmt","lvaluearrstmt","ind","modulereusestmt","optional","idlist","n3","expression", "u","newnt","arithmeticorbooleanexpr","n7","anyterm","n8","arithmeticexpr","n4","term","n5","factor","op1","op2","logicalop","relationalop","declarestmt","conditionalstmt","casestmts","n9","value","dflt","iterativestmt","range","countnt"};

    int indx=0; int lineNumber=1;
    lex* tokenReturn = malloc(sizeof(lex));
    
    stack_node* stack = malloc(sizeof(stack_node));
    stack->term = program;
    stack->tag = NON_TERMINAL;
    stack->next = malloc(sizeof(stack_node));
    // stack = stack->next;
    stack->next->term = DOLLAR;
    stack->next->tag = TERMINAL;
    stack->next->next=NULL;
    // what is non terminal's line number?
    ptree_node* tree_root=create_tree_node("program",0,-1,program,"PROGRAM",1,NULL);
    stack->ptr = tree_root;
    getNextToken(input,&indx,&lineNumber,tokenReturn, fin);
    int tokFollow = 0;
    while(1){
        // if stack head is terminal
        if(stack->tag==TERMINAL && stack->term==DOLLAR && tokenReturn->token==DOLLAR){
            printf("\nPARSED SUCCESSFULLY\n");
            stack = pop(stack);
            break;
        }
        if(stack->tag==TERMINAL){
            if(tokenReturn->token==stack->term){
                // if(tokenReturn->line == 9)printf("......%s %s\n", termString[stack->term], tokenReturn->lexeme);
                stack->ptr->value = tokenReturn->value;
                strcpy(stack->ptr->lexeme, tokenReturn->lexeme);
                stack = pop(stack);
                // if (tokFollow == 0 && tokenReturn->token != DOLLAR)
                do{
                    getNextToken(input,&indx,&lineNumber,tokenReturn, fin);
                }while(tokenReturn->token == ERROR);
                // if(tokenReturn->line == 9)printf("After get......%s %s\n", termString[stack->term], tokenReturn->lexeme);
                // tokFollow = 0;
                continue;
            }
            else{
                //Error1 - terminal on stack and terminal in input - don't match
                //Works only when a wrong token is present in place of a correct token
                // printf("ERROR at line %d TOKEN MISMATCH Expected %s Found %s[Stack, Input] -> [%s, %s] \n", tokenReturn->line, termString[stack->term], tokenReturn->lexeme);
                printf("SYNTAX ERROR at line %d TOKEN MISMATCH Expected %s Found %s\n", tokenReturn->line, termString[stack->term], tokenReturn->lexeme);
                if (!(stack->tag == TERMINAL && stack->term == DOLLAR))
                    stack = pop(stack);
                    getNextToken(input,&indx,&lineNumber,tokenReturn, fin);
                // tokFollow = 0;
                // raise parser error   
            }
        }
        else{
            int rule = pt[stack->term][tokenReturn->token];
            if(rule==-1){
                // raise parser error
                //Error2 - non-terminal on stack and terminal in input - keep moving until you encounter first/follow symbol
                while (tokenReturn->token != DOLLAR){
                    //non-terminal and matches first
                    if (stack->tag == NON_TERMINAL && ((((__uint128_t)1<<(tokenReturn->token)) & first[stack->term].val )!= 0 )){
                            // stack = pop(stack);
                        break;
                    }
                    else if (stack->tag == NON_TERMINAL && ((((__uint128_t)1<<(tokenReturn->token)) & follow[stack->term].val )!= 0)){
                        stack = pop(stack);
                        break;
                    }
                    
                    printf("SYNTAX ERROR at line %d on [Stack, Input] -> [%s, %s] NO RULE FOUND. ", tokenReturn->line, NtermString[stack->term], tokenReturn->lexeme);
                    printf("Expected one of: [");
                    for (int i=0; i<COUNTT; i++){
                        if (pt[stack->term][i] != -1)
                            printf(" %s", termString[i]);
                    }
                    printf("]\n");
                    
                    getNextToken(input,&indx,&lineNumber,tokenReturn, fin);
                }
            }
                // tokMismatch = 0;
                // getNextToken(input,&indx,&lineNumber,tokenReturn);
            else{
                gnode* alpha= gr[rule]->next;
                ptree_node** ptrs = (stack->ptr)->children;
                ptree_node* parent = stack->ptr;
                stack->ptr->rule = rule;
                stack = pop(stack);
                gnode* arr[CHILDREN_SIZE];
                int i=0;
                while(alpha!=NULL){
                    arr[i]=alpha;
                    char lexeme[100],symbol[100];
                    if(alpha->tag==TERMINAL){
                        strcpy(symbol,termString[alpha->term]);
                        strcpy(lexeme,tokenReturn->lexeme);
                    }
                    else{
                        strcpy(symbol,NtermString[alpha->term]);
                        strcpy(lexeme,NtermString[alpha->term]);
                    }
                    ptrs[i]=create_tree_node(tokenReturn->lexeme,-2,lineNumber,alpha->term,symbol,alpha->tag, parent);
                    
                    alpha=alpha->next;
                    i++;
                }
                stack = push(arr,ptrs,i,stack);
            }
        }
    }
    // printTree(tree_root);
    return tree_root;
}

void printNode(ptree_node* node, FILE* fout ){
    if (node->parentNode == NULL)
        fprintf(fout, "Rule: %d\tLexeme: %25s\tLinenumber: %5d\tToken: %3d\tValue: %s\tParent: %25s\tIsLeaf: %s\tSymbol: %25s\n", node->rule, node->lexeme, node->lineNumber, node->token,"-----", "ROOT", " NO", node->symbol);
    else if(node->isLeaf == 1 && node->token==RNUM){
        fprintf(fout, "Rule: %d\tLexeme: %25s\tLinenumber: %5d\tToken: %3d\tValue: %5f\tParent: %25s\tIsLeaf: %s\tSymbol: %25s\n", node->rule,node->lexeme, node->lineNumber, node->token, node->value.rnum, node->parentNode->symbol, "YES", node->symbol);
    }else if(node->isLeaf == 1 && node->token==NUM){
        fprintf(fout, "Rule: %d\tLexeme: %25s\tLinenumber: %5d\tToken: %3d\tValue: %5d\tParent: %25s\tIsLeaf: %s\tSymbol: %25s\n", node->rule,node->lexeme, node->lineNumber, node->token, node->value.num, node->parentNode->symbol, "YES", node->symbol);
    }else if(node->isLeaf == 1)
        fprintf(fout, "Rule: %d\tLexeme: %25s\tLinenumber: %5d\tToken: %3d\tValue: %s\tParent: %25s\tIsLeaf: %s\tSymbol: %25s\n", node->rule,node->lexeme, node->lineNumber, node->token, "-----", node->parentNode->symbol, "YES", node->symbol);
    else{
        fprintf(fout, "Rule: %d\tLexeme: %25s\tLinenumber: %5s\tToken: %3d\tValue: %s\tParent: %25s\tIsLeaf: %s\tSymbol: %25s\n", node->rule,"------", "-----", node->token, "-----", node->parentNode->symbol, " NO", node->symbol);
    }
}

void printTree(ptree_node* head, FILE *fout){
    if(head==NULL){
        return;
    }
    ptree_node* temp = head;
    // printTree(temp->children[0], fout);
    printNode(temp, fout);  
      
    for(int i=0; i<CHILDREN_SIZE;i++){
        printTree(temp->children[i],fout);
    }
    return;
}