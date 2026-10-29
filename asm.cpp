// #define _DEBUG
#include "asm.h"
//------------------------------------------------------------------------------------
#define _PRT 0 //   zero is off \ other is on
#include "log.cpp"
#include "/Applications/VsСode/Onegin/fileread.cpp"
//------------------------------------------------------------------------------------

#undef PRINT_ELEM_T
#undef SCANF_ELEM_T

#define stack_elem char
#define PRINT_ELEM_T(x) printf("%d", x);
#define SCANF_ELEM_T(x) scanf("%d", &x);



/*
1) считываем команду
2) вторым символом считываем аргумент, обрабатываем его наличие
3) переводим команду в номер инструкции, аргумент оставляем без изменений
4) записываем команду char-ом, аргумент интом
4.0) разбиение на строки не нужно, логику обработки аргумента оставляем функции выполняющей инструкцию 

- можно не трогать свитч, пока и так сойдет
! реализовать запись бинарника без разбиения на строки 

git clone git@github.com:tikuznezov/Processor-VM-.git
*/

// COMP cut to func
// COMP bin file info

// COMP argc argv
int main(int argc, char *argv[])
{
    // sort hash
    InstHash hash_tab[COUNT_OF_INSTRUCTIONS] = {};
    InitHashT(hash_tab);

    // Init source file names
    File_names fname = {.src   = (argc > 1) ? argv[1] : "src1.asm",
                        .b_out = (argc > 2) ? argv[2] : "code.bin",
                        .t_out = (argc > 3) ? argv[3] : "code.txt",};

    File src = ReadSringsFromFile(fname.src);

    // FIXME label init
    Label labels[LABELS_COUNT] = {};
    for (int i = 0; i < LABELS_COUNT; i++)
    {
        labels[i].hash = -1;
        labels[i].link_command = -1;
    }

    const int repeat_count = 2;
    for (int a = 0; a < repeat_count; a++)
    {
        AsmReadWrite(&src, fname, labels, hash_tab);
    }

    return 0;
}


// FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------------------------------

InstHash* InitHashT(InstHash *hash_tab)
{
    assert(hash_tab);

    for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
    {
        hash_tab[i].hash = HashString((Instructions[i]).name);
        hash_tab[i].inst = Instructions[i];
        // PR("Inst '%8s' hash = %3d;\n", hash_t[i].inst.name, hash_t[i].hash);
    }

    // Debug info
    #if _PRT == 1
        PR("* Run Qsort\n");
        qsort(hash_tab, COUNT_OF_INSTRUCTIONS, sizeof(InstHash), CompUInt);
        PR("* End Qsort\n");
        PR("* SORTED HASH TABLE:\n")
        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
        {
            PR("|-----------------|\n");
            PR("| %8s = %4d |\n", hash_tab[i].inst.name, hash_tab[i].hash);
        }
        PR("|-----------------|\n");
    #else
        qsort(hash_tab, COUNT_OF_INSTRUCTIONS, sizeof(InstHash), CompUInt);
    #endif

    for (int i = 1; i < COUNT_OF_INSTRUCTIONS; i++)
    {
        if (hash_tab[i].hash == hash_tab[i-1].hash)
        {
            PRED;
            printf("--> Find collision!!!\n");
            PR("\t| Hash_table[%2d] | %8s | %4d |\n", i-1, hash_tab[i-1].inst.name, hash_tab[i-1].hash);
            PR("\t| Hash_table[%2d] | %8s | %4d |\n", i  , hash_tab[  i].inst.name, hash_tab[  i].hash);
            DEF_COL;
        }
        else
        {
            if (i == COUNT_OF_INSTRUCTIONS-1)
                PR("* There's no collisions.\n");
        }
    }

    return hash_tab;
}

Instruction GetCommand(const char *str_beg, const InstHash *hash_tab)
{
    assert(str_beg);
    assert(hash_tab);

    Instruction inst = {};

    char inst_name[MAX_COM_LEN] = "";

    if (sscanf(str_beg, "%[A-Z]", inst_name) == 1)
    {
        PR("\n* Inst \"%s\";\n", inst_name);
        uint hash = HashString(inst_name);
        InstHash *found = (InstHash*) bsearch(&hash, hash_tab, COUNT_OF_INSTRUCTIONS, sizeof(InstHash), CompUInt);
        if (found != NULL)
        {
            PYELLOW;
            PR("* Get hash instruction '%s'(%d), hash = %u;\n", (*found).inst.name, (*found).inst.num, (*found).hash);
            DEF_COL;
            return (*found).inst;
        }
        else
        {
            PRED;
            PR("--> Unknown command!\n");
            DEF_COL;
        }
    }
    else
    {
        PRED;
        PR("--> There's no instruction!\n");
        DEF_COL;
    }

    return {};
}

