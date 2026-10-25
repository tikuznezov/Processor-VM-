#ifdef _DEBUG

#include "stack.h"

#ifndef _debug_h
#define _debug_h


//-------------------------------------------------------------------------------------------------------------------------------------------------

const char *     LOG_FILE_NAME       = "STACK_LOG.log";
const stack_elem POISON              = ('s'+'a'+'t'+'o'+'r'+'u') * ('p'+'i'+'d'+'o'+'r'+'a'+'s');
const unsigned long long BORDER_LINE = 0xD0DACDDDD;

// const unsigned long long BORDER_LINE   = ('c'+'a'+'n'+'a'+'r'+'y') * ('b'+'o'+'r'+'d'+'e'+'r'+'l'+'i'+'n'+'e');

enum STACK_ERR
{
    ITS_OKAY = 0,
    NO_MEMORY_TO_STACK = 676767,
    SIZE_MORE_CAPACITY,
    STACK_DOESNOT_EXIST,
    FIND_POISON,
    OUT_OF_MEM_ZONE,
    STACK_IS_VOID,
    STACK_CALLOC_ERROR,
    EMPTY_STACK,
    POP_CRUSHED,
    PUSH_CRUSHED,
    CTOR_CRUSHED,
    DEST_CRUSHED,
    MEM_LIMIT_OR_LOWER_THAN_ZERO,
    GET_GOJY
};


//-------------------------------------------------------------------------------------------------------------------------------------------------

#define LOG(name, ...)  fprintf(log_file, name, ## __VA_ARGS__);
#define LOG_FUNC_INFO   LOG("\n[INFO] [%s:%d] (%s) --> started...\n", file_name, line_num, __FUNCTION__)
#define END_FUNC_LOG    LOG("[INFO] (%s) --> completed.\n\n", __FUNCTION__)
#define CRUSH_FUNC_LOG  LOG("[WARN] (%s) --> CRUSHED!!!\n\n", __FUNCTION__)

#define PRINT_VAR_WITH_NAME(x)  LOG("--> %s = ", #x) PRINT_ELEM_T(x); LOG(" <--\n")
#define PRINT_VAR(x)            PRINT_ELEM_T(x);
#define PRINT_FUL_STACK(x)      PrintStack(x, #x, x->capacity);
#define PRINT_STACK(x)          PrintStack(x, #x, x->size);

#define ASRT_ST_P(stack)        if (stack == NULL){ \
                                    fprintf(log_file, "--> Stack is not defined. (stack is NULL pointer)\n"); \
                                    return STACK_DOESNOT_EXIST;}

#define ASRT_ST_MEM(stack)      if (stack->data == NULL){ \
                                    fprintf(log_file, "--> Stack is empty. (data is NULL pointer)\n"); \
                                    return STACK_IS_VOID;}

//-------------------------------------------------------------------------------------------------------------------------------------------------

// bool IsPoison(stack_elem element);

int PrintStack(Stack_t *stack);

int StackVerify(Stack_t *stack);

size_t CalcMemToCanary(size_t canary_sz, size_t arr_el_sz, size_t el_cont);

void PrintError(int error);

//-------------------------------------------------------------------------------------------------------------------------------------------------


#endif

#endif