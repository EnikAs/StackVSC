#include "stackfunc.h"
#include "calcfunc.h"

int main()
{
    
    //printf("asd");
    Stack stk = {};
    StackCtor(&stk, 100);
    extern FILE* log_file;// a >> w 
    FILE* file_for_calc = fopen("calc.txt", "r");
    if (file_for_calc == NULL)
    {
        printf("FILE NOT OPEND!!!");
        return 0;
    }

    printf("StArT\n");

    do_all_commands(file_for_calc, &stk);

    printf ("\n||this means that StackDump gonna start his job||\n");
    $StackDump(&stk);

    StackDtor(&stk);
   
   /*
    for (int i = 500 ; i > 0 ; i--)
    {
        StackPush(&stk, i);
    }

    for (int i = 250 ; i > 0 ; i--)
    {
        StackPop(&stk);
    }
   
    StackPush(&stk, 123);
    StackPush(&stk, 123);
    StackPush(&stk, 123);
    StackPush(&stk, 123);
    StackPush(&stk, 123);
    StackPush(&stk, 123);
    StackPush(&stk, 123);

    $StackDump(&stk);
    StackDtor(&stk);
     */

    return 0;
}