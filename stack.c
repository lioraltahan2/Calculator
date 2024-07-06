#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.c"

typedef struct numStackNode_t
{
    int value;
    struct numStackNode_t *next;
} numStackNode_t;

typedef struct numStack_t
{
    struct numStackNode_t *head;
} numStack_t;

void stack_push(numStack_t *stack, int num)
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

int stack_pop(numStack_t *stack)
{
    int val = stack->head->value;
    numStackNode_t *new_head = stack->head->next;
    stack->head = new_head;
    return val;
}