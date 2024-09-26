#include <stdio.h>
#include <math.h>

#include "stack.h"

int ctor_stack(Main_Stack_Struct *stack_data)
{
    stack_data->stack_array = (char*) calloc(FIRST_TIME_STRUCT_CTOR, stack_data->data_type_size);
    stack_data->capacity    = (size_t)FIRST_TIME_STRUCT_CTOR;
    stack_data->counter     = 0;
    stack_data->stack_code_return = ALL_IS_OK;
}

int dtor_stack(Main_Stack_Struct *stack_data)
{
    if()//address and meanings
    free(stack_data->stack_array);
    //FIXME add smth else
}


int stack_push(Main_Stack_Struct *stack_data)
{
    //FIXME check ptrs & segfaults
    stack_data->counter++;
    if()//for realloc


    stack_data->stack_array[stack_data->counter - 1] = stack_data->stack_elem; 

    //return checker;
}

int stack_pop(Main_Stack_Struct *stack_data)
{
    stack_data->stack_array[stack_data-> counter - 1] = NULL;

    //struct decrementation

    if() //realloc
    //sreturn checker;
}


void_sex stack_dump()
{
    //printf every address meaning etc
}

int stack_is_err()
{
    //err checker
}