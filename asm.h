#include "vm.h"
#include "/Applications/VsСode/Onegin/onegin.h"

// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

struct InstHash
{
    uint hash;
    Instruction inst;
};

const size_t MAX_COM_LEN = 10;

const int    UNREAD_VAL  = -27394383;

const int   LABELS_COUNT = 20;

// FUNCTIONS
//-------------------------------------------------------------------------------------------------------------------------------------------------

size_t StrLen(const char *str);

void DisplayBin(size_t cur_index, char *code);

void ClearTerm();

int GetCommandV1(const char *str);

int GetValue(const char *str);

Instruction GetCommand(const char *str_beg, InstHash *hash_tab);

InstHash* InitHashT(InstHash *hash_tab);

int GetArgs(char *str_beg, int *labels, int *arg1, char *com_num);

bool FindLabel(char *str_beg, int *label, int *labels, int cur_com_num, size_t string_num);

int AsmReadWrite(File *src, File_names fname, int *labels, InstHash *hash_tab);

bool FindJmp(char *str_beg, int *labels, int *cur_com_num, Instruction *cur_com, FILE *txt_out, int bin_out);

bool FindInst(int arg1, int count_read_el, int *cur_com_num, Instruction *cur_com, FILE *txt_out, int bin_out);

uint HashCom(const char *com_name);

int CompUInt(const void *a, const void *b);

//-------------------------------------------------------------------------------------------------------------------------------------------------