#include <stdio.h>

#include "ast.h"

/*------------------------------------------------------------------------*/

static void ast_graphviz_r(struct ast *node)
{
	const char *parent = node->name ? node->name : "anonymous";

	printf("\t%s_%p [label=\"struct %s\", shape=box, fillcolor=darkseagreen4];\n", parent, (void*)node, parent);

	for (struct ast *i = node->right; i; i = i->left) {
		const char *name = i->name ? i->name : "anonymous";
		const char *type = i->right->name ? i->right->name : "anonymous";

		printf("\t%s_%p [label=\"%s\", fillcolor=darkolivegreen3];\n", name, (void*)i, name);
		printf("\t%s_%p -> %s_%p\n", parent, (void*)node, name, (void*)i);
		printf("\t%s_%p -> %s_%p [color=blue, style=dotted]\n", name, (void*)i, type, (void*)i->right);

		/* go deeper? */
		if (i->right->right) {
			ast_graphviz_r(i->right);
		} else {
			printf("\t%s_%p [label=\"%s\", shape=box, fillcolor=darkseagreen];\n", type, (void*)i->right, type);
		}
	}
}

/*------------------------------------------------------------------------*/

void ast_graphviz(struct ast *root)
{
	printf("digraph %s {\n", root->name);
	printf("\tnode [style=filled];\n\n");
	ast_graphviz_r(root);
	printf("}\n");

	ast_free(root);
}
