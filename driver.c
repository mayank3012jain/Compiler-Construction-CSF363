
#include "lexer.c"

int main(){
    FILE *fp = fopen("input.txt", "r");
    char read_buffer[BUFFER_SIZE];
    char read_char;
    int read_ptr = 0;

    while((read_char = fgetc(fp)) != EOF){
        read_buffer[read_ptr] = read_char;
        read_ptr++;
    }

    int LINE_NUMBER = 1;
    read_ptr = 0;
    lex tokenReturn;
    while(1){
        tokenReturn = getNextToken(read_buffer, &read_ptr, &LINE_NUMBER);
        printf(tokenReturn.lexeme);
        if (tokenReturn.token == ENDOFFILE)
        	break;
    }
}
