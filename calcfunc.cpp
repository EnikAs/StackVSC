#include "calcfunc.h"
extern FILE* log_file;// a >> w 


int do_all_commands (FILE* file_stream, Stack* stk)
{
    buffer buf = {};
    buffer_init(&buf, file_stream);
    Commands* com = commands_init(&buf);
    get_all_commands(com, &buf);
    for (int i = 0 ; i < buf.tmp_string_cunt ; i++)
    {   
        do_one_command(com[i], stk);
    }

    return 0;
}

size_t scanf_file_size (FILE* file_stream)
{
    fseek(file_stream, 0, SEEK_END);
    size_t size_of_file = ftell(file_stream);
    fseek(file_stream, 0, SEEK_SET);//rewind

    return size_of_file;
}

int buffer_init (buffer* buf, FILE* file_stream)
{
    buf->buffer_size = scanf_file_size (file_stream);
    buf->buffer = (char*) calloc(buf->buffer_size, sizeof(char));
    buf->buffer_size = fread(buf->buffer, sizeof(char), buf->buffer_size, file_stream);

    buf_string_cunt(buf);
    
    return 0;
}

void buf_string_cunt (buffer* buf)
{
    size_t string_cunt = 0;
    for (int i = 0 ; i < buf->buffer_size ; i++)
    {
        if (buf->buffer[i] == '\n' && buf->buffer[i])
        {
            buf->string_cunt += 1;
            buf->buffer[i] = '\0';       
        }
    }

}

Commands* commands_init (buffer* buf)
{
    Commands* com = (Commands*) calloc(buf->string_cunt + 1, sizeof(com));// "+ 1" для того, чтобы сделать пустую структуру, как '\0'
    if (com == NULL)
    {
        printf("NOT ENOUGH MEMORY");
        return NULL;
    }
    return com;
}

int get_all_commands (Commands* com, buffer* buf)
{
    int end_check = -1;
    while (end_check != END_OF_FILE)
    {
        end_check = get_one_command(com, buf);
    }
    return 0;
}

int get_one_command (Commands* com, buffer* buf)
{
    com[buf->tmp_string_cunt].command = ((char*)buf->buffer + buf->tmp_pos);
    com[buf->tmp_string_cunt].lenght = strlen(com[buf->tmp_string_cunt].command);

    while (buf->buffer[buf->tmp_pos] != '\0')
    {
        buf->tmp_pos++;
    } 
    buf->tmp_pos++;
    buf->tmp_string_cunt++;
    
    if (buf->tmp_pos > buf->buffer_size)
        return END_OF_FILE;
    return -1;
}

int type_of_command (Commands com)
{
    printf("%s|", com.command);
    if (strncmp(com.command, "pus", 3) == 0)
    {
        printf("\nIt is gonna be push\n");
        return PUSH;
    }
    if (strncmp(com.command, "mul", 3) == 0)
        return MUL;

    if (strncmp(com.command, "pop", 3) == 0)
        return POP;

    if (strncmp(com.command, "sub", 3) == 0)
        return SUB;

    if (strncmp(com.command, "div", 3) == 0)
        return DIV;

    if (strncmp(com.command, "out", 3) == 0)
        return OUT;

    if (strncmp(com.command, "aff", 3) == 0)
        return AFF;

    else
        return INCORRECT_INPUT;
}
elem_t get_int_from_com (Commands com)
{
    elem_t tmp_int = 0;
    for(int i = 0 ; i < com.lenght ; i++)
    {
        if (com.command[i] <= '9' && com.command[i] >= '0')
        {
            tmp_int *= 10;
            tmp_int += com.command[i] - '0';
        }
    }
    return tmp_int;
}
void case_push(Commands com, Stack* stk)
{
    elem_t tmp_int = get_int_from_com(com);  
    StackPush(stk, tmp_int);
}

void case_pop(Commands com, Stack* stk)
{
    StackPop(stk);
}

void case_mul(Commands com, Stack* stk)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp1 = StackPop(stk);
    tmp2 = StackPop(stk);
    StackPush(stk, tmp1 * tmp2);
}

void case_div(Commands com, Stack* stk)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp2 = StackPop(stk);
    tmp1 = StackPop(stk);// ВОПРОС О ТОМ, ЧТО НА ЧТО НУЖНО ДЕЛИТЬ (ПЕРВЫЙ ЧЛЕН НА ВТОРОЙ ИЛИ НАОБОРОТ!!!) 
    StackPush(stk, tmp1 / tmp2);
}

void case_aff(Commands com, Stack* stk)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp1 = StackPop(stk);
    tmp2 = StackPop(stk);
    StackPush(stk, tmp1 + tmp2);
}

void case_sub(Commands com, Stack* stk)
{
    elem_t tmp1 = 0, tmp2 = 0;
    tmp2 = StackPop(stk);
    tmp1 = StackPop(stk);// ВОПРОС О ТОМ, ЧТО ИЗ ЧЕГО НУЖНО ВЫЧИТАТЬ!!! 
    StackPush(stk, tmp1 - tmp2);
}

void case_out(Commands com, Stack* stk)
{
    StackDtor(stk);
}

int do_one_command (Commands com, Stack* stk)
{
    switch(type_of_command(com))
    {
        case PUSH:
                    printf(" suka! ");
                    case_push(com, stk);
                    break;
        case MUL:
                    case_mul(com, stk);                    
                    break;
        case POP:
                    case_pop(com, stk);        
                    break;
        case SUB:
                    case_sub(com, stk);        
                    break;
        case DIV:
                    case_div(com, stk);        
                    break;
        case OUT:
                    case_out(com, stk);        
                    break;
        case AFF:
                    case_aff(com, stk);        
                    break; 
        case INCORRECT_INPUT:
                    printf(" GOVNO ");                   
    }
    return 0;
}