#include "stackfunc.h"

using namespace std;

int main()
{
    Stack stk = {};
    StackCtor(&stk, 100);

    for (int i = 500 ; i > 0 ; i--)
    {
        StackPush(&stk, i);
    }

    for (int i = 250 ; i > 0 ; i--)
    {
        StackPop(&stk);
    }

   // stk.data[123] = 228322;
    $StackDump(&stk);

    StackDtor (&stk);

    return 0;
}