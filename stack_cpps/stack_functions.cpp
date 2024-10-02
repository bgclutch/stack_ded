#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>


#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


//#define BREAK_MODE
#define STACK_DUMP(stack_data) stack_dump(stack_data, __FILE__, __func__, __LINE__) 
#define RETURN_ERROR(error_sum) return_error(error_sum, __FILE__, __func__, __LINE__)


const long canary_value  = 0x71EBCEE;                         //FIXME long long to long
const long canary_offset = sizeof(long) / sizeof(StackElem_t);
const size_t Struct_Ctor_Size = 4;
StackElem_t My_Empty_Element  = '\0';



Error_Codes ctor_stack(Main_Stack_Struct *stack_data)
{
    assert(stack_data && "stack data address is NULL");         // FIXME assert or make disableable

    stack_data->extended_arr = (char*) calloc(Struct_Ctor_Size * sizeof(StackElem_t) +
                                             2 * canary_offset, sizeof(char));

    fprintf(stderr, "extended arr address:%p\n\n", stack_data->extended_arr);

    if(stack_data->extended_arr == nullptr) //hueta check
        return SMTH_WAS_BROKEN;  

 
    stack_data->stack_array = (stack_data->extended_arr + canary_offset); // TODO make 1 pointer instead of 2
    
    stack_data->capacity = Struct_Ctor_Size;
    stack_data->size     = 0;


    stack_data->left_st_canary  = canary_value;
    stack_data->right_st_canary = canary_value;


    for(long i = 0; i < stack_data->capacity; i++)
        stack_data->stack_array[i] = My_Empty_Element;

    put_canaries(stack_data);
    
    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

Error_Codes dtor_stack(Main_Stack_Struct *stack_data)
{
    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data);

    if(!is_struct_addresses_okay(stack_data))//address and meanings
        return SMTH_WAS_BROKEN;

    memset(stack_data->stack_array, My_Empty_Element, (size_t)stack_data->size); // FIXME memset is only for bytes

    stack_data->size            = 0;
    stack_data->capacity        = 0;
    stack_data->left_st_canary  = 0;
    stack_data->right_st_canary = 0;

    free(stack_data->extended_arr); 
    stack_data->extended_arr = nullptr;

    return ALL_IS_OK;
}


Error_Codes stack_push(Main_Stack_Struct *stack_data, StackElem_t elem)
{
    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data);

    realloc_maker(stack_data, realloc_if_up_needed(*stack_data)); // FIXME check errors

    stack_data->stack_array[stack_data->size++] = elem;

    #ifdef BREAK_MODE
    {
        stack_data->left_st_canary = 0;
        stack_data->size = -4;
        stack_data->capacity = -10;
        stack_data->aboba = nullptr;
        stack_data->stack_array = nullptr;
        stack_data->right_st_canary = 8874187721;
        
    }
    #endif

    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

//FIXME error checking
Error_Codes stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem)
{
    assert(elem);
    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data); 
    
    if(stack_data->size == 0)
    {
        return SMTH_WAS_BROKEN;
    }

    *elem = stack_data->stack_array[--stack_data->size]; 
    fprintf(stack_data->aboba, "popped elem %c\n", *elem);

    stack_data->stack_array[stack_data->size] = My_Empty_Element;  

    realloc_maker(stack_data, realloc_if_down_needed(*stack_data)); // FIXME check error

    RETURN_ERROR(stack_is_err(stack_data));
    STACK_DUMP(stack_data); // TODO use macros
    return ALL_IS_OK;
}


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line) //add checks
{
    RETURN_ERROR(stack_is_err(stack_data));

    put_stars(stack_data->aboba);
    fprintf  (stack_data->aboba, "%s %s:%lu\n", file_name, func_name, line);
    put_stars(stack_data->aboba);
    
    fprintf  (stack_data->aboba, "stack address begin: %p\n",     stack_data->stack_array);
    fprintf  (stack_data->aboba, "stack address end: %p\n",      &stack_data->stack_array[stack_data->capacity]);
    put_stars(stack_data->aboba);
 
    fprintf  (stack_data->aboba, "stack size: %ld\n",             stack_data->size);
    fprintf  (stack_data->aboba, "stack size address: %p\n",     &(stack_data->size));
    put_stars(stack_data->aboba); 

    fprintf  (stack_data->aboba, "stack capacity: %ld\n",         stack_data->capacity);
    fprintf  (stack_data->aboba, "stack capacity address: %p\n", &(stack_data->capacity));
    put_stars(stack_data->aboba);   

    for(long i = 0; i < stack_data->size; i++) //printing NON empty element
    {
        fprintf(stack_data->aboba, "stack element non empty %ld: %c\n",           i, stack_data->stack_array[i]);
        fprintf(stack_data->aboba, "stack element non empty %ld address: %p\n\n", i, &(stack_data->stack_array[i]));
    }
    
    for(long i = stack_data->size; i < stack_data->capacity; i++)//printing empty element
    {
        fprintf(stack_data->aboba, "stack element empty %ld: %c\n",           i,  stack_data->stack_array[i]);
        fprintf(stack_data->aboba, "stack element empty %ld address: %p\n\n", i, &(stack_data->stack_array[i]));
    }
    

    // TODO print empty elements
    put_stars(stack_data->aboba);          
     

 
    fprintf(stack_data->aboba, "\n\n\n\n");
    //printf every address meaning etc
    RETURN_ERROR(stack_is_err(stack_data));
}


