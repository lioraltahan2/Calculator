#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.c"
#include "stack.c"

node_t *shunting_yard(node_t *infix_expression)
{
    node_t *exp_current = infix_expression;
    tokenStack_t *operators_stack = malloc(sizeof(tokenStack_t));
    node_t *output_head = malloc(sizeof(node_t));
    node_t *output_current = output_head;
    while (exp_current != NULL)
    {
        token_t *token = exp_current->data;
        if (token->num != NULL_NUM)
        {
            output_current->data = token;
            output_current->next = malloc(sizeof(node_t));
            output_current = output_current->next;
            exp_current = exp_current->next;
            continue;
        }
        if (token->func != NULL || token->op == '(')
        {
            token_stack_push(operators_stack, token);
            exp_current = exp_current->next;
            continue;
        }
        if (token->op == ')')
        {
            while (1)
            {
                token = token_stack_pop(operators_stack);
                if (token->op == '(')
                {
                    break;
                }
                output_current->data = token;
                output_current->next = malloc(sizeof(node_t));
                output_current = output_current->next;
                continue;
            }
            exp_current = exp_current->next;
            continue;
        }
        char curr_op = token->op;
        char top_op = NULL_OP;
        if (operators_stack->head != NULL)
        {
            top_op = operators_stack->head->value->op;
        }
        while (top_op != NULL_OP && top_op != '(' && (curr_op - top_op >= 0 || top_op == '/' && curr_op != '^'))
        {
            token_t *top_token = token_stack_pop(operators_stack);
            output_current->data = top_token;
            output_current->next = malloc(sizeof(node_t));
            output_current = output_current->next;
            if (operators_stack->head == NULL)
            {
                break;
            }
            top_op = operators_stack->head->value->op;
        }
        token_stack_push(operators_stack, token);
        exp_current = exp_current->next;
    }
    while (operators_stack->head != NULL)
    {
        token_t *token = token_stack_pop(operators_stack);
        output_current->data = token;
        output_current->next = malloc(sizeof(node_t));
        output_current = output_current->next;
    }
    return output_head;
}