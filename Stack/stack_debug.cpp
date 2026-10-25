#ifdef _DEBUG

#include "stack.h"
#include "debug.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------

FILE *const log_file = fopen(LOG_FILE_NAME, "w");


//-------------------------------------------------------------------------------------------------------------------------------------------------


int PrintStack(Stack_t *stack, const char *name, size_t len)
{
    ASRT_ST_P(stack); // return
    ASRT_ST_MEM(stack); // return

    LOG("\\/-----------------------------------------------------\\/\n")
    LOG("* %s[%p]: size = %zu, capacity = %zu;\n\n", name, stack->data, stack->size, stack->capacity)

    if (*(stack->c1) == BORDER_LINE)
    {
        LOG("Left canary = %X;\n", *(stack->c1));
    }

    if (len == 0)
    {
        LOG("\tThere's no data.\n")
    }
    else // print elements
    {
        for (size_t i = 0; i < len; i++)
        {
            LOG("\tstack[%4zu] = ", i) 
            PRINT_VAR(stack->data[i]);
            LOG(";\n")
        }
        if (*(stack->c2) == BORDER_LINE)
        {
            LOG("Right canary = %X;\n", *(stack->c2));
        }
        else
        {
            LOG("--> Right canary broke!\n")
        }
    }

    LOG("/\\-----------------------------------------------------/\\\n")
    return 0;
}

// bool IsPoison(stack_elem element)
// {
//     if (element == POISON)
//         return FIND_POISON;
//     else
//         return 0;
// }

int StackVerify(Stack_t *stack)
{
    LOG("* Stack verifying...\n")
    ASRT_ST_P(stack); // return
    ASRT_ST_MEM(stack); // return

    // stack is defined and data not null

    int err_code = ITS_OKAY;

    if (stack->capacity > MEM_LIM)
        {LOG("--> Stack capacity is lower then zero or overflowed!\n")}

    if (stack->size > MEM_LIM)
        {LOG("--> Stack size is lower then zero or overflowed!\n")}

    if (stack->size > stack->capacity)
    {
        LOG("--> Stack size is greater than capacity!\n")
        err_code = SIZE_MORE_CAPACITY;
    }

    if ((stack->capacity > 0) && (stack->capacity < MEM_LIM) && (stack->size < MEM_LIM))
        for (size_t i = 0; i < stack->size; i++)
        {
            if (stack->data[i] == POISON)
            {
                LOG("--> Stack[%zu] == POISON!\n", i)
                err_code = FIND_POISON;
            }
        }
    else if (stack->capacity == 0 or stack->size == 0)
        {LOG("--> Stack is empty")}
    else
        {LOG("capacity (size) greater MEM_LIM.\n")}

    if (stack->c1 != NULL)
    {
    if (*(stack->c1) != BORDER_LINE)
        {LOG("--> Left canary is crushed!\n")}
    else
        {LOG("* Left canary is okay.\n")}
    }

    if (stack->c2 != NULL)
    {
    if ((stack->capacity < MEM_LIM) && (*(stack->c2) == BORDER_LINE))
        {LOG("* Right canary is okay.\n")}
    else if (stack->capacity < MEM_LIM)
        {LOG("--> Right canary is undefined!\n")}
    else
        {LOG("--> Right canary is crushed.\n")}
    }


    if (stack->struct_c1 != BORDER_LINE)
        {LOG("--> Left STRUCT canary is crushed!\n")}
    else
        {LOG("* Left STRUCT canary is okay.\n")}

    if (stack->struct_c2 != BORDER_LINE)
        {LOG("--> Right STRUCT canary is crushed!\n")}
    else
        {LOG("* Right STRUCT canary is okay.\n")}

    LOG("* Stack verifying completed.\n")
    return err_code;
}

size_t CalcMemToCanary(size_t canary_sz, size_t arr_el_sz, size_t el_cont)
{
    size_t blocks_count = ((arr_el_sz * el_cont + canary_sz - 1) / canary_sz) + 2;

    return blocks_count * canary_sz;
}

void PrintError(int error)
{
    switch (error)
    {
        case ITS_OKAY:
            LOG("* Error: no errors.\n")
            break;

        case MEM_LIMIT_OR_LOWER_THAN_ZERO:
            LOG("--> Error: requested memory is greater than MEM_LIM or lower than zero!\n")
            break;

        case STACK_CALLOC_ERROR:
            LOG("--> Error: calloc returned NULL, stack was not created!\n")
            break;

        case NO_MEMORY_TO_STACK:
            LOG("--> Error: realloc returned NULL, no memory to expand stack!\n")
            break;

        case SIZE_MORE_CAPACITY:
            LOG("--> Error: stack size is greater than capacity!\n")
            break;

        case FIND_POISON:
            LOG("--> Error: POISON found among stack elements!\n")
            break;

        case CTOR_CRUSHED:
            LOG("--> Error: stack verification failed in StackCtor!\n")
            break;

        case PUSH_CRUSHED:
            LOG("--> Error: stack verification failed in StackPush!\n")
            break;

        case POP_CRUSHED:
            LOG("--> Error: stack verification failed in StackPop!\n")
            break;

        case DEST_CRUSHED:
            LOG("--> Error: stack is NULL or verification failed in StackDestructor!\n")
            break;

        case GET_GOJY:
            LOG("--> Error: attempt to pop from an empty stack (canary reached)!\n")
            break;

        default:
            LOG("--> Error: unknown error code %d!\n", error)
            break;
    }
    return;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------

#endif