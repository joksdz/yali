#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdio.h>   // java.io.BufferedReader, InputStreamReader
#include <stdlib.h>  // java.nio.file.Files, java.util.List (for memory allocation)
#include <stdbool.h> // Useful for I/O tracking and flags
#include <string.h>  // java.nio.stringset.Charset (for working with C strings)
#include "../strings/str.h" //string lib





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
	//handeling none tokens(only needed for hash table search maybe there is a better approach but what can i do am only human after all :D )
,TOKEN_NONE
}TokenType;

typedef enum { VAL_NIL, VAL_NUMBER, VAL_STRING,VAL_IDENTIFIER } LiteralType;

typedef struct {
    LiteralType type;
    union {
        string string;
        double number;
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
void report(int line, string where, char * message);
void error(int line , char * message);
Token makeToken(TokenType type, string lexeme, Literal literal, int line);
TokenArray scanTokens(Scanner *scan);
bool isAtEnd(Scanner *scan);
char advance(Scanner *scan);
void addToken(Scanner *scan ,TokenType type);
void scanToken(Scanner *scan);
void addTokenLiteral(Scanner *scan, TokenType type, Literal literal);
void pushToken(TokenArray *arr, Token t);
bool match(Scanner *scan,char c);
void String(Scanner *scan);
void number(Scanner *scan);
bool isDigit(char c);
char peek(Scanner *scan);
char peekNext(Scanner *scan);

#endif

