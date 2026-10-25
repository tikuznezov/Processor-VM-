#ifndef _STACKCPP
#define _STACKCPP


#include "stack.h"
#include "debug.h"
#include "stack_debug.cpp"

// #define _DEBUG
//-------------------------------------------------------------------------------------------------------------------------------------------------

int StackCtorINIT(Stack_t *stack, size_t capacity)
{
    // COMP check uninit stack

    // get memory
    #ifdef _DEBUG
        LOG("\n------------------------------------------------------------------------------------------------------------\n\n")
        LOG_FUNC_INFO
        size_t mem = CalcMemToCanary(sizeof(stack->c1), sizeof(stack_elem), capacity);
        if (mem >= MEM_LIM)
        {
            *error = MEM_LIMIT_OR_LOWER_THAN_ZERO;
            LOG("--> Memory limit exceed!\n")
            return MEM_LIMIT_OR_LOWER_THAN_ZERO;
        }
        LOG("Size of memory to stack with canary = %zu\n", mem)
    #else 
        size_t mem = sizeof(stack_elem) * capacity;
    #endif

    stack->data = (stack_elem *) calloc(1, mem);

    // set started values
    stack->capacity = capacity;
    stack->size = 0;

    #ifdef _DEBUG
        if (stack->data == NULL)
        {
            // FIXME
            LOG("--> Stack creation error!\n")
            CRUSH_FUNC_LOG
            *error = STACK_CALLOC_ERROR;
            return STACK_CALLOC_ERROR;
        }
        else
        {
            LOG("* Stack successfully created.\n")
            // FIXME
        }

        LOG("\t- mem = %zu, sizeof(c2) = %zu;\n", mem, sizeof(stack->c2))

        LOG("\t- stack->c1 = %p;\n", stack->data);
        stack->c1   = (unsigned long long *) stack->data;

        LOG("\t- stack->c2 = %p;\n", ((char *)stack->data) + mem - sizeof(stack->c2))
        stack->c2   = (unsigned long long *)(((char *)stack->data) + mem - sizeof(stack->c2));

        LOG("\t- stack->data = %p;\n", (char *)stack->data + sizeof(stack->c1))
        stack->data = (stack_elem *)((char *)stack->data + sizeof(stack->c1));

        LOG("* c1, c2, data get their addresses.\n\n")

        *(stack->c1) = BORDER_LINE;
        *(stack->c2) = BORDER_LINE;
        stack->struct_c1 = BORDER_LINE;
        stack->struct_c2 = BORDER_LINE;

        for (size_t i = 0; i < stack->capacity; i++)
        {
            stack->data[i] = POISON;
        }

        if (StackVerify(stack) != 0)
        {
            CRUSH_FUNC_LOG
            *error = CTOR_CRUSHED;
            return CTOR_CRUSHED;
        }
        PRINT_FUL_STACK(stack);
        END_FUNC_LOG
    #endif

    // return error code
    return 0;
}

int StackPushINIT(Stack_t *stack, stack_elem pushed_el)
{
    // COMP check stack

    #ifdef _DEBUG
        LOG_FUNC_INFO

        if (StackVerify(stack) != 0)
        {
            CRUSH_FUNC_LOG
            *error = PUSH_CRUSHED;
            return PUSH_CRUSHED;
        }

        if (stack->capacity > stack->size)
        {
        LOG("* PUSH stack[%zu] = ", (stack->size)) 
        PRINT_VAR(pushed_el);
        LOG(";\n")
        }
    #else
        assert(stack);
        assert(stack->data);
    #endif

    if (stack->capacity <= stack->size)
    {
        #ifdef _DEBUG
        LOG("* Attempt to write stack[%zu] = ", stack->size)
        PRINT_VAR(pushed_el);
        LOG(";\n")
        LOG("* PUSH called DomainExpansion.\n")
        PRINT_FUL_STACK(stack);
        #endif
        DomainExpansion(stack, CAPACITY_FACTOR_UP);
    }

    // push element
    if (stack->size < stack->capacity)
    {
        stack->data[stack->size] = pushed_el;
        stack->size++;
    }
    #ifdef _DEBUG
    END_FUNC_LOG
    #endif
    return 0;
}

