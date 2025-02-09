#pragma once

typedef enum ObjectKind
{
    INTEGER,
    FLOAT,
} object_kind_t;

typedef union ObjectData
{
    int v_int;
    float v_float;
} object_data_t;

typedef struct Object
{
    object_kind_t kind;
    object_data_t data;
} object_t;