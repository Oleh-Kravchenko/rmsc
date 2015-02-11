#include <string.h>
#include <jansson.h>

/*------------------------------------------------------------------------*/

json_t *char2json(char *c)
{
	return (json_object());
}

/*------------------------------------------------------------------------*/

json_t *int2json(int i)
{
	return (json_integer(i));
}

/*------------------------------------------------------------------------*/

char char4json(json_t *i)
{
	return (json_integer_value(i));
}

/*------------------------------------------------------------------------*/

int int4json(json_t *i)
{
	return (json_integer_value(i));
}

/*------------------------------------------------------------------------*/

json_t *intp2json(int *array, unsigned count)
{
	json_t *json;

	if (!(json = json_array())) {
		return (NULL);
	}

	for (unsigned i = 0; i < count; ++ i) {
		json_t *item;

		if (!(item = json_integer(array[i]))) {
			goto err;
		}

		if (json_array_append_new(json, item)) {
			goto err;
		}
	}

	return (json);

err:
	json_decref(json);

	return (NULL);
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

json_t *charp2json(char *array, unsigned count)
{
	return (json_string(array));
}

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

void charp4json(json_t *json, char *array, unsigned count)
{
	strncpy(array, json_string_value(json), count - 1);
	array[count - 1] = 0;
}
