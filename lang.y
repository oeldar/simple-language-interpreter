%{
#include <stdio.h>
#include <vector>
#include <iostream>
#include "expression_tree.hpp"
#include "environment.hpp"

int yylex();
void yyerror(const char* s);

extern Environment env;
%}

%union {
  int num;
  char* str;
  std::vector<ExpressionNode*>* exprs;
  std::vector<std::string>* ids;
  ExpressionNode* node;
}

%start START

%token PRINT
%token<num> NUMBER
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token FN
%token RETURN

%right '='
%left '<' '>'
%left '+' '-'
%left '*' '/'

%type<node> STMT
%type<node> EXPR
%token<str> ID
%type<ids> ID_LIST
%type<exprs> BLOCK_CODE
%type<exprs> STMT_LIST
%type<exprs> EXPR_LIST


%%

START: %empty
     | START STMT { $2->evaluate(env); }
     ;

STMT: EXPR ';'          { $$ = $1; }
    | IF EXPR THEN STMT ELSE STMT { $$ = new IfThenElseNode{$2, $4, $6}; }
    | IF EXPR THEN STMT           { $$ = new IfThenElseNode{$2, $4, nullptr};} 
    | WHILE EXPR DO STMT          { $$ = new WhileDoNode{$2, $4}; }
    | BLOCK_CODE                  { $$ = new BlockCodeNode{$1}; }
    | FN ID '(' ID_LIST ')' BLOCK_CODE { $$ = new FunctionDefinitionNode{$2, $4, $6}; }
    | RETURN EXPR ';'                { $$ = new ReturnNode{$2}; }
    ;

ID_LIST: ID_LIST ',' ID   { $$ = $1, $$->push_back($3); }
       | ID   { $$ = new std::vector<std::string>(); $$->push_back($1); }
       | %empty     { $$ = new std::vector<std::string>(); }
       ;

BLOCK_CODE: '{' STMT_LIST '}'     { $$ = $2; }
          ;

STMT_LIST: STMT_LIST STMT { $$ = $1; $$->push_back($2);}
         | STMT   { $$ = new std::vector<ExpressionNode*>(); $$->push_back($1); }
         | %empty { $$ = new std::vector<ExpressionNode*>(); }

EXPR: '-' EXPR           { $$ = new MultNode{new NumberNode{-1}, $2}; }
    | NUMBER               { $$ = new NumberNode{$1}; }
    | EXPR '+' EXPR        { $$ = new PlusNode{$1, $3}; }
    | EXPR '-' EXPR        { $$ = new MinusNode{$1, $3}; }
    | EXPR '*' EXPR        { $$ = new MultNode{$1, $3}; }
    | EXPR '/' EXPR        { $$ = new DivNode{$1, $3}; }
    | EXPR '=' EXPR        { $$ = new EqualNode{$1, $3};}
    | EXPR '<' '=' EXPR    { $$ = new LeqThanNode{$1, $4}; }
    | EXPR '>' '=' EXPR    { $$ = new GeqThanNode{$1, $4}; }
    | EXPR '<' '>' EXPR    { $$ = new NotEqualNode{$1, $4};}
    | EXPR '<' EXPR        { $$ = new LessThanNode{$1, $3};}
    | EXPR '>' EXPR        { $$ = new GreaterThanNode{$1, $3};}
    | '(' EXPR ')'         { $$ = $2; }
    | ID '(' EXPR_LIST ')' { $$ = new FunctionCallNode{$1, $3}; }
    | PRINT '(' EXPR ')'   { $$ = new PrintNode{$3}; }
    | ID ':' '=' EXPR      { $$ = new AssignmentNode{$1, $4}; }
    | ID                   { $$ = new VariableNode{$1}; }
    ;

EXPR_LIST: EXPR_LIST ',' EXPR { $$ = $1, $$->push_back($3); }
         | EXPR { $$ = new std::vector<ExpressionNode*>(); $$->push_back($1); }
         | %empty { $$ = new std::vector<ExpressionNode*>(); }
         ;


ID_LIST: ID_LIST ',' ID   { $$ = $1; $$->push_back($3); }
       | ID               { $$ = new std::vector<std::string>(); $$->push_back($1); }
       ;

%%

Environment env;

int main() {
  yyparse();
}

void yyerror(const char *s) {
  extern int yylineno;  // defined and maintained in lex
  extern char *yytext;  // defined and maintained in lex
  fprintf(stderr, "Error: %s at symbol '%s' on line %d\n", s, yytext, yylineno);
}
