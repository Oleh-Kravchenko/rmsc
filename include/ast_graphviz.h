/**
 * @file
 */

#ifndef __RMSC_AST_GRAPHVIZ_H
#define __RMSC_AST_GRAPHVIZ_H

#include "ast.h"

/**
 * @brief graphviz main subroutine
 * @param [in] argc number of arguments in @p argv
 * @param [in] argv array of arguments
 * @return status code
 * @retval EXIT_SUCCESS successful execution
 * @retval EXIT_FAILURE error occurred
 */
int main_graphviz(int argc, char **argv);

#endif /* __RMSC_AST_GRAPHVIZ_H */
