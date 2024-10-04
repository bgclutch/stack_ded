#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>


#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


#ifdef CANARY_MODE
    const size_t canary_offset = sizeof(long) / sizeof(StackElem_t);
    const uint64_t canary_value  = 0x3DB510BA17D4EE; //dumayte // FIXME long long to long
#else
    const size_t canary_offset   = 0;
    const uint64_t canary_value  = 0; //dumayte // FIXME long long to long

#endif

const size_t Struct_Ctor_Size =  8;
static StackElem_t Poison_Element  = '~'; // ???


Error_Codes ctor_stack(Main_Stack_Struct *stack_data)
{
    ASSERT(stack_data && "stack data address is NULL") DEBUG_VAR(;)      // FIXME assert or make disableable (simple error here)

    #ifdef CANARIES
        stack_data->stack_array = (char*) calloc(Struct_Ctor_Size * sizeof(StackElem_t) +
                                                 2 * canary_offset, sizeof(char));
    #else
        stack_data->stack_array = (char*) calloc(Struct_Ctor_Size, sizeof(StackElem_t));
    #endif

    if(stack_data->stack_array == nullptr) //hueta check
        return STACK_ARRAY_ADDRESS_IS_BAD; // TODO make 1 pointer instead of 2
    
    stack_data->capacity = Struct_Ctor_Size;
    stack_data->size     = 0;


    CANARIES
    (stack_data->left_st_canary  = canary_value;
    stack_data->right_st_canary = canary_value;)


    memset(stack_data->stack_array + canary_offset, Poison_Element, stack_data->capacity);

    // for(size_t i = canary_offset; i < stack_data->capacity; i++)
    //     stack_data->stack_array[i] = Poison_Element;

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    CANARIES(Error_Codes put_canaries_code = put_canaries(stack_data);)

    CANARIES(ASSERT(!put_canaries_code && "put canaries error") DEBUG_VAR(;)) 

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)
    
    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

Error_Codes dtor_stack(Main_Stack_Struct *stack_data)
{
    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 
    STACK_DUMP(stack_data);

    if(!is_struct_addresses_okay(stack_data)) //address and meanings
        return STACK_ARRAY_ADDRESS_IS_BAD;

    memset(stack_data->stack_array + canary_offset, Poison_Element, stack_data->capacity); // FIXME memset is only for bytes

    free(stack_data->stack_array); 
    stack_data->stack_array = nullptr;
    ASSERT(!stack_data->stack_array && "stack array != nullptr") DEBUG_VAR(;) 

    return ALL_IS_OK;
}


Error_Codes stack_push(Main_Stack_Struct *stack_data, StackElem_t elem)
{
    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    Error_Codes realloc_code = realloc_maker(stack_data, realloc_if_up_needed(*stack_data));

    if(realloc_code == STACK_NOT_REALLOCED) 
        return realloc_code;


    ASSERT(!realloc_code && "realloc error") DEBUG_VAR(;)  // FIXME check errors

    stack_data->stack_array[canary_offset + stack_data->size++] = elem;

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;)
    STACK_DUMP(stack_data);
    return ALL_IS_OK;
}