#define ISEND(char_symbol) if ((char_symbol != '\n') && (char_symbol != ';') && (char_symbol != '\0'))      \
{                                                                                                           \
    PRED                                                                                                    \
    printf("--> Incorrect instruction format in <");                                                        \
    int i = 0; while(string_begin[i] != '\n') printf("%c", str_beg[i++]);                                   \
    printf(">\n");                                                                                          \
    return(100);                                                                                            \
    DEF_COL                                                                                                 \
}

#define FINDEND                                                                                             \
char *char_p = str_beg + shift;                                                                             \
while (*char_p == ' ' || *char_p == '\t') char_p++;                                                         \
ISEND(*char_p)

bool IsEnd(char char_symbol)
{
    if ((char_symbol != '\n') && (char_symbol != ';') && (char_symbol != '\0'))
    {
        return false;
    }
    else
        return true;
}

// int GetArgsV1(char *str_beg, int *labels, int *arg1, char *com_num)
// {
//     char trash[MAX_COM_LEN] = "";
//     int shift = 0;
//     int read_el_c = 0;
//     char buf1[MAX_STR_LENGTH] = "";
//     char buf2[MAX_STR_LENGTH] = "";
//
//     // stack call
//     if (2 == sscanf(str_beg, "%[A-Zrm] %d%n", trash, arg1, &shift))
//     {
//         FINDEND;
//         PR("* GET Stack operation = %d;\n", *arg1);
//         *com_num |= 0x80;
//         read_el_c++;
//     }
//     // jmp label
//     else if (3 == sscanf(str_beg, "%[A-Zrm] %1[:]%d%n", trash, trash, arg1, &shift))
//     {
//         FINDEND;
//         *arg1 = labels[*arg1];
//         *com_num |= 0x80;
//         PR("* JMP label = %d;\n", *arg1);
//         read_el_c++;
//     }
//     // memory access
//     else if (2 == sscanf(str_beg, "%[A-Zrm] [%d]%n", trash, arg1, &shift))
//     {
//         FINDEND;
//         PR("* MEM access [%d];\n", *arg1);
//         *com_num |= 0x20;
//         // 20
//         read_el_c++;
//     }
//     // memory[register] access
//     // else if (3 == sscanf(str_beg, "[A_Zrm] [%1[a-z]x + ]"))
//     // {
//
//     // }
//     // register access
//     else if (3 == sscanf(str_beg, "%[A-Zrm] %1[a-g]%1[x]%n", trash, arg1, trash, &shift))
//     {
//         FINDEND;
//         PR("* REG access = [%d] = %cx;\n", *arg1 - 'a', *arg1);
//         *arg1 = *arg1 - 'a';
//         *com_num |= 0x40;
//         // 40
//         read_el_c++;
//     }
//     else if (1 == sscanf(str_beg, "%[A-Zrm]%n", trash, &shift))
//     {
//         FINDEND;
//     }
//     else
//     {
//         // PYELLOW PR("<%s>\n", str_beg); DEF_COL
//         PR("There's no args. Or args not found.\n");
//         return 0;
//     }
//     return read_el_c;
// }


