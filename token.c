#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "number.c"

int NULL_NUM = -1;
char NULL_OP = '\0';

typedef struct token_t
{
    myNum_t *num;
    char op;
    char *func;
} token_t;

typedef struct node_t
{
    token_t *data;
    struct node_t *next;
} node_t;

bool function_checker(char **input, token_t *token)
{
    int i = 0;
    while (i < strlen(*input) && ((*input)[i] == '_' || ((*input)[i] <= 'Z' && (*input)[i] >= 'A')))
    {
        i++;
    }
    if (i == 0)
    {
        return false;
    }
    char func[i];
    strncpy(func, *input, i);
    (*input) += i;
    token->func = func;
    token->num = NULL;
    token->op = NULL_OP;
    return true;
}

bool operator_checker(char **input, token_t *token)
{
    char options[8] = {'+', '-', '/', '^', '*', '(', ')', ','};
    for (int i = 0; i < 8; i++)
    {
        if ((*input)[0] == options[i])
        {
            token->op = options[i];
            token->num = NULL;
            token->func = NULL;
            (*input)++;
            return true;
        }
    }
    return false;
}

bool int_checker(char **input, token_t *token)
{
    if ((*input)[0] < '0' || (*input)[0] > '9')
    {
        return false;
    }
    token->num = malloc(sizeof(myNum_t));
    token->func = NULL;
    token->op = NULL_OP;
    num_from_string(input, token->num);
    return true;
}

bool parse_token(char **input, token_t *token)
{
    while (isspace(**input))
    {
        (*input)++;
    }
    if (strlen(*input) == 0)
    {
        return false;
    }
    if (operator_checker(input, token) || function_checker(input, token) || int_checker(input, token))
    {
        return true;
    }
    return false;
}
