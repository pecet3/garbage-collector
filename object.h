#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef enum ObjectKind
{
    INTEGER,
    FLOAT,
    STRING,
    VECTOR3,
    ARRAY,
} object_kind_t;

typedef struct Object object_t;

typedef struct Vector3
{
    object_t *x;
    object_t *y;
    object_t *z;
} vector3_t;

typedef struct Array
{
    size_t size;
    object_t **elements;
} array_t;

typedef union ObjectData
{
    int v_int;
    float v_float;
    char *v_string;
    vector3_t v_vector3;
    array_t v_array;
} object_data_t;

typedef struct Object
{
    bool is_marked;
    object_kind_t kind;
    object_data_t data;
} object_t;

void object_free(object_t *obj);
object_t *array_get(object_t *obj, size_t index);