#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MAX_RESOURCES 32

//__attribute__ is not from standard C and is only avaliable in gcc and clang compiler.
#define INIT_RESOURCE_STACK(stack_name)                               \
    __attribute__((cleanup(stack_cleanup))) ResourceStack stack_name; \
    stack_init(&stack_name)

#define ADD_RESOURCE(stack_name, resource, cleanup) \
    stack_push(&stack_name, (void *)resource, (CleanupFunc)cleanup)

typedef void (*CleanupFunc)(void *);

typedef struct
{
    void *resource;
    CleanupFunc cleanup;
} Resource;

typedef struct
{
    Resource resources[MAX_RESOURCES];
    size_t count;
} ResourceStack;

void stack_init(ResourceStack *stack);
bool stack_push(ResourceStack *stack, void *resource, CleanupFunc cleanup);
void stack_cleanup(ResourceStack *stackPointer);

