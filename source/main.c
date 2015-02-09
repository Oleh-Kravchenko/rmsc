#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <jansson.h>

#include <rmsc/ast.h>
#include <rmsc/rmsc.h>

/*------------------------------------------------------------------------*/

void ast_tabs(unsigned tab)
{
	while (tab) {
		putchar('\t');

		-- tab;
	}
}

/*------------------------------------------------------------------------*/

void ast_serialize(const struct ast *node, unsigned tab, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->r; i; i = i->l) {
		const char *name = i->name;
		const char *type = i->r->name;

		char *ncpath;
		char *njpath;
		char *jroot;


		/* format path for C and for JSON */
		if (cpath) {
			asprintf(&ncpath, "%s.%s", cpath, i->name);
		} else {
			asprintf(&ncpath, "%s->%s", node->name, i->name);
		}

		if (jpath) {
			asprintf(&njpath, "%s_%s", jpath, i->name);
			jroot = (char*)jpath;
		} else {
			asprintf(&njpath, "json_%s_%s", node->name, i->name);
			asprintf(&jroot, "json_%s", node->name);
		}


		/* serialize node */
		if (i->r->r) {
			assert(!i->pointer);

			ast_tabs(tab);
			printf("json_object_set_new(%s, \"%s\", %s);\n\n", jroot, name, njpath);

			ast_tabs(tab);
			printf("json_t *%s = json_object();\n", njpath);

			ast_serialize(i->r, tab, ncpath, njpath);
		} else {
			ast_tabs(tab);

			if (i->pointer) {
				printf("json_t *%s = %sp2json(%s, __countof(%s));\n", njpath, type, ncpath, ncpath);
			} else {
				printf("json_t *%s = %s2json(%s);\n", njpath, type, ncpath);
			}

			ast_tabs(tab);
			printf("json_object_set_new(%s, \"%s\", %s);\n", jroot, name, njpath);
		}


		/* free strings */
		if (jroot != jpath) {
			free(jroot);
		}

		free(ncpath);
		free(njpath);
	}
}

/*------------------------------------------------------------------------*/

void ast_deserialize(const struct ast *node, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->r; i; i = i->l) {
		const char *name = i->name;
		const char *type = i->r->name;

		char *ncpath;
		char *njpath;
		char *jroot;


		/* format path for json and for c */
		if (cpath) {
			asprintf(&ncpath, "%s.%s", cpath, i->name);
		} else {
			asprintf(&ncpath, "%s->%s", node->name, i->name);
		}

		if (jpath) {
			asprintf(&njpath, "%s_%s", jpath, i->name);
			jroot = (char*)jpath;
		} else {
			asprintf(&njpath, "json_%s_%s", node->name, i->name);
			asprintf(&jroot, "json_%s", node->name);
		}


		/* deserialize node */
		if (i->r->r) {
			printf("\n\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			ast_deserialize(i->r, ncpath, njpath);
		} else {
			printf("\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			if (i->pointer) {
				printf("\t%sp4json(%s, &%s, __countof(%s));\n", type, njpath, ncpath, ncpath);
			} else {
				printf("\t%s = %s4json(%s);\n", ncpath, type, njpath);
			}
		}


		/* free strings */
		if (jroot != jpath) {
			free(jroot);
		}

		free(ncpath);
		free(njpath);
	}
}

/*------------------------------------------------------------------------*/

void do_work(struct ast* root)
{
	static int once = 0;

	if (!once) {
		printf("#include <rmsc/rmsc.h>\n\n");

		once = 1;
	}

	printf("json_t *%s2json(struct %s* %s)\n", root->name, root->name, root->name);
	printf("{\n");
	printf("\tjson_t *json_%s = json_object();\n\n", root->name);
	ast_serialize(root, 1, NULL, NULL);
	printf("\n\treturn(json_%s);\n", root->name);
	printf("}\n\n");


	printf("void %s4json(json_t *json_%s, struct %s* %s)\n", root->name, root->name, root->name, root->name);
	printf("{\n");
	ast_deserialize(root, NULL, NULL);
	printf("}\n\n");

	void reset();
	reset();
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
