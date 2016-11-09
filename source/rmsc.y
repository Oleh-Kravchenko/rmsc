%{
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include "rmsc.lex.h"
#include "flexbison.h"

static void (*ast_print)(struct ast*);
%}

%union {
	char *str;

	struct ast *node;

	unsigned dimension;
}

%token HEXADECIMAL
%token STRUCT
%token INTEGER
%token <str> IDENTIFIER
%token IGNORE

%type <node> type
%type <node> fields
%type <dimension> arrays

%destructor {
#ifndef NDEBUG
	fprintf(stderr, "free at %d %s\n", @$.first_line, $$);
#endif /* NDEBUG */
	free($$);
} <str>

%destructor {
#ifndef NDEBUG
	fprintf(stderr, "free at %d %p\n", @$.first_line, (void*)$$);
#endif /* NDEBUG */
	ast_free($$);
} <node>

%%

statement	:
		|	restful statement
		|	IGNORE statement
		;

restful	:	STRUCT IDENTIFIER '{' fields '}' ';'	{ ast_print(ast_new_struct($2, $4)); flex_reset(); }
	;

fields	:	type IDENTIFIER arrays ';'			{ $$ = ast_new_field($2, NULL, $1, $3); }
	|	type IDENTIFIER arrays ';' fields	{ $$ = ast_new_field($2, $5, $1, $3); }
	;

arrays	:								{ $$ = 0; }
	|	'[' expression ']' arrays	{ $$ = $4 + 1;}
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
	assert(s);

	extern int yylineno;
	va_list ap;

	fprintf(stderr, "line %d: ", yylineno);

	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);

	fprintf(stderr, "\n");
}

/*------------------------------------------------------------------------*/

void bison_set_ast(void (*ast_func)(struct ast*))
{
	assert(ast_func);

	ast_print = ast_func;
}
