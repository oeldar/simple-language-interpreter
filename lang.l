%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <vector>
#include "expression_tree.hpp"
#include "environment.hpp"

#include "lang.tab.h"
%}

ws         [ \t\n]
number     [0-9]
letter     [a-zA-Z_]
id         {letter}({letter}|{number})*

%%

{ws}+              ;
"print"            { return PRINT; } 
"if"               { return IF; }
"then"             { return THEN; }
"else"             { return ELSE; }
"while"            { return WHILE; }
"do"               { return DO; }
"fn"               { return FN; }
"return"           { return RETURN; }
{id}               { yylval.str = strdup(yytext); return ID; }
{number}+          { yylval.num = atoi(yytext); return NUMBER; }
.                  { return *yytext; }

%%
