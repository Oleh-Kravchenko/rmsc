#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ast.h"
#include "flexbison.h"

/*------------------------------------------------------------------------*/

static FILE *hdr = NULL;
static FILE *src = NULL;

/*------------------------------------------------------------------------*/

static void printtab(FILE *stream, unsigned tab)
{
	assert(stream);

	while (tab) {
		fprintf(stream, "\t");

		-- tab;
	}
}

/*------------------------------------------------------------------------*/

static void ast_restful2json(const struct ast *node, unsigned tab, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name;
		const char *type = i->right->name;

		char *ncpath;
		char *njpath = NULL;
		char *jroot = NULL;


		/* format path for C and for JSON */
		if (cpath) {
			if (-1 == asprintf(&ncpath, "%s.%s", cpath, i->name)) {
				ncpath = NULL;

				goto err;
			}
		} else {
			if (-1 == asprintf(&ncpath, "%s->%s", node->name, i->name)) {
				ncpath = NULL;

				goto err;
			}
		}

		if (jpath) {
			if (-1 == asprintf(&njpath, "%s_%s", jpath, i->name)) {
				njpath = NULL;

				goto err;
			}

			jroot = (char*)jpath;
		} else {
			if (-1 == asprintf(&njpath, "json_%s_%s", node->name, i->name)) {
				njpath = NULL;

				goto err;
			}

			if (-1 == asprintf(&jroot, "json_%s", node->name)) {
				jroot = NULL;

				goto err;
			}
		}


		/* serialize node */
		if (i->right->right) {
			assert(!i->dimension);

			printtab(src, tab);
			fprintf(src, "json_object_set_new(%s, \"%s\", %s);\n\n", jroot, name, njpath);

			printtab(src, tab);
			fprintf(src, "json_t *%s = json_object();\n", njpath);

			ast_restful2json(i->right, tab, ncpath, njpath);
		} else {
			printtab(src, tab);

			if (i->dimension) {
				fprintf(src, "json_t *%s = %sp2json(%s, countof(%s));\n", njpath, type, ncpath, ncpath);
			} else {
				fprintf(src, "json_t *%s = %s2json(%s);\n", njpath, type, ncpath);
			}

			printtab(src, tab);
			fprintf(src, "json_object_set_new(%s, \"%s\", %s);\n", jroot, name, njpath);
		}


	err:
		/* free strings */
		if (jroot != jpath) {
			free(jroot);
		}

		free(ncpath);
		free(njpath);
	}
}

/*------------------------------------------------------------------------*/

static void ast_restful4json(const struct ast *node, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name;
		const char *type = i->right->name;

		char *ncpath;
		char *njpath = NULL;
		char *jroot = NULL;


		/* format path for json and for c */
		if (cpath) {
			if (-1 == asprintf(&ncpath, "%s.%s", cpath, i->name)) {
				ncpath = NULL;

				goto err;
			}
		} else {
			if (-1 == asprintf(&ncpath, "%s->%s", node->name, i->name)) {
				ncpath = NULL;

				goto err;
			}
		}

		if (jpath) {
			if (-1 == asprintf(&njpath, "%s_%s", jpath, i->name)) {
				njpath = NULL;

				goto err;
			}

			jroot = (char*)jpath;
		} else {
			if (-1 == asprintf(&njpath, "json_%s_%s", node->name, i->name)) {
				njpath = NULL;

				goto err;
			}

			if (-1 == asprintf(&jroot, "json_%s", node->name)) {
				jroot = NULL;

				goto err;
			}
		}


		/* deserialize node */
		if (i->right->right) {
			fprintf(src, "\n\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			ast_restful4json(i->right, ncpath, njpath);
		} else {
			fprintf(src, "\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			if (i->dimension) {
				fprintf(src, "\t%sp4json(%s, %s, countof(%s));\n", type, njpath, ncpath, ncpath);
			} else {
				fprintf(src, "\t%s = %s4json(%s);\n", ncpath, type, njpath);
			}
		}


	err:
		/* free strings */
		if (jroot != jpath) {
			free(jroot);
		}

		free(ncpath);
		free(njpath);
	}
}

/*------------------------------------------------------------------------*/

