#ifndef _vm
#define _vm

typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);
#define SCANF_ELEM_T(x) scanf("%lg", &x);

#define _PRT 1 //   zero is off \ other is on
#define CRUSH_BIN NAN

// #define _DEBUG
#include "instructions.h"
#include "Stack/stack.h"

#include     <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include      <time.h>
#include      <math.h>
#include   <stdbool.h>
#include    <assert.h>
#include  <sys/stat.h>
#include    <unistd.h>
#include     <fcntl.h>
#include <sys/types.h>
#include     <ctype.h>
#include    <stdarg.h>




// ENUMs and STRUCTs
//-------------------------------------------------------------------------------------------------------------------------------------------------

const size_t REG_COUNT = 4;
const int SC_WEIGHT = 60;
const int SC_HEIGHT = 30;
const size_t MEM_SIZE  = SC_WEIGHT * SC_HEIGHT;

struct Processor
{
    char *code;
    size_t code_size;
    Stack_t stack;
    Stack_t ret_address;
    size_t pc;
    stack_elem reg[REG_COUNT];
    stack_elem ram[MEM_SIZE];
};

// struct File_names
// {
//     const char * const src;
//     const char * const t_out;
//     const char * const b_out;
// };


// FUNCTOINS
//-------------------------------------------------------------------------------------------------------------------------------------------------

ssize_t SizeofFile(const char *file_name);

void PR_PROC (Processor *proc);

void ClearTerm();

int CompInstUp(const void *a, const void *b);

bool IsEqual(double arg1, double arg2);

int CopyInstructions(Instruction *Insts);

int ReadBinFile(const char *filename, char **buffer_p, size_t *sizeof_buffer);

int RunBin(Processor proc);

void PrintErrorCode(int err_code);


// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

const size_t BUF_SIZE = 1000; // FIXME

const double EPSILON = 0.001;

const int MAX_INSTRUCTION_CALLS = 500;

const size_t COM_LEN = 1;
const size_t ARG_LEN = 4;

#endif


