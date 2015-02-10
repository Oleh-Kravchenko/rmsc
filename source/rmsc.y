%{
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <jansson.h>

#include "ast.h"
#include "rmsc.h"



#include "ast.h"

void ast_restful(struct ast* root);
void ast_graphviz(struct ast *root);

void reset();
#define YYERROR_VERBOSE

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
%destructor { fprintf(stderr, "free at %d %p\n", @$.first_line, (void*)$$); ast_free($$); } <node>

%%

statement	:	restful
			|	restful statement
			|	IGNORE
			|	IGNORE statement
			;

restful	:	STRUCT IDENTIFIER '{' fields '}' ';'	{ ast_graphviz(ast_new_struct($2, $4)); reset(); }
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

	fprintf(stderr, "line %d: ", yylineno);

	va_start(ap, s);
	vfprintf(stderr, s, ap);
	va_end(ap);

	fprintf(stderr, "\n");
}

/*------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	char outfile[PATH_MAX];
	char infile[PATH_MAX];

	/* verify input arguments */
	do {
		int opt;

		/* initialize */
		*infile = 0;
		*outfile = 0;

		while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
			switch (opt) {
				case 'i':
					strncpy(infile, optarg, sizeof(infile));
					break;

				case 'o':
					strncpy(outfile, optarg, sizeof(outfile));
					break;

				case 'h':
					puts("Usage: rmsc -i INPUT_FILE -o OUTPUT_FILE\n"
						"\n"
						"Startup:\n"
						"  -i  name of input file.\n"
						"  -o  name of output file.\n"
						"\n"
						"Description:\n"
						"Restful meta struct compiler produce code for serializing"
						" C structures from/to json."
					);

					return (EXIT_SUCCESS);

				default:
					puts("Try 'rmsc -h' for more options.");

					return (EXIT_FAILURE);
			}
		}

		/* check conflicts */
		if (!*infile) {
			puts("Please specify input file!");

			return (EXIT_FAILURE);
		}
	} while (0);


	/* fixup IO streams */
	stdin = freopen(infile, "r", stdin);
	assert(stdin);

	if (*outfile) {
		stdout = freopen(outfile, "w", stdout);
		assert(stdout);
	}


	/* run parser */
	if (yyparse()) {
		return (EXIT_FAILURE);
	}

	return (0);
}
