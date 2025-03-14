#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "object.h"
#include "vm.h"

void object_free(object_t *obj)
{
    switch (obj->kind)
    {
    case INTEGER:
    case FLOAT:
        break;
    case STRING:
        free(obj->data.v_string);
        break;
    case VECTOR3:
    {
        break;
    }
    case ARRAY:
    {
        free(obj->data.v_array.elements);
        break;
    }
    }
    free(obj);
}

object_t *new_object(vm_t *vm)
{
    object_t *obj = calloc(1, sizeof(object_t));
    if (obj == NULL)
    {
        return NULL;
    }
    vm_track_object(vm, obj);

    obj->is_marked = false;
    return obj;
}

object_t *new_integer(vm_t *vm, int value)
{
    object_t *obj = new_object(vm);
    if (obj == NULL)
    {
        return NULL;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;

    return obj;
}

object_t *new_float(vm_t *vm, float value)
{
    object_t *obj = new_object(vm);
    if (obj == NULL)
    {
        return NULL;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;

    return obj;
}

object_t *new_string(vm_t *vm, char *value)
{
    if (value == NULL)
    {
        return NULL;
    }
    object_t *obj = new_object(vm);
    if (obj == NULL)
    {
        return NULL;
    }

    obj->kind = STRING;
    obj->data.v_string = malloc(strlen(value) + 1);
    if (obj->data.v_string == NULL)
    {
        return NULL;
    }
    strcpy(obj->data.v_string, value);

    return obj;
}

object_t *new_vector3(vm_t *vm, object_t *x, object_t *y, object_t *z)
{
    if (x == NULL || y == NULL || z == NULL)
    {
        return NULL;
    }

    object_t *obj = new_object(vm);
    if (obj == NULL)
    {
        return NULL;
    }

    obj->kind = VECTOR3;

    refcount_inc(x);
    refcount_inc(y);
    refcount_inc(z);

    obj->data.v_vector3.x = x;
    obj->data.v_vector3.y = y;
    obj->data.v_vector3.z = z;

    return obj;
}

// array

object_t *add(vm_t *vm, object_t *a, object_t *b)
{
    if (a == NULL || b == NULL)
    {
        return NULL;
    }

    switch (a->kind)
    {
    case INTEGER:
        switch (b->kind)
        {
        case INTEGER:
            return new_integer(vm, a->data.v_int + b->data.v_int);
        case FLOAT:
            return new_float(vm, (float)a->data.v_int + b->data.v_float);
        default:
            return NULL;
        }
    case FLOAT:
        switch (b->kind)
        {
        case INTEGER:
            return new_integer(vm, a->data.v_int + (float)b->data.v_int);
        case FLOAT:
            return new_float(vm, a->data.v_int + b->data.v_float);
        default:
            return NULL;
        }
    case STRING:
    {
        if (b->kind != STRING)
            return NULL;
        int length = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
        char *newstring = calloc(sizeof(char *), length);
        strcat(newstring, a->data.v_string);
        strcat(newstring, b->data.v_string);
        object_t *obj = new_string(vm, newstring);
        free(newstring);
        return obj;
    }
    case VECTOR3:
    {
        if (b->kind != VECTOR3)
            return NULL;
        return new_vector3(vm,
                           add(vm, a->data.v_vector3.x, b->data.v_vector3.x),
                           add(vm, a->data.v_vector3.y, b->data.v_vector3.y),
                           add(vm, a->data.v_vector3.z, b->data.v_vector3.z));
    }
    case ARRAY:
    {
        if (b->kind != ARRAY)
            return NULL;
        int lenght = a->data.v_array.size + b->data.v_array.size;
        object_t *obj = new_array(vm, lenght);
        for (size_t i = 0; i < a->data.v_array.size; i++)
        {
            array_set(obj, i, array_get(a, i));
        }
        for (size_t i = 0; i < b->data.v_array.size; i++)
        {
            array_set(obj, a->data.v_array.size + i, array_get(b, i));
        }
        return obj;
    }
    }
}

int get_length(object_t *obj)
{
    if (obj == NULL)
    {
        return -1;
    }

    switch (obj->kind)
    {
    case INTEGER:
        return 1;
    case FLOAT:
        return 1;
    case STRING:
        return strlen(obj->data.v_string);
    case VECTOR3:
        return 3;
    case ARRAY:
        return obj->data.v_array.size;
    default:
        return -1;
    }
}

object_t *array_get(object_t *obj, size_t index)
{
    if (obj == NULL)
    {
        return NULL;
    }
    if (obj->kind != ARRAY)
    {
        return NULL;
    }
    if (obj->data.v_array.size <= index)
    {
        return NULL;
    }

    return obj->data.v_array.elements[index];
}

bool array_set(object_t *obj, size_t index, object_t *value)
{
    if (obj == NULL || value == NULL)
    {
        return false;
    }
    if (obj->kind != ARRAY)
    {
        return false;
    }
    if (obj->data.v_array.size <= index)
    {
        return false;
    }
    refcount_inc(value);
    if (obj->data.v_array.elements[index] != NULL)
    {
        refcount_dec(obj->data.v_array.elements[index]);
    }
    obj->data.v_array.elements[index] = value;
    return true;
}

object_t *new_array(vm_t *vm, size_t size)
{
    object_t *obj = new_object(vm);
    if (obj == NULL)
    {
        return NULL;
    }
    obj->kind = ARRAY;

    object_t **elements = calloc(size, sizeof(object_t *));
    if (elements == NULL)
    {
        return NULL;
    }

    array_t array = {.size = size, .elements = elements};

    obj->data.v_array = array;

    return obj;
}