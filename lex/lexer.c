#include <stdio.h>   // java.io.BufferedReader, InputStreamReader
#include <stdlib.h>  // java.nio.file.Files, java.util.List (for memory allocation)
#include <stdbool.h> // Useful for I/O tracking and flags
#include <string.h>  // java.nio.stringset.Charset (for working with C strings)
#include "../strings/str.h" //string lib

//--------------------structs---------------
typedef enum TokenType {
  // Single-stringacter tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two stringacter tokens.
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
        string string;
    } as;
} Literal;


typedef struct Token{
TokenType type;
string lexeme;
int line;
Literal literal;

}Token;

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenArray;

typedef struct Scanner{
string source;
int start;
int current;
int line;
TokenArray tokens;
}Scanner;


//-----------func declaration---------------

void runFile(string path);
void runPrompt();
void run(string source);
void report(int line, string where, string message);
void error(int line , string message);
Token makeToken(TokenType type, string lexeme, Literal literal, int line);
TokenArray scanTokens(Scanner *scan);
bool isAtEnd(Scanner *scan);
char advance(Scanner *scan);
void addToken(Scanner *scan ,TokenType type);
void scanToken(Scanner *scan);
void addTokenLiteral(Scanner *scan, TokenType type, Literal literal);
void pushToken(TokenArray *arr, Token t);
//--------------------main------------------
int main(int argc,char *argv[]){

	if ( argc>2){
		printf("Usage: yali [script]");
		return 64;
	}else if (argc == 2){
   string file ; 
file.data = argv[1];
file.len = strlen(argv[1]);
		runFile(file);
	}else{
	runPrompt();
	}
	return 0;

}
//----------------runFile--------------------
void runFile(string path){
	FILE *file = fopen(path.data,"rb");
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

	string source;
	source.data = malloc(bytes +1);
	long sizeRead = fread(source.data, 1, bytes, file);
	if(sizeRead != bytes){
		printf("Error:read size missmatch");
	}
	source.len  =sizeRead;
	source.data[bytes] = '\0';
	fclose(file);
	run(source);
	free(source.data);
}
//------------------------runPrompt----------------

void runPrompt(){

 for (;;) { 
      printf("> ");
      char buf[102400];
      char *r =fgets(buf,102400,stdin);
      if (r == NULL) break;

      string line; 
      line.data = buf ;
      line.data[strcspn(line.data, "\n")] = '\0';
     line.len = strlen(line.data);	
      run(line);
    }


}

//----------------------------makeToken------------------------------------

Token makeToken(TokenType type, string lexeme, Literal literal, int line) {
    Token t;
    t.type = type;
    t.lexeme = lexeme;
    t.literal = literal;
    t.line = line;
    return t;
}
//-------------------isAtEnd-----------------------------------------------
bool isAtEnd(Scanner *scan) {
    return scan->current >= scan->source.len;
}
//--------------------scanTokens---------------------------------------------
TokenArray scanTokens(Scanner *scan) {
    while (!isAtEnd(scan)) {
        scan->start = scan->current;
        scanToken(scan);
    }

    Literal nilLiteral;
    nilLiteral.type = VAL_NIL;
    Token eofToken = makeToken(EOFF, strCreate(""), nilLiteral, scan->line);
    pushToken(&scan->tokens, eofToken);

    return scan->tokens;
}
//---------------------------scanToken--------------------------------------

    void scanToken(Scanner *scan) {
    char c = advance(scan);
    switch (c) {
      case '(': addToken(scan ,LEFT_PAREN); break;
      case ')': addToken(scan ,RIGHT_PAREN); break;
      case '{': addToken(scan ,LEFT_BRACE); break;
      case '}': addToken(scan ,RIGHT_BRACE); break;
      case ',': addToken(scan ,COMMA); break;
      case '.': addToken(scan ,DOT); break;
      case '-': addToken(scan ,MINUS); break;
      case '+': addToken(scan ,PLUS); break;
      case ';': addToken(scan ,SEMICOLON); break;
      case '*': addToken(scan ,STAR); break; 
    }
  }
//-----------------------------advance-------------------------------------
char advance(Scanner *scan) {
    return scan->source.data[scan->current++];
}
//----------------------------pushToken------------------------------------
  void pushToken(TokenArray *arr, Token t) {
    if (arr->count == arr->capacity) {
        arr->capacity = arr->capacity == 0 ? 8 : arr->capacity * 2;
        arr->tokens = realloc(arr->tokens, arr->capacity * sizeof(Token));
    }
    arr->tokens[arr->count++] = t;
}
//-----------------------------addTokenLiteral----------------------------
void addTokenLiteral(Scanner *scan, TokenType type, Literal literal) {
    string lexeme = subStr(scan->source.data, scan->start, scan->current);
    Token t = makeToken(type, lexeme, literal, scan->line);
    pushToken(&scan->tokens, t);
}
//---------------------addToken---------------------------------
void addToken(Scanner *scan, TokenType type) {
    Literal nilLiteral;
    nilLiteral.type = VAL_NIL;
    addTokenLiteral(scan, type, nilLiteral);
}

//------------------------run-----------------------------------
void run(string source) {
    Scanner scan;
    scan.start = 0;
    scan.current = 0;
    scan.line = 1;
    scan.tokens.tokens = NULL;
    scan.tokens.count = 0;
    scan.tokens.capacity = 0;
    scan.source = source;
    scanTokens(&scan);   
                                 
                                 

    TokenArray tokens = scan.tokens;
    int i = 0;
    while (i < tokens.count) {
        printf("%s\n", tokens.tokens[i].lexeme.data);
        i++;
    }
}
//-----------------------error--------------------------------------
void error(int line , string message){
report(line,strCreate(""),message);
}
//-----------------------------report---------------------------------
void report(int line, string where, string message){


}
