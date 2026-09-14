#ifndef MAP_H
#define MAP_H

#include "../lex/lexer.h"
typedef struct {
    string key; 
    TokenType val;
     
} Entry;

typedef struct {
    int count;      
    int capacity;
    Entry *entries;
} Map;


unsigned long hash(string key);
void initMap(Map *map);
void freeMap(Map *map);
void insertMap(Map *map, string key, TokenType val);
TokenType getValFromMap(Map *map, string key);
void delMap(Map *map, string key);
void dispMap(Map *map);

#endif

