#ifndef STR_H
#define STR_H

typedef struct string {
    char *data;
    int len;
} string;

string strCreate(const char *str);
string subStr(char *source, int start, int end);
void strFree(string *s);
int strLen(string s);
int strCmp(string s1, string s2);
string stringConcat(string s1, string s2);

#endif