//FIXME error checking
Error_Codes stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem)
{
    ASSERT(elem) DEBUG_VAR(;) 

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;)  
    
    if(stack_data->size == 0)
    {
        return EMPTY_STACK;
    }

    *elem = stack_data->stack_array[canary_offset + (--stack_data->size)]; 

    DEBUG_VAR
    (put_stars(stack_data->dump_file);
    fprintf(stack_data->dump_file, "popped elem '%c'\n", *elem);)

    stack_data->stack_array[canary_offset + stack_data->size] = Poison_Element;  

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    Error_Codes realloc_code = realloc_maker(stack_data, realloc_if_down_needed(*stack_data));

    if(realloc_code == STACK_NOT_REALLOCED)
        return realloc_code;

    ASSERT(!realloc_code && "realloc error") DEBUG_VAR(;) 

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 
    STACK_DUMP(stack_data); // TODO use macros
    return ALL_IS_OK;
}


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line) //add checks
{
    DEBUG_VAR(
    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 

    put_stars(stack_data->dump_file);
    fprintf  (stack_data->dump_file, "%s %s:%lu\n", file_name, func_name, line);
    put_stars(stack_data->dump_file);
    
    CANARIES(
    fprintf(stack_data->dump_file, "left struct canary: 0x%0lx\n",      stack_data->left_st_canary);
    fprintf(stack_data->dump_file, "left struct canary address: %p\n", &stack_data->left_st_canary);
    put_stars(stack_data->dump_file);)

    fprintf  (stack_data->dump_file, "stack size: %zu\n",                stack_data->size);
    fprintf  (stack_data->dump_file, "stack size address: %p\n",       &(stack_data->size));
    put_stars(stack_data->dump_file);   
  
    fprintf  (stack_data->dump_file, "stack capacity: %zu\n",            stack_data->capacity);
    fprintf  (stack_data->dump_file, "stack capacity address: %p\n",   &(stack_data->capacity));
    put_stars(stack_data->dump_file);   

    fprintf  (stack_data->dump_file, "stack address begin: %p\n",       stack_data->stack_array);
    fprintf  (stack_data->dump_file, "stack address end: %p\n",        &stack_data->stack_array[stack_data->capacity]);
    put_stars(stack_data->dump_file);

    CANARIES(
    fprintf(stack_data->dump_file, "left stack canary: 0x%0lx\n",   *(uint64_t*)(stack_data->stack_array));
    fprintf(stack_data->dump_file, "left stack canary address: %p\n\n",          stack_data->stack_array);)

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

    CANARIES(
    fprintf(stack_data->dump_file, "right stack canary: 0x%0lx\n", *(uint64_t*)(stack_data->stack_array + canary_offset + stack_data->capacity));
    fprintf(stack_data->dump_file, "right stack canary address: %p\n",          stack_data->stack_array + canary_offset + stack_data->capacity);
    put_stars(stack_data->dump_file);)

    HASH(
    fprintf(stack_data->dump_file, "struct hash value: 0x%0lx\n",        stack_data->hash_struct);
    fprintf(stack_data->dump_file, "struct hash address: %p\n",         &stack_data->hash_struct);
    put_stars(stack_data->dump_file);)
    
    CANARIES(
    fprintf(stack_data->dump_file, "right struct canary: 0x%0lx\n",      stack_data->right_st_canary);
    fprintf(stack_data->dump_file, "right struct canary address: %p\n", &stack_data->right_st_canary);
    put_stars(stack_data->dump_file);)
    put_stars(stack_data->dump_file);       
     
 
    fprintf(stack_data->dump_file, "\n\n\n\n");
    //printf every address meaning etc

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;))
}


size_t stack_is_err(Main_Stack_Struct *stack_data) // FIXME CHANGE ZALUPA
{
    DEBUG_VAR(
    size_t errors_sum = 0;
    HASH(size_t my_hash = hash_struct_sum(stack_data);)

    if(!is_struct_addresses_okay(stack_data))   errors_sum += 0x02;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    if(stack_data->capacity <= 0)               errors_sum += 0x04;
    HASH(if(stack_data->hash_struct != my_hash) errors_sum += 0x08;)
    if(stack_data->size > stack_data->capacity) errors_sum += 0x0F;
    if(stack_data->dump_file == nullptr)        errors_sum += 0x10;

    for(size_t i = 0; i < stack_data->size; i++)
        if(stack_data->stack_array[i + canary_offset] == Poison_Element) errors_sum += 0x40;

    return errors_sum;)
}


