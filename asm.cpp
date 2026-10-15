#include "vm.h"
#include "asm.h"

#include "log.cpp"
#include "vm.cpp"
#include "/Applications/VsСode/Onegin/fileread.cpp"

#define TO_STR(x) #x


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

int main(void)
{
    const char * const src_name = "src1.asm";
    const char *out_name     = "com1.txt";
    const char *out_bun_name = "code.bin";

    File src = ReadSringsFromFile(src_name);

    int labels[LABELS_COUNT] = {};
    for (int i = 0; i < LABELS_COUNT; i++)
        labels[i] = -1;


    for (int a = 0; a < 2; a++)
    {
        FILE *code_out = fopen(out_name, "w");

        int bin_out = open(out_bun_name, O_TRUNC | O_CREAT | O_WRONLY, 0755);

        PR("* Files created.\n");

        Instruction cur_com = Instructions[0];
        int label = -1;
        int cur_com_num = 0;
        char temp_name[MAX_COM_LEN] = "";
        int  arg1 = 0;
        int count_read_el = 0;

        printf("Press ENTER to continue...");
        getchar();

        for (size_t i = 0; i <= src.str_count; i++)
        {

            label = -1;
            arg1 = 0;

            char *str_beg = src.str_pointers[i].beg;

            if (str_beg[0] == '\n' || str_beg[0] == ' ')
                continue;

            if (str_beg[0] == ':')
            {
                PRED
                PR("* Find label on %zu string.\n", i);
                sscanf(str_beg, ":%d", &label);
                PR("*Label[%d] = <%d>\n", label, cur_com_num);
                labels[label] = cur_com_num;
                DEF_COL;
                continue;
            }

            count_read_el = sscanf(str_beg, "%s %d\n", temp_name, &arg1);
            PGREEN PR("* Current command = <%s>, count_read_el = %2d, arg = %4d.\n", temp_name, count_read_el, arg1); DEF_COL
            cur_com = GetCom2(temp_name);
            // printf("Read command[%3zu] = %d;\n", i, cur_com);

            if (cur_com.name[0] == 'J')
            {
                str_beg+=2;
                if (sscanf(str_beg, " :%d\n", &arg1))
                {
                    PRED PR("fprintf <%d %d>\n", cur_com.num, labels[arg1]); DEF_COL
                    fprintf(code_out, "%d %d\n", cur_com.num, labels[arg1]);
                    write(bin_out, &(cur_com.num), sizeof(cur_com.num));
                    write(bin_out, &(labels[arg1]), sizeof(labels[arg1]));
                }
                else if (sscanf(str_beg, " %d\n", &arg1))
                {
                    PBLUE PR("fprintf <%d %d>\n", cur_com.num, arg1); DEF_COL
                    fprintf(code_out, "%d %d\n", cur_com.num, arg1);
                    write(bin_out, &(cur_com.num), sizeof(cur_com.num));
                }
                cur_com_num+=2;
                continue;
            }

            if (count_read_el == 1)
            {
                PYELLOW PR("fprintf <%d>\n", cur_com.num); DEF_COL
                fprintf(code_out, "%d\n", cur_com.num);
                write(bin_out, &(cur_com.num), sizeof(cur_com.num));
                cur_com_num++;
            }
            else if (count_read_el == 2)
            {
                PYELLOW PR("fprintf <%d %d>\n", cur_com.num, arg1); DEF_COL
                fprintf(code_out, "%d %d\n", cur_com.num, arg1);
                write(bin_out, &(cur_com.num), sizeof(cur_com.num));
                write(bin_out, &(arg1), sizeof(arg1));
                cur_com_num+=2;
            }
        }
    }

    return 0;
}


Instruction GetCom2(const char *command)
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


int GetCommand(const char *str)
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