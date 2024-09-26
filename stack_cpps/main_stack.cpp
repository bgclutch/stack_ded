#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../stack_headers/stack.h"


struct Main_Stack_Struct stack_data = {};


int main(void)
{
    char test[] = {'a', '1', '77', 'k', '!', '@'};

    stack_data.data_type_size = sizeof(test[0]);//FIXME later
    
    ctor_stack(&stack_data);

    stack_push(&stack_data);


    return 0;
}