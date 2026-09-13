// ik this code looks like ai gen but no its not its just my style :)
/**
 * _____ _                     _____                 _        
 * |_   _| |                   /  __ \               | |       
 * | | | |      _____   _____| /  \/ __ _ _ __   __| |_   _  
 * | | | |     / _ \ \ / / _ \ |    / _` | '_ \ / _` | | | |
 * _| |_| |___| (_) \ V /  __/ \__/\ (_| | | | | (_| | |_| |
 * \___/\_____/\___/ \_/ \___|\____/\__,_|_| |_|\__,_|\__, |
 *                                                     __/ |
 *                                                    |___/  
 */

#include "lexer.h"
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

//---------------------------match-------------------------------------
bool match(Scanner *scan,char c){
if (isAtEnd(scan)) return false;
if(scan->source.data[scan->current] == c) return true;
scan->current++;
return true;
}
//-----------------------------peek-----------------------------------
char peek(Scanner *scan){
if (isAtEnd(scan)) return '\0';
return scan->source.data[scan->current];
}
char peekNext(Scanner *scan){
if (isAtEnd(scan)) return '\0';
return scan->source.data[scan->current+1];
}

//----------------------------String-----------------------------------
void String(Scanner *scan){
while(peek(scan)!='\"' && !isAtEnd(scan)){
if(peek(scan) == '\n') scan->line++;
advance(scan);
}

if(isAtEnd(scan)){
error(scan->line,"Unterminated string");
return;
}
advance(scan);
string value = subStr(scan->source.data,scan->start+1,scan->current-1);
addTokenLiteral(scan,STRING,(Literal){VAL_STRING,value});
}
//-----------------------------isDigit-----------------------------------
bool isDigit(char c){
return c >= '0' && c <= '9';
}
//-----------------------------identifier--------------------------------

void identifier(Scanner *scan){
	  while (isalnum(peek(scan))) advance(scan);

    addTokenLiteral(scan,IDENTIFIER,(Literal){VAL_IDENTIFIER , subStr(scan->source.data,scan->start,scan->current).data});
}


//-----------------------------str2duble-----------------------------------
double str2double(string str){
char *end;
double val = strtod(str.data,&end);
if (val == 0 && end == str.data) return 0;
return val;
}
//----------------------------Number-----------------------------------
void number(Scanner *scan){
while(isDigit(peek(scan))) advance(scan);
if (peek(scan) == '.' && isDigit(peekNext(scan))) {
    advance(scan); //this consumes the . (since peak doesnt)
    while (isDigit(peek(scan))) advance(scan); //(and this consumes the rest of the number after the .)
}
double value = str2double(subStr(scan->source.data,scan->start,scan->current));
addTokenLiteral(scan,NUMBER,(Literal){VAL_NUMBER,.as={.number=value} });
                         //                       ^
			 //                       | 
			 //                       thanks to zaizrnation annihilation for this one 
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
      case '!': addToken(scan,match(scan,'=') ? BANG_EQUAL : BANG); break; 
      case '=': addToken(scan,match(scan,'=') ? EQUAL_EQUAL : EQUAL); break; 
      case '<': addToken(scan,match(scan,'=') ? LESS_EQUAL : LESS); break; 
      case '>': addToken(scan,match(scan,'=') ? GREATER_EQUAL : GREATER); break; 
      case '/':
	if (match(scan,'/')) {
	while (!isAtEnd(scan)&& peek(scan) != '\n') advance(scan);
	} else {
	addToken(scan ,SLASH);
	}
	break;
      case ' ':
      case '\r':
      case '\t': break; //skips whitespace
      case '\n':
	scan->line++;
	break;
      case '"': String(scan); break;
	
	default: 
	if (isDigit(c)) {
		number(scan);
	}else if (isalpha(c)) {
		identifier(scan);
	}
	else{
		error(scan->line,"Unexpected character.");
	}
	break;
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
void error(int line , char * message){
report(line,strCreate(""),message);
}
//-----------------------------report---------------------------------
void report(int line, string where, char* message){

}
