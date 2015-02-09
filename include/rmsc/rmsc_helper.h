#ifndef __RMSC_HELPER_H
#define __RMSC_HELPER_H

#include <jansson.h>

#define __restful

#ifndef __countof
#	define __countof(x) (sizeof(x) / sizeof(x[0]))
#endif /* __countof() */

json_t *char2json(char *c);

json_t *int2json(int i);

char char4json(json_t *i);

int int4json(json_t *i);

json_t *intp2json(int *array, unsigned count);

void intp4json(json_t *json, int *array, unsigned count);

#endif /* __RMSC_HELPER_H */
