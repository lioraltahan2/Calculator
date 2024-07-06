#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "shunting_yard.c"

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
    int b = num_stack_pop(stack);
    int a = num_stack_pop(stack);
    switch (op)
    {
    case '+':
        num_stack_push(stack, a + b);
        return;
    case '-':
        num_stack_push(stack, a - b);
        return;
    case '/':
        num_stack_push(stack, a / b);
        return;
    case '*':
        num_stack_push(stack, a * b);
        return;
    case '^':
        num_stack_push(stack, pow(a, b));
        return;
    }
}

void function_handler(char *func, numStack_t *stack)
{
    if (strcmp(func, "MAX") == 0)
    {
        num_stack_push(stack, max(num_stack_pop(stack), num_stack_pop(stack)));
    }
    if (strcmp(func, "MIN") == 0)
    {
        num_stack_push(stack, min(num_stack_pop(stack), num_stack_pop(stack)));
    }
    if (strcmp(func, "ABS") == 0)
    {
        num_stack_push(stack, abs(num_stack_pop(stack)));
    }
    if (strcmp(func, "MOSHE") == 0)
    {
        num_stack_push(stack, 1337);
    }
}

int evaluate_rpn(node_t *expression)
{
    node_t *current = expression;
    numStack_t *stack = malloc(sizeof(numStack_t));
    while (current != NULL && current->data != NULL)
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
            num_stack_push(stack, current->data->num);
        }
        current = current->next;
    }
    return num_stack_pop(stack);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Didn't get any expression to calculate");
        return 0;
    }
    node_t *head = parse_expression(argv[1]);
    node_t *last = head;
    while (last->next != NULL)
    {
        last = last->next;
    }
    for (int i = 2; i < argc; i++)
    {
        node_t *curr = parse_expression(argv[i]);
        last->next = curr;
        while (last->next != NULL)
        {
            last = last->next;
        }
    }
    int x = evaluate_rpn(shunting_yard(head));
    printf("%d", x);
}