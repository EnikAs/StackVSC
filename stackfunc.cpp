#include "stackfunc.h"

FILE* log_file = fopen("log.txt", "w");

void data_ptr_fix (Stack* stk, int key)
{
    if (key == MINUS)
    {
        stk->data = (elem_t*)( (char*)stk->data - sizeof(uint64_t) );
    }
    else if (key == PLUS)
    {
        stk->data = (elem_t*)( (char*)stk->data + sizeof(uint64_t) );
    }
    else
    { 
        printf("INCORRECT KEY, PROGRAMM WILL DO STRANGE THINGS !!!! ");
        stk->errors = BAD_KEY_DATA_FIX;
        $StackDump(stk);
    }
}

 int StackCtor (Stack* stk, int capacity)
{
    stk->data = (elem_t*) realloc(stk->data, ( sizeof(elem_t) * capacity + 2 * sizeof(uint64_t) ));
    //stk->data = (elem_t*) realloc(stk->data, sizeof(elem_t) * capacity );
    *(uint64_t*)stk->data = CANARY_CONST;
    data_ptr_fix(stk, PLUS);   
    memset(stk->data, POISON666, capacity * sizeof(elem_t));    
    *(uint64_t*)(stk->data + capacity) = CANARY_CONST;
    stk->capacity = capacity;

    $murmurhash_for_stack(stk, REPLACE);
    $murmurhash_for_data(stk, REPLACE);
    $StackOKCheck(stk);
    return CORRECT;
}

int StackReSize (Stack* stk, float multiple_const)
{
    $murmurhash_for_data(stk, CHECK);
    $murmurhash_for_stack(stk, CHECK);
    stk->capacity = stk->capacity * multiple_const;
    data_ptr_fix(stk, MINUS);   
    stk->data = (elem_t*) realloc(stk->data, (sizeof(elem_t) * stk->capacity + 2 * sizeof(uint64_t)) );
    *(uint64_t*)stk->data = CANARY_CONST;
    data_ptr_fix(stk, PLUS);   
    *((uint64_t*)(stk->data + stk->capacity)) = CANARY_CONST;

    $murmurhash_for_data(stk, REPLACE);
    $murmurhash_for_stack(stk, REPLACE);
    $StackOKCheck(stk);
    return CORRECT;
}

int StackPush (Stack* stk, elem_t value)
{
    $StackOKCheck(stk);
    $murmurhash_for_data(stk, CHECK);
    $murmurhash_for_stack(stk, CHECK);
    if (stk->size_of_stack == stk->capacity)
    {
        StackReSize(stk, CONST_FOR_MR_DANIIL);
        $StackOKCheck(stk);
    }

    stk->data[stk->size_of_stack++] = value;
    //* ((elem_t*) ( (char*)stk->data + (stk->size_of_stack++) * sizeof(elem_t) ) ) = value;

    $murmurhash_for_data(stk, REPLACE);
    $murmurhash_for_stack(stk, REPLACE);
    $StackOKCheck(stk);
    return 0;
}


elem_t StackPop (Stack* stk)
{
    $StackOKCheck(stk);
    $murmurhash_for_data(stk, CHECK);
    $murmurhash_for_stack(stk, CHECK);
    if (stk->size_of_stack - 1  == stk->capacity / 3 )
    {
        StackReSize(stk, RESIZE_2_3);
        $StackOKCheck(stk);
    }
    if (stk->size_of_stack == 0)
    {
        stk->size_of_stack -= 1;
        $StackOKCheck(stk);
    }
    elem_t tmp = stk->data[stk->size_of_stack - 1];

    if (tmp == POISON666)
    {
        stk->errors = POISONED_VAL;
        $StackDump(stk);
    }

    $StackOKCheck(stk);

    (stk->data[--stk->size_of_stack]) = POISON666;
   
    $murmurhash_for_data(stk, REPLACE);
    $murmurhash_for_stack(stk, REPLACE);
    $StackOKCheck(stk);
    return tmp;

}

void StackDtor (Stack* stk)
{
    $StackOKCheck(stk);
    stk->size_of_stack = 0;
    stk->capacity = 0;

    data_ptr_fix(stk, MINUS);
    free(stk->data);
    free(stk);
    stk->if_destructed = true;
    (stk->data) = (elem_t*)POISON1488;
}

