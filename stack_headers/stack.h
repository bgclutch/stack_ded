#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;
typedef char StackElem_t;

// FIXME make typedef StackElem_t

enum Error_Codes 
{
    ALL_IS_OK                 = 0x00,
    SMTH_WAS_BROKEN           = 0x01,
    STRUCT_ADDRESS_IS_BAD     = 0x02,
    NEG_CAPACITY              = 0x04,
    NEG_SIZE                  = 0x08,
    SIZE_BIGGER_THAN_CAPACITY = 0x0F,
    FILE_PTR_IS_ZERO          = 0x10,
    //ADD SOME ERRORS CODES
    //realloc error (na podumat')
};

enum Realloc_Codes
{
    REAL_UP          =   1,
    REAL_DOWN        =   2,
    STRUCT_CTOR_SIZE =   4,
    INIT_CODE        = -10
};


struct Main_Stack_Struct
{ 
    long       left_st_canary;
    StackElem_t*  stack_array;  
    StackElem_t* extended_arr;
    long                 size;
    long             capacity;
    FILE*               aboba;
    long      right_st_canary;
    
};


Error_Codes ctor_stack(Main_Stack_Struct *stack_data, size_t size);
Error_Codes dtor_stack(Main_Stack_Struct *stack_data);


Error_Codes stack_push(Main_Stack_Struct *stack_data, StackElem_t elem);
Error_Codes stack_pop(Main_Stack_Struct *stack_data, StackElem_t *elem);


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line);


int is_arr_incr_need(Main_Stack_Struct stack_data);


int is_arr_decr_need(Main_Stack_Struct stack_data);


int is_struct_addresses_okay(Main_Stack_Struct *stack_data);


size_t stack_is_err(Main_Stack_Struct *stack_data);


void put_stars(FILE* file);


Error_Codes realloc_maker(Main_Stack_Struct *stack_data);


Error_Codes return_error(size_t err_code, const char* file, const char* func, int line);


Error_Codes put_canaries(Main_Stack_Struct *stack_data);


#endif //STACK_H_