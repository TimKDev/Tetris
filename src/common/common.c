#include "common.h"
#include <stdio.h>
#include <stdlib.h>

void malloc_check(void *malloc_result)
{
    if (malloc_result == NULL)
    {
        printf("Memory allocation failed.");
        exit(1);
    }
}