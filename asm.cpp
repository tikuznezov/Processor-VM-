// #define _DEBUG
#include "asm.h"
//------------------------------------------------------------------------------------
#include "log.cpp"
#include "vm.cpp"
#include "/Applications/VsСode/Onegin/fileread.cpp"
//------------------------------------------------------------------------------------

#undef PRINT_ELEM_T(x)
#undef SCANF_ELEM_T(x)

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
                        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
                        {
                            hash_tab[i].hash = HashCom((Instructions[i]).name);
                            hash_tab[i].inst = Instructions[i];
                            // PR("Inst '%8s' hash = %3d;\n", hash_t[i].inst.name, hash_t[i].hash);
                        }
                        PR("Run Qsort\n");
                        qsort(hash_tab, COUNT_OF_INSTRUCTIONS, sizeof(InstHash), CompUInt);
                        PR("End Qsort\n");
                        for (int i = 0; i < COUNT_OF_INSTRUCTIONS; i++)
                        {
                            PR("%s = [%d]\n", hash_tab[i].inst.name, hash_tab[i].hash);
                        }

    // Init source file names
    File_names fname = {.src   = (argc > 1) ? argv[1] : "src1.asm",
                        .t_out = (argc > 2) ? argv[2] : "code.txt",
                        .b_out = (argc > 3) ? argv[3] : "code.bin",};

    File src = ReadSringsFromFile(fname.src);

    // FIXME label init
    int labels[LABELS_COUNT] = {};
    for (int i = 0; i < LABELS_COUNT; i++)
        labels[i] = -1;

    const int repeat_count = 2;
    for (int a = 0; a < repeat_count; a++)
    {
        AsmReadWrite(&src, fname, labels, hash_tab);
    }
    FreeFile(&src);

    return 0;
}


// FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------------------------------

