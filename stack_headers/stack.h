#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;

enum Program_Codes 
{
    SMTH_WAS_BROKEN  = 0,
    ALL_IS_OK        = 1,
    REALLOC_COEF     = 4,
    //ADD SOME ERRORS CODES

    STRUCT_CTOR_SIZE = 20

};

struct Main_Stack_Struct
{
    char* stack_array;
    double stack_elem;
    size_t counter;
    size_t capacity;
    size_t capacity_coefficient;
    size_t data_type_size;
    enum Program_Codes stack_code_return;
    FILE* aboba;
};


int ctor_stack(Main_Stack_Struct *stack_data);
int dtor_stack(Main_Stack_Struct *stack_data);


int stack_push(Main_Stack_Struct *stack_data);
int stack_pop (Main_Stack_Struct *stack_data);


void_sex stack_dump(Main_Stack_Struct *stack_data, const char* call_func_name);


// int stack_is_err();


void put_stars(FILE* file);


int realloc_maker(Main_Stack_Struct *stack_data);

#endif //STACK_H_