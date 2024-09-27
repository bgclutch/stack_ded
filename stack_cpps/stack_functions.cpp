#include <stdio.h>
#include <math.h>
#include <string.h>


#include "../stack_headers/stack.h"

#define ADDRESSES_CHECKING stack_data && stack_data->stack_array
#define NEED_CAPACITY_INCR stack_data->counter  - stack_data->capacity == 1
#define NEED_CAPACITY_DECR stack_data->counter * (size_t)REALLOC_COEF ==  stack_data->capacity


int ctor_stack(Main_Stack_Struct *stack_data)
{
    stack_data->stack_array = (char*) calloc(STRUCT_CTOR_SIZE, stack_data->data_type_size); // FIXME check
    stack_data->capacity    = (size_t)STRUCT_CTOR_SIZE;
    stack_data->capacity_coefficient = 1; // TODO refactor
    stack_data->counter     = 0; // FIXME rename
    stack_data->stack_code_return = ALL_IS_OK;
    stack_dump(stack_data, "ctor_stack");

    return 1; // TODO return error and remove stack_code_return field
}

int dtor_stack(Main_Stack_Struct *stack_data)
{
    stack_dump(stack_data, "dtor_stack");

    if(ADDRESSES_CHECKING)//address and meanings
    {
        memset(stack_data->stack_array, '\0', stack_data->counter);
        stack_data->counter = 0;
        stack_data->capacity = (size_t)STRUCT_CTOR_SIZE;
        // FIXME free
        stack_data->stack_array = (char*) realloc(stack_data->stack_array, stack_data->capacity); // FIXME check
    }
    //FIXME add smth else
    stack_dump(stack_data, "dtor_stack");

    return 1;
}


int stack_push(Main_Stack_Struct *stack_data) // FIXME pass elem as argument
{
    stack_dump(stack_data, "stack_push");

    if(NEED_CAPACITY_INCR) // FIXME avoid using macros. Make functions
    {
        stack_data->capacity_coefficient ++;
        realloc_maker(stack_data);
    }

    if(ADDRESSES_CHECKING)//for realloc
    {
        stack_data->counter++;
        stack_data->stack_array[stack_data->counter - 1] = (char)stack_data->stack_elem; // FIXME cringe
    }
    //FIXME check ptrs & segfaults
    stack_dump(stack_data, "stack_push");

    //return checker;
    return 1;
}

//FIXME error checking
int stack_pop(Main_Stack_Struct *stack_data) // FIXME return popped value through pointer (not in struct)
{
    stack_dump(stack_data, "stack_pop"); 

    if (ADDRESSES_CHECKING)
    {
        fprintf(stack_data->aboba, "counter (printed between dumps!!!): %lu\n", stack_data->counter);
        fprintf(stack_data->aboba, "stack array[%lu]: %c\n",stack_data->counter - 1, stack_data->stack_array[stack_data->counter - 1]);
        if(stack_data->counter != 1) // FIMXE fix check
        {
            stack_data->stack_array[stack_data->counter - 1] = '\0';
            stack_data->counter--;   
        }
        else {    
            stack_data->stack_array[stack_data->counter - 1] = '\0';
        }
        fprintf(stack_data->aboba, "counter (printed after dumps!!!): %lu\n", stack_data->counter);
        fprintf(stack_data->aboba, "stack array[%lu]: %c\n",stack_data->counter - 1, stack_data->stack_array[stack_data->counter - 1]);
             
    }
    
    // if(NEED_CAPACITY_DECR) //realloc
    // {
    //     stack_data->capacity_coefficient -= 2;
    //     realloc_maker(stack_data);
    // }

    //FIXME add realloc func

    stack_dump(stack_data, "stack_pop"); // TODO use macros

    //return checker;
    return 1;
}


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* call_func_name)
{
    put_stars(stack_data->aboba);
    fprintf  (stack_data->aboba, "%s\n", call_func_name);
    put_stars(stack_data->aboba);
    
    fprintf  (stack_data->aboba, "stack address: %p\n", stack_data->stack_array);
    for(size_t i = 0; i < stack_data->counter; i++)
    {
        fprintf(stack_data->aboba, "stack element %lu: %c\n", i, stack_data->stack_array[i]);
        fprintf(stack_data->aboba, "stack element %lu address: %p\n", i, &(stack_data->stack_array[i]));
    }

    // TODO print empty elements
    put_stars(stack_data->aboba);

    fprintf  (stack_data->aboba, "stack elem push/pop: %c\n",                  (char)stack_data->stack_elem);
    fprintf  (stack_data->aboba, "stack elem push/pop address: %p\n",         &(stack_data->stack_elem));
    put_stars(stack_data->aboba);          
     
    fprintf  (stack_data->aboba, "stack counter: %lu\n",                        stack_data->counter);
    fprintf  (stack_data->aboba, "stack counter address: %p\n",               &(stack_data->counter));
    put_stars(stack_data->aboba);          
     
    fprintf  (stack_data->aboba, "stack capacity: %lu\n",                       stack_data->capacity);
    fprintf  (stack_data->aboba, "stack capacity address: %p\n",              &(stack_data->capacity));
    put_stars(stack_data->aboba);          
 
    fprintf  (stack_data->aboba, "stack capacity coefficient: %lu\n",          stack_data->capacity_coefficient);
    fprintf  (stack_data->aboba, "stack capacity coefficient address: %p\n",  &(stack_data->capacity_coefficient));
    put_stars(stack_data->aboba);
 
    fprintf  (stack_data->aboba, "stack code return: %d\n",                   stack_data->stack_code_return);
    fprintf  (stack_data->aboba, "stack code return address: %p\n",           &(stack_data->stack_code_return));

    put_stars(stack_data->aboba);
    fprintf(stack_data->aboba, "\n\n\n\n");
    //printf every address meaning etc
}


// int stack_is_err()
// {
//     //err checker
// }


int realloc_maker(Main_Stack_Struct *stack_data)
{
    stack_data->stack_array = (char*) realloc(stack_data->stack_array,
                                              stack_data->capacity_coefficient * (size_t)STRUCT_CTOR_SIZE); // FIXME fix check

    return ADDRESSES_CHECKING ? (int)ALL_IS_OK : (int)SMTH_WAS_BROKEN;
}


void put_stars(FILE* file)
{
    for(int i = 0; i < 50; i++)
    {
        fputc('*', file);
    }
    fputc('\n', file);
}