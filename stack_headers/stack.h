#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;

enum Program_Codes
{
    SMTH_WAS_BROKEN = 0,
    ALL_IS_OK = 1
    //ADD SOME ERRORS CODES

};

struct Main_Stack_Struct
{
    char* stack_data;
    size_t counter;
    size_t capacity;
    enum Program_Codes stack_code_return;
};

#endif //STACK_H_