size_t stack_is_err(Main_Stack_Struct *stack_data) // FIXME CHANGE ZALUPA
{
    size_t errors_sum = 0;

    if(!is_struct_addresses_okay(stack_data))   errors_sum += 0x02;
    if(stack_data->capacity <= 0)               errors_sum += 0x04;
    if(stack_data->size < 0)                    errors_sum += 0x08;
    if(stack_data->size > stack_data->capacity) errors_sum += 0x0F;
    if(stack_data->aboba == nullptr)            errors_sum += 0x10;

    return errors_sum;
}


Error_Codes realloc_maker(Main_Stack_Struct *stack_data, int scale_coef) // FIXME add checker
{
    RETURN_ERROR(stack_is_err(stack_data));

    if(scale_coef <= 0) //???
    {
        return SMTH_WAS_BROKEN; 
    }

    long new_capacity = (scale_coef == 2) ? stack_data->capacity * 2 : stack_data->capacity / 4;


    StackElem_t *new_array = (StackElem_t*) realloc(stack_data->extended_arr, (size_t)(new_capacity + 2 * canary_offset)); // FIXME fix check

    stack_data->capacity     = new_capacity;
    stack_data->extended_arr  = new_array;


    if(scale_coef == 2)
        memset(&new_array[stack_data->size], My_Empty_Element, (size_t)(new_capacity - stack_data->size + canary_offset) * sizeof(StackElem_t));

    stack_data->stack_array = (stack_data->stack_array + canary_offset);

    fprintf(stderr, "end of extended array %p\n\n", &stack_data->extended_arr[stack_data->capacity + 2 * canary_offset]);

    stack_data->stack_array = stack_data->extended_arr + canary_offset;

    put_canaries(stack_data);
    
    RETURN_ERROR(stack_is_err(stack_data));
    return ALL_IS_OK;
}


void put_stars(FILE* file)
{
    for(int i = 0; i < 50; i++)
    {
        fputc('*', file);
    }
    fputc('\n', file);
}


int realloc_if_up_needed(Main_Stack_Struct stack_data)
{

    if(stack_data.capacity - stack_data.size <= 1)
        return 2;
    else
        return 0;

}


int realloc_if_down_needed(Main_Stack_Struct stack_data)
{
    if(4 <= stack_data.size && stack_data.size * 4 <= stack_data.capacity)
        return 4;
    else
        return 0;
}


Error_Codes return_error(size_t err_code, const char* file, const char* func, int line)
{
    if(err_code != 0)
    {
        fprintf(stderr, RED_TEXT("ERROR DETECTED IN ") MAGENTA_TEXT("%s:") BLUE_TEXT("%s:") GREEN_TEXT("%d\n")
                        RED_TEXT("ERROR SUM = ") GREEN_TEXT("%lu\n"), file, func, line, err_code);

        fprintf(stderr, MAGENTA_TEXT("\nERROR CODES:\n")
                        YELLOW_TEXT("STRUCT_ADDRESS_IS_BAD     =  ")   RED_TEXT("2\n")
                        YELLOW_TEXT("NEG_CAPACITY              =  ")   RED_TEXT("4\n")
                        YELLOW_TEXT("NEG_SIZE                  =  ")   RED_TEXT("8\n")
                        YELLOW_TEXT("SIZE_BIGGER_THAN_CAPACITY = ")   RED_TEXT("16\n")
                        YELLOW_TEXT("FILE_PTR_IS_ZERO          = ")   RED_TEXT("32\n\n"));

        assert(0); // FIXME
    }
    else
    {
        return ALL_IS_OK;
    }
}


int is_struct_addresses_okay(Main_Stack_Struct *stack_data)
{
    if(stack_data && stack_data->stack_array)
        return 1;
    else
        return 0;
}


Error_Codes put_canaries(Main_Stack_Struct *stack_data)
{
    //RETURN_ERROR(stack_is_err(stack_data));

    *((long*)stack_data->extended_arr) = canary_value;

    *((long*)(stack_data->extended_arr + +canary_offset + stack_data->capacity)) = canary_value;

    fprintf(stderr, "left canary address %p && value 0x%ldX\n"
                    "right canary address %p && value 0x%ldX"
                    "stack array begin address %p && stack array end address %p\n\n", (stack_data->stack_array - canary_offset),
                     *(long*)(stack_data->stack_array + stack_data->capacity), stack_data->stack_array, *(long*)(stack_data->stack_array + stack_data->capacity));

    RETURN_ERROR(stack_is_err(stack_data));
    return ALL_IS_OK;
}




//TODO canary_t (d_type) cond compile I WAS RIGHT ABOUT CANARIES 
//TODO make poison instead of '\0' check 'free' poison all in debug
//TODO push && pop - above 2 strings on release
//TODO get data_type from console  
//TODO poison before canaries (?)
//TODO check poison by address && meaning(?) (all stack)
//TODO memcpy???????
//TODO hash through control sum than real hash (non cryptographic) (hash = hash * 33 xor(current byte)) 
//TODO  
//TODO  
//TODO   
//TODO  
//TODO  
//TODO 
//TODO  
//TODO  
//TODO 
//TODO  
//TODO  
//TODO 