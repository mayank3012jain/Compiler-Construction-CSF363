
#include"lexer.h"

int main(){
    FILE *fp = fopen("input.txt", "r");
    char read_buffer[BUFFER_SIZE];
    char read_char;
    int read_ptr = 0;

    while((read_char = fgetc(fp)) != EOF){
        read_buffer[read_ptr] = read_char;
        read_ptr++;
    }

    long long LINE_NUMBER = 1;
    lex tokenReturn;
    while(1){
        tokenReturn = getNextToken(read_buffer, &read_ptr, LINE_NUMBER);
        if (tokenReturn.token == ENDOFFILE)
    }
}
