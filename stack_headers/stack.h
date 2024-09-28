#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;
typedef char StackElem_t;

enum Program_Codes 
{
    SMTH_WAS_BROKEN    = 0,
    ALL_IS_OK          = 1,
    REALLOC_COEF       = 4,
    //ADD SOME ERRORS CODES
    //realloc error (na podumat')

    STRUCT_CTOR_SIZE   = 5

};

// FIXME make typedef StackElem_t

struct Main_Stack_Struct
{
    StackElem_t* stack_array;
    size_t size;
    size_t capacity;
    FILE* aboba;
};


int ctor_stack(Main_Stack_Struct *stack_data, size_t size);
int dtor_stack(Main_Stack_Struct *stack_data);


int stack_push(Main_Stack_Struct *stack_data, StackElem_t elem);
int stack_pop (Main_Stack_Struct *stack_data, StackElem_t *elem);


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* file_name, const char* func_name, size_t line);


int is_arr_incr_need(Main_Stack_Struct stack_data);


int is_arr_decr_need(Main_Stack_Struct stack_data);


int is_struct_addresses_okay(Main_Stack_Struct *stack_data);


// int stack_is_err();


void put_stars(FILE* file);


int realloc_maker(Main_Stack_Struct *stack_data, int var);

#endif //STACK_H_