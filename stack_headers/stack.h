#ifndef STACK_H_
#define STACK_H_


#define DEBUG

#ifdef DEBUG 
    #define DEBUG_VAR(...) __VA_ARGS__
    #define ASSERT(...) assert(__VA_ARGS__)
    #define RETURN_ERROR(error_sum) return_error(error_sum, __FILE__, __func__, __LINE__)
#else
    #define DEBUG_VAR(...)  
    #define ASSERT(...) 
    #define RETURN_ERROR(...) 
#endif

#define STACK_DUMP(stack_data)  stack_dump( stack_data, __FILE__, __func__, __LINE__) 
//#define BREAK_MODE



#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef void void_sex;
typedef char StackElem_t;


// FIXME make typedef StackElem_t

enum Error_Codes 
{
    ALL_IS_OK                  = 0x00,
    SMTH_WAS_BROKEN            = 0x01,
    STACK_ARRAY_ADDRESS_IS_BAD = 0x02,
    NEG_CAPACITY               = 0x04,
    WRONG_HASH                 = 0x08,
    STACK_OVERFLOW             = 0x0F,
    FILE_PTR_IS_ZERO           = 0x10,
    EMPTY_STACK                = 0x20,
    STACK_NOT_REALLOCED        = 0x72,                                                                                                  
    //ADD SOME ERRORS CODES
    //realloc error (na podumat')
};

struct Main_Stack_Struct
{ 
    uint64_t   left_st_canary;
    StackElem_t*  stack_array;  
    size_t               size;
    size_t           capacity;
    FILE*           dump_file;
    uint64_t      hash_struct;
    uint64_t  right_st_canary;
};


Error_Codes ctor_stack(Main_Stack_Struct *stack_data);
Error_Codes dtor_stack(Main_Stack_Struct *stack_data);


Error_Codes stack_push(Main_Stack_Struct *stack_data, StackElem_t elem);
Error_Codes stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem);


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line);


size_t realloc_if_up_needed(Main_Stack_Struct stack_data);


size_t realloc_if_down_needed(Main_Stack_Struct stack_data);


int is_struct_addresses_okay(Main_Stack_Struct *stack_data);


size_t stack_is_err(Main_Stack_Struct *stack_data, size_t hash);


void put_stars(FILE* file);


Error_Codes realloc_maker(Main_Stack_Struct *stack_data, size_t scale_coef);


Error_Codes return_error(size_t err_code, const char* file, const char* func, int line);


Error_Codes put_canaries(Main_Stack_Struct *stack_data);


size_t struct_elem_hash(const void* address, size_t size);


size_t hash_struct_sum(const Main_Stack_Struct *stack_data);



#endif //STACK_H_