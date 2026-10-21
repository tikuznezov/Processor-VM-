#include "vm.h"
#include "log.cpp"
#include "/Applications/VsСode/Stack/stack.h"
#include "/Applications/VsСode/Stack/stack.cpp"

int error = 0;

// считывает все в указанный буфер
// читает аргументы и команды в массив интов
int ReadTxtFile(const char *filename, int **buffer_p, size_t *sizeof_buffer)
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

int ReadBinFile(const char *filename, char **buffer_p, size_t *sizeof_buffer)
{
    PR_FST;

    int src_file = open(filename, O_RDONLY);

    size_t buf_size = SizeofFile(filename);
    PR("* starting calloc...\n");
    char *buffer = (char *) calloc(buf_size, sizeof(char));
    if (buffer == NULL)
    {
        printf("--> Buffer creation error (calloc error)!\n");
    }
    PR("* calloc comp\n");

    ssize_t read_bite_count = read(src_file, buffer, buf_size);

    *sizeof_buffer = buf_size / sizeof(char);
    *buffer_p = buffer;

    for (int i = 0; i < *sizeof_buffer; i++)
    {
        PR("[%d] ", (buffer)[i]);
    }

    PR_FED;
    return 0;
}
// группа функций соотв номерам команд

// выполняет бинарный файл
int RunBin(Processor proc)
{
    PR_FST;

    StackCtor(&(proc.stack), 2);
    StackCtor(&(proc.ret_address), 2);

    // проход по массиву с кодом по индексу ip/pc

    // limit instruction calls to avoid an inf cycle
    int count_inst_calls = 0;
    int err = 0;

    // qsort((void *)Instructions, COUNT_OF_INSTRUCTIONS, sizeof(Instruction), CompInstUp);

    // FIXME bin search
    while ((proc.pc < proc.code_size) && (count_inst_calls <= MAX_INSTRUCTION_CALLS))
    {
        count_inst_calls++;

        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
        {
            if ((proc.code[proc.pc] & 0x1F) == ((Instructions[i]).num))
                err = (Instructions[i]).execute(&proc); // call function
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

// display error codes
void PrintErrorCode(int err_code)
{
    switch (err_code)
    {
        case UNKNOWN_CRUSH:
            PR("--> Error: Unknown operation!\n");
            break;

        case PUSH_CRUSH:
            PR("--> Error in PUSH!\n");
            break;

        case POP_CRUSH:
            PR("--> Error in POP!\n");
            break;

        case JMP_CRUSH:
            PR("--> Error in JMP!\n");
            break;

        case JB_CRUSH:
            PR("--> Error in JB!\n");
            break;

        case JBE_CRUSH:
            PR("--> Error in JBE!\n");
            break;

        case JE_CRUSH:
            PR("--> Error in JE!\n");
            break;

        case JNE_CRUSH:
            PR("--> Error in JNE!\n");
            break;

        case JAE_CRUSH:
            PR("--> Error in JAE!\n");
            break;

        case JA_CRUSH:
            PR("--> Error in JA!\n");
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

        case IN_CRUSH:
            PR("--> Error in IN!\n");
            break;

        case CALL_CRUSH:
            PR("--> Error in CALL!\n");
            break;

        case RET_CRUSH:
            PR("--> Error in RET!\n");
            break;

        default:
            PR("--> Unknown error code: %d!\n", err_code);
            break;
    }
}

// return file size in bites
size_t SizeofFile(const char *file_name)
{
    PR_FST;
    assert(file_name);

    struct stat statistics = {};
    ssize_t fsize = -1;

    if (stat(file_name, &statistics) == 0)
    {
        fsize = (ssize_t)statistics.st_size;
        PR("Размер файла: %zu байт\n", (long)fsize);
    }
    else 
    {
        perror("Ошибка при вызове stat");
    }

    PR_FED;
    return fsize;
}

// display processor data
void PR_PROC(Processor *proc)
{
    PYELLOW;
    PR("PROCESSOR:\n");
    PR("* RUN pc = (%zu)\n", proc->pc);
    DEF_COL;
    PR("Code = ");
    for (int i = 0; i < proc->code_size; i++)
    {
        if (i == proc->pc) PRED;
        PR("[%d]", proc->code[i])
        if (i == proc->pc) DEF_COL;
    }
    PR("\n");
    for (int i = 0; i < sizeof(proc->reg)/sizeof(proc->reg[0]); i++)
        PR("Reg[%c] = %lg;\n", i + 'A', (double)proc->reg[i]);


    PR("Stack = ");
    for (int i = 0; i < proc->stack.size; i++)
    {
        PR("[%lg] ", proc->stack.data[i])
    }
    PR(";\n");

    PYELLOW;
    PR("PROCESSOR completed;\n")
    DEF_COL;
    return;
}


// PROCESSOR FUNCTIONS
//----------------------------------------------------------------------------------------------------------------------

#define read_arg (proc->code[++proc->pc] << 24) | (proc->code[++proc->pc] << 16) | (proc->code[++proc->pc] << 8) | proc->code[++proc->pc];

// COMP функции должны обрабатывать после себя такое количество аргументов, сколько ими предусмотрено и возвращать 
// указатель на следующую инструкцию 

int InstUNKNOWN(Processor *proc)
{
    printf("--> Unknown instruction!\n");
    return 1;
}
int InstPUSH (Processor *proc)
{
    PR_FST;

    PR_PROC(proc);


    // STACK
    if (proc->code[proc->pc] & 0x80)
    {
        int arg = read_arg;
        PR("* PUSH to stack (%d);\n", arg);
        StackPush(&(proc->stack), arg);
    }
    // REGISTER
    else if (proc->code[proc->pc] & 0x40)
    {
        int reg_i = read_arg;
        stack_elem arg = proc->reg[reg_i];
        PR("* PUSH from REG[%cx] (%lg);\n", reg_i + 'a', arg);
        StackPush(&(proc->stack), arg);
    }
    // MEMORY
    else if (proc->code[proc->pc] & 0x20)
    {
        int mem_i = read_arg;
        stack_elem arg = proc->RAM[mem_i];
        PR("* PUSH from RAM[%d] (%lg);\n", mem_i, arg);
        StackPush(&(proc->stack), arg);
    }
    else
    {
        PR("--> PUSH has'n arguments!\n");
        return 1;
    }
    proc->pc++;

    PR_FED;
    return 0;
}
int InstPOP  (Processor *proc)
{
    PR_FST;

    PR_PROC(proc);

    if (proc->stack.size > 0)
    {
        PR("Stack size = %zu;\n", proc->stack.size);
        // STACK
        if (proc->code[proc->pc] & 0x80)
        {
            PR("--> POP has stack arguments!\n");
            return 1;
        }
        // REGISTER
        else if (proc->code[proc->pc] & 0x40)
        {
            int reg_i = read_arg;
            stack_elem arg = StackPop(&(proc->stack));
            proc->reg[reg_i] = arg;
            PR("* POP to REG[%cx] (%lg);\n", reg_i + 'a', arg);
        }
        // MEMORY
        else if (proc->code[proc->pc] & 0x20)
        {
            int mem_i = read_arg;
            stack_elem arg = StackPop(&(proc->stack));
            proc->RAM[mem_i] = arg;
            PR("* POP to RAM[%d] (%lg);\n", mem_i, arg);
        }
        else
        {
            stack_elem arg = StackPop(&proc->stack);
            PR("* POP (%lg) from stack", arg);
        }
        proc->pc++;
    }
    else
    {
        PR("--> Try to POP empty stack!\n");
        return POP_CRUSH;
    }

    PR_FED;
    return 0;
}
int InstADD  (Processor *proc) // FIXME temp
{
    PR_FST;
    PR_PROC(proc);
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
    PR_FST;
    PR_PROC(proc);
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
    PR_FST
    PR_PROC(proc);
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
int InstDIV  (Processor *proc) // FIXME div by zero
{
    PR_FST;
    PR_PROC(proc);
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
    PR_PROC(proc);
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
    PR_PROC(proc);
    // proc->pc++;
    PR_FED;
    return HLT_CRUSH;
}
int InstIN   (Processor *proc) // FIXME clean buffer
{
    PR_FST;
    PR_PROC(proc);

    stack_elem input;
    printf("Called IN. Input any number: ");
    SCANF_ELEM_T(input);
    ClearTerm();

    StackPush(&(proc->stack), input);
    PR("* IN(%d)\n", proc->code[proc->pc]);
    proc->pc++;

    PR_FED;
    return 0;
}
int InstCALL (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);

    StackPush(&proc->ret_address, proc->pc + 2);
    InstJMP(proc);

    PR_FED;
    return 0;
}
int InstRET  (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);

    if (proc->ret_address.size >= 1)
    {
        int ret_ad = StackPop(&proc->ret_address);
        proc->pc = ret_ad;
    }
    else
    {
        PR("--> Try POP empty stack in RET!\n");
        return RET_CRUSH;
    }

    PR_FED;
    return 0;
}

// JUMPS
//--------------------------------------------------------------------------------------------------

#define JMP_AND_VER(name)                                                                   \
if ((mark >= 0) && (mark < proc->code_size))                                                \
{                                                                                           \
    proc->pc = mark;                                                                        \
}                                                                                           \
else                                                                                        \
{                                                                                           \
    PR("--> Try to " name " out of bounds!\n");                                             \
    return JMP_CRUSH;                                                                       \
}

#define JMP_IF(cmp, name)                                                                   \
PR_FST;                                                                                     \
int st_size = 0;                                                                            \
PR_PROC(proc);                                                                              \
if (((st_size = proc->stack.size) >= 2))                                                    \
{                                                                                           \
    int mark = read_arg;                                                                    \
    stack_elem el1 = StackPop(&(proc->stack));                                              \
    stack_elem el2 = StackPop(&(proc->stack));                                              \
    PR("* " name " if (%lg " #cmp " %lg)\n", el2, el1);                                     \
    if (el2 cmp el1) {JMP_AND_VER(name);}                                                   \
    else {PR("* Comparison is incorrect."); proc->pc += 1;}                                 \
}                                                                                           \
else                                                                                        \
{                                                                                           \
    if (st_size < 2) {PR("--> Try to compare empty stack in " name "!\n");}                 \
    proc->pc += 5;                                                                          \
}                                                                                           \
PR_FED;                                                                                     \
return 0;

int InstJMP  (Processor *proc)
{
    PR_FST;

    if ((proc->code[proc->pc + 1] >= 0) && (proc->code[proc->pc + 1] < proc->code_size))
    {
        size_t mark = read_arg;
        PR("* JMP to pc[%d];\n", mark);
        proc->pc = mark;
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
    JMP_IF(<, "JB");
}
int InstJBE  (Processor *proc)
{
    JMP_IF(<=, "JBE");
}
int InstJE   (Processor *proc) // FIXME epsilon equal
{
    JMP_IF(==, "JE");
}
int InstJNE  (Processor *proc)
{
    JMP_IF(!=, "JNE");
}
int InstJAE  (Processor *proc)
{
    JMP_IF(>=, "JAE");
}
int InstJA   (Processor *proc)
{
    JMP_IF(>, "JA");
}


int CompInstUp(const void *a, const void *b) 
{
    int arg1 = ((Instruction *)a)->num;
    int arg2 = ((Instruction *)b)->num;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------

