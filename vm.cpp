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
int RunBin(Processor proc)
{
    PR_FST;

    StackCtor(&(proc.stack), 2);

    // проход по массиву с кодом по индексу ip/pc

    // limit instruction calls to avoid an inf cycle
    int count_inst_calls = 0;
    int err = 0;

    while ((proc.pc < proc.code_size) && (count_inst_calls <= MAX_INSTRUCTION_CALLS))
    {
        count_inst_calls++;
        PYELLOW PR("* RUN pc = (%zu)\n", proc.pc); DEF_COL

        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
        {
            if (proc.code[proc.pc] == ((Instructions[i]).num))
                err = (Instructions[i]).execute(&proc);
        }
        if (err != 0)
            return err;
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
        return POP_CRUSH;
    }

    PR_FED;
    return 0;
}
int InstPOPr (Processor *proc)
{
    PR_FST;

    if ((proc->stack.size > 0))
    {
        if ((proc->code[proc->pc + 1] - 1 < REG_COUNT) && (proc->code[proc->pc + 1] - 1 >= 0))

        {
            proc->reg[proc->code[proc->pc + 1] - 1] = StackPop(&(proc->stack));
            PR("* POP(%d)\n", proc->code[proc->pc]);
            proc->pc++;
            proc->pc++;
            PR("* Next instuction code = %zu.\n", proc->pc);
        }
        else
        {
            PR("--> Try to POPr in undefined register!\n");
            return POPr_CRUSH;
        }
    }
    else
    {
        PR("--> Try to POPr empty stack!\n");
        return POPr_CRUSH;
    }

    PR_FED;
    return 0;
}
int InstPUSHr(Processor *proc)
{
    PR_FST;

    StackPush(&(proc->stack), proc->reg[proc->code[proc->pc + 1] - 1]);
    PR("* PUSHr(%d)\n", proc->reg[proc->code[proc->pc + 1] - 1]);
    proc->pc++;
    proc->pc++;

    PR_FED;
    return 0;
}
int InstADD  (Processor *proc) // FIXME temp
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
        return ADD_CRUSH;
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
        return SUB_CRUSH;
    }
    PR_FED;
    return 0;
}
int InstMUL  (Processor *proc)
{
    if (proc->stack.size >= 2)
    {
        StackPush(&(proc->stack), StackPop(&(proc->stack)) * StackPop(&(proc->stack)));
        PR("* MUL:\n");
        proc->pc++;
    }
    else
    {
        PR("--> Try to MUL empty stack!\n");
        return MUL_CRUSH;
    }

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
        return DIV_CRUSH;
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
        return OUT_CRUSH;
    }
    PR_FED
    return 0;
}
int InstHLT  (Processor *proc)
{
    PR_FST;
    proc->pc++;
    PR_FED;
    return HLT_CRUSH;
}



#define JMP_AND_VER(name)                                                                       \
    if ((proc->code[proc->pc + 1] >= 0) && (proc->code[proc->pc + 1] < proc->code_size))        \
        {proc->pc = proc->code[proc->pc + 1];}                                                  \
    else                                                                                        \
        {PR("--> Try to " name " out of bounds!\n"); return JMP_CRUSH;}

#define JMP_IF(cmp, name) int st_size = 0; if (((st_size = proc->stack.size) >= 2) && (StackPop(&(proc->stack)) cmp StackPop(&(proc->stack)))) \
                            {JMP_AND_VER(name);} \
                          else \
                            {if (st_size < 2) {PR("--> Try to compare empty stack in " name "!\n");} \
                                else {PR("* Comparison is incorrect.");} proc->pc += 2;} 


int InstJMP  (Processor *proc)
{
    PR_FST;

    if ((proc->code[proc->pc + 1] >= 0) && (proc->code[proc->pc + 1] < proc->code_size))
    {
        proc->pc = proc->code[proc->pc + 1];
    }
    else
    {
        PR("--> Try to JMP out of bounds!\n");
        return JMP_CRUSH;
    }

    PR_FED;
    return 0;
}
int InstJB   (Processor *proc)
{
    PR_FST;

    JMP_IF(<, "JB");

    PR_FED;
    return 0;
}
int InstJBE  (Processor *proc) // FIXME to define 
{
    PR_FST; 

    JMP_IF(<=, "JBE");

    PR_FED;
    return 0;
}
int InstJE   (Processor *proc)
{
    PR_FST;

    JMP_IF(==, "JE");

    PR_FED;
    return 0;
}
int InstJNE  (Processor *proc)
{
    PR_FST;

    JMP_IF(!=, "JNE");

    PR_FED;
    return 0;
}
int InstJAE  (Processor *proc)
{
    PR_FST;

    JMP_IF(>, "JAE");

    PR_FED;
    return 0;
}
int InstJA   (Processor *proc)
{
    PR_FST;

    JMP_IF(>=, "JA");

    PR_FED;
    return 0;
}


void PrintErrorCode(int err_code)
{
    switch (err_code)
    {
        case PUSH_CRUSH:
            PR("--> Error in PUSH!\n");
            break;

        case POP_CRUSH:
            PR("--> Error in POP!\n");
            break;

        case POPr_CRUSH:
            PR("--> Error in POPr!\n");
            break;

        case PUSHr_CRUSH:
            PR("--> Error in PUSHr!\n");
            break;

        case JMP_CRUSH:
            PR("--> Error in JMP!\n");
            break;

        case ADD_CRUSH:
            PR("--> Error in ADD!\n");
            break;

        case SUB_CRUSH:
            PR("--> Error in SUB!\n");
            break;

        case MUL_CRUSH:
            PR("--> Error in MUL!\n");
            break;

        case DIV_CRUSH:
            PR("--> Error in DIV!\n");
            break;

        case OUT_CRUSH:
            PR("--> Error in OUT!\n");
            break;

        case HLT_CRUSH:
            PR("* Program successfully completed.\n");
            break;

        default:
            PR("--> Unknown error code: %d!\n", err_code);
            break;
    }
}