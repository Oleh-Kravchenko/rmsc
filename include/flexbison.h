/**
 * @file
 */

#ifndef __RMSC_FLEXBISON_H
#define __RMSC_FLEXBISON_H

#include "ast.h"

/** reset state of scanner */
void flex_reset();

/**
 * @brief set up handler for abstract syntax tree
 * @param [in] ast_func pointer to function
 */
void bison_set_ast(void (*ast_func)(struct ast*));

/**
 * @brief print message into stderr stream
 * @param [in] s message
 *
 * This is printf() like function.
 */
void yyerror(const char *s, ...);

/** run bison parser */
int yyparse(void);

#endif /* __RMSC_FLEXBISON_H */