Error_Codes realloc_maker(Main_Stack_Struct *stack_data, size_t scale_coef) // FIXME add checker
{
    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;)

    if(scale_coef != 2 && scale_coef != 4) //???
    {
        return ALL_IS_OK; 
    }

    size_t new_capacity = 0;

    if(scale_coef == 2)
    {
        new_capacity = stack_data->capacity * scale_coef;
    }
    else if (scale_coef == 4)
    {
        new_capacity = stack_data->capacity / scale_coef;
    }


    StackElem_t *new_array = (StackElem_t*) realloc(stack_data->stack_array,
                                                   (new_capacity + 2 * canary_offset)); // FIXME fix check

    if(new_array == nullptr)
    {
        fprintf(stderr, "stack RED_TEXT(couldn't realloced) at address:YELLOW_TEXT(%p\n)", new_array);
        return STACK_NOT_REALLOCED;
    }


    stack_data->capacity      = new_capacity;
    stack_data->stack_array   = new_array;


    if(scale_coef == 2)
        memset((new_array + stack_data->size + canary_offset), Poison_Element, (stack_data->capacity - stack_data->size) * sizeof(StackElem_t));

    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    CANARIES(Error_Codes put_canaries_code = put_canaries(stack_data));

    CANARIES(ASSERT(!put_canaries_code && "put canaries error") DEBUG_VAR(;)) 
    
    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 
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


size_t realloc_if_up_needed(Main_Stack_Struct stack_data)
{

    if(stack_data.capacity - stack_data.size <= 1)
        return 2;
    else
        return 0;

}


size_t realloc_if_down_needed(Main_Stack_Struct stack_data)
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
                        YELLOW_TEXT("STRUCT_ADDRESS_IS_BAD =  ")   RED_TEXT("2\n")
                        YELLOW_TEXT("NEG_CAPACITY          =  ")   RED_TEXT("4\n")
                        YELLOW_TEXT("WRONG_HASH            =  ")   RED_TEXT("8\n")
                        YELLOW_TEXT("STACK_OVERFLOW        = ")   RED_TEXT("16\n")
                        YELLOW_TEXT("FILE_PTR_IS_ZERO      = ")   RED_TEXT("32\n")
                        YELLOW_TEXT("POISON_IN_STACK       = ")   RED_TEXT("64\n\n"));

        ASSERT(0) DEBUG_VAR(;)  // FIXME
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

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;)

    *((uint64_t*)stack_data->stack_array) = canary_value;

    *((uint64_t*)(stack_data->stack_array + canary_offset + stack_data->capacity)) = canary_value;


    HASH(stack_data->hash_struct = hash_struct_sum(stack_data);)

    RETURN_ERROR(stack_is_err(stack_data)) DEBUG_VAR(;) 
    return ALL_IS_OK;
}

size_t struct_elem_hash(const void* address, size_t size)
{
    size_t hash = 5831;

    for(size_t i = 0; i < size; i++)
    {
        hash = hash * 33 ^ (size_t)((const uint8_t*)address + i);
    }

    return hash;
}


size_t hash_struct_sum(const Main_Stack_Struct *stack_data)
{
    size_t stack_hash_sum = 0;

    CANARIES(stack_hash_sum += struct_elem_hash(                              (uint8_t*)stack_data->left_st_canary,
                                                                              sizeof(stack_data->left_st_canary));)
 
    stack_hash_sum += struct_elem_hash(stack_data->stack_array,               stack_data->capacity * sizeof(StackElem_t) + 
                                                                              2 * sizeof(canary_value));
 
    stack_hash_sum += struct_elem_hash(&stack_data->size,                     sizeof(stack_data->size));
 
    stack_hash_sum += struct_elem_hash(&stack_data->capacity,                 sizeof(stack_data->capacity));

    CANARIES(stack_hash_sum += struct_elem_hash(&stack_data->right_st_canary, sizeof(stack_data->right_st_canary));)
    
    return stack_hash_sum;
}



//TODO canary_t (d_type) cond compile I WAS RIGHT ABOUT CANARIES 
//TODO make poison instead of '\0' check 'free' poison all in debug
//TODO push && pop - above 2 strings on release
//TODO get data_type from console (?)
//TODO poison before canaries (?)
//TODO check poison by address && meaning(?) (all stack)
//TODO memcpy???????
//TODO hash through control sum than real hash (non cryptographic) (hash = hash * 33 xor(current byte)) 
//TODO #ON_DEBUG_NOT_ASSERT assert(!...) // !!!!
//TODO   
//TODO  
//TODO for void*: before every push/pop operation need to know size of current elem, stack size in uint8_t
//TODO checks && realloc will depend on size of current elem
//TODO  
//TODO  
//TODO 
//TODO  
//TODO  
//TODO 