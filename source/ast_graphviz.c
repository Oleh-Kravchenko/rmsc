#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "flexbison.h"

/*------------------------------------------------------------------------*/

static const char *fc_struct = "darkseagreen4";
static const char *fc_field = "darkolivegreen3";
static const char *fc_type = "darkseagreen";

/*------------------------------------------------------------------------*/

static void ast_graphviz_int(struct ast *node)
{
	const char *parent = node->name ? node->name : "anonymous";

	printf("\t%s_%p [label=\"struct %s\", shape=box, fillcolor=%s];\n\n",
		parent, (void*)node, parent, fc_struct
	);

	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name ? i->name : "anonymous";
		const char *type = i->right->name ? i->right->name : "anonymous";

		printf("\t%s_%p [label=\"%s\", fillcolor=%s];\n", name, (void*)i, name, fc_field);
		printf("\t%s_%p -> %s_%p\n", parent, (void*)node, name, (void*)i);
		printf("\t%s_%p -> %s_%p\n", name, (void*)i, type, (void*)i->right);

		/* go deeper? */
		if (i->right->right) {
			ast_graphviz_int(i->right);
		} else {
			printf("\t%s_%p [label=\"%s\", shape=box, fillcolor=%s];\n",
				type, (void*)i->right, type, fc_type
			);
		}
	}
}

/*------------------------------------------------------------------------*/

static void ast_graphviz(struct ast *root)
{
	printf("digraph %s {\n", root->name);
	printf("\tnode [style=filled];\n\n");
	ast_graphviz_int(root);
	printf("}\n");

	ast_free(root);
}

/*------------------------------------------------------------------------*/

int main_graphviz(int argc, char **argv)
{
	/* parse input arguments */
	do {
		char outfile[PATH_MAX];
		char infile[PATH_MAX];
		int opt;

		/* initialize */
		*outfile = 0;
		*infile = 0;

		/* verify input arguments */
		while ((opt = getopt(argc, argv, "i:o:")) != -1) {
			switch (opt) {
				case 'i':
					strncpy(infile, optarg, sizeof(infile) - 1);
					infile[sizeof(infile) - 1] = 0;

					break;

				case 'o':
					strncpy(outfile, optarg, sizeof(outfile) - 1);
					outfile[sizeof(outfile) - 1] = 0;

					break;

				default:
					puts("Try 'rmsc -h' for more options.");

					return (EXIT_FAILURE);
			}
		}

		/* setup */
		bison_set_ast(ast_graphviz);

		if (*infile) {
			if (!freopen(infile, "r", stdin)) {
				perror(infile);

				return (EXIT_FAILURE);
			}
		}

		if (*outfile) {
			if (!freopen(outfile, "w", stdout)) {
				perror(outfile);

				return (EXIT_FAILURE);
			}
		}
	} while (0);

	/* run parser */
	if (yyparse()) {
		return (EXIT_FAILURE);
	}

	/* files redirected to stdio is closed by stdlib, no resource leaks! */
	return (EXIT_SUCCESS);
}
