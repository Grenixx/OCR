#include "stack.h"

struct stack *stack_push(struct stack *s, struct t_point *p)
{
    struct stack *new = malloc(sizeof(struct stack));
    new->next = NULL;
    new->point = p;
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
        free((*s)->point);
        *s = stack_pop(*s);
    }
}

struct t_point *stack_peek(struct stack *s)
{
    return s->point;
}