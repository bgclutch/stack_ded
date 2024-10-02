#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


int main(void)
{
    struct Main_Stack_Struct stack_data = {};

    stack_data.dump_file = fopen("stack_output.txt", "w"); // FIXME check
    assert(stack_data.dump_file);

    StackElem_t elem = 0;

    char test[] = {'a', '1', '^', 'k', '!', '@', '4', '/', 'j', '4', 'o'};
    
    ctor_stack(&stack_data); // FIXME check return value

    for(size_t i = 0; i < sizeof(test) / sizeof(StackElem_t); i++)
    {
        elem = test[i];
        stack_push(&stack_data, elem);
    }

    for(long long i = stack_data.size; i >= 1; i--)
    {
        stack_pop(&stack_data, &elem);
    }

    dtor_stack(&stack_data);

    if(fclose(stack_data.dump_file) != 0)
        printf("ty eblan?\n");

    return 0;
}