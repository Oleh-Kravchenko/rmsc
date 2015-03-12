
#include <string.h>
#include <jansson.h>

/*------------------------------------------------------------------------*/

json_t *int2json(int i)
{
	return (json_integer(i));
}

/*------------------------------------------------------------------------*/

json_t *intp2json(int *array, unsigned count)
{
	json_t *json;

	if (!(json = json_array())) {
		return (NULL);
	}

	for (unsigned i = 0; i < count; ++ i) {
		/* json_array_append_new() steal reference, so no leaking here */
		if (json_array_append_new(json, json_integer(array[i]))) {
			/* json_decref() automatically free all array items */
			json_decref(json); json = NULL;

			break;
		}
	}

	return (json);
}

/*------------------------------------------------------------------------*/

int int4json(json_t *i)
{
	return (json_integer_value(i));
}

/*------------------------------------------------------------------------*/

void intp4json(json_t *json, int *array, unsigned count)
{
	json_t *item;
	size_t i;

	json_array_foreach(json, i, item) {
		if (i >= count) {
			break;
		}

		array[i] = json_integer_value(item);
	}
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

json_t *char2json(char *c)
{
	return (json_object());
}

/*------------------------------------------------------------------------*/

json_t *charp2json(char *array, unsigned count)
{
	return (json_string(array));
}

/*------------------------------------------------------------------------*/

char char4json(json_t *i)
{
	return (json_integer_value(i));
}

/*------------------------------------------------------------------------*/

void charp4json(json_t *json, char *array, unsigned count)
{
	strncpy(array, json_string_value(json), count - 1);
	array[count - 1] = 0;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

json_t *time_t2json(time_t value)
{
	return (int2json(value));
}

/*------------------------------------------------------------------------*/

time_t time_t4json(json_t *json)
{
	return (json_integer_value(json));
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

json_t *double2json(double value)
{
	return (json_real(value));
}

/*------------------------------------------------------------------------*/

double double4json(json_t *json)
{
	return (json_real_value(json));
}