stack_elem StackPopINIT(Stack_t *stack)
{
    #ifdef _DEBUG
        LOG_FUNC_INFO
        if (StackVerify(stack) != 0)
        {
            CRUSH_FUNC_LOG
            *error = POP_CRUSHED;
            return POP_CRUSHED;
        }
        LOG("* POP stack[%zu] = ", stack->size-1) 
        PRINT_VAR(stack->data[stack->size-1]); 
        LOG(";\n")

        if ((stack->data[stack->size - 1] == BORDER_LINE) || stack->size < 1)
        {
            LOG("--> POP canary!\n")
            *error = GET_GOJY;
            CRUSH_FUNC_LOG
            return POISON;
        }
        else 
            END_FUNC_LOG
            stack_elem pop_el= stack->data[--stack->size];
            stack->data[stack->size] = POISON;
    #else
        assert(stack);
        assert(stack->data);

        stack_elem pop_el = stack->data[--stack->size];
    #endif

    if ((stack->size < ((size_t) stack->capacity / CAPACITY_FACTOR_UP)) && (stack->size > MIN_CAP))
    {
        #ifdef _DEBUG
        LOG("* Stack size = %zu. Stack reduction...\n", stack->size)
        LOG("* POP called DomainExpansion.\n")
        PRINT_FUL_STACK(stack);
        #endif
        DomainExpansion(stack, CAPACITY_FACTOR_DOWN);
    }



    // считать среднеквадратичное отклонение. размер стека от 2х до 3х сигма
    return pop_el;
}

int StackDestructorINIT(Stack_t *stack)
{
    #ifdef _DEBUG
        LOG_FUNC_INFO
        PRINT_FUL_STACK(stack);
        StackVerify(stack);
        if (stack == NULL || stack->data == NULL)
            {
                CRUSH_FUNC_LOG
                *error = DEST_CRUSHED;
                return DEST_CRUSHED;
            }

        for (size_t i = 0; i < stack->capacity; i++)
            stack->data[i] = POISON;
        stack->data = (stack_elem *)stack->c1;
    #else
        assert(stack);
        assert(stack->data);
    #endif

    free(stack->data);
    stack->data = NULL;
    stack = NULL;

    #ifdef _DEBUG
    END_FUNC_LOG
    #endif
    return 0;
}

// COMP
int DomainExpansionINIT(Stack_t *stack, double capacity_factor)
{
    #ifdef _DEBUG
        LOG_FUNC_INFO

        if (StackVerify(stack) != 0)
            {
                CRUSH_FUNC_LOG
                *error = DEST_CRUSHED;
                return DEST_CRUSHED;
            }

        LOG("Expansion started...\n")
        LOG("* Current values:\n")
        LOG("\t- Data address = %p;\n", stack->data)
        LOG("\t- Capacity     = %zu;\n", stack->capacity)
        PRINT_FUL_STACK(stack);
    #else
        assert(stack);
        assert(stack->data);
    #endif

    size_t new_capacity = (size_t)(stack->capacity * capacity_factor);

    #ifdef _DEBUG
        LOG("* Attempt to expand memory...\n")
        LOG("* Requested memory size = %zu\n", new_capacity)

        size_t mem = CalcMemToCanary(sizeof(stack->c1), sizeof(stack_elem), new_capacity);
        LOG("Size of memory to stack with canary = %zu\n", mem)

        stack_elem *new_pointer = (stack_elem *) realloc((void *)(stack->c1), mem);
        if (new_pointer != NULL)
        {
            LOG("mem = %zu, sizeof(c2) = %zu;\n", mem, sizeof(stack->c2))

            LOG("stack->c1 = %p;\n", new_pointer);
            stack->c1   = (unsigned long long *) new_pointer;

            LOG("stack->c2 = %p;\n", ((char *)new_pointer) + mem - sizeof(stack->c2))
            stack->c2   = (unsigned long long *)(((char *)new_pointer) + mem - sizeof(stack->c2));
            *(stack->c2) = BORDER_LINE;

            LOG("stack->data = %p;\n", (char *)new_pointer + sizeof(stack->c1))
            stack->data = (stack_elem *)((char *)new_pointer + sizeof(stack->c1));
        }
    #else
        stack_elem *new_pointer = (stack_elem *) realloc((void *)(stack->data    ), new_capacity * sizeof(stack_elem));
    #endif

    if (new_pointer != NULL)
    {
        stack->capacity = new_capacity;
        #ifndef _DEBUG
        stack->data     = new_pointer;
        #endif

        #ifdef _DEBUG
            for (size_t i = stack->size; i < stack->capacity; i++)
                stack->data[i] = POISON;
            LOG("* Memory successfully expanded.\n")
            LOG("* New values:\n")
            LOG("\t- Data address = %p;\n", stack->data)
            LOG("\t- Capacity     = %zu;\n", stack->capacity)
            PRINT_FUL_STACK(stack);
            END_FUNC_LOG
        #endif
    }
    else
    {
        #ifdef _DEBUG
        *error = NO_MEMORY_TO_STACK;
        END_FUNC_LOG
        #endif
        return 1;
    }

    return 0;
}

#endif