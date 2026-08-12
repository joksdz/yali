#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef struct string {
char *data; 
int len; 

}string; 

//creates a string frome a char pointer source 
string strCreate(const char*str){
int len = strlen(str);
string strr;
strr.len = len;
strr.data = malloc(len+1);
strncpy(strr.data,str,len+1);
return strr ;
}
//creates a sub string 
string subStr(char *source ,int start,int end){
int len = end- start;
string str;
str.len = len;
str.data = malloc(len +1);
int i =0;
for(int s = start; s <end ; s++){
str.data[i]=source[s];
i++;
}
str.data[i++]= '\0';
return str;
}


void strFree(string *s){
s->len=0;
free(s->data);
}
//returns len
int strLen(string s){

return s.len;
}
//compares 2 strings 
int strCmp(string s1,string s2){
if(strLen(s1) != strLen(s2)){
return 69;
}else{
int r =memcmp(s1.data,s2.data,strLen(s1));
if(r==0)return 0;

return r;
}
}
string stringConcat(string s1, string s2){
string new; 
new.len = strLen(s1) + strLen(s2);
new.data = malloc(new.len +1 );
strncpy(new.data,s1.data,s1.len );
strncpy(new.data+s1.len,s2.data,s2.len);
new.data[new.len]= '\0';
return new ;
}


