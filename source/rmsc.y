%{
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <rmsc/ast.h>

int yylex();

void yyerror(const char *s, ...);

%}

%union {
	char *str;

	struct ast *node;
}

%token HEXADECIMAL
%token STRUCT
%token INTEGER
%token <str> IDENTIFIER
%token IGNORE

%type <node> type
%type <node> fields

%destructor { fprintf(stderr, "free at %d %s\n", @$.first_line, $$); free($$); } <str>

%%

statement	:	restful
			|	restful statement
			|	IGNORE
			|	IGNORE statement
			;

restful	:	STRUCT IDENTIFIER '{' fields '}' ';'	{ do_work(ast_new_struct($2, $4)); }
		;

fields	:	type IDENTIFIER ';'								{ $$ = ast_new_struct($2, $1); }
		|	type IDENTIFIER '[' expression ']' ';'			{ $$ = ast_newp_struct($2, $1); }
		|	type IDENTIFIER ';' fields						{ $$ = ast_set_field($2, $4, $1); }
		|	type IDENTIFIER '[' expression ']' ';' fields	{ $$ = ast_setp_field($2, $7, $1); }
		;

type	:	IDENTIFIER							{ $$ = ast_new_type($1); }
		|	STRUCT '{' fields '}'				{ $$ = ast_new_struct(NULL, $3); }
		|	STRUCT IDENTIFIER '{' fields '}'	{ $$ = ast_new_struct($2, $4); }
		;

expression	:	INTEGER
			|	IDENTIFIER	{ free($1); }
			|	HEXADECIMAL
			;

%%

void yyerror(const char *s, ...)
{
	extern int yylineno;
	va_list ap;

	fprintf(stderr, "%d: error: ", yylineno);

	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);

	fprintf(stderr, "\n");
}