void StackDump (Stack* stk, const int str_num, const char* func_name, const char* file_name)
{
    if (stk->if_destructed == true)
    {
        $printf("STACK IS DESTRUCTED !!!");
        assert(ERROR && "STACK IS DESTRUCTED!!!");
    }
    uint64_t left_data_canary = *((uint64_t*)((char*)stk->data - sizeof(uint64_t))); 
    uint64_t right_data_canary = *((uint64_t*)(stk->data + stk->capacity));
    //check_canary(stk, left_data_canary, right_data_canary);
    //left_data_canary = ((uint64_t*)((char*)stk->data - sizeof(uint64_t)));

    $printf ("\nIn file %s, in function %s on line %d\n", file_name, func_name, str_num);
    if (stk)
    {
        $printf("\n-------------------------------------\n\n");
        $printf("Stack got some problems)))\n");
        switch (stk->errors)
        {
            case STACK_OVERFLOW:
                $printf("Error: STACK OVERFLOW\n");
                break;
            case BAD_DATA_PTR:
                $printf("Error: BAD DATA POINTER\n");
                break;
            case STACK_IS_DESTRUCTED:
                $printf("Error: STACK IS DESTRUCTED\n");
                break;
            case L_CANARY_ERROR:
                $printf("Error: LEFT CANARY IS DEAD\n");
                break;
            case R_CANRY_ERROR:
                $printf("Error: RIGHT CANARY IS DEAD\n");
                break;
            case L_DATA_CANARY_ERROR:
                $printf("Error: LEFT DATA CANARY IS DEAD\n");
                break;
            case R_DATA_CANARY_ERROR:
                $printf("Error: RIGHT DATA CANARY IS DEAD\n");
                break;
            case POISONED_VAL:
                $printf("Error: USAGE OF POISONED VALUE\n");
                break;
            case STACK_SIZE_EQ_ZERO:
                $printf("Error: STACK SIZE BELOW ZERO\n");
                break;
            case HASH_ERROR:
                $printf("Error: STACK (NOT DATA IN STACK) HASH ERROR\n");
                break;
            case DATA_HASH_ERROR:
                $printf("Error: DATA HASH ERROR\n");
                break;
            case BAD_KEY_HASH:
                $printf("Error: INCORRECT KEY OF USING HASH FUNCTIONS\n");
                break;
            case BAD_KEY_DATA_FIX:
                $printf("Error: INCORRECT KEY OF USING DATA FIX FUNCTIONS\n");
                break;                
            default:
                $printf("Error: CONGRATULATIONS ! (THERE IS NO ERRORS !!!)\n");
                break;
        }
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

       

        if (stk->data)
        {
        $printf("left data canary = %ju | ptr = %d ", left_data_canary, (uint64_t*)((char*)stk->data - sizeof(uint64_t)));
        if (left_data_canary == CANARY_CONST)
            $printf(" --- OK !!!\n");
        else   
            $printf("BAD CANARY, DATA IS BROKEN\n");

        for (int i = 0 ; i < stk->capacity ; i++)
        {
            if (stk->data[i] == POISON666)
            {
                $printf("data[%d] = POISON666 ", i);
                $printf("| ptr = %d\n", &stk->data[i]); 
            }
            else
            {
            $printf("data[%d] = %d ", i, stk->data[i]);
            $printf("| ptr = %d\n", &stk->data[i]);
            }        
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

    //StackDtor(stk);
    assert(ERROR && "Bad stk");

}

int StackOKCheck (Stack* stk)
{

    
    if (stk->if_destructed == true)
    { 
        stk->errors = STACK_IS_DESTRUCTED;
        $StackDump(stk);
    }

    if (stk->size_of_stack == 0 && stk->capacity == 0)
        return CORRECT;

    if (stk == NULL)
    {
        $StackDump(stk);
    }

    $murmurhash_for_data(stk, CHECK);
    $murmurhash_for_stack(stk, CHECK);

    if (stk->errors != 0)
        $StackDump(stk);

    if (stk->size_of_stack < 0 )
    {
        stk->errors = STACK_SIZE_EQ_ZERO;
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
    tmp_can_l = ((uint64_t*)((char*)stk->data - sizeof(uint64_t)) );
    tmp_can_r = ( (uint64_t*)( (char*)stk->data + stk->capacity * sizeof(elem_t) ) );
    if (*tmp_can_l != CANARY_CONST)
        return L_DATA_CANARY_ERROR;
    if (*tmp_can_r != CANARY_CONST)
        return R_DATA_CANARY_ERROR;
    else
        return CORRECT;
}
unsigned int murmurHash (char * key, unsigned int len)
{
  const unsigned int m = 0x5bd1e995;
  const unsigned int seed = 0;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;
  unsigned int k;

  while (len >= 4)
  {
      k  = data[0];
      k |= data[1] << 8;
      k |= data[2] << 16;
      k |= data[3] << 24;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      len -= 4;
  }

  switch (len)
  {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

int murmurhash_for_stack (Stack* stk, int key)
{
    unsigned int hash = 0;
    hash += murmurHash((char*)&stk->capacity, sizeof(int) );
    hash += murmurHash((char*)&stk->size_of_stack, sizeof(int) );
    hash += murmurHash((char*)&stk->if_destructed, sizeof(bool) );
    hash += murmurHash((char*)&stk->errors, sizeof(int) );

    if (key == CHECK && stk->hash_stk == hash)
        return CORRECT;
    else if (key == CHECK && stk->hash_stk != hash)
    {
        stk->errors = HASH_ERROR;
        $StackDump(stk);
        return HASH_ERROR;
    }
    else if (key == REPLACE)
    {
        stk->hash_stk = hash;
    }
    else
    {
        stk->errors = BAD_KEY_HASH;
        $StackDump(stk);
        return BAD_KEY_HASH;
    }     
}

int murmurhash_for_data (Stack* stk, int key)
{
    unsigned int hash = 0;
    hash = murmurHash((char*)stk->data, stk->capacity * sizeof(elem_t));

    if (key == CHECK && stk->hash_data == hash)
        return CORRECT;
    else if (key == CHECK && stk->hash_data != hash)
    {
        stk->errors = DATA_HASH_ERROR;
        $StackDump(stk);
        return DATA_HASH_ERROR;
    }
    else if (key == REPLACE)
    {
        stk->hash_data = hash;
    }
    else
    {
        stk->errors = BAD_KEY_HASH;
        $StackDump(stk);
        return BAD_KEY_HASH;
    }    
}