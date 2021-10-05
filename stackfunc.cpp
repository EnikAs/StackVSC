#include "stackfunc.h"

FILE* log_file = fopen("log.txt", "w");

void data_ptr_fix (Stack* stk, int key)
{
    if (key == MINUS)
    {
        stk->data = (elem*)( (char*)stk->data - sizeof(uint64_t) );
    }
    else if (key == PLUS)
    {
        stk->data = (elem*)( (char*)stk->data + sizeof(uint64_t) );
    }
    else
    { 
        printf("INCORRECT KEY, PROGRAMM WILL DO STRANGE THINGS !!!! ");
        $StackDump(stk);
    }
}

 int StackCtor (Stack* stk, int capacity)
{
    stk->data = (elem*) realloc(stk->data, ( sizeof(elem) * capacity + 2 * sizeof(uint64_t) ));
    //stk->data = (elem*) realloc(stk->data, sizeof(elem) * capacity );
    *(uint64_t*)stk->data = CANARY_CONST;
    data_ptr_fix(stk, PLUS);   
    memset(stk->data, POISON666, capacity * sizeof(elem));    
    *(uint64_t*)(stk->data + capacity) = CANARY_CONST;
    stk->capacity = capacity;

    StackOKCheck(stk);
    return CORRECT;
}

int StackReSize (Stack* stk, float multiple_const)
{
    stk->capacity = stk->capacity * multiple_const;
    $printf("capacity2 = %d\n", stk->capacity);
    data_ptr_fix(stk, MINUS);   
    stk->data = (elem*) realloc(stk->data, (sizeof(elem) * stk->capacity + 2 * sizeof(uint64_t)) );
    *(uint64_t*)stk->data = CANARY_CONST;
    data_ptr_fix(stk, PLUS);   
    *((uint64_t*)(stk->data + stk->capacity)) = CANARY_CONST;
    StackOKCheck(stk);
    return CORRECT;
}

int StackPush (Stack* stk, elem value)
{
    if (stk->size_of_stack == stk->capacity)
    {
        StackReSize(stk, CONST_FOR_MR_DANIIL);
        StackOKCheck(stk);
    }

    stk->data[stk->size_of_stack++] = value;
    //* ((elem*) ( (char*)stk->data + (stk->size_of_stack++) * sizeof(elem) ) ) = value;
    return 0;
}


elem StackPop (Stack* stk)
{
    if (stk->size_of_stack - 1  == stk->capacity / 3 )
    {
        StackReSize(stk, RESIZE_2_3);
        StackOKCheck(stk);
    }
    elem tmp = stk->data[stk->size_of_stack - 1];

/*
    if (tmp == POISON666)
        $StackDump(stk);
    (stk->data[--stk->size_of_stack]) = POISON666;
*/   
    return tmp;

}

void StackDtor (Stack* stk)
{
    stk->size_of_stack = 0;
    stk->capacity = 0;

    data_ptr_fix(stk, MINUS);
    free(stk->data);
    free(stk);
    stk->if_destructed = true;
    (stk->data) = (elem*)POISON666;
}

