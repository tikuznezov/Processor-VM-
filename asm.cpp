#include "vm.h"
#include "asm.h"

#include "log.cpp"
#include "/Applications/VsСode/Onegin/fileread.cpp"

    // open file input
    // open file output

    // for lines in file 
    //     read line 
    //     transcribe command 
    //     if () get command

    //     write to output

    // close files


int main(void)
{
    const char * const src_name = "src1.txt";
    const char *out_name = "com1.txt";
    // FILE *CodeSrc = fopen(src_name, "r");
    FILE *code_out = fopen(out_name, "w");

    File src = ReadSringsFromFile("src1.txt");

    // printf("len = %zu\n", StrLen("lol"));

    // int cmd = GetCommand("PUSH");
    // printf("com = %d\n", cmd);

    int cur_com = UNKNOWN;
    for (size_t i = 0; i <= src.str_count; i++)
    {
        cur_com = GetCommand(src.str_pointers[i].beg);
        printf("current command[%3zu] = %d;\n", i+1, cur_com);
        fprintf(code_out, "%d", cur_com);

        if (cur_com == PUSH)
        {
            int argument = GetValue(src.str_pointers[i].beg);
            if (argument == UNREAD_VAL)
            {
                return UNREAD_VAL;
            }
            fprintf(code_out, " %d", argument);
            PR("* Get value = %d;\n", argument);
        }
        fprintf(code_out, "\n");
    }



    return 0;
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