#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>


#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


//#define BREAK_MODE

#define STACK_DUMP(stack_data)  stack_dump( stack_data, __FILE__, __func__, __LINE__) 


const uint64_t canary_value  = 0x71EBCEE; //dumayte // FIXME long long to long
const size_t canary_offset = sizeof(long) / sizeof(StackElem_t);
const size_t Struct_Ctor_Size =    4;
StackElem_t My_Empty_Element  = '\0';



Error_Codes ctor_stack(Main_Stack_Struct *stack_data)
{
    ASSERT(stack_data && "stack data address is NULL") TEMP_VAR(;)      // FIXME assert or make disableable

    stack_data->stack_array = (char*) calloc(Struct_Ctor_Size * sizeof(StackElem_t) +
                                             2 * canary_offset, sizeof(char));

    if(stack_data->stack_array == nullptr) //hueta check
        return STACK_ARRAY_ADDRESS_IS_BAD; // TODO make 1 pointer instead of 2
    
    stack_data->capacity = Struct_Ctor_Size;
    stack_data->size     = 0;


    stack_data->left_st_canary  = canary_value;
    stack_data->right_st_canary = canary_value;


    for(size_t i = canary_offset; i < stack_data->capacity; i++)
        stack_data->stack_array[i] = My_Empty_Element;

    stack_data->hash_struct = hash_sum(*stack_data);

    TEMP_VAR(Error_Codes put_canaries_code = ) put_canaries(stack_data);

    ASSERT(!put_canaries_code && "put canaries error") TEMP_VAR(;) 

    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data));
    
    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

Error_Codes dtor_stack(Main_Stack_Struct *stack_data)
{
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    STACK_DUMP(stack_data);

    if(!is_struct_addresses_okay(stack_data)) //address and meanings
        return STACK_ARRAY_ADDRESS_IS_BAD;

    memset(stack_data->stack_array + canary_offset, My_Empty_Element, stack_data->capacity); // FIXME memset is only for bytes

    free(stack_data->stack_array); 
    stack_data->stack_array = nullptr;
    ASSERT(!stack_data->stack_array && "stack array != nullptr") TEMP_VAR(;) 

    return ALL_IS_OK;
}


Error_Codes stack_push(Main_Stack_Struct *stack_data, StackElem_t elem)
{
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    //STACK_DUMP(stack_data);

    stack_data->hash_struct = hash_sum(*stack_data);

    TEMP_VAR(Error_Codes realloc_code =) realloc_maker(stack_data, realloc_if_up_needed(*stack_data));

    ASSERT(!realloc_code && "realloc error") TEMP_VAR(;)  // FIXME check errors

    stack_data->stack_array[canary_offset + stack_data->size++] = elem;


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

    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(my_hash = hash_sum(*stack_data));

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;)
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

//FIXME error checking
Error_Codes stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem)
{
    ASSERT(elem) TEMP_VAR(;) 
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    //STACK_DUMP(stack_data); 
    
    if(stack_data->size == 0)
    {
        return EMPTY_STACK;
    }

    *elem = stack_data->stack_array[canary_offset + (--stack_data->size)]; 

    put_stars(stack_data->dump_file);
    fprintf(stack_data->dump_file, "popped elem '%c'\n", *elem);

    stack_data->stack_array[canary_offset + stack_data->size] = My_Empty_Element;  

    stack_data->hash_struct = hash_sum(*stack_data);

    TEMP_VAR(Error_Codes realloc_code =) realloc_maker(stack_data, realloc_if_up_needed(*stack_data));
    ASSERT(!realloc_code && "realloc error") TEMP_VAR(;) 

    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    STACK_DUMP(stack_data); // TODO use macros
    return ALL_IS_OK;
}


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line) //add checks
{
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 

    put_stars(stack_data->dump_file);
    fprintf  (stack_data->dump_file, "%s %s:%lu\n", file_name, func_name, line);
    put_stars(stack_data->dump_file);
    
    fprintf(stack_data->dump_file, "left struct canary: 0x%0lx\n",      stack_data->left_st_canary);
    fprintf(stack_data->dump_file, "left struct canary address: %p\n", &stack_data->left_st_canary);
    put_stars(stack_data->dump_file);

    fprintf  (stack_data->dump_file, "stack size: %zu\n",                stack_data->size);
    fprintf  (stack_data->dump_file, "stack size address: %p\n",       &(stack_data->size));
    put_stars(stack_data->dump_file);   
  
    fprintf  (stack_data->dump_file, "stack capacity: %zu\n",            stack_data->capacity);
    fprintf  (stack_data->dump_file, "stack capacity address: %p\n",   &(stack_data->capacity));
    put_stars(stack_data->dump_file);   

    fprintf  (stack_data->dump_file, "stack address begin: %p\n",       stack_data->stack_array);
    fprintf  (stack_data->dump_file, "stack address end: %p\n",        &stack_data->stack_array[stack_data->capacity]);
    put_stars(stack_data->dump_file);

    fprintf(stack_data->dump_file, "left arr canary: 0x%0lx\n",   *(uint64_t*)(stack_data->stack_array));
    fprintf(stack_data->dump_file, "left arr canary address: %p\n\n",      stack_data->stack_array);

    for(size_t i = 0; i < stack_data->size; i++) // printing NON empty element
    {
        fprintf(stack_data->dump_file, "stack element non empty %zu: %c\n",           i,   stack_data->stack_array[i + canary_offset]);
        fprintf(stack_data->dump_file, "stack element non empty %zu address: %p\n\n", i, &(stack_data->stack_array[i + canary_offset]));
    }
    
    for(size_t i = stack_data->size; i < stack_data->capacity; i++) // printing empty element
    {
        fprintf(stack_data->dump_file, "stack element empty %zu: %c\n",           i,       stack_data->stack_array[i + canary_offset]);
        fprintf(stack_data->dump_file, "stack element empty %zu address: %p\n\n", i,     &(stack_data->stack_array[i + canary_offset]));
    }

    fprintf(stack_data->dump_file, "right arr canary: 0x%0lx\n", *(uint64_t*)(stack_data->stack_array + canary_offset + stack_data->capacity));
    fprintf(stack_data->dump_file, "right arr canary address: %p\n",     stack_data->stack_array + canary_offset + stack_data->capacity);
    put_stars(stack_data->dump_file);


    fprintf(stack_data->dump_file, "struct hash value: 0x%0lx\n", stack_data->hash_struct);
    fprintf(stack_data->dump_file, "struct hash address: %p\n", &stack_data->hash_struct);
    put_stars(stack_data->dump_file);
    
    fprintf(stack_data->dump_file, "right struct canary: 0x%0lx\n",       stack_data->right_st_canary);
    fprintf(stack_data->dump_file, "right struct canary address: %p\n", &stack_data->right_st_canary);
    put_stars(stack_data->dump_file);
    put_stars(stack_data->dump_file);          
     
 
    fprintf(stack_data->dump_file, "\n\n\n\n");
    //printf every address meaning etc

    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;)
}