void StackDump (Stack* stk, const int str_num, const char* func_name, const char* file_name)
{
    uint64_t left_data_canary = *((uint64_t*)((char*)stk->data - sizeof(uint64_t))); 
    uint64_t right_data_canary = *((uint64_t*)(stk->data + stk->capacity));
    //check_canary(stk, left_data_canary, right_data_canary);
    //left_data_canary = ((uint64_t*)((char*)stk->data - sizeof(uint64_t)));

    $printf ("\nIn file %s, in function %s on line %d\n", file_name, func_name, str_num);
    if (stk)
    {
        $printf("\n-------------------------------------\n\n");
        $printf("Stack got some problems)))\n");
        $printf("Stack error code = %d\n", stk->errors);
        $printf("Stack is destructed = ");
        $printf(stk->if_destructed ? "true" : "false");
        $printf("\nleft canary = %ju | ptr = %d", stk->canary_left, &stk->canary_left);
        if (stk->canary_left == CANARY_CONST)
            $printf(" --- OK !!!\n");
        else   
            $printf("BAD CANARY, DATA IS BROKEN\n");

        $printf("size = %d\n", stk->size_of_stack);
        $printf("capacity = %d\n", stk->capacity);

        $printf("right canary = %ju | ptr = %d", stk->canary_right, &(stk->canary_right));
        if (stk->canary_right == CANARY_CONST)
            $printf(" --- OK !!!\n");
        else   
            $printf("BAD CANARY, DATA IS BROKEN\n");

        $printf("\n-----------------------\n\n");

        $printf("left data canary = %ju | ptr = %d ", left_data_canary, (uint64_t*)((char*)stk->data - sizeof(uint64_t)));
        if (left_data_canary == CANARY_CONST)
            $printf(" --- OK !!!\n");
        else   
            $printf("BAD CANARY, DATA IS BROKEN\n");
        //$printf("data[%d] = %ju \n", -1, *((uint64_t*)((char*)stk->data - sizeof(uint64_t))) );
        if (stk->data)
        {
        for (int i = 0 ; i < stk->capacity ; i++)
        {
            $printf("data[%d] = %d ", i, stk->data[i]);
            $printf("| ptr = %d\n", &stk->data[i]);
        }
        
        $printf("right data canary = %ju | ptr = %d", right_data_canary,  (uint64_t*)(stk->data + stk->capacity) );
        if (right_data_canary == CANARY_CONST)
            $printf(" --- OK !!!\n");
        else   
            $printf("BAD CANARY, DATA IS BROKEN\n");
        }
        else   
            $printf("BAD DATA PTR!!!!");
    }
    else
    {
        $printf("stk = NULL!!!");
    }

   assert(ERROR && "Bad stk");

}

int StackOKCheck (Stack* stk)
{
    if (stk->size_of_stack == 0 && stk->capacity == 0)
        return CORRECT;

    if (stk == NULL)
    {
        $StackDump(stk);
    }

    if (stk->capacity < stk->size_of_stack)
    {   
        stk->errors = STACK_OVERFLOW;
        $StackDump(stk);
    }
    
    if (stk->data == NULL)
    {
        stk->errors = BAD_DATA_PTR;
        $StackDump(stk);
    }
       
    if (stk->if_destructed == true)
    {
        stk->errors = STACK_IS_DESTRUCTED;
        $StackDump(stk);
    }
    
    if (stk->canary_left != CANARY_CONST)
    {
        stk->errors = L_CANARY_ERROR;
        $StackDump(stk);
    }
    
    if (stk->canary_right != CANARY_CONST)
    {
        stk->errors = R_CANRY_ERROR;
        $StackDump(stk);
    }
    
    uint64_t left_data_canary = 0, right_data_canary = 0;
    int tmp_canary_checker = check_canary(stk, &left_data_canary, &right_data_canary);

    if (tmp_canary_checker == L_DATA_CANARY_ERROR)
    {
        stk->errors = L_DATA_CANARY_ERROR;
        $StackDump(stk);
    }
    
    if (tmp_canary_checker == R_DATA_CANARY_ERROR)
    {
        stk->errors = R_DATA_CANARY_ERROR;
        $StackDump(stk);
    }
    
    return CORRECT;
}

int check_canary (Stack* stk, uint64_t* tmp_can_l, uint64_t* tmp_can_r)
{
    //data_ptr_fix(stk, MINUS);
    tmp_can_l = ((uint64_t*)((char*)stk->data - sizeof(uint64_t)) );
    //data_ptr_fix(stk, PLUS);
    tmp_can_r = ( (uint64_t*)( (char*)stk->data + stk->capacity * sizeof(elem) ) );
    if (*tmp_can_l != CANARY_CONST)
        return L_DATA_CANARY_ERROR; // TODO ERRORS !!!
    if (*tmp_can_r != CANARY_CONST)
        return R_DATA_CANARY_ERROR;
    else
        return CORRECT;
}