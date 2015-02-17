/**
 * @file
 */

#ifndef __RMSC_AST_H
#define __RMSC_AST_H

/** node of abstract syntax tree */
struct ast {
	/** name of node */
	char *name;

	/** left branch of tree */
	struct ast *left;

	/** @brief right branch of tree
	 *
	 * Represent type node or child items of struct.
	 */
	struct ast *right;

	/** dimension of array */
	unsigned dimension;
};

/**
 * @brief allocate ast node for type definition
 * @param [in] name type name
 * @return ast node
 * @retval NULL error occurred
 */
struct ast *ast_new_type(char *name);

/**
 * @brief allocate ast node for struct definition
 * @param [in] name struct name
 * @param [in] right child items of struct
 * @return ast node
 * @retval NULL error occurred
 */
struct ast *ast_new_struct(char *name, struct ast *right);

/**
 * @brief allocate ast node for struct field
 * @param [in] name field name
 * @param [in] left next ast node
 * @param [in] right type of field
 * @param [in] dimension dimension of array
 * @return ast node
 * @retval NULL error occurred
 */
struct ast *ast_new_field(char *name, struct ast *left, struct ast *right, unsigned dimension);

/**
 * @brief free memory used by ast
 * @param [in] node pointer to first node of ast (can be NULL)
 */
void ast_free(struct ast *node);

#endif /* __RMSC_AST_H */
