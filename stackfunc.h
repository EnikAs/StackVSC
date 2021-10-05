#ifndef STACKFUNC_H_INCLUDED
#define STACKFUNC_H_INCLUDED

#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>





#define $StackDump(stk) StackDump(stk, __LINE__, __func__, __FILE__)
#define $printf(...) fprintf(log_file, ##__VA_ARGS__)


typedef int elem_t;

extern FILE* log_file;  // да, глобалка, да плохо, а что поделать !

const uint64_t CANARY_CONST = 0xDED32DED32DED32;

const float RESIZE_2_3 = 2.0 / 3;

const int ERROR = 0;

const int POISON666 = 1488322;

const int CONST_FOR_MR_DANIIL = 2;

enum for_memory_allocation_check
{
    CORRECT,
    INCORRECT,
    MEMORY_LEAK
};

enum for_stk_ptr_fix
{
    MINUS,
    PLUS
};

enum for_StackOk_correct_check
{   
    STACK_OVERFLOW = 228,
    BAD_DATA_PTR,
    STACK_IS_DESTRUCTED,
    L_CANARY_ERROR,
    R_CANRY_ERROR,
    L_DATA_CANARY_ERROR,
    R_DATA_CANARY_ERROR,
    POISONED_VAL,
    STACK_SIZE_EQ_ZERO,
    HASH_ERROR,
    CHECK,
    REPLACE,
    BAD_KEY
};


/*

enum for_hash_сalc
{
    CHECK = 12312,
    REPLACE =121234 ,
    BAD_KEY = 21231
};
*/
struct Stack
{

    uint64_t canary_left = CANARY_CONST;
    elem_t* data;
    unsigned int hash_stk = 0;
    unsigned int hash_data;
    int errors = 0;
    int size_of_stack;
    int capacity;
    bool if_destructed = false;
    uint64_t canary_right = CANARY_CONST;

};

void              data_ptr_fix                       (Stack* stk, int key);

int               StackCtor                          (Stack* stk, int capacity);

int               StackOKCheck                       (Stack* stk);

void              StackDump                          (Stack* stk, const int str_num, const char* func_name, const char* file_name);

void              StackDtor                          (Stack* stk);

int               StackReSize                        (Stack* stk, float multiple_const);

int               check_canary                       (Stack* stk, uint64_t* tmp_can_l, uint64_t* tmp_can_r);

int               StackPush                          (Stack* stk, elem_t value);

elem_t            StackPop                           (Stack* stk);

unsigned int      murmurHash                         (char * key, unsigned int len);

int               murmurhash_for_stack               (Stack* stk, int key);

int               murmurhash_for_data                (Stack* stk, int key);

#endif // STACKFUNC_H_INCLUDED