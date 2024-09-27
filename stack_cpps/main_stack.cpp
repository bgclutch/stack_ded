#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../stack_headers/stack.h"


struct Main_Stack_Struct stack_data = {}; // FIXME global


int main(void)
{
    stack_data.aboba = fopen("stack_output.txt", "w");

    char test[] = {'a', '1', '^', 'k', '!', '@', '4', '/', 'j'};

    stack_data.data_type_size = sizeof(test[0]);//FIXME change it to console parsing later
    
    ctor_stack(&stack_data);

    for(size_t i = 0; i < sizeof(test) / sizeof(char); i++)
    {
        stack_data.stack_elem = test[i];
        stack_push(&stack_data);
    }

    for(long i = stack_data.counter - 1; i >= 0; i++)
    {
        stack_pop(&stack_data);
    }

    dtor_stack(&stack_data);

    free(stack_data.stack_array);

    if(fclose(stack_data.aboba) != 0)
        printf("ty eblan?\n");

    return 0;
}