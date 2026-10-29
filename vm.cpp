#define _VM
#include "vm.h"
#include "log.cpp"
#include "Stack/stack.cpp"

#include "raylib.h"
int error = 0;

// считывает все в указанн`ый буфер
// читает аргументы и команды в массив интов

// TODO file info, watermark, header
// TODO битовые поля
// TODO intel file listing

// MAIN
//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    const char *src_bin = (argc > 1) ? argv[1] : "code.bin";
    // const char *src_txt = (argc > 1) ? argv[1] : "code.txt";
    int err = 0;
    Processor proc = {};

    ReadBinFile(src_bin, &(proc.code), &(proc.code_size));
    // ReadTxtFile(src_txt, &(proc.code), &(proc.code_size));
    err = RunBin(proc);

    if (err != 0)
    {
        if (err != HLT_CRUSH) PRED;
        PR("* Completed 'RunBin'.\n");
        PrintErrorCode(err);
        DEF_COL;
    }

    return 0;
}

// FUNCTIONS
//-----------------------------------------------------------------------------------------------------------------------

int ReadBinFile(const char *filename, char **buffer_p, size_t *sizeof_buffer)
{
    PR_FST;
    assert(filename);
    assert(buffer_p);
    assert(sizeof_buffer);

    int src_file = open(filename, O_RDONLY);

    if (src_file == -1)
    {
        PRED
        printf("Error opening file: \"%s\";\n", filename);
        perror("");
        DEF_COL
        return -1; // FIXME err code
    }

    // creation code buffer
    ssize_t buf_size = SizeofFile(filename);
    if (buf_size == -1)
    {
        printf("--> File size was calculated incorrect!\n");
        return (int)buf_size;
    }
    PR("* Start creating code buffer by calloc...\n");
    char *buffer = (char *) calloc((size_t)buf_size, sizeof(char));
    if (buffer == NULL)
    {
        printf("--> Buffer creation error (calloc error)!\n");
    }
    PR("* Code buffer successfully created.\n");

    ssize_t read_bite_count = read(src_file, buffer, (size_t)buf_size);
    PR("* Count of read byte from binary file = %zd.\n", read_bite_count);
    if (read_bite_count != buf_size)
    {
        PR("--> The file was not read to the end!\n")
        return -1;
    }

    *sizeof_buffer = (size_t)buf_size / sizeof(char);
    *buffer_p = buffer;

    for (size_t i = 0; i < *sizeof_buffer; i++)
    {
        PR("[%d] ", (buffer)[i]);
    }

    PR_FED;
    return 0;
}

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

    Instruction Insts[COUNT_OF_INSTRUCTIONS] = {};
    CopyInstructions(Insts);

    while ((proc.pc < proc.code_size) && (count_inst_calls <= MAX_INSTRUCTION_CALLS) && (err == 0))
    {
        count_inst_calls++;

        Instruction current = {};
        current.num = (proc.code[proc.pc] & 0x1F);

        // binary search function structure
        Instruction *found = (Instruction *) bsearch(&current, Instructions, COUNT_OF_INSTRUCTIONS, sizeof(Instruction), CompInstUp);
        PR("* found '%s'\n", found->name);
        // function exe
        err = found->execute(&proc); // call function
    }

    if (count_inst_calls >= MAX_INSTRUCTION_CALLS)
    {
        PRED;
        printf("--> The maximum number of instruction calls has been exceeded!!!\n");
        DEF_COL;
    }

    // TODO функция, которая производит бинарный поиск по массиву структур и выбирает нужную команду в цикле
    // она же увеличивает индекс команды на 1 после выполнения. Таким образом джамп впишет нужный индекс и процессор перескочит назад.

    PR_FED;
    return err;
}

