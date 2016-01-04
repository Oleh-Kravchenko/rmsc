#include <jansson.h>
#include <rmsc.h>
#include <string.h>

/*------------------------------------------------------------------------*/
/* bool                                                                   */
/*------------------------------------------------------------------------*/

inline json_t *bool2json(bool value)
{
	return (json_boolean(value));
}

/*------------------------------------------------------------------------*/

inline bool bool4json(const json_t *json)
{
	return (json_boolean_value(json));
}

/*------------------------------------------------------------------------*/

json_t *boolp2json(const bool *array, unsigned count)
{
	json_t *json = json_array();

	for (unsigned i = 0; i < count; ++ i) {
		if (json_array_append_new(json, json_boolean(array[i]))) {
			json_decref(json);

			return (NULL);
		}
	}

	return (json);
}

/*------------------------------------------------------------------------*/

unsigned boolp4json(const json_t *json, bool *array, unsigned count)
{
	size_t index;
	json_t *value;

	json_array_foreach(json, index, value) {
		/*  bounds checking for array */
		if (index >= count) {
			break;
		}

		array[index] = json_boolean_value(value);
	}

	return (index);
}

/*------------------------------------------------------------------------*/
/* char                                                                   */
/*------------------------------------------------------------------------*/

inline json_t *char2json(char value)
{
	return (json_integer(value));
}

/*------------------------------------------------------------------------*/

inline char char4json(const json_t *json)
{
	return (json_integer_value(json));
}

/*------------------------------------------------------------------------*/

inline json_t *charp2json(const char *array, unsigned count)
{
	return (json_stringn(array, strnlen(array, count)));
}

/*------------------------------------------------------------------------*/

unsigned charp4json(const json_t *json, char *array, unsigned count)
{
	const char *s = json_string_value(json);

	if (s) {
		strncpy(array, s, count - 1);
		array[count - 1] = 0;

		return (strlen(array));
	}

	return (0);
}

/*------------------------------------------------------------------------*/
/* double                                                                 */
/*------------------------------------------------------------------------*/

inline json_t *double2json(double value)
{
	return (json_real(value));
}

/*------------------------------------------------------------------------*/

inline double double4json(const json_t *json)
{
	return (json_real_value(json));
}

/*------------------------------------------------------------------------*/

json_t *doublep2json(const double *array, unsigned count)
{
	json_t *json = json_array();

	for (unsigned i = 0; i < count; ++ i) {
		if (json_array_append_new(json, json_real(array[i]))) {
			json_decref(json);

			return (NULL);
		}
	}

	return (json);
}

/*------------------------------------------------------------------------*/

unsigned doublep4json(const json_t *json, double *array, unsigned count)
{
	size_t index;
	json_t *value;

	json_array_foreach(json, index, value) {
		/*  bounds checking for array */
		if (index >= count) {
			break;
		}

		array[index] = json_real_value(value);
	}

	return (index);
}

/*------------------------------------------------------------------------*/
/* int                                                                    */
/*------------------------------------------------------------------------*/

inline json_t *int2json(int value)
{
	return (json_integer(value));
}

/*------------------------------------------------------------------------*/

inline int int4json(const json_t *json)
{
	return (json_integer_value(json));
}

/*------------------------------------------------------------------------*/

json_t *intp2json(const int *array, unsigned count)
{
	json_t *json = json_array();

	for (unsigned i = 0; i < count; ++ i) {
		if (json_array_append_new(json, json_integer(array[i]))) {
			json_decref(json);

			return (NULL);
		}
	}

	return (json);
}

/*------------------------------------------------------------------------*/

unsigned intp4json(const json_t *json, int *array, unsigned count)
{
	size_t index;
	json_t *value;

	json_array_foreach(json, index, value) {
		/*  bounds checking for array */
		if (index >= count) {
			break;
		}

		array[index] = json_integer_value(value);
	}

	return (index);
}
