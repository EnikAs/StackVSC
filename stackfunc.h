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

const int ERROR = 0;

const int POISON666 = 0xDEADBEEF;

const int CONST_FOR_MR_DANIIL = 2;

enum for_memory_allocation_check
{
    CORRECT,
    INCORRECT,
    MEMORY_LEAK
};

struct Stack
{
    elem* data;

    int size_of_stack;
    int capacity;
    bool if_destructed = false;

};

int StackCtor(Stack* stk, int capacity);

int StackOKCheck (const Stack* stk);

void StackDump (const Stack* stk, const int str_num, const char* func_name, const char* file_name );

void StackDtor (Stack* stk);

int StackReSize (Stack* stk);

int StackPush (Stack* stk, elem value);

elem StackPop (Stack* stk);

#endif // STACKFUNC_H_INCLUDED