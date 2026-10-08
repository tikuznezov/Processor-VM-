typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);

// #define _DEBUG

#include "vm.h"
#include "log.cpp"
#include "/Applications/Vs code/Steck/stack.h"
#include "/Applications/Vs code/Steck/debug.h"
#include "/Applications/Vs code/Steck/stack.cpp"
#include "/Applications/Vs code/Steck/stack_debug.cpp"


const size_t BUF_SIZE = 1000;

// считывает все в указанный буфер
int ReadBinFile(const char *filename, int **buffer_p, size_t *sizeof_buffer)
{
    PR_FST;
    FILE *Code = fopen(filename, "r");

    PR("* starting calloc...\n");
    int *buffer = (int *) calloc(BUF_SIZE, sizeof(int)); // FIXME
    if (buffer == NULL)
    {
        printf("--> Buffer creation error (calloc error)!\n");
    }
    PR("* calloc comp\n");

    size_t readmark = 1;
    *sizeof_buffer = 0;
    while ((readmark != 0) && (readmark != -1))
    {
        PR("current index = [%zu]\n", *sizeof_buffer);
        readmark = fscanf(Code, "%d", &(buffer[(*sizeof_buffer)]));
        PR("read (%d)\n", (buffer[(*sizeof_buffer)]));
        (*sizeof_buffer)++;

        if ((readmark == 1) && ((buffer[(*sizeof_buffer)] == 501)))
        {
            readmark = fscanf(Code, "%d", &(buffer[(*sizeof_buffer)]));
            (*sizeof_buffer)++;
        }
    }

    *buffer_p = buffer;
    (*sizeof_buffer)--; // он считывает конец файла и увеличивает счетчик. чтобы не писать проверку достижения конца на каждом этапе в самом конце вычитаем 1

    PR("sizeof(buffer) = %zu;\n", *sizeof_buffer);
    PR("buffer =");
    for (int i = 0; i < *sizeof_buffer; i++)
        PR(" [%d]", buffer[i]);
    PR(";\n");

    PR_FED;
    return 0;
}

// группа функций соотв номерам команд

// выполняет бинарный файл
stack_elem RunBin(const int *code, const size_t sizeof_buffer)
{
    PR_FST;
    size_t buf_index = 0;

    Stack_t stack = {};
    StackCtor(&stack, 2);

    while (buf_index < sizeof_buffer)
    {
        switch (code[buf_index])
        {
            case PUSH:
            {
                buf_index++;
                StackPush(&stack, code[buf_index]);
                PR("* PUSH(%d)\n", code[buf_index]);
                break;
            }
            case ADD:
            {
                if (stack.size >= 2)
                {
                    stack_elem temp1 = StackPop(&stack);
                    stack_elem temp2 = StackPop(&stack);
                    StackPush(&stack, temp1 + temp2);
                    PR("* ADD\n");
                }
                else
                {
                    PR("--> Try to ADD empty stack!\n");
                }
                break;
            }
            case DIV:
            {
                if (stack.size >= 2)
                {
                    stack_elem temp1 = StackPop(&stack);
                    stack_elem temp2 = StackPop(&stack);
                    StackPush(&stack, temp2 / temp1);
                    PR("* DIV:\n");
                }
                else
                {
                    PR("--> Try to DIV empty stack!\n");
                }
                break;
            }
            case OUT:
            {
                PR("* OUT\n");
                printf("Result = ");
                stack_elem temp1 = StackPop(&stack);
                StackPush(&stack, temp1);
                PRINT_ELEM_T(temp1);
                printf(";\n");
                break;
            }
            case HLT:
            {
                PR("* HLT\n");
                break;
            }
            case SUB:
            {
                if (stack.size >= 2)
                {
                    stack_elem temp1 = StackPop(&stack);
                    stack_elem temp2 = StackPop(&stack);
                    StackPush(&stack, temp2 - temp1);
                    PR("* SUB\n");
                }
                else
                {
                    PR("--> Try to SUB empty stack!\n");
                }
                break;
            }

            default:
            {
                printf("--> Unknown instruction!\n");
            }
        }
        buf_index++;
    }


    PR_FED;
    return StackPop(&stack);
}

























