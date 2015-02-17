#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_restful.h"
#include "ast_graphviz.h"

/*------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	do {
		if (argc <= 1) {
			break;
		}

		if (!strcmp(argv[1], "-h")) {
			printf(
				"rmsc - built %s %s\n\n"
				"Restful meta structure compiler generate code\n"
				"for serializing C structures.\n"
				"\n"
				"Usage:\n"
				"  rmsc [restful] -i INPUT_FILE -p PREFIX\n"
				"  rmsc graphviz [-i INPUT_FILE] [-o OUTPUT_FILE]\n"
				"  rmsc -h\n"
				"\n"
				"Startup:\n"
				"  -h  print this help\n"
				"\n"
				"Serialization:\n"
				"  restful\n"
				"    -i  input header file (*.h)\n"
				"    -p  prefix for output files (*.c; *.h)\n"
				"\n"
				"  graphviz:\n"
				"    -i  input header file (*.h)\n"
				"    -o  output graphviz file (*.dot)\n",
				__DATE__, __TIME__
			);

			return (EXIT_SUCCESS);
		} else if ('-' == *argv[1]) {
			return (main_restful(argc, argv));
		} else if (!strcmp(argv[1], "restful")) {
			return (main_restful(argc - 1, argv + 1));
		} else if (!strcmp(argv[1], "graphviz")) {
			return (main_graphviz(argc - 1, argv + 1));
		}
	} while (0);

	puts("rmsc: missing or invalid arguments\n"
		"Try 'rmsc -h' for more options."
	);

	return (EXIT_FAILURE);
}