size_t stack_is_err(Main_Stack_Struct *stack_data, size_t hash) // FIXME CHANGE ZALUPA
{
    size_t errors_sum = 0;

    if(!is_struct_addresses_okay(stack_data))   errors_sum += 0x02;
    if(stack_data->capacity <= 0)               errors_sum += 0x04;
    if(stack_data->hash_struct != hash)         errors_sum += 0x08;
    if(stack_data->size > stack_data->capacity) errors_sum += 0x0F;
    if(stack_data->dump_file == nullptr)        errors_sum += 0x10;

    return errors_sum;
}


Error_Codes realloc_maker(Main_Stack_Struct *stack_data, int scale_coef) // FIXME add checker
{
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;)

    if(scale_coef <= 0) //???
    {
        return ALL_IS_OK; 
    }

    size_t new_capacity = 0;

    if(scale_coef == 2)
    {
        new_capacity = stack_data->capacity * (size_t)scale_coef;
    }
    else if (scale_coef == 4)
    {
        new_capacity = stack_data->capacity / (size_t)scale_coef;
    }


    StackElem_t *new_array = (StackElem_t*) realloc(stack_data->stack_array, (new_capacity + 2 * canary_offset)); // FIXME fix check

    stack_data->capacity      = new_capacity;
    stack_data->stack_array   = new_array;


    if(scale_coef == 2)
        memset((new_array + stack_data->size + canary_offset), My_Empty_Element, (stack_data->capacity - stack_data->size) * sizeof(StackElem_t));

    stack_data->hash_struct = hash_sum(*stack_data);

    TEMP_VAR(Error_Codes put_canaries_code = ) put_canaries(stack_data);
    ASSERT(!put_canaries_code && "put canaries error") TEMP_VAR(;) 
    
    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
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
                        YELLOW_TEXT("WRONG_HASH                =  ")   RED_TEXT("8\n")
                        YELLOW_TEXT("SIZE_BIGGER_THAN_CAPACITY = ")   RED_TEXT("16\n")
                        YELLOW_TEXT("FILE_PTR_IS_ZERO          = ")   RED_TEXT("32\n\n"));

        ASSERT(0) TEMP_VAR(;)  // FIXME
        return ALL_IS_OK;
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
    TEMP_VAR(size_t my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;)

    *((uint64_t*)stack_data->stack_array) = canary_value;

    *((uint64_t*)(stack_data->stack_array + canary_offset + stack_data->capacity)) = canary_value;


    stack_data->hash_struct = hash_sum(*stack_data);
    TEMP_VAR(my_hash = hash_sum(*stack_data);) 

    RETURN_ERROR(stack_is_err(stack_data, my_hash)) TEMP_VAR(;) 
    return ALL_IS_OK;
}

size_t hash_sum(const Main_Stack_Struct stack_data)
{
    size_t hash = 5831;
    size_t array_size = stack_data.capacity * sizeof(StackElem_t) + 2 * sizeof(canary_value);
    char* data = (char*)stack_data.stack_array;

    for(size_t i = 0; i < array_size; i++)
    {
        hash = hash * 33 ^ (size_t)data[i];
    }

    return hash;
}




//TODO canary_t (d_type) cond compile I WAS RIGHT ABOUT CANARIES 
//TODO make poison instead of '\0' check 'free' poison all in debug
//TODO push && pop - above 2 strings on release
//TODO get data_type from console  
//TODO poison before canaries (?)
//TODO check poison by address && meaning(?) (all stack)
//TODO memcpy???????
//TODO hash through control sum than real hash (non cryptographic) (hash = hash * 33 xor(current byte)) 
//TODO  #ON_DEBUG_NOT_ASSERT assert(!...) // !!!!
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