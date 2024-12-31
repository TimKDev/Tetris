#include "resource-stack.h"

void stack_init(ResourceStack *stack)
{
    stack->count = 0;
}

bool stack_push(ResourceStack *stack, void *resource, CleanupFunc cleanup)
{
    // In general if the resource is null, it is not nessary to clean up
    if (stack->count >= MAX_RESOURCES || resource == NULL)
    {
        return false;
    }

    stack->resources[stack->count].resource = resource;
    stack->resources[stack->count].cleanup = cleanup;
    stack->count++;
    return true;
}

void stack_cleanup(ResourceStack *stack)
{
    while (stack->count > 0)
    {
        stack->count--;
        if (stack->resources[stack->count].cleanup)
        {
            stack->resources[stack->count].cleanup(
                stack->resources[stack->count].resource);
        }
    }
}