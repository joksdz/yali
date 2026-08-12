#include <stdio.h>   // java.io.BufferedReader, InputStreamReader
#include <stdlib.h>  // java.nio.file.Files, java.util.List (for memory allocation)
#include <stdbool.h> // Useful for I/O tracking and flags
#include <string.h>  // java.nio.charset.Charset (for working with C strings)
#include <errno.h>

//--------------------structs---------------
typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,EOFF
}TokenType;

typedef enum { VAL_NIL, VAL_NUMBER, VAL_STRING } LiteralType;

typedef struct {
    LiteralType type;
    union {
        double number;
        char *string;
    } as;
} Literal;


typedef struct Token{
TokenType type;
char *lexeme;
int line;
Literal literal;

}Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenArray;

typedef struct Scanner{
int start;
int current;
int line;
TokenArray tokens;
}Scanner;


//-----------func declaration---------------

void runFile(char *path);
void runPrompt();
void run(char *source);

//--------------------main------------------
int main(int argc,char *argv[]){

	if ( argc>2){
		printf("Usage: yali [script]");
		return 64;
	}else if (argc == 2){
		runFile(argv[1]);
	}else{
	runPrompt();
	}
	return 0;

}


//----------------runFile--------------------
void runFile(char *path){
	FILE *file = fopen(path,"rb");
	if(file ==NULL){
		perror("Error: Couldn't open the file");
		 exit(66);
	}



	fseek(file,0,SEEK_END);

	long bytes = ftell(file);
	fseek(file , 0 , SEEK_SET);
	if (bytes < 0 ){
		printf("Error: couldn't determine file size");
		
		exit(66);
	}

	char *source = malloc(bytes+1);
	long sizeRead = fread(source, 1, bytes, file);
	if(sizeRead == bytes){
		printf("Error:read size missmatch");
	}
	source[bytes] = '\0';
	fclose(file);
	run(source);
	free(source);
}
//------------------------runPrompt----------------

void runPrompt(){

 for (;;) { 
      printf("> ");
      char *line;
      scanf("%s",line);
      if (line == NULL) break;
      run(line);
    }


}
void run(char *source){
Scanner scan;
TokenArray tokens = scan.tokens;




}


void error(int line , string message){
report(line,"",message);
}

void report(int line, string where, string message){


}
