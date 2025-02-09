#include <stdlib.h>
#include "object.h"

object_t *new_integer(int value)
{
    object_t *obj = malloc(sizeof(object_t));
    if (obj == NULL)
    {
        return;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;
}

object_t *new_float(float value)
{
    object_t *obj = malloc(sizeof(object_t));
    if (obj == NULL)
    {
        return;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;
}