#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "ast.h"

/*------------------------------------------------------------------------*/

static void printtab(unsigned tab)
{
	while (tab) {
		putchar('\t');

		-- tab;
	}
}

/*------------------------------------------------------------------------*/

static void ast_restful_serialize(const struct ast *node, unsigned tab, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name;
		const char *type = i->right->name;

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
		if (i->right->right) {
			assert(!i->dimension);

			printtab(tab);
			printf("json_object_set_new(%s, \"%s\", %s);\n\n", jroot, name, njpath);

			printtab(tab);
			printf("json_t *%s = json_object();\n", njpath);

			ast_restful_serialize(i->right, tab, ncpath, njpath);
		} else {
			printtab(tab);

			if (i->dimension) {
				printf("json_t *%s = %sp2json(%s, __countof(%s));\n", njpath, type, ncpath, ncpath);
			} else {
				printf("json_t *%s = %s2json(%s);\n", njpath, type, ncpath);
			}

			printtab(tab);
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

static void ast_restful_deserialize(const struct ast *node, const char *cpath, const char *jpath)
{
	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name;
		const char *type = i->right->name;

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
		if (i->right->right) {
			printf("\n\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			ast_restful_deserialize(i->right, ncpath, njpath);
		} else {
			printf("\tjson_t *%s = json_object_get(%s, \"%s\");\n", njpath, jroot, name);

			if (i->dimension) {
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

void ast_restful(struct ast* root)
{
	static int once = 0;

	if (!once) {
		printf("#include <rmsc.h>\n\n");

		once = 1;
	}

	printf("json_t *%s2json(struct %s* %s)\n", root->name, root->name, root->name);
	printf("{\n");
	printf("\tjson_t *json_%s = json_object();\n\n", root->name);
	ast_restful_serialize(root, 1, NULL, NULL);
	printf("\n\treturn(json_%s);\n", root->name);
	printf("}\n\n");


	printf("void %s4json(json_t *json_%s, struct %s* %s)\n", root->name, root->name, root->name, root->name);
	printf("{\n");
	ast_restful_deserialize(root, NULL, NULL);
	printf("}\n\n");

	ast_free(root);
}
