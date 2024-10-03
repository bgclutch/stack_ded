#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


int main(void)
{
    Main_Stack_Struct stack_data = {};

    stack_data.dump_file = fopen("stack_output.txt", "w"); // FIXME check
    assert(stack_data.dump_file);

    StackElem_t elem = 0;

    char test[] = {'a', '1', '^', 'k', '!', '@', '4', '/', 'j', '4', 'o'};
    
    Error_Codes ctor_result = ctor_stack(&stack_data); // FIXME check return value
    assert(!ctor_result && "stack constructor error");

    Error_Codes push_result = ALL_IS_OK;
    for(size_t i = 0; i < sizeof(test) / sizeof(StackElem_t); i++)
    {
        elem = test[i];
        push_result = stack_push(&stack_data, elem);
        assert(!push_result && "push result error");
    }

    Error_Codes pop_result = ALL_IS_OK;
    for(size_t i = stack_data.size; i >= 1; i--)
    {
        pop_result = stack_pop(&stack_data, &elem);
        assert(!pop_result && "pop result error");
    }

    Error_Codes dtor_result = dtor_stack(&stack_data);
    assert(!dtor_result && "dtor result error");

    if(fclose(stack_data.dump_file) != 0)
        printf("ty eblan?\n");

    return 0;
}