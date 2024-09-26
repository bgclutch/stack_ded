#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;

enum Program_Codes 
{
    SMTH_WAS_BROKEN = 0,
    ALL_IS_OK = 1,
    //ADD SOME ERRORS CODES

    FIRST_TIME_STRUCT_CTOR = 20

};

struct Main_Stack_Struct
{
    char* stack_array;
    char stack_elem;
    size_t counter;
    size_t capacity;
    size_t data_type_size;
    enum Program_Codes stack_code_return;
};


int ctor_stack(Main_Stack_Struct *stack_data);
int dtor_stack(Main_Stack_Struct *stack_data);


int stack_push(Main_Stack_Struct *stack_data);
int stack_pop (Main_Stack_Struct *stack_data);


void_sex stack_dump();


int stack_is_err();


#endif //STACK_H_