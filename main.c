#include <stdio.h>
#include <stdlib.h>
#include "object.h"
int main()
{
    printf("hello world\n");
    object_t *a = new_integer(6767);

    return 0;
}