#include "../lex/lexer.h"
typedef struct {
    char *key; 
    TokenType val;
     
} Entry;

typedef struct {
    int count;      
    int capacity;
    Entry *entries;
} Map;


unsigned long hash(char *key);
void initMap(Map *map);
void freeMap(Map *map);
void insertMap(Map *map, char *key, TokenType val);
TokenType getValFromMap(Map *map, char *key);
void delMap(Map *map, char *key);
void dispMap(Map *map);


