/**
 * @file
 */

#ifndef __RMSC_AST_GRAPHVIZ_H
#define __RMSC_AST_GRAPHVIZ_H

#include "ast.h"

/**
 * @brief produce output for graphviz and release ast tree
 * @param [in] root ast tree
 */
void ast_graphviz(struct ast *root);

#endif /* __RMSC_AST_GRAPHVIZ_H */
