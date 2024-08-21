%{
#include "setlang.tab.h"
#include <stdlib.h>
#include <string.h>


%}
%option yylineno
number      [0-9]+ 
white       [ \n\t\r]+
id          [a-zA-Z][a-zA-Z0-9]*

%%

"collection"                            { return COLLECTION; }
"set"                                   { return SET; }
"int"                                   { return INT; }
"str"                                   { return STR; }
"{"                                     { return LBRACE; }
"}"                                     { return RBRACE; }
";"                                     { return SEMICOLON; }
"["                                     { return LBRACKET; }
"]"                                     { return RBRACKET; }
","                                     { return COMMA; }
"+"                                     { return PLUS; }
"-"                                     { return MINUS;}
"*"                                     { return MULTIPLY; }
"/"                                     { return DIVIDE; }
"&"                                     { return AND; }
"|"                                     { return PIPE; }
">"                                     { return GREATER_THAN; }
"<"                                     { return LESS_THAN; }
">="                                    { return GREATER_EQUAL; }
"<="                                    { return LESS_EQUAL; }
"=="                                    { return EQUAL; }
"!="                                    { return NOT; }
"for"                                   { return FOR; }
"while"                                 { return WHILE; }
"if"                                    { return IF; }
"else"                                  { return ELSE;}
":"                                     { return COLON; }
"output"                                { return OUTPUT; }
"input"                                 { return INPUT; }
"("                                     { return LPAREN; }
")"                                     { return RPAREN; }
"or"                                    { return OR; }
"="                                     { return ASSIGN; }
{number}                                { yylval.ival = atoi(yytext);   return    NUMBER; }
{id}                                    { yylval.sval = strdup(yytext); return  IDENTIFIER; }
\"(\\.|[^\"])*\"                        { yylval.sval = strdup(yytext); return STRING_LITERAL; }

<<EOF>>                                 { return YYEOF; }


{white}                 { /*Nothing*/ }

[ \t\r\n]+              { /* Ignore whitespace */ }

.                       { fprintf(stderr, "Unexpected character: %c at line %d\n", yytext[0], yylineno); return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}

