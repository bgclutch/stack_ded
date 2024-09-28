#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>


#include "../stack_headers/stack.h"

#define STACK_DUMP(stack_data) stack_dump(stack_data, __FILE__, __func__, __LINE__) 

StackElem_t My_Empty_Element = '\0';


int ctor_stack(Main_Stack_Struct *stack_data, size_t size)
{
    if(stack_data == nullptr && stack_data->stack_array == nullptr)
        return SMTH_WAS_BROKEN;

    stack_data->stack_array = (char*) calloc(STRUCT_CTOR_SIZE, size); // FIXME check

    if(stack_data->stack_array == nullptr) //hueta check
        return SMTH_WAS_BROKEN;  

    stack_data->capacity = (size_t)STRUCT_CTOR_SIZE; // TODO refactor
    stack_data->size     = 0; // FIXME rename

    for(size_t i = 0; i < stack_data->capacity; i++)
        stack_data->stack_array[i] = My_Empty_Element;

    STACK_DUMP(stack_data);

    return ALL_IS_OK; // TODO return error and remove stack_code_return field
}

int dtor_stack(Main_Stack_Struct *stack_data)
{
    STACK_DUMP(stack_data);

    if(!is_struct_addresses_okay(stack_data))//address and meanings
        return SMTH_WAS_BROKEN;

    memset(stack_data->stack_array, My_Empty_Element, stack_data->size);

    for(size_t i = 0; i < stack_data->size; i++)
    {
        if(stack_data->stack_array[i] != My_Empty_Element)
            return SMTH_WAS_BROKEN;
    }

    stack_data->size = 0;
    stack_data->capacity = 0;
    // FIXME free
    free(stack_data->stack_array); // FIXME check

    if(stack_data->stack_array != nullptr) //FREE STACK
        return SMTH_WAS_BROKEN;
    //FIXME add smth else
    STACK_DUMP(stack_data);

    return ALL_IS_OK;
}


int stack_push(Main_Stack_Struct *stack_data, StackElem_t elem) // FIXME pass elem as argument
{
    STACK_DUMP(stack_data);

    if(!is_struct_addresses_okay(stack_data))
        return SMTH_WAS_BROKEN;

    if(is_arr_incr_need(*stack_data)) // FIXME avoid using macros. Make functions
        realloc_maker(stack_data, 1); //check errors

    stack_data->size++;
    stack_data->stack_array[stack_data->size - 1] = elem; // FIXME cringe
    //FIXME check ptrs & segfaults
    STACK_DUMP(stack_data);

    //return checker;
    return ALL_IS_OK;
}

//FIXME error checking
int stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem) // FIXME return popped value through pointer (not in struct)
{
    STACK_DUMP(stack_data); 

    if(!is_struct_addresses_okay(stack_data))
    {
        return SMTH_WAS_BROKEN;
    }
    // fprintf(stack_data->aboba, "size (printed between dumps!!!): %lu\n", stack_data->size);
    // fprintf(stack_data->aboba, "stack array[%lu]: %c\n",stack_data->size - 1, stack_data->stack_array[stack_data->size - 1]);
    if(stack_data->size == 0) //FIXME fix check
    {
        return SMTH_WAS_BROKEN;
    }
    *elem = stack_data->stack_array[stack_data->size - 1]; 
    fprintf(stack_data->aboba, "popped elem %c\n", *elem);
    stack_data->stack_array[stack_data->size - 1] = My_Empty_Element;  
    (stack_data->size)--;  
    if(is_arr_decr_need(*stack_data)) //realloc
        realloc_maker(stack_data, 2);
    // fprintf(stack_data->aboba, "size (printed after dumps!!!): %lu\n", stack_data->size);
    // fprintf(stack_data->aboba, "stack array[%lu]: %c\n",stack_data->size - 1, stack_data->stack_array[stack_data->size - 1]);
            
    //FIXME add realloc func

    STACK_DUMP(stack_data); // TODO use macros

    //return checker;
    return ALL_IS_OK;
}


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line) //add checks
{
    put_stars(stack_data->aboba);
    fprintf  (stack_data->aboba, "%s %s:%lu\n", file_name, func_name, line);
    put_stars(stack_data->aboba);
    
    fprintf  (stack_data->aboba, "stack address begin: %p\n", stack_data->stack_array);
    fprintf  (stack_data->aboba, "stack address end: %p\n", &stack_data->stack_array[stack_data->capacity]);
    put_stars(stack_data->aboba);

    fprintf  (stack_data->aboba, "stack size: %lu\n",              stack_data->size);
    fprintf  (stack_data->aboba, "stack size address: %p\n",     &(stack_data->size));
    put_stars(stack_data->aboba); 

    fprintf  (stack_data->aboba, "stack capacity: %lu\n",          stack_data->capacity);
    fprintf  (stack_data->aboba, "stack capacity address: %p\n", &(stack_data->capacity));
    put_stars(stack_data->aboba);   

    for(size_t i = 0; i < stack_data->size; i++) //printing NON empty element
    {
        fprintf(stack_data->aboba, "stack element non empty %lu: %c\n",           i, stack_data->stack_array[i]);
        fprintf(stack_data->aboba, "stack element non empty %lu address: %p\n\n", i, &(stack_data->stack_array[i]));
    }
    
    // for(size_t i = stack_data->size; i < stack_data->capacity; i++)//printing empty element
    // {
    //     if(stack_data->size > stack_data->capacity)
    //     {
    //         fprintf(stderr, "PIZDEC %s:%d", __func__, __LINE__);
    //         assert(0);
    //     }
    //     fprintf(stack_data->aboba, "stack element empty %lu: %c\n",           i,  stack_data->stack_array[i]);
    //     fprintf(stack_data->aboba, "stack element empty %lu address: %p\n\n", i, &(stack_data->stack_array[i]));
    // }
    

    // TODO print empty elements
    put_stars(stack_data->aboba);          
     

 
    fprintf(stack_data->aboba, "\n\n\n\n");
    //printf every address meaning etc
}


// int stack_is_err()
// {
//     err checker
// }


int realloc_maker(Main_Stack_Struct *stack_data, int var) // FIXME add checker
{
    if(!is_struct_addresses_okay(stack_data))
        return SMTH_WAS_BROKEN;

    if(var == 1)
    {
        stack_data->stack_array = (char*) realloc(stack_data->stack_array, stack_data->capacity* 2); // FIXME fix check
        stack_data->capacity *= 2;
    }
    else if(var == 2)
    {
        stack_data->stack_array = (char*) realloc(stack_data->stack_array, stack_data->capacity / 2); // FIXME fix check
        stack_data->capacity /= 2;
    }

    if(stack_data->stack_array == nullptr)
        return SMTH_WAS_BROKEN; //TODO add spec error code

    return ALL_IS_OK;
}


void put_stars(FILE* file)
{
    for(int i = 0; i < 30; i++)
    {
        fputc('*', file);
    }
    fputc('\n', file);
}


int is_arr_incr_need(Main_Stack_Struct stack_data)
{
    if(stack_data.capacity - stack_data.size <= 1)
        return 1;
    else
        return 0;

}


int is_arr_decr_need(Main_Stack_Struct stack_data)
{
    if(stack_data.size * 2 <= stack_data.capacity && stack_data.size >= 5)
        return 1;
    else
        return 0;
}


int is_struct_addresses_okay(Main_Stack_Struct *stack_data)
{
    if(stack_data && stack_data->stack_array)
        return 1;
    else
        return 0;
}