int GetArgs(char *string_begin, Label *labels, int *arg1, char *com_num)
{
    assert(string_begin);
    assert(labels);
    assert(arg1);
    assert(com_num);

    char *str_beg = string_begin;
    char buffer[MAX_STR_LENGTH] = "";
    char str_arg[MAX_STR_LENGTH] = "";
    int int_arg = 0;
    char char_arg = 0;

    int shift = 0;
    int read_el_c = 0;
    int scanf_mark = 0;

    PR("* RUN get args.\n")

    if (1 == sscanf(str_beg, "%[A-Za-z]%n", buffer, &shift))
    {
        PR("* Find instruction \"%s\";\n", buffer);
        // PUSH| [ax + 1]
        // ----|
        str_beg += shift;

        // PR("Searching arguments in \"%s\";\n", str_beg);

        PR("* Skip \"");
        while (str_beg[0] == ' ' || str_beg[0] == '\t')
        {
            PR("%c", str_beg[0]);
            str_beg++;
        }
        PR("\";\n");

        if (IsEnd(*str_beg))
        {
            PR("* Instruction have no arguments.\n");
            return 0;
        }

        // PUSH   |[ax + 1]
        // -------|

        // PR("Searching arguments in \"%s\";\n", str_beg);


        // Stack operation
        if (sscanf(str_beg, "%d%n", arg1, &shift) == 1)
        {
            FINDEND;
            PR("* GET Stack operation = %d;\n", *arg1);
            *com_num |= 0x80;
            read_el_c++;
        }
        // Memory[register] operation
        else if ((scanf_mark = sscanf(str_beg, "[%1[a-z]x + %d]%n", str_arg, &int_arg, &shift)) > 0)
        {
            int reg_index = str_arg[0] - 'a';

            *com_num |= 0x60;

            if (scanf_mark == 1)
            {
                // FINDEND;
                *arg1 = reg_index;
                PR("* MEM access [ reg[%d] ] <=> [%cx];\n", *arg1, str_arg[0]);
                read_el_c++;
            }
            else if (scanf_mark == 2)
            {
                // FINDEND;
                *arg1 = reg_index + int_arg;
                PR("* MEM access [ reg[%d] ] <=> [%cx + %d];\n", *arg1, str_arg[0], int_arg);
                read_el_c++;
            }
            return read_el_c;
        }
        // Register operation
        else if (sscanf(str_beg, "%1[a-z]%c%n", str_arg, buffer, &shift) == 2)
        {
            if (buffer[0] == 'x')
            {
                FINDEND;
                *arg1 = (int) str_arg[0];
                PR("* REG[%d] access = %cx;\n", *arg1 - 'a', *arg1);
                *arg1 = *arg1 - 'a';
                *com_num |= 0x40;
                read_el_c++;
            }
            else
            {
                PR("--> Incorrect register access argument!\n");
                PR("--> Error in <%s>;\n", str_beg)
                return 0; // FIXME error code 
            }
        }
        // Memory operation
        else if (sscanf(str_beg, "[%d]%n", &int_arg, &shift) == 1)
        {
            FINDEND;
            *arg1 = int_arg;
            PR("* MEM access [%d];\n", *arg1);
            *com_num |= 0x20;
            read_el_c++;
        }
        // Label
        else if (sscanf(str_beg, ":%s%n", str_arg, &shift))
        {
            FINDEND;
            PR("* Label access \"%s\";\n", str_arg);
            uint label_hash = HashString(str_arg);
            for (size_t i = 0; i < LABELS_COUNT; i++)
            {
                if (label_hash == labels[i].hash)
                {
                    *arg1 = labels[i].link_command;
                    read_el_c++;
                }
            }
            if (read_el_c == 0)
            {
                PRED PR("--> Unknown label!\n"); DEF_COL
                *arg1 = -1;
                read_el_c++;
            }
        }
        else
        {
            // PYELLOW PR("<%s>\n", str_beg); DEF_COL
            PR("There's no args. Or args not found.\n");
            return 0; // FIXME err code
        }
    }
    return read_el_c;
}

size_t StrLen(const char *str)
{
    size_t len = 0;
    while ((*str != '\0') && (*str != '\n') && (len < MAX_STR_LENGTH))
    {
        len++;
        str++;
    }

    if (len >= MAX_STR_LENGTH)
        printf("--> The maximum number of characters per line has been exceeded!\n");

    return len;
}

int AsmReadWrite(File *src, File_names fname, Label *labels, InstHash *hash_tab)
{

    // FILE *txt_out = fopen(fname.t_out, "w");
    int bin_out = open(fname.b_out, O_TRUNC | O_CREAT | O_WRONLY, 0755);
    File bin = {};
    FileSize(fname.b_out, &bin);

    char *code = (char *) calloc(bin.size, sizeof(char));

    PR("* Files created.\n");

    size_t free_label_index = 0;
    Instruction cur_com = Instructions[0];
    char temp_name[MAX_COM_LEN] = "";
    size_t cur_com_num = 0;
    size_t cur_index = 0;
    // int count_read_el = 0;

    PR("Press ENTER to continue...");
    PAUSE;

    for (size_t i = 0; i <= src->str_count; i++)
    {
        int  arg1 = 0;

        char *str_beg = src->str_pointers[i].beg;

        // skip comments and empty lines
        if (str_beg[0] == '\n' || str_beg[0] == ' ' || str_beg[0] == ';')
            continue;

        // remember label in list
        if (FindLabel(str_beg, labels, cur_index, i, &free_label_index)) 
            continue;

        // get command
        cur_com = GetCommand(str_beg, hash_tab);
        code[cur_index] = cur_com.num & 0x1F;
        cur_com_num++;

        // check args
        if (GetArgs(str_beg, labels, &arg1, &(code[cur_index++])))
        {
            PR("* Writing argument = %d;\n", arg1);
            code[cur_index++] = (arg1 >> 24) & 0xFF;
            code[cur_index++] = (arg1 >> 16) & 0xFF;
            code[cur_index++] = (arg1 >>  8) & 0xFF;
            code[cur_index++] = (arg1 >>  0) & 0xFF;
        }
    }

    if ((_PRT == 1))
        DisplayBin(cur_index, code);

    write(bin_out, code, cur_index);

    PrintLabels(labels);

    return 0;
}