void ast_restful(struct ast* root)
{
	/* serialize header */
	fprintf(hdr, "json_t *%s2json(struct %s* %s);\n\n", root->name, root->name, root->name);
	fprintf(hdr, "void %s4json(json_t *json_%s, struct %s* %s);\n\n", root->name, root->name, root->name, root->name);


	/* serialize source */
	fprintf(src, "json_t *%s2json(struct %s* %s)\n", root->name, root->name, root->name);
	fprintf(src, "{\n");
	fprintf(src, "\tjson_t *json_%s = json_object();\n\n", root->name);
	ast_restful2json(root, 1, NULL, NULL);
	fprintf(src, "\n\treturn(json_%s);\n", root->name);
	fprintf(src, "}\n\n");

	fprintf(src, "void %s4json(json_t *json_%s, struct %s* %s)\n", root->name, root->name, root->name, root->name);
	fprintf(src, "{\n");
	ast_restful4json(root, NULL, NULL);
	fprintf(src, "}\n\n");


	ast_free(root);
}

/*------------------------------------------------------------------------*/

int main_restful(int argc, char **argv)
{
	char infile[PATH_MAX];
	char prefix[PATH_MAX];
	int rc = EXIT_FAILURE;


	/* parse input arguments */
	do {
		int opt;

		/* initialize */
		*infile = 0;
		*prefix = 0;

		/* verify input arguments */
		while ((opt = getopt(argc, argv, "i:p:")) != -1) {
			switch (opt) {
				case 'i':
					strncpy(infile, optarg, sizeof(infile) - 1);
					infile[sizeof(infile) - 1] = 0;

					break;

				case 'p':
					strncpy(prefix, optarg, sizeof(prefix) - 1);
					prefix[sizeof(prefix) - 1] = 0;

					break;

				default:
					puts("Try 'rmsc -h' for more options.");

					return (rc);
			}
		}

		/* setup */
		bison_set_ast(ast_restful);

		if (!*infile) {
			puts("rmsc: no input file\n"
				"Try 'rmsc -h' for more options."
			);

			return (rc);
		}

		if (!*prefix) {
			puts("rmsc: no prefix\n"
				"Try 'rmsc -h' for more options."
			);

			return (rc);
		}

		/* redirect stdin stream */
		if (!freopen(infile, "r", stdin)) {
			perror(infile);

			return (rc);
		}
	} while (0);


	/* do work */
	char *_prefix = NULL;
	char hdrname[PATH_MAX];
	char srcname[PATH_MAX];

	do {
		/* format paths */
		snprintf(hdrname, sizeof(hdrname), "%s.h", prefix);
		snprintf(srcname, sizeof(srcname), "%s.c", prefix);

		if (!(_prefix = strdup(prefix))) {
			perror(prefix);

			return (rc);
		}

		const char *def_prefix = basename(_prefix);


		/* allocate resources and open files */
		if (!(hdr = fopen(hdrname, "w"))) {
			perror(hdrname);

			break;
		}

		if (!(src = fopen(srcname, "w"))) {
			perror(srcname);

			break;
		}


		/* print defines to header file */
		fprintf(hdr, "#ifndef __%s_H\n", def_prefix);
		fprintf(hdr, "#define __%s_H\n\n", def_prefix);
		fprintf(hdr, "#include <rmsc.h>\n");
		fprintf(hdr, "#include \"%s\"\n\n", infile);

		/* include header in source file*/
		fprintf(src, "#include \"%s\"\n\n", hdrname);


		/* run parser */
		if (yyparse()) {
			break;
		}


		/* complete header file */
		fprintf(hdr, "#endif /* __%s_H rmsc - Build %s %s */\n",
			def_prefix, __DATE__, __TIME__
		);

		/* complete source file */
		fprintf(src, "/* rmsc - Build %s %s */\n", __DATE__, __TIME__);


		rc = EXIT_SUCCESS;
	} while (0);


	/* free used resources */
	do {
		free(_prefix);

		if (hdr) {
			fclose(hdr);

			if (rc) {
				/* error occurred, remove header file */
				unlink(hdrname);
			}
		}

		if (src) {
			fclose(src);

			if (rc) {
				/* error occurred, remove source file */
				unlink(srcname);
			}
		}
	} while (0);


	return (rc);
}
