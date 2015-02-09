#include <stdlib.h>
#include <string.h>

#include <rmsc/ast.h>

/*------------------------------------------------------------------------*/

struct ast *ast_new_type(const char *name)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name ? strdup(name) : NULL;
	node->l = node->r = NULL;
	node->pointer = 0;

	if (name && !node->name) {
		free(node);

		node = NULL;
	}

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_new_struct(const char *name, struct ast *right)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name ? strdup(name) : NULL;
	node->l = NULL;
	node->r = right;
	node->pointer = 0;

	if (name && !node->name) {
		free(node);

		node = NULL;
	}

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_newp_struct(const char *name, struct ast *right)
{
	struct ast *node = ast_new_struct(name, right);

	if (node) {
		node->pointer = 1;
	}

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_set_field(const char *name, struct ast *left, struct ast *right)
{
	struct ast *node;

	if (!(node = malloc(sizeof(*node)))) {
		return (node);
	}

	node->name = name ? strdup(name) : NULL;
	node->l = left;
	node->r = right;
	node->pointer = 0;

	if (name && !node->name) {
		free(node);

		node = NULL;
	}

	return (node);
}

/*------------------------------------------------------------------------*/

struct ast *ast_setp_field(const char *name, struct ast *left, struct ast *right)
{
	struct ast *node = ast_set_field(name, left, right);

	if (node) {
		node->pointer = 1;
	}

	return (node);
}