// FIXME collisions
void PrintLabels(Label *labels)
{
    PR("\nLabels array:\n")
    PR("/-------NAME-|--HASH-|--LINK-\\\n");
    for (size_t i = 0; i < LABELS_COUNT; i++)
    {
        PR("|------------|-------|-------|\n");
        PR("| %10s | %5d | %5d |\n", labels[i].name, labels[i].hash, labels[i].link_command);
    }
    PR("\\------------|-------|-------/\n");
}

bool FindLabel(char *str_beg, Label *labels, int cur_index, size_t string_num, size_t *free_index)
{
    char buf[10] = "";
    char name[MAX_STR_LENGTH] = "";
    int shift = 0;

    if (sscanf(str_beg, "%[:]%s%n", buf, name, &shift) == 2)
    {
        PRED
        PR("\n* Find label on %zu string.\n", string_num);
        DEF_COL

        labels[*free_index].hash = HashString(name);
        strncpy(labels[*free_index].name, name, MAX_STR_LENGTH-1);
        labels[*free_index].link_command = cur_index;

        PR("* Label \"%s\" links to %d; Hash = %u;\n", labels[*free_index].name, labels[*free_index].link_command, labels[*free_index].hash);
        *free_index = *free_index + 1;
        PR("* Label was successfully saved.\n");
        PR("Next free index is %zu;\n", *free_index);

        return true;
    }
    return false;
}

uint HashString(const char *com_name)
{
    PR("* Hash (%8s) = ", com_name);
    uint len = strnlen(com_name, MAX_COM_LEN);
    uint hash = 0;
    int p = 53;
    int m = 9973;
    for (int i = 0; i < len; i++)
    {
        hash += com_name[i] * pow(p, i);
        hash %= m;
    }
    PR("%4d;\n", hash);
    return hash;
}

Instruction GetCommandV2(const char *command)
{
    PR_FST;

    int len = StrLen(command);
    PR("* Current command <%4s>\n", command);
    for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
    {
        if (len == StrLen(Instructions[i].name) && (!strncmp(command, Instructions[i].name, len)))
        {
            PR("* get command <%s>\n", Instructions[i].name);
            PR_FED;
            return Instructions[i];
        }
    }
    PR_FED;
    return Instructions[0];
}

int GetValue(const char *str)
{
    PR_FST;
    int value = UNREAD_VAL;
    char com[MAX_COM_LEN] = "";

    int state = sscanf(str, "%s %d", com, &value);
    if (state == 2)
    {
        PR("* Successfully get value.\n")
    }
    else
    {
        PRED printf("--> PUSH argument WASN'T READ!\n"); DEF_COL
    }

    PR_FED;
    return value;
}

int CompUInt(const void *a, const void *b)
{
    // PR_FST
    // PR("a = %u;\nb = %u;\n", ((InstHash *)a)->hash, ((InstHash *)b)->hash);
    const unsigned int arg1 = ((InstHash *)a)->hash;
    const unsigned int arg2 = ((InstHash *)b)->hash;

    // PR_FED
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void DisplayBin(size_t cur_index, char *code)
{
    PGREEN; PR("\nDo you want to display binary list? (y/n)\n"); DEF_COL;
    char trash[10] = "";
    if (1 == scanf("%[y]", trash))
    {
        PR("\n");
        PR("/======================\\\n");
        PR("|  DEC  |  HEX  | INDX |\n");
        PR("|----------------------|\n");
        for (int i = 0; i < cur_index; i++)
            {
                PR("| %4hhu  |   %02X  | %4d |\n", code[i], (unsigned char)code[i], i);
                PR("|----------------------|\n");
            }
        PR("\n");
        ClearTerm();
    }
    else
    {
        PR("\n");
    }
}






#undef stack_elem