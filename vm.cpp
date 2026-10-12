#include "vm.h"
#include "log.cpp"
#include "/Applications/VsСode/Stack/stack.h"
#include "/Applications/VsСode/Stack/debug.h"
#include "/Applications/VsСode/Stack/stack.cpp"
#include "/Applications/VsСode/Stack/stack_debug.cpp"


// считывает все в указанный буфер
// читает аргументы и команды в массив интов
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
stack_elem RunBin(Processor proc)
{
    PR_FST;

    StackCtor(&(proc.stack), 2);

    // проход по массиву с кодом по индексу ip/pc

    int count_inst_calls = 0;

    while ((proc.pc < proc.code_size) && (count_inst_calls <= MAX_INSTRUCTION_CALLS))
    {
        count_inst_calls++;
        PR("* RUN pc = (%zu)\n", proc.pc);

        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
        {
            if (proc.code[proc.pc] == ((Instructions[i]).num))
                (Instructions[i]).execute(&proc);
        }
    }

    if (count_inst_calls >= MAX_INSTRUCTION_CALLS)
    {
        PRED;
        PR("--> The maximum number of instruction calls has been exceeded!!!\n")
        DEF_COL;
    }

    // TODO функция, которая производит бинарный поиск по массиву структур и выбирает нужную команду в цикле
    // она же увеличивает индекс команды на 1 после выполнения. Таким образом джамп впишет нужный индекс и процессор перескочит назад.

    PR_FED;
    return 0;
}




// FIXME функции должны обрабатывать после себя такое количество аргументов, сколько ими предусмотрено и возвращать 
// указатель на следующую инструкцию 

int InstUNKNOWN(Processor *proc)
{
    printf("--> Unknown instruction!\n");
    return 1;
}
int InstPUSH (Processor *proc)
{
    PR_FST;

    proc->pc++;
    StackPush(&(proc->stack), proc->code[proc->pc]);
    PR("* PUSH(%d)\n", proc->code[proc->pc]);
    proc->pc++;

    PR_FED;
    return 0;
}
int InstPOP  (Processor *proc)
{
    PR_FST;
    if (proc->stack.size > 0)
    {
        StackPop(&(proc->stack));
        PR("* POP(%d)\n", proc->code[proc->pc]);
        proc->pc++;
        PR("* Next instuction code = %zu.\n", proc->pc);
    }
    else
    {
        PR("--> Try to POP empty stack!\n");
        return 1;
    }
    PR_FED;
    return 0;
}
int InstPOPr (Processor *proc)
{
    PR_FED;
    return 0;
}
int InstPUSHr(Processor *proc)
{
    PR_FED;
    return 0;
}
int InstJMP  (Processor *proc)
{
    PR_FED;
    return 0;
}
int InstADD  (Processor *proc)
{
    if (proc->stack.size >= 2)
    {
        StackPush(&(proc->stack), StackPop(&(proc->stack)) + StackPop(&(proc->stack)));
        PR("* ADD");
        proc->pc++;
    }
    else
    {
        PR("--> Try to ADD empty stack!\n");
        return 1;
    }
    PR_FED;
    return 0;
}
int InstSUB  (Processor *proc)
{
    if (proc->stack.size >= 2)
    {
        StackPush(&(proc->stack), 0 - StackPop(&(proc->stack)) + StackPop(&(proc->stack)));
        PR("* SUB\n");
        proc->pc++;
    }
    else
    {
        PR("--> Try to SUB empty stack!\n");
        return 1;
    }
    PR_FED;
    return 0;
}
int InstMUL  (Processor *proc)
{
    PR_FED;
    return 0;
}
int InstDIV  (Processor *proc)
{
    if (proc->stack.size >= 2)
    {
        StackPush(&(proc->stack), 1 / StackPop(&(proc->stack)) * StackPop(&(proc->stack)));
        PR("* DIV:\n");
        proc->pc++;
    }
    else
    {
        PR("--> Try to DIV empty stack!\n");
        return 1;
    }

    PR_FED;
    return 0;
}
int InstOUT  (Processor *proc)
{
    PR_FST
    if (proc->stack.size > 0)
    {
        PR("* OUT\n");
        printf("Result = ");
        PRINT_ELEM_T(StackPop(&(proc->stack)));
        printf(";\n");
        proc->pc++;
    }
    else
    {
        PR("--> Try to OUT empty stack!\n");
        return 1;
    }
    PR_FED
    return 0;
}
int InstHLT  (Processor *proc)
{
    PR_FST;
    proc->pc++;
    PR_FED;
    return 1;
}