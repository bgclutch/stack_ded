#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <math.h>

typedef void void_sex;


#define RED     "\e[0;31m" ///< red
#define GREEN   "\e[0;32m" ///< green
#define YELLOW  "\e[0;33m" ///< yellow
#define BLUE    "\e[0;34m" ///< blue
#define MAGENTA "\e[0;35m" ///< magenta
#define RES_COL    "\e[0m" ///< reset color

#define RED_TEXT(text)     RED text RES_COL     ///< red text     + reset color  
#define GREEN_TEXT(text)   GREEN text RES_COL   ///< green text   + reset color
#define YELLOW_TEXT(text)  YELLOW text RES_COL  ///< yellow text  + reset color
#define BLUE_TEXT(text)    BLUE text RES_COL    ///< blue text    + reset color
#define MAGENTA_TEXT(text) MAGENTA text RES_COL ///< magenta text + reset color

enum Program_Codes 
{
    SMTH_WAS_BROKEN  = 0,
    ALL_IS_OK        = 1,
    REALLOC_COEF     = 4,
    //ADD SOME ERRORS CODES

    STRUCT_CTOR_SIZE = 20

};

// FIXME make typedef StackElem_t

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