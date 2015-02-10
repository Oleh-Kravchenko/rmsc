/**
 * @file
 */

#ifndef __RMSC_H
#define __RMSC_H

#include <jansson.h>

/** keyword for rmsc */
#define __restful

/**
 * @def __countof
 * count of array items
 */
#ifndef __countof
#	define __countof(x) (sizeof(x) / sizeof(x[0]))
#endif /* __countof() */

/**
 * @brief allocate json_t value
 * @param [in] value interger value
 * @return pointer to json_t value
 * @retval NULL error occurred
 */
json_t *int2json(int value);

/**
 * @brief allocate json_t value from array
 * @param [in] array pointer to integer array
 * @param [in] count number of elements in the array
 * @return pointer to json_t value
 * @retval NULL error occurred
 */
json_t *intp2json(int *array, unsigned count);

/**
 * @brief retrieve value from json_t
 * @param [in] json json_t
 * @return retrieved value
 */
int int4json(json_t *json);

/**
 * @brief retrive array values from json_t
 * @param [in] json json_t
 * @param [out] array pointer to array
 * @param [in] count number of elements in the array
 */
void intp4json(json_t *json, int *array, unsigned count);


/** @copydoc int2json() */
json_t *char2json(char *value);

/** @copydoc int4json() */
char char4json(json_t *json);

#endif /* __RMSC_H */
