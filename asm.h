#include "/Applications/VsСode/Onegin/onegin.h"

// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------


const size_t MAX_COM_LEN = 10;

const int    UNREAD_VAL  = -27394383;

const int   LABELS_COUNT = 20;

struct File_names
{
    const char * const src;
    const char *t_out;
    const char *b_out;
};

// FUNCTIONS
//-------------------------------------------------------------------------------------------------------------------------------------------------

size_t StrLen(const char *str);

int GetCommandV1(const char *str);

int GetValue(const char *str);

Instruction GetCommand(const char *command);

bool FindLabel(char *str_beg, int *label, int *labels, int cur_com_num, size_t string_num);

int AsmReadWrite(File *src, File_names fname, int repeat_count);

bool FindJmp(char *str_beg, int *labels, int *cur_com_num, Instruction *cur_com, FILE *txt_out, int bin_out);

bool FindInst(int arg1, int count_read_el, int *cur_com_num, Instruction *cur_com, FILE *txt_out, int bin_out);

//-------------------------------------------------------------------------------------------------------------------------------------------------