/// @brief display error codes. Don't crush program.
/// @param err_code code of error.
void PrintErrorCode(int err_code)
{
    PRED;
    switch (err_code)
    {
        case UNKNOWN_CRUSH:
            printf("--> Error: Unknown operation!\n");
            break;

        case PUSH_CRUSH:
            printf("--> Error in PUSH!\n");
            break;

        case POP_CRUSH:
            printf("--> Error in POP!\n");
            break;

        case JMP_CRUSH:
            printf("--> Error in JMP!\n");
            break;

        case JB_CRUSH:
            printf("--> Error in JB!\n");
            break;

        case JBE_CRUSH:
            printf("--> Error in JBE!\n");
            break;

        case JE_CRUSH:
            printf("--> Error in JE!\n");
            break;

        case JNE_CRUSH:
            printf("--> Error in JNE!\n");
            break;

        case JAE_CRUSH:
            printf("--> Error in JAE!\n");
            break;

        case JA_CRUSH:
            printf("--> Error in JA!\n");
            break;

        case ADD_CRUSH:
            printf("--> Error in ADD!\n");
            break;

        case SUB_CRUSH:
            printf("--> Error in SUB!\n");
            break;

        case MUL_CRUSH:
            printf("--> Error in MUL!\n");
            break;

        case DIV_CRUSH:
            printf("--> Error in DIV!\n");
            break;

        case OUT_CRUSH:
            printf("--> Error in OUT!\n");
            break;

        case HLT_CRUSH:
            PGREEN
            PR("* Program successfully completed.\n");
            break;

        case IN_CRUSH:
            printf("--> Error in IN!\n");
            break;

        case CALL_CRUSH:
            printf("--> Error in CALL!\n");
            break;

        case RET_CRUSH:
            printf("--> Error in RET!\n");
            break;

        case SQRT_CRUSH:
            printf("--> Error in SQRT!\n");
            break;

        default:
            printf("--> Unknown error code: %d!\n", err_code);
            break;
    }
    DEF_COL
}

/// @brief calculate file size by stat structure
/// @param file_name file name
/// @return file size in bites
ssize_t SizeofFile(const char *file_name)
{
    PR_FST;
    assert(file_name);

    struct stat statistics = {};
    ssize_t fsize = -1;

    if (stat(file_name, &statistics) == 0)
    {
        fsize = (ssize_t)statistics.st_size;
        PR("Размер файла: %zd байт\n", fsize);
    }
    else 
    {
        perror("Ошибка при вызове stat");
    }

    PR_FED;
    return fsize;
}

/// @brief display processor data. Debug function. Shows registers, stack, code and current instruction.
/// @param proc Current processor struct pointer.
void PR_PROC(Processor *proc)
{
    if (_PRT != 1) return;

    PYELLOW;
    PR("PROCESSOR:\n");
    if (proc == NULL) PR("--> Processor pointer is NULL!");
    if (proc->code == NULL) PR("--> Processor code array is NULL!");
    assert(proc);
    assert(proc->code);

    PR("* RUN pc = (%zu)\n", proc->pc);
    DEF_COL;
    PR("Code = ");
    for (size_t i = 0; i < proc->code_size; i++)
    {
        if (i == proc->pc) PRED;
        PR("[%d]", proc->code[i])
        if (i == proc->pc) DEF_COL;
    }
    PR("\n");
    for (int i = 0; i < (int) (sizeof(proc->reg)/sizeof(proc->reg[0])); i++)
        PR("Reg[%c] = %lg;\n", i + 'a', (double)proc->reg[i]);


    PR("Stack = ");
    for (size_t i = 0; i < proc->stack.size; i++)
    {
        PR("[%lg] ", proc->stack.data[i])
    }
    PR(";\n");

    PYELLOW;
    PR("PROCESSOR completed;\n")
    DEF_COL;
    return;
}


/// @brief Std compare Instructions by .num 
/// @param a Instruction *a
/// @param b Instruction *b
/// @return 1 if (a > b); -1 if (a < b); 0 if equal.
int CompInstUp(const void *a, const void *b) 
{
    assert(a);
    assert(b);
    const char arg1 = ((const Instruction *)a)->num;
    const char arg2 = ((const Instruction *)b)->num;
    // PR("arg1 = %hhu, arg2 = %hhu;\n", arg1, arg2);
    
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

/// @brief compare equal of double
/// @param arg1 double number
/// @param arg2 double number
/// @return true if ((|arg1 - arg2| < EPSILON)
bool IsEqual(double arg1, double arg2)
{
    return (fabs(arg1 - arg2) < EPSILON);
}

/// @brief copy array of Instructions to another array of Instructions. Size of out array must be larger or equal src array.
/// @param Insts src - Instruction[]
/// @return error code
int CopyInstructions(Instruction *Insts)
{
    assert(Insts);

    for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
        Insts[i] = Instructions[i];

    PR("* RUN qsort...\n");
    qsort((void *) Insts, COUNT_OF_INSTRUCTIONS, sizeof(Instruction), CompInstUp);
    PR("* END qsort;\n");

    PR("|----------|------|\n");
    for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
    {
        PR("| %8s | %4hhu |\n", Insts[i].name, Insts[i].num);
        PR("|----------|------|\n");
    }
    return 0;
}

/// @brief old function which read data from src.txt to binary buffer. Unused.
/// @param filename string file name.
/// @param buffer_p buffer pointer.
/// @param sizeof_buffer variable to get count of read elements.
/// @return error code
int ReadTxtFile(const char *filename, int **buffer_p, size_t *sizeof_buffer)
{
    PR_FST;
    FILE *Code = fopen(filename, "r");

    PR("* starting calloc...\n");
    int *buffer = (int *) calloc(BUF_SIZE, sizeof(int)); // FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (buffer == NULL)
    {
        printf("--> Buffer creation error (calloc error)!\n");
    }
    PR("* calloc comp\n");

    int readmark = 1;
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
    for (size_t i = 0; i < *sizeof_buffer; i++)
        PR(" [%d]", buffer[i]);
    PR(";\n");

    PR_FED;
    return 0;
}


// PROCESSOR INSTRUCTIONS
//----------------------------------------------------------------------------------------------------------------------

// read 4 char to int
#define read_arg (int)(((unsigned char)proc->code[++proc->pc] << 24) | ((unsigned char)proc->code[++proc->pc] << 16) | ((unsigned char)proc->code[++proc->pc] << 8) | (unsigned char)proc->code[++proc->pc]);

// COMP функции должны обрабатывать после себя такое количество аргументов, сколько ими предусмотрено и возвращать указатель на следующую инструкцию 
int InstUNKNOWN(Processor *proc)
{
    PR_FST;
    PR_PROC(proc);

    printf("--> Unknown instruction!\n");
    return UNKNOWN_COMMAND;

    PR_FED;
}
int InstPUSH (Processor *proc)
{
    PR_FST;

    PR_PROC(proc);

    // MEMORY[REGISTER]
    if ((proc->code[proc->pc] & 0x60) == 0x60)
    {
        int reg_index = (int) read_arg;
        int mem_index = (int) proc->reg[reg_index];

        stack_elem arg = proc->ram[mem_index];
        PR("* PUSH from RAM[%cx] (%lg);\n", reg_index + 'a', arg);
        StackPush(&(proc->stack), arg);
    }

    // STACK
    else if (proc->code[proc->pc] & 0x80)
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
        stack_elem arg = proc->ram[mem_i];
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

        if (((proc->code[proc->pc] & 0x60)) == 0x60)
        {
            int reg_index = (int) read_arg;
            int mem_index = (int) proc->reg[reg_index];

            stack_elem arg = StackPop(&(proc->stack));
            proc->ram[mem_index] = arg;
            PR("* POP to RAM[%d] <=> RAM[%cx] = (%lg);\n", mem_index, reg_index + 'a', arg);
        }
        // STACK
        else if (proc->code[proc->pc] & 0x80)
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
            proc->ram[mem_i] = arg;
            PR("* POP to RAM[%d] (%lg);\n", mem_i, arg);
        }
        else
        {
            stack_elem arg = StackPop(&proc->stack);
            PR("* POP (%lg) from stack", arg);
            return (int)(arg = 0);
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
int InstADD  (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);
    if (proc->stack.size >= 2)
    {
        stack_elem arg1 = StackPop(&(proc->stack));
        stack_elem arg2 = StackPop(&(proc->stack));

        StackPush(&(proc->stack), arg1 + arg2);
        PR("* ADD(%lg + %lg) = %lg;", arg1, arg2, arg1 + arg2);
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
        stack_elem arg1 = StackPop(&(proc->stack));
        stack_elem arg2 = StackPop(&(proc->stack));

        StackPush(&(proc->stack), arg2 - arg1);
        PR("* SUB(%lg - %lg) = %lg;\n", arg2, arg1, arg2-arg1);
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
        stack_elem arg1 = StackPop(&(proc->stack));
        stack_elem arg2 = StackPop(&(proc->stack));

        StackPush(&(proc->stack), arg1 * arg2);
        PR("* MUL(%lg * %lg) = %lg;\n", arg1, arg2, arg1*arg2);
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
    PR_FST;
    PR_PROC(proc);
    if (proc->stack.size >= 2)
    {
        stack_elem arg1 = StackPop(&(proc->stack));
        stack_elem arg2 = StackPop(&(proc->stack));

        if (IsEqual(arg1, 0))
        {
            PRED PR("\n\n--> Division by zero!\n\n  "); DEF_COL
            return DIV_CRUSH;
        }

        StackPush(&(proc->stack), arg2 / arg1);
        PR("* DIV(%lg / %lg) = %lg;\n", arg2, arg1, arg2/arg1);
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
        stack_elem arg1 = StackPop(&(proc->stack));
        PR("* OUT(%lg)\n", arg1);
        printf("Result = ");
        PRINT_ELEM_T(arg1);
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
int InstIN   (Processor *proc)
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

    StackPush(&proc->ret_address, proc->pc + COM_LEN + ARG_LEN);
    int err = InstJMP(proc);

    PR_FED;
    return err;
}
int InstRET  (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);

    if (proc->ret_address.size >= 1)
    {
        size_t ret_ad = (size_t) StackPop(&proc->ret_address);
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
int InstSQRT (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);
    if (proc->stack.size >= 1)
    {
        stack_elem arg1 = StackPop(&(proc->stack));
        if (arg1 < 0)
            return SQRT_CRUSH;
        PR("* SQRT(%lg) = %lg;\n", arg1, sqrt(arg1));
        stack_elem res = sqrt(arg1);
        StackPush(&(proc->stack), res);
        
        proc->pc++;
    }
    else
    {
        PR("--> Try to SQRT empty stack!\n");
        return SQRT_CRUSH;
    }
    PR_FED;
    return 0;
}

// JUMPS
//--------------------------------------------------------------------------------------------------

int InstJMP  (Processor *proc)
{
    PR_FST;
    assert(proc);
    size_t mark = (size_t)read_arg;
    if ((mark >= 0) && (mark < proc->code_size))
    {
        PR("* JMP to pc[%zu];\n", mark);
        proc->pc = mark;
    }
    else
    {
        PR("--> Try to JMP[%zu] out of bounds!\n", mark);
        return JMP_CRUSH;
    }
    
    PR_FED;
    return 0;
}
int InstJE   (Processor *proc)
{
    PR_FST;
    assert(proc);
    size_t st_size = 0;
    PR_PROC(proc);
    if (((st_size = proc->stack.size) >= 2))
    {
        stack_elem el1 = StackPop(&(proc->stack));
        stack_elem el2 = StackPop(&(proc->stack));
        PR("* JE if (%lg == %lg)\n", el2, el1);
        if (IsEqual(el1, el2))
        {
            return InstJMP(proc);
        }
        else {PR("* Comparison is incorrect."); proc->pc += COM_LEN + ARG_LEN;}
    }
    else
    {
        if (st_size < 2) {PR("--> Try to compare empty stack in JE!\n");}
        proc->pc += COM_LEN + ARG_LEN;
    }
    PR_FED;
    return 0;
}
int InstJNE  (Processor *proc)
{
    PR_FST;
    assert(proc);
    size_t st_size = 0;
    PR_PROC(proc);
    if (((st_size = proc->stack.size) >= 2))
    {
        stack_elem el1 = StackPop(&(proc->stack));
        stack_elem el2 = StackPop(&(proc->stack));
        PR("* JNE if (%lg != %lg)\n", el2, el1);
        if (!IsEqual(el1, el2))
        {
            return InstJMP(proc);
        }
        else {PR("* Comparison is incorrect."); proc->pc += COM_LEN + ARG_LEN;}
    }
    else
    {
        if (st_size < 2) {PR("--> Try to compare empty stack in JE!\n");}
        proc->pc += COM_LEN + ARG_LEN;
    }
    PR_FED;
    return 0;
}


#define JMP_AND_VER(name){                                                                  \
    if ((mark >= 0) && (mark < proc->code_size))                                                \
    {                                                                                           \
        proc->pc = mark;                                                                        \
    }                                                                                           \
    else                                                                                        \
    {                                                                                           \
        PR("--> Try to " name " out of bounds!\n");                                             \
        return JMP_CRUSH;                                                                       \
    }                                                                                           \
}
#define JMP_IF(cmp, name){                                                                  \
PR_FST;                                                                                     \
assert(proc);                                                                               \
size_t st_size = 0;                                                                         \
PR_PROC(proc);                                                                              \
if (((st_size = proc->stack.size) >= 2))                                                    \
{                                                                                           \
    size_t mark = (size_t)read_arg;                                                         \
    stack_elem el1 = StackPop(&(proc->stack));                                              \
    stack_elem el2 = StackPop(&(proc->stack));                                              \
    PR("* " name " if (%lg " #cmp " %lg)\n", el2, el1);                                     \
    if (el2 cmp el1) {JMP_AND_VER(name);}                                                   \
    else {PR("* Comparison is incorrect."); proc->pc += COM_LEN;}                           \
}                                                                                           \
else                                                                                        \
{                                                                                           \
    if (st_size < 2) {PR("--> Try to compare empty stack in " name "!\n");}                 \
    proc->pc += COM_LEN + ARG_LEN;                                                          \
}                                                                                           \
PR_FED;                                                                                     \
return 0;                                                                                   \
}

int InstJB   (Processor *proc)
{
    JMP_IF(<, "JB");
}
int InstJBE  (Processor *proc)
{
    JMP_IF(<=, "JBE");
}
int InstJAE  (Processor *proc)
{
    JMP_IF(>=, "JAE");
}
int InstJA   (Processor *proc)
{
    JMP_IF(>, "JA");
}

// DISPLAY FUNCTIONS
//--------------------------------------------------------------------------------------------------

#define InWind(x_len, y_len, name) InitWindow(x_len, y_len, name); window_state = true;

#define ClWind CloseWindow(); window_state = false;

int InstDISP (Processor *proc)
{
    PR_FST;
    PR_PROC(proc);

    for (int y = 0; y < SC_HEIGHT; y++)
    {
        for (int x = 0; x < SC_WEIGHT; x++)
        {
            switch ((int) (proc->ram[y*SC_WEIGHT + x]))
            {
                case 1:
                {
                    printf("&");
                    break;
                }
                case 0:
                {
                    printf("`");
                    break;
                }
                default:
                {
                    printf(".");
                    break;
                }
            }
        }
        printf("\n");
        // sleep(1);
    }
    proc->pc++;

    PR_FED;
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------