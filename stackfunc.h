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


typedef int elem;

extern FILE* log_file;  //Äà, ãëîáàëüíàÿ ïåðåìåííàÿ, íî áåç íåå êîä óæàñíî íåêðàñèâûé))))

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

struct Stack
{

    uint64_t canary_left = CANARY_CONST;
    elem* data;
    int size_of_stack;
    int capacity;
    bool if_destructed = false;
    uint64_t canary_right = CANARY_CONST;

};

void              data_ptr_fix                       (Stack* stk, int key);

int               StackCtor                          (Stack* stk, int capacity);

int               StackOKCheck                       (Stack* stk);

void              StackDump                          (Stack* stk, const int str_num, const char* func_name, const char* file_name );

void              StackDtor                          (Stack* stk);

int               StackReSize                        (Stack* stk, float multiple_const);

int               check_canary                       (Stack* stk, uint64_t* tmp_can_l, uint64_t* tmp_can_r);

int               StackPush                          (Stack* stk, elem value);

elem              StackPop                           (Stack* stk);

#endif // STACKFUNC_H_INCLUDED