#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "shunting_yard.c"

void support_neg_numbers(node_t *head)
{
    if (head->data->op == '-')
    {
        node_t *temp = malloc(sizeof(node_t));
        temp->data = malloc(sizeof(token_t));
        temp->data->num = get_zero();
        temp->data->op = NULL_OP;
        temp->next = head;
        head = temp;
    }
    node_t *prev = head->next;
    node_t *curr = prev->next;
    while (curr != NULL)
    {
        if (curr->data->op != '-' || prev->data->num != NULL)
        {
            prev = curr;
            curr = curr->next;
            continue;
        }
        prev->next = malloc(sizeof(node_t));
        prev->next->data = malloc(sizeof(token_t));
        prev = prev->next;
        prev->data->num = NULL;
        prev->data->op = '(';
        prev->next = malloc(sizeof(node_t));
        prev->next->data = malloc(sizeof(token_t));
        prev = prev->next;
        prev->data->num = get_zero();
        prev->data->op = NULL_OP;
        prev->next = curr;
        prev = curr->next;
        curr = curr->next->next;
        prev->next = malloc(sizeof(node_t));
        prev->next->data = malloc(sizeof(token_t));
        prev = prev->next;
        prev->data->num = NULL;
        prev->data->op = ')';
        prev->next = curr;
    }
}

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
    myNum_t *b = num_stack_pop(stack);
    myNum_t *a = num_stack_pop(stack);
    switch (op)
    {
    case '+':
        num_stack_push(stack, addition(a, b));
        free(a);
        free(b);
        return;
    case '-':
        num_stack_push(stack, subtraction(a, b));
        free(a);
        free(b);
        return;
    case '/':
        num_stack_push(stack, division(a, b));
        free(a);
        free(b);
        return;
    case '*':
        num_stack_push(stack, multipication(a, b));
        free(a);
        free(b);
        return;
    case '^':
        num_stack_push(stack, power(a, b));
        free(a);
        free(b);
        return;
    }
}

void function_handler(char *func, numStack_t *stack)
{
    if (strcmp(func, "MAX") == 0)
    {
        num_stack_push(stack, num_max(num_stack_pop(stack), num_stack_pop(stack)));
    }
    if (strcmp(func, "MIN") == 0)
    {
        num_stack_push(stack, num_min(num_stack_pop(stack), num_stack_pop(stack)));
    }
    if (strcmp(func, "ABS") == 0)
    {
        num_stack_push(stack, num_abs(num_stack_pop(stack)));
    }
    if (strcmp(func, "MOSHE") == 0)
    {
        num_stack_push(stack, get_moshe());
    }
}

void evaluate_rpn(node_t *expression)
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
        if (current->data->num != NULL)
        {
            num_stack_push(stack, current->data->num);
        }
        current = current->next;
    }
    print_num(num_stack_pop(stack));
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
    support_neg_numbers(head);
    evaluate_rpn(shunting_yard(head));
}