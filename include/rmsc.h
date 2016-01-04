/**
 * @file
 */

#ifndef __RMSC_H
#define __RMSC_H

#include <jansson.h>
#include <stdbool.h>
#include <string.h>

/** keyword for rmsc */
#define __restful

/*------------------------------------------------------------------------*/
/* bool                                                                   */
/*------------------------------------------------------------------------*/

/**
 * @brief allocate json_t value
 * @param [in] value input value
 * @return pointer to json_t value
 * @retval NULL error occurred
 */
json_t *bool2json(bool value);

/**
 * @brief retrieve value from json_t
 * @param [in] json json_t
 * @return retrieved value
 */
bool bool4json(const json_t *json);

/**
 * @brief allocate json_t value from array
 * @param [in] array pointer to values array
 * @param [in] count number of elements in the array
 * @return pointer to json_t value
 * @retval NULL error occurred
 */
json_t *boolp2json(const bool *array, unsigned count);

/**
 * @brief retrive array from json_t
 * @param [in] json json_t
 * @param [out] array pointer to array
 * @param [in] count number of elements in the array
 * @return number of retrieved elements
 */
unsigned boolp4json(const json_t *json, bool *array, unsigned count);

/*------------------------------------------------------------------------*/
/* char                                                                   */
/*------------------------------------------------------------------------*/

/** @copydoc bool2json() */
json_t *char2json(char value);

/** @copydoc bool4json() */
char char4json(const json_t *json);

/** @copydoc boolp2json() */
json_t *charp2json(const char *array, unsigned count);

/** @copydoc boolp4json() */
unsigned charp4json(const json_t *json, char *array, unsigned count);

/*------------------------------------------------------------------------*/
/* double                                                                 */
/*------------------------------------------------------------------------*/

/** @copydoc bool2json() */
json_t *double2json(double value);

/** @copydoc bool4json() */
double double4json(const json_t *json);

/** @copydoc boolp2json() */
json_t *doublep2json(const double *array, unsigned count);

/** @copydoc boolp4json() */
unsigned doublep4json(const json_t *json, double *array, unsigned count);

/*------------------------------------------------------------------------*/
/* int                                                                    */
/*------------------------------------------------------------------------*/

/** @copydoc bool2json() */
json_t *int2json(int value);

/** @copydoc bool4json() */
int int4json(const json_t *json);

/** @copydoc boolp2json() */
json_t *intp2json(const int *array, unsigned count);

/** @copydoc boolp4json() */
unsigned intp4json(const json_t *json, int *array, unsigned count);

#endif /* __RMSC_H */
