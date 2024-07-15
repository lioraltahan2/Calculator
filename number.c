#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

unsigned int block_size = 9;
unsigned int max_block = 10e8;

typedef struct numNode_t
{
    unsigned int num;       // value
    struct numNode_t *next; // next digit (less significant)
    struct numNode_t *prev; // previous digit (more significant)
} numNode_t;

typedef struct myNum_t
{
    unsigned int length;   // number of blocks in the number
    int sign;              // -1 -> neg number, 1 -> pos number
    struct numNode_t *msd; // most significant digits
    struct numNode_t *lsd; // least significant digits
} myNum_t;

myNum_t *subtraction(myNum_t *a, myNum_t *b);

myNum_t *get_zero()
{
    myNum_t *zero = malloc(sizeof(myNum_t));
    zero->length = 0;
    zero->sign = 1;
    return zero;
}

void itoa(int val, char **buff, int base)
{

    int i = 30;

    for (; val && i; --i, val /= base)

        (*buff)[i] = "0123456789abcdef"[val % base];

    (*buff) += i + 1;
}

void update_length(myNum_t *a)
{
    a->length = 0;
    numNode_t *curr = a->lsd;
    int counter = 0;
    while (curr != NULL)
    {
        counter++;
        if (curr->num != 0)
        {
            a->length = counter;
        }
        curr = curr->next;
    }
    a->msd = a->lsd;
    counter = a->length;
    if (counter == 0)
    {
        a->msd = NULL;
        a->lsd = NULL;
        return;
    }
    while (--counter)
    {
        a->msd = a->msd->next;
    }
    curr = a->msd->next;
    while (curr != NULL)
    {
        numNode_t *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int num_comp(myNum_t *a, myNum_t *b)
{
    update_length(a);
    update_length(b);
    if (a->sign == -1 && b->sign == 1)
    {
        return -1;
    }
    if (b->sign == -1 && a->sign == 1)
    {
        return 1;
    }
    if (a->length > b->length)
    {
        return 1 * (a->sign);
    }
    if (b->length > a->length)
    {
        return -1 * (a->sign);
    }
    if (a->length == 0)
    {
        return 0;
    }
    int ans = 0;
    numNode_t *a_curr = a->msd;
    numNode_t *b_curr = b->msd;
    while (ans == 0 && a_curr != NULL)
    {
        ans = a_curr->num - b_curr->num;
        a_curr = a_curr->prev;
        b_curr = b_curr->prev;
    }
    return ans * a->sign;
}

void num_from_string(char **input, myNum_t *num)
{
    num->lsd = malloc(sizeof(numNode_t));
    num->msd = num->lsd;
    num->sign = 1;
    num->length = 0;
    while (input != NULL && strlen(*input) != 0 && (*input)[0] >= '0' && (*input)[0] <= '9')
    {
        if (num->lsd->next != NULL)
        {
            numNode_t *temp = num->lsd;
            num->lsd->prev = malloc(sizeof(numNode_t));
            num->lsd = num->lsd->prev;
            num->lsd->next = temp;
        }
        num->lsd->num = 0;
        int counter = 0;
        num->length++;
        while (counter < block_size && strlen(*input) != 0 && (*input)[0] >= '0' && (*input)[0] <= '9')
        {
            num->lsd->num *= 10;
            num->lsd->num += (*input)[0] - '0';
            (*input)++;
            counter++;
        }
    }
    update_length(num);
}

void print_num(myNum_t *num)
{
    if (num->length == 0)
    {
        printf("0");
    }
    numNode_t *curr = num->msd;
    char string_num[(num->length) * block_size + 1];
    char block_num[block_size];
    if (num->sign == -1)
    {
        printf("-");
    }
    while (curr != NULL)
    {
        printf("%d", curr->num);
        curr = curr->prev;
    }
}

myNum_t *num_min(myNum_t *a, myNum_t *b)
{
    if (num_comp(a, b) < 0)
    {
        free(b);
        return a;
    }
    free(a);
    return b;
}

myNum_t *num_max(myNum_t *a, myNum_t *b)
{
    if (num_comp(a, b) < 0)
    {
        free(a);
        return b;
    }
    free(b);
    return a;
}

myNum_t *num_abs(myNum_t *a)
{
    a->sign = 1;
    return a;
}

myNum_t *get_moshe()
{
    char *moshe = "1337";
    myNum_t *num = malloc(sizeof(myNum_t));
    num_from_string(&moshe, num);
    return num;
}

myNum_t *get_one()
{
    char *one = "1";
    myNum_t *num = malloc(sizeof(myNum_t));
    num_from_string(&one, num);
    return num;
}

myNum_t *addition(myNum_t *a, myNum_t *b)
{
    if (b->sign == -1)
    {
        b->sign = 1;
        return subtraction(a, b);
    }
    if (a->sign == -1)
    {
        a->sign = 1;
        return subtraction(b, a);
    }
    if (a->length < b->length)
    {
        return addition(b, a);
    }
    // now we can assume both numbers are positive! and length(a) >= length(b)
    numNode_t *curr_a = a->lsd;
    numNode_t *curr_b = b->lsd;
    numNode_t *prev = NULL;
    myNum_t *ans = malloc(sizeof(myNum_t));
    ans->length = 0;
    int leftover = 0;
    while (curr_a != NULL && curr_b != NULL)
    {
        ans->length++;
        ans->msd = malloc(sizeof(numNode_t));
        ans->msd->num = (curr_a->num + curr_b->num + leftover) % max_block;
        leftover = (curr_a->num + curr_b->num + leftover) / max_block;
        if (prev == NULL)
        {
            ans->lsd = ans->msd;
        }
        else
        {
            prev->next = ans->msd;
            ans->msd->prev = prev;
        }
        prev = ans->lsd;
        curr_a = curr_a->next;
        curr_b = curr_b->next;
    }
    while (curr_a != NULL)
    {
        ans->length++;
        ans->msd = malloc(sizeof(numNode_t));
        ans->msd->num = (curr_a->num + leftover) % max_block;
        leftover = (curr_a->num + leftover) / max_block;
        if (prev == NULL)
        {
            ans->lsd = ans->msd;
        }
        else
        {
            prev->next = ans->lsd;
            ans->msd->prev = prev;
        }
        prev = ans->msd;
        curr_a = curr_a->next;
    }
    if (leftover != 0)
    {
        ans->msd = malloc(sizeof(numNode_t));
        ans->msd->num = (leftover);
        if (prev == NULL)
        {
            ans->lsd = ans->msd;
        }
        else
        {
            prev->next = ans->msd;
            ans->msd->prev = prev;
        }
    }
    update_length(ans);
    return ans;
}

myNum_t *subtraction(myNum_t *a, myNum_t *b)
{
    if (b->sign == -1)
    {
        b->sign = 1;
        return addition(a, b);
    }
    if (a->sign == -1)
    {
        b->sign = 1;
        myNum_t *num = addition(a, b);
        num->sign = -1;
        return num;
    }
    if (num_comp(a, b) < 0)
    {
        myNum_t *num = subtraction(b, a);
        num->sign = -1;
        return num;
    }
    myNum_t *ans = malloc(sizeof(myNum_t));
    numNode_t *curr_a = a->lsd;
    numNode_t *curr_b = b->lsd;
    ans->sign = 1;
    while (curr_a != NULL && curr_b != NULL)
    {
        if (curr_b->num > curr_a->num)
        {
            numNode_t *temp = curr_a->next;
            while (temp->num == 0)
            {
                temp = temp->next;
            }
            temp->num--;
            temp = temp->prev;
            while (temp != curr_a)
            {
                temp->num += max_block - 1;
                temp = temp->prev;
            }
            temp->num += max_block;
        }
        numNode_t *prev = ans->msd;
        ans->msd = malloc(sizeof(numNode_t));
        ans->msd->num = curr_a->num - curr_b->num;
        ans->length++;
        curr_a = curr_a->next;
        curr_b = curr_b->next;
        if (prev == NULL)
        {
            ans->lsd = ans->msd;
        }
        else
        {
            prev->next = ans->msd;
            ans->msd->prev = prev;
        }
    }
    while (curr_a != NULL)
    {
        numNode_t *prev = ans->msd;
        ans->msd = malloc(sizeof(numNode_t));
        ans->msd->num = curr_a->num;
        ans->length++;
        prev->next = ans->msd;
        ans->msd->prev = prev;
    }
    update_length(ans);
    return ans;
}

myNum_t *multipication(myNum_t *a, myNum_t *b)
{
    myNum_t *zero = get_zero();
    myNum_t *one = get_one();
    myNum_t *ans = get_zero();
    while (num_comp(b, zero) > 0)
    {
        ans = addition(ans, a);
        b = subtraction(b, one);
    }

    return ans;
}

myNum_t *division(myNum_t *a, myNum_t *b)
{
    myNum_t *ans = get_zero();
    myNum_t *zero = get_zero();
    myNum_t *one = get_one();
    while (num_comp(a, zero) > 0)
    {
        a = subtraction(a, b);
        ans = addition(ans, one);
    }
    free(one);
    free(zero);
    return ans;
}

myNum_t *power(myNum_t *a, myNum_t *b)
{
    myNum_t *zero = get_zero();
    myNum_t *one = get_one();
    myNum_t *ans = get_one();
    while (num_comp(b, zero) > 0)
    {
        ans = multipication(ans, a);
        b = subtraction(b, one);
    }
    free(one);
    free(zero);
    return ans;
}
