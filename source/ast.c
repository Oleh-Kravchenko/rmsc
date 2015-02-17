#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

/*------------------------------------------------------------------------*/

struct ast *ast_new_field(char *name, struct ast *left, struct ast *right, unsigned dimension)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name;
	node->left = left;
	node->right = right;
	node->dimension = dimension;

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_new_type(char *name)
{
	return (ast_new_field(name, NULL, NULL, 0));
}

/*------------------------------------------------------------------------*/

struct ast *ast_new_struct(char *name, struct ast *right)
{
	return (ast_new_field(name, NULL, right, 0));
}

/*------------------------------------------------------------------------*/

void ast_free(struct ast *node)
{
	if (!node) {
		return;
	}

	ast_free(node->left);
	ast_free(node->right);

	free(node->name);
	free(node);
}
