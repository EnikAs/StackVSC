#ifndef CALCFUNC_H_INCLUDED
#define CALCFUNC_H_INCLUDED

#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include "stackfunc.h"

extern FILE* log_file;  // да, глобалка, да плохо, а что поделать

enum correct_check
{
    INCORRECTT
};

enum end_of_file
{
    END_OF_FILE,
    NOT_END_OF_FILE
};

struct Commands
{
    char* command;
    int lenght;

};

enum commands
{
    CTOR = 23,
    PUSH,
    POP,
    MUL,
    DIV,
    SUB,
    AFF,
    OUT,
    INCORRECT_INPUT
};

struct buffer 
{
    char* buffer;
    int string_cunt = 0;
    int tmp_string_cunt = 0;
    int buffer_size = 0;
    int tmp_pos = 0;
};

int do_all_commands (FILE* file_stream, Stack* stk);

size_t scanf_file_size (FILE* file_stream);

int buffer_init (buffer* buf, FILE* file_stream);

void buf_string_cunt (buffer* buf);

Commands* commands_init (buffer* buf);

int get_all_commands (Commands* com, buffer* buf);

int get_one_command (Commands* com, buffer* buf);

int type_of_command (Commands com);

void case_push (Commands com, Stack* stk);

void case_pop (Commands com, Stack* stk);

void case_mul (Commands com, Stack* stk);

void case_div (Commands com, Stack* stk);

void case_aff (Commands com, Stack* stk);

void case_sub (Commands com, Stack* stk);

void case_out (Commands com, Stack* stk);

int do_one_command (Commands com, Stack* stk);

#endif 