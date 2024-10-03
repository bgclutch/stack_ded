#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef void void_sex;
typedef char StackElem_t;

// FIXME make typedef StackElem_t

enum Error_Codes 
{
    ALL_IS_OK                 = 0x00,
    SMTH_WAS_BROKEN           = 0x01,
    STRUCT_ADDRESS_IS_BAD     = 0x02,
    NEG_CAPACITY              = 0x04,
    WRONG_HASH                = 0x08,
    SIZE_BIGGER_THAN_CAPACITY = 0x0F,
    FILE_PTR_IS_ZERO          = 0x10,
    //ADD SOME ERRORS CODES
    //realloc error (na podumat')
};

enum Realloc_Codes
{
    REAL_UP   = 2,
    REAL_DOWN = 4
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


int realloc_if_up_needed(Main_Stack_Struct stack_data);


int realloc_if_down_needed(Main_Stack_Struct stack_data);


int is_struct_addresses_okay(Main_Stack_Struct *stack_data);


size_t stack_is_err(Main_Stack_Struct *stack_data, size_t hash);


void put_stars(FILE* file);


Error_Codes realloc_maker(Main_Stack_Struct *stack_data, int scale_coef);


Error_Codes return_error(size_t err_code, const char* file, const char* func, int line);


Error_Codes put_canaries(Main_Stack_Struct *stack_data);


size_t hash_sum(Main_Stack_Struct stack_data);


#endif //STACK_H_