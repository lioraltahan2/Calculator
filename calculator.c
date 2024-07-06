#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.c"
#include <math.h>

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

node_t *parse_expression(char *input)
{
    node_t *head = malloc(sizeof(node_t));
    head->data = malloc(sizeof(token_t));
    node_t *current = head;
    node_t *last = NULL;
    while (parse_token(&input, current->data))
    {
        current->next = malloc(sizeof(node_t));
        last = current;
        current = current->next;
        current->data = malloc(sizeof(token_t));
    }
    if (last != NULL)
    {
        free(last->next);
        last->next = NULL;
    }
    return head;
}

void operator_handler(char op, numStack_t *stack)
{
    int b = stack_pop(stack);
    int a = stack_pop(stack);
    switch (op)
    {
    case '+':
        stack_push(stack, a + b);
        return;
    case '-':
        stack_push(stack, a - b);
        return;
    case '/':
        stack_push(stack, a / b);
        return;
    case '*':
        stack_push(stack, a * b);
        return;
    case '^':
        stack_push(stack, pow(a, b));
        return;
    }
}

void function_handler(char *func, numStack_t *stack)
{
    if (strcmp(func, "MAX") == 0)
    {
        stack_push(stack, max(stack_pop(stack), stack_pop(stack)));
    }
    if (strcmp(func, "MIN") == 0)
    {
        stack_push(stack, min(stack_pop(stack), stack_pop(stack)));
    }
    if (strcmp(func, "ABS") == 0)
    {
        stack_push(stack, abs(stack_pop(stack)));
    }
    if (strcmp(func, "MOSHE") == 0)
    {
        stack_push(stack, 1337);
    }
}

int evaluate_rpn(node_t *expression)
{
    node_t *current = expression;
    numStack_t *stack = malloc(sizeof(numStack_t));
    while (current != NULL)
    {
        if (current->data->func != NULL)
        {
            function_handler(current->data->func, stack);
        }
        if (current->data->op != NULL_OP)
        {
            operator_handler(current->data->op, stack);
        }
        if (current->data->num != NULL_NUM)
        {
            stack_push(stack, current->data->num);
        }
        current = current->next;
    }
    return stack_pop(stack);
}

int main(int argc, char *argv[])
{
    node_t *head = parse_expression("4 18 9 3 - / +");
    int x = evaluate_rpn(head);
    printf("%d", x);
}