#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct numStackNode_t
{
    int value;
    struct numStackNode_t *next;
} numStackNode_t;

typedef struct numStack_t
{
    struct numStackNode_t *head;
} numStack_t;

typedef struct tokenStackNode_t
{
    token_t *value;
    struct tokenStackNode_t *next;
} tokenStackNode_t;

typedef struct tokenStack_t
{
    struct tokenStackNode_t *head;
} tokenStack_t;

void num_stack_push(numStack_t *stack, int num)
{
    if (stack == NULL)
    {
        stack->head = malloc(sizeof(numStackNode_t));
        stack->head->value = num;
        return;
    }
    numStackNode_t *old_head = stack->head;
    stack->head = malloc(sizeof(numStackNode_t));
    stack->head->value = num;
    stack->head->next = old_head;
}

int num_stack_pop(numStack_t *stack)
{
    int val = stack->head->value;
    numStackNode_t *new_head = stack->head->next;
    stack->head = new_head;
    return val;
}

void token_stack_push(tokenStack_t *stack, token_t *token)
{
    if (stack == NULL)
    {
        stack->head = malloc(sizeof(tokenStackNode_t));
        stack->head->value = token;
        return;
    }
    tokenStackNode_t *old_head = stack->head;
    stack->head = malloc(sizeof(tokenStackNode_t));
    stack->head->value = token;
    stack->head->next = old_head;
}

token_t *token_stack_pop(tokenStack_t *stack)
{
    token_t *val = stack->head->value;
    tokenStackNode_t *new_head = stack->head->next;
    stack->head = new_head;
    return val;
}