Instruction GetCommand(const char *str_beg, InstHash *hash_tab)
{
    Instruction inst = {};

    char inst_name[MAX_COM_LEN] = "";

    if (sscanf(str_beg, "%[A-Z]", inst_name))
    {
        PR("\n* Inst %s;\n", inst_name);
        uint hash = HashCom(inst_name);
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

int GetArgs(char *str_beg, int *labels, int *arg1, char *com_num)
{
    char trash[MAX_COM_LEN] = "";
    int trsh[2];

    int read_el_c = 0;

    // stack call
    if (2 == sscanf(str_beg, "%[A-Zrm] %d \n", trash, arg1))
    {
        // 80
        PR("* GET Stack operation = %d;\n", *arg1);
        *com_num |= 0x80;
        read_el_c++;
    }
    // jmp label
    else if (3 == sscanf(str_beg, "%[A-Zrm] %[:]%d \n", trash, trash, arg1))
    {
        *arg1 = labels[*arg1];
        *com_num |= 0x80;
        PR("* JMP label = %d;\n", *arg1);
        read_el_c++;
    }
    // memory access
    else if (2 == sscanf(str_beg, "%[A-Zrm] [%d] \n", trash, arg1))
    {
        PR("* MEM access [%d];\n", *arg1);
        *com_num |= 0x20;
        // 20
        read_el_c++;
    }
    // register access
    else if (3 == sscanf(str_beg, "%[A-Zrm] %1[a-g]%1[x] \n", trash, arg1, trash))
    {
        PR("* REG access = [%d] = %cx;\n", *arg1 - 'a', *arg1);
        *arg1 = *arg1 - 'a';
        *com_num |= 0x40;
        // 40
        read_el_c++;
    }
    else
    {
        // PYELLOW PR("<%s>\n", str_beg); DEF_COL
        PR("There's no args. Or args not found.\n");
        return 0;
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

int AsmReadWrite(File *src, File_names fname, int *labels, InstHash *hash_tab)
{

    // FILE *txt_out = fopen(fname.t_out, "w");
    int bin_out = open(fname.b_out, O_TRUNC | O_CREAT | O_WRONLY, 0755);
    File bin = {};
    FileSize(fname.b_out, &bin);

    char *code = (char *) calloc(bin.size, sizeof(char));

    PR("* Files created.\n");

    Instruction cur_com = Instructions[0];
    int label;
    int  arg1;
    char temp_name[MAX_COM_LEN] = "";
    size_t cur_com_num = 0;
    size_t cur_index = 0;
    int count_read_el = 0;

    printf("Press ENTER to continue...");
    getchar();

    for (size_t i = 0; i <= src->str_count; i++)
    {

        label = -1;
        arg1 = 0;

        char *str_beg = src->str_pointers[i].beg;

        if (str_beg[0] == '\n' || str_beg[0] == ' ')
            continue;

        if (FindLabel(str_beg, &label, labels, cur_index, i)) continue;

        cur_com = GetCommand(str_beg, hash_tab);
        code[cur_index] = cur_com.num & 0x1F;
        cur_com_num++;
        if (GetArgs(str_beg, labels, &arg1, &(code[cur_index++])))
        {
            code[cur_index++] = (arg1 >> 24) & 0xFF;
            code[cur_index++] = (arg1 >> 16) & 0xFF;
            code[cur_index++] = (arg1 >>  8) & 0xFF;
            code[cur_index++] = (arg1 >>  0) & 0xFF;
        }
    }
                for (int i = 0; i < cur_index; i++)
                    {printf("[%X] -- [%3d]\n", code[i], i);}
                printf("\n");

    write(bin_out, code, cur_index);

    return 0;
}

bool FindLabel(char *str_beg, int *label, int *labels, int cur_index, size_t string_num)
{
    if (str_beg[0] == ':')
    {
        PRED
        PR("* Find label on %zu string.\n", string_num);
        sscanf(str_beg, ":%d", label);
        DEF_COL
        PR("* Label[%d] = <%d>\n", *label, cur_index);
        if ((*label >= 0) && (*label < LABELS_COUNT))
        {
            labels[*label] = cur_index;
            PGREEN
            PR("* Label was successfully saved.\n");
        }
        else
        {
            PRED
            PR("\n\n--> Libel wasn't saved!!!\n\n");
            return false;
        }
        DEF_COL;
        return true;
    }
    return false;
}

uint HashCom(const char *com_name)
{
    PR("Hash (%8s) = ", com_name);
    uint len = strnlen(com_name, MAX_COM_LEN);
    uint hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash += com_name[i];
    }
    hash %= 51;
    hash += 6;
    PR("%3d;\n", hash);
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

int GetCommandV1(const char *str)
{
    PR_FST

    int com = UNKNOWN;
    size_t len = StrLen(str);

    if (_PRT)
    {
        PR("str = <")
        for (size_t i = 0; i < len; i++)
            PR("%c", str[i]);
        PR(">\n");
        PR("len = %zu;\n", len);
    }

    if ((len >= 4) && !strncmp(str, "PUSH", 4))
    {
        com = PUSH;
        PR("* get PUSH\n");
    }
    else if ((len >= 3) && !strncmp(str, "ADD", 3))
    {
        com = ADD;
        PR("* get ADD\n");
    }
    else if ((len >= 3) && !strncmp(str, "DIV", 3))
    {
        com = DIV;
        PR("* get DIV\n");
    }
    else if ((len >= 3) && !strncmp(str, "OUT", 3))
    {
        com = OUT;
        PR("* get OUT\n");
    }
    else if ((len >= 3) && !strncmp(str, "HLT", 3))
    {
        com = HLT;
        PR("* get HLT\n");
    }
    else if ((len >= 3) && !strncmp(str, "SUB", 3))
    {
        com = SUB;
        PR("* get SUB\n");
    }
    else
    {
        // FIXME add line and file name
        PR("--> Unknown command!\n");
    }
    PR_FED;
    return com;
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








#undef stack_elem