#include <stdlib.h>

#include "ast.h"

/*------------------------------------------------------------------------*/

struct ast *ast_new_type(char *name)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name;
	node->left = node->right = NULL;
	node->dimension = 0;

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_new_struct(char *name, struct ast *right)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name;
	node->left = NULL;
	node->right = right;
	node->dimension = 0;

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_newp_struct(char *name, struct ast *right)
{
	struct ast *node = ast_new_struct(name, right);

	if (node) {
		++ node->dimension;
	}

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_set_field(char *name, struct ast *left, struct ast *right)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name;
	node->left = left;
	node->right = right;
	node->dimension = 0;

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_setp_field(char *name, struct ast *left, struct ast *right)
{
	struct ast *node = ast_set_field(name, left, right);

	if (node) {
		++ node->dimension;
	}

	return (node);
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
