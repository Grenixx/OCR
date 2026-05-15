#include "stack.h"

struct stack *stack_push(struct stack *s, void *e)
{
    struct stack *new = malloc(sizeof(struct stack));
    new->next = NULL;
    new->data = e;
    if (s == NULL)
        return new;
    new->next = s;
    return new;
}

struct stack *stack_pop(struct stack *s)
{
    if (!s)
        return NULL;
    struct stack *next = s->next;
    free(s);
    return next;
}

void stack_destroy(struct stack **s)
{
    if (!s)
        return;
    while (*s)
    {
        *s = stack_pop(*s);
    }
}

void *stack_peek(struct stack *s)
{
    return s->data;
}