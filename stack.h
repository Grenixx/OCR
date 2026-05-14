#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct t_point
{
    int x;
    int y;
};

struct stack
{
    struct t_point *point;
    struct stack *next;
};

/* Push a new element to the stack and returns the new top */
struct stack *stack_push(struct stack *s, struct t_point element);

/* Pop from the stack and returns its new top */
struct stack *stack_pop(struct stack *s);

/* Empty and frees the whole stack */
void stack_destroy(struct stack **s);

/* Returns the current top of the stack */
struct t_point stack_peek(struct stack *s);

#endif /* ! STACK